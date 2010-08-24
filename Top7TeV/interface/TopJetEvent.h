#ifndef TopJetEvent_h
#define TopJetEvent_h

/**_________________________________________________________________
   class:   TopJetEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopJetEvent.h,v 1.3 2010/08/23 01:48:56 yumiceva Exp $

________________________________________________________________**/


#ifdef NOSCRAMV
#include "TopCandidateEvent.h"
#include "TopGenJetEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopCandidateEvent.h"
#include "Yumiceva/Top7TeV/interface/TopGenJetEvent.h"
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
    Int_t ntracks;
    Int_t ndaughters;

    //___
    Float_t btag_TCHE;
    Float_t btag_TCHP;
    Float_t btag_SSVHE;
    Float_t btag_SSVHP;

    //___ MC
    TopGenJetEvent mc;

    ClassDef(TopJetEvent,1);

};

#endif

