#ifndef TopHistograms_h
#define TopHistograms_h

/**_________________________________________________________________
   class:   TopHistograms.h
   package: Analyzers/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopHistograms.h,v 1.5 2007/05/30 15:18:22 yumiceva Exp $

________________________________________________________________**/


#include <TString.h>
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

class TopHistograms {

  public:

	TopHistograms();
	~TopHistograms();

	void Init(TString type, TString suffix1="", TString suffix2="");
	void fill1d(TString name, Double_t x );
	void fill2d(TString name, Double_t x, Double_t y);
	void Print(TString extension="png", TString tag="");
	void SaveToFile(TString filename="Ttplots.root");

  private:

	std::map<std::string, TCanvas*> cv_map;
	std::map<std::string, TH1*> h1;
	std::map<std::string, TH2*> h2;
	
	
};

#endif
