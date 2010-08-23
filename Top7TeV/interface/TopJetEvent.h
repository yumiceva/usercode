#ifndef TopJetEvent_h
#define TopJetEvent_h

/**_________________________________________________________________
   class:   TopJetEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopJetEvent.h,v 1.2 2010/08/18 21:13:05 yumiceva Exp $

________________________________________________________________**/


#ifdef NOSCRAMV
#include "TopCandidateEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopCandidateEvent.h"
#endif


class TopJetEvent : public TopCandidateEvent
{

public:

    TopJetEvent() : TopCandidateEvent()
    {
        Reset();
    }
        ~TopJetEvent() {}

    virtual void Reset();
	
    //___
    Float_t btag_TCHE;
    Float_t btag_TCHP;
    Float_t btag_SSVHE;
    Float_t btag_SSVHP;

    ClassDef(TopJetEvent,1);

};

#endif

