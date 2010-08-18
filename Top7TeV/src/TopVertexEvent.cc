/**_________________________________________________________________
   class:   TopVertexEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopVertexEvent.cc,v 1.1 2010/08/18 16:28:43 yumiceva Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopVertexEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopVertexEvent.h"
#endif

#include <math.h>

ClassImp(TopVertexEvent)

//_______________________________________________________________
void TopVertexEvent::Reset()
{

  vx = -1;
  vy = -1;
  vz = -1;
  ndof = -1;
  rho = -1;

}
