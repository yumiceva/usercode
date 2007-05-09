#ifndef plot_summary_h
#define plot_summary_h
/** \class plot_summary
 *  
 * Analyze ROOT files produced by analyzer and create plots
 *
 * \author Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)
 *
 * \version $Id: plot_summary.h,v 1.2 2007/01/22 04:48:40 yumiceva Exp $
 *
 */

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

#include <iostream>

#include "BTagSummary.h"

class plot_summary {

  public:
	TChain            *fChain;
	Int_t             fCurrent;
	TFile            *ffile;
	TFile            *foutfile;
	TString           fsamplename;
		
	BTagSummary *fBTagSummary[3];
	std::map<std::string, TCanvas*> cv_map;
	std::map<std::string, TH1*> h1;
	std::map<std::string, TH2*> h2;
	
	//plot_summary(TTree *tree=0);
	plot_summary(TString filename);
	virtual ~plot_summary();
	virtual Int_t  GetEntry(Long64_t entry);
	virtual void     Init(TChain *tree);
	virtual void     Loop();
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Show(Long64_t entry = -1);
	virtual Int_t    Cut(Long64_t entry);
	virtual Bool_t   Notify();
	//virtual std::string itoa(int i);
	//virtual std::string ftoa(float i);
	void SampleName(TString sample) {
		fsamplename = sample;
	};
	void Print(string extension="png",string tag="") {
		if ( tag != "" ) tag = "_"+tag;
		
		for(std::map<std::string,TCanvas*>::const_iterator icv=cv_map.begin(); icv!=cv_map.end(); ++icv){

			string tmpname = icv->first;
			TCanvas *acv = icv->second;
			acv->Print(TString(tmpname+tag+"."+extension));
		}		
	};
	
	void SaveToFile(TString filename="plots_summary.root") {

		//if (!foutfile) {
			
		foutfile = new TFile(filename,"RECREATE");
		for(std::map<std::string,TH1* >::const_iterator ih=h1.begin(); ih!=h1.end(); ++ih){
			TH1 *htemp = ih->second;
			htemp->Write();
		}
		for(std::map<std::string,TH2* >::const_iterator ih=h2.begin(); ih!=h2.end(); ++ih){
			TH2 *htemp = ih->second;
			htemp->Write();
		}
		
		foutfile->Write();
		foutfile->Close();
	};
		
};

#endif

#ifdef plot_summary_cxx
plot_summary::plot_summary(TString filename)
{
	//ffile = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
	//ffile = new TFile(filename);
	//TTree *tree = (TTree*)gDirectory->Get("summary");
		
	TChain *tree = new TChain("summary");
	tree->Add(filename);
	
	fBTagSummary[0] = new BTagSummary();
	//fBTagSummary[1] = new BTagSummary();
	//fBTagSummary[2] = new BTagSummary();

		
	//cout << " file loaded and objects created" << endl;
	Init(tree);
	
}
/*
plot_summary::plot_summary(TTree *tree)
{
  fBTagSummary[0] = new BTagSummary();
  //fBTagSummary[1] = new BTagSummary();
  //fBTagSummary[2] = new BTagSummary();

// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("analyze_d0_phi_3x_all.root");
      if (!f) {
         f = new TFile("analyze_d0_phi_3x_all.root");
      }
      tree = (TTree*)gDirectory->Get("summary");

   }
   fsamplename = "";
   Init(tree);
}
*/

plot_summary::~plot_summary()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

std::string itoa(int i) {
  char temp[20];
  sprintf(temp,"%d",i);
  return((std::string)temp);
}

std::string ftoa(float i, const char *format="0") {
  char temp[20];
  if ( format == "0" ) sprintf(temp,"%.2f",i);
  else sprintf(temp,format,i);
  
  return((std::string)temp);
}

std::string GetStringFlavour(int i) {
	if ( i == 5 ) { return "_b"; }
	else if ( i == 4 ) { return "_c"; }
	else if ( i>0 && i<=3 ) { return "_udsg"; }
	else if ( i == 21 ) { return "_udsg"; }
	else if ( i == 0 ) { return "ambiguous";}
	else { 
		//cout << " not defined flavour " << i << endl;
		return "";
	}
}

void NormalizeHistograms(std::vector< TH1* > hist) {

	for ( std::vector< TH1* >::size_type ihist = 0; ihist != hist.size() ; ++ihist ) {
		hist[ihist]->Scale(1./hist[ihist]->Integral());
	}
	
}

Int_t plot_summary::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t plot_summary::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TChain::Class()) return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void plot_summary::Init(TChain *tree)
{

	if (!tree) return;

	fChain = tree;
	fCurrent = -1;
	
	fChain->SetBranchAddress("TC.",&fBTagSummary[0]);
	//fChain->SetBranchAddress("summaryKVF.",&fBTagSummary[1]);
	//fChain->SetBranchAddress("summaryTKF.",&fBTagSummary[2]);

	//cout << "Init done" << endl;
}

Bool_t plot_summary::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void plot_summary::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t plot_summary::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef plot_summary_cxx
