#ifndef TopGenMuonEvent_h
#define TopGenMuonEvent_h

/**_________________________________________________________________
   class:   TopGenMuonEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopGenMuonEvent.h,v 1.2 2010/08/18 21:13:05 yumiceva Exp $

________________________________________________________________**/


#ifdef NOSCRAMV
#include "TopGenCandidateEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopGenCandidateEvent.h"
#endif


class TopGenMuonEvent : public TopGenCandidateEvent
{

public:

  TopGenMuonEvent() : TopGenCandidateEvent()
    {
      Reset();
    }
    ~TopGenMuonEvent() {}

    virtual void Reset();
	
    //___
 
    ClassDef(TopGenMuonEvent,1);

};

#endif

