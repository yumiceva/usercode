
void plotLimits(TString sample="muons")
{
  gROOT->SetStyle("Plain");

  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();

  gROOT->ProcessLine(".L CMSTopStyle.cc+");
  CMSTopStyle();

  // frame
  TH2F* frame = new TH2F("frame", "frame", 10, 700, 1700, 100, 0, 6); // for semilog: 0.01, 4
  frame->GetXaxis()->SetTitle("W'(tb) mass [GeV]");
  frame->GetYaxis()->SetTitle("#sigma#upointBR(t#rightarrowWb,W#rightarrowl#nu) [pb]");
  frame->SetNdivisions(505);

  // 1 sigma expected
  //float y[18] = {0.358728, 0.281431, 0.234517, 0.18687, 0.17062, 0.141074, 0.130556, 0.122411, 0.0975945, 0.194227, 0.236236, 0.26224, 0.2811, 0.350176, 0.383035, 0.467865, 0.576722, 0.716664};//mu
  //float y[18] = {0.277904, 0.207619, 0.155422, 0.127035, 0.113681, 0.104298, 0.0914065, 0.0859689, 0.0831026, 0.171106, 0.178547, 0.184368, 0.216946, 0.239412, 0.273257, 0.352083, 0.46228, 0.572094};//mu
  //float y[18] = {0.209004, 0.165859, 0.114068, 0.099366, 0.0976725, 0.0791712, 0.068054, 0.0607815, 0.0565296, 0.131673, 0.139425, 0.156768, 0.181906, 0.211852, 0.222727, 0.240788, 0.342769, 0.428826 };//comb
  float y[16] = {0.434245, 0.406708, 0.424477, 0.654714, 0.858104, 1.07795, 1.71916, 1.68738, 3.53462, 3.54302, 2.28553, 1.74204, 1.34942, 0.8965, 0.90367, 0.893258};
  //float x[18] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1600, 1500, 1400, 1300, 1200, 1100, 1000, 900, 800};
  float x[16] = {800, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1600, 1500, 1400, 1300, 1200, 1100, 1000, 800};
  
  // 2 sigma expected
  //float y2[18] = {0.272466, 0.204374, 0.171273, 0.139436, 0.127694, 0.10703, 0.0988706, 0.092174, 0.0713491, 0.280564, 0.321915, 0.368581, 0.386568, 0.509747, 0.533772, 0.627044, 0.782301, 0.980959, };//mu
  //float y2[18] = {0.204005, 0.148925, 0.108721, 0.0907065, 0.0866197, 0.0765106, 0.0687406, 0.0668612, 0.0630056, 0.238396, 0.254995, 0.267838, 0.2988, 0.348534, 0.417958, 0.519854, 0.666588, 0.793745}; //mu
  //float y2[18] = {0.154812, 0.124289, 0.0827802, 0.0755105, 0.0684192, 0.0579809, 0.0514388, 0.0454054, 0.0410477, 0.225559, 0.224141, 0.254421, 0.291123, 0.331405, 0.32643, 0.346824, 0.474801, 0.603776};//comb
  float y2[16] = {0.309372, 0.29297, 0.321181, 0.476416, 0.638279, 0.804499, 1.27137, 1.22713, 5.01237, 5.4264, 3.18795, 2.52046, 1.82543, 1.2667, 1.27823, 1.22304};
  //float x2[18] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1600, 1500, 1400, 1300, 1200, 1100, 1000, 900, 800};
  float x2[16] = {800, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1600, 1500, 1400, 1300, 1200, 1100, 1000, 800};

  TGraph* sigma1 = new TGraph(16, x, y);
  sigma1->SetFillColor(kGreen);
  sigma1->SetLineColor(kGreen);
  TGraph* sigma2 = new TGraph(16, x2, y2);
  sigma2->SetFillColor(kYellow);
  sigma2->SetLineColor(kYellow);

  // expected central
  //float y_expec[9] = {0.514687, 0.403164, 0.33249, 0.269663, 0.243511, 0.199258, 0.184251, 0.169157, 0.138424};//mu
  //float y_expec[9] = {0.388926, 0.307083, 0.238449, 0.188024, 0.168278, 0.150104, 0.128963, 0.120138, 0.118696};//mu
  //float y_expec[9] = {0.302267, 0.241051, 0.16756, 0.139799, 0.140696, 0.12036, 0.101943, 0.0901897, 0.0839297};//comb
  float y_expec[8] = {0.617032, 0.585046, 0.62811, 0.946014, 1.21994, 1.56437, 2.46334, 2.39085};
  //float x_expec[9] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600};
  float x_expec[8] = {800, 1000, 1100, 1200, 1300, 1400, 1500, 1600};

  TGraph* expec = new TGraph(8, x_expec, y_expec);
  expec->SetLineStyle(kDashed);
  expec->SetLineWidth(2);
  
  // observed
  //float y_obs[9] = {0.719111, 0.466673, 0.320686, 0.207868, 0.166946, 0.18364, 0.187096, 0.207545, 0.179993};//mu
  //float y_obs[9] = {0.835543, 0.514064, 0.244006, 0.139701, 0.102246, 0.102707, 0.108371, 0.0930329, 0.0755863};//mu
  //float y_obs[9] = {0.542922, 0.402802, 0.23763, 0.112371, 0.0678945, 0.0757037, 0.0898949, 0.0801001, 0.0746494};//comb
  float y_obs[8] = {1.35317, 0.64252, 0.499368, 0.513477, 0.841406, 1.25651, 1.76003, 1.65371};
  //float x_obs[9] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600};
  float x_obs[8] = {800, 1000, 1100, 1200, 1300, 1400, 1500, 1600};

  TGraph* obs = new TGraph(8, x_obs, y_obs);
  obs->SetMarkerStyle(20);

  // REMOVE
  //  sigma1->RemovePoint(9);
  //sigma1->RemovePoint(9);
  //sigma2->RemovePoint(9);
  //sigma2->RemovePoint(9);
  //obs->RemovePoint(9);
  //expec->RemovePoint(9);

  // theory
  float x_th[10] = {800 ,   900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 2000};
  //float y_th[6] = {1.6, 0.926, 0.566, 0.094, 0.063, 0.0093}; OLD
  float y_th[10] = {2.426,1.389,0.849,0.516,0.334,0.214,0.141,0.094, 0.0623, 0.014};

  TGraph* theory = new TGraph(10, x_th, y_th);
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
  //  theory->Draw("c");
  //  cv->SetLogy();

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
  //aleg->AddEntry( theory, "Theory W' RH","L");
  aleg->Draw();

  TString sbanner = "#splitline{CMS Preliminary}{3.55 fb^{-1} at #sqrt{s}=7TeV #mu+jets}";
  if ( sample == "combined" ) sbanner = "#splitline{CMS Preliminary}{3.55 fb^{-1} at #sqrt{s}=7TeV #mu+jets,e+jets}";
  if ( sample == "electrons" ) sbanner = "#splitline{CMS Preliminary}{3.55 fb^{-1} at #sqrt{s}=7TeV e+jets}";

  TLatex *banner = new TLatex(0.28,0.3, sbanner);
  banner->SetNDC();
  banner->SetTextSize(0.035);
  banner->Draw();

  TString outname = "WprimeRH_Limits_mu.pdf";
  if ( sample == "combined" ) outname = "WprimeRH_Limits_combined.pdf";
  if ( sample == "electrons" ) outname = "WprimeRH_Limits_electrons.pdf";
  cv->Print(outname);
    
}
