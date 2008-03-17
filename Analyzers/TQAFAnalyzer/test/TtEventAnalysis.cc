// -*- C++ -*-
//
// Package:    TtEventAnalysis
// Class:      TtEventAnalysis
// 
/**\class TtEventAnalysis TtEventAnalysis.cc TopQuarkAnalysis/TtEventAnalysis/src/TtEventAnalysis.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// $Id: TtEventAnalysis.cc,v 1.1 2008/02/05 21:10:23 yumiceva Exp $
//
//


// system include files
#include <memory>

// user include files
#include "TtEventAnalysis.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "PhysicsTools/Utilities/interface/DeltaR.h"
#include "Math/GenVector/VectorUtil.h"

#include "MEzCalculator.h"

#include "TLorentzVector.h"

#include "CSA07EffAnalyser/CSA07EffAnalyser/interface/CSA07ProcessId.h"

//
// constructors and destructor
//
TtEventAnalysis::TtEventAnalysis(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  debug             = iConfig.getUntrackedParameter<bool>   ("debug");
  fwriteAscii       = iConfig.getUntrackedParameter<bool>   ("writeAscii");
  fasciiFileName    = iConfig.getUntrackedParameter<std::string> ("asciiFileName");
  fApplyWeights     = iConfig.getUntrackedParameter<bool>   ("applyWeights");
  rootFileName      = iConfig.getUntrackedParameter<std::string> ("rootFileName");
  genEvnSrc         = iConfig.getParameter<edm::InputTag> ("genEventSource");
  leptonFlavour     = iConfig.getParameter<std::string>   ("leptonFlavour"); 
  muonSrc           = iConfig.getParameter<edm::InputTag> ("muonSource");
  electronSrc       = iConfig.getParameter<edm::InputTag> ("electronSource");
  metSrc            = iConfig.getParameter<edm::InputTag> ("metSource");
  bjetSrc           = iConfig.getParameter<edm::InputTag> ("bjetSource");
  ljetSrc           = iConfig.getParameter<edm::InputTag> ("ljetSource");
  //evtsols           = iConfig.getParameter<edm::InputTag> ("EvtSolution");
  
  ffilter           = iConfig.getUntrackedParameter<bool>   ("GenFilter");

  feventToProcess   = iConfig.getUntrackedParameter<int> ("event");
  fdisplayJets      = iConfig.getUntrackedParameter<bool>   ("displayJets");


  // write ascii output
  if (fwriteAscii) {
    std::cout << " Results will be also saved in ascii file: " << fasciiFileName << std::endl;
    fasciiFile.open(fasciiFileName.c_str());
  }

  // Create the root file
  theFile = new TFile(rootFileName.c_str(), "RECREATE");
  theFile->mkdir("Generator");
  theFile->cd();
  theFile->mkdir("Muons");
  theFile->cd();
  theFile->mkdir("MET");
  theFile->cd();
  theFile->mkdir("Jets");
  theFile->cd();
  theFile->mkdir("Mass");
  if (fdisplayJets) {
	  theFile->cd();
	  theFile->mkdir("DisplayJets");
  }
  theFile->cd();
  //theFile->mkdir("Other");
  //theFile->cd();

  //h_Jet   = new HTOP1("Jets_");
  //h_Mass  = new HTOP2("Mass_");
  //h_Other = new HTOP3("Other_");
  
  // initialize cuts
  cut_map["cut0"] = "Initial selection";
  //cut_map["cut1"] = "Min Muon pt";
  //cut_map["cut2"] = "Min Jet Et";
  //cut_map["cut3"] = "Min MET";
  //cut_map["cut4"] = "";

  // initialize histograms
  hmuons_ = new TQAFHistograms();
  hmet_   = new TQAFHistograms();
  hjets_  = new TQAFHistograms();
  hgen_   = new TQAFHistograms();
  hmass_  = new TQAFHistograms();
  if (fdisplayJets) hdisp_  = new TQAFHistograms();
  
  for (std::map<TString, TString>::const_iterator imap=cut_map.begin();
	   imap!=cut_map.end(); ++imap) {
	  
	  TString acut = imap->first;
	  
	  hjets_->Init("Jets",acut);
	  //h_->Init("jets",acut,"MC");
	  hmuons_->Init("Muons",acut);
	  //h_->Init("muons",acut,"MC");
	  hmet_->Init("MET",acut);
	  //h_->Init("MET",acut,"MC");
	  //hmass_->Init("Mass",acut);
	  
	  //h_->Init("others",acut,"MC");
  }
  hgen_->Init("generator");

  hmass_->Init("Mass","cut1");
  hmass_->Init("Mass","cut2");

  hjets_->Init("Jets","cut1");
   
  if (fdisplayJets) hdisp_->Init("DisplayJets","cut0");
  
  nevents = 0;
  

}


TtEventAnalysis::~TtEventAnalysis()
{

	std::cout << "[TtEventAnalysis] Total events analyzed = " << nevents << std::endl;
	
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
   if (debug) std::cout << "[TtEventAnalysis] Destructor called" << std::endl;
   
   if (fwriteAscii) fasciiFile.close();

   theFile->cd();
   theFile->cd("Muons");
   hmuons_->Save();
   
   theFile->cd();
   theFile->cd("MET");
   hmet_->Save();

   theFile->cd();
   theFile->cd("Jets");
   hjets_->Save();
   
   theFile->cd();
   theFile->cd("Generator");
   hgen_->Save();

   theFile->cd();
   theFile->cd("Mass");
   hmass_->Save();

   if (fdisplayJets) {
	   theFile->cd();
	   theFile->cd("DisplayJets");
	   hdisp_->Save();
   }
   
   //Release the memory
   //delete h_Jet;
   //delete h_Mass;
   //delete h_Other;

   //delete h_;
   
   //Close the Root file
   theFile->Close();
   if (debug) std::cout << "************* Finished writing histograms to file" << std::endl;

}

void
TtEventAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{	

  double weight = 1.;

  if (fApplyWeights) {

    edm::Handle< double> weightHandle;
    iEvent.getByLabel ("csaweightproducer","weight", weightHandle);
    weight = * weightHandle;

  }



	//check if we process one event
	bool processthis = false;
	
	if (feventToProcess>-1 && nevents==feventToProcess) processthis = true;
	else if (feventToProcess==-1) processthis = true;

	if (processthis) {
   // retrieve gen objects	
   edm::Handle<TtGenEvent > genEvent;
   iEvent.getByLabel(genEvnSrc, genEvent);

   if (debug) {
     //std::cout << "GenEvent = " << genEvent << std::endl;
     TtGenEvent gevent=*genEvent;
     std::cout << "Flag= " << gevent.isSemiLeptonic() << std::endl;
     if(gevent.lepton())  std::cout << "LeptonId = " << gevent.lepton()->pdgId() << std::endl;
     if (gevent.leptonBar()) std::cout << "LeptonBarId = " << gevent.leptonBar()->pdgId() << std::endl;
   }

   // retrieve the reco-objects
   edm::Handle< std::vector<pat::Muon> > muonHandle;
   iEvent.getByLabel(muonSrc, muonHandle);

   std::vector<pat::Muon> muons = *muonHandle;
   if (debug) std::cout << "got muons" << std::endl;
//   edm::Handle< std::vector<TopElectron> > electrons;
//   iEvent.getByLabel(electronSrc, electrons);

   edm::Handle< std::vector<pat::MET> > metHandle;
   iEvent.getByLabel(metSrc, metHandle);

   std::vector<pat::MET> met = *metHandle;
   if (debug) std::cout << "got met" << std::endl;
//   edm::Handle< std::vector<TopJet> > bjets;
//   iEvent.getByLabel(bjetSrc, bjets);

   edm::Handle< std::vector<pat::Jet> > jetHandle;
   iEvent.getByLabel(ljetSrc, jetHandle);
   std::vector<pat::Jet> jets = *jetHandle;
   if (debug) std::cout << "got jets" << std::endl;
   
   // get the event solution
   /*
   edm::Handle< std::vector<TtSemiEvtSolution> > eSols; 
   iEvent.getByLabel(evtsols, eSols);

   std::vector<TtSemiEvtSolution> sols;
   sols = *eSols;
   if (debug) std::cout << "got solutions" << std::endl;
   */
   // GENERATOR
   const reco::Candidate *genMuon   = genEvent->singleLepton();
   const reco::Candidate *genNu     = genEvent->singleNeutrino();
   const reco::Candidate *genTop    = genEvent->top();
   const reco::Candidate *genTopBar = genEvent->topBar();
   const reco::Candidate *genHadWp  = genEvent->hadronicDecayQuark();
   const reco::Candidate *genHadWq  = genEvent->hadronicDecayQuarkBar();
   const reco::Candidate *genHadb   = genEvent->hadronicDecayB();
   const reco::Candidate *genLepb   = genEvent->leptonicDecayB();

   if (debug) std::cout << "got generator candidates" << std::endl;

   if (genTop || genTopBar) {
	   LorentzVector gentoppairP4;
	   gentoppairP4 = genTop->p4() + genTopBar->p4();
	   hgen_->Fill1d("gen_toppair_mass", gentoppairP4.M() );
	   if (debug) std::cout << " gen top rapidities: t_y = " << genTop->y()
							<< " tbar_y = " << genTopBar->y() << std::endl;
	   if (genHadWp && genHadWq && genMuon && genHadb && genLepb && genNu) {
		   hgen_->Fill2d("gentop_rapidities", genTop->y(), genTopBar->y() );
		   //std::cout << " done gen 1" << std::endl;
		   hgen_->Fill1d("gen_nu_pz", genNu->p4().Pz() );
		   //std::cout << " done gen 2" << std::endl; 
		   hgen_->Fill1d("gen_deltaR_qb", DeltaR<reco::Candidate>()(*genHadWq, *genHadb ) );
     //std::cout << " done gen 3" << std::endl; 
		   hgen_->Fill1d("gen_deltaR_pb", DeltaR<reco::Candidate>()(*genHadWp, *genHadb ) );
		   hgen_->Fill1d("gen_deltaR_pq", DeltaR<reco::Candidate>()(*genHadWp, *genHadWq ) );
		   //std::cout << " done gen 4" << std::endl; 
		   hgen_->Fill1d("gen_deltaR_lb", DeltaR<reco::Candidate>()(*genMuon , *genLepb ) );
		   //std::cout << " done gen 5" << std::endl; 
		   hgen_->Fill1d("gen_deltaR_qLepb", DeltaR<reco::Candidate>()(*genHadWq , *genLepb ) );
		   //std::cout << " done gen 6" << std::endl; 
		   hgen_->Fill1d("gen_deltaR_qmu", DeltaR<reco::Candidate>()(*genHadWq , *genMuon ) );
	   } else {
		   std::cout << " No top decay generator info, what happen here?" << std::endl;
		   if (!genHadWp) std::cout << " no genHadWp" << std::endl;
		   if (!genHadWq) std::cout << " no genHadWq" << std::endl; 
		   if (!genHadb) std::cout << " no genHadb" << std::endl; 
		   if (!genLepb) std::cout << " no genLepb" << std::endl;
		   if (!genMuon) std::cout << " no genMuon" << std::endl; 
		   if (!genNu) std::cout << " no genNu" << std::endl;
	   }
	   if (debug) std::cout << "done gen histo" << std::endl;
   } else {
	   std::cout << "no ttbar in generator" << std::endl;
   }

   // MUONS
   bool found_goodmuon = false;
   int nmuons = muons.size();
   hmuons_->Fill1d(TString("muons")+"_cut0",nmuons, weight);
   for( size_t imu=0; imu != muons.size(); ++imu) {
	   
	   hmuons_->Fill1d(TString("muon_pt")+"_cut0", muons[imu].pt(), weight);
	   //hmuons_->Fill1d(TString("muon_normchi2")+"_"+"cut0",muons[imu].combinedMuon()->chi2()/muons[imu].combinedMuon()->ndof());
	   hmuons_->Fill1d(TString("muon_caloIso")+"_"+"cut0",muons[imu].caloIso(), weight);
	   hmuons_->Fill1d(TString("muon_trackIso")+"_"+"cut0",muons[imu].trackIso(), weight);  
	   //std::cout << "caloiso= " << muons[imu].getCaloIso() << std::endl;
	   //std::cout << "trkiso= " << muons[imu].getTrackIso() << std::endl;
	   found_goodmuon = true;
   }

   TLorentzVector muonP4;
   int muonCharge = 0;
   if ( found_goodmuon) {
	   muonP4.SetPxPyPzE(muons[0].px(),muons[0].py(),muons[0].pz(),muons[0].energy());
	   muonCharge = muons[0].charge();
   }
   
   if (debug) std::cout << "muons done" << std::endl;
   
   if ( found_goodmuon ) {
   // MET
   // met is corrected by muon momentum, how about muon energy?
   if (met.size() == 0 and debug ) std::cout << "MET size collection is zero!" << std::endl; 
   for( size_t imet=0; imet != met.size(); ++imet) {
	   hmet_->Fill1d(TString("MET")+"_"+"cut0", met[imet].et(), weight );
	   hmet_->Fill1d(TString("MET_deltaR_muon")+"_"+"cut0", DeltaR<reco::Candidate>()( met[imet] , muons[0] ), weight  );
   }
   if (debug) std::cout << "MET done" << std::endl;
   
   // JETS
   TLorentzVector jetP4[4];
   
   hjets_->Fill1d(TString("jets")+"_"+"cut0", jets.size(), weight );
   for( size_t ijet=0; ijet != jets.size(); ++ijet) {

	   TLorentzVector tmpP4;
	   tmpP4.SetPxPyPzE(jets[ijet].px(),jets[ijet].py(),jets[ijet].pz(),jets[ijet].energy());
	   
	   if(ijet==0) {
		   if (debug) std::cout << "leading jet et: " << jets[ijet].et() << std::endl;
		   hjets_->Fill1d(TString("jet0_et")+"_"+"cut0", jets[ijet].et(), weight );
		   hjets_->Fill1d(TString("jet0_eta")+"_"+"cut0", jets[ijet].eta(), weight );
		   jetP4[ijet] = tmpP4;
		   // display jets//
		   if (fdisplayJets) {
			   std::vector<CaloTowerRef> jetCaloRefs = jets[ijet].getConstituents();
			   if (debug) std::cout << " jetCaloRefs size = " << jetCaloRefs.size() << std::endl;
			   for ( size_t icalo=0; icalo != jetCaloRefs.size(); ++icalo ) {
				   if (debug) std::cout << " got icalo: " << "energy: "<< jetCaloRefs[icalo]->energy() << " eta: " << jetCaloRefs[icalo]->eta() <<  std::endl;
				   //if ( jetCaloRefs[icalo]->eta() < 1.740 )
				   hdisp_->Fill2d(TString("jet0_calotowerI")+"_cut0", jetCaloRefs[icalo]->eta(), jetCaloRefs[icalo]->phi(), jetCaloRefs[icalo]->energy() );
				   //else
				   //hjets_->Fill2d(TString("jet0_calotowerII")+"_cut0", jetCaloRefs[icalo]->eta(), jetCaloRefs[icalo]->phi(), jetCaloRefs[icalo]->energy() );				
			   }
		   }
	   }
	   if(ijet==1) {
		   hjets_->Fill1d(TString("jet1_et")+"_"+"cut0", jets[ijet].et() );
		   jetP4[ijet] = tmpP4;
		   // display jets//
		   if (fdisplayJets) {
			   std::vector<CaloTowerRef> jetCaloRefs = jets[ijet].getConstituents();
			   for ( size_t icalo=0; icalo != jetCaloRefs.size(); ++icalo ) {
				   hdisp_->Fill2d(TString("jet1_calotowerI")+"_cut0", jetCaloRefs[icalo]->eta(), jetCaloRefs[icalo]->phi(), jetCaloRefs[icalo]->energy() );
			   }
		   }
	   }
	   if(ijet==2) {
		   hjets_->Fill1d(TString("jet2_et")+"_"+"cut0", jets[ijet].et() );
		   jetP4[ijet] = tmpP4;
		   if (fdisplayJets) {
			   std::vector<CaloTowerRef> jetCaloRefs = jets[ijet].getConstituents();
			   for ( size_t icalo=0; icalo != jetCaloRefs.size(); ++icalo ) {
				   hdisp_->Fill2d(TString("jet2_calotowerI")+"_cut0", jetCaloRefs[icalo]->eta(), jetCaloRefs[icalo]->phi(), jetCaloRefs[icalo]->energy() );
			   }
		   }
	   }
	   if(ijet==3) {
		   hjets_->Fill1d(TString("jet3_et")+"_"+"cut0", jets[ijet].et() );
		   jetP4[ijet] = tmpP4;
		   if (fdisplayJets) {
			   std::vector<CaloTowerRef> jetCaloRefs = jets[ijet].getConstituents();
			   for ( size_t icalo=0; icalo != jetCaloRefs.size(); ++icalo ) {
				   hdisp_->Fill2d(TString("jet3_calotowerI")+"_cut0", jetCaloRefs[icalo]->eta(), jetCaloRefs[icalo]->phi(), jetCaloRefs[icalo]->energy() );
			   }
		   }
	   }

	   hjets_->Fill1d(TString("jet_et")+"_"+"cut0", jets[ijet].et(), weight );
	   hjets_->Fill1d(TString("jet_eta")+"_"+"cut0", jets[ijet].eta(), weight );
	   hjets_->Fill1d(TString("jet_phi")+"_"+"cut0", jets[ijet].phi(), weight );

	   if (ijet<4)
		   hjets_->Fill1d(TString("jet_deltaR_muon")+"_cut0",ROOT::Math::VectorUtil::DeltaR( tmpP4.Vect(), muonP4.Vect() ), weight );
	   
	   
   }
   if (debug) std::cout << "got leading jets" << std::endl;
   
   // find Mega jet opposite to muon
   TLorentzVector mjetP4;
   int nmerged = 0;
   int tmp_mjet = -1;
   for ( int ij=0; ij<4; ++ij) {

	   double deltaR = ROOT::Math::VectorUtil::DeltaR( jetP4[ij].Vect(), muonP4.Vect() );
	   double maxdeltaR = 0;
	   if (debug) std::cout << " deltaR(jet,muon) = " << deltaR << std::endl;
	   if ( deltaR > 2.5 ) {
		   maxdeltaR = deltaR;
		   tmp_mjet = ij;
		   break;
	   }
   }
   bool found_Mjet = false;
   // merge jets into mega jet
   std::vector<int> listofmergedjets;
   if ( tmp_mjet != -1 ) {

	   mjetP4 = jetP4[tmp_mjet];
	   for( size_t ijet=0; ijet != jets.size(); ++ijet) {

		   TLorentzVector tmpP4;
		   tmpP4.SetPxPyPzE(jets[ijet].px(),jets[ijet].py(),jets[ijet].pz(),jets[ijet].energy());
		   double deltaR = ROOT::Math::VectorUtil::DeltaR( tmpP4.Vect(), mjetP4.Vect() );
		   if ( ijet != tmp_mjet && deltaR < 1.6 ) {
			   
			   mjetP4 = mjetP4 + tmpP4;
			   listofmergedjets.push_back(ijet);
			   hjets_->Fill1d(TString("Mjet_et_subjets")+"_"+"cut0", tmpP4.Et(), weight );
			   nmerged++;
			   
		   }
	   }

	   found_Mjet = true;
   }
   if (debug) std::cout << "got Mjet?" << found_Mjet << " Et = " << mjetP4.Et() << " with listofmergedjets.size = " << listofmergedjets.size() << std::endl;
   
   if ( found_Mjet ) {
	   
	   hjets_->Fill1d(TString("Mjet_et")+"_"+"cut0", mjetP4.Et(), weight );
	   hjets_->Fill1d(TString("Mjet_mass")+"_"+"cut0", mjetP4.M(), weight );
	   hjets_->Fill1d(TString("Mjet_deltaR_mu")+"_"+"cut0",ROOT::Math::VectorUtil::DeltaR( mjetP4.Vect(), muonP4.Vect() ), weight );
	   hjets_->Fill1d(TString("Mjet_njets")+"_"+"cut0", nmerged, weight );
	   
	   // find b jet: another leading jet close to the muon
	   TLorentzVector bjetP4;
	   bool bfound = false;
	   for( size_t ijet=0; ijet < 4; ++ijet) {

		   if (debug) std::cout << " ijet = " << ijet << std::endl;
		   if (bfound) break;

		   if ( listofmergedjets.size() == 0 && ( (int)ijet)!= tmp_mjet) {

			   bjetP4 = jetP4[ijet];
			   if (debug) std::cout << " bjet ijet = " << ijet << std::endl;
			   bfound = true;
			   break;
		   }
		   else {
			   for ( size_t imerged = 0; imerged!= listofmergedjets.size(); ++imerged) {

				   if (debug) std::cout << "   merged jet list = " << listofmergedjets[imerged]  << std::endl;
			   
				   if ( ijet != listofmergedjets[imerged] && ( (int)ijet)!= tmp_mjet ) {
					   bjetP4 = jetP4[ijet];
					   if (debug) std::cout << " bjet ijet = " << ijet << std::endl;
					   bfound = true;
					   break;
				   }
			   }
		   }
	   }

	   hjets_->Fill1d(TString("Mjet_deltaR_b")+"_"+"cut0",ROOT::Math::VectorUtil::DeltaR( mjetP4.Vect(), bjetP4.Vect() ), weight );
	   hmuons_->Fill1d(TString("muon_deltaR_b")+"_cut0",ROOT::Math::VectorUtil::DeltaR(muonP4.Vect(), bjetP4.Vect()), weight);

	   // now find a good b candidate opposite to Mjet
	   bool found_LepbJet = false;
	   if ( ROOT::Math::VectorUtil::DeltaR( muonP4.Vect(), bjetP4.Vect() ) < 0.75 ) { 
	     // before was deltaR(mjet,bjet) > 1 ?
		   //hjets_->Fill1d(TString("Mjet_mass")+"_"+"cut1", mjetP4.M() );
		   //hjets_->Fill1d(TString("Mjet_et")+"_"+"cut1", mjetP4.Et() );
		   found_LepbJet = true;
	   }
	   
	   // Solving for neutrino Pz from W->mu+nu
	   double neutrinoPz = -999999.;
	   TLorentzVector nuP4;
	   bool found_nu = false;
	   bool found_goodMET = false;
	   if ( met.size()>0 && muons.size()>0 ) {
		   MEzCalculator zcalculator;
		   zcalculator.SetMET( met[0] );
		   zcalculator.SetMuon( muons[0] );
		   neutrinoPz = zcalculator.Calculate();
		   
		   nuP4.SetPxPyPzE(met[0].px(), met[0].py(), neutrinoPz,
						   sqrt(met[0].px()*met[0].px()+met[1].py()*met[1].py()+neutrinoPz*neutrinoPz) );
		   
		   hmuons_->Fill1d(TString("muon_deltaR_nu")+"_cut0",ROOT::Math::VectorUtil::DeltaR( muonP4.Vect(), nuP4.Vect() ), weight );
		   hmuons_->Fill1d(TString("muon_deltaPhi_nu")+"_cut0",ROOT::Math::VectorUtil::DeltaPhi( muonP4.Vect(), nuP4.Vect() ), weight );
		   hmet_->Fill1d(TString("nu_pz")+"_cut0",neutrinoPz, weight);
		   
		   found_nu = true;
		   
		   if ( ROOT::Math::VectorUtil::DeltaPhi( muonP4.Vect(), nuP4.Vect() ) < 0.75 ) found_goodMET = true;
	   }
	   if (debug) std::cout << "got neutrino? " << found_nu << std::endl;
	   if (debug) std::cout << "got good MET? " << found_goodMET << std::endl;
	   
	   
	   if ( found_LepbJet && found_nu && found_goodMET ) {
		   
		   TLorentzVector LepTopP4;
		   LepTopP4 = muonP4 + nuP4 + bjetP4;
		   
		   hjets_->Fill1d(TString("bjet_mass")+"_cut1", LepTopP4.M(), weight );
		   hjets_->Fill1d(TString("bjet_et")+"_cut1", LepTopP4.Et(), weight );
		   hjets_->Fill1d(TString("Mjet_mass")+"_cut1", mjetP4.M(), weight );
		   hjets_->Fill1d(TString("Mjet_et")+"_cut1", mjetP4.Et(), weight );
		   
		   TLorentzVector topPairP4 = mjetP4 + LepTopP4;
		   TLorentzVector topPairP4CM;
		   topPairP4CM.Boost(topPairP4.Px(),topPairP4.Py(),topPairP4.Pz());
		   
		   hjets_->Fill1d(TString("topPair_mass")+"_cut1", topPairP4.M(), weight );
		   //hjets_->Fill1d(TString("topPair_et")+"_"+"cut1", topPairP4.Et() );

		   double CosThetaCM = 0;
		   
		   // top quark is leptonic jet
		   if (muonCharge == 1 ) {
			   TLorentzVector LepTopP4CM;
			   LepTopP4CM.Boost(topPairP4.Px(),topPairP4.Py(),topPairP4.Pz());
			   CosThetaCM = LepTopP4CM.Angle(topPairP4.Vect());
		   }
		   // anti-top quark is leptonic jet
		   else if (muonCharge == -1 ) {
			   TLorentzVector mjetP4CM;
			   mjetP4CM.Boost(topPairP4.Px(),topPairP4.Py(),topPairP4.Pz());
			   CosThetaCM = mjetP4CM.Angle(mjetP4CM.Vect());
		   }			   
		   
		   if (debug) std::cout << "got all, and top pair." << std::endl;

		   if (fwriteAscii) {
		     // dump Muon
		     fasciiFile << "-15 " << muonP4.Px() <<" "<< muonP4.Py() <<" "<< muonP4.Pz() << std::endl;
		     // dump Neutrino
		     fasciiFile << "-5 " << nuP4.Px() <<" "<< nuP4.Py() <<" "<< nuP4.Pz() << std::endl;
		     // dump jets
		     for( size_t ijet=0; ijet != jets.size(); ++ijet) {

		       fasciiFile << jets[ijet].energy() <<" "<< jets[ijet].px() <<" "<<jets[ijet].py()<<" "<<jets[ijet].pz()<< std::endl;
		       
		     }
		   }
	   }

   }
	
   // SOLUTONS
   // are jets treated massless?
   TLorentzVector recjetP4[4];
   TLorentzVector recHadtP4;
   TLorentzVector recLeptP4;
   TLorentzVector recHadWP4;
   TLorentzVector recLepWP4;
   TLorentzVector rectPairP4;

   TLorentzVector fitjetP4[4];
   TLorentzVector fitHadtP4;
   TLorentzVector fitLeptP4;
   TLorentzVector fittPairP4;
  
   //if (debug) std::cout << "sol.size = " << sols.size() << std::endl;
	
   /*
   for( size_t isol=0; isol != sols.size(); ++isol) {
	   
	   // all solutions
	   recjetP4[0].SetPtEtaPhiE(sols[isol].getCalHadp().pt(),
								sols[isol].getCalHadp().eta(),
								sols[isol].getCalHadp().phi(),
								sols[isol].getCalHadp().energy());
	   recjetP4[1].SetPtEtaPhiE(sols[isol].getCalHadq().pt(),
								sols[isol].getCalHadq().eta(),
								sols[isol].getCalHadq().phi(),
								sols[isol].getCalHadq().energy());
	   recjetP4[2].SetPtEtaPhiE(sols[isol].getCalHadb().pt(),
								sols[isol].getCalHadb().eta(),
								sols[isol].getCalHadb().phi(),
								sols[isol].getCalHadb().energy());
	   recjetP4[3].SetPtEtaPhiE(sols[isol].getCalLepb().pt(),
								sols[isol].getCalLepb().eta(),
								sols[isol].getCalLepb().phi(),
								sols[isol].getCalLepb().energy());
	   recHadtP4.SetPtEtaPhiE(sols[isol].getCalHadt().pt(),
								 sols[isol].getCalHadt().eta(),
								 sols[isol].getCalHadt().phi(),
								 sols[isol].getCalHadt().energy());
	   recLeptP4.SetPtEtaPhiE(sols[isol].getCalLept().pt(),
								 sols[isol].getCalLept().eta(),
								 sols[isol].getCalLept().phi(),
								 sols[isol].getCalLept().energy());
	   
	   rectPairP4 = recHadtP4 + recLeptP4;

	   fitHadtP4.SetPtEtaPhiE(sols[isol].getFitHadt().pt(),
								 sols[isol].getFitHadt().eta(),
								 sols[isol].getFitHadt().phi(),
								 sols[isol].getFitHadt().energy());
	   fitLeptP4.SetPtEtaPhiE(sols[isol].getFitLept().pt(),
								 sols[isol].getFitLept().eta(),
								 sols[isol].getFitLept().phi(),
								 sols[isol].getFitLept().energy());
	   
	   fittPairP4 = fitHadtP4 + fitLeptP4;

	   //hmass_->Fill1d(TString("nu_pz")+"_"+"cut0", sols[isol].getNeutrinoPz());
	   
	   hmass_->Fill1d(TString("WTolnu")+"_"+"cut0", sols[isol].getCalLepW().mass());
	   hmass_->Fill1d(TString("tToWlnuj")+"_"+"cut0", sols[isol].getCalLept().mass());
	   
	   hmass_->Fill1d(TString("WTojj")+"_"+"cut0", sols[isol].getCalHadW().mass());
	   hmass_->Fill1d(TString("tTojjj")+"_"+"cut0", sols[isol].getCalHadt().mass());
	   	   
	   hmass_->Fill1d(TString("kinfit_probchi2")+"_"+"cut0", sols[isol].getProbChi2() );
	   hmass_->Fill1d(TString("topPair")+"_"+"cut0", rectPairP4.M() );
	   hmass_->Fill1d(TString("fittopPair")+"_"+"cut0", fittPairP4.M() );
	   hmass_->Fill1d(TString("topPairRes")+"_"+"cut0", (rectPairP4.M() - gentoppairP4.M())/gentoppairP4.M() );
	   hmass_->Fill1d(TString("fittopPairRes")+"_"+"cut0", (fittPairP4.M() - gentoppairP4.M())/gentoppairP4.M() );

	   if (sols[isol].getProbChi2() > 0 ) {
		   hmass_->Fill1d(TString("EtpPull")+"_"+"cut0",(sols[isol].getFitHadp().et() - sols[isol].getCalHadp().et() )/sqrt(sols[isol].getFitHadp().getCovM()[0]) );
		   hmass_->Fill1d(TString("fitEtpPull")+"_"+"cut0",(sols[isol].getFitHadp().et() - sols[isol].getGenHadp()->et() )/sqrt(sols[isol].getFitHadp().getCovM()[0]) );
		   hmass_->Fill1d(TString("EtlbPull")+"_"+"cut0",(sols[isol].getFitLepb().et() - sols[isol].getCalLepb().et() )/sqrt(sols[isol].getFitLepb().getCovM()[0]) );
		   hmass_->Fill1d(TString("fitEtlbPull")+"_"+"cut0",(sols[isol].getFitLepb().et() - sols[isol].getGenLepb()->et() )/sqrt(sols[isol].getFitLepb().getCovM()[0]) );
	   }
   }

   if (sols.size() > 0) {

	   int isol = sols[0].getMCBestJetComb();

	   if (isol >= 0 ) {
	   
	   recjetP4[0].SetPtEtaPhiE(sols[isol].getCalHadp().pt(),
								sols[isol].getCalHadp().eta(),
								sols[isol].getCalHadp().phi(),
								sols[isol].getCalHadp().energy());
	   recjetP4[1].SetPtEtaPhiE(sols[isol].getCalHadq().pt(),
								sols[isol].getCalHadq().eta(),
								sols[isol].getCalHadq().phi(),
								sols[isol].getCalHadq().energy());
	   recjetP4[2].SetPtEtaPhiE(sols[isol].getCalHadb().pt(),
								sols[isol].getCalHadb().eta(),
								sols[isol].getCalHadb().phi(),
								sols[isol].getCalHadb().energy());
	   recjetP4[3].SetPtEtaPhiE(sols[isol].getCalLepb().pt(),
								sols[isol].getCalLepb().eta(),
								sols[isol].getCalLepb().phi(),
								sols[isol].getCalLepb().energy());
	   recHadtP4.SetPtEtaPhiE(sols[isol].getCalHadt().pt(),
								 sols[isol].getCalHadt().eta(),
								 sols[isol].getCalHadt().phi(),
								 sols[isol].getCalHadt().energy());
	   recLeptP4.SetPtEtaPhiE(sols[isol].getCalLept().pt(),
								 sols[isol].getCalLept().eta(),
								 sols[isol].getCalLept().phi(),
								 sols[isol].getCalLept().energy());
	   
	   rectPairP4 = recHadtP4 + recLeptP4;

	   fitHadtP4.SetPtEtaPhiE(sols[isol].getFitHadt().pt(),
								 sols[isol].getFitHadt().eta(),
								 sols[isol].getFitHadt().phi(),
								 sols[isol].getFitHadt().energy());
	   fitLeptP4.SetPtEtaPhiE(sols[isol].getFitLept().pt(),
								 sols[isol].getFitLept().eta(),
								 sols[isol].getFitLept().phi(),
								 sols[isol].getFitLept().energy());
	   
	   fittPairP4 = fitHadtP4 + fitLeptP4;

	   //hmass_->Fill1d(TString("nu_pz")+"_"+"cut1", sols[isol].getNeutrinoPz());
	   
	   hmass_->Fill1d(TString("WTolnu")+"_"+"cut1", sols[isol].getCalLepW().mass());
	   hmass_->Fill1d(TString("tToWlnuj")+"_"+"cut1", sols[isol].getCalLept().mass());
	   
	   hmass_->Fill1d(TString("WTojj")+"_"+"cut1", sols[isol].getCalHadW().mass());
	   hmass_->Fill1d(TString("tTojjj")+"_"+"cut1", sols[isol].getCalHadt().mass());

	   hmass_->Fill1d(TString("kinfit_probchi2")+"_"+"cut1", sols[isol].getProbChi2() );
	   hmass_->Fill1d(TString("topPair")+"_"+"cut1", rectPairP4.M() );
	   hmass_->Fill1d(TString("fittopPair")+"_"+"cut1", fittPairP4.M() );
	   hmass_->Fill1d(TString("topPairRes")+"_"+"cut1", (rectPairP4.M() - gentoppairP4.M())/gentoppairP4.M() );
	   hmass_->Fill1d(TString("fittopPairRes")+"_"+"cut1", (fittPairP4.M() - gentoppairP4.M())/gentoppairP4.M() );

	   if (sols[isol].getProbChi2() > 0 ) {
		   hmass_->Fill1d(TString("EtpPull")+"_"+"cut1",(sols[isol].getFitHadp().et() - sols[isol].getCalHadp().et() )/sqrt(sols[isol].getFitHadp().getCovM()[0]) );
		   hmass_->Fill1d(TString("fitEtpPull")+"_"+"cut1",(sols[isol].getFitHadp().et() - sols[isol].getGenHadp()->et() )/sqrt(sols[isol].getFitHadp().getCovM()[0]) );
		   hmass_->Fill1d(TString("EtlbPull")+"_"+"cut1",(sols[isol].getFitLepb().et() - sols[isol].getCalLepb().et() )/sqrt(sols[isol].getFitHadp().getCovM()[0]) );
		   hmass_->Fill1d(TString("fitEtlbPull")+"_"+"cut1",(sols[isol].getFitLepb().et() - sols[isol].getGenLepb()->et() )/sqrt(sols[isol].getFitLepb().getCovM()[0]) );
	   }
	   }
	   //  
	   //
	   isol = sols[0].getSimpleBestJetComb();
	   if (isol >= 0 ) {
	   
	   recjetP4[0].SetPtEtaPhiE(sols[isol].getCalHadp().pt(),
								sols[isol].getCalHadp().eta(),
								sols[isol].getCalHadp().phi(),
								sols[isol].getCalHadp().energy());
	   recjetP4[1].SetPtEtaPhiE(sols[isol].getCalHadq().pt(),
								sols[isol].getCalHadq().eta(),
								sols[isol].getCalHadq().phi(),
								sols[isol].getCalHadq().energy());
	   recjetP4[2].SetPtEtaPhiE(sols[isol].getCalHadb().pt(),
								sols[isol].getCalHadb().eta(),
								sols[isol].getCalHadb().phi(),
								sols[isol].getCalHadb().energy());
	   recjetP4[3].SetPtEtaPhiE(sols[isol].getCalLepb().pt(),
								sols[isol].getCalLepb().eta(),
								sols[isol].getCalLepb().phi(),
								sols[isol].getCalLepb().energy());
	   recHadtP4.SetPtEtaPhiE(sols[isol].getCalHadt().pt(),
								 sols[isol].getCalHadt().eta(),
								 sols[isol].getCalHadt().phi(),
								 sols[isol].getCalHadt().energy());
	   recLeptP4.SetPtEtaPhiE(sols[isol].getCalLept().pt(),
								 sols[isol].getCalLept().eta(),
								 sols[isol].getCalLept().phi(),
								 sols[isol].getCalLept().energy());
	   
	   rectPairP4 = recHadtP4 + recLeptP4;

	   fitHadtP4.SetPtEtaPhiE(sols[isol].getFitHadt().pt(),
								 sols[isol].getFitHadt().eta(),
								 sols[isol].getFitHadt().phi(),
								 sols[isol].getFitHadt().energy());
	   fitLeptP4.SetPtEtaPhiE(sols[isol].getFitLept().pt(),
								 sols[isol].getFitLept().eta(),
								 sols[isol].getFitLept().phi(),
								 sols[isol].getFitLept().energy());
	   
	   fittPairP4 = fitHadtP4 + fitLeptP4;

	   //hmass_->Fill1d(TString("nu_pz")+"_"+"cut2", sols[isol].getNeutrinoPz());
	   
	   hmass_->Fill1d(TString("WTolnu")+"_"+"cut2", sols[isol].getCalLepW().mass());
	   hmass_->Fill1d(TString("tToWlnuj")+"_"+"cut2", sols[isol].getCalLept().mass());
	   
	   hmass_->Fill1d(TString("WTojj")+"_"+"cut2", sols[isol].getCalHadW().mass());
	   hmass_->Fill1d(TString("tTojjj")+"_"+"cut2", sols[isol].getCalHadt().mass());

	   hmass_->Fill1d(TString("kinfit_probchi2")+"_"+"cut2", sols[isol].getProbChi2() );
	   hmass_->Fill1d(TString("topPair")+"_"+"cut2", rectPairP4.M() );
	   hmass_->Fill1d(TString("fittopPair")+"_"+"cut2", fittPairP4.M() );
	   hmass_->Fill1d(TString("topPairRes")+"_"+"cut2", (rectPairP4.M() - gentoppairP4.M())/gentoppairP4.M() );
	   hmass_->Fill1d(TString("fittopPairRes")+"_"+"cut2", (fittPairP4.M() - gentoppairP4.M())/gentoppairP4.M() );

	   if (sols[isol].getProbChi2() > 0 ) {
		   hmass_->Fill1d(TString("EtpPull")+"_"+"cut2",(sols[isol].getFitHadp().et() - sols[isol].getCalHadp().et() )/sqrt(sols[isol].getFitHadp().getCovM()[0]) );
		   hmass_->Fill1d(TString("fitEtpPull")+"_"+"cut2",(sols[isol].getFitHadp().et() - sols[isol].getGenHadp()->et() )/sqrt(sols[isol].getFitHadp().getCovM()[0]) );
		   hmass_->Fill1d(TString("EtlbPull")+"_"+"cut2",(sols[isol].getFitLepb().et() - sols[isol].getCalLepb().et() )/sqrt(sols[isol].getFitLepb().getCovM()[0]) );
		   hmass_->Fill1d(TString("fitEtlbPull")+"_"+"cut2",(sols[isol].getFitLepb().et() - sols[isol].getGenLepb()->et() )/sqrt(sols[isol].getFitLepb().getCovM()[0]) );
	   }
	   }
	   
   }

   */
   
   /*
   if ( sols.size() > 0 ) 

   if ( (sols[0].getGenEvent()->isSemiLeptonic()     ) && 
        (sols[0].getGenEvent()->numberOfBQuarks()==2 ) ){ 

      int bestSol = sols[0].getMCBestJetComb();
      histo1 = h_Jet;
      histo3 = h_Other;

      jET_g[0] = ((sols[bestSol]).getGenHadp())->et();
      jET_g[1] = ((sols[bestSol]).getGenHadq())->et();
      jET_g[2] = ((sols[bestSol]).getGenHadb())->et();
      jET_g[3] = ((sols[bestSol]).getGenLepb())->et();
      jh_g[0]  = ((sols[bestSol]).getGenHadp())->eta();
      jh_g[1]  = ((sols[bestSol]).getGenHadq())->eta();
      jh_g[2]  = ((sols[bestSol]).getGenHadb())->eta();
      jh_g[3]  = ((sols[bestSol]).getGenLepb())->eta();

      jET_r[0] = ((sols[bestSol]).getRecHadp()).et();
      jET_r[1] = ((sols[bestSol]).getRecHadq()).et();
      jET_r[2] = ((sols[bestSol]).getRecHadb()).et();
      jET_r[3] = ((sols[bestSol]).getRecLepb()).et();
      jh_r[0]  = ((sols[bestSol]).getRecHadp()).eta();
      jh_r[1]  = ((sols[bestSol]).getRecHadq()).eta();
      jh_r[2]  = ((sols[bestSol]).getRecHadb()).eta();
      jh_r[3]  = ((sols[bestSol]).getRecLepb()).eta();

      jET_c[0] = ((sols[bestSol]).getCalHadp()).et();
      jET_c[1] = ((sols[bestSol]).getCalHadq()).et();
      jET_c[2] = ((sols[bestSol]).getCalHadb()).et();
      jET_c[3] = ((sols[bestSol]).getCalLepb()).et();
      jh_c[0]  = ((sols[bestSol]).getCalHadp()).eta();
      jh_c[1]  = ((sols[bestSol]).getCalHadq()).eta();
      jh_c[2]  = ((sols[bestSol]).getCalHadb()).eta();
      jh_c[3]  = ((sols[bestSol]).getCalLepb()).eta();

      for (int i=0; i<4; i++){
          histo1->Fill1a(jET_g[i],jh_g[i]);
          histo1->Fill1b(jET_r[i],jh_r[i]);
          histo1->Fill1c(jET_c[i],jh_c[i]);
      }

      if ( sols[bestSol].getProbChi2()>0 ) {
         jET_f[0] = ((sols[bestSol]).getFitHadp()).et();
         jET_f[1] = ((sols[bestSol]).getFitHadq()).et();
         jET_f[2] = ((sols[bestSol]).getFitHadb()).et();
         jET_f[3] = ((sols[bestSol]).getFitLepb()).et();
         jh_f[0]  = ((sols[bestSol]).getFitHadp()).eta();
         jh_f[1]  = ((sols[bestSol]).getFitHadq()).eta();
         jh_f[2]  = ((sols[bestSol]).getFitHadb()).eta();
         jh_f[3]  = ((sols[bestSol]).getFitLepb()).eta();
	 for (int i=0; i<4; i++){
             histo1->Fill1d(jET_f[i],jh_f[i]);
         }
         histo3->Fill3a( (sols[bestSol]).getProbChi2() );
      }
      histo3->Fill3b( (sols[bestSol]).getMCBestSumAngles() );

      mth[0] = ((sols[bestSol]).getRecHadt()).mass();
      mth[1] = ((sols[bestSol]).getCalHadt()).mass();
      mth[2] = ((sols[bestSol]).getFitHadt()).mass();

      mtl[0] = ((sols[bestSol]).getRecLept()).mass();
      mtl[1] = ((sols[bestSol]).getCalLept()).mass();
      mtl[2] = ((sols[bestSol]).getFitLept()).mass();

      mWh[0] = ((sols[bestSol]).getRecHadW()).mass();
      mWh[1] = ((sols[bestSol]).getCalHadW()).mass();
      mWh[2] = ((sols[bestSol]).getFitHadW()).mass();

      mWl[0] = ((sols[bestSol]).getRecLepW()).mass();
      mWl[1] = ((sols[bestSol]).getFitLepW()).mass();
      histo2 = h_Mass;
      histo2->Fill2(mth[0],mth[1],mth[2],mtl[0],mtl[1],mtl[2],mWh[0],mWh[1],mWh[2],mWl[0],mWl[1]);

      // Fill jet calibration check plots
      if( (sols[bestSol].getMCChangeWQ() == 0) || (sols[bestSol].getMCChangeWQ() == 1) ){
 
        histo1->Fill1e(jET_r[0],jET_r[0]/jET_g[0],jET_c[0]/jET_g[0]);
        histo1->Fill1e(jET_r[1],jET_r[1]/jET_g[1],jET_c[1]/jET_g[1]);

        if(sols[bestSol].getProbChi2()>0){
          histo1->Fill1f(jET_r[0],jET_f[0]/jET_g[0]);
          histo1->Fill1f(jET_r[1],jET_f[1]/jET_g[1]);
        }
      }

   }
   */
   
   /*
   for (std::vector<TopMuon>::const_iterator mu_i = muons->begin(); mu_i != muons->end(); mu_i++)
   {
       std::cout <<"mu eta = "<<(*mu_i).eta()<<" mother = "<<(*mu_i).mother()<<std::endl;
   }

   for (std::vector<TopElectron>::const_iterator e_i = electrons->begin(); e_i != electrons->end(); e_i++)
   {
       std::cout <<"e eta = "<<(*e_i).eta()<<" mother = "<<(*e_i).mother()<<std::endl;
   }

   for (std::vector<TopMET>::const_iterator met_i = met->begin(); met_i != met->end(); met_i++)
   {
       std::cout <<"met eta = "<<(*met_i).eta()<<" mother = "<<(*met_i).mother()<<std::endl;
   }

   for (std::vector<TopJet>::const_iterator jet_i = bjets->begin(); jet_i != bjets->end(); jet_i++)
   {
       std::cout <<"bjet eta = "<<(*jet_i).eta()<<" mother = "<<(*jet_i).mother()<<std::endl;
   }

   std::cout <<" ============================ " <<std::endl;
   */
   }
	}

   nevents++;
}


//define this as a plug-in
DEFINE_FWK_MODULE(TtEventAnalysis);
