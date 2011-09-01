//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 25 13:10:09 2011 by ROOT version 5.27/06b
// from TTree top/top
// found on file: /uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/41x/v2/SingleMu2011A_v1.root
//////////////////////////////////////////////////////////

#ifndef Analyzer_h
#define Analyzer_h

#include <TROOT.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TProofOutputFile.h>
const Int_t kMaxtop = 1;

#include "Yumiceva/Top7TeV/interface/TopEventNtuple.h"
#include "Yumiceva/TreeAnalyzer/interface/MuonSelector.h"
#include "Yumiceva/TreeAnalyzer/interface/ElectronSelector.h"
//#include "Yumiceva/TreeAnalyzer/interface/HistoManager.h"
#include "Yumiceva/TreeAnalyzer/interface/METzCalculator.h"

#include <map>
#include <string>
#include <vector>

class Analyzer : public TSelector {

private:
  void            ParseInput();
  TString         fMyOpt;
  int             fChannel;
  bool            fVerbose;
  bool            fIsMC;
  bool            fdoQCD1SideBand;
  bool            fdoQCD2SideBand;
  //HistoManager    *fHist;
  TString         fSample;
  TH1F            *h1test;
  TH1D            *hcutflow;
  map< string, TH1*> hmuons;
  map< string, TH1*> helectrons;
  map< string, TH1*> hjets;
  map< string, TH1*> hPVs;
  map< string, TH1*> hMET;
  map< string, TH1*> hM;
  vector< string > fCutLabels;
  vector< double > fpu_weights_vec;
  METzCalculator fzCalculator;

public :

   TTree            *fChain;   //!pointer to the analyzed TTree or TChain
   TopEventNtuple   *ntuple;
   TFile            *fFile;
   TProofOutputFile *fProofFile; // For optimized merging of the ntuple
   MuonSelector     fMuSelector;
   ElectronSelector fEleSelector;
   map< string, double > cutmap;

   Analyzer(TTree * /*tree*/ =0):h1test(0),fChain(0),ntuple(),fFile(0),fProofFile(0),fMuSelector(),fEleSelector() 
     {
       fChannel = 1; //default mu+jets
       fVerbose = false;
       fIsMC = true;
       fSample = "";
       fdoQCD1SideBand = false;
       fdoQCD2SideBand = false;
     }
   virtual ~Analyzer() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();
   
   ClassDef(Analyzer,0);
};

#endif


//#ifdef Analyzer_cxx
void Analyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   Info("Init","tree: %p", tree);

   // Set branch addresses and branch pointers
   if (!tree) return;
   
   fChain = tree;
   fChain->SetBranchAddress("top.", &ntuple); 
}

Bool_t Analyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

//#endif // #ifdef Analyzer_cxx
