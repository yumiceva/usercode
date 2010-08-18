/**_________________________________________________________________
   class:   TopMuonEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopMuonEvent.cc,v 1.19 2009/10/26 22:51:24 kellerjd Exp $

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
    normchi2 = -1;
    isTracker = -1;
    d0 = -1;
    d0err = -1;
    d0wrtPV2d = -1;
    d0wrtPV2derr = -1;

}
