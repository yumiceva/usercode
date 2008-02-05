#ifndef TQAFHistograms_h
#define TQAFHistograms_h

/**_________________________________________________________________
   class:   TQAFHistograms.h
   package: Analyzers/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TQAFHistograms.h,v 1.10 2007/10/31 19:34:56 yumiceva Exp $

________________________________________________________________**/


#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"

class TQAFHistograms {

  public:

	TQAFHistograms();
	~TQAFHistograms();

	void Init(TString type, TString suffix1="", TString suffix2="");
	void Fill1d(TString name, Double_t x );
	void Fill2d(TString name, Double_t x, Double_t y, Double_t weight=1);
	void Print(TString extension="png", TString tag="");
	void Save();
	void SaveToFile(TString filename="Ttplots.root");
	void Fit(TString name, Double_t mean);
	void DeleteHisto() {
	  for(std::map<TString,TH1* >::const_iterator ih=h1.begin(); ih!=h1.end(); ++ih){
	    TH1 *htemp = ih->second;
	    delete htemp;
	  }
	  for(std::map<TString,TH2* >::const_iterator ih=h2.begin(); ih!=h2.end(); ++ih){
	    TH2 *htemp = ih->second;
	    delete htemp;
	  }
	};

  private:

	std::map<TString, TCanvas*> cv_map;
	std::map<TString, TH1*> h1;
	std::map<TString, TH2*> h2;
	TFile            *ffile;
	TFile            *foutfile;		
	
};

#endif
