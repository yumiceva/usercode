#ifndef TopJetEvent_h
#define TopJetEvent_h

/**_________________________________________________________________
   class:   TopJetEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopJetEvent.h,v 1.7 2011/05/16 19:43:37 yumiceva Exp $

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
    Float_t id_neutralEmE;
    Float_t id_chargedEmE;
    Float_t id_muonMultiplicity;
    
    //___
    Int_t ntracks;
    Int_t nSVs;
    Int_t ndaughters;
    Float_t jetArea;

    //___
    Float_t btag_TCHE;
    Float_t btag_TCHP;
    Float_t btag_SSVHE;
    Float_t btag_SSVHP;
    Float_t btag_CSV;

    //___ MC
    TopGenJetEvent mc;

    ClassDef(TopJetEvent,1);

};

#endif

