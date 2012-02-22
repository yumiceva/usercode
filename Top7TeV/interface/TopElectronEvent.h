#ifndef TopElectronEvent_h
#define TopElectronEvent_h

/**_________________________________________________________________
   class:   TopElectronEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopElectronEvent.h,v 1.10 2011/11/07 16:53:18 yumiceva Exp $

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
    Float_t d0wrtBS2d;
    Float_t d0wrtBS2derr;
    Float_t d0err;
    Float_t d0wrtPV2d;
    Float_t d0wrtPV2derr;

    Float_t iso03_track;
    Float_t iso03_ecal;
    Float_t iso03_hcal;
    Float_t iso03_ecalveto;
    Float_t iso03_hcalveto;
    Float_t reliso03;
    Float_t pfreliso;
    Int_t pfconvertion;

    Int_t eidHyperTight1MC;
    Int_t eidLooseMC;
    Int_t pass70;
    Int_t pass95;
    Int_t missingHits;
    Float_t convDist;
    Float_t convDcot;
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

