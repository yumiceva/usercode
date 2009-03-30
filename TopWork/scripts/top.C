#define top_cxx
#include "top.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>

#include <iostream>

void top::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L top.C
//      Root > top t
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

	TH2D *h_IPS_iso_ttbar = new TH2D("h_IPS_iso_ttbar","h_IPS_iso_ttbar",100,0,1,100,0,10);
	TH2D *h_IPS_iso_qcd = new TH2D("h_IPS_iso_qcd","h_IPS_iso_qcd",100,0,1,100,0,10);
	TH2D *h_IPS_iso_all = new TH2D("h_IPS_iso_all","h_IPS_iso_all",100,0,1,100,0,10);

	double wttbar = 0.0094;
	double wqcd = 0.3907;

	double Na, Nb, Nc, Nd;
	Na = Nb = Nc = Nd = 0;
	
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

	  TString filename(fChain->GetCurrentFile()->GetName());

	  double d0sig = TMath::Abs(top_muon_d0[0])/top_muon_d0Error[0];
	  double reliso = top_muon_old_reliso[0];
	  
	  if ( top_njets >=4 ) {
		  if ( filename.Contains("TTJets") ) {
			  
			  h_IPS_iso_ttbar->Fill(top_muon_old_reliso[0],TMath::Abs(top_muon_d0[0])/top_muon_d0Error[0]);

			  if ( reliso < 0.7 && d0sig < 3 ) Nb += wttbar;
			  if ( reliso > 0.95 && d0sig > 5 ) Nc += wttbar;
			  if ( reliso < 0.7 && d0sig > 5 ) Nd += wttbar;
		  }
		  if ( filename.Contains("MuPt15") ) {

			  if ( reliso > 0.95 && d0sig < 3 ) Na += wqcd;
			  if ( reliso < 0.7 && d0sig < 3 ) Nb += wqcd;
			  if ( reliso > 0.95 && d0sig > 5 ) Nc += wqcd;
			  if ( reliso < 0.7 && d0sig > 5 ) Nd += wqcd;
			  
			  h_IPS_iso_qcd->Fill(top_muon_old_reliso[0],TMath::Abs(top_muon_d0[0])/top_muon_d0Error[0]);
		  }
	  }
   }

   h_IPS_iso_ttbar->SetXTitle("Combined Relative Isolation");
   h_IPS_iso_ttbar->SetYTitle("Impact parameter significance");

   h_IPS_iso_qcd->Scale(wqcd);
   h_IPS_iso_ttbar->Scale(wttbar);
   h_IPS_iso_ttbar->SetMarkerColor(2);
   h_IPS_iso_ttbar->SetLineColor(2);   
   h_IPS_iso_ttbar->Draw("BOX");
   
   h_IPS_iso_qcd->Draw("BOX same");

   h_IPS_iso_all->Add(h_IPS_iso_ttbar,h_IPS_iso_qcd,0.0094,0.3907);
   
   

   /*
   Na = h_IPS_iso_qcd->Integral(h_IPS_iso_qcd->GetXaxis()->FindBin(0.95),-1,0,h_IPS_iso_qcd->GetYaxis()->FindBin(3));
   Nb = h_IPS_iso_all->Integral(0, h_IPS_iso_all->GetXaxis()->FindBin(0.7),0,h_IPS_iso_all->GetYaxis()->FindBin(3));
   Nc = h_IPS_iso_all->Integral(h_IPS_iso_all->GetXaxis()->FindBin(0.95),-1,h_IPS_iso_all->GetYaxis()->FindBin(5),-1);
   Nd = h_IPS_iso_all->Integral(0, h_IPS_iso_all->GetXaxis()->FindBin(0.7),h_IPS_iso_all->GetYaxis()->FindBin(5),-1);
   */
   
   cout << " Observed Na = " << Nb*Nc/Nd << endl;
   cout << " Expected Na = " << Na << endl;
}
