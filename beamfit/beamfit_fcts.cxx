#include "beamfit_fcts.h"
#include "global.h"
Double_t zdis(Double_t z, Double_t sigma, Double_t *parms)
{ 
  //---------------------------------------------------------------------------
  //  This is a simple function to parameterize the z-vertex distribution. This 
  // is parametrized by a simple normalized gaussian distribution. 
  //---------------------------------------------------------------------------  
  Double_t sig = sqrt(sigma*sigma+parms[Par_Sigma]*parms[Par_Sigma]);
  Double_t result = (exp(-((z-parms[Par_Z0])*(z-parms[Par_Z0]))/(2.0*sig*sig)))/(sig*sqrt2pi);
  return result;
} 


void zfcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *params, Int_t iflag)
{
  //----------------------------------------------------------------------------------
  // this is the function used by minuit to do the unbinned fit to the z distribution
  //----------------------------------------------------------------------------------
  double mass;   

  f = 0.0;
  zDataConstIter iterator = zdata.begin();
  for ( zDataConstIter iterator = zdata.begin(); iterator != zdata.end(); ++i) {

    mass =  params[bgrfrac+offset]*
      bgrmassdis(iterator->Getmass(),params,Bsample->Getmin_mass(),Bsample->Getmax_mass())+
      (1.0-params[bgrfrac+offset])*signalmassdis(iterator->Getmass(),iterator->Getsigmamass(),params);


    f    = log(mass)+f;
    
    ++iterator;
    }

  f= -2.0*f;
  return ;
}


