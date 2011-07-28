{
  TChain *tt = new TChain("/PATNtupleMaker/top");
  //tt->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/41x/v2/SingleMu2011A_v1.root");
  tt->Add("/uscms_data/d3/weizou/ROOTSample/PATSkim/2011MC/42x_v8/Wprime_M-1000_Z2_Mu.root");

  tt->Process("Analyzer.C+","verbose");

}
