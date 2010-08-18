#ifndef TopMuonEvent_h
#define TopMuonEvent_h

/**_________________________________________________________________
   class:   TopMuonEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopMuonEvent.h,v 1.1 2010/08/18 16:28:42 yumiceva Exp $

________________________________________________________________**/


#ifdef NOSCRAMV
#include "TopCandidateEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopCandidateEvent.h"
#endif


class TopMuonEvent : public TopCandidateEvent
{

public:

    TopMuonEvent() : TopCandidateEvent()
    {
        Reset();
    }
        ~TopMuonEvent() {}

    virtual void Reset();
	
    //___
    Int_t IsTrackerMuon;
    Int_t IsLooseIsoMuon;
    Int_t IsTightIsoMuon;

    Float_t d0;
    Float_t d0err;
    Float_t d0wrtPV2d;
    Float_t d0wrtPV2derr;

    Int_t muonhits;
    Int_t trackerhits;
    Int_t muonstations;
    Int_t normchi2;
    
    Float_t iso03_track;
    Float_t iso03_ecal;
    Float_t iso03_hcal;
    Float_t iso03_ecalveto;
    Float_t iso03_hcalveto;
    Float_t reliso03;
    Float_t deltaR;


    ClassDef(TopMuonEvent,1);

};

#endif

