/**_________________________________________________________________
   class:   TopEventNtuple.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopEventNtuple.cc,v 1.7 2011/03/01 22:18:51 yumiceva Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopEventNtuple.h"
#else
#include "Yumiceva/Top7TeV/interface/TopEventNtuple.h"
#endif

#include <math.h>

ClassImp(TopEventNtuple)

// ROOT

//_______________________________________________________________
TopEventNtuple::TopEventNtuple()
{
    this->Reset();
}



//_______________________________________________________________
TopEventNtuple::~TopEventNtuple()
{
}

//_______________________________________________________________
void TopEventNtuple::Reset()
{

    event      = -1;
    run        = -1;
    lumi       = -1;
    dataType   = -1;

    //njets      = -1;
    //nmuons     = -1;
    //nelectrons = -1;
    //nvertices  = -1;

    muons.clear();
    //Calojets.clear();
    //JPTjets.clear();
    PFjets.clear();
    //PFlowjets.clear();
    electrons.clear();
    PFelectrons.clear();
    vertices.clear();

    //CaloMET        = -999;
    //CaloMETeta     = -999;
    //CaloMETphi     = -999;
    //CaloHt         = -1;
    //tcMET = -999;
    //tcMETphi = -999;
    //tcHt = -999;
    PFMET = -999;
    PFMETphi = -999;
    PFHt = -999;
    //PFlowMET = -999;
    //PFlowMETphi = -999;
    //PFlowHt = -999;

    IsZevent = -1;

    rho = -1;

    flavorHistory = -999;
    gen.Reset();

    //M3         = -1;
    //chi2sorting= -1;
}
