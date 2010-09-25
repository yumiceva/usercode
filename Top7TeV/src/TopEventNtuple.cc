/**_________________________________________________________________
   class:   TopEventNtuple.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopEventNtuple.cc,v 1.4 2010/08/26 17:58:22 yumiceva Exp $

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
    Calojets.clear();
    JPTjets.clear();
    PFjets.clear();
    electrons.clear();
    vertices.clear();

    CaloMET        = -999;
    //CaloMETeta     = -999;
    CaloMETphi     = -999;
    CaloHt         = -1;
    tcMET = -999;
    tcMETphi = -999;
    tcHt = -999;
    PFMET = -999;
    PFMETphi = -999;
    PFHt = -999;
    IsZevent = -1;

    flavorHistory = -999;
    gen.Reset();

    //M3         = -1;
    //chi2sorting= -1;
}
