
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

  TDSet *mc_wp_1200 = new TDSet("top","*","/PATNtupleMaker");
  mc_wp_1200->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/Wprime_M-1200_Z2_Mu.root");
  mc_wp_1200->Process("Analyzer.C+","sample=Wprime_1200");

  TDSet *mc_wp_1500 = new TDSet("top","*","/PATNtupleMaker");
  mc_wp_1500->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/Wprime_M-1500_Z2_Mu.root");
  mc_wp_1500->Process("Analyzer.C+","sample=Wprime_1500");

  TDSet *mc_wp_2000 = new TDSet("top","*","/PATNtupleMaker");
  mc_wp_2000->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/Wprime_M-2000_Z2_Mu.root");
  mc_wp_2000->Process("Analyzer.C+","sample=Wprime_2000");

  TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
  mc_ttbar->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/TTbar_Z2_Mu.root");
  mc_ttbar->Process("Analyzer.C+","sample=ttbar");

  TDSet *mc_Wjets = new TDSet("top","*","/PATNtupleMaker");
  mc_Wjets->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/WJets_Z2_Mu.root");
  mc_Wjets->Process("Analyzer.C+","sample=WJets");

  TDSet *mc_QCD = new TDSet("top","*","/PATNtupleMaker");
  mc_QCD->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/QCD_Z2_Mu.root");
  mc_QCD->Process("Analyzer.C+","sample=QCD");

  TDSet *mc_STsch = new TDSet("top","*","/PATNtupleMaker");
  mc_STsch->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/STsch_Z2_Mu.root");
  mc_STsch->Process("Analyzer.C+","sample=STsch");

  TDSet *mc_STtch = new TDSet("top","*","/PATNtupleMaker");
  mc_STtch->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/STtch_Z2_Mu.root");
  mc_STtch->Process("Analyzer.C+","sample=STtch");

  TDSet *mc_STtWch = new TDSet("top","*","/PATNtupleMaker");
  mc_STtWch->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/STtWch_Z2_Mu.root");
  mc_STtWch->Process("Analyzer.C+","sample=STtWch");

  TDSet *mc_WW = new TDSet("top","*","/PATNtupleMaker");
  mc_WW->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/WW_Z2_Mu.root");
  mc_WW->Process("Analyzer.C+","sample=WW");

  TDSet *mc_WZ = new TDSet("top","*","/PATNtupleMaker");
  mc_WZ->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/WZ_Z2_Mu.root");
  mc_WZ->Process("Analyzer.C+","sample=WZ");

  TDSet *mc_ZJets = new TDSet("top","*","/PATNtupleMaker");
  mc_ZJets->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/ZJets_Z2_Mu.root");
  mc_ZJets->Process("Analyzer.C+","sample=ZJets");

  TDSet *data = new TDSet("top","*","/PATNtupleMaker");
  data->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011Data/42x_v8/SingleMu_May10ReReco_Mu.root");
  data->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011Data/42x_v8/SingleMu_PromptReco-v4_Mu.root");
  data->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011Data/42x_v8/SingleMu_PromptReco-v4_exlcusive_Mu.root");
  data->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011Data/42x_v8/SingleMu_PromptReco-v5_Mu.root");
  data->Process("Analyzer.C+","sample=data");


}
