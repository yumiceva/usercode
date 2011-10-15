
void plotLimits()
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

  float y[18] = {0.345698, 0.271688, 0.220314, 0.183075, 0.166142, 0.135943, 0.114359, 0.104191, 0.0977176, 0.211317, 0.214426, 0.243926, 0.274105, 0.351391, 0.388873, 0.451273, 0.581997, 0.709318};
  float x[18] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1600, 1500, 1400, 1300, 1200, 1100, 1000, 900, 800};

  // 2 sigma expected
  //  float y2[20] = {0.34674, 0.251571, 0.22142, 0.183831, 0.167184, 0.141277, 0.112155, 0.101186, 0.0974571, 0.476385, 1.94895, 0.417412, 0.419166, 0.506077, 0.559515, 0.658741, 0.715166, 0.923511, 1.09508, 1.35574};
  //float x2[20] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 2000, 2000, 1600, 1500, 1400, 1300, 1200, 1100, 1000, 900, 800};

  float y2[18] = {0.256708, 0.195869, 0.16299, 0.131868, 0.113787, 0.0931079, 0.0847969, 0.0769055, 0.069865, 0.293661, 0.3108, 0.32708, 0.397286, 0.499492, 0.542058, 0.630205, 0.796846, 1.01067};
  float x2[18] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1600, 1500, 1400, 1300, 1200, 1100, 1000, 900, 800};

  TGraph* sigma1 = new TGraph(18, x, y);
  sigma1->SetFillColor(kGreen);
  sigma1->SetLineColor(kGreen);
  TGraph* sigma2 = new TGraph(18, x2, y2);
  sigma2->SetFillColor(kYellow);
  sigma2->SetLineColor(kYellow);

  // expected central
  //float x_expec[10] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 2000};
  //float y_expec[10] = {0.677829, 0.505865, 0.435165, 0.367922, 0.326033, 0.268729, 0.241117, 0.213371, 0.202733, 0.946754};
  float y_expec[9] = {0.493583, 0.394866, 0.31215, 0.26517, 0.236317, 0.192805, 0.166015, 0.152947, 0.140099};
  float x_expec[9] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600};


  TGraph* expec = new TGraph(9, x_expec, y_expec);
  expec->SetLineStyle(kDashed);
  expec->SetLineWidth(2);
  
  // observed
  //  float x_obs[10] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 2000};
  //float y_obs[10] = {0.923312, 0.840667, 0.561406, 0.318798, 0.229979, 0.32366, 0.359522, 0.348388, 0.300723, 0.917808};
  float y_obs[9] = {0.792876, 0.548186, 0.383445, 0.27296, 0.203571, 0.260504, 0.289699, 0.261139, 0.241566};
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

  TLatex *banner = new TLatex(0.28,0.87,"#splitline{CMS Preliminary}{2.18 fb^{-1} at #sqrt{s}=7TeV #mu+jets,e+jets}");
  banner->SetNDC();
  banner->SetTextSize(0.035);
  banner->Draw();

  cv->Print("WprimeLimits.pdf");
    
}