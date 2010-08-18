/**_________________________________________________________________
   class:   TopElectronEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopElectronEvent.cc,v 1.19 2009/10/26 22:51:24 kellerjd Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopElectronEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopElectronEvent.h"
#endif

#include <math.h>

ClassImp(TopElectronEvent)


//_______________________________________________________________
void TopElectronEvent::Reset()
{

    //_____
    normchi2 = -1;
    isTracker = -1;
    d0 = -1;
    d0err = -1;
    d0wrtPV2d = -1;
    d0wrtPV2derr = -1;

}
