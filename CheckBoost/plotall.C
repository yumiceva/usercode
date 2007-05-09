#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"


bool SortByMaximum( TH1* lh, TH1* rh ) {

        return lh->GetMaximum() > rh->GetMaximum();

}


void plotall() {

	TFile *files[4];
	
	files[0]   = new TFile("VtxTest_qcd_gauss.root");
	files[1]   = new TFile("VtxTest_qcd_beta_nocrossing.root");
	files[2]   = new TFile("VtxTest_qcd_beta_crossing.root");
	files[3]   = new TFile("VtxTest_qcd_beta_crossing_with_cms.root");

	TH1D *hVtxX[4];
	TH1D *hVtxY[4];
	TH1D *hVtxZ[4];
	TH2D *hVtxXY[4];
	TH1D *hPx[4];
	TH1D *hPy[4];
	TH1D *hPz[4];
	TH1D *hPt[4];

	int color_line[4] = {1,2,3,4};
	
	for (int i=0; i<4; i++ ) {
		files[i]->cd();
		hVtxX[i] = (TH1D*) gDirectory->Get("VtxHistx");
		hVtxY[i] = (TH1D*) gDirectory->Get("VtxHisty");
		hVtxZ[i] = (TH1D*) gDirectory->Get("VtxHistz");
		hVtxXY[i] = (TH2D*) gDirectory->Get("VtxHistxy");

		hPx[i] = (TH1D*) gDirectory->Get("pxHist");
		hPy[i] = (TH1D*) gDirectory->Get("pyHist");
		hPz[i] = (TH1D*) gDirectory->Get("pzHist");
		hPt[i] = (TH1D*) gDirectory->Get("ptHist");

		hVtxX[i]->SetLineColor(color_line[i]);
		hVtxY[i]->SetLineColor(color_line[i]);
		hVtxZ[i]->SetLineColor(color_line[i]);
		hVtxXY[i]->SetLineColor(color_line[i]);
		hPx[i]->SetLineColor(color_line[i]);
		hPy[i]->SetLineColor(color_line[i]);
		hPz[i]->SetLineColor(color_line[i]);
		hPt[i]->SetLineColor(color_line[i]);
		
	}

	TCanvas *cv0 = new TCanvas("cv0","cv0",700,700);
	for (int i=0; i<4; i++) {
		//hVtxX[i]->Add(hVtxX[0],-1.);
		
		if ( i==0 ) hVtxX[i]->Draw();
		else hVtxX[i]->Draw("same");

	}
	
	TCanvas *cv1 = new TCanvas("cv1","cv1",700,700);
	for (int i=0; i<4; i++) {
		//hVtxX[i]->Add(hVtxX[0],-1.);
		
		if ( i==0 ) hVtxY[i]->Draw();
		else hVtxY[i]->Draw("same");

	}

	TCanvas *cv2 = new TCanvas("cv2","cv2",700,700);
	for (int i=0; i<4; i++) {
		//hVtxX[i]->Add(hVtxX[0],-1.);
		
		if ( i==0 ) hVtxZ[i]->Draw();
		else hVtxZ[i]->Draw("same");

	}

	TCanvas *cv3 = new TCanvas("cv3","cv3",700,700);
	for (int i=1; i<4; i++) {
		//hPt[i]->Add(hPt[0],-1.);
		
		if ( i==0 ) hPt[i]->Draw();
		else hPt[i]->Draw("same");

	}
	
	TCanvas *cv4 = new TCanvas("cv4","cv4",700,700);
	for (int i=0; i<4; i++) {
		//hPx[i]->Add(hPx[0],-1.);
		
		if ( i==0 ) hPx[i]->Draw();
		else hPx[i]->Draw("same");

	}
	
	TCanvas *cv5 = new TCanvas("cv5","cv5",700,700);
	for (int i=0; i<4; i++) {
		//hPy[i]->Add(hPy[0],-1.);
		
		if ( i==0 ) hPy[i]->Draw();
		else hPy[i]->Draw("same");

	}

	TCanvas *cv6 = new TCanvas("cv6","cv6",700,700);
	for (int i=0; i<4; i++) {
		//hPz[i]->Add(hPx[0],-1.);
		
		if ( i==0 ) hPz[i]->Draw();
		else hPz[i]->Draw("same");

	}
	
	

}



