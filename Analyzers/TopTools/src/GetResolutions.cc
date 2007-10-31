
//
#include "Analyzers/TopTools/interface/GetResolutions.h"
#include<iostream>

// constructor with path; default should not be used
GetResolutions::GetResolutions() {
}

GetResolutions::GetResolutions(TString resopath) {
  std::cout << "=== Constructing a GetResolutions... " << std::endl; 
  resoFile = new TFile(resopath);
  if (!resoFile) std::cout<<"### No resolutions fits for this file available: "<<resopath<<"... ###"<<std::endl;
  TString  resObsName[8] = {"_ares","_bres","_cres","_dres","_thres","_phres","_etres","_etares"};
  
  TList* keys = resoFile->GetListOfKeys();
  TIter nextitem(keys);
  TKey* key = NULL;
  while((key = (TKey*)nextitem())) {
    TString name = key->GetName();
    
      if(name.Contains("etabin") && (!name.Contains("etbin"))) {
        for(int p=0; p<8; p++){
          if(name.Contains(resObsName[p])){
            TString etabin = name; etabin.Remove(0,etabin.Index("_")+1); etabin.Remove(0,etabin.Index("_")+7);
            int etaBin = etabin.Atoi();
            TH1F *tmp = (TH1F*) (resoFile->GetKey(name)->ReadObj());
            fResVsET[p][etaBin] = (TF1)(*(tmp -> GetFunction("F_"+name)));
	  }
        }
      }
    
  }
  // find etabin values
  TH1F *tmpEta = (TH1F*) (resoFile->GetKey("hEtaBins")->ReadObj());
  for(int b=1; b<=tmpEta->GetNbinsX(); b++) etabinVals.push_back(tmpEta->GetXaxis()->GetBinLowEdge(b));
  etabinVals.push_back(tmpEta->GetXaxis()->GetBinUpEdge(tmpEta->GetNbinsX()));
  std::cout<<"Found "<<etabinVals.size()-1<< " eta-bins with edges: ( ";
  for(size_t u=0; u<etabinVals.size(); u++) std::cout<<etabinVals[u]<<", ";
  std::cout<<"\b\b )"<<std::endl;
  
  std::cout << "=== done." << std::endl;
}


// destructor
GetResolutions::~GetResolutions() {
  delete resoFile;
}

double GetResolutions::getObsRes(int obs, int eta, double eT){
  double res = fResVsET[obs][eta].Eval(eT);
  return res;
}

int GetResolutions::getEtaBin(double eta){
  int nrEtaBins = etabinVals.size()-1;
  int bin = nrEtaBins-1;
  for(int i=0; i<nrEtaBins; i++) {
    if(fabs(eta) > etabinVals[i] && fabs(eta) < etabinVals[i+1]) bin = i;
  }
  return bin;
}
