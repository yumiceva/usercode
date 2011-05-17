// -*- C++ -*-
//
// Package:    PATNtupleMaker
// Class:      PATNtupleMaker
// 
/**\class PATNtupleMaker PATNtupleMaker.cc Exercise/PATNtupleMaker/src/PATNtupleMaker.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Francisco Yumiceva, Fermilab
//         Created:  Fri Jun 11 12:14:21 CDT 2010
// $Id: PATNtupleMaker.h,v 1.5 2011/05/16 19:43:37 yumiceva Exp $
//
//

#ifndef TOP_EDANALYZERS_PATNtupleMaker
#define TOP_EDANALYZERS_PATNtupleMaker

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"

#include "Yumiceva/Top7TeV/interface/TopEventNtuple.h"

#include "TFile.h"
#include "TTree.h"

class TH1;

//
// class declaration
//

class PATNtupleMaker : public edm::EDFilter 
{
public:
  explicit PATNtupleMaker(const edm::ParameterSet&);
  ~PATNtupleMaker();

private:
  virtual void beginJob() ;
  virtual bool filter( edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  // ----------member data ---------------------------
  JetIDSelectionFunctor                jetIdLoose_;
  PFJetIDSelectionFunctor              pfjetIdLoose_;

  TFile *theFile;
  TTree *ftree;
  std::string ntuplefile_;

  edm::InputTag hltTag_;
  std::vector< std::string > hltList_;
  edm::InputTag muonTag_;
  edm::InputTag PVTag_;
  edm::InputTag electronTag_;
  edm::InputTag PFelectronTag_;
  edm::InputTag PFjetTag_;
  edm::InputTag RhojetTag_;
  edm::InputTag PFMETTag_;
  
  TH1 *_cutflow;
  bool isDataInput_;
  bool verbose_;
  bool applyTrigger_;
  bool storeTrigger_;

  TopEventNtuple *_ntuple;

};

#endif
