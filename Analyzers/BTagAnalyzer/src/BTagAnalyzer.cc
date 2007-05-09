
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
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfoFwd.h"

#include "DataFormats/EgammaReco/interface/SuperCluster.h"

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

#include <map>

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

  CorrCaloJetCollectionTags_ = iConfig.getParameter<std::string>("CorrJets");

  GenJetCollectionTags_ = iConfig.getParameter<std::string>("GenJets");

  JetTrackAssociatorTags_ = iConfig.getParameter<std::string>("JetTracks");

  SimTrkCollectionTags_ = iConfig.getParameter<std::string>("SimTracks");
  
  jetFlavourIdentifier_ = JetFlavourIdentifier(iConfig.getParameter<edm::ParameterSet>("jetIdParameters"));
  jetFlavourIdentifier2_ = JetFlavourIdentifier(iConfig.getParameter<edm::ParameterSet>("jetIdParameters2"));

  conesize_ = iConfig.getParameter<double>("coneSize");
						 
  // get list of taggers, just one for the moment
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

  feventcounter = 0;
  
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


void BTagAnalyzer::endJob() {

	std::cout << "[BTagAnalyzer] Total events processed: " << feventcounter << std::endl;
	
  rootFile_->cd();

}

reco::GenJet BTagAnalyzer::GetGenJet(reco::CaloJet calojet, reco::GenJetCollection genJetColl) {

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

SimTrack BTagAnalyzer::GetGenTrk(reco::Track atrack, edm::SimTrackContainer simTrkColl, edm::SimVertexContainer simVtcs) {

	  SimTrack matchedTrk;

  double predelta = 99999.;
  for (SimTrackContainer::const_iterator gentrk = simTrkColl.begin(); gentrk != simTrkColl.end(); ++gentrk) {

	  double delta  = ROOT::Math::VectorUtil::DeltaR( TVector3((*gentrk).momentum().x(),(*gentrk).momentum().y(),(*gentrk).momentum().z()) , atrack.momentum() );
	  int type = (*gentrk).type();
	if ( delta < 0.2 && delta<predelta && ((*gentrk).charge() == atrack.charge() ) && 
		( abs(type)==11 || abs(type)==13 || abs(type)==15 || abs(type)==211 || abs(type)==321 ) ) {
		  matchedTrk = *gentrk;
		  predelta = delta;
		  HepLorentzVector v = (simVtcs)[(*gentrk).vertIndex()].position();
		  
		  //std::cout << "gentrk: vx = " << v.x() << std::endl;
		  //std::cout << "rectrk: vx = " << atrack.vx() << std::endl;
		  
	  }
  }
  
  return matchedTrk;
}

int
BTagAnalyzer::GetMotherId(edm::SimVertexContainer simVtxColl, edm::SimTrackContainer simTrkColl, SimTrack muonMC) {

  // fill map of simtracks
  std::map<unsigned, unsigned> geantToIndex;
  for( unsigned it=0; it<simTrkColl.size(); ++it ) {
    geantToIndex[ simTrkColl[it].trackId() ] = it;
  }

  // The origin vertex
  int vertexId = muonMC.vertIndex();
  SimVertex vertex = simVtxColl[vertexId];

  // The mother track 
  int motherId = -1;
  if( vertex.parentIndex() ) { // there is a parent to this vertex
 
    // geant id of the mother
    unsigned motherGeandId =   vertex.parentIndex(); 
    std::map<unsigned, unsigned >::iterator association  
      = geantToIndex.find( motherGeandId );
    if(association != geantToIndex.end() )
      motherId = association->second;
  }
  
  
  int motherType = motherId == -1 ? 0 : simTrkColl[motherId].type();

  return motherType;

}

// ------------ method called to produce the data  ------------
void
BTagAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{

	jetFlavourIdentifier_.readEvent(iEvent);
	jetFlavourIdentifier2_.readEvent(iEvent);
	
    // get tracks from two collections
	Handle<reco::TrackCollection> recTrks[2];
	iEvent.getByLabel(recoTrackList_[0], recTrks[0]);

	
	// get PV collections
	Handle<reco::VertexCollection> recVtxs[3];
	iEvent.getByLabel(recoVtxList_[0], recVtxs[0] );
	
	//
	Handle<reco::JetTracksAssociationCollection> jetTracksAssociation;
	iEvent.getByLabel(JetTrackAssociatorTags_, jetTracksAssociation);

	Handle<reco::MuonCollection> muonsColl;
	iEvent.getByLabel(MuonCollectionTags_, muonsColl);

	Handle<edm::SimTrackContainer> simtrkColl;
	iEvent.getByLabel(SimTrkCollectionTags_, simtrkColl);
	
	Handle<reco::CaloJetCollection> jetsColl;
	iEvent.getByLabel(CaloJetCollectionTags_, jetsColl);

	Handle<reco::CaloJetCollection> jetsCorrColl;
	iEvent.getByLabel(CorrCaloJetCollectionTags_, jetsCorrColl);

	Handle<reco::GenJetCollection> genjetsColl;
	iEvent.getByLabel(GenJetCollectionTags_, genjetsColl);

	Handle<edm::SimVertexContainer> simVtxColl;
	iEvent.getByLabel( "g4SimHits", simVtxColl);
	const edm::SimVertexContainer simVtcs =    *(simVtxColl.product());


	// truth tracks
	//edm::Handle<TrackingParticleCollection>  TPCollectionH ;
	//iEvent.getByLabel("trackingtruth","TrackTruth",TPCollectionH);
	//const TrackingParticleCollection tPC = *(TPCollectionH.product());

	// b-tag
	edm::Handle<reco::JetTagCollection> tagHandle;
	iEvent.getByLabel(moduleLabel_[0], tagHandle);
	const reco::JetTagCollection & btagColl = *(tagHandle.product());

	edm::Handle<reco::TrackCountingTagInfoCollection> tagInfoHandle;
	iEvent.getByLabel(moduleLabel_[0], tagInfoHandle);
	const reco::TrackCountingTagInfoCollection & btagInfo = *(tagInfoHandle.product());

	const reco::CaloJetCollection recoJets =   *(jetsColl.product());
	const reco::CaloJetCollection recoCorrJets =   *(jetsCorrColl.product());
	const reco::GenJetCollection  genJets  =   *(genjetsColl.product());
	const reco::MuonCollection    recoMuons =  *(muonsColl.product());
	const edm::SimTrackContainer simTrks =    *(simtrkColl.product());
	
	// MC
  
	//Handle<SimTrackContainer> simTrks;
	//iEvent.getByLabel( simG4_, simTrks);

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
	fMySummary[ispl]->ngenjets = genJets.size();

	CaloJetCollection::const_iterator jet;
	//GenJetCollection::const_iterator genjet;
	reco::MuonCollection::const_iterator muon;

	//reco::JetTagCollection::iterator btagite;


	// mc truth 
/*	reco::RecoToSimCollection phits = 
	  associatorByHits->associateRecoToSim(recTrks[0],TPCollectionH,&iEvent );
	reco::RecoToSimCollection pchi2 =
		associatorByChi2->associateRecoToSim(recTrks[0],TPCollectionH,&iEvent );
*/	
	int btagCollsize = btagColl.size();
	int ijet = 0;
	bool muonfound = false;
	for( jet = recoJets.begin(); jet != recoJets.end(); ++jet ) {

		int tmptotmuon = 0;
		
		for ( muon = recoMuons.begin(); muon != recoMuons.end(); ++muon) {

			Track muonTrk = *muon->track();

			
			TrackingParticleRef TrueHitsTrk;
			/*
			try{
				std::vector<std::pair<TrackingParticleRef, double> > trkTrueVec = phits[muon->track()];
				for (std::vector<std::pair<TrackingParticleRef, double> >::const_iterator it = trkTrueVec.begin(); it != trkTrueVec.end(); ++it) {
					TrueHitsTrk = it->first;
					double assocChi2 = it->second;
					
					fMySummary[ispl]->muon_chi2_hits_mc.push_back(assocChi2);
					fMySummary[ispl]->muon_pt_hits_mc.push_back( (*TrueHitsTrk).pt() );
					fMySummary[ispl]->muon_eta_hits_mc.push_back((*TrueHitsTrk).eta());
					fMySummary[ispl]->muon_phi_hits_mc.push_back((*TrueHitsTrk).phi());
					fMySummary[ispl]->muon_charge_hits_mc.push_back((*TrueHitsTrk).charge());
					fMySummary[ispl]->muon_p_hits_mc.push_back((*TrueHitsTrk).p());
					fMySummary[ispl]->muon_pdgid_hits_mc.push_back( (*TrueHitsTrk).pdgId() );
				}
			}
			catch (Exception iEvent) {
				std::cout << " track matched by hits to 0 MC tracks" << std::endl;
			}
			
			
			try{
				std::vector<std::pair<TrackingParticleRef, double> > trkTrueVec = pchi2[muon->track()];
				for (std::vector<std::pair<TrackingParticleRef, double> >::const_iterator it = trkTrueVec.begin(); it != trkTrueVec.end(); ++it) {
					TrueHitsTrk = it->first;
					double assocChi2 = it->second;
					
					fMySummary[ispl]->muon_chi2_chi2_mc.push_back(assocChi2);
					fMySummary[ispl]->muon_pt_chi2_mc.push_back( (*TrueHitsTrk).pt() );
					fMySummary[ispl]->muon_eta_chi2_mc.push_back((*TrueHitsTrk).eta());
					fMySummary[ispl]->muon_phi_chi2_mc.push_back((*TrueHitsTrk).phi());
					fMySummary[ispl]->muon_charge_chi2_mc.push_back((*TrueHitsTrk).charge());
					fMySummary[ispl]->muon_p_chi2_mc.push_back((*TrueHitsTrk).p());
					fMySummary[ispl]->muon_pdgid_chi2_mc.push_back( (*TrueHitsTrk).pdgId() );
				}
			}
			catch (Exception iEvent) {
				std::cout << " track matched by chi2 to 0 MC tracks" << std::endl;
			}
			*/
			
			double delta  = ROOT::Math::VectorUtil::DeltaR(jet->p4().Vect(), muonTrk.momentum() );//muon->momentum());

			if ( (delta > conesize_) || muon->pt()<4 ) continue;

			muonfound = true;

			fMySummary[ispl]->jet_p.push_back(jet->p());
			fMySummary[ispl]->jet_pt.push_back(jet->pt());
			fMySummary[ispl]->jet_eta.push_back(jet->eta());
			fMySummary[ispl]->jet_phi.push_back(jet->phi());
			fMySummary[ispl]->jet_et.push_back(jet->et());
			fMySummary[ispl]->jet_vx.push_back(jet->vx());
			fMySummary[ispl]->jet_vy.push_back(jet->vy());
			fMySummary[ispl]->jet_vz.push_back(jet->vz());
						
            // find generated jet
			reco::GenJet genjet = this->GetGenJet(*jet,genJets);
			fMySummary[ispl]->genjet_p.push_back(genjet.p());
			fMySummary[ispl]->genjet_pt.push_back(genjet.pt());
			fMySummary[ispl]->genjet_eta.push_back(genjet.eta());
			fMySummary[ispl]->genjet_phi.push_back(genjet.phi());
			fMySummary[ispl]->genjet_et.push_back(genjet.et());
			fMySummary[ispl]->genjet_vx.push_back(genjet.vx());
            fMySummary[ispl]->genjet_vy.push_back(genjet.vy());
            fMySummary[ispl]->genjet_vz.push_back(genjet.vz());

				
			fMySummary[ispl]->jet_deltar.push_back(delta);
			
			fMySummary[ispl]->muon_pt.push_back(muonTrk.pt());
			fMySummary[ispl]->muon_eta.push_back(muonTrk.eta());
			fMySummary[ispl]->muon_phi.push_back(muonTrk.phi());
			fMySummary[ispl]->muon_charge.push_back(muonTrk.charge());
			fMySummary[ispl]->muon_p.push_back(muonTrk.p());
			fMySummary[ispl]->muon_trkchi2.push_back(muonTrk.chi2());
			fMySummary[ispl]->muon_trkndof.push_back(muonTrk.ndof());
			fMySummary[ispl]->muon_chi2.push_back( (*(muon->combinedMuon())).chi2() );
			fMySummary[ispl]->muon_ndof.push_back( (*(muon->combinedMuon())).ndof() );
			Track muonSA = *muon->standAloneMuon();
			int nhit = muonSA.recHitsSize();
			fMySummary[ispl]->muon_SArechits.push_back( nhit );
			nhit = muonTrk.recHitsSize();
			fMySummary[ispl]->muon_trkrechits.push_back( nhit );
			fMySummary[ispl]->muon_d0.push_back(muonTrk.d0());
			fMySummary[ispl]->muon_d0sigma.push_back(muonTrk.d0Error());
			fMySummary[ispl]->muon_vx.push_back(muonTrk.vx());
			fMySummary[ispl]->muon_vy.push_back(muonTrk.vy());
			fMySummary[ispl]->muon_vz.push_back(muonTrk.vz());
			
			// muon ECAL
			//const reco::SuperCluster & muon_sc = *(muon->superCluster() );
			//math::XYZPoint v( 0, 0, 0 ); // this should be taken from something else...
			//math::XYZVector p = muon_sc.energy() * ( muon_sc.position() - v ).unit();
			//double mu_ecal_e = sqrt( 0.105658*0.105658 + p.mag2() ); // using muon mass
			//std::cout << "muon ecal e= " << mu_ecal_e << " px = " << p.x() << " py = " << p.y() << " pz = " << p.z() << std::endl;
			
			// find a sim track
			SimTrack genmuon = this->GetGenTrk(muonTrk, simTrks, simVtcs );
			fMySummary[ispl]->muon_p_chi2_mc.push_back( genmuon.momentum().vect().mag());
			fMySummary[ispl]->muon_pt_chi2_mc.push_back( genmuon.momentum().perp() );
			fMySummary[ispl]->muon_phi_chi2_mc.push_back( genmuon.momentum().phi() );
			fMySummary[ispl]->muon_eta_chi2_mc.push_back( genmuon.momentum().pseudoRapidity() );
			fMySummary[ispl]->muon_charge_chi2_mc.push_back( genmuon.charge() );
			fMySummary[ispl]->muon_vx_chi2_mc.push_back(genmuon.momentum().x());
                        fMySummary[ispl]->muon_vy_chi2_mc.push_back(genmuon.momentum().y());
                        fMySummary[ispl]->muon_vz_chi2_mc.push_back(genmuon.momentum().z());
			fMySummary[ispl]->muon_pdgid_chi2_mc.push_back( genmuon.type() );

			fMySummary[ispl]->muon_mother_pdgid_chi2_mc.push_back( GetMotherId(simVtcs, simTrks, genmuon));

			TVector3 tmpvec(jet->p4().Vect().X(),jet->p4().Vect().Y(),  jet->p4().Vect().Z());
			TVector3 muonvec(muonTrk.momentum().X(), muonTrk.momentum().Y(),muonTrk.momentum().Z());
			
			tmpvec += muonvec;

			double ptrel = muonvec.Perp(tmpvec);

			// cross-check ptrel
			//std::cout << "ptrel = " << ptrel << std::endl;
			//double tmpdot = (muonvec*tmpvec)/tmpvec.Mag();
			//std::cout << "check = " << sqrt(muon->p()*muon->p() - tmpdot*tmpdot ) << std::endl;

			fMySummary[ispl]->jet_ptrel.push_back(ptrel);
			fMySummary[ispl]->jet_flavour_alg.push_back(jetFlavourIdentifier_.identifyBasedOnPartons(*jet).flavour());	
			fMySummary[ispl]->jet_flavour_phy.push_back(jetFlavourIdentifier2_.identifyBasedOnPartons(*jet).flavour());	

			CaloJet corrjet = recoCorrJets[ijet];
			fMySummary[ispl]->jetcorr_p.push_back(corrjet.p());
			fMySummary[ispl]->jetcorr_pt.push_back(corrjet.pt());
			fMySummary[ispl]->jetcorr_et.push_back(corrjet.et());
			TVector3 tmpveccorr(corrjet.p4().Vect().X(),corrjet.p4().Vect().Y(), corrjet.p4().Vect().Z());
			//TVector3 muonveccorr(muonTrk.momentum().X(), muonTrk.momentum().Y(),muonTrk.momentum().Z());
			tmpveccorr += muonvec;
			double ptrelcorr = muonvec.Perp(tmpveccorr);
			fMySummary[ispl]->jetcorr_ptrel.push_back(ptrelcorr);
			
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
		ijet++;
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
