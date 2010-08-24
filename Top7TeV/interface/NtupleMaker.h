// -*- C++ -*-
//
// Package:    NtupleMaker
// Class:      NtupleMaker
// 
/**\class NtupleMaker NtupleMaker.cc Exercise/NtupleMaker/src/NtupleMaker.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  "Jian Wang"
//        Modified:  Samvel Khalatian
//         Created:  Fri Jun 11 12:14:21 CDT 2010
// $Id: NtupleMaker.h,v 1.1 2010/08/18 16:28:42 yumiceva Exp $
//
//

#ifndef TOP_EDANALYZERS_NtupleMaker
#define TOP_EDANALYZERS_NtupleMaker

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "RecoJets/JetAlgorithms/interface/JetIDHelper.h"
#include "Yumiceva/Top7TeV/interface/TopEventNtuple.h"

#include "TFile.h"
#include "TTree.h"

class TH1;

//
// class declaration
//

class NtupleMaker : public edm::EDFilter {

public:
  explicit NtupleMaker(const edm::ParameterSet&);
  ~NtupleMaker();

private:
  virtual void beginJob() ;
  virtual bool filter( edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------
  TFile *theFile;
  TTree *ftree;
  
  edm::TriggerNames hltNames_;
  helper::JetIDHelper *jetID;
  edm::InputTag hltTag_;
  
  TH1 *_cutflow;
  bool _isDataInput;
  TopEventNtuple *_ntuple;
};

#endif
