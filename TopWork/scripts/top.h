//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Mar 28 22:49:27 2009 by ROOT version 5.22/00
// from TTree top/top
// found on file: nominalABCD/ABCD_TTJets_all.root
//////////////////////////////////////////////////////////

#ifndef top_h
#define top_h

#include <vector>
using namespace std;

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
   const Int_t kMaxtop = 1;

class top {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
 //BooEventNtuple  *top.;
   Int_t           top_event;
   Int_t           top_run;
   Int_t           top_dataType;
   Int_t           top_njets;
   Int_t           top_nmuons;
   Int_t           top_nvertices;
   Int_t           top_ngenjets;
   vector<float>   top_jet_pt;
   vector<float>   top_jet_eta;
   vector<float>   top_jet_phi;
   vector<float>   top_jet_e;
   vector<float>   top_jet_et;
   vector<int>     top_jet_ntrks;
   vector<int>     top_jet_flavour;
   vector<float>   top_jetcorrection;
   vector<float>   top_genjet_pt;
   vector<float>   top_genjet_eta;
   vector<float>   top_genjet_phi;
   vector<float>   top_genjet_e;
   vector<float>   top_muon_px;
   vector<float>   top_muon_py;
   vector<float>   top_muon_pz;
   vector<float>   top_muon_e;
   vector<float>   top_muon_normchi2;
   vector<float>   top_muon_d0;
   vector<float>   top_muon_d0Error;
   vector<float>   top_muon_old_reliso;
   vector<float>   top_muon_new_reliso;
   vector<float>   top_muon_ptrel;
   vector<float>   top_MET;
   vector<float>   top_Ht;
   vector<float>   top_genmuon_px;
   vector<float>   top_genmuon_py;
   vector<float>   top_genmuon_pz;
   vector<float>   top_genmuon_e;
   vector<int>     top_genmuon_pdg;
   vector<int>     top_genmoun_motherpdg;
   vector<float>   top_gentop_px;
   vector<float>   top_gentop_py;
   vector<float>   top_gentop_pz;
   vector<float>   top_gentop_e;
   vector<float>   top_gentop_charge;
   vector<int>     top_gentop_hadronic;
   vector<float>   top_gennu_px;
   vector<float>   top_gennu_py;
   vector<float>   top_gennu_pz;
   vector<float>   top_gennu_e;
   vector<int>     top_gennu_pdg;

   // List of branches
   TBranch        *b_top_event;   //!
   TBranch        *b_top_run;   //!
   TBranch        *b_top_dataType;   //!
   TBranch        *b_top_njets;   //!
   TBranch        *b_top_nmuons;   //!
   TBranch        *b_top_nvertices;   //!
   TBranch        *b_top_ngenjets;   //!
   TBranch        *b_top_jet_pt;   //!
   TBranch        *b_top_jet_eta;   //!
   TBranch        *b_top_jet_phi;   //!
   TBranch        *b_top_jet_e;   //!
   TBranch        *b_top_jet_et;   //!
   TBranch        *b_top_jet_ntrks;   //!
   TBranch        *b_top_jet_flavour;   //!
   TBranch        *b_top_jetcorrection;   //!
   TBranch        *b_top_genjet_pt;   //!
   TBranch        *b_top_genjet_eta;   //!
   TBranch        *b_top_genjet_phi;   //!
   TBranch        *b_top_genjet_e;   //!
   TBranch        *b_top_muon_px;   //!
   TBranch        *b_top_muon_py;   //!
   TBranch        *b_top_muon_pz;   //!
   TBranch        *b_top_muon_e;   //!
   TBranch        *b_top_muon_normchi2;   //!
   TBranch        *b_top_muon_d0;   //!
   TBranch        *b_top_muon_d0Error;   //!
   TBranch        *b_top_muon_old_reliso;   //!
   TBranch        *b_top_muon_new_reliso;   //!
   TBranch        *b_top_muon_ptrel;   //!
   TBranch        *b_top_MET;   //!
   TBranch        *b_top_Ht;   //!
   TBranch        *b_top_genmuon_px;   //!
   TBranch        *b_top_genmuon_py;   //!
   TBranch        *b_top_genmuon_pz;   //!
   TBranch        *b_top_genmuon_e;   //!
   TBranch        *b_top_genmuon_pdg;   //!
   TBranch        *b_top_genmoun_motherpdg;   //!
   TBranch        *b_top_gentop_px;   //!
   TBranch        *b_top_gentop_py;   //!
   TBranch        *b_top_gentop_pz;   //!
   TBranch        *b_top_gentop_e;   //!
   TBranch        *b_top_gentop_charge;   //!
   TBranch        *b_top_gentop_hadronic;   //!
   TBranch        *b_top_gennu_px;   //!
   TBranch        *b_top_gennu_py;   //!
   TBranch        *b_top_gennu_pz;   //!
   TBranch        *b_top_gennu_e;   //!
   TBranch        *b_top_gennu_pdg;   //!

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
		/*
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("nominalABCD/ABCD_TTJets_all.root");
      if (!f) {
         f = new TFile("nominalABCD/ABCD_TTJets_all.root");
      }
      tree = (TTree*)gDirectory->Get("top");

   }
		*/
	TChain * chain = new TChain("top","");
	chain->Add("nominalABCD/ABCD_TTJets_all.root/top");
	chain->Add("nominalABCD/ABCD_MuPt15_all.root/top");
		//chain->Add("nominalABCD/ABCD_WJets_all.root/top");
		//chain->Add("nominalABCD/ABCD_ZJets_all.root/top");
	
