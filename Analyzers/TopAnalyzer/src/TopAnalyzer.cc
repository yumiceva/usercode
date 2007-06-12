
#include "Analyzers/TopAnalyzer/interface/TopAnalyzer.h"

#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// reco track and vertex
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/BTauReco/interface/JetTagFwd.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfoFwd.h"

#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

// simulated vertices,..., add <use name=SimDataFormats/Vertex> and <../Track>
#include <SimDataFormats/Vertex/interface/SimVertex.h>
#include <SimDataFormats/Vertex/interface/SimVertexContainer.h>
#include <SimDataFormats/Track/interface/SimTrack.h>
#include <SimDataFormats/Track/interface/SimTrackContainer.h>

//generator level + CLHEP
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "CLHEP/HepMC/GenEvent.h"
#include "CLHEP/HepMC/GenVertex.h"

// HepPDT // for simtracks
#include "SimGeneral/HepPDT/interface/HepPDTable.h"
#include "SimGeneral/HepPDT/interface/HepParticleData.h"

// Root
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "Math/LorentzVector.h"

using namespace edm;
using namespace reco;

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TopAnalyzer::TopAnalyzer(const ParameterSet& iConfig)
{

	
   //now do what ever initialization is needed
  simG4_=iConfig.getParameter<edm::InputTag>( "simG4" );
  
  // open output file to store results
  outputFile_  = iConfig.getUntrackedParameter<std::string>("outputFile");
  // create or update output root file
  rootFile_ = TFile::Open(outputFile_.c_str(),"RECREATE");
  // verbose std output
  verbose_= iConfig.getUntrackedParameter<bool>("verbose", false);
  // default tree
  ftree = new TTree("summary","summary");
  ftree->AutoSave();

  // get list of tracks
  //recoTrackList_ = iConfig.getUntrackedParameter<std::vector<std::string> >("TrkCollectionList");
  // get list of PV
  //recoVtxList_ = iConfig.getUntrackedParameter<std::vector<std::string> >("PVCollectionList");

  //
  MuonCollectionTags_ = iConfig.getParameter<std::string>("Muons");
  
  CaloJetCollectionTags_ = iConfig.getParameter<std::string>("Jets");

  GenJetCollectionTags_ = iConfig.getParameter<std::string>("GenJets");

  //JetTrackAssociatorTags_ = iConfig.getParameter<std::string>("JetTracks");

  SimTrkCollectionTags_ = iConfig.getParameter<std::string>("SimTracks");
  
  jetFlavourIdentifier_ = JetFlavourIdentifier(iConfig.getParameter<edm::ParameterSet>("jetIdParameters"));

  conesize_ = iConfig.getParameter<double>("coneSize");
						 
  // get list of taggers
  bTaggerList_ = iConfig.getUntrackedParameter<std::vector<std::string> >("bTaggerList");
  int isample=0;
  for(std::vector<std::string>::iterator objectName = bTaggerList_.begin(); objectName != bTaggerList_.end(); ++objectName) {
	  if ( *objectName == "TrackCounting" ) {
		  //std::cout << "in TrackCounting ini" << std::endl;
		  isample=0;
		  moduleLabel_.push_back("trackCountingJetTags");
		  //std::cout << "in TrackCounting summary to be created" << std::endl;
		  fmyEvent = new TopEvent();
		  //std::cout << "in TrackCounting summary created" << std::endl;
		  ftree->Branch("top.","TopEvent",&fmyEvent,64000,1);
		  //ftree->Branch("TC.","TopEvent",&fmyEvent[isample],64000,1);
		  //std::cout << " ini done" << std::endl;
	  }
	  /// fixme: change below to storage only one branch
	  /*
	  if ( *objectName == "SoftElectron" ) {
		  isample=1;
		  fmyEvent[isample] = new TopEvent();
		  ftree->Branch("SE.","TopEvent",&fmyEvent[isample],64000,1);
	  }
	  if ( *objectName == "SoftMuon" ) {
		  isample=2;
		  fmyEvent[isample] = new TopEvent();
		  ftree->Branch("SM.","TopEvent",&fmyEvent[isample],64000,1);
	  }
	  */
  }
	  
  
 
  // counters
  fnEvent = 0;
  fnAccepted = 0;
  fnEvent_cut0 = 0;
  
}


TopAnalyzer::~TopAnalyzer()
{
	ftree->Write();

	if ( fmyEvent ) delete fmyEvent;
	delete ftree;
	delete rootFile_;
}



