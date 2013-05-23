//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 15 20:37:04 2013 by ROOT version 5.32/00
// from TTree top/top
// found on file: /uscms/home/yumiceva/lpc1/4tops2013_ntuples_v1/muons/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola.root
//////////////////////////////////////////////////////////

#ifndef SimpleAnalyzer_h
#define SimpleAnalyzer_h

#define STANDALONE

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1D.h>
#include <TString.h>
#include <TProofOutputFile.h>

// DEFINE ntuple objects
#include "Yumiceva/Top7TeV/interface/TopEventNtuple.h"
// DEFINE selectors
#include "Yumiceva/TreeAnalyzer/interface/MuonSelector.h"
#include "Yumiceva/TreeAnalyzer/interface/ElectronSelector.h"
// DEFINE PU rewighting
//#include "PhysicsTools/Utilities/interface/Lumi3DReWeighting.h"

// STD libraries
#include <map>
#include <string>
#include <vector>


class SimpleAnalyzer : public TSelector {

private:
  void            ParseInput();
  void            CreateHistograms();
  TString         fMyOpt;
  int             fChannel;
  bool            fVerbose;
  bool            fIsMC;
  TString         fSample;
  TString         fOutdir;
  vector< string > fCutLabels;
  // single histograms
  TH1F            *h1test;
  TH1D            *hcutflow;
  // histogram containers - maps
  map< string, TH1*> hmuons;
  map< string, TH1*> hjets;
  map< string, TH1*> hPVs;
  // PU weights
  //edm::Lumi3DReWeighting LumiWeights_;

public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   TopEventNtuple   *ntuple;
   TFile            *fFile;
   TProofOutputFile *fProofFile; // For optimized merging of the ntuple
   MuonSelector     fMuSelector;
   ElectronSelector fEleSelector;

   map< string, double > cutmap;

   SimpleAnalyzer(TTree * /*tree*/ =0):h1test(0),fChain(0),ntuple(),fFile(0),fProofFile(0),fMuSelector(),fEleSelector() 
     {
       fChannel = 1; //default mu+jets
       fVerbose = false;
       fIsMC = true;
       fSample = "";
       fOutdir = "";
     }
   virtual ~SimpleAnalyzer() { }
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

   ClassDef(SimpleAnalyzer,0);
};

#endif

//#ifdef SimpleAnalyzer_cxx
void SimpleAnalyzer::Init(TTree *tree)
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

Bool_t SimpleAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

//#endif // #ifdef SimpleAnalyzer_cxx