	tree = chain;

   
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
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("top.event", &top_event, &b_top_event);
   fChain->SetBranchAddress("top.run", &top_run, &b_top_run);
   fChain->SetBranchAddress("top.dataType", &top_dataType, &b_top_dataType);
   fChain->SetBranchAddress("top.njets", &top_njets, &b_top_njets);
   fChain->SetBranchAddress("top.nmuons", &top_nmuons, &b_top_nmuons);
   fChain->SetBranchAddress("top.nvertices", &top_nvertices, &b_top_nvertices);
   fChain->SetBranchAddress("top.ngenjets", &top_ngenjets, &b_top_ngenjets);
   fChain->SetBranchAddress("top.jet_pt", &top_jet_pt, &b_top_jet_pt);
   fChain->SetBranchAddress("top.jet_eta", &top_jet_eta, &b_top_jet_eta);
   fChain->SetBranchAddress("top.jet_phi", &top_jet_phi, &b_top_jet_phi);
   fChain->SetBranchAddress("top.jet_e", &top_jet_e, &b_top_jet_e);
   fChain->SetBranchAddress("top.jet_et", &top_jet_et, &b_top_jet_et);
   fChain->SetBranchAddress("top.jet_ntrks", &top_jet_ntrks, &b_top_jet_ntrks);
   fChain->SetBranchAddress("top.jet_flavour", &top_jet_flavour, &b_top_jet_flavour);
   fChain->SetBranchAddress("top.jetcorrection", &top_jetcorrection, &b_top_jetcorrection);
   fChain->SetBranchAddress("top.genjet_pt", &top_genjet_pt, &b_top_genjet_pt);
   fChain->SetBranchAddress("top.genjet_eta", &top_genjet_eta, &b_top_genjet_eta);
   fChain->SetBranchAddress("top.genjet_phi", &top_genjet_phi, &b_top_genjet_phi);
   fChain->SetBranchAddress("top.genjet_e", &top_genjet_e, &b_top_genjet_e);
   fChain->SetBranchAddress("top.muon_px", &top_muon_px, &b_top_muon_px);
   fChain->SetBranchAddress("top.muon_py", &top_muon_py, &b_top_muon_py);
   fChain->SetBranchAddress("top.muon_pz", &top_muon_pz, &b_top_muon_pz);
   fChain->SetBranchAddress("top.muon_e", &top_muon_e, &b_top_muon_e);
   fChain->SetBranchAddress("top.muon_normchi2", &top_muon_normchi2, &b_top_muon_normchi2);
   fChain->SetBranchAddress("top.muon_d0", &top_muon_d0, &b_top_muon_d0);
   fChain->SetBranchAddress("top.muon_d0Error", &top_muon_d0Error, &b_top_muon_d0Error);
   fChain->SetBranchAddress("top.muon_old_reliso", &top_muon_old_reliso, &b_top_muon_old_reliso);
   fChain->SetBranchAddress("top.muon_new_reliso", &top_muon_new_reliso, &b_top_muon_new_reliso);
   fChain->SetBranchAddress("top.muon_ptrel", &top_muon_ptrel, &b_top_muon_ptrel);
   fChain->SetBranchAddress("top.MET", &top_MET, &b_top_MET);
   fChain->SetBranchAddress("top.Ht", &top_Ht, &b_top_Ht);
   fChain->SetBranchAddress("top.genmuon_px", &top_genmuon_px, &b_top_genmuon_px);
   fChain->SetBranchAddress("top.genmuon_py", &top_genmuon_py, &b_top_genmuon_py);
   fChain->SetBranchAddress("top.genmuon_pz", &top_genmuon_pz, &b_top_genmuon_pz);
   fChain->SetBranchAddress("top.genmuon_e", &top_genmuon_e, &b_top_genmuon_e);
   fChain->SetBranchAddress("top.genmuon_pdg", &top_genmuon_pdg, &b_top_genmuon_pdg);
   fChain->SetBranchAddress("top.genmoun_motherpdg", &top_genmoun_motherpdg, &b_top_genmoun_motherpdg);
   fChain->SetBranchAddress("top.gentop_px", &top_gentop_px, &b_top_gentop_px);
   fChain->SetBranchAddress("top.gentop_py", &top_gentop_py, &b_top_gentop_py);
   fChain->SetBranchAddress("top.gentop_pz", &top_gentop_pz, &b_top_gentop_pz);
   fChain->SetBranchAddress("top.gentop_e", &top_gentop_e, &b_top_gentop_e);
   fChain->SetBranchAddress("top.gentop_charge", &top_gentop_charge, &b_top_gentop_charge);
   fChain->SetBranchAddress("top.gentop_hadronic", &top_gentop_hadronic, &b_top_gentop_hadronic);
   fChain->SetBranchAddress("top.gennu_px", &top_gennu_px, &b_top_gennu_px);
   fChain->SetBranchAddress("top.gennu_py", &top_gennu_py, &b_top_gennu_py);
   fChain->SetBranchAddress("top.gennu_pz", &top_gennu_pz, &b_top_gennu_pz);
   fChain->SetBranchAddress("top.gennu_e", &top_gennu_e, &b_top_gennu_e);
   fChain->SetBranchAddress("top.gennu_pdg", &top_gennu_pdg, &b_top_gennu_pdg);
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
