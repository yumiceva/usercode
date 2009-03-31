#define top_cxx
#include "top.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TMultigraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>

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

	// define parameters to variate size of boxes
	double dx = 0.05; //variation in x
	double dy = 0.5; // variation in y
	int ndx = 6; // number of bins
	int ndy = 6; // number of bins

	// define A,B,C,D boxes
	double AX0 = 0.9; double AY0 = 0;
	double AXf = 1; double AYf = 3;

	double BX0 = 0; double BY0 = 0;
	double BXf = 0.7; double BYf = AYf;

	double CX0 = AX0; double CY0 = 5;
	double CXf = AXf; double CYf = 10;

	double DX0 = BX0; double DY0 = CY0;
	double DXf = BXf; double DYf = CYf;

	// initial value variate x and y
	double dx0_ = BXf - dx*ndx/2;
	double dy0_ = CY0 - dy*ndy/2;

	// count
	double NA[6][6];
	double NB[6][6];
	double NC[6][6];
	double ND[6][6];

	for ( int ii= 0; ii < ndx; ii++) {
		for ( int jj =0; jj < ndy; jj++) {

			NA[ii][jj] = NB[ii][jj] = NC[ii][jj] = ND[ii][jj] = 0;
		}
	}
	
	// weights
	double wttbar = 0.0094;
	double wqcd = 0.3907;
	double wwjets = 0.0177;
	double wzjets = 0.041;
	
	double Na, Nb, Nc, Nd;
	Na = Nb = Nc = Nd = 0;

	double Nttbar, NWjets, NZjets, Nqcd;
	Nttbar = NWjets = NZjets = Nqcd = 0;
	
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

			  for ( int ii= 0; ii < ndx; ii++) {

				  for ( int jj =0; jj < ndy; jj++) {

					  //if ( reliso > AX0 && reliso <= AXf && d0sig >= AY0 && d0sig < AYf ) { NA[ii][jj] += wttbar; }
					  if ( reliso >= BX0 && reliso < (dx0_ + dx* ii) && d0sig >= BY0 && d0sig < BYf ) { NB[ii][jj] += wttbar;}
					  if ( reliso > CX0 && reliso <= CXf && d0sig > (dy0_+dy*jj) && d0sig <= CYf ) { NC[ii][jj] += wttbar;}
					  if ( reliso >= DX0 && reliso < (dx0_+dx*ii) && d0sig > (dy0_+dy*jj) && d0sig <= DYf ) { ND[ii][jj] += wttbar;} 
				  }
			  }
			  
			  if ( reliso > 0.95 && d0sig < 3 ) { Nttbar += wttbar; }
			  if ( reliso < 0.7 && d0sig < 3 ) Nb += wttbar;
			  if ( reliso > 0.95 && d0sig > 5 ) Nc += wttbar;
			  if ( reliso < 0.7 && d0sig > 5 ) Nd += wttbar;
		  }
		  if ( filename.Contains("MuPt15") ) {

			  for ( int ii= 0; ii < ndx; ii++) {

				  for ( int jj =0; jj < ndy; jj++) {

					  if ( reliso > AX0 && reliso <= AXf && d0sig >= AY0 && d0sig < AYf ) { NA[ii][jj] += wqcd; }
					  if ( reliso >= BX0 && reliso < (dx0_ + dx* ii) && d0sig >= BY0 && d0sig < BYf ) { NB[ii][jj] += wqcd;}
					  if ( reliso > CX0 && reliso <= CXf && d0sig > (dy0_+dy*jj) && d0sig <= CYf ) { NC[ii][jj] += wqcd;}
					  if ( reliso >= DX0 && reliso < (dx0_+dx*ii) && d0sig > (dy0_+dy*jj) && d0sig <= DYf ) { ND[ii][jj] += wqcd;} 
				  }
			  }

			  if ( reliso > 0.95 && d0sig < 3 ) { Na += wqcd; Nqcd+= wqcd; }
			  if ( reliso < 0.7 && d0sig < 3 ) Nb += wqcd;
			  if ( reliso > 0.95 && d0sig > 5 ) Nc += wqcd;
			  if ( reliso < 0.7 && d0sig > 5 ) Nd += wqcd;
			  
			  h_IPS_iso_qcd->Fill(top_muon_old_reliso[0],TMath::Abs(top_muon_d0[0])/top_muon_d0Error[0]);
		  }
		  if ( filename.Contains("WJets") ) {

			  if ( reliso > 0.95 && d0sig < 3 ) { NWjets += wwjets; }
			  
		  }
		  if ( filename.Contains("ZJets") ) {
			  
			  if ( reliso > 0.95 && d0sig < 3 ) { NZjets += wzjets; }
		  }
	  }
   }

   TMultiGraph *mg = new TMultiGraph();
   
   for ( int ii= 0; ii < ndx; ii++) {

	   double xaxis[6];
	   double ratio[6];
	   double errorY[6];
	   double errorX[6];

	   cout << "ii= " << ii << endl;
	   
	   for ( int jj =0; jj < ndy; jj++) {

		   ratio[jj] = (NB[ii][jj]*NC[ii][jj])/(NA[ii][jj]*ND[ii][jj]);
		   errorY[jj] = sqrt( pow(sqrt(NB[ii][jj])*NC[ii][jj]/(NA[ii][jj]*ND[ii][jj]),2) +
							  pow(sqrt(NC[ii][jj])*NB[ii][jj]/(NA[ii][jj]*ND[ii][jj]),2) +
							  pow(sqrt(NA[ii][jj])*NB[ii][jj]*NC[ii][jj]/(NA[ii][jj]*NA[ii][jj]*ND[ii][jj]),2) +
							  pow(sqrt(ND[ii][jj])*NB[ii][jj]*NC[ii][jj]/(NA[ii][jj]*ND[ii][jj]*ND[ii][jj]),2) );
		   errorX[jj] = 0.;
		   xaxis[jj] = dy0_ + dy*jj;

		   cout << " NA[" << ii << "," << jj << "]=" << NA[ii][jj] << endl;
		   cout << " NB[" << ii << "," << jj << "]=" << NB[ii][jj] << endl;
		   cout << " NC[" << ii << "," << jj << "]=" << NC[ii][jj] << endl;
		   cout << " ND[" << ii << "," << jj << "]=" << ND[ii][jj] << endl;
		   cout << "  jj = " << jj << " ratio = " << ratio[jj] << " xaxis = " << xaxis[jj] << endl;
		   
	   }

	   TGraphErrors *gr = new TGraphErrors(6,xaxis, ratio, errorX, errorY);
	   gr->Draw("AP");
	   mg->Add( gr, "p" );
	   
   }

   TCanvas *cv = new TCanvas("cv","cv",600,600);
   mg->Draw("a");

   TCanvas *cv1 = new TCanvas("cv1","cv1",600,600);
   
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

   cout << "\n" << " N ttbar = " << Nttbar << endl;
   cout << " N qcd = " << Nqcd << endl;
   cout << " N Wjets = " << NWjets << endl;
   cout << " N Zjets = " << NZjets << endl;
   
}
