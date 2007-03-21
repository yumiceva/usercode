#ifndef BTagAnalyzer_h
#define BTagAnalyzer_h


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

#include "RecoBTag/MCTools/interface/JetFlavour.h"
#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"

// Root
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>

#include "Analyzers/BTagAnalyzer/interface/BTagSummary.h"


// class declaration

class BTagAnalyzer : public edm::EDAnalyzer {


public:
  explicit BTagAnalyzer(const edm::ParameterSet&);
  ~BTagAnalyzer();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginJob(edm::EventSetup const&);
  virtual void endJob();
  
private:
 
  // ----------member data ---------------------------
  std::string outputFile_;                   // output file
  std::vector< std::string > recoTrackList_; // list of track collections
  std::vector< std::string > recoVtxList_;   // list of vertex collections
  std::vector< std::string > TaggerList_;    // list of b-tagggers
  std::vector< std::string > moduleLabel_;
  std::string JetTrackAssociatorTags_;
  std::string MuonCollectionTags_;
  std::string CaloJetCollectionTags_;

  JetFlavourIdentifier jetFlavourIdentifier_;

  double conesize_;
  
  TFile*  rootFile_;             
  bool verbose_;
  edm::InputTag simG4_;
  double simUnit_;               

  //std::map<std::string, TH1*> h;

  TTree *ftree;
  BTagSummary *fMySummary[3];

  int feventcounter;
  
};

#endif
