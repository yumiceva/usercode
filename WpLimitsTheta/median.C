#include <vector>
#include <map>
#include <string>

void median(bool IsObserved=false, int mass=800)
{

  //  TFile f("results_observed.root");
  
  std::vector<double> limits;

  TTree *T = (TTree*) gDirectory->Get("products");
  double bayes = 0;
  T->SetBranchAddress("bayes__quant09500",&bayes);
  Int_t nentries = (Int_t)T->GetEntries();
  for (Int_t i=0;i<nentries;i++) {
    T->GetEntry(i);
    limits.push_back( bayes );
  }

  std::map<int, double> xsec;
  //xsec[800] = 5.26;
  //xsec[900] = 1.93;
  //xsec[1000] = 0.773;
  //xsec[1400] = 0.228;
  //xsec[1500] = 0.034;
  xsec[800] = 1.6697;
  xsec[900] = 0.97609;
  xsec[1000] = 0.58782;
  xsec[1100] = 0.36266;
  xsec[1200] = 0.22815;
  xsec[1300] = 0.14584;
  xsec[1400] = 0.094496;
  xsec[1500] = 0.061946;
  xsec[1600] = 0.04102;
  //xsec[2000] = 7.935e-3;

  float sigma = xsec[mass];

  std::sort(limits.begin(), limits.end());
  double median = limits[int(0.5*limits.size())];
  double onesigma_low = limits[int(0.16*limits.size())];
  double onesigma_high = limits[int(0.84*limits.size())];
  double twosigma_low = limits[int(0.025*limits.size())];
  double twosigma_high = limits[int(0.975*limits.size())];

  string type = "Expected";
  if (IsObserved) type = "Observed";
    
  cout <<  "median"<< type <<" " << mass << " " <<	sigma*median << endl;

 if (! IsObserved) {
   cout << "OneSigmaDown " << mass << " " <<	sigma*onesigma_low << endl;

   cout << "OneSigmaUp " << mass << " " <<  sigma*onesigma_high << endl;
   
   cout << "TwoSigmaDown " << mass << " " << sigma*twosigma_low << endl;

   cout << "TwoSigmaUp " << mass << " " <<  sigma*twosigma_high << endl;
 }

}

