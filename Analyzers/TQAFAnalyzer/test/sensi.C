
void sensi() {


	TCanvas *cv = new TCanvas("cv","cv",700,700);

	Double_t x[4],y[4];
	Int_t n = 4;

	x[0] = 1000.; y[0] = 20.;
	x[1] = 2000.; y[1] = 1.;
	x[2] = 3000.; y[2] = 0.7;
	x[3] = 4000.; y[3] = 0.6;
	
	TGraph *g = new TGraph(n,x,y);
	g->Draw("ACP");
	g->GetXaxis()->SetTitle("m_{t#bar{t}} [GeV/c^{2}]");
	g->GetYaxis()->SetTitle("#sigma #times BR [pb]");

	gPad->SetLogy();
	gPad->SetGrid();
}
