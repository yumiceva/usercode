#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <TRint.h>
#include <TH1.h>
#include <TF1.h>
#include <TF2.h>
#include <TH2.h>
#include "TNtuple.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TMath.h"


float betaf(float z,float z0)
{
  //
  // This function models the width of the beam as a function of z
  //
  //
  // below are the Tevatron values:
  //
  const float betastar  = 35.; 
  const float emmitance = 1.4e-7;
  //
  // LHC values: 
  //
  // const float betastar  = 50.;
  // const float emmitance = 4.5e-8;
  //
  // 
  float x;
  x = sqrt(emmitance*(betastar+(((z-z0)*(z-z0))/betastar)));
  //std::cout << z << " , " << x << "\n";
  return x;
}
int main(int argc, char **argv)
{
 static TROOT beamfit("beamfit","Beam fitting");
 static TRint app("app",&argc,argv,NULL,0);   
 int nevents = 10000;
 const char* filename = "gen.root"; 
 
  //  const float beam_x =  0.2;
  //const float beam_y = -0.3;
  //const float beam_dx =  0.0001;
  //const float beam_dy =  0.0002;
  //const float beam_sigmax = 0.02;
  //const float beam_sigmay = 0.01;
  //const float beam_sigmaz = 15.0;
  //  float betaf();
  const float beam_x =  0.01;
  const float beam_y =  0.02;
  const float beam_dx =  0.0001;
  const float beam_dy =  0.0002;
  const float beam_sigmaz = 20.0;  
  TF1 *beam_profile_z = (TF1*)gROOT->GetFunction("gaus")->Clone("beam_profile_z");
  beam_profile_z->SetParameter(2,beam_sigmaz);
  beam_profile_z->SetRange(-100,100);
  //
  const float sigmaz = 1.0; 
  TF1 *sigma_profile_z = (TF1*)gROOT->GetFunction("gaus")->Clone("sigma_profile_z");
  sigma_profile_z->SetParameter(2,sigmaz);
  sigma_profile_z->SetRange(-10,10);
  TFile* file = TFile::Open(filename,"RECREATE");
  TNtuple* tnt = new TNtuple("tnt","simulated data","run:event:pt:d0:phi:sigmaD:z0:sigmaz0:x:y");
   
  TRandom* rand = new TRandom3();
  gRandom = rand;
  int run = 1;
  for(int ev = 0 ; ev < nevents ; ++ev) {
    int ntracks = rand->Poisson(30);
    for(int t = 0 ; t < ntracks ; ++t) {
      float z     = beam_profile_z->GetRandom();
      float sigz  = sigma_profile_z->GetRandom();
      float beam_sigmax = betaf(z,0.0);
      float beam_sigmay = betaf(z,0.0);
      float x = rand->Gaus(beam_x + z * beam_dx,beam_sigmax);
      float y = rand->Gaus(beam_y + z * beam_dy,beam_sigmay);
      float phi0 =  rand->Rndm()* 2 * TMath::Pi();
      float real_d0 = - x * sin(phi0) + y * cos(phi0);
      float pt = 2.0;
      float sigmad0 = 0.00100; // assume  
      float d0 = rand->Gaus(real_d0,sigmad0);
      tnt->Fill(run,ev,pt,d0,phi0,sigmad0,z,sigz,x,y);
    }
    //    ntracks = rand->Poisson(12);
    //for(int t = 0 ; t < ntracks ; ++t) {
    //  float z = beam_profile_z->GetRandom();
    //  float x = rand->Rndm()* 20 - 10;
    //  float y = rand->Rndm()* 20 - 10;
    //  //std::cout << x << " , " << y << " , " << z << "\n";
    //  float phi0 =  rand->Rndm()* 2 * TMath::Pi();
    //  float real_d0 = - x * sin(phi0) + y * cos(phi0);
    //  float pt = 0;
    //  float sigmad0 = 0.400;
    //  float d0 = rand->Gaus(real_d0,sigmad0);
    //  tnt->Fill(run,ev,pt,d0,phi0,sigmad0,z,x,y);
    //}
  }
  tnt->Write();
  file->Close();
  app.Run();   
  return 0;
}


  
