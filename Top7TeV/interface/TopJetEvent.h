#ifndef TopJetEvent_h
#define TopJetEvent_h

/**_________________________________________________________________
   class:   TopJetEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopJetEvent.h,v 1.3 2009/07/30 06:02:19 jengbou Exp $

________________________________________________________________**/


#ifdef NOSCRAMV
#include "TopBaseEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopBaseEvent.h"
#endif


class TopJetEvent : public TopBaseEvent
{

public:

    TopJetEvent() : TopBaseEvent()
    {
        Reset();
    }
        ~TopJetEvent() {}

    virtual void Reset();
	
    //___

    ClassDef(TopJetEvent,1);

};

#endif

