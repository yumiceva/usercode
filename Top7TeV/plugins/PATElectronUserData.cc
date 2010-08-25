// -*- C++ -*-
//
// Package:    PATElectronUserData
// Class:      PATElectronUserData
// 
/**\class PATElectronUserData PATElectronUserData.cc User/PATElectronUserData/src/PATElectronUserData.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Malina Kirn
//         Created:  Tue Mar 17 15:19:19 EDT 2009
// $Id: PATElectronUserData.cc,v 1.5 2010/06/28 19:23:45 mkirn Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

//
// class decleration
//

class PATElectronUserData : public edm::EDProducer {
   public:
      explicit PATElectronUserData(const edm::ParameterSet&);
      ~PATElectronUserData();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      edm::InputTag src_;
      edm::InputTag ecalRecHitsEBsrc_;
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
PATElectronUserData::PATElectronUserData(const edm::ParameterSet& iConfig):
  src_(iConfig.getParameter<edm::InputTag>("src")),
  ecalRecHitsEBsrc_(iConfig.getParameter<edm::InputTag>("ecalRecHitsEBsrc"))
{
  produces<std::vector<pat::Electron> >();
}


PATElectronUserData::~PATElectronUserData()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
PATElectronUserData::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<edm::View<pat::Electron> > electrons;
  iEvent.getByLabel(src_,electrons);

  // For 'swiss cross' spike cleaning
  edm::Handle<EcalRecHitCollection> recHits;
  iEvent.getByLabel(ecalRecHitsEBsrc_, recHits);
  const EcalRecHitCollection *myRecHits = recHits.product();

  std::auto_ptr<std::vector<pat::Electron> > output(new std::vector<pat::Electron>());
  for (edm::View<pat::Electron>::const_iterator electron = electrons->begin(), end = electrons->end(); electron != end; ++electron) {
    pat::Electron myElectron = *electron; // copy
    //float combRelIso = (electron->trackIso()+electron->caloIso())/electron->et();
    float combRelIso = (electron->dr03TkSumPt()+electron->dr03EcalRecHitSumEt()+electron->dr03HcalTowerSumEt())/electron->et();
    myElectron.addUserFloat("CombRelIso", combRelIso);

    const reco::CaloClusterPtr seed = electron->superCluster()->seed(); // seed cluster                                 
    DetId seedId = seed->seed();
    EcalSeverityLevelAlgo severity;
    float mySwissCross = severity.swissCross(seedId, *myRecHits);
    myElectron.addUserFloat("SwissCross", mySwissCross);

    output->push_back(myElectron);
  }
  iEvent.put(output); 
}

// ------------ method called once each job just before starting event loop  ------------
void 
PATElectronUserData::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PATElectronUserData::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(PATElectronUserData);
