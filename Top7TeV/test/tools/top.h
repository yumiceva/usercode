//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Aug 25 15:43:16 2010 by ROOT version 5.22/00d
// from TTree top/top
// found on file: /uscms/home/samvel/Code/ttmuj/NtupleMaker/CMSSW_3_6_3/src/Top/Production/test/prod_25_Aug_2010_1/ntuple_patskim.root
//////////////////////////////////////////////////////////

#ifndef top_h
#define top_h

#include <vector>
#include <iostream>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
   const Int_t kMaxtop = 1;

#include "TopEventNtuple.h"

using namespace std;

class top {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   TopEventNtuple  *ntuple;

   // List of branches
   //TBranch        *b_top_ntuple;   //!

   top(TTree *tree=0);
   virtual ~top();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef top_cxx
top::top(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/1.34pb-1/ttmuj_data_Aug25.root");
      if (!f) {
         f = new TFile("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/1.34pb-1/ttmuj_data_Aug25.root");
         f->cd("/PATNtupleMaker");
      }
      tree = (TTree*)gDirectory->Get("top");
      ntuple = new TopEventNtuple();
   }
   cout << "got tree" << endl;
   Init(tree);
}

top::~top()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t top::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t top::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void top::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   //   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("top.", &ntuple);

   Notify();
}

Bool_t top::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void top::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t top::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef top_cxx
