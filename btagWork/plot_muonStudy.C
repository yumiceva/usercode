
void plot_muonStudy() {

	TFile *f = TFile::Open("muonStudy.root");

	TH1F *hjet1pt = (TH1F*) gDirectory->Get("hjet1pt");
	TH1F *hjet2pt = (TH1F*) gDirectory->Get("hjet2pt");
	TH1F *hjet3pt = (TH1F*) gDirectory->Get("hjet3pt");


	TCanvas *cv0 = new TCanvas("cv0","cv0", 700, 700);

	hjet3pt->SetLineColor(kRed);
	hjet2pt->SetLineColor(kBlue);
	//hjet1pt->SetLineColor();
	
	hjet3pt->SetLineWidth(2);
	hjet2pt->SetLineWidth(2);
	hjet1pt->SetLineWidth(2);

	hjet1pt->SetXTitle("jet p_{T} [GeV/c]");
	hjet1pt->Draw();
	hjet3pt->Draw("same");
	hjet2pt->Draw("same");

	TCanvas *cv1 = new TCanvas("cv1","cv1", 700, 700);

	hjet3eta->SetLineColor(kRed);
	hjet2eta->SetLineColor(kBlue);
	//hjet1eta->SetLineColor();
	hjet3eta->SetLineWidth(2);
	hjet2eta->SetLineWidth(2);
	hjet1eta->SetLineWidth(2);

	hjet1eta->SetXTitle("jet #eta");
	hjet1eta->Draw();
	hjet2eta->Draw("same");
	hjet3eta->Draw("same");

	
	
}
