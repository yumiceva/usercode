/**_________________________________________________________________
   class:   TopMyGenEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopMyGenEvent.cc,v 1.1 2010/08/23 01:49:41 yumiceva Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopMyGenEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopMyGenEvent.h"
#endif

#include <math.h>

ClassImp(TopMyGenEvent)

// ROOT


//_______________________________________________________________
void TopMyGenEvent::Reset()
{

  //__
  isSemiLeptonic = 0;
  isDilepton = 0;
  LeptonicChannel = 0;

   t_pt = -99;
   t_eta = -99;
   t_phi = -99;
   t_e = -99;
   tbar_pt = -99;
   tbar_eta = -99;
   tbar_phi = -99;
   tbar_e = -99;
   nu_pt = -99;
   nu_eta = -99;
   nu_phi = -99;
   nu_e = -99;
   mu_pt = -99;
   mu_eta = -99;
   mu_phi = -99;
   mu_e = -99;
   Wp_pt = -99;
   Wp_eta = -99;
   Wp_phi = -99;
   Wp_e = -99;
   Wq_pt = -99;
   Wq_eta = -99;
   Wq_phi = -99;
   Wq_e = -99;
   bHad_pt = -99;
   bHad_eta = -99;
   bHad_phi = -99;
   bHad_e = -99;
   bLep_pt = -99;
   bLep_eta = -99;
   bLep_phi = -99;
   bLep_e = -99;

}
