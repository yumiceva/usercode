/**_________________________________________________________________
   class:   TopBaseEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopBaseEvent.cc,v 1.19 2009/10/26 22:51:24 kellerjd Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopBaseEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopBaseEvent.h"
#endif

#include <math.h>

ClassImp(TopBaseEvent)

// ROOT


//_______________________________________________________________
void TopBaseEvent::Reset()
{

    //___ kinematics _______________________________
    px = -1;
    py = -1;
    pz = -1;
    pt = -1;
    e = -1;

}
