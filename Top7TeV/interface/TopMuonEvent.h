#ifndef TopMuonEvent_h
#define TopMuonEvent_h

/**_________________________________________________________________
   class:   TopMuonEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopMuonEvent.h,v 1.3 2009/07/30 06:02:19 jengbou Exp $

________________________________________________________________**/


#ifdef NOSCRAMV
#include "TopBaseEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopBaseEvent.h"
#endif


class TopMuonEvent : public TopBaseEvent
{

public:

    TopMuonEvent() : TopBaseEvent()
    {
        Reset();
    }
        ~TopMuonEvent() {}

    virtual void Reset();
	
    //___
    Int_t normchi2;
    Int_t isTracker;
    Float_t d0;
    Float_t d0err;
    Float_t d0wrtPV2d;
    Float_t d0wrtPV2derr;

    ClassDef(TopMuonEvent,1);

};

#endif

