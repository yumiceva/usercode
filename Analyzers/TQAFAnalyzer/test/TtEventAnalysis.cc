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
// $Id: TtEventAnalysis.cc,v 1.5 2008/06/02 20:16:01 yumiceva Exp $
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

#include "Analyzers/TQAFAnalyzer/test/MEzCalculator.h"

#include "TLorentzVector.h"

//#include "PhysicsTools/HepMCCandAlgos/interface/CSA07ProcessId.h"

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
  //  cut_map["cut1"] = "cut1";
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

  hmuons_->Init("Muons","cut1");

  hgen_->Init("generator");

  hmass_->Init("Mass","cut1");
  hmass_->Init("Mass","cut2");

  hjets_->Init("Jets","cut1");
  hmet_->Init("MET","cut1");

  if (fdisplayJets) hdisp_->Init("DisplayJets","cut0");
  
  nevents = 0;
  nbadmuons = 0;
  nWcomplex = 0;

}


TtEventAnalysis::~TtEventAnalysis()
{

	std::cout << "[TtEventAnalysis] Total events analyzed = " << nevents << std::endl;
	std::cout << "[TtEventAnalysis] Number of bad muon events = " << nbadmuons << std::endl;
	std::cout << "[TtEventAnalysis] Number of complex solutions = " << nWcomplex << std::endl;
	
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

double
TtEventAnalysis::dij(TLorentzVector p1, TLorentzVector p2, double mass, bool min) {

	TLorentzVector ptot = p1 + p2;
	Double_t theta1 = TMath::ACos( (p1.Vect().Dot(ptot.Vect()))/(p1.P()*ptot.P()) );
	Double_t theta2 = TMath::ACos( (p2.Vect().Dot(ptot.Vect()))/(p2.P()*ptot.P()) );
	double theta = theta1+theta2;
	double Emin2ij = p1.E() * p1.E();
	double Emax2ij = p2.E() * p2.E();
	if ( p1.E()>p2.E() ) { Emin2ij = p2.E() * p2.E(); Emax2ij = p1.E() * p1.E();}

	double result = TMath::Sin(theta/2)*TMath::Sin(theta/2)*Emin2ij/(mass*mass);
	if (!min) result = TMath::Sin(theta/2)*TMath::Sin(theta/2)*Emax2ij/(mass*mass);

	return result;
	
}


double
TtEventAnalysis::Psi(TLorentzVector p1, TLorentzVector p2, double mass) {

	TLorentzVector ptot = p1 + p2;
	Double_t theta1 = TMath::ACos( (p1.Vect().Dot(ptot.Vect()))/(p1.P()*ptot.P()) );
	Double_t theta2 = TMath::ACos( (p2.Vect().Dot(ptot.Vect()))/(p2.P()*ptot.P()) );
	Double_t min = 0;
	if (p1.Pt() > p2.Pt() ) min = TMath::Sqrt(TMath::Sqrt(p2.P()/p1.P()));
	else min = TMath::Sqrt(TMath::Sqrt(p1.P()/p2.P()));
	double th1th2 = theta1 + theta2;
	double psi = (p1.P()+p2.P())*TMath::Sin((th1th2)/2)*min/mass;	

/*
	TLorentzVector ptot = p1 + p2;
	Double_t theta1 = TMath::ACos( (p1.Vect().Dot(ptot.Vect()))/(p1.P()*ptot.P()) );
	Double_t theta2 = TMath::ACos( (p2.Vect().Dot(ptot.Vect()))/(p2.P()*ptot.P()) );
	//Double_t sign = 1.;
	//if ( (theta1+theta2) > (TMath::Pi()/2) ) sign = -1.;
	double th1th2 = theta1 + theta2;
	double psi = (p1.P()+p2.P())*TMath::Abs(TMath::Sin(th1th2))/(2.* mass );
	if ( th1th2 > (TMath::Pi()/2) )
		psi = (p1.P()+p2.P())*( 1. + TMath::Abs(TMath::Cos(th1th2)))/(2.* mass );

*/
	return psi;
}

double
TtEventAnalysis::PtRel(TLorentzVector p, TLorentzVector paxis) {

	TVector3 p3 = p.Vect();
	TVector3 p3axis = paxis.Vect();

	return p3.Perp(p3axis);

}

void
TtEventAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{	

  double weight = 1.;

  if (fApplyWeights) {

    edm::Handle< double> weightHandle;
    iEvent.getByLabel ("csa07EventWeightProducer","weight", weightHandle);
    weight = * weightHandle;

  }
  //std::cout << " weight = " << weight << std::endl;

	//check if we process one event
	bool processthis = false;

	if (debug) std::cout << " nevents = " << nevents << std::endl;
	
	if (feventToProcess>-1 && nevents==feventToProcess) processthis = true;
	else if (feventToProcess==-1) processthis = true;

	// retrieve gen objects	
	edm::Handle<TtGenEvent > genEvent;
	iEvent.getByLabel(genEvnSrc, genEvent);
	// filter muonic top decays at generator level if requested
	/*
	  if (fapplyGenFilter && processthis) {
		
		processthis = false;
		TtGenEvent gevent=*genEvent;
		if (gevent.isSemiLeptonic() ) {
			if (gevent.lepton()) {
				if (abs(gevent.lepton()->pdgId()) == 13 ) processthis = true;
			}
			if (gevent.leptonBar()) {
				if (abs(gevent.leptonBar()->pdgId()) == 13 ) processthis = true;
			}
		}
		if (debug && !processthis) std::cout << " This event has not contain a muonic top decay, skipping" << std::endl;
	}
	*/
	
	if (processthis) {
   
   if (debug) {
     //std::cout << "GenEvent = " << genEvent << std::endl;
     TtGenEvent gevent=*genEvent;
     std::cout << "generator, isSemiLeptonic = " << gevent.isSemiLeptonic() << std::endl;
     if(gevent.lepton())  std::cout << "LeptonId = " << gevent.lepton()->pdgId() << std::endl;
     if (gevent.leptonBar()) std::cout << "LeptonBarId = " << gevent.leptonBar()->pdgId() << std::endl;
   }

   //**** Retrieve Candidate COLLECTIONS ****//
   edm::Handle< std::vector<pat::Muon> > muonHandle;
   iEvent.getByLabel(muonSrc, muonHandle);

   std::vector<pat::Muon> muons = *muonHandle;
   if (debug) std::cout << "got muons" << std::endl;
   
   edm::Handle< std::vector<pat::MET> > metHandle;
   iEvent.getByLabel(metSrc, metHandle);

   std::vector<pat::MET> met = *metHandle;
   if (debug) std::cout << "got met" << std::endl;

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
   
   //**** GENERATOR ****//
   const reco::Candidate *genMuon   = genEvent->singleLepton();
   const reco::Candidate *genNu     = genEvent->singleNeutrino();
   const reco::Candidate *genTop    = genEvent->top();
   const reco::Candidate *genTopBar = genEvent->topBar();
   const reco::Candidate *genHadWp  = genEvent->hadronicDecayQuark();
   const reco::Candidate *genHadWq  = genEvent->hadronicDecayQuarkBar();
   const reco::Candidate *genHadb   = genEvent->hadronicDecayB();
   const reco::Candidate *genLepb   = genEvent->leptonicDecayB();

   if (debug) std::cout << "got generator candidates" << std::endl;
   double genNupz = 0;
   
   if (genTop || genTopBar) {
	   LorentzVector gentoppairP4;
	   gentoppairP4 = genTop->p4() + genTopBar->p4();
	   hgen_->Fill1d("gen_toppair_mass", gentoppairP4.M() );
	   hgen_->Fill1d("gen_toppair_pt", gentoppairP4.pt() );

	   hgen_->Fill1d("gen_top_pt", genTop->pt() );
	   hgen_->Fill1d("gen_top_eta", genTop->eta() );
	   if ( genTop->pt() > 50 ) hgen_->Fill1d("gen_top_eta1", genTop->eta() );
	   if ( genTop->pt() > 100 ) hgen_->Fill1d("gen_top_eta2", genTop->eta() );
	   if ( genTop->pt() > 150 ) hgen_->Fill1d("gen_top_eta3", genTop->eta() );
	   
	   
	   if (debug) std::cout << " gen top rapidities: t_y = " << genTop->y()
							<< " tbar_y = " << genTopBar->y() << std::endl;
	   if (genHadWp && genHadWq && genMuon && genHadb && genLepb && genNu) {
		   hgen_->Fill2d("gentop_rapidities", genTop->y(), genTopBar->y() );
		   genNupz = genNu->p4().Pz();
		   hgen_->Fill1d("gen_nu_pz", genNupz );
		   hgen_->Fill1d("gen_deltaR_qb", DeltaR<reco::Candidate>()(*genHadWq, *genHadb ) );
		   hgen_->Fill1d("gen_deltaR_pb", DeltaR<reco::Candidate>()(*genHadWp, *genHadb ) );
		   hgen_->Fill1d("gen_deltaR_pq", DeltaR<reco::Candidate>()(*genHadWp, *genHadWq ) );
		   hgen_->Fill1d("gen_deltaR_lb", DeltaR<reco::Candidate>()(*genMuon , *genLepb ) );
		   hgen_->Fill1d("gen_deltaR_qLepb", DeltaR<reco::Candidate>()(*genHadWq , *genLepb ) );
		   hgen_->Fill1d("gen_deltaR_qmu", DeltaR<reco::Candidate>()(*genHadWq , *genMuon ) );
		   hgen_->Fill1d("gen_deltaR_muLepb", DeltaR<reco::Candidate>()(*genMuon, *genLepb ) );

		   // i should fill it twice with p and q jets....
		   hgen_->Fill2d("gen_deltaR_pq_vs_toppt", DeltaR<reco::Candidate>()(*genHadWp, *genHadWq ), genEvent->hadronicDecayTop()->pt()  );
		   hgen_->Fill2d("gen_deltaR_qb_vs_toppt", DeltaR<reco::Candidate>()(*genHadWq, *genHadb ), genEvent->hadronicDecayTop()->pt()  );
		   hgen_->Fill2d("gen_deltaR_muLepb_vs_toppt", DeltaR<reco::Candidate>()(*genMuon, *genLepb ), genEvent->hadronicDecayTop()->pt()  );
		   
		   LorentzVector tmpgentop = genEvent->hadronicDecayTop()->p4();
		   LorentzVector tmpgenW = genEvent->hadronicDecayW()->p4();
		   LorentzVector tmpgenWp= genEvent->hadronicDecayQuark()->p4();
		   LorentzVector tmpgenWq= genEvent->hadronicDecayQuarkBar()->p4();
		   TVector3 genp1(tmpgenWp.Px(),tmpgenWp.Py(),tmpgenWp.Pz());
		   TVector3 genp2(tmpgenWq.Px(),tmpgenWq.Py(),tmpgenWq.Pz());
		   TVector3 genptot(tmpgenW.Px(),tmpgenW.Py(),tmpgenW.Pz());
		   Double_t tmptheta1 = TMath::ACos( (genp1.Dot(genptot))/(tmpgenWp.P()*tmpgenW.P()) );
		   Double_t tmptheta2 = TMath::ACos( (genp2.Dot(genptot))/(tmpgenWq.P()*tmpgenW.P()) );
		   hgen_->Fill2d("gen_toprapidity_vs_psi_pq",tmpgentop.Rapidity(), (tmpgenWp.P()+tmpgenWq.P())*TMath::Sin(tmptheta1+tmptheta2)/(2.*tmpgentop.M() ));
		   hgen_->Fill2d("gen_toprapidity_vs_deltaR_pq",tmpgentop.Rapidity(),DeltaR<reco::Candidate>()(*genHadWp, *genHadWq )  );
		   TLorentzVector tmpgenP4Wp = TLorentzVector(tmpgenWp.Px(),tmpgenWp.Py(),tmpgenWp.Pz(),tmpgenWp.E());
		   TLorentzVector tmpgenP4Wq = TLorentzVector(tmpgenWq.Px(),tmpgenWq.Py(),tmpgenWq.Pz(),tmpgenWq.E());
		   
		   hgen_->Fill2d("gen_toprapidity_vs_dminij_pq",tmpgentop.Rapidity(), dij(tmpgenP4Wp,tmpgenP4Wq,tmpgentop.M()) );
		   hgen_->Fill2d("gen_toprapidity_vs_dmaxij_pq",tmpgentop.Rapidity(), dij(tmpgenP4Wp,tmpgenP4Wq,tmpgentop.M(), false) );

		   double tmppL= (tmpgenW.Px()*tmpgentop.Px()+tmpgenW.Py()*tmpgentop.Py()+tmpgenW.Pz()*tmpgentop.Pz()) / tmpgentop.P();
		   double tmppT= TMath::Sqrt(tmpgenW.P()*tmpgenW.P() - tmppL*tmppL);
		   
		   hgen_->Fill2d("gen_HadW_pT_vs_pL", tmppT, tmppL );

		   LorentzVector tmpgenhadb = genHadb->p4();
		   tmppL= (tmpgenhadb.Px()*tmpgentop.Px()+tmpgenhadb.Py()*tmpgentop.Py()+tmpgenhadb.Pz()*tmpgentop.Pz()) / tmpgentop.P();
		   tmppT= TMath::Sqrt(tmpgenhadb.P()*tmpgenhadb.P() - tmppL*tmppL); 
		   
		   hgen_->Fill2d("gen_Hadb_pT_vs_pL", tmppT, tmppL );

		   LorentzVector tmpgenmu = genMuon->p4();
		   LorentzVector tmpgenLepW = genEvent->leptonicDecayW()->p4();
		   tmppL= (tmpgenmu.Px()*tmpgenLepW.Px()+tmpgenmu.Py()*tmpgenLepW.Py()+tmpgenmu.Pz()*tmpgenLepW.Pz()) / tmpgenLepW.P();
		   tmppT= TMath::Sqrt(tmpgenmu.P()*tmpgenmu.P() - tmppL*tmppL); 
		   
		   double tmpcosCM = TMath::Cos(TMath::ASin(2.*tmppT/tmpgenLepW.M()));

		   LorentzVector tmpgennu = genNu->p4();
		   hgen_->Fill2d("gen_cosCM_vs_psi",tmpcosCM,Psi(TLorentzVector(tmpgenmu.Px(),tmpgenmu.Py(),tmpgenmu.Pz(),tmpgenmu.E()),
														 TLorentzVector(tmpgennu.Px(),tmpgennu.Py(),tmpgennu.Pz(),tmpgennu.E()),
														 tmpgenLepW.M()));
										
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


   //**** Four Vectors ****
   TLorentzVector jetP4[4];
   TLorentzVector myMETP4;
   TLorentzVector METP4;
   TLorentzVector muonP4;
   TLorentzVector nuP4;
   TLorentzVector topPairP4;
   TLorentzVector hadTopP4;
   TLorentzVector lepTopP4;
   
   //**** JETS ****//
   //////////////////
   hjets_->Fill1d(TString("jets")+"_"+"cut0", jets.size(), weight );

   if (debug) std::cout << " number of jet = " << jets.size() << std::endl;
   
   for( size_t ijet=0; ijet != jets.size(); ++ijet) {

	   TLorentzVector tmpP4;
	   tmpP4.SetPxPyPzE(jets[ijet].px(),jets[ijet].py(),jets[ijet].pz(),jets[ijet].energy());

	   if (jets[ijet].pt() > 20. ) myMETP4 = myMETP4 + TLorentzVector(jets[ijet].px(),jets[ijet].py(),0,jets[ijet].pt());
	   
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
	   hjets_->Fill2d(TString("jet_ptVseta")+"_cut0", jets[ijet].et(),jets[ijet].eta(), weight);
	   
   }
   if (debug) std::cout << "got leading jets" << std::endl;

   
   
   // MUONS
   bool found_goodmuon = false;
   int nmuons = muons.size();
   hmuons_->Fill1d(TString("muons")+"_cut0",nmuons, weight);
   hmuons_->FillvsJets2d(TString("muons_vsJets")+"_cut0",nmuons, jets, weight );   
   int muonCharge = 0;
   for( size_t imu=0; imu != muons.size(); ++imu) {
	   
	   hmuons_->Fill1d(TString("muon_pt")+"_cut0", muons[imu].pt(), weight);
	   //hmuons_->Fill1d(TString("muon_normchi2")+"_"+"cut0",muons[imu].combinedMuon()->chi2()/muons[imu].combinedMuon()->ndof());
	   hmuons_->Fill1d(TString("muon_caloIso")+"_"+"cut0",muons[imu].caloIso(), weight);
	   hmuons_->Fill1d(TString("muon_trackIso")+"_"+"cut0",muons[imu].trackIso(), weight);  

	   //std::cout << "muon id: " << muons[imu].leptonID() << std::endl;
	   // let's do muon ID
	   bool passmu = false;
	   //Track muonTrk = *itMuon->track();
	   //int nhit = muonTrk.numberOfValidHits();
	   //double normChi2 = (*(muons[imu].combinedMuon())).chi2()/muons[imu].combineMuon()->ndof();
	   //if ( (nhit > 7) && (normChi2>5) ) passmu = true;

	   if ( muons[imu].pt() > 20 ) passmu = true;
	   // do I have a good muon?
	   //if ( ( TMath::Abs( muons[imu].leptonID() -1 ) < 1.e4 ) && !found_goodmuon ) {
	   if ( !found_goodmuon && passmu ) {
		   muonP4.SetPxPyPzE(muons[imu].px(),muons[imu].py(),muons[imu].pz(),muons[imu].energy());
		   muonCharge = muons[imu].charge();
		   found_goodmuon = true;

		   hmuons_->Fill1d(TString("muon_pt")+"_cut1", muons[imu].pt(), weight);
		   hmuons_->Fill1d(TString("muon_caloIso")+"_"+"cut1",muons[imu].caloIso(), weight);
		   hmuons_->Fill1d(TString("muon_trackIso")+"_"+"cut1",muons[imu].trackIso(), weight);
		   hmuons_->FillvsJets2d(TString("muons_vsJets")+"_cut1",nmuons, jets, weight );  

	   }
   }

   if (debug) std::cout << "muons done " << found_goodmuon << std::endl;
   
   // skip event if a good muon is not found
   if (! found_goodmuon ) {
	   nbadmuons++;
	   if (debug) std::cout << "not good muon found skipping" << std::endl;
	   return;
   }
	
   
   for( size_t ijet=0; ijet != 4; ++ijet ) {
	   TLorentzVector tmpP4;
	   tmpP4.SetPxPyPzE(jets[ijet].px(),jets[ijet].py(),jets[ijet].pz(),jets[ijet].energy());
	   hjets_->Fill1d(TString("jet_deltaR_muon")+"_cut0",ROOT::Math::VectorUtil::DeltaR( tmpP4.Vect(), muonP4.Vect() ), weight );
   }
   if (debug) std::cout << " jet_deltaR_muon histo filled" << std::endl;
      
   // plot my MET
   hmet_->Fill1d(TString("myMET")+"_cut0", myMETP4.Pt());
   // correct my MET
   myMETP4 = myMETP4 + TLorentzVector(muonP4.Px(),muonP4.Py(),0,muonP4.Pt());
   hmet_->Fill1d(TString("myMET")+"_cut1", myMETP4.Pt());
   // MET
   // met is corrected by muon momentum, how about muon energy?
   if (met.size() != 1 ) std::cout << "MET size collection is different from ONE! size="<< met.size() << std::endl;
   
   
   for( size_t imet=0; imet != met.size(); ++imet) {
	   hmet_->Fill1d(TString("MET")+"_"+"cut0", met[imet].et(), weight );
	   hmet_->Fill1d(TString("MET_eta")+"_"+"cut0", met[imet].eta(), weight );
	   hmet_->Fill1d(TString("MET_phi")+"_"+"cut0", met[imet].phi(), weight );
	   hmet_->Fill1d(TString("MET_deltaR_muon")+"_"+"cut0", DeltaR<reco::Candidate>()( met[imet] , muons[0] ), weight  );
	   hmet_->FillvsJets2d(TString("MET_vsJets")+"_cut0",met[imet].et(), jets, weight );
   }

   METP4.SetPxPyPzE(met[0].px(), met[0].py(), met[0].pz(), met[0].energy());
   myMETP4 = (-1)*myMETP4;
   
   if (debug) std::cout << "MET done" << std::endl;
   
   
   

   // Solving for neutrino Pz from W->mu+nu
   double neutrinoPz = -999999.;
   
   bool found_nu = false;
   bool found_goodMET = false;
  
   if ( met.size()>0 && muons.size()>0 ) {
	   MEzCalculator zcalculator;
	   // ok let's use myMET
	   zcalculator.SetMET( METP4 );
	   //zcalculator.SetMET(myMETP4);
	   zcalculator.SetMuon( muonP4 );//muons[0] );
	   if (debug) zcalculator.Print();
	   
	   neutrinoPz = zcalculator.Calculate(1);// 1 = closest to the lepton Pz, 3 = largest cosineCM
	   if (zcalculator.IsComplex()) nWcomplex += 1;
	   
	   if (debug) std::cout << " reconstructed neutrino Pz = " << neutrinoPz << std::endl;
	   nuP4.SetPxPyPzE(met[0].px(), met[0].py(), neutrinoPz,
	   		   sqrt(met[0].px()*met[0].px()+met[0].py()*met[0].py()+neutrinoPz*neutrinoPz) );
	   //nuP4 = myMETP4 + TLorentzVector(0,0,neutrinoPz,neutrinoPz);
	   
	   hmuons_->Fill1d(TString("muon_deltaR_nu")+"_cut0",ROOT::Math::VectorUtil::DeltaR( muonP4.Vect(), nuP4.Vect() ), weight );
	   hmuons_->Fill1d(TString("muon_deltaPhi_nu")+"_cut0",ROOT::Math::VectorUtil::DeltaPhi( muonP4.Vect(), nuP4.Vect() ), weight );
	   hmet_->Fill1d(TString("nu_pz")+"_cut0",neutrinoPz, weight);
	   hmet_->Fill1d(TString("nu_eta")+"_cut0",nuP4.Eta(), weight);
	   hmet_->Fill1d(TString("delta_nu_pz")+"_cut0",(neutrinoPz - genNupz), weight);
	   found_nu = true;

	   hmass_->Fill1d(TString("LeptonicW_mass")+"_cut1",(muonP4+nuP4).M(), weight );

	   hmet_->Fill1d(TString("LeptonicW_dij")+"_cut0",dij(muonP4, nuP4, 80.4), weight);

	   
	   // apply psi cut to MET
	   //double LepW_psi = Psi(muonP4, nuP4, 80.4);
	   //hmet_->Fill1d(TString("LeptonicW_psi")+"_cut0",LepW_psi, weight);	   
	   // select good MET
	   // first try using deltaR:
	   //if ( ROOT::Math::VectorUtil::DeltaPhi( muonP4.Vect(), nuP4.Vect() ) < 0.75 ) found_goodMET = true;
	   // second, using psi:
	   //if ( (muonP4+nuP4).M() < 100. && LepW_psi < 2 ) {

	   if ( (muonP4+nuP4).M() < 150. ) {
		   found_goodMET = true;
		   hmet_->Fill1d(TString("delta_nu_pz")+"_cut1",(neutrinoPz - genNupz), weight);
	   }

	   
	   
   }
   if (debug) std::cout << "got neutrino? " << found_nu << std::endl;
   if (debug) std::cout << "got good MET? " << found_goodMET << std::endl;

   
   
   // write ASCII file if requested
   if (fwriteAscii) {
	   // dump generated neutrino
	   if (genNu) fasciiFile << "-1 " << genNu->p4().Px() <<" "<< genNu->p4().Py() <<" "<< genNu->p4().Pz() << std::endl;
	   else fasciiFile << "-1 0 0 0" << std::endl;
	   // dump Muon
	   fasciiFile << "-15 " << muonP4.Px() <<" "<< muonP4.Py() <<" "<< muonP4.Pz() << std::endl;
	   // dump Neutrino
	   fasciiFile << "-5 " << nuP4.Px() <<" "<< nuP4.Py() <<" "<< nuP4.Pz() << std::endl;
	   // dump jets with pt>20
	   for( size_t ijet=0; ijet != jets.size(); ++ijet) {
		   
		   if (jets[ijet].pt() > 20. ) fasciiFile << jets[ijet].energy() <<" "<< jets[ijet].px() <<" "<<jets[ijet].py()<<" "<<jets[ijet].pz()<< std::endl;
		       
	   }
	   // dump weight
	   fasciiFile << weight << std::endl;
   }

   // find delta R of leptonic W with other jets
   bool found_leadingJet = false;
   size_t ith_leadingJet = 0;
   TLorentzVector leadingP4;
   
			   
   if ( found_goodMET) {

	   TLorentzVector lepWP4 = muonP4 + nuP4;
	   lepTopP4 = lepTopP4 + lepWP4;
	   
	   for( size_t ijet=0; ijet != jets.size(); ++ijet) {

		   if (jets[ijet].pt() <= 20. ) continue;
		   
		   TLorentzVector tmpP4;
		   tmpP4.SetPxPyPzE(jets[ijet].px(),jets[ijet].py(),jets[ijet].pz(),jets[ijet].energy());

		   double deltaR_jLepW = ROOT::Math::VectorUtil::DeltaR( tmpP4.Vect(), lepWP4.Vect() );
		   hjets_->Fill1d(TString("jet_deltaR_LeptonicW")+"_cut0", deltaR_jLepW ,weight);

		   if ( !found_leadingJet && deltaR_jLepW>2. ) {
			   found_leadingJet = true;
			   leadingP4 = tmpP4;
			   ith_leadingJet = ijet;
			   if (debug) std::cout << "found leading jet" <<std::endl;
		   }

	   }

	   if (debug) std::cout << "leading jet ("<<leadingP4.Px() <<","<<leadingP4.Py()<<","<<leadingP4.Pz()<<","<<leadingP4.E()<<")"<<std::endl;
	   if (debug) std::cout << "lepW jet ("<<lepWP4.Px() <<","<<lepWP4.Py()<<","<<lepWP4.Pz()<<","<<lepWP4.E()<<")"<<std::endl;
	   
	   if ( found_leadingJet ) {
	     //add leading jet to hadronic top
	     hadTopP4 += leadingP4;

		   for( size_t ijet=0; ijet != jets.size(); ++ijet) {

			   if (jets[ijet].pt() <= 20. ) continue;
			   
		     if (ith_leadingJet != ijet) {
			   TLorentzVector tmpP4;
			   tmpP4.SetPxPyPzE(jets[ijet].px(),jets[ijet].py(),jets[ijet].pz(),jets[ijet].energy());
			   
			   double psi_LepTop = Psi(tmpP4, lepWP4, 175.0 );
			   if (debug) std::cout << "psi_LepTop= " << psi_LepTop << std::endl;
			   double psi_HadTop = Psi(tmpP4, leadingP4, 175.0 );
			   if (debug) std::cout << "psi_HadTop= " << psi_HadTop << std::endl;
			   
			   hjets_->Fill1d(TString("LeptonicTop_psi")+"_cut0", psi_LepTop, weight);
			   hjets_->Fill1d(TString("HadronicTop_psi")+"_cut0", psi_HadTop, weight);

			   if ( psi_LepTop < psi_HadTop ) lepTopP4 += tmpP4;
			   else hadTopP4 += tmpP4;
			   
			   //if ( psi_LepTop>=0 && psi_HadTop>=0) {
			   // if ( psi_LepTop < psi_HadTop ) lepTopP4 += tmpP4;
			   // else hadTopP4 += tmpP4;
			   //}
			   //if ( psi_LepTop < 0 && psi_HadTop >= 0 ) hadTopP4 += tmpP4;
			   //if ( psi_LepTop >= 0 && psi_HadTop < 0 ) lepTopP4 += tmpP4;
		     }
		   }
	   }

	   if (debug) {
		   std::cout << "leptonic top pt= " << lepTopP4.Pt() << " mass= " << lepTopP4.M() << std::endl;
		   std::cout << "hadronic top pt= " << hadTopP4.Pt() << " mass= " << hadTopP4.M() <<std::endl;
	   }
	   
	   hjets_->Fill1d(TString("LeptonicTop_pt")+"_cut0", lepTopP4.Pt(), weight);
	   hjets_->Fill1d(TString("HadronicTop_pt")+"_cut0", hadTopP4.Pt(), weight);
	   if (debug) std::cout << "done pt" << std::endl;
	   
	   hmass_->Fill1d(TString("LeptonicTop_mass")+"_cut1", lepTopP4.M(), weight);
	   hmass_->Fill1d(TString("HadronicTop_mass")+"_cut1", hadTopP4.M(), weight);
	   if (debug) std::cout << "done mass" << std::endl;
	   
	   topPairP4 = hadTopP4+lepTopP4;
	   
	   hmass_->Fill1d(TString("topPair")+"_cut1", topPairP4.M(), weight);
	   
	   if (debug) std::cout << "done." << std::endl;
	   
   }// found good MET
  
   
 
   //if (debug) std::cout << "got Mjet?" << found_Mjet << " Et = " << mjetP4.Et() << " with listofmergedjets.size = " << listofmergedjets.size() << std::endl;
 
	  
     
	}
	// count all events
   nevents++;
}


//define this as a plug-in
DEFINE_FWK_MODULE(TtEventAnalysis);
