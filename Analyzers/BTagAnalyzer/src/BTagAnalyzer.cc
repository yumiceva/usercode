
#include "Analyzers/BTagAnalyzer/interface/BTagAnalyzer.h"

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
BTagAnalyzer::BTagAnalyzer(const ParameterSet& iConfig)
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

  JetTrackAssociatorTags_ = iConfig.getParameter<std::string>("JetTracks");
  
  jetFlavourIdentifier_ = JetFlavourIdentifier(iConfig.getParameter<edm::ParameterSet>("jetIdParameters"));

  conesize_ = iConfig.getParameter<double>("coneSize");
						 
  // get list of taggers
  TaggerList_ = iConfig.getUntrackedParameter<std::vector<std::string> >("TaggerList");
  int isample=0;
  for(std::vector<std::string>::iterator objectName = TaggerList_.begin(); objectName != TaggerList_.end(); ++objectName) {
	  if ( *objectName == "TrackCounting" ) {
		  //std::cout << "in TrackCounting ini" << std::endl;
		  isample=0;
		  moduleLabel_.push_back("trackCountingJetTags");
		  //std::cout << "in TrackCounting summary to be created" << std::endl;
		  fMySummary[isample] = new BTagSummary();
		  //std::cout << "in TrackCounting summary created" << std::endl;
		  ftree->Branch("TC.","BTagSummary",&fMySummary[isample],64000,1);
		  //std::cout << " ini done" << std::endl;
	  }
	  if ( *objectName == "SoftElectron" ) {
		  isample=1;
		  fMySummary[isample] = new BTagSummary();
		  ftree->Branch("SE.","BTagSummary",&fMySummary[isample],64000,1);
	  }
	  if ( *objectName == "SoftMuon" ) {
		  isample=2;
		  fMySummary[isample] = new BTagSummary();
		  ftree->Branch("SM.","BTagSummary",&fMySummary[isample],64000,1);
	  }
	  
  }
	  
  
  //simUnit_= 1.0;  // starting with CMSSW_1_2_x ??
  if ( (edm::getReleaseVersion()).find("CMSSW_1_1_",0)!=std::string::npos){
    simUnit_=0.1;  // for use in  CMSSW_1_1_1 tutorial
  }
  simUnit_= 1.;  // apparently not, still need this

  feventcounter++;
  
}


BTagAnalyzer::~BTagAnalyzer()
{
	ftree->Write();
		
	
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  
    
	if ( fMySummary[0] ) delete fMySummary[0];
	//std::cout << " delete" << std::endl;
	//if ( fMySummary[1] ) delete fMySummary[1];
	//std::cout << " delete" << std::endl;
	//if ( fMySummary[2] ) delete fMySummary[2];
	//std::cout << " delete" << std::endl;
	delete ftree;
	delete rootFile_;
}



//
// member functions
//
void BTagAnalyzer::beginJob(edm::EventSetup const& iSetup){
  std::cout << " bTagAnalyzer::beginJob  conversion from sim units to rec units is " << simUnit_ << std::endl;

  rootFile_->cd();
  // release validation histograms used in DoCompare.C
    
}


void BTagAnalyzer::endJob() {
  rootFile_->cd();

}


