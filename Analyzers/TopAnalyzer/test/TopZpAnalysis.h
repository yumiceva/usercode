#ifndef TopZpAnalysis_h
#define TopZpAnalysis_h

/**_________________________________________________________________
   class:   TopAnalysis.h
   package: Analyzers/TopAnalyzer


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopZpAnalysis.h,v 1.1 2007/11/03 00:00:27 yumiceva Exp $

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
#include "Analyzers/TopTools/interface/GetResolutions.h"

class TopZpAnalysis {

  public:

	TopZpAnalysis(TString filename);
	virtual ~TopZpAnalysis();

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
	void ApplyKinFit( bool option = true ) { fkinfit = option; };
	void SetMinKFChi2( double value) { fminKFchi2 = value; };
	void ApplyFilter( bool option = true ) { ffilter = option; };
	void ApplyPartonMatching( bool option = true ) { ffilter = option; };
	void SetJES( double value ) { fscale = value; };
	void Weight( double value, int sample ) { fweight = value;fsample=sample;};
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
	bool              ffilter;
	bool              ftopo;
	bool              fPartonMatching;
	double            fscale;
	Double_t          fminKFchi2;
	Double_t          fphicut;
	Double_t          fweight;
	Int_t             fsample;
	
	TopEvent *fevent;
	TopHistograms *h_;

	std::map<TString, TString> cut_map;

	GetResolutions *ResJets_;
	GetResolutions *ResbJets_;
	GetResolutions *ResMuons_;
	GetResolutions *ResMET_;
	
		//std::map<std::string, TCanvas*> cv_map;
		//std::map<std::string, TH1*> h1;
		//std::map<std::string, TH2*> h2;
	
};

#endif

