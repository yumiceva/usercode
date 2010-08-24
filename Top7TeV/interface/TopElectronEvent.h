#ifndef TopElectronEvent_h
#define TopElectronEvent_h

/**_________________________________________________________________
   class:   TopElectronEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopElectronEvent.h,v 1.2 2010/08/18 21:13:05 yumiceva Exp $

________________________________________________________________**/


#ifdef NOSCRAMV
#include "TopCandidateEvent.h"
#include "TopGenElectronEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopCandidateEvent.h"
#include "Yumiceva/Top7TeV/interface/TopGenElectronEvent.h"
#endif


class TopElectronEvent : public TopCandidateEvent
{

 public:

  TopElectronEvent() : TopCandidateEvent()
    {
      Reset();
    }
    ~TopElectronEvent() {}

    virtual void Reset();

    //___
    Float_t d0;
    Float_t d0err;
    Float_t d0wrtPV2d;
    Float_t d0wrtPV2derr;

    Float_t reliso03;

    //___ MC
    TopGenElectronEvent mc;

    ClassDef(TopElectronEvent,1);

};

#endif

