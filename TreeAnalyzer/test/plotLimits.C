
void plotLimits(TString sample="muons")
{
  gROOT->SetStyle("Plain");

  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();

  gROOT->ProcessLine(".L CMSTopStyle.cc+");
  CMSTopStyle();

  // frame
  TH2F* frame = new TH2F("frame", "frame", 10, 700, 1700, 100, 0, 1.8); // for semilog: 0.01, 4
  frame->GetXaxis()->SetTitle("W'(tb) mass [GeV]");
  frame->GetYaxis()->SetTitle("#sigma#upointBR(t#rightarrowWb,W#rightarrowl#nu) [pb]");
  frame->SetNdivisions(505);

  // 1 sigma expected
  //  float y[20] = {0.465839, 0.344319, 0.301805, 0.252836, 0.22705, 0.187713, 0.164403, 0.143326, 0.13704, 0.659113, 1.3483, 0.295105, 0.310359, 0.357195, 0.398338, 0.483473, 0.531641, 0.635671, 0.730211, 0.989865};
  //float x[20] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 2000, 2000, 1600, 1500, 1400, 1300, 1200, 1100, 1000, 900, 800};
  //float y[18] = {0.539629, 0.384349, 0.30736, 0.269626, 0.246526, 0.206321, 0.191609, 0.167634, 0.13406, 0.273342, 0.344173, 0.388415, 0.435259, 0.524824, 0.56201, 0.651129, 0.802885, 1.08597};
  float y[18] = {0.358728, 0.281431, 0.234517, 0.18687, 0.17062, 0.141074, 0.130556, 0.122411, 0.0975945, 0.194227, 0.236236, 0.26224, 0.2811, 0.350176, 0.383035, 0.467865, 0.576722, 0.716664};//mu
  float y[18] = {0.268165, 0.217801, 0.158252, 0.130713, 0.126213, 0.0976389, 0.0909423, 0.081222, 0.0689493, 0.135096, 0.164272, 0.180872, 0.200358, 0.251939, 0.262668, 0.326623, 0.430224, 0.550267};//comb
  //float y[18] = {0.345698, 0.271688, 0.220314, 0.183075, 0.166142, 0.135943, 0.114359, 0.104191, 0.0977176, 0.211317, 0.214426, 0.243926, 0.274105, 0.351391, 0.388873, 0.451273, 0.581997, 0.709318};
  float x[18] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1600, 1500, 1400, 1300, 1200, 1100, 1000, 900, 800};

  // 2 sigma expected
  float y2[18] = {0.272466, 0.204374, 0.171273, 0.139436, 0.127694, 0.10703, 0.0988706, 0.092174, 0.0713491, 0.280564, 0.321915, 0.368581, 0.386568, 0.509747, 0.533772, 0.627044, 0.782301, 0.980959, };//mu
  float y2[18] = {0.199536, 0.162468, 0.123992, 0.0959844, 0.0951372, 0.073903, 0.0716594, 0.0621366, 0.0506642, 0.190405, 0.245443, 0.244038, 0.282975, 0.340368, 0.351096, 0.453723, 0.570529, 0.749732}; //comb
  float x2[18] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1600, 1500, 1400, 1300, 1200, 1100, 1000, 900, 800};

  TGraph* sigma1 = new TGraph(18, x, y);
  sigma1->SetFillColor(kGreen);
  sigma1->SetLineColor(kGreen);
  TGraph* sigma2 = new TGraph(18, x2, y2);
  sigma2->SetFillColor(kYellow);
  sigma2->SetLineColor(kYellow);

  // expected central
  float y_expec[9] = {0.514687, 0.403164, 0.33249, 0.269663, 0.243511, 0.199258, 0.184251, 0.169157, 0.138424};//mu
  float y_expec[9] = {0.380794, 0.305337, 0.225965, 0.184704, 0.175413, 0.140373, 0.128817, 0.115562, 0.0950101};//comb
  float x_expec[9] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600};


  TGraph* expec = new TGraph(9, x_expec, y_expec);
  expec->SetLineStyle(kDashed);
  expec->SetLineWidth(2);
  
  // observed
  float y_obs[9] = {0.719111, 0.466673, 0.320686, 0.207868, 0.166946, 0.18364, 0.187096, 0.207545, 0.179993};//mu
  float y_obs[9] = {0.593208, 0.321309, 0.277535, 0.209598, 0.160347, 0.171819, 0.169697, 0.171704, 0.163548};//comb
  float x_obs[9] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600};


  TGraph* obs = new TGraph(9, x_obs, y_obs);
  obs->SetMarkerStyle(20);

  // REMOVE
  //  sigma1->RemovePoint(9);
  //sigma1->RemovePoint(9);
  //sigma2->RemovePoint(9);
  //sigma2->RemovePoint(9);
  //obs->RemovePoint(9);
  //expec->RemovePoint(9);

  // theory
  float x_th[9] = {800 ,   900, 1000, 1100, 1200, 1300, 1400, 1500, 2000};
  //float y_th[6] = {1.6, 0.926, 0.566, 0.094, 0.063, 0.0093}; OLD
  float y_th[9] = {2.426,1.389,0.849,0.516,0.334,0.214,0.141,0.094,0.014};

  TGraph* theory = new TGraph(9, x_th, y_th);
  //theory->SetLineStyle(kDashed);
  theory->SetLineColor(2);
  theory->SetLineWidth(2);

  // plot
  TCanvas *cv = new TCanvas("cv","cv",700,700);
  frame->Draw();
  sigma2->Draw("F");
  sigma1->Draw("F");
  expec->Draw("c");
  obs->Draw("Pc");
  theory->Draw("c");

  TLegend *aleg = new TLegend(0.68,0.62,0.95,0.89);
  aleg->SetMargin(0.12);
  aleg->SetTextSize(0.025);
  aleg->SetFillColor(10);
  aleg->SetBorderSize(0);
  aleg->SetLineStyle(0);
  aleg->SetFillStyle(0);
  
  aleg->AddEntry( obs, "95% Observed","P");
  aleg->AddEntry( expec, "95% Expected","L");
  aleg->AddEntry( sigma1, "#pm1#sigma Expected","FL");
  aleg->AddEntry( sigma2, "#pm2#sigma Expected","FL");
  aleg->AddEntry( theory, "Theory W' RH","L");
  aleg->Draw();

  TString sbanner = "#splitline{CMS Preliminary}{2.18 fb^{-1} at #sqrt{s}=7TeV #mu+jets}";
  if ( sample == "combined" ) sbanner = "#splitline{CMS Preliminary}{2.18 fb^{-1} at #sqrt{s}=7TeV #mu+jets,e+jets}";
  if ( sample == "electrons" ) sbanner = "#splitline{CMS Preliminary}{2.18 fb^{-1} at #sqrt{s}=7TeV e+jets}";

  TLatex *banner = new TLatex(0.28,0.87, sbanner);
  banner->SetNDC();
  banner->SetTextSize(0.035);
  banner->Draw();

  TString outname = "WprimeRH_Limits_mu.pdf";
  if ( sample == "combined" ) outname = "WprimeRH_Limits_combined.pdf";
  if ( sample == "electrons" ) outname = "WprimeRH_Limits_electrons.pdf";
  cv->Print(outname);
    
}
