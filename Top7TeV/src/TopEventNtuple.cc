/**_________________________________________________________________
   class:   TopEventNtuple.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopEventNtuple.cc,v 1.1 2010/08/18 16:28:43 yumiceva Exp $

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
    jets.clear();
    electrons.clear();
    vertices.clear();

    MET        = -1;
    METeta     = -999;
    METphi     = -999;
    Ht         = -1;
    
    M3         = -1;
    chi2sorting= -1;
}
