/**_________________________________________________________________
   class:   TopAnalysis.cc
   package: Analyzer/TopAnalyzer


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopAnalysis.cc,v 1.1 2007/05/25 20:10:38 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/TopAnalyzer/test/TopAnalysis.h"



//_______________________________________________________________
TopAnalysis::TopAnalysis(TString filename) {

	fverbose = false;
	fcorrections = true;
	fkinfit = false;
	fminKFchi2 = 0.;
	fphicut = 0.;
	
	TChain *tree = new TChain("summary");
	tree->Add(filename);
	
	fevent = new TopEvent();
	
	Init(tree);

	// initialize histograms
	h_ = new TopHistograms();
	h_->Init("jets");
	h_->Init("jets","","MC");
	h_->Init("muons");
	h_->Init("muons","","MC");
	h_->Init("MET");
	h_->Init("MET","","MC");
	h_->Init("others");
	h_->Init("others","","MC");
	
}

//_______________________________________________________________
TopAnalysis::~TopAnalysis() {
	if (!fChain) return;
	delete fChain->GetCurrentFile();
}

//_______________________________________________________________
void TopAnalysis::Loop()
{
	

}


//_______________________________________________________________
Int_t TopAnalysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}

//_______________________________________________________________
Long64_t TopAnalysis::LoadTree(Long64_t entry)
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

//_______________________________________________________________
void TopAnalysis::Init(TChain *tree)
{

	if (!tree) return;

	fChain = tree;
	fCurrent = -1;
	
	fChain->SetBranchAddress("top.",&fevent);
	
}

//_______________________________________________________________
Bool_t TopAnalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

//_______________________________________________________________
void TopAnalysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

//_______________________________________________________________
Int_t TopAnalysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
