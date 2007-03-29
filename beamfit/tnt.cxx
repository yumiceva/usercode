#define tnt_cxx
#include "tnt.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>

void tnt::Book()
{

  hvxy = new TH2F("vxy" ,"x vers. y vertex",100,-0.01,0.01,100,-0.01,0.01);
  hvxyz= new TH3F("vxyz" ,"x y z vertex"   ,100,-0.01,0.01,200,-40.,40.,100,-0.01,0.01);
  hdphi= new TH2F("dphi","d vers phi",100,-0.,6.283,100,-0.2,0.2);
  hd = new TH2F("d" ,"d vers. z ",100,-40.,40.,100,-0.01,0.01); 
  hvxz = new TH2F("vxz" ,"x vers. z vertex",100,-40.,40.,100,-0.01,0.01);
  hsigma  = new TH1F("sigma"  ,"sigma d "   ,100,0.,0.01);
  hsx  = new TH1F("sx"  ,"sigma vers. z"   ,50,-40.,40.);
  hpt  = new TH1F("pt"  ,"pt"   ,100,0.,20.);
  hsx  -> Sumw2();
  hsd  = new TH1F("sd"  ,"d0 vers. z"   ,50,-40.,40.);
  hsd  -> Sumw2();
  hsw  = new TH1F("sw"  ,"sigma weight vers. z"   ,50,-40.,40.);
  hsw  -> Sumw2();
}
zData  tnt::Loop(int maxEvents)
{
//   In a ROOT session, you can do:
//      Root > .L tnt.C
//      Root > tnt t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
//   TH2F *hvxy = new TH2F("vxy","x vers. y vertex",100,-0.4,0.4,100,-0.4,0.4);
  zData zvector;
   if (fChain == 0) return zvector;
   zvector.erase(zvector.begin(),zvector.end());
   Long64_t nentries = fChain->GetEntriesFast();

   Int_t nbytes = 0, nb = 0;
   int theevent = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      //      if (sigmaD <0.05&&pt>4.0)
      //	{
      
      //if (z0>-20. && z0<20. && pt>4 ) {
	  if (pt>2 ) {
		  if (maxEvents>0 && maxEvents == theevent) break;
		  hvxy->Fill(x,y);
		  hvxyz->Fill(x,z0,y);
		  hdphi->Fill(phi,d0);
		  hvxz->Fill(z0,x);
		  hsd->Fill(z0,fabs(d0));
          hd->Fill(z0,d0);  
		  hsx->Fill(z0,fabs(x));
		  hpt->Fill(pt,1.0);
		  hsw->Fill(z0,1.0);   
          hsigma->Fill(sigmaD);

		  // for reco ntuples:
		  if (pt>3 && nStripHit>=9 && nPixelHit>=3 && std::abs(d0)<0.08 &&
			  (chi2/ndof)<5 && TMath::Prob(chi2, (int)ndof)>0.02 && std::abs(eta)<2.2 ) {
			  zvector.push_back(data(z0,sigmaz0,-d0,sigmaD,phi,pt,1.));
		  }
		  theevent++;
      }
	  // if (Cut(ientry) < 0) continue;
	  //	}
   }
   std::cout << zvector.size() << " tracks read in.\n";
   hsx->Divide(hsw);
   hsd->Divide(hsw);
   return zvector;
}
