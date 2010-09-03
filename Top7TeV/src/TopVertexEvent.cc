/**_________________________________________________________________
   class:   TopVertexEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopVertexEvent.cc,v 1.2 2010/08/23 01:49:41 yumiceva Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopVertexEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopVertexEvent.h"
#endif


ClassImp(TopVertexEvent)

//_______________________________________________________________
void TopVertexEvent::Reset()
{

  vx = -999;
  vy = -999;
  vz = -999;
  ndof = -999;
  rho = -999;
  IsGood = 0;

}
