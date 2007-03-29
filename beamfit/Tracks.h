//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Aug 31 15:26:18 2006 by ROOT version 5.10/00
// from TTree Tracks/Tracks
// found on file: dphi1.root
//////////////////////////////////////////////////////////

#ifndef Tracks_h
#define Tracks_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <cstring>

class Tracks {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   Double_t        evt_beamX;
   Double_t        evt_beamY;
   Double_t        evt_beamZ;
   Double_t        evt_slopeX;
   Double_t        evt_slopeY;
   Int_t           evt_evetnum;
   Int_t           evt_runnum;
   Double_t        trk_curvature;
   Double_t        trk_d0;
   Double_t        trk_eta;
   Double_t        trk_lambda;
   Double_t        trk_pT;
   Double_t        trk_phi0;
   Double_t        trk_px;
   Double_t        trk_py;
   Double_t        trk_pz;
   Double_t        trk_sigmaCurvature;
   Double_t        trk_sigmaD0;
   Double_t        trk_sigmaLambda;
   Double_t        trk_sigmaPhi0;
   Double_t        trk_sigmaZ0;
   Double_t        trk_theta;
   Double_t        trk_z0;
   Int_t           trk_nCotAx;
   Int_t           trk_nCotSt;
   Int_t           trk_nSvxRphi;

   // List of branches
   TBranch        *b_evt;   //!
   TBranch        *b_trk;   //!
   Tracks(const char* inputfname ,const char* outputfname ,TTree *tree=0);
     //   Tracks(TTree *tree=0);
   virtual ~Tracks();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   const char* inputfname;
   const char* outputfname; 
};

#endif

#ifdef Tracks_cxx
Tracks::Tracks(const char* inputfname ,const char* fname ,TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  std::cout<<inputfname<<"   "<<outputfname<<std::endl;
   std::cout<<"hi hi" <<std::endl;
   char dir[80];
   strcpy(dir,inputfname);
   strcat(dir,":/dphi");
   std::cout<<"hi hi1   " <<dir<<std::endl;
   outputfname=fname;
   if (tree == 0) {
     TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(inputfname);
     if (!f) {
       f = new TFile(inputfname);

         f->cd(dir);
     }
      tree = (TTree*)gDirectory->Get("Tracks");

   }
   Init(tree);
}

Tracks::~Tracks()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Tracks::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Tracks::LoadTree(Long64_t entry)
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

void Tracks::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses of the tree
   // will be set. It is normaly not necessary to make changes to the
   // generated code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running with PROOF.

   // Set branch addresses
   if (tree == 0) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("evt",&evt_beamX);
   fChain->SetBranchAddress("trk",&trk_curvature);
   Notify();
}

Bool_t Tracks::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. Typically here the branch pointers
   // will be retrieved. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed.

   // Get branch pointers
   b_evt = fChain->GetBranch("evt");
   b_trk = fChain->GetBranch("trk");

   return kTRUE;
}

void Tracks::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Tracks::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Tracks_cxx
