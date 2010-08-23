/**_________________________________________________________________
   class:   TopGenJetEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopGenJetEvent.cc,v 1.1 2010/08/18 21:13:05 yumiceva Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopGenJetEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopGenJetEvent.h"
#endif

ClassImp(TopGenJetEvent)

//_______________________________________________________________
void TopGenJetEvent::Reset()
{

  pt = -999;
  eta = -999;
  phi = -999;
  e = -999;

  flavor =-999;

}
