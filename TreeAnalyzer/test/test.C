{
  TChain *tt = new TChain("/PATNtupleMaker/top");
  tt->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/41x/v2/SingleMu2011A_v1.root");
  tt->Process("Analyzer.C+");

}
