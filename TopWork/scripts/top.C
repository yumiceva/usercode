#define top_cxx
#include "top.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>

#include <iostream>

void top::Loop(int type, int njets)
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

	// type:

	// 1: CombRelIso vs IP significance
	double binX0 = 0;
	double binX1 = 1;
	double binY0 = 0;
	double binY1 = 10;
	// define parameters to variate size of boxes
	double dx = 0.05; //variation in x
	double dy = 0.5; // variation in y
	int ndx = 6; // number of bins
	int ndy = 6; // number of bins

	// define A,B,C,D boxes
	double AX0 = 0.95; double AY0 = 0; //signal box
	double AXf = 1; double AYf = 3;

	double BX0 = 0; double BY0 = AY0;
	double BXf = 0.7; double BYf = AYf;

	double CX0 = AX0; double CY0 = 5;
	double CXf = AXf; double CYf = 10;

	double DX0 = BX0; double DY0 = CY0;
	double DXf = BXf; double DYf = CYf;

	TString titleX = "Combined Relative Isolation";
	TString titleY = "Impact Parameter significance";
	
	// 2: CombRelIso vs MET
	if ( type == 2 ) {
		binX0 = 0;
		binX1 = 1;
		binY0 = 0;
		binY1 = 200;

		dx = 0.05;
		dy = 1.;
		
		AX0 = 0.95; AY0 = 35; //signal box
		AXf = 1; AYf = 200;

		BX0 = 0.2; BY0 = AY0;
		BXf = 0.7; BYf = AYf;

		CX0 = AX0; CY0 = 10;
		CXf = AXf; CYf = 30;

		DX0 = BX0; DY0 = CY0;
		DXf = BXf; DYf = CYf;

		titleX = "Combined Relative Isolation";
		titleY = "MET";
	}
	
	// 3: IP significance vs pTrel
	if ( type == 3 ) {
		binX0 = 0;
		binX1 = 200;
		binY0 = 0;
		binY1 = 10;

		dx = 2;
		dy = 0.5;
		
		AX0 = 0.95; AY0 = 30; //signal box
		AXf = 1; AYf = 200;

		BX0 = 0; BY0 = AY0;
		BXf = 0.7; BYf = AYf;

		CX0 = AX0; CY0 = 0;
		CXf = AXf; CYf = 20;

		DX0 = BX0; DY0 = CY0;
		DXf = BXf; DYf = CYf;
		
		titleX = "p_{T}^{Rel}";
		titleY = "Impact Parameter significance";
	}

	// 4: pTrel vs MET
	if ( type == 4 ) {
		binX0 = 0;
		binX1 = 1;
		binY0 = 0;
		binY1 = 200;

		dx = 0.05;
		dy = 10;
		
	}
	
	// 5: Isolation vs Ht
	if ( type == 5 ) {
		binX0 = 0;
		binX1 = 1;
		binY0 = 0;
		binY1 = 2000;

		dx = 0.05;
		dy = 5;
		
		AX0 = 0.95; AY0 = 350; //signal box
		AXf = 1; AYf = 2000;

		BX0 = 0.2; BY0 = AY0;
		BXf = 0.7; BYf = AYf;

		CX0 = AX0; CY0 = 150;
		CXf = AXf; CYf = 300;

		DX0 = BX0; DY0 = CY0;
		DXf = BXf; DYf = CYf;

		titleX = "Combined Relative Isolation";
		titleY = "Ht";
	}
	
	TH2D *h_2d_ttbar = new TH2D("h_2d_ttbar","h_2d_ttbar",100,binX0,binX1,100,binY0,binY1);
	TH2D *h_2d_qcd = new TH2D("h_2dqcd","h_2d_qcd",100,binX0,binX1,100,binY0,binY1);
	TH2D *h_2d_all = new TH2D("h_2d_all","h_2d_all",100,binX0,binX1,100,binY0,binY1);

	
	h_2d_ttbar->SetXTitle(titleX);
	h_2d_ttbar->SetYTitle(titleY);
	
	// initial value variate x and y
	double dx0_ = BXf - dx*ndx/2;
	double dy0_ = CY0 - dy*ndy/2;

	// count
	double expNA[6][6];
	double NAqcd[6][6];
	double NBqcd[6][6];
	double NCqcd[6][6];
	double NDqcd[6][6];
	double NAw[6][6];
	double NBw[6][6];
	double NCw[6][6];
	double NDw[6][6];
	double NAt[6][6];
	double NBt[6][6];
	double NCt[6][6];
	double NDt[6][6];
	double NAz[6][6];
	double NBz[6][6];
	double NCz[6][6];
	double NDz[6][6];
	double NA[6][6];
	double NB[6][6];
	double NC[6][6];
	double ND[6][6];

	for ( int ii= 0; ii < ndx; ii++) {
		for ( int jj =0; jj < ndy; jj++) {

			NAqcd[ii][jj] = NBqcd[ii][jj] = NCqcd[ii][jj] = NDqcd[ii][jj] = expNA[ii][jj] = 0;
			NAt[ii][jj] = NBt[ii][jj] = NCt[ii][jj] = NDt[ii][jj] = 0;
			NAw[ii][jj] = NBw[ii][jj] = NCw[ii][jj] = NDw[ii][jj] = 0;
			NAz[ii][jj] = NBz[ii][jj] = NCz[ii][jj] = NDz[ii][jj] = 0;
			NA[ii][jj] = NB[ii][jj] = NC[ii][jj] = ND[ii][jj] = 0;
		}
	}
	
	// weights
	double wttbar = 0.0101;
	double wwjets = 0.0977;
	double wzjets = 0.078;
	double wqcd = 1.3161;
	
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

	  double varX = 0;
	  double varY = 0;
	  if ( type == 1 ) {
		  varX = reliso;
		  varY = d0sig;
	  }
	  if ( type == 2 ) {
		  varX = reliso;
		  varY = top_MET[0];
		  if ( d0sig >= 3 ) continue;
	  }
	  if ( type == 3 ) {
		  varX = top_muon_ptrel[0];
		  varY = d0sig;
	  }
	  if ( type == 4 ) {
		  varX = top_muon_ptrel[0];
		  varY = top_MET[0];
		  if ( d0sig >= 3 ) continue;
	  }
	  if ( type == 5 ) {
		  varX = reliso;
		  varY = top_Ht[0];
		  if ( d0sig >= 3 ) continue;
	  }

	  bool runloop = false;
	  
	  if ( njets < 4 && top_njets == njets) runloop = true;

	  if ( njets == 4 && top_njets >= njets ) runloop = true;

	  if (runloop)
	  {
		  
		  if ( filename.Contains("TTJets") ) {
			  
			  h_2d_ttbar->Fill(varX,varY);

			  for ( int ii= 0; ii < ndx; ii++) {

				  for ( int jj =0; jj < ndy; jj++) {

					  if ( varX > AX0 && varX <= AXf && varY >= AY0 && varY < AYf ) { NAt[ii][jj] += 1; }
					  if ( varX >= BX0 && varX < (dx0_ + dx* ((double)ii)) && varY >= BY0 && varY < BYf ) { NBt[ii][jj] += 1;}
					  if ( varX > CX0 && varX <= CXf && varY > (dy0_+dy*((double)jj)) && varY <= CYf ) { NCt[ii][jj] += 1;}
					  if ( varX >= DX0 && varX < (dx0_+dx*((double)ii)) && varY > (dy0_+dy*((double)jj)) && varY <= DYf ) { NDt[ii][jj] += 1;} 
				  }
			  }
			  
			  if ( varX > AX0 && varX <= AXf && varY >= AY0 && varY < AYf ) { Nttbar += wttbar; }
			  //if ( reliso < BXf && d0sig < BYf ) Nb += wttbar;
			  //if ( reliso > AX0 && d0sig > 5 ) Nc += wttbar;
			  //if ( reliso < 0.7 && d0sig > 5 ) Nd += wttbar;
		  }
		  if ( filename.Contains("MuPt15") ) {

			  for ( int ii= 0; ii < ndx; ii++) {

				  for ( int jj =0; jj < ndy; jj++) {

					  if ( varX > AX0 && varX <= AXf && varY >= AY0 && varY < AYf ) { NAqcd[ii][jj] += 1; expNA[ii][jj] += wqcd; }
					  if ( varX >= BX0 && varX < (dx0_ + dx* ((double)ii)) && varY >= BY0 && varY < BYf ) { NBqcd[ii][jj] += 1;}
					  if ( varX > CX0 && varX <= CXf && varY > (dy0_+dy*((double)jj)) && varY <= CYf ) { NCqcd[ii][jj] += 1;}
					  if ( varX >= DX0 && varX < (dx0_+dx*((double)ii)) && varY > (dy0_+dy*((double)jj)) && varY <= DYf ) { NDqcd[ii][jj] += 1;} 
				  }
			  }

			  if ( varX > AX0 && varX <= AXf && varY >= AY0 && varY < AYf ) { Nqcd+= wqcd;}
			  //if ( reliso > 0.95 && d0sig < 3 ) { Na += wqcd; Nqcd+= wqcd; }
			  //if ( reliso < 0.7 && d0sig < 3 ) Nb += wqcd;
			  //if ( reliso > 0.95 && d0sig > 5 ) Nc += wqcd;
			  //if ( reliso < 0.7 && d0sig > 5 ) Nd += wqcd;
			  
			  h_2d_qcd->Fill(varX,varY);
		  }
		  if ( filename.Contains("WJets") ) {

			  for ( int ii= 0; ii < ndx; ii++) {

				  for ( int jj =0; jj < ndy; jj++) {

					  if ( varX > AX0 && varX <= AXf && varY >= AY0 && varY < AYf ) { NAw[ii][jj] += 1;}
					  if ( varX >= BX0 && varX < (dx0_ + dx* ((double)ii)) && varY >= BY0 && varY < BYf ) { NBw[ii][jj] += 1;}
					  if ( varX > CX0 && varX <= CXf && varY > (dy0_+dy*((double)jj)) && varY <= CYf ) { NCw[ii][jj] += 1;}
					  if ( varX >= DX0 && varX < (dx0_+dx*((double)ii)) && varY > (dy0_+dy*((double)jj)) && varY <= DYf ) { NDw[ii][jj] += 1;} 
				  }
			  }

			  if ( varX > AX0 && varX <= AXf && varY >= AY0 && varY < AYf ) { NWjets += wwjets;}
			  
			  //if ( reliso > 0.95 && d0sig < 3 ) { NWjets += wwjets; }
			  
		  }
		  if ( filename.Contains("ZJets") ) {
			  
			  for ( int ii= 0; ii < ndx; ii++) {

				  for ( int jj =0; jj < ndy; jj++) {

					  if ( varX > AX0 && varX <= AXf && varY >= AY0 && varY < AYf ) { NAz[ii][jj] += 1; }
					  if ( varX >= BX0 && varX < (dx0_ + dx* ((double)ii)) && varY >= BY0 && varY < BYf ) { NBz[ii][jj] += 1;}
					  if ( varX > CX0 && varX <= CXf && varY > (dy0_+dy*((double)jj)) && varY <= CYf ) { NCz[ii][jj] += 1;}
					  if ( varX >= DX0 && varX < (dx0_+dx*((double)ii)) && varY > (dy0_+dy*((double)jj)) && varY <= DYf ) { NDz[ii][jj] += 1;} 
				  }
			  }

			  if ( varX > AX0 && varX <= AXf && varY >= AY0 && varY < AYf ) { NZjets += wzjets;}
			  //if ( reliso > 0.95 && d0sig < 3 ) { NZjets += wzjets; }
		  }
	  }
   }

   TMultiGraph *mg = new TMultiGraph();
   double markers[6] = {22, 23, 24, 25, 26, 27};
   double colors[6] = {1, 2, 3, 4, 5, 6};
   TLegend *legend = new TLegend(0.6,0.2,0.8,0.4);
   legend->SetMargin(0.12);
   legend->SetTextSize(0.035);
   legend->SetFillColor(10);
   legend->SetBorderSize(0);

   double theNa = 0;
   double theNaErr = 0;
   
   for ( int ii= 0; ii < ndx; ii++) {

	   double xaxis[6];
	   double yaxis[6];
	   double ratio[6];
	   double errorY[6];
	   double errorX[6];
	   
	   yaxis[ii] = (dx0_ + dx* ((double)ii));
	   cout << " ii = " << ii << " yaxis[ii] = " << yaxis[ii] << endl;

	   for ( int jj =0; jj < ndy; jj++) {

		   NB[ii][jj] = wttbar*NBt[ii][jj] + wwjets*NBw[ii][jj] + wzjets*NBz[ii][jj] + wqcd*NBqcd[ii][jj];
		   NC[ii][jj] = wttbar*NCt[ii][jj] + wwjets*NCw[ii][jj] + wzjets*NCz[ii][jj] + wqcd*NCqcd[ii][jj];
		   ND[ii][jj] = wttbar*NDt[ii][jj] + wwjets*NDw[ii][jj] + wzjets*NDz[ii][jj] + wqcd*NDqcd[ii][jj];

		   double deltaNB = sqrt(wttbar*wttbar*NBt[ii][jj] + wwjets*wwjets*NBw[ii][jj] + wzjets*wzjets*NBz[ii][jj] + wqcd*wqcd*NBqcd[ii][jj]);
		   double deltaNC = sqrt(wttbar*wttbar*NCt[ii][jj] + wwjets*wwjets*NCw[ii][jj] + wzjets*wzjets*NCz[ii][jj] + wqcd*wqcd*NCqcd[ii][jj]);
		   double deltaND = sqrt(wttbar*wttbar*NDt[ii][jj] + wwjets*wwjets*NDw[ii][jj] + wzjets*wzjets*NDz[ii][jj] + wqcd*wqcd*NDqcd[ii][jj]);
		   
		   ratio[jj] = (NB[ii][jj]*NC[ii][jj])/(expNA[ii][jj]*ND[ii][jj]);

		   errorY[jj] = sqrt( pow(NC[ii][jj]/ND[ii][jj],2)*deltaNB*deltaNB + pow(NB[ii][jj]/ND[ii][jj],2)*deltaNC*deltaNC +
							  pow((NB[ii][jj]*NC[ii][jj])/(ND[ii][jj]*ND[ii][jj]),2)*deltaND*deltaND )/expNA[ii][jj];
		   //errorY[jj] = (sqrt(NB[ii][jj]*NC[ii][jj])/(expNA[ii][jj]*ND[ii][jj]))*sqrt(NC[ii][jj]+NB[ii][jj]+
//																				   NB[ii][jj]*NC[ii][jj]/expNA[ii][jj]+NB[ii][jj]*NC[ii][jj]/ND[ii][jj]);
		   errorX[jj] = 0.;
		   xaxis[jj] = dy0_ + dy*jj;

		   //double obsNa_error = sqrt( ( NC[ii][jj] + NB[ii][jj] + NB[ii][jj]*NC[ii][jj]/ND[ii][jj] ) * NB[ii][jj]*NC[ii][jj]/(ND[ii][jj]*ND[ii][jj]) );
		   double obsNa_error = sqrt( pow(NC[ii][jj]/ND[ii][jj],2)*deltaNB*deltaNB + pow(NB[ii][jj]/ND[ii][jj],2)*deltaNC*deltaNC +
									  pow((NB[ii][jj]*NC[ii][jj])/(ND[ii][jj]*ND[ii][jj]),2)*deltaND*deltaND );
		   // Observed Na
		   double obsNa =  NB[ii][jj]*NC[ii][jj]/ND[ii][jj];
				
		   cout << " NA[" << ii << "," << jj << "]=" << NA[ii][jj] << endl;
		   cout << " NB[" << ii << "," << jj << "]=" << NB[ii][jj] << endl;
		   cout << " NC[" << ii << "," << jj << "]=" << NC[ii][jj] << endl;
		   cout << " ND[" << ii << "," << jj << "]=" << ND[ii][jj] << endl;
		   cout << "  jj = " << jj << " ratio = " << ratio[jj] << " xaxis = " << xaxis[jj] << endl;
		   cout << "  NA expected = " << expNA[ii][jj] << " NA observed = " << obsNa << " \\pm " << obsNa_error << endl;
		   if (ii==3 && jj == 3) {
			   theNa = obsNa;
			   theNaErr = obsNa_error;
		   }
	   }

	   TGraphErrors *gr = new TGraphErrors(6,xaxis, ratio, errorX, errorY);
	   gr->SetMarkerStyle(markers[ii]);
	   gr->SetMarkerColor(colors[ii]);
	   gr->SetLineColor(colors[ii]);
	   //gr->Draw("AP");
	   
	   char histname[20];
	   sprintf(histname, "CombRelIso > %0.2f", (dx0_ + dx* ((double)ii)) );
	   legend->AddEntry( gr, histname, "p");
	   mg->Add( gr, "p" );
	   
   }

   TCanvas *cv = new TCanvas("cv","cv",600,600);
   
   mg->Draw("a");
   mg->GetHistogram()->SetXTitle(titleY);
   legend->Draw();
   
   TCanvas *cv1 = new TCanvas("cv1","cv1",600,600);
   
   
   h_2d_qcd->Scale(wqcd);
   h_2d_ttbar->Scale(wttbar);
   h_2d_ttbar->SetMarkerColor(2);
   h_2d_ttbar->SetLineColor(2);   
   h_2d_ttbar->Draw("BOX");
   h_2d_qcd->Draw("BOX same");

   //h_2d_all->Add(h_IPS_iso_ttbar,h_IPS_iso_qcd);
   
   

   /*
   Na = h_IPS_iso_qcd->Integral(h_IPS_iso_qcd->GetXaxis()->FindBin(0.95),-1,0,h_IPS_iso_qcd->GetYaxis()->FindBin(3));
   Nb = h_IPS_iso_all->Integral(0, h_IPS_iso_all->GetXaxis()->FindBin(0.7),0,h_IPS_iso_all->GetYaxis()->FindBin(3));
   Nc = h_IPS_iso_all->Integral(h_IPS_iso_all->GetXaxis()->FindBin(0.95),-1,h_IPS_iso_all->GetYaxis()->FindBin(5),-1);
   Nd = h_IPS_iso_all->Integral(0, h_IPS_iso_all->GetXaxis()->FindBin(0.7),h_IPS_iso_all->GetYaxis()->FindBin(5),-1);
   */
/*
   double obsNa_error = sqrt( ( Nc + Nb + Nb*Nc/Nd ) * Nb*Nc/(Nd*Nd) );
   
   cout << " Observed Na = " << Nb*Nc/Nd << " \\pm " << obsNa_error << endl;
   cout << " Expected Na = " << Na << endl;
*/
   
   cout << "\n" << " N ttbar = " << Nttbar << endl;
   cout << " N Wjets = " << NWjets << endl;
   cout << " N Zjets = " << NZjets << endl;
   cout << " N qcd = " << Nqcd << endl;

   cout << "\n" << " NA expected = " << expNA[0][0] << " NA observed = " << theNa << " \\pm " << theNaErr << endl;

}
