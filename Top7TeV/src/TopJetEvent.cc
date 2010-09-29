/**_________________________________________________________________
   class:   TopJetEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopJetEvent.cc,v 1.4 2010/08/25 16:03:42 yumiceva Exp $

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
  id_neutralEmE = -999;
  id_chargedEmE = -999;
  id_muonMultiplicity = -999;

  //___
  ntracks = -999;
  nSVs = -999;
  ndaughters = -999;

  //___
  btag_TCHE = -999;
  btag_TCHP = -999;
  btag_SSVHE= -999;
  btag_SSVHP= -999;

  mc.Reset();

}