//
// member functions
//
void TopAnalyzer::beginJob(edm::EventSetup const& iSetup){
  std::cout << "TopAnalyzer::beginJob() " << std::endl;

  rootFile_->cd();
  // release validation histograms used in DoCompare.C

  /*
  // track matching MC
  edm::ESHandle<TrackAssociatorBase> theChiAssociator;
  iSetup.get<TrackAssociatorRecord>().get("TrackAssociatorByChi2",theChiAssociator);
  associatorByChi2 = (TrackAssociatorBase *) theChiAssociator.product();
  
  edm::ESHandle<TrackAssociatorBase> theHitsAssociator;
  iSetup.get<TrackAssociatorRecord>().get("TrackAssociatorByHits",theHitsAssociator);
  associatorByHits = (TrackAssociatorByHits *) theHitsAssociator.product();
  */
}


void TopAnalyzer::endJob() {
	
	std::cout << "TopAnalyzer::endJob() " << std::endl;
	
	rootFile_->cd();
	double fraction = 0;
	std::cout << "[TopAnalyzer] Total number of events processed: \t" << fnEvent << std::endl;
	fraction = ((double)fnAccepted)/((double)fnEvent);
	std::cout << "[TopAnalyzer] Total events accepted by filter:  \t" << fnAccepted << "\t ("<< fraction << "%)" << std::endl;
	fraction = ((double)fnEvent_cut0)/((double)fnAccepted);
	std::cout << "[TopAnalyzer] Total events accepted by cut 0 :  \t" << fnEvent_cut0 << "]t (" << fraction << "%)" << std::endl;

}

reco::GenJet TopAnalyzer::GetGenJet(reco::CaloJet calojet, reco::GenJetCollection genJetColl) {

  reco::GenJet matchedJet;

  double predelta = 99999.;
  for (GenJetCollection::const_iterator genjet = genJetColl.begin(); genjet != genJetColl.end(); ++genjet) {

    double delta  = ROOT::Math::VectorUtil::DeltaR(genjet->p4().Vect(), calojet.p4().Vect() );

    if ( delta < 0.2 && delta<predelta ) {
      matchedJet = *genjet;
      predelta = delta;
    }
  }
  
  return matchedJet;
}

SimTrack TopAnalyzer::GetGenTrk(reco::Track atrack, edm::SimTrackContainer simTrkColl) {

  SimTrack matchedTrk;

  double predelta = 99999.;
  for (SimTrackContainer::const_iterator gentrk = simTrkColl.begin(); gentrk != simTrkColl.end(); ++gentrk) {

	  double delta  = ROOT::Math::VectorUtil::DeltaR( TVector3((*gentrk).momentum().x(),(*gentrk).momentum().y(),(*gentrk).momentum().z()) , atrack.momentum() );
	  
	  if ( delta < 0.2 && delta<predelta && ((*gentrk).charge() == atrack.charge() ) ) {
		  matchedTrk = *gentrk;
		  predelta = delta;
	  }
  }
  
  return matchedTrk;
}

bool
TopAnalyzer::GenFilter(const Event& iEvent, const EventSetup& iSetup) {

  using namespace edm;
  bool accepted = false;
  Handle<HepMCProduct> evt;

  iEvent.getByLabel("VtxSmeared", evt);
  HepMC::GenEvent * myGenEvent = new  HepMC::GenEvent(*(evt->GetEvent()));
  	
  int nW= 0;
  bool hasmuonW[2] = {false};
  bool ishadronicW[2] = {false};
	
  for ( HepMC::GenEvent::particle_iterator p = myGenEvent->particles_begin();
	p != myGenEvent->particles_end(); ++p ) {

    if ( !accepted && ( abs((*p)->pdg_id()) == 24 ) && (*p)->status() == 3 ) {
			
      if ((*p)->momentum().invariantMass() < (80.-25.) || (*p)->momentum().invariantMass() > (80.+25.) ) continue;
      
      std::vector<HepMC::GenParticle*> children = (*p)->listChildren();
      std::vector<HepMC::GenParticle*>::const_iterator aDaughter;
      for (aDaughter = children.begin();aDaughter != children.end();aDaughter++) {

	//std::cout << "daughters= " << children.size() << std::endl;
	//std::cout << " pdgid= " << (*aDaughter)->pdg_id() << std::endl;

	//HepMC::GenParticle* myDa= new HepMC::GenParticle(*(*aDaughter));
	
	if ( ((*aDaughter)->status() != 2) &&
	     (abs((*aDaughter)->pdg_id()) == abs(13)) ) {
	  //std::cout << " is muon" << std::endl;
	  hasmuonW[nW] = true;
	}

	if (     (abs((*aDaughter)->pdg_id())<11 )  ) {
	  //std::cout << " is hadronic "<< std::endl;
	  ishadronicW[nW] = true;
	}
			
	//		std::cout << (*aDaughter)->momentum().perp() << " " << (*aDaughter)->momentum().eta() << std::endl;
	// pt and eta cuts on lepton
	//if ((*aDaughter)->momentum().perp() < leptonPtMin_)		accepted = false;
	//if ((*aDaughter)->momentum().perp() > leptonPtMax_)		accepted = false;
	//if (fabs((*aDaughter)->momentum().eta()) > leptonEtaMax_) accepted = false;
	//if (fabs((*aDaughter)->momentum().eta()) < leptonEtaMin_)	accepted = false;
	
      }
      nW++;
    }
  }

  if ( hasmuonW[0] && ishadronicW[1] ) accepted = true;
  if ( hasmuonW[1] && ishadronicW[0] ) accepted = true;
	
  if (accepted) {
    fnAccepted++;
    //      std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++"<< std::endl;
    LogDebug("Filter") << "Event " << iEvent.id().event()  << " accepted" << std::endl;
    //      std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++"<< std::endl;
    //       myGenEvent->print();
    delete myGenEvent;
    return true;
  }

  delete myGenEvent;
  return false;

}




