// -*- C++ -*-
//
// Package:    PATElectronNtupleMaker
// Class:      PATElectronNtupleMaker
// 
/**\class PATElectronNtupleMaker PATElectronNtupleMaker.cc Exercise/PATElectronNtupleMaker/src/PATElectronNtupleMaker.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  "Jian Wang"
//        Modified:  Samvel Khalatian
//         Created:  Fri Jun 11 12:14:21 CDT 2010
// $Id: PATElectronNtupleMaker.h,v 1.3 2010/11/03 17:36:15 yumiceva Exp $
//
//

#ifndef TOP_EDANALYZERS_PATElectronNtupleMaker
#define TOP_EDANALYZERS_PATElectronNtupleMaker

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Common/interface/TriggerNames.h"
//#include "RecoJets/JetAlgorithms/interface/JetIDHelper.h"
#include "Yumiceva/Top7TeV/interface/TopEventNtuple.h"

#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"


#include "TFile.h"
#include "TTree.h"

class TH1;

//
// class declaration
//

class PATElectronNtupleMaker : public edm::EDFilter 
{
public:
  explicit PATElectronNtupleMaker(const edm::ParameterSet&);
  ~PATElectronNtupleMaker();

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
  edm::InputTag muonTag_;
  edm::InputTag electronTag_;
  edm::InputTag PFelectronTag_;
  edm::InputTag calojetTag_;
  edm::InputTag JPTjetTag_;
  edm::InputTag PFjetTag_;
  edm::InputTag PFlowjetTag_;
  edm::InputTag caloMETTag_;
  edm::InputTag tcMETTag_;
  edm::InputTag PFMETTag_;
  edm::InputTag PFlowMETTag_;

  TH1 *_cutflow;
  bool _isDataInput;
  TopEventNtuple *_ntuple;

};

#endif
