
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
  recoTrackList_ = iConfig.getUntrackedParameter<std::vector<std::string> >("TrkCollectionList");
  // get list of PV
  recoVtxList_ = iConfig.getUntrackedParameter<std::vector<std::string> >("PVCollectionList");

  //
  MuonCollectionTags_ = iConfig.getParameter<std::string>("Muons");
  
  CaloJetCollectionTags_ = iConfig.getParameter<std::string>("Jets");

  GenJetCollectionTags_ = iConfig.getParameter<std::string>("GenJets");

  JetTrackAssociatorTags_ = iConfig.getParameter<std::string>("JetTracks");

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
		  ftree->Branch("TopEvent","TopEvent",&fmyEvent,64000,1);
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
	  
  
  
  //simUnit_= 1.0;  // starting with CMSSW_1_2_x ??
  if ( (edm::getReleaseVersion()).find("CMSSW_1_1_",0)!=std::string::npos){
    simUnit_=0.1;  // for use in  CMSSW_1_1_1 tutorial
  }
  simUnit_= 1.;  // apparently not, still need this

  fnEvent = 0;
  fnAccepted = 0;
  
}


TopAnalyzer::~TopAnalyzer()
{
	ftree->Write();
		
	
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  
    
	if ( fmyEvent ) delete fmyEvent;
	//std::cout << " delete" << std::endl;
	//if ( fmyEvent[1] ) delete fmyEvent[1];
	//std::cout << " delete" << std::endl;
	//if ( fmyEvent[2] ) delete fmyEvent[2];
	//std::cout << " delete" << std::endl;
	delete ftree;
	delete rootFile_;
}



//
// member functions
//
void TopAnalyzer::beginJob(edm::EventSetup const& iSetup){
  std::cout << " bTagAnalyzer::beginJob  conversion from sim units to rec units is " << simUnit_ << std::endl;

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
  rootFile_->cd();

  std::cout << " Total number of events processed: \t" << fnEvent << std::endl;
  std::cout << " Total events accepted by filter:  \t" << fnAccepted << std::endl;

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
TopAnalyzer::filter(const Event& iEvent, const EventSetup& iSetup) {

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
	bool passfilter = filter(iEvent,iSetup);

	if (passfilter) {

		jetFlavourIdentifier_.readEvent(iEvent);
	
		// initialize jet flavour tool
		jetFlavourIdentifier_.readEvent(iEvent);
	
		// get tracks from collections
		Handle<reco::TrackCollection> recTrks[2];
		iEvent.getByLabel(recoTrackList_[0], recTrks[0]); // use only one collection for the moment

	
		// get PV collections
		Handle<reco::VertexCollection> recVtxs[3];
		iEvent.getByLabel(recoVtxList_[0], recVtxs[0] );// use only one collection for the moment
	
		// get jet associator
		Handle<reco::JetTracksAssociationCollection> jetTracksAssociation;
		iEvent.getByLabel(JetTrackAssociatorTags_, jetTracksAssociation);

		// get muon collection
		Handle<reco::MuonCollection> muonsColl;
		iEvent.getByLabel(MuonCollectionTags_, muonsColl);

		// get simulated collection of tracks
		Handle<edm::SimTrackContainer> simtrkColl;
		iEvent.getByLabel(SimTrkCollectionTags_, simtrkColl);

		// get jet collection
		Handle<reco::CaloJetCollection> jetsColl;
		iEvent.getByLabel(CaloJetCollectionTags_, jetsColl);

		// get generated jet collection
		Handle<reco::GenJetCollection> genjetsColl;
		iEvent.getByLabel(GenJetCollectionTags_, genjetsColl);

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

		// get products
		const reco::CaloJetCollection recoJets =   *(jetsColl.product());
		const reco::GenJetCollection  genJets  =   *(genjetsColl.product());
		const reco::MuonCollection    recoMuons =  *(muonsColl.product());
		const edm::SimTrackContainer simTrks =    *(simtrkColl.product());
	
		// MC
  
		//Handle<SimTrackContainer> simTrks;
		//iEvent.getByLabel( simG4_, simTrks);

		bool MC=false;
		Handle<HepMCProduct> evtMC;
  
		// for now just use one collection
		//int ispl=0;

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
		
		// define iterators
		CaloJetCollection::const_iterator jet;
		GenJetCollection::const_iterator genjet;
		reco::MuonCollection::const_iterator muon;
		SimTrackContainer::const_iterator simmuon;
		//reco::JetTagCollection::iterator btagite;
		
		// loop over gen jets
		
		for ( genjet = genJets.begin(); genjet != genJets.end(); ++genjet ) {

			fmyEvent->genjet_p.push_back(   (*genjet).p());
			fmyEvent->genjet_pt.push_back(  (*genjet).pt());
			fmyEvent->genjet_eta.push_back( (*genjet).eta());
			fmyEvent->genjet_phi.push_back( (*genjet).phi());
			fmyEvent->genjet_et.push_back(  (*genjet).et());
			fmyEvent->genjet_vx.push_back(  (*genjet).vx());
			fmyEvent->genjet_vy.push_back(  (*genjet).vy());
			fmyEvent->genjet_vz.push_back(  (*genjet).vz());	
			fmyEvent->genjet_flavour.push_back( jetFlavourIdentifier_.identifyBasedOnPartons(*genjet).flavour() );
		}

		// loop over sim tracks and select only muons
		for ( simmuon = simTrks.begin(); simmuon != simTrks.end(); ++simmuon ) {
			SimTrack genmuon = *simmuon;
			if ( abs(genmuon.type()) == 13 ) {

				fmyEvent->lepton_p_chi2_mc.push_back( genmuon.momentum().vect().mag() );
				fmyEvent->lepton_pt_chi2_mc.push_back( genmuon.momentum().perp() );
				fmyEvent->lepton_phi_chi2_mc.push_back( genmuon.momentum().phi() );
				fmyEvent->lepton_eta_chi2_mc.push_back( genmuon.momentum().pseudoRapidity() );
				fmyEvent->lepton_charge_chi2_mc.push_back( genmuon.charge() );
				fmyEvent->lepton_vx_chi2_mc.push_back(genmuon.momentum().x());
				fmyEvent->lepton_vy_chi2_mc.push_back(genmuon.momentum().y());
				fmyEvent->lepton_vz_chi2_mc.push_back(genmuon.momentum().z());
				fmyEvent->lepton_pdgid_chi2_mc.push_back( genmuon.type() );
				
			}
			
		}
	
	
		ftree->Fill();
	}
	
}

//define this as a plug-in
DEFINE_FWK_MODULE(TopAnalyzer);
