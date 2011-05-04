/**_________________________________________________________________
   class:   TopTrigger.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopTrigger.cc,v 1.3 2010/09/03 16:16:21 yumiceva Exp $

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
  pass = false;

}
