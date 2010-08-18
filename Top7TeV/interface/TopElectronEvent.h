#ifndef TopElectronEvent_h
#define TopElectronEvent_h

/**_________________________________________________________________
   class:   TopElectronEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopElectronEvent.h,v 1.3 2009/07/30 06:02:19 jengbou Exp $

________________________________________________________________**/


#ifdef NOSCRAMV
#include "TopBaseEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopBaseEvent.h"
#endif


class TopElectronEvent : public TopBaseEvent
{

public:

    TopElectronEvent() : TopBaseEvent()
    {
        Reset();
    }
        ~TopElectronEvent() {}

    virtual void Reset();

    //___
    Int_t normchi2;
    Int_t isTracker;
    Float_t d0;
    Float_t d0err;
    Float_t d0wrtPV2d;
    Float_t d0wrtPV2derr;

    ClassDef(TopElectronEvent,1);

};

#endif

