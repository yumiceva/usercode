
void runAnalysis() 
{
  TString desdir = "/uscms/home/yumiceva/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/";
  TProof *p = TProof::Open("lite://", desdir ,desdir);

  //p->AddDynamicPath("");
  p->Exec("gSystem->Load(\"/uscms/home/yumiceva/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libYumicevaTop7TeV.so\")");
  p->AddIncludePath("/uscms/home/yumiceva/work/CMSSW_4_2_4/src/");

  p->Archive(" ",desdir);

  //p->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION", "LOCAL"));

  TDSet *mc_wp_1000 = new TDSet("top","*","/PATNtupleMaker");
  mc_wp_1000->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/Wprime_M-1000_Z2_Mu.root");
  mc_wp_1000->Process("Analyzer.C+","sample=Wprime_1000");

  TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
  mc_ttbar->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/TTbar_Z2_Mu.root");
  mc_ttbar->Process("Analyzer.C+","sample=ttbar");


}
