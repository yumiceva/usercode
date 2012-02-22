/**_________________________________________________________________
   class:   TopJetEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopJetEvent.cc,v 1.7 2011/10/10 20:53:15 yumiceva Exp $

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
  uncorrpx = -999;
  uncorrpy = -999;

  //___
  id_neutralEmE = -999;
  id_chargedEmE = -999;
  id_muonMultiplicity = -999;

  //___
  ntracks = -999;
  nSVs = -999;
  ndaughters = -999;
  jetArea = -999;

  //___
  btag_TCHE = -999;
  btag_TCHP = -999;
  btag_SSVHE= -999;
  btag_SSVHP= -999;
  btag_CSV = -999;

  mc.Reset();

}
