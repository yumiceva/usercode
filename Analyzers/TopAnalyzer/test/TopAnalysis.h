#ifndef TopAnalysis_h
#define TopAnalysis_h

/**_________________________________________________________________
   class:   TopAnalysis.h
   package: Analyzers/TopAnalyzer


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopAnalysis.h,v 1.4 2007/05/31 21:25:08 yumiceva Exp $

________________________________________________________________**/

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLorentzVector.h"

#include <iostream>

#include "Analyzers/TopEvent/interface/TopEvent.h"
#include "Analyzers/TopTools/interface/TopWSolver.h"
#include "Analyzers/TopTools/interface/TopHistograms.h"

class TopAnalysis {

  public:

	TopAnalysis(TString filename);
	virtual ~TopAnalysis();

	// root stuff
	virtual Int_t    GetEntry(Long64_t entry);
	virtual void     Init(TChain *tree);
	virtual void     Loop(int max_entries=0);
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Show(Long64_t entry = -1);
	virtual Int_t    Cut(Long64_t entry);
	virtual Bool_t   Notify();

	// analysis stuff
	void ApplyJetCorrections(bool option = true ) {fcorrections = option; };
	void ApplyKinFit( bool option = true ) { fkinfit = option; };	void SetMinKFChi2( double value) { fminKFchi2 = value; };
	void SetPhiCut( double value) { fphicut = value; };
	void Verbose(bool option=true) { fverbose = option; };
	void SampleName(TString sample) {
		fsamplename = sample;
	};
	void Print(std::string extension="png",std::string tag="");
	void SaveToFile(TString filename="plots_summary.root");
	
  private:

	TChain           *fChain;
	Int_t             fCurrent;
	TFile            *ffile;
	TFile            *foutfile;
	TString           fsamplename;
	bool              fverbose;
	bool              fcorrections;
	bool              fkinfit;
	Double_t          fminKFchi2;
	Double_t          fphicut;

	TopEvent *fevent;
	TopHistograms *h_;

	std::map<TString, TString> cut_map;
	
		//std::map<std::string, TCanvas*> cv_map;
		//std::map<std::string, TH1*> h1;
		//std::map<std::string, TH2*> h2;
	
};

#endif

