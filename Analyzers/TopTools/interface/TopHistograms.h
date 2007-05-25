#ifndef TopAnalysis_h
#define TopAnalysis_h

/**_________________________________________________________________
   class:   TopHistograms.h
   package: Analyzers/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopHistograms.h,v 1.1 2007/04/25 03:53:54 yumiceva Exp $

________________________________________________________________**/


#include <TString.h>
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

class TopHistograms {

  public:

	TopHistograms();
	~TopHistograms();

	void Init();
	void fill1d(TString name, Double_t x );
	void fill2d(TString name, Double_t x, Double_t y)"
	void Print(TString extension="png", TString tag="");
	void SaveToFile(TString filename="plots_summary.root");
	
	
}

