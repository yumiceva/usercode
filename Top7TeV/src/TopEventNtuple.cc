/**_________________________________________________________________
   class:   TopEventNtuple.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopEventNtuple.cc,v 1.19 2009/10/26 22:51:24 kellerjd Exp $

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

    njets      = -1;
    nmuons     = -1;
    nelectrons = -1;
    nvertices  = -1;

    muons.clear();
    //jets.clear();
    electrons.clear();
    //vertices.clear();

    MET        = -1;
    METx       = -1;
    METy       = -1;
    Ht         = -1;
    
    M3         = -1;
    chi2sorting= -1;
}
