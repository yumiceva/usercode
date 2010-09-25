/**_________________________________________________________________
   class:   TopElectronEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopElectronEvent.cc,v 1.3 2010/08/24 22:04:19 yumiceva Exp $

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

    IsTight = -1;
    pass70 = -1;
    pass95 = -1;

    IsConversion = -1;

    reliso03 = -1;

    mc.Reset();

}
