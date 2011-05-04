#ifndef TopElectronEvent_h
#define TopElectronEvent_h

/**_________________________________________________________________
   class:   TopElectronEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopElectronEvent.h,v 1.5 2010/10/01 23:06:37 yumiceva Exp $

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
    
    Int_t IsTight;
    Int_t pass70;
    Int_t pass95;
    Int_t missingHits;
    Int_t convDist;
    Int_t convDcot;
    Int_t IsConversion;
    Float_t etasc;
    Float_t deltaetasc;
    Float_t deltaphisc;
    Float_t sigmaIetaIeta;
    Float_t HoE;

    //___ MC
    TopGenElectronEvent mc;

    ClassDef(TopElectronEvent,1);

};

#endif

