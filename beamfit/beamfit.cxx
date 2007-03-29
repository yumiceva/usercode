#include <iostream>
#include <stdlib.h>
#include <TROOT.h>
#include <TFile.h>
#include <TRint.h>
#include <TH1.h>
#include <TF1.h>
#include <TF2.h>
#include <TH2.h>
#include <TMinuit.h>
#include <TSystem.h>
#include <TGraph.h>
#include "tnt.h"
#include "global.h"
#include "beamfit_fcts.h"


const char par_name[dim][20]={"z0  ","sigma ","emmitance","beta*"};
Double_t params[dim],errparams[dim];
Double_t sfpar[dim],errsfpar[dim]; 

static Double_t step[dim] = {0.001,0.001,0.001,0.001};
zData zdata;//!


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
  f = 0.0;
  for ( zDataConstIter iter = zdata.begin(); iter != zdata.end(); ++iter) 
    {
      f = log(zdis(iter->Z,iter->SigZ,params))+f;
    }
  f= -2.0*f;
  return ;
}

float betaf( float betastar,  float emmitance,float z,float z0)
{
  float x = sqrt(emmitance*(betastar+(((z-z0)*(z-z0))/betastar)));
  return x;
}

Double_t ddis(Double_t z, Double_t sigma,Double_t d, Double_t sigmad, Double_t *parms)
{ 
  //---------------------------------------------------------------------------
  // This is a simple function to parameterize the sigma of the beam at a given z.  
  // This is parametrized by a simple normalized gaussian distribution. 
  //---------------------------------------------------------------------------  
  Double_t sig = betaf( parms[Par_beta],parms[Par_eps],z,parms[Par_Z0]);
  sig          = sqrt(sig*sig+sigmad*sigmad);
   Double_t result = (exp(-(d*d)/(2.0*sig*sig)))/(sig*sqrt2pi);
  return result;
} 
void dfcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *params, Int_t iflag)
{
  //----------------------------------------------------------------------------------
  // this is the function used by minuit to do the unbinned fit to the IP distribution 
  //----------------------------------------------------------------------------------
  f = 0.0;
  for ( zDataConstIter iter = zdata.begin(); iter != zdata.end(); ++iter) 
    {
      f = log(ddis(iter->Z,iter->SigZ,iter->D,iter->SigD,params))+f;
    }
  f= -2.0*f;
  return ;
}

void cfcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *params, Int_t iflag)
{
  //-----------------------------------------------------------------------------------
  // this is the function used by minuit to do the unbinned finned combined in z an IP.
  //-----------------------------------------------------------------------------------
  f = 0.0;
  for ( zDataConstIter iter = zdata.begin(); iter != zdata.end(); ++iter) 
    {
      f = log(ddis(iter->Z,iter->SigZ,iter->D,iter->SigD,params)*zdis(iter->Z,iter->SigZ,params))+f;
    }
  f= -2.0*f;
  return ;
}



int main(int argc, char **argv)
{
 static TROOT beamfit("beamfit","Beam fitting");
 static TRint app("app",&argc,argv,NULL,0);   
 tnt *t = new tnt("cortrackdata.root");
 t->Book();
 zdata=t->Loop();
 t->hsd->Draw();
 TMinuit *gmMinuit = new TMinuit(2); 
 gmMinuit->SetFCN(zfcn);
 int ierflg = 0;
 sfpar[Par_Sigma] = 21.;
 sfpar[Par_Z0]    = 0.5;
 sfpar[Par_beta]  = 35.;
 sfpar[Par_eps]   = 1.4e-7;
 // sfpar[Par_Sigma] = 5.;
 // sfpar[Par_Z0]    = 0.0;
 //sfpar[Par_beta]  = 50.; 
 //sfpar[Par_eps]   = 4.5e-8;
 for (int i = 0; i<2; i++) {   
   gmMinuit->mnparm(i,par_name[i],sfpar[i],step[i],0,0,ierflg);
 }
 gmMinuit->Migrad(); 
 gmMinuit->SetFCN(dfcn); 
 for (int i = 2; i<4; i++) {   
   gmMinuit->mnparm(i,par_name[i],sfpar[i],step[i],0,0,ierflg);
 }
 gmMinuit->Migrad();
 gmMinuit->SetFCN(cfcn); 
 for (int i = 0; i<4; i++) {   
   gmMinuit->mnparm(i,par_name[i],sfpar[i],step[i],0,0,ierflg);
 }
 gmMinuit->Migrad();

 //   for (int i=0; i<2; i++) {
 //  gmMinuit->GetParameter(i,sfpar[i],errsfpar[i]);
 // }
 app.Run();   
 return 0;
}
