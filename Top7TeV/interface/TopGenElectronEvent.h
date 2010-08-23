#ifndef TopGenElectronEvent_h
#define TopGenElectronEvent_h

/**_________________________________________________________________
   class:   TopGenElectronEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopGenElectronEvent.h,v 1.2 2010/08/18 21:13:05 yumiceva Exp $

________________________________________________________________**/


#ifdef NOSCRAMV
#include "TopGenCandidateEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopGenCandidateEvent.h"
#endif


class TopGenElectronEvent : public TopGenCandidateEvent
{

public:

  TopGenElectronEvent() : TopGenCandidateEvent()
    {
      Reset();
    }
    ~TopGenElectronEvent() {}

    virtual void Reset();
	
    //___


    ClassDef(TopGenElectronEvent,1);

};

#endif

