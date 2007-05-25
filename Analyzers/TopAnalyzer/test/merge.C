
void merge() {

  gSystem->Load("libAnalyzersTopEvent.so");
  TChain ch("summary");                     

  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_11.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_12.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_13.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_17.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_1.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_20.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_21.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_22.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_23.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_24.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_25.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_26.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_27.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_28.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_2.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_30.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_32.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_34.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_35.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_36.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_38.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_42.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_43.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_44.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_45.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_46.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_47.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_48.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_49.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_50.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_5.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_6.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_7.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_8.root");
  ch.Add("dcache:/pnfs/cms/WAX/resilient/yumiceva/1_2_3/BTagAnalyzer/toprex_results_9.root");

  ch.Merge("merged_toprex.root");

}
