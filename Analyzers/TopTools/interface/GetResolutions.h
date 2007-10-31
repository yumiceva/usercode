#ifndef GetResolutions_h
#define GetResolutions_h

#include "TF1.h"
#include "TH1.h"
#include "TFile.h"
#include "TKey.h"
#include "TString.h"

class GetResolutions {

  public:
    GetResolutions();
    GetResolutions(TString);
    ~GetResolutions();	

    double getObsRes(int, int, double);
    int    getEtaBin(double);
    
  private:
    TFile * resoFile;
    std::vector<double> etabinVals;
    TF1 fResVsET[10][10];
    
};

#endif
