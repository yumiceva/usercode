#ifndef MyHLTSummaryFilter_h
#define MyHLTSummaryFilter_h

/** \class MyHLTSummaryFilter
 *
 *  
 *  This class is an HLTFilter (-> EDFilter) implementing a smart HLT
 *  trigger cut, specified as a string such as "pt>15 && -3<eta<3",
 *  for objects in the TriggerSummaryAOD product, allowing to cut on
 *  variables relating to their 4-momentum representation
 *
 *  $Date: 2009/09/25 13:04:00 $
 *  $Revision: 1.1 $
 *
 *  \author Martin Grunewald
 *
 */

#include "HLTrigger/HLTcore/interface/HLTFilter.h"

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "TH1.h"
#include<string>

//
// class declaration
//

class MyHLTSummaryFilter : public HLTFilter {

   public:

      explicit MyHLTSummaryFilter(const edm::ParameterSet&);
      ~MyHLTSummaryFilter();
      virtual bool filter(edm::Event&, const edm::EventSetup&);

   private:
      edm::InputTag summaryTag_; // input tag identifying TriggerSummaryAOD
      edm::InputTag memberTag_;  // which packed-up collection or filter
      std::string   cut_;        // smart cut
      int           min_N_;      // number of objects passing cuts required

      StringCutObjectSelector<trigger::TriggerObject> select_; // smart selector
      TH1F *eventCount;
};

#endif //MyHLTSummaryFilter_h
