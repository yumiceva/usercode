

void massplot() {

	TFile *ttbarf = TFile::Open("TtEventAnalysis_inclttbar.root");
	TFile *zp1f = TFile::Open("TtEventAnalysis_Zp1TeV.root");
	TFile *zp2f = TFile::Open("TtEventAnalysis_Zp2TeV.root");
	TFile *zp3f = TFile::Open("TtEventAnalysis_Zp3TeV.root");
	TFile *zp4f = TFile::Open("TtEventAnalysis_Zp4TeV.root");

	ttbarf->cd();
	TH1D *mttbar = (TH1D*) gDirectory->Get("Jets/topPair_mass_cut1");
	zp1f->cd();
	TH1D *mzp1 = (TH1D*) gDirectory->Get("Jets/topPair_mass_cut1");
	zp2f->cd();
	TH1D *mzp2 = (TH1D*) gDirectory->Get("Jets/topPair_mass_cut1");
	zp3f->cd();
	TH1D *mzp3 = (TH1D*) gDirectory->Get("Jets/topPair_mass_cut1");
	zp4f->cd();
	TH1D *mzp4 = (TH1D*) gDirectory->Get("Jets/topPair_mass_cut1");
	

	TCanvas *cv = new TCanvas("cv","cv",700,700);

	cout << "mttbar = " << mttbar->GetEntries() << endl;
	cout << "mzp1 = " << mzp1->GetEntries() << endl;
	cout << "mzp2 = " << mzp2->GetEntries() << endl;
	cout << "mzp3 = " << mzp3->GetEntries() << endl;
	cout << "mzp4 = " << mzp4->GetEntries() << endl;
		
	mttbar->Scale((1000.*833*0.15)/19022.);
	mzp1->Scale((1000.*2*20.*0.15)/7762.);
	mzp2->Scale((1000.*2*1.*0.15)/7929.);
	mzp3->Scale((1000.*2*0.7*0.15)/8760.);
	mzp4->Scale((1000.*2*0.5*0.15)/7812.);

	TH1D *mzp1tot = (TH1D*) mzp1->Clone("mzp1tot");
	TH1D *mzp2tot = (TH1D*) mzp2->Clone("mzp2tot");
	TH1D *mzp3tot = (TH1D*) mzp3->Clone("mzp3tot");
	TH1D *mzp4tot = (TH1D*) mzp4->Clone("mzp4tot");

	mzp1tot->Add(mttbar);
	mzp2tot->Add(mttbar);
	mzp3tot->Add(mttbar);
	mzp4tot->Add(mttbar);
	
	mzp1tot->SetLineColor(2);
	mzp2tot->SetLineColor(3);
	mzp3tot->SetLineColor(4);
	mzp4tot->SetLineColor(6);

	mzp1tot->SetYTitle("Events/(44 GeV/c^{2})");
	
	mzp1tot->Draw();
	mzp2tot->Draw("same");
	mzp3tot->Draw("same");
	mzp4tot->Draw("same");
	mttbar->Draw("same");

	mttbar->Fit("expo","","same",400,2000);
	gPad->SetLogy();
}
