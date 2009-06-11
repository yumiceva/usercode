

void sigmas() {

	TFile *fn = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_TTJets-madgraph_Fall08_all_all.root");

	hHadW_n = (TH1F*) gDirectory->Get("Mass/MCHadronicW_mass_cut2");	
	hHadTop_n = (TH1F*) gDirectory->Get("Mass/MCHadronicTop_mass_cut2");
	hLepTop_n = (TH1F*) gDirectory->Get("Mass/MCLeptonicTop_mass_cut2");

	TFile *fplus = TFile::Open("nominal_JESUp/TopAnalysis_TTJets-madgraph_Fall08_all_all.root");

	hHadW_p = (TH1F*) gDirectory->Get("Mass/MCHadronicW_mass_cut2");	
	hHadTop_p = (TH1F*) gDirectory->Get("Mass/MCHadronicTop_mass_cut2");
	hLepTop_p = (TH1F*) gDirectory->Get("Mass/MCLeptonicTop_mass_cut2");
	
	TFile *fminos = TFile::Open("nominal_JESDown/TopAnalysis_TTJets-madgraph_Fall08_all_all.root");

	hHadW_m = (TH1F*) gDirectory->Get("Mass/MCHadronicW_mass_cut2");	
	hHadTop_m = (TH1F*) gDirectory->Get("Mass/MCHadronicTop_mass_cut2");
	hLepTop_m = (TH1F*) gDirectory->Get("Mass/MCLeptonicTop_mass_cut2");

	TString sp = "/n###########################################################/n/n";
	
	//fits
	TCanvas *cv = new TCanvas("cv","cv",700,700);
	hHadW_n->Draw();
	hHadW_n->Fit("gaus","v","",60,110);cout<< hHadW_n->GetName() << endl;
	cout << sp << endl;
	hHadW_p->Fit("gaus","v","",60,110);cout << sp << endl;
	hHadW_m->Fit("gaus","v","",60,110);cout << sp << endl;
	hHadW_n->Draw();

	TCanvas *cv1 = new TCanvas("cv1","cv1",700,700);
	hHadTop_n->Draw();
	hHadTop_n->Fit("gaus","v","",130,220);cout<< hHadTop_n->GetName() << endl;
	cout << sp << endl;
	hHadTop_p->Fit("gaus","v","",130,220);cout << sp << endl;
	hHadTop_m->Fit("gaus","v","",130,220);cout << sp << endl;
	hHadTop_n->Draw();
	
	TCanvas *cv2 = new TCanvas("cv2","cv2",700,700);
	hLepTop_n->Draw();
	hLepTop_n->Fit("gaus","v","",130,210);cout<< hLepTop_n->GetName() << endl;
	cout << sp << endl;
	hLepTop_p->Fit("gaus","v","",130,210);cout << sp << endl;
	hLepTop_m->Fit("gaus","v","",130,210);cout << sp << endl;
	hLepTop_n->Draw();
	
}

