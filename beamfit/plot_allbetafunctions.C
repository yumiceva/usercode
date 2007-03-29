

#include "TF1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TAxis.h"
#include "TLine.h"

Double_t beamwidth( Double_t *x, Double_t *parm ) {

	//const double emmitance = 3.75e-8;
	//const double betastar = 55.;
	//const double z0 = 0.;
	//double x = sqrt(emmitance*(betastar+(((z-z0)*(z-z0))/betastar)));
	//return x;
	return 1.e4*sqrt(parm[0]*(parm[1]+(x[0]*x[0]/parm[1])));
}

Double_t beamwidthM( Double_t *x, Double_t *parm ) {

	return (-1.e4)*sqrt(parm[0]*(parm[1]+(x[0]*x[0]/parm[1])));
}



void plot_allbetafunctions() {

	TCanvas *cv1 = new TCanvas("cv1","cv1",700,700);

	TF1 *fptev = new TF1("fptev",beamwidth,-40,40,2);
	fptev->SetParameter(0,1.39e-7);
	fptev->SetParameter(1,35.);
	fptev->SetLineColor(kBlue);
	fptev->SetLineStyle(9);
	
	fptev->Draw();
	fptev->SetMaximum(37.);
	fptev->SetMinimum(-37.);
	fptev->GetXaxis()->SetTitle("s=z-z_{0} [cm]");
	fptev->GetYaxis()->SetTitle("beam width [#mum]");

	TF1 *fp100 = new TF1("fp100",beamwidth,-40,40,2);
	fp100->SetParameter(0,5.03e-8);
	fp100->SetParameter(1,200.);
	fp100->SetLineColor(kRed);
	fp100->SetLineStyle(2);
	
	fp100->Draw("same");
	//fp100->SetMaximum(28.);
	//fp100->SetMinimum(-28.);
	fp100->GetXaxis()->SetTitle("s=z-z_{0} [cm]");
	fp100->GetYaxis()->SetTitle("beam width [#mum]");

	TF1 *fntev = new TF1("fntev",beamwidthM,-40,40,2);
	fntev->SetParameter(0,1.39e-7);
	fntev->SetParameter(1,35.);
	fntev->SetLineColor(kBlue);
	fntev->SetLineStyle(9);
	
	fntev->Draw("same");
	
	TF1 *fn100 = new TF1("fn100",beamwidthM,-40,40,2);
	fn100->SetParameter(0,5.03e-8);
	fn100->SetParameter(1,200.);
	fn100->SetLineColor(kRed);
	fn100->SetLineStyle(2);
	
	fn100->Draw("same");

	TF1 *fp55 = new TF1("fp55",beamwidth,-40,40,2);
	fp55->SetParameter(0,5.03e-8);
	fp55->SetParameter(1,55.);
	fp55->Draw("same");
	
	TF1 *fn55 = new TF1("fn55",beamwidthM,-40,40,2);
	fn55->SetParameter(0,5.03e-8);
	fn55->SetParameter(1,55.);
	fn55->Draw("same");

	TLine *line1[4];
	line1[0] = new TLine(-7.55/2.,14,-7.55/2.,18);
	line1[1] = new TLine(-7.55/2.,-14,-7.55/2.,-18);
	line1[2] = new TLine(7.55/2.,14,7.55/2.,18);
	line1[3] = new TLine(7.55/2.,-14,7.55/2.,-18);

	TLine *line2[4];
	line2[0] = new TLine(-25./2.,21,-25./2.,25.);
	line2[1] = new TLine(-25./2.,-21,-25./2.,-25.);
	line2[2] = new TLine(25./2.,21,25./2.,25.);
	line2[3] = new TLine(25./2.,-21,25./2.,-25.);
	for (int il =0; il<4; il++) {
		line1[il]->Draw();
		line2[il]->SetLineColor(kBlue);
		line2[il]->Draw();
	}

	
		
}
