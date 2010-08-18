/**_________________________________________________________________
   class:   TopCandidateEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopCandidateEvent.cc,v 1.1 2010/08/18 16:28:43 yumiceva Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopCandidateEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopCandidateEvent.h"
#endif

#include <math.h>

ClassImp(TopCandidateEvent)

// ROOT


//_______________________________________________________________
void TopCandidateEvent::Reset()
{

    //___ kinematics _______________________________
    eta = -999;
    phi = -999;
    pt = -1;
    e = -1;

    vx = -1;
    vy = -1;
    vz = -1;

}
