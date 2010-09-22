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
// $Id: PATElectronNtupleMaker.h,v 1.2 2010/08/19 19:27:19 yumiceva Exp $
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
  TFile *theFile;
  TTree *ftree;
  std::string ntuplefile_;

  edm::TriggerNames hltNames_;
  edm::InputTag hltTag_;
  edm::InputTag muonTag_;
  edm::InputTag electronTag_;
  edm::InputTag jetTag_;
  edm::InputTag metTag_;

  TH1 *_cutflow;
  bool _isDataInput;
  TopEventNtuple *_ntuple;

};

#endif
