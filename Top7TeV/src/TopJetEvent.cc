/**_________________________________________________________________
   class:   TopJetEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopJetEvent.cc,v 1.1 2010/08/18 16:28:43 yumiceva Exp $

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
  btag_TCHE = -999;
  btag_TCHP = -999;
  btag_SSVHE= -999;
  btag_SSVHP= -999;

}