// ------------ method called to produce the data  ------------
void
BTagAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{

	jetFlavourIdentifier_.readEvent(iEvent);
	
    // get tracks from two collections
	Handle<reco::TrackCollection> recTrks[2];
	iEvent.getByLabel(recoTrackList_[0], recTrks[0]);

	
	// get PV collections
	Handle<reco::VertexCollection> recVtxs[3];
	iEvent.getByLabel(recoVtxList_[0], recVtxs[0] );
	
	//
	Handle<reco::JetTracksAssociationCollection> jetTracksAssociation;
	iEvent.getByLabel(JetTrackAssociatorTags_, jetTracksAssociation);

	Handle<reco::MuonCollection> muons;
	iEvent.getByLabel(MuonCollectionTags_, muons);

	Handle<reco::CaloJetCollection> jets;
	iEvent.getByLabel(CaloJetCollectionTags_, jets);

	// b-tag
	edm::Handle<reco::JetTagCollection> tagHandle;
	iEvent.getByLabel(moduleLabel_[0], tagHandle);
	const reco::JetTagCollection & btagColl = *(tagHandle.product());

	edm::Handle<reco::TrackCountingTagInfoCollection> tagInfoHandle;
	iEvent.getByLabel(moduleLabel_[0], tagInfoHandle);
	const reco::TrackCountingTagInfoCollection & btagInfo = *(tagInfoHandle.product());

	const reco::CaloJetCollection recoJets =   *(jets.product());
	const reco::MuonCollection    recoMuons =  *(muons.product());

	// MC
  
	Handle<SimTrackContainer> simTrks;
	iEvent.getByLabel( simG4_, simTrks);

	bool MC=false;
	Handle<HepMCProduct> evtMC;
  
	// for now just use one collection
	int ispl=0;

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
  
  	  
	fMySummary[ispl]->Reset();
	
	fMySummary[ispl]->event = iEvent.id().event();
	fMySummary[ispl]->run = iEvent.id().run();

	
	// loop over jets
	fMySummary[ispl]->njets = recoJets.size();
	fMySummary[ispl]->nmuons = recoMuons.size();
	
	CaloJetCollection::const_iterator jet;
	reco::MuonCollection::const_iterator muon;

	//reco::JetTagCollection::iterator btagite;

	int btagCollsize = btagColl.size();

	bool muonfound = false;
	for( jet = recoJets.begin(); jet != recoJets.end(); ++jet ) {

		int tmptotmuon = 0;
		
		for ( muon = recoMuons.begin(); muon != recoMuons.end(); ++muon) {

			double delta  = ROOT::Math::VectorUtil::DeltaR(jet->p4().Vect(), muon->momentum());

			if ( (delta > conesize_) || muon->pt()<5 ) continue;

			muonfound = true;
			
			fMySummary[ispl]->jet_pt.push_back(jet->pt());
			fMySummary[ispl]->jet_eta.push_back(jet->eta());
			fMySummary[ispl]->jet_phi.push_back(jet->phi());
			fMySummary[ispl]->jet_et.push_back(jet->et());

			fMySummary[ispl]->jet_deltar.push_back(delta);

			fMySummary[ispl]->muon_pt.push_back(muon->pt());
			fMySummary[ispl]->muon_eta.push_back(muon->eta());
			fMySummary[ispl]->muon_phi.push_back(muon->phi());
			fMySummary[ispl]->muon_charge.push_back(muon->charge());
			fMySummary[ispl]->muon_p.push_back(muon->p());
			
			TVector3 tmpvec(jet->p4().Vect().X(),jet->p4().Vect().Y(),  jet->p4().Vect().Z());
			TVector3 muonvec(muon->momentum().X(), muon->momentum().Y(),muon->momentum().Z());
			tmpvec += muonvec;

			double ptrel = muonvec.Perp(tmpvec);

			// cross-check ptrel
			//std::cout << "ptrel = " << ptrel << std::endl;
			//double tmpdot = (muonvec*tmpvec)/tmpvec.Mag();
			//std::cout << "check = " << sqrt(muon->p()*muon->p() - tmpdot*tmpdot ) << std::endl;

			fMySummary[ispl]->jet_ptrel.push_back(ptrel);
			fMySummary[ispl]->jet_flavour.push_back(jetFlavourIdentifier_.identifyBasedOnPartons(*jet).flavour());	

			int isbTagged = 0;
			double small = 1.e-5;
			double discriminator0 = -9999.;
			double discriminator1 = -9999.;
			double discriminator2 = -9999.;
			
			for (int ib = 0; ib != btagCollsize; ++ib ) {
				
				// naive way to check for similar jets
				if ( std::abs(jet->pt() - btagColl[ib].jet().pt())< small && std::abs(jet->eta() - btagColl[ib].jet().eta())< small ) {
					isbTagged = 1;
					discriminator0 = btagInfo[ib].discriminator(0,1);
					discriminator1 = btagInfo[ib].discriminator(1,1);
					discriminator2 = btagInfo[ib].discriminator(2,1);
					
					break;
				}
			}
			fMySummary[ispl]->jet_btagged.push_back(isbTagged);
			fMySummary[ispl]->btag_discriminator0.push_back(discriminator0);
			fMySummary[ispl]->btag_discriminator1.push_back(discriminator1);
			fMySummary[ispl]->btag_discriminator2.push_back(discriminator2);
			
			tmptotmuon++;

		}
		//std::cout << " total muons in jet = " << tmptotmuon << std::endl;			
		
	}

	if ( muonfound ) {

		for( jet = recoJets.begin(); jet != recoJets.end(); ++jet ) {

			fMySummary[ispl]->otherjet_pt.push_back(jet->pt());
			fMySummary[ispl]->otherjet_eta.push_back(jet->eta());
			fMySummary[ispl]->otherjet_phi.push_back(jet->phi());
			fMySummary[ispl]->otherjet_et.push_back(jet->et());
			
			fMySummary[ispl]->otherjet_flavour.push_back(jetFlavourIdentifier_.identifyBasedOnPartons(*jet).flavour());	
			int isbTagged = 0;
			double small = 1.e-5;
			double discriminator0 = -9999.;
			double discriminator1 = -9999.;
			double discriminator2 = -9999.;
			
			for (int ib = 0; ib != btagCollsize; ++ib ) {
				
				// naive way to check for similar jets
				if ( std::abs(jet->pt() - btagColl[ib].jet().pt())< small && std::abs(jet->eta() - btagColl[ib].jet().eta())< small ) {
					isbTagged = 1;
					discriminator0 = btagInfo[ib].discriminator(0,1);
					discriminator1 = btagInfo[ib].discriminator(1,1);
					discriminator2 = btagInfo[ib].discriminator(2,1);
					
					break;
				}
			}
			fMySummary[ispl]->otherjet_btagged.push_back(isbTagged);
			fMySummary[ispl]->otherbtag_discriminator0.push_back(discriminator0);
			fMySummary[ispl]->otherbtag_discriminator1.push_back(discriminator1);
			fMySummary[ispl]->otherbtag_discriminator2.push_back(discriminator2);

		}
	}

	
	ftree->Fill();

	feventcounter++;
	
}

//define this as a plug-in
DEFINE_FWK_MODULE(BTagAnalyzer);
