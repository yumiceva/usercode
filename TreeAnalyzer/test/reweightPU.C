
void reweightPU()
{

  gROOT->SetStyle("CMS");

  TFile *fdata = TFile::Open("Pileup_2011_EPS_8_jul.root");
  TFile *fmc   = TFile::Open("results_ttbar.root");

  fdata->cd();
  TH1D *hdata = dynamic_cast<TH1D*>( gDirectory->Get("pileup") );
  hdata->Scale(1/(hdata->Integral() ));

  fmc->cd();
  TH1F *hmc = dynamic_cast<TH1F*> ( gDirectory->Get("PVs/NPV_ttbar") );
  hmc->Scale(1/(hmc->Integral()));

  TH1D *hweights = dynamic_cast<TH1D*> hdata->Clone("weights");
  hweights->Reset();

  hweights->Divide( hdata, hmc);

  hweights->Print("all");

  cout << endl << "pu_weights[25] = { " << endl;
  for (int ibin=1; ibin <= hweights->GetNbinsX(); ++ibin)
    {
      cout << " " << hweights->GetBinContent(ibin) << "," << endl;
    }
  cout << "};" << endl;

  TCanvas *cv0 = new TCanvas("cv0","ObservedPU", 700,700);
  hdata->Draw();
  hdata->SetXTitle("Number of Primary Vertices");
  hdata->SetYTitle("a.u.");
  hdata->GetYaxis()->SetTitleOffset(1.6);

  cv0->Print("ObservedPU.pdf");

  TCanvas *cv1 = new TCanvas("cv1","SimulatedPU", 700, 700);
  hmc->Draw();
  hmc->SetXTitle("Number of Primary Vertices");
  hmc->SetYTitle("a.u.");
  hmc->GetYaxis()->SetTitleOffset(1.6);

  cv1->Print("SimulatedPU.pdf");

}
