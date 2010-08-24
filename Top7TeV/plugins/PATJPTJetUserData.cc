// -*- C++ -*-
//
// Package:    PATJPTJetUserData
// Class:      PATJPTJetUserData
// 
/**\class PATJPTJetUserData PATJPTJetUserData.cc User/PATJPTJetUserData/src/PATJPTJetUserData.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Francisco Yumiceva
//         Created:  Tue Mar 17 15:19:19 EDT 2009
// $Id: PATJPTJetUserData.cc,v 1.1 2010/08/24 22:04:40 yumiceva Exp $
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

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/JPTJetCollection.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/JetID.h"

#include "DataFormats/Math/interface/deltaR.h"
//
// class decleration
//

class PATJPTJetUserData : public edm::EDProducer {
   public:
      explicit PATJPTJetUserData(const edm::ParameterSet&);
      ~PATJPTJetUserData();

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
PATJPTJetUserData::PATJPTJetUserData(const edm::ParameterSet& iConfig):
  src_(iConfig.getParameter<edm::InputTag>("src"))
{
  produces<std::vector<pat::Jet> >();
}


PATJPTJetUserData::~PATJPTJetUserData()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
PATJPTJetUserData::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(src_,jets);

  edm::Handle<reco::JPTJetCollection> jptjets;
  iEvent.getByLabel( "ak5JPTJetsL2L3", jptjets);

  edm::Handle<edm::ValueMap<reco::JetID> > jetsID;
  iEvent.getByLabel("ak5JetID",jetsID);

  std::auto_ptr<std::vector<pat::Jet> > output(new std::vector<pat::Jet>());

  for (edm::View<pat::Jet>::const_iterator jet = jets->begin(), end = jets->end(); jet != end; ++jet) {

    pat::Jet myJet = *jet; // copy

    //float DeltaR = 3.;
    float emEnergyFraction = 0.;
    int n90 = 0;
    float fhpd = 0.;

    for(reco::JPTJetCollection::const_iterator jptjet = jptjets->begin(); jptjet != jptjets->end(); ++jptjet) {

      double dr = deltaR(myJet.eta(), myJet.phi(), jptjet->eta(), jptjet->phi());
      if (dr< 0.01) {
	edm::RefToBase<reco::Jet>  jetRef = jptjet->getCaloJetRef();
	reco::CaloJet const * rawcalojet = dynamic_cast<reco::CaloJet const *>( &* jetRef);

	emEnergyFraction = rawcalojet->emEnergyFraction();
	
	fhpd = (*jetsID)[jetRef].fHPD;
	n90 = (*jetsID)[jetRef].n90Hits;

	break;
      }
    }
      
    
    myJet.addUserFloat("MyemEnergyFraction", emEnergyFraction);
    myJet.addUserFloat("Myn90", n90);
    myJet.addUserFloat("MyfHPD", fhpd);

    output->push_back(myJet);
  }
  iEvent.put(output); 
}

// ------------ method called once each job just before starting event loop  ------------
void 
PATJPTJetUserData::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PATJPTJetUserData::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(PATJPTJetUserData);
