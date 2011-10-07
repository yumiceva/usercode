
void runAnalysis(TString sample="all",bool NoGUI=false) 
{
  TString desdir = "/uscms/home/yumiceva/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/";
  TProof *p = TProof::Open("lite://", desdir ,desdir);

  //p->AddDynamicPath("");
  p->Exec("gSystem->Load(\"/uscms/home/yumiceva/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libYumicevaTop7TeV.so\")");
  p->Exec("gSystem->Load(\"/uscms/home/yumiceva/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libCondFormatsJetMETObjects.so\")");
  p->AddIncludePath("/uscms/home/yumiceva/work/CMSSW_4_2_4/src/");

  p->Archive(" ",desdir);

  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);

  //p->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION", "LOCAL"));

  if (sample=="MCcomphep"||sample=="WpRH800"||sample=="all")
    {
      TDSet *mc_wp_rh_800 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_800->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WprimeToTB_M-800_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_800->Process("Analyzer.C+","sample=WpRH800");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);  
  if (sample=="MCcomphep"||sample=="WpRH900"||sample=="all")
    {
      TDSet *mc_wp_rh_900 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_900->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WprimeToTB_M-900_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_900->Process("Analyzer.C+","sample=WpRH900");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MCcomphep"||sample=="WpRH1000"||sample=="all")
    {
      TDSet *mc_wp_rh_1000 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1000->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WprimeToTB_M-1000_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1000->Process("Analyzer.C+","sample=WpRH1000");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MCcomphep"||sample=="WpRH1100"||sample=="all")
    {
      TDSet *mc_wp_rh_1100 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1100->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WprimeToTB_M-1100_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1100->Process("Analyzer.C+","sample=WpRH1100");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MCcomphep"||sample=="WpRH1200"||sample=="all")
    {
      TDSet *mc_wp_rh_1200 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1200->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WprimeToTB_M-1200_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1200->Process("Analyzer.C+","sample=WpRH1200");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MCcomphep"||sample=="WpRH1300"||sample=="all")
    {
      TDSet *mc_wp_rh_1300 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1300->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WprimeToTB_M-1300_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1300->Process("Analyzer.C+","sample=WpRH1300");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MCcomphep"||sample=="WpRH1400"||sample=="all")
    {
      TDSet *mc_wp_rh_1400 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1400->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WprimeToTB_M-1400_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1400->Process("Analyzer.C+","sample=WpRH1400");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MCcomphep"||sample=="WpRH1500"||sample=="all")
    {
      TDSet *mc_wp_rh_1500 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1500->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WprimeToTB_M-1500_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1500->Process("Analyzer.C+","sample=WpRH1500");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MCcomphep"||sample=="WpRH1600"||sample=="all")
    {
      TDSet *mc_wp_rh_1600 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1600->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WprimeToTB_M-1600_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1600->Process("Analyzer.C+","sample=WpRH1600");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);

  /*

  if (sample=="MC"||sample=="Wprime800"||sample=="all")
    {
      TDSet *mc_wp_800 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_800->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/Wprime800_Mu.root");
      mc_wp_800->Process("Analyzer.C+","sample=Wprime_800");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="Wprime1000"||sample=="all")
    {
      TDSet *mc_wp_1000 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_1000->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/Wprime1000_Mu.root");
      mc_wp_1000->Process("Analyzer.C+","sample=Wprime_1000");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="Wprime1200"||sample=="all")
    {
      TDSet *mc_wp_1200 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_1200->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1//Wprime1200_Mu.root");
      mc_wp_1200->Process("Analyzer.C+","sample=Wprime_1200");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="Wprime1500"||sample=="all")
    {
      TDSet *mc_wp_1500 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_1500->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/Wprime1500_Mu.root");
      mc_wp_1500->Process("Analyzer.C+","sample=Wprime_1500");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="Wprime2000"||sample=="all")
    {
      TDSet *mc_wp_2000 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_2000->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/Wprime2000_Mu.root");
      mc_wp_2000->Process("Analyzer.C+","sample=Wprime_2000");
    }
  */

  if (sample=="MC"||sample=="ttbar"||sample=="all")
    {
      TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/TTbar_Mu.root");
      mc_ttbar->Process("Analyzer.C+","sample=ttbar");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="WJets"||sample=="all")
    {
      TDSet *mc_Wjets = new TDSet("top","*","/PATNtupleMaker");
      mc_Wjets->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WJets_Mu.root");
      mc_Wjets->Process("Analyzer.C+","sample=WJets");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="QCD"||sample=="all")
    {
      TDSet *mc_QCD = new TDSet("top","*","/PATNtupleMaker");
      mc_QCD->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/QCD_Mu.root");
      mc_QCD->Process("Analyzer.C+","sample=QCD");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="STsch"||sample=="all")
    {
      TDSet *mc_STsch = new TDSet("top","*","/PATNtupleMaker");
      mc_STsch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/STsch_Mu.root");
      mc_STsch->Process("Analyzer.C+","sample=STsch");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="STtch"||sample=="all")
    {
      TDSet *mc_STtch = new TDSet("top","*","/PATNtupleMaker");
      mc_STtch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/STtch_Mu.root");
      mc_STtch->Process("Analyzer.C+","sample=STtch");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="STtWch"||sample=="all")
    {
      TDSet *mc_STtWch = new TDSet("top","*","/PATNtupleMaker");
      mc_STtWch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/STtWch_Mu.root");
      mc_STtWch->Process("Analyzer.C+","sample=STtWch");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="STsch_bar"||sample=="all")
    {
      TDSet *mc_STsch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STsch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/STsch_bar_Mu.root");
      mc_STsch_bar->Process("Analyzer.C+","sample=STsch_bar");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="STtch_bar"||sample=="all")
    {
      TDSet *mc_STtch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STtch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/STtch_bar_Mu.root");
      mc_STtch_bar->Process("Analyzer.C+","sample=STtch_bar");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="STtWch_bar"||sample=="all")
    {
      TDSet *mc_STtWch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STtWch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/STtWch_bar_Mu.root");
      mc_STtWch_bar->Process("Analyzer.C+","sample=STtWch_bar");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="WW"||sample=="all")
    {
      TDSet *mc_WW = new TDSet("top","*","/PATNtupleMaker");
      mc_WW->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WW_Mu.root");
      mc_WW->Process("Analyzer.C+","sample=WW");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="WZ"||sample=="all")
    {
      TDSet *mc_WZ = new TDSet("top","*","/PATNtupleMaker");
      mc_WZ->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WZ_Mu.root");
      mc_WZ->Process("Analyzer.C+","sample=WZ");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="ZJets"||sample=="all")
    {
      TDSet *mc_ZJets = new TDSet("top","*","/PATNtupleMaker");
      mc_ZJets->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/ZJets_Mu.root");
      mc_ZJets->Process("Analyzer.C+","sample=ZJets");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="data"||sample=="all")
    {
      TDSet *data = new TDSet("top","*","/PATNtupleMaker");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/Run2011_May10ReReco_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/Run2011_PromptReco1_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/Run2011_PromptReco2_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/SingleMu_Aug05ReReco_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/SingleMu_PromptReco-v6_Mu.root");
      data->Process("Analyzer.C+","sample=data");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="dataQCD2"||sample=="all")
    {
      TDSet *dataQCD2 = new TDSet("top","*","/PATNtupleMaker");
      dataQCD2->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/Run2011_May10ReReco_Mu.root");
      dataQCD2->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/Run2011_PromptReco1_Mu.root");
      dataQCD2->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/Run2011_PromptReco2_Mu.root");
      dataQCD2->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/SingleMu_Aug05ReReco_Mu.root");
      dataQCD2->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/SingleMu_PromptReco-v6_Mu.root");
      dataQCD2->Process("Analyzer.C+","QCD2 sample=data");
    }

}
