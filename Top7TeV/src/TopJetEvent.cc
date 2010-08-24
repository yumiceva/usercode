/**_________________________________________________________________
   class:   TopJetEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopJetEvent.cc,v 1.2 2010/08/23 01:49:41 yumiceva Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopJetEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopJetEvent.h"
#endif

#include <math.h>

ClassImp(TopJetEvent)

//_______________________________________________________________
void TopJetEvent::Reset()
{

  //___
  ntracks = -999;
  ndaughters = -999;

  //___
  btag_TCHE = -999;
  btag_TCHP = -999;
  btag_SSVHE= -999;
  btag_SSVHP= -999;

  mc.Reset();

}
