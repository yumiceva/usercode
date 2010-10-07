/** \class MyHLTSummaryFilter
 *
 * See header file for documentation
 *
 *  $Date: 2010/10/01 23:07:15 $
 *  $Revision: 1.1 $
 *
 *  \author Martin Grunewald
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Yumiceva/Top7TeV/interface/MyHLTSummaryFilter.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

//
// constructors and destructor
//
MyHLTSummaryFilter::MyHLTSummaryFilter(const edm::ParameterSet& iConfig) :
  summaryTag_ (iConfig.getParameter<edm::InputTag>("summary")),
  memberTag_  (iConfig.getParameter<edm::InputTag>("member" )),
  cut_        (iConfig.getParameter<std::string>  ("cut"    )),
  min_N_      (iConfig.getParameter<int>          ("minN"   )),
  select_     (cut_                                          )
{
  edm::LogInfo("MyHLTSummaryFilter")
     << "Summary/member/cut/ncut : "
     << summaryTag_.encode() << " "
     << memberTag_.encode() << " " 
     << cut_<< " " << min_N_ ;

   //register your products
   //produces<trigger::TriggerFilterObjectWithRefs>();

  edm::Service<TFileService> fs;
  eventCount = fs->make<TH1F>("eventCount" , "Number of events" , 2, 0, 2 );

}

MyHLTSummaryFilter::~MyHLTSummaryFilter()
{
}

//
// member functions
//

// ------------ method called to produce the data  ------------
bool
MyHLTSummaryFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace std;
   using namespace edm;
   using namespace reco;
   using namespace trigger;

   eventCount->Fill(0);

   Handle<TriggerEvent> summary;
   iEvent.getByLabel(summaryTag_,summary);

   if (!summary.isValid()) {
     LogError("MyHLTSummaryFilter") << "Trigger summary product " 
				  << summaryTag_.encode() 
				  << " not found! Filter returns false always";
     return false;
   }

   size_type n(0);
   size_type index(0);

   // check if we want to look at a filter and its passing physics objects
   index=summary->filterIndex(memberTag_);
   if (index<summary->sizeFilters()) {
     const Keys& KEYS (summary->filterKeys(index));
     const size_type n1(KEYS.size());
     for (size_type i=0; i!=n1; ++i) {
       const TriggerObject& TO( summary->getObjects().at(KEYS[i]) );
       if (select_(TO)) n++;
     }
     const bool accept(n>=min_N_);
     LogInfo("MyHLTSummaryFilter")
       << " Filter objects: " << n << "/" << n1;
     if (accept) eventCount->Fill( 1 );
     return accept;
   }
   
   // check if we want to cut on all physics objects of a full "L3" collection
   index=summary->collectionIndex(memberTag_);
   if (index<summary->sizeCollections()) {
     const Keys& KEYS (summary->collectionKeys());
     const size_type n0 (index == 0? 0 : KEYS.at(index-1));
     const size_type n1 (KEYS.at(index));
     for (size_type i=n0; i!=n1; ++i) {
       const TriggerObject& TO( summary->getObjects().at(i) );
       if (select_(TO)) n++;
     }
     const bool accept(n>=min_N_);
     LogInfo("MyHLTSummaryFilter")
       << " Collection objects: " << n << "/" <<n1-n0;
     if (accept) eventCount->Fill( 1 );
     return accept;
   }

   // can't help you, bailing out!
   const bool accept (false);
   LogInfo("MyHLTSummaryFilter") << " Default decision: " << accept;

   if (accept) eventCount->Fill( 1 );

   return accept;

}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MyHLTSummaryFilter);
