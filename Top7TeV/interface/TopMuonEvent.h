#ifndef TopMuonEvent_h
#define TopMuonEvent_h

/**_________________________________________________________________
   class:   TopMuonEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopMuonEvent.h,v 1.6 2011/10/31 15:10:14 yumiceva Exp $

________________________________________________________________**/


#ifdef NOSCRAMV
#include "TopCandidateEvent.h"
#include "TopGenMuonEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopCandidateEvent.h"
#include "Yumiceva/Top7TeV/interface/TopGenMuonEvent.h"
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
    Float_t d0wrtBS2d;
    Float_t d0wrtBS2derr;
    Float_t d0wrtPV2d;
    Float_t d0wrtPV2derr;

    Int_t muonhits;
    Int_t trackerhits;
    Int_t muonstations;
    Int_t muonstationsold;
    Float_t normchi2;
    Int_t pixelhits;

    Float_t iso03_track;
    Float_t iso03_ecal;
    Float_t iso03_hcal;
    Float_t iso03_ecalveto;
    Float_t iso03_hcalveto;
    Float_t reliso03;
    Float_t pfreliso;
    Float_t CalodeltaR;
    Float_t JPTdeltaR;
    Float_t PFdeltaR;

    Float_t CalEhad;
    Float_t CalEho;
    Float_t CalEem;
    Float_t CaloCompatibility;

    //___ MC
    TopGenMuonEvent mc;

    ClassDef(TopMuonEvent,1);

};

#endif

