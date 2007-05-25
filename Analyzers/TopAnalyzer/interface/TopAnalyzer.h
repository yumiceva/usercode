#ifndef TopAnalyzer_h
#define TopAnalyzer_h


// system include files
#include <memory>
#include <string>
#include <vector>
 
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//generator level + CLHEP
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "CLHEP/HepMC/GenEvent.h"
#include "CLHEP/HepMC/GenVertex.h"
#include "CLHEP/HepMC/GenParticle.h"

// vertex stuff
#include <DataFormats/VertexReco/interface/Vertex.h>
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

// simulated vertices,..., add <use name=SimDataFormats/Vertex> and <../Track>
#include <SimDataFormats/Vertex/interface/SimVertex.h>
#include <SimDataFormats/Vertex/interface/SimVertexContainer.h>
#include <SimDataFormats/Track/interface/SimTrack.h>
#include <SimDataFormats/Track/interface/SimTrackContainer.h>

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/BTauReco/interface/JetTagFwd.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "RecoBTag/MCTools/interface/JetFlavour.h"
#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"

#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"

// Root
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>
#include "Math/LorentzVector.h"

#include "Analyzers/TopEvent/interface/TopEvent.h"


TrackAssociatorBase * associatorByHits;

// class declaration

class TopAnalyzer : public edm::EDAnalyzer {


public:
  explicit TopAnalyzer(const edm::ParameterSet&);
  ~TopAnalyzer();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginJob(edm::EventSetup const&);
  virtual void endJob();
  
  reco::GenJet GetGenJet(reco::CaloJet calojet, reco::GenJetCollection genJetColl);
  SimTrack GetGenTrk(reco::Track atrack, edm::SimTrackContainer simTrkColl);

  bool GenFilter(const edm::Event&, const edm::EventSetup&);


private:
 
  // ----------member data ---------------------------
  std::string outputFile_;                    // output filename
  std::vector< std::string > recoTrackList_;  // list of track collections
  std::vector< std::string > recoVtxList_;    // list of vertex collections
  std::vector< std::string > bTaggerList_;    // list of b-tagggers
  std::vector< std::string > moduleLabel_;
  std::string JetTrackAssociatorTags_;
  std::string MuonCollectionTags_;
  std::string CaloJetCollectionTags_;
  std::string GenJetCollectionTags_;
  std::string SimTrkCollectionTags_;
  
  JetFlavourIdentifier jetFlavourIdentifier_;

  double conesize_;
  
  TFile*  rootFile_;             
  bool verbose_;
  edm::InputTag simG4_;
  double simUnit_;               

  //std::map<std::string, TH1*> h;

  TTree *ftree;
  TopEvent *fmyEvent;

  int fnEvent;
  int fnAccepted;
  int fnEvent_cut0;
  
  TrackAssociatorBase *associatorByChi2;
  TrackAssociatorByHits *associatorByHits;
  
};

#endif
