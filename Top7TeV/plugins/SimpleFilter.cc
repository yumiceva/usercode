// -*- C++ -*-
//
// Package:    SimpleFilter
// Class:      SimpleFilter
// 
/**\class SimpleFilter SimpleFilter.cc Exercise/SimpleFilter/src/SimpleFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  "Jian Wang"
//        Modified:  Samvel Khalatian
//         Created:  Fri Jun 11 12:14:21 CDT 2010
// $Id: SimpleFilter.h,v 1.1 2010/08/18 16:28:42 yumiceva Exp $
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "RecoJets/JetAlgorithms/interface/JetIDHelper.h"
#include "FWCore/Framework/interface/MakerMacros.h"

//
// class declaration
//
using namespace std;
using namespace edm;
using namespace reco;


class SimpleFilter : public edm::EDFilter {
public:
  explicit SimpleFilter(const edm::ParameterSet&);
  ~SimpleFilter();

private:
  virtual void beginJob() ;
  virtual bool filter( edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  helper::JetIDHelper *jetID;

};


SimpleFilter::SimpleFilter(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  jetID = new helper::JetIDHelper(iConfig.getParameter<ParameterSet>("JetIDParams"));

}


SimpleFilter::~SimpleFilter()
{

}


// ------------ method called to for each event  ------------
bool
SimpleFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  bool pass_event = false;

  // -[ Jets ]-
  Handle<CaloJetCollection> jets;
  iEvent.getByLabel("ak5CaloJetsL2L3",jets);

  // Jets
  int njets = 0;
  for(CaloJetCollection::const_iterator jet = jets->begin(); jet != jets->end(); ++jet)
    {
      jetID->calculate(iEvent, *jet);
      if (jet->pt()>30.
	  &&abs(jet->eta())<2.4)
	  //&&jet->emEnergyFraction()>0.01
	  //&&jetID->n90Hits()>1
	  //&&jetID->fHPD()<0.98)
        {
	  
	  ++njets;
        }
    }

  if (njets>=4) pass_event = true;

  return pass_event;

}

// ------------ method called once each job just before starting event loop  ------------
void 
SimpleFilter::beginJob()
{

}

// ------------ method called once each job just after ending the event loop  ------------
void 
SimpleFilter::endJob() {

}

//define this as a plug-in
DEFINE_FWK_MODULE(SimpleFilter);
