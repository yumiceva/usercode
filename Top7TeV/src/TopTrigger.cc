/**_________________________________________________________________
   class:   TopTrigger.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopTrigger.cc,v 1.1 2011/05/04 22:37:20 yumiceva Exp $

 ________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopTrigger.h"
#else
#include "Yumiceva/Top7TeV/interface/TopTrigger.h"
#endif

ClassImp(TopTrigger)

//_______________________________________________________________
  void TopTrigger::Reset()
{

  name = "";
  prescale = 1;
  accept = false;

}
