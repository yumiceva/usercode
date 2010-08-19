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
// Original Author:  "Jian Wang"
//        Modified:  Samvel Khalatian
//         Created:  Fri Jun 11 12:14:21 CDT 2010
// $Id: PATNtupleMaker.h,v 1.1 2010/08/18 16:28:42 yumiceva Exp $
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
#include "FWCore/Common/interface/TriggerNames.h"
//#include "RecoJets/JetAlgorithms/interface/JetIDHelper.h"
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
  TFile *theFile;
  TTree *ftree;

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
