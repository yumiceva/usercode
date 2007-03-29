#include <iostream>
#include <stdlib.h>
#include <TROOT.h>
#include "TFile.h"
#include "TTree.h"

#include "TMatrixD.h"
#include "TMatrixDSym.h"
#include "TNtuple.h"
#include <vector>
#include <iostream>
#include "tnt.h"
#include "global.h"
#include <TApplication.h>
#include <string>
using namespace std;
double sigmabeam2 = 0.002 * 0.002;
zData zdata;//!

void fit(TMatrixD &x, TMatrixDSym &V)
{
  TMatrixDSym Vint(4);
  TMatrixD b(4,1);
  Double_t weightsum = 0;

  Vint.Zero();
  b.Zero();
  TMatrixD g(4,1);
  TMatrixDSym temp(4);
  for(zDataConstIter i = zdata.begin() ; i != zdata.end() ; ++i) {
    //std::cout << "weight  " << sqrt(i->weight2) << "\n";
    if(i->weight2 == 0) continue;

    g(0,0) = - sin(i->Phi);
    g(1,0) =   cos(i->Phi);
    g(2,0) = i->Z * g(0,0);
    g(3,0) = i->Z * g(1,0);
    
    temp.Zero();
    for(int j = 0 ; j < 4 ; ++j) {
      for(int k = j ; k < 4 ; ++k) {
	temp(j,k) += g(j,0) * g(k,0);
      }
    }
    double sigma2 = sigmabeam2 +  (i->SigD)* (i->SigD) / i->weight2;
    Vint += (temp * (1 / sigma2));
    b += (i->D / sigma2 * g);
    weightsum += sqrt(i->weight2);
  }
  Double_t determinant;
  V = Vint.InvertFast(&determinant);
  x = V  * b;
  std::cout << "Sum of all weights:" << weightsum << "\n";
  std::cout << "x0                :" << x(0,0)    << " +- " << sqrt(V(0,0)) << " cm \n"; 
  std::cout << "y0                :" << x(1,0)    << " +- " << sqrt(V(1,1)) << " cm \n"; 
  std::cout << "x slope           :" << x(2,0)    << " +- " << sqrt(V(2,2)) << " cm \n";  
  std::cout << "y slope           :" << x(3,0)    << " +- " << sqrt(V(3,3)) << " cm \n";
}

//Double_t cutOnD(const TMatrixD& x, Double_t dcut)
//{
//  TMatrixD g(1,4);
//  Double_t weightsum = 0;
//  for(zDataConstIter i = zdata.begin() ; i != zdata.end() ; ++i) {
//    g(0,0) = - sin(i->Phi);
//    g(0,1) =   cos(i->Phi);
//    g(0,2) = i->Z * g(0,0);
//    g(0,3) = i->Z * g(0,1);
//    TMatrixD dcor = g * x;
//    //std::cout << dcor.GetNrows() << " , " << dcor.GetNcols() << "\n";
//    if(std::abs(i->D -  dcor(0,0)) > dcut) {
//      i->weight2 = 0; 
//    } else {
//      i->weight2 = 1;
//      weightsum += 1;
//    }
//  }
//  return weightsum;
//} 

Double_t cutOnChi2(const TMatrixD& x, Double_t chi2cut)
{
  TMatrixD g(1,4);
  Double_t weightsum = 0;
  for(zDataIter i = zdata.begin() ; i != zdata.end() ; ++i) {
    g(0,0) = - sin(i->Phi);
    g(0,1) =   cos(i->Phi);
    g(0,2) = i->Z * g(0,0);
    g(0,3) = i->Z * g(0,1);
    TMatrixD dcor = g * x;
    //std::cout << dcor.GetNrows() << " , " << dcor.GetNcols() << "\n";
    Double_t chi2 = (i->D -  dcor(0,0))* (i->D -  dcor(0,0)) / 
      (sigmabeam2 +  (i->SigD)*(i->SigD));
    if(chi2 > chi2cut) {
      i->weight2 = 0; 
    } else {
      i->weight2 = 1;
      weightsum += 1;
    }
  }
  //std::cout << weightsum << "\n";
  return weightsum;
} 
int main(int argc, char **argv)
{
  if ( argc < 3 || "-h" == string(argv[1]) || "--help" == string(argv[1]) )
    {
      std::cout << "Usage: " << argv[0] << "inputfile outputfile " << 
std::endl;
      return 1;
    }

  // static TROOT dphifit("dphi","Beam line fitting");
 // static TApplication app("app",&argc,argv);

 tnt *t = new tnt(argv[1]);
 t->Book();
 zdata=t->Loop();
 // t->hsd->Draw();
  TMatrixD x(4,1);
  TMatrixDSym V(4);
  fit(x,V);
  Double_t dcut = 4.0;
  Double_t chi2cut = 20;
  //while( cutOnD(x,dcut) > 0.5 * zdata.size() ) {
  // below is a very artificial cut requesting that 50 % of the sample survive
  // we hould investigate if there are better criteria than that.
  //
  //  while( cutOnChi2(x,chi2cut) > 0.5 * zdata.size() ) {
  //  fit(x,V); 
  //  dcut /= 1.5;
  //  chi2cut /= 1.5;
  //}

  const char* filename = argv[2]; 
  TFile* file = TFile::Open(filename,"RECREATE");
  TNtuple* tnt = new TNtuple("tnt","CDF data corrected","run:event:pt:d0:phi:sigmaD:z0:sigmaz0:x:y");
  int counter=0;
  for(zDataConstIter i = zdata.begin() ; i != zdata.end() ; ++i) 
    {
      if(i->weight2 == 0) continue;
      //
      // assume linear approximation applies to keep things simple:
      // in this case only D changes. 
  Double_t xv = x(0,0)+(i->Z)* x(2,0);
  Double_t yv = x(1,0)+(i->Z)* x(3,0);
  Double_t dp = (i->D)+xv*sin(i->Phi)-yv*cos(i->Phi);
  if (fabs(dp)<0.02){
    tnt->Fill(1,1,1.5,dp,i->Phi,i->SigD,i->Z,i->SigZ,xv,yv);
    //tnt->Fill(1,1,1.5,dp,i->Phi,i->SigD,i->Z,i->SigZ,0.0,0.0);
    counter++;
  }
  }
  std::cout<<counter<<std::endl;
  tnt->Write();
  file->Close();
  //  app.Run();
}


