
void runbatch() {

  gROOT->SetStyle("CMS");
  gSystem->Load("BTagSummary_cc.so");
  //gStyle->SetOptStat(110);
  gSystem->Load("plot_summary_C.so");
  plot_summary t("../../../UserCode/Yumiceva/submit2uaf/results/analysis_qcd*.root");
  t.Loop();


}
