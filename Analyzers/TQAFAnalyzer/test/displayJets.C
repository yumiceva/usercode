
void displayJets() {

	TFile *_file0 = TFile::Open("TtEventAnalysis_test.root");
		
	gROOT->SetStyle("Plain");

	TH2D *hj0 = gDirectory->Get("DisplayJets/jet0_calotowerI_cut0");
	TH2D *hj1 = gDirectory->Get("DisplayJets/jet1_calotowerI_cut0");
	TH2D *hj2 = gDirectory->Get("DisplayJets/jet2_calotowerI_cut0");
	TH2D *hj3 = gDirectory->Get("DisplayJets/jet3_calotowerI_cut0");
	
	hj0->SetFillColor(2);
	hj1->SetFillColor(3);
	hj2->SetFillColor(4);
	hj3->SetFillColor(6);
	
	THStack *hs = new THStack("hs","hs");
	hs->Add(hj0);
	hs->Add(hj1);
	hs->Add(hj2);
	hs->Add(hj3);

	hs->Draw("lego1 FB");
	
}
