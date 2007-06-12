#ifndef TopHistograms_h
#define TopHistograms_h

/**_________________________________________________________________
   class:   TopHistograms.h
   package: Analyzers/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopHistograms.h,v 1.8 2007/05/31 16:05:09 yumiceva Exp $

________________________________________________________________**/


#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"

class TopHistograms {

  public:

	TopHistograms();
	~TopHistograms();

	void Init(TString type, TString suffix1="", TString suffix2="");
	void Fill1d(TString name, Double_t x );
	void Fill2d(TString name, Double_t x, Double_t y);
	void Print(TString extension="png", TString tag="");
	void SaveToFile(TString filename="Ttplots.root");
	void Fit(TString name, Double_t mean);
	
  private:

	std::map<TString, TCanvas*> cv_map;
	std::map<TString, TH1*> h1;
	std::map<TString, TH2*> h2;
	TFile            *ffile;
	TFile            *foutfile;		
	
};

#endif
