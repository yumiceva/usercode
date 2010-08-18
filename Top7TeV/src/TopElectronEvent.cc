/**_________________________________________________________________
   class:   TopElectronEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopElectronEvent.cc,v 1.1 2010/08/18 16:28:43 yumiceva Exp $

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
    d0 = -1;
    d0err = -1;
    d0wrtPV2d = -1;
    d0wrtPV2derr = -1;

    reliso03 = -1;

}
