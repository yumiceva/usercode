/**_________________________________________________________________
   class:   TopGenCandidateEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopGenCandidateEvent.cc,v 1.1 2010/08/18 21:13:05 yumiceva Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopGenCandidateEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopGenCandidateEvent.h"
#endif

#include <math.h>

ClassImp(TopGenCandidateEvent)

// ROOT


//_______________________________________________________________
void TopGenCandidateEvent::Reset()
{

  //__
  pdgId = -999;
  mom_pdgId = -999;
  
  mom_pt = -999;
  mom_eta= -999;
  mom_phi= -999;

}
