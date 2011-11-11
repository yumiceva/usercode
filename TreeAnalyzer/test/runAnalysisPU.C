
void runAnalysisPU(TString sample="all") 
{
  TString desdir = "/uscms/home/yumiceva/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/";
  TProof *p = TProof::Open("lite://", desdir ,desdir);

  //p->AddDynamicPath("");
  p->Exec("gSystem->Load(\"/uscms/home/yumiceva/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libYumicevaTop7TeV.so\")");
  p->Exec("gSystem->Load(\"/uscms/home/yumiceva/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libCondFormatsJetMETObjects.so\")");
  p->AddIncludePath("/uscms/home/yumiceva/work/CMSSW_4_2_4/src/");

  p->Archive(" ",desdir);

  //p->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION", "LOCAL"));

  if (sample=="MCcomphep"||sample=="WpRH800"||sample=="all")
    {
      TDSet *mc_wp_rh_800 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_800->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-800_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_800->Process("Analyzer.C+","PUUP sample=WpRH800");
      mc_wp_rh_800->Process("Analyzer.C+","PUDOWN sample=WpRH800");
    }

  if (sample=="MCcomphep"||sample=="WpRH900"||sample=="all")
    {
      TDSet *mc_wp_rh_900 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_900->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-900_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_900->Process("Analyzer.C+","PUUP sample=WpRH900");

      mc_wp_rh_900->Process("Analyzer.C+","PUDOWN sample=WpRH900");
    }

  if (sample=="MCcomphep"||sample=="WpRH1000"||sample=="all")
    {
      TDSet *mc_wp_rh_1000 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1000->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1000_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1000->Process("Analyzer.C+","PUUP sample=WpRH1000");

      mc_wp_rh_1000->Process("Analyzer.C+","PUDOWN sample=WpRH1000");
    }

  if (sample=="MCcomphep"||sample=="WpRH1100"||sample=="all")
    {
      TDSet *mc_wp_rh_1100 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1100->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1100_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1100->Process("Analyzer.C+","PUUP sample=WpRH1100");

      mc_wp_rh_1100->Process("Analyzer.C+","PUDOWN sample=WpRH1100");
    }

  if (sample=="MCcomphep"||sample=="WpRH1200"||sample=="all")
    {
      TDSet *mc_wp_rh_1200 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1200->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1200_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1200->Process("Analyzer.C+","PUUP sample=WpRH1200");

      mc_wp_rh_1200->Process("Analyzer.C+","PUDOWN sample=WpRH1200");
    }

  if (sample=="MCcomphep"||sample=="WpRH1300"||sample=="all")
    {
      TDSet *mc_wp_rh_1300 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1300->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1300_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1300->Process("Analyzer.C+","PUUP sample=WpRH1300");

      mc_wp_rh_1300->Process("Analyzer.C+","PUDOWN sample=WpRH1300");
    }

  if (sample=="MCcomphep"||sample=="WpRH1400"||sample=="all")
    {
      TDSet *mc_wp_rh_1400 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1400->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1400_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1400->Process("Analyzer.C+","PUUP sample=WpRH1400");

      mc_wp_rh_1400->Process("Analyzer.C+","PUDOWN sample=WpRH1400");
    }

  if (sample=="MCcomphep"||sample=="WpRH1500"||sample=="all")
    {
      TDSet *mc_wp_rh_1500 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1500->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1500_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1500->Process("Analyzer.C+","PUUP sample=WpRH1500");

      mc_wp_rh_1500->Process("Analyzer.C+","PUDOWN sample=WpRH1500");
    }

  if (sample=="MCcomphep"||sample=="WpRH1600"||sample=="all")
    {
      TDSet *mc_wp_rh_1600 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_rh_1600->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1600_RightWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_rh_1600->Process("Analyzer.C+","PUUP sample=WpRH1600");

      mc_wp_rh_1600->Process("Analyzer.C+","PUDOWN sample=WpRH1600");
    }

  if (sample=="MCcomphep"||sample=="WpRH2000"||sample=="all")
    {
      TDSet *mc_wp_2000 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_2000->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-2000_Pythia_TuneZ2_7TeV_Mu.root");
      mc_wp_2000->Process("Analyzer.C+","PUUP sample=WpRH2000");

      mc_wp_2000->Process("Analyzer.C+","PUDOWN sample=WpRH2000");
    }

  if (sample=="MCcomphepL"||sample=="WpLH800"||sample=="all")
    {
      TDSet *mc_wp_lh_800 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_lh_800->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-800_LeftWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_lh_800->Process("Analyzer.C+","PUUP sample=WpLH800");
      mc_wp_lh_800->Process("Analyzer.C+","PUDOWN sample=WpLH800");
    }

  if (sample=="MCcomphepL"||sample=="WpLH1000"||sample=="all")
    {
      TDSet *mc_wp_lh_1000 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_lh_1000->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1000_LeftWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_lh_1000->Process("Analyzer.C+","PUUP sample=WpLH1000");

      mc_wp_lh_1000->Process("Analyzer.C+","PUDOWN sample=WpLH1000");
    }
  if (sample=="MCcomphepL"||sample=="WpLH1100"||sample=="all")
    {
      TDSet *mc_wp_lh_1100 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_lh_1100->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1100_LeftWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_lh_1100->Process("Analyzer.C+","PUUP sample=WpLH1100");

      mc_wp_lh_1100->Process("Analyzer.C+","PUDOWN sample=WpLH1100");
    }

  if (sample=="MCcomphepL"||sample=="WpLH1200"||sample=="all")
    {
      TDSet *mc_wp_lh_1200 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_lh_1200->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1200_LeftWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_lh_1200->Process("Analyzer.C+","PUUP sample=WpLH1200");

      mc_wp_lh_1200->Process("Analyzer.C+","PUDOWN sample=WpLH1200");
    }

  if (sample=="MCcomphepL"||sample=="WpLH1300"||sample=="all")
    {
      TDSet *mc_wp_lh_1300 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_lh_1300->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1300_LeftWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_lh_1300->Process("Analyzer.C+","PUUP sample=WpLH1300");

      mc_wp_lh_1300->Process("Analyzer.C+","PUDOWN sample=WpLH1300");
    }

  if (sample=="MCcomphepL"||sample=="WpLH1400"||sample=="all")
    {
      TDSet *mc_wp_lh_1400 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_lh_1400->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1400_LeftWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_lh_1400->Process("Analyzer.C+","PUUP sample=WpLH1400");

      mc_wp_lh_1400->Process("Analyzer.C+","PUDOWN sample=WpLH1400");
    }

  if (sample=="MCcomphepL"||sample=="WpLH1500"||sample=="all")
    {
      TDSet *mc_wp_lh_1500 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_lh_1500->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1500_LeftWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_lh_1500->Process("Analyzer.C+","PUUP sample=WpLH1500");

      mc_wp_lh_1500->Process("Analyzer.C+","PUDOWN sample=WpLH1500");
    }
  if (sample=="MCcomphepL"||sample=="WpLH1600"||sample=="all")
    {
      TDSet *mc_wp_lh_1600 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_lh_1600->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1600_LeftWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_lh_1600->Process("Analyzer.C+","PUUP sample=WpLH1600");

      mc_wp_lh_1600->Process("Analyzer.C+","PUDOWN sample=WpLH1600");
    }


  if (sample=="MCcomphepMix"||sample=="WpMix800"||sample=="all")
    {
      TDSet *mc_wp_mix_800 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_mix_800->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-800_MixRLWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_mix_800->Process("Analyzer.C+","PUUP sample=WpMix800");
      mc_wp_mix_800->Process("Analyzer.C+","PUDOWN sample=WpMix800");
    }

  if (sample=="MCcomphepMix"||sample=="WpMix900"||sample=="all")
    {
      TDSet *mc_wp_mix_900 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_mix_900->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-900_MixRLWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_mix_900->Process("Analyzer.C+","PUUP sample=WpMix900");

      mc_wp_mix_900->Process("Analyzer.C+","PUDOWN sample=WpMix900");
    }

  if (sample=="MCcomphepMix"||sample=="WpMix1000"||sample=="all")
    {
      TDSet *mc_wp_mix_1000 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_mix_1000->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1000_MixRLWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_mix_1000->Process("Analyzer.C+","PUUP sample=WpMix1000");

      mc_wp_mix_1000->Process("Analyzer.C+","PUDOWN sample=WpMix1000");
    }

  if (sample=="MCcomphepMix"||sample=="WpMix1100"||sample=="all")
    {
      TDSet *mc_wp_mix_1100 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_mix_1100->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1100_MixRLWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_mix_1100->Process("Analyzer.C+","PUUP sample=WpMix1100");

      mc_wp_mix_1100->Process("Analyzer.C+","PUDOWN sample=WpMix1100");
    }

  if (sample=="MCcomphepMix"||sample=="WpMix1200"||sample=="all")
    {
      TDSet *mc_wp_mix_1200 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_mix_1200->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1200_MixRLWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_mix_1200->Process("Analyzer.C+","PUUP sample=WpMix1200");

      mc_wp_mix_1200->Process("Analyzer.C+","PUDOWN sample=WpMix1200");
    }

  if (sample=="MCcomphepMix"||sample=="WpMix1300"||sample=="all")
    {
      TDSet *mc_wp_mix_1300 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_mix_1300->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1300_MixRLWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_mix_1300->Process("Analyzer.C+","PUUP sample=WpMix1300");

      mc_wp_mix_1300->Process("Analyzer.C+","PUDOWN sample=WpMix1300");
    }

  if (sample=="MCcomphepMix"||sample=="WpMix1400"||sample=="all")
    {
      TDSet *mc_wp_mix_1400 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_mix_1400->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1400_MixRLWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_mix_1400->Process("Analyzer.C+","PUUP sample=WpMix1400");

      mc_wp_mix_1400->Process("Analyzer.C+","PUDOWN sample=WpMix1400");
    }

  if (sample=="MCcomphepMix"||sample=="WpMix1500"||sample=="all")
    {
      TDSet *mc_wp_mix_1500 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_mix_1500->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1500_MixRLWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_mix_1500->Process("Analyzer.C+","PUUP sample=WpMix1500");

      mc_wp_mix_1500->Process("Analyzer.C+","PUDOWN sample=WpMix1500");
    }

  if (sample=="MCcomphepMix"||sample=="WpMix1600"||sample=="all")
    {
      TDSet *mc_wp_mix_1600 = new TDSet("top","*","/PATNtupleMaker");
      mc_wp_mix_1600->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WprimeToTB_M-1600_MixRLWprime_TuneZ2_7TeV_comphep_Mu.root");
      mc_wp_mix_1600->Process("Analyzer.C+","PUUP sample=WpMix1600");

      mc_wp_mix_1600->Process("Analyzer.C+","PUDOWN sample=WpMix1600");
    }

  if (sample=="MC"||sample=="ttbar"||sample=="all")
    {
      TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/TTbar_Mu.root");
      mc_ttbar->Process("Analyzer.C+","PUUP sample=ttbar");

      mc_ttbar->Process("Analyzer.C+","PUDOWN sample=ttbar");
    }

  if (sample=="MC"||sample=="WJets"||sample=="all")
    {
      TDSet *mc_Wjets = new TDSet("top","*","/PATNtupleMaker");
      mc_Wjets->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WJets_Mu.root");
      mc_Wjets->Process("Analyzer.C+","PUUP sample=WJets");

      mc_Wjets->Process("Analyzer.C+","PUDOWN sample=WJets");
    }

  if (sample=="MC"||sample=="QCD"||sample=="all")
    {
      TDSet *mc_QCD = new TDSet("top","*","/PATNtupleMaker");
      mc_QCD->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/QCD_Mu.root");
      mc_QCD->Process("Analyzer.C+","PUUP sample=QCD");

      mc_QCD->Process("Analyzer.C+","PUDOWN sample=QCD");
    }

  if (sample=="MC"||sample=="STsch"||sample=="all")
    {
      TDSet *mc_STsch = new TDSet("top","*","/PATNtupleMaker");
      mc_STsch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/STsch_Mu.root");
      mc_STsch->Process("Analyzer.C+","PUUP sample=STsch");

      mc_STsch->Process("Analyzer.C+","PUDOWN sample=STsch");
    }

  if (sample=="MC"||sample=="STtch"||sample=="all")
    {
      TDSet *mc_STtch = new TDSet("top","*","/PATNtupleMaker");
      mc_STtch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/STtch_Mu.root");
      mc_STtch->Process("Analyzer.C+","PUUP sample=STtch");

      mc_STtch->Process("Analyzer.C+","PUDOWN sample=STtch");
    }

  if (sample=="MC"||sample=="STtWch"||sample=="all")
    {
      TDSet *mc_STtWch = new TDSet("top","*","/PATNtupleMaker");
      mc_STtWch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/STtWch_Mu.root");
      mc_STtWch->Process("Analyzer.C+","PUUP sample=STtWch");

      mc_STtWch->Process("Analyzer.C+","PUDOWN sample=STtWch");
    }

  if (sample=="MC"||sample=="STsch_bar"||sample=="all")
    {
      TDSet *mc_STsch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STsch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/STsch_bar_Mu.root");
      mc_STsch_bar->Process("Analyzer.C+","PUUP sample=STsch_bar");

      mc_STsch_bar->Process("Analyzer.C+","PUDOWN sample=STsch_bar");
    }

  if (sample=="MC"||sample=="STtch_bar"||sample=="all")
    {
      TDSet *mc_STtch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STtch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/STtch_bar_Mu.root");
      mc_STtch_bar->Process("Analyzer.C+","PUUP sample=STtch_bar");

      mc_STtch_bar->Process("Analyzer.C+","PUDOWN sample=STtch_bar");
    }

  if (sample=="MC"||sample=="STtWch_bar"||sample=="all")
    {
      TDSet *mc_STtWch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STtWch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/STtWch_bar_Mu.root");
      mc_STtWch_bar->Process("Analyzer.C+","PUUP sample=STtWch_bar");

      mc_STtWch_bar->Process("Analyzer.C+","PUDOWN sample=STtWch_bar");
    }

  if (sample=="MC"||sample=="WW"||sample=="all")
    {
      TDSet *mc_WW = new TDSet("top","*","/PATNtupleMaker");
      mc_WW->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WW_Mu.root");
      mc_WW->Process("Analyzer.C+","PUUP sample=WW");

      mc_WW->Process("Analyzer.C+","PUDOWN sample=WW");
    }

  if (sample=="MC"||sample=="WZ"||sample=="all")
    {
      TDSet *mc_WZ = new TDSet("top","*","/PATNtupleMaker");
      mc_WZ->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WZ_Mu.root");
      mc_WZ->Process("Analyzer.C+","PUUP sample=WZ");

      mc_WZ->Process("Analyzer.C+","PUDOWN sample=WZ");
    }

  if (sample=="MC"||sample=="ZJets"||sample=="all")
    {
      TDSet *mc_ZJets = new TDSet("top","*","/PATNtupleMaker");
      mc_ZJets->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/ZJets_Mu.root");
      mc_ZJets->Process("Analyzer.C+","PUUP sample=ZJets");

      mc_ZJets->Process("Analyzer.C+","PUDOWN sample=ZJets");
    }

}
