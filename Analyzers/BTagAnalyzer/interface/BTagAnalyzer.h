#ifndef BTagAnalyzer_h
#define BTagAnalyzer_h
/** \class edm::EDAnalyzer BTagAnalysis
 *  
 * Analyzer to select jets together with a muon on it.
 *
 * \author Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)
 *
 * \version $Id: BTagAnalyzer.h,v 1.2 2007/01/22 04:48:40 yumiceva Exp $
 *
 */

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

// event class
#include "Analyzers/BTagAnalyzer/interface/BTagSummary.h"


TrackAssociatorBase * associatorByHits;

// class declaration

class BTagAnalyzer : public edm::EDAnalyzer {


public:
  explicit BTagAnalyzer(const edm::ParameterSet&);
  ~BTagAnalyzer();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginJob(edm::EventSetup const&);
  virtual void endJob();
  
  reco::GenJet GetGenJet(reco::CaloJet calojet, reco::GenJetCollection genJetColl);
  SimTrack GetGenTrk(reco::Track atrack, edm::SimTrackContainer simTrkColl, edm::SimVertexContainer simVtcs);
  int GetMotherId(edm::SimVertexContainer simVtxColl, edm::SimTrackContainer simTrkColl, SimTrack muonMC);
  
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
  std::string CorrCaloJetCollectionTags_;
  std::string GenJetCollectionTags_;
  std::string SimTrkCollectionTags_;
  
  JetFlavourIdentifier jetFlavourIdentifier_;
  JetFlavourIdentifier jetFlavourIdentifier2_;

  double conesize_;
  
  TFile*  rootFile_;             
  bool verbose_;
  edm::InputTag simG4_;
  double simUnit_;               

  //std::map<std::string, TH1*> h;

  TTree *ftree;
  BTagSummary *fMySummary[3];

  int feventcounter;

  TrackAssociatorBase *associatorByChi2;
  TrackAssociatorByHits *associatorByHits;
  
};

#endif