// ------------ method called to produce the data  ------------
void
TopAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{
	// count events
	fnEvent++;

	// apply filter
	bool passfilter = GenFilter(iEvent,iSetup);

	if (passfilter) {
		
		// initialize jet flavour tool
		jetFlavourIdentifier_.readEvent(iEvent);

		// initialize jet corrector
		const JetCorrector *acorrector = JetCorrector::getJetCorrector("MCJetCorrectorIcone5",iSetup);
		
		// get jet associator
		//Handle<reco::JetTracksAssociationCollection> jetTracksAssociation;
		//iEvent.getByLabel(JetTrackAssociatorTags_, jetTracksAssociation);
		//const reco::CaloJetCollection recoJets =   *(jetsColl.product()); // get product
		
		// get muon collection
		Handle<reco::MuonCollection> muonsColl;
		iEvent.getByLabel(MuonCollectionTags_, muonsColl);
		const reco::MuonCollection recoMuons =  *(muonsColl.product());
		
		// get simulated collection of tracks
		Handle<edm::SimTrackContainer> simtrkColl;
		iEvent.getByLabel(SimTrkCollectionTags_, simtrkColl);
		const edm::SimTrackContainer simTrks =    *(simtrkColl.product());

		// get simulated collection of vertices
		Handle<edm::SimVertexContainer> simVtxColl;
		iEvent.getByLabel( "g4SimHits", simVtxColl);
		const edm::SimVertexContainer simVtcs =    *(simVtxColl.product());
		
		// get jet collection
		Handle<reco::CaloJetCollection> jetsColl;
		iEvent.getByLabel(CaloJetCollectionTags_, jetsColl);
		const reco::CaloJetCollection recoJets =   *(jetsColl.product());
		
		// get generated jet collection
		Handle<reco::GenJetCollection> genjetsColl;
		iEvent.getByLabel(GenJetCollectionTags_, genjetsColl);
		const reco::GenJetCollection  genJets  =   *(genjetsColl.product());
		
		// truth tracks
		//edm::Handle<TrackingParticleCollection>  TPCollectionH ;
		//iEvent.getByLabel("trackingtruth","TrackTruth",TPCollectionH);
		//const TrackingParticleCollection tPC = *(TPCollectionH.product());

		// get b tagger collection
		edm::Handle<reco::JetTagCollection> tagHandle;
		iEvent.getByLabel(moduleLabel_[0], tagHandle); // use only one for the moment
		const reco::JetTagCollection & btagColl = *(tagHandle.product());

		edm::Handle<reco::TrackCountingTagInfoCollection> tagInfoHandle;
		iEvent.getByLabel(moduleLabel_[0], tagInfoHandle);
		const reco::TrackCountingTagInfoCollection & btagInfo = *(tagInfoHandle.product());
		int btagCollsize = btagColl.size();
				
		bool MC=false;
		Handle<HepMCProduct> evtMC;
  
		try {
			iEvent.getByLabel("VtxSmeared",evtMC);
			MC=true;
			if(verbose_){
				std::cout << "VtxSmeared HepMCProduct found"<< std::endl;
			}
		
		} catch(const Exception&){
      
			try{
				iEvent.getByLabel("source",evtMC);
				if(verbose_){
					std::cout << "source HepMCProduct found"<< std::endl;
				}
				MC=true;
			} catch(const Exception&) {
				MC=false;
				if(verbose_){
					std::cout << "no HepMCProduct found"<< std::endl;
				}
			}
		}
  
		// reset event container
		fmyEvent->Reset();

		// fill event ID
		fmyEvent->event = iEvent.id().event();
		fmyEvent->run = iEvent.id().run();
		
		// fill total number of objects
		fmyEvent->njets = recoJets.size();
		fmyEvent->nleptons = recoMuons.size();
		fmyEvent->ngenjets = genJets.size();
		fmyEvent->ngenleptons = simTrks.size();


		// generator stuff
		
		HepMC::GenEvent * myGenEvent = new  HepMC::GenEvent(*(evtMC->GetEvent()));
		for ( HepMC::GenEvent::particle_iterator p = myGenEvent->particles_begin();
		      p != myGenEvent->particles_end(); ++p ) {
		  // select ttbar
		  if ( abs((*p)->pdg_id()) == 6 ) {
			  fmyEvent->gentop_px.push_back((*p)->momentum().px());
			  fmyEvent->gentop_py.push_back((*p)->momentum().py());
			  fmyEvent->gentop_pz.push_back((*p)->momentum().pz());
			  fmyEvent->gentop_e.push_back((*p)->momentum().e());
			  fmyEvent->gentop_charge.push_back((*p)->pdg_id());
		  }
		  // select neutrino
		  if ( abs((*p)->pdg_id()) == 12 || abs((*p)->pdg_id()) == 14 ) {
		    fmyEvent->gennu_px.push_back((*p)->momentum().px());
		    fmyEvent->gennu_py.push_back((*p)->momentum().py());
		    fmyEvent->gennu_pz.push_back((*p)->momentum().pz());
		    fmyEvent->gennu_pdg.push_back((*p)->pdg_id());
                  }

		}

		//f ( fnAccepted < 3 ) {
		//std::cout << " njets= " << recoJets.size()
		//    << " nleptons= " << recoMuons.size() << std::endl;
		//}

		// define iterators
		CaloJetCollection::const_iterator jet;
		GenJetCollection::const_iterator genjet;
		reco::MuonCollection::const_iterator muon;
		SimTrackContainer::const_iterator simmuon;
		//reco::JetTagCollection::iterator btagite;

		// require at least 4 jets and at least 1 muon
		if ( recoJets.size() < 4  || recoMuons.size()==0 ) return;
		fnEvent_cut0++;

		// Lorentz vectors
		TLorentzVector p4Jet[4];
		TLorentzVector twobody[6];
		TLorentzVector threebody[12];
		TLorentzVector p4Muon;
		TVector3 p3MET;
		
		// loop over jets
		int ijet = 0;
		for( jet = recoJets.begin(); jet != recoJets.end(); ++jet ) {

			if ( ijet < 4 ) {
				p4Jet[ijet].SetXYZT(jet->px(),jet->py(),jet->pz(),jet->energy());
				fmyEvent->jet_px.push_back(jet->px());
				fmyEvent->jet_py.push_back(jet->py());
				fmyEvent->jet_pz.push_back(jet->pz());
				fmyEvent->jet_e.push_back(jet->energy());



				//if ( fnAccepted ) {
				//	  std::cout << " jet px= " << jet->px()
				//    << " jet py= " << jet->py() << std::endl;
				//  std::cout << " p4Jetx= " << p4Jet[ijet].Px()
				//    << " p4Jety= " << p4Jet[ijet].Py() << std::endl;
				  //}

				// get jet correction
				//fmyEvent->jet_correction.push_back( acorrector->correction(*jet, iEvent, iSetup) );

				// b-tagging
				/*
				double small = 1.e-5;
				double discriminant = -9999.;
				
				for (int ib = 0; ib != btagCollsize; ++ib ) {

				  // simple way to check for similar jets
				  if ( std::abs(jet->pt() - btagColl[ib].jet().pt())< small && std::abs(jet->eta() - 
													btagColl[ib].jet().eta())< small ) {
				    
				    discriminant = btagInfo[ib].discriminator(1,1);
				    
				    break;
				  }
				}
				*/
				//fmyEvent->jet_btag_discriminant.push_back( discriminant );

				// MET
				p3MET += TVector3(jet->px(),jet->py(),0.);
				
			}
			ijet++;
			fmyEvent->alljet_px.push_back(jet->px());
			fmyEvent->alljet_py.push_back(jet->py());
			fmyEvent->alljet_pz.push_back(jet->pz());
			fmyEvent->alljet_e.push_back(jet->energy());
			// get jet correction
			fmyEvent->jet_correction.push_back( acorrector->correction(*jet, iEvent, iSetup) );
			// b-tagging
			double small = 1.e-5;
			double discriminant = -9999.;
				
			for (int ib = 0; ib != btagCollsize; ++ib ) {

				// simple way to check for similar jets
				if ( std::abs(jet->pt() - btagColl[ib].jet().pt())< small && std::abs(jet->eta() - 
																					  btagColl[ib].jet().eta())< small ) {
				    
				    discriminant = btagInfo[ib].discriminator(1,1);
				    
				    break;
				}
			}
			fmyEvent->jet_btag_discriminant.push_back( discriminant );
			
			fmyEvent->alljet_et.push_back(jet->et());
			fmyEvent->alljet_eta.push_back(jet->eta());
			fmyEvent->alljet_phi.push_back(jet->phi());
		}
		
		// loop over muons
		double mass_mu = 0.105658; // GeV/c
		int imuon = 0;
		for (muon = recoMuons.begin(); muon != recoMuons.end(); ++muon) {
		  
		  Track combinedMuon = *(muon->combinedMuon());

		  double global_chi2 = combinedMuon.chi2();
		  double global_ndof = combinedMuon.ndof();
		  double mu_pt = combinedMuon.pt();
		  
		  // find a sim track
		  SimTrack genmuon = this->GetGenTrk(combinedMuon, simTrks );
		  
		  if ( mu_pt > 10 ) {
		    p4Muon.SetXYZT(combinedMuon.px(), combinedMuon.py(), combinedMuon.pz(), TMath::Sqrt(combinedMuon.p() * combinedMuon.p() + mass_mu*mass_mu) );
		    fmyEvent->muon_px.push_back(combinedMuon.px());
		    fmyEvent->muon_py.push_back(combinedMuon.py());
		    fmyEvent->muon_pz.push_back(combinedMuon.pz());
		    fmyEvent->muon_e.push_back(p4Muon.E());

		    //std::cout << " p4MuonPx= " << p4Muon.Px() 
		    //      << " p4MuonPy= " << p4Muon.Py() << std::endl;

			fmyEvent->muon_mc_pdgid.push_back(genmuon.type());
			p3MET += TVector3(combinedMuon.px(),combinedMuon.py(),0.);
		    imuon++;
		  }
		  fmyEvent->allmuon_pt.push_back(mu_pt);
		  fmyEvent->allmuon_normchi2.push_back(global_chi2/global_ndof);
		  fmyEvent->allmuon_d0.push_back(combinedMuon.d0());
		  fmyEvent->allmuon_d0Error.push_back(combinedMuon.d0Error());
		  fmyEvent->allmuon_nrechits.push_back(combinedMuon.recHitsSize());
		  fmyEvent->allmuon_mc_pdgid.push_back( genmuon.type() );
		  
		}
		//std::cout << "muons that passed selection: " << imuon << std::endl;

		// set MET
		fmyEvent->met.push_back(p3MET.Mag());
		
		// find masses
		twobody[0] = p4Jet[0] + p4Jet[1];
		twobody[1] = p4Jet[0] + p4Jet[2];
		twobody[2] = p4Jet[0] + p4Jet[3];
		twobody[3] = p4Jet[1] + p4Jet[2];
		twobody[4] = p4Jet[1] + p4Jet[3];
		twobody[5] = p4Jet[2] + p4Jet[3];
		
		for ( int i=0; i<6; i++ ) {
		  fmyEvent->twobody.push_back( twobody[i] );
		}

		threebody[0] = twobody[0] + p4Jet[2];
		threebody[1] = twobody[0] + p4Jet[3];
		threebody[2] = twobody[1] + p4Jet[1];
		threebody[3] = twobody[1] + p4Jet[3];
		threebody[4] = twobody[2] + p4Jet[1];
		threebody[5] = twobody[2] + p4Jet[2];
		threebody[6] = twobody[3] + p4Jet[0];
		threebody[7] = twobody[3] + p4Jet[3];
		threebody[8] = twobody[4] + p4Jet[0];
		threebody[9] = twobody[4] + p4Jet[2];
		threebody[10] = twobody[5] + p4Jet[0];
		threebody[11] = twobody[5] + p4Jet[1];
		
		for (int i=0; i<6; i++) {
		  fmyEvent->threebody.push_back( threebody[i] );
		}

		
		// fill event
		ftree->Fill();
		
	}// close filter
	
}

//define this as a plug-in
DEFINE_FWK_MODULE(TopAnalyzer);
