// -*- C++ -*-
//
// Package:    TopHLTFilter
// Class:      TopHLTFilter
// 
/**\class TopHLTFilter TopHLTFilter.cc Exercise/TopHLTFilter/src/TopHLTFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  
// $Id: TopHLTFilter.cc,v 1.1 2010/08/23 01:49:17 yumiceva Exp $
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
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "TH1.h"

//
// class declaration
//
using namespace std;
using namespace edm;


class TopHLTFilter : public edm::EDFilter {
public:
  explicit TopHLTFilter(const edm::ParameterSet&);
  ~TopHLTFilter();

private:
  virtual void beginJob() ;
  virtual bool filter( edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  //___
  TH1F *eventCount;
  edm::TriggerNames hltNames_;
  edm::InputTag hltTag_;
};


TopHLTFilter::TopHLTFilter(const edm::ParameterSet& iConfig):
  hltTag_(iConfig.getParameter< InputTag >("hltTag"))
{
  //now do what ever initialization is needed

  edm::Service<TFileService> fs;
  eventCount = fs->make<TH1F>("eventCount" , "Number of events" , 2, 0, 2 );

}


TopHLTFilter::~TopHLTFilter()
{

}


// ------------ method called to for each event  ------------
bool
TopHLTFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  eventCount->Fill(0);

  bool pass_event = false;

  // -[ Trigger ]-                                                                                                            
  Handle<TriggerResults> hlt;
  iEvent.getByLabel(hltTag_,hlt);
  const TriggerNames & hltNames_ = iEvent.triggerNames(*hlt);

  //bool hlt_mu_ = false;
  for(size_t itrig = 0; itrig != hlt->size(); itrig++){
    string hltName = hltNames_.triggerName(itrig);
    if(hltName == "HLT_Mu9"&&hlt->accept(itrig))
      {
	//hlt_mu_ = true;
	pass_event = true;
	eventCount->Fill( 1 );
	break;
      }
  }

  
  return pass_event;

}

// ------------ method called once each job just before starting event loop  ------------
void 
TopHLTFilter::beginJob()
{

}

// ------------ method called once each job just after ending the event loop  ------------
void 
TopHLTFilter::endJob() {

}

//define this as a plug-in
DEFINE_FWK_MODULE(TopHLTFilter);
