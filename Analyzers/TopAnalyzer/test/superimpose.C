
void superimpose() {

	gROOT->SetStyle("CMS");

	TFile *pythia = new TFile("plots_pythia.root");
	TH1D *p_twobody = (TH1D*) gDirectory->Get("twobody");
	TH1D *p_threebody = (TH1D*) gDirectory->Get("threebody");
	
	TFile *toprex = new TFile("plots_toprex.root");
	TH1D *t_twobody = (TH1D*) gDirectory->Get("twobody");
	TH1D *t_threebody = (TH1D*) gDirectory->Get("threebody");

	TCanvas *cv1 = new TCanvas("cv1","cv1",700,700);
	t_twobody->SetLineColor(4);
	t_twobody->Draw();
	p_twobody->Draw("same");
	
	TCanvas *cv2 = new TCanvas("cv2","cv2",700,700);
	
	t_threebody->SetLineColor(4);
	t_threebody->Draw();
	p_threebody->Draw("same");
	
}
