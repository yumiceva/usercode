/**_________________________________________________________________
   class:   TopMuonEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopMuonEvent.cc,v 1.4 2010/09/28 15:49:24 yumiceva Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopMuonEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopMuonEvent.h"
#endif

#include <math.h>

ClassImp(TopMuonEvent)

//_______________________________________________________________
void TopMuonEvent::Reset()
{
   
    //___
  IsTrackerMuon = 0;
  IsLooseIsoMuon = 0;
  IsTightIsoMuon = 0;

  d0 = -1;
  d0err = -1;
  d0wrtPV2d = -1;
  d0wrtPV2derr = -1;

  muonhits = -1;
  trackerhits = -1;
  muonstations = -1;
  normchi2 = -1;
  pixelhits = -1;

  iso03_track = -1;
  iso03_ecal = -1;
  iso03_hcal = -1;
  iso03_ecalveto = -1;
  iso03_hcalveto = -1;
  reliso03 = -1;
  pfreliso = -1;
  CalodeltaR = -1;
  JPTdeltaR = -1;
  PFdeltaR = -1;

  CalEhad = -999;
  CalEho = -999;
  CalEem = -999;
  CaloCompatibility = -999;

  mc.Reset();

}
