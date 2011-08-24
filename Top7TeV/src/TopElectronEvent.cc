/**_________________________________________________________________
   class:   TopElectronEvent.cc
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopElectronEvent.cc,v 1.6 2011/05/04 22:37:20 yumiceva Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopElectronEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopElectronEvent.h"
#endif

#include <math.h>

ClassImp(TopElectronEvent)


//_______________________________________________________________
void TopElectronEvent::Reset()
{

    //_____
    d0 = -1;
    d0err = -1;
    d0wrtPV2d = -1;
    d0wrtPV2derr = -1;

    IsTight = -1;
    pass70 = -1;
    pass95 = -1;

    missingHits = -1;
    convDist = -1;
    convDcot = -1;
    IsConversion = -1;
    etasc = -999;
    deltaetasc = -999;
    deltaphisc = -999;
    sigmaIetaIeta = -999;
    HoE = -999;

    iso03_track = -1;
    iso03_ecal = -1;
    iso03_hcal = -1;
    iso03_ecalveto = -1;
    iso03_hcalveto = -1;
    reliso03 = -1;

    mc.Reset();

}
