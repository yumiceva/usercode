
void runZprime(TString sample="all") 
{
  TString desdir = "/uscms/home/yumiceva/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/";
  TProof *p = TProof::Open("lite://", desdir ,desdir);

  //p->AddDynamicPath("");
  p->Exec("gSystem->Load(\"/uscms/home/yumiceva/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libYumicevaTop7TeV.so\")");
  p->Exec("gSystem->Load(\"/uscms/home/yumiceva/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libCondFormatsJetMETObjects.so\")");
  p->AddIncludePath("/uscms/home/yumiceva/work/CMSSW_4_2_4/src/");

  p->Archive(" ",desdir);



  //p->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION", "LOCAL"));

  if (sample=="MC"||sample=="ttbar"||sample=="all")
    {
      TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/TTbar_Mu.root");
      mc_ttbar->Process("Zprime.C+","sample=ttbar");
    }

  if (sample=="MC"||sample=="WJets"||sample=="all")
    {
      TDSet *mc_Wjets = new TDSet("top","*","/PATNtupleMaker");
      mc_Wjets->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WJets_Mu.root");
      mc_Wjets->Process("Zprime.C+","sample=WJets");
    }

  if (sample=="MC"||sample=="QCD"||sample=="all")
    {
      TDSet *mc_QCD = new TDSet("top","*","/PATNtupleMaker");
      mc_QCD->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/QCD_Mu.root");
      mc_QCD->Process("Zprime.C+","sample=QCD");
    }

  if (sample=="MC"||sample=="STsch"||sample=="all")
    {
      TDSet *mc_STsch = new TDSet("top","*","/PATNtupleMaker");
      mc_STsch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/STsch_Mu.root");
      mc_STsch->Process("Zprime.C+","sample=STsch");
    }

  if (sample=="MC"||sample=="STtch"||sample=="all")
    {
      TDSet *mc_STtch = new TDSet("top","*","/PATNtupleMaker");
      mc_STtch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/STtch_Mu.root");
      mc_STtch->Process("Zprime.C+","sample=STtch");
    }

  if (sample=="MC"||sample=="STtWch"||sample=="all")
    {
      TDSet *mc_STtWch = new TDSet("top","*","/PATNtupleMaker");
      mc_STtWch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/STtWch_Mu.root");
      mc_STtWch->Process("Zprime.C+","sample=STtWch");
    }

  if (sample=="MC"||sample=="STsch_bar"||sample=="all")
    {
      TDSet *mc_STsch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STsch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/STsch_bar_Mu.root");
      mc_STsch_bar->Process("Zprime.C+","sample=STsch_bar");
    }

  if (sample=="MC"||sample=="STtch_bar"||sample=="all")
    {
      TDSet *mc_STtch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STtch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/STtch_bar_Mu.root");
      mc_STtch_bar->Process("Zprime.C+","sample=STtch_bar");
    }

  if (sample=="MC"||sample=="STtWch_bar"||sample=="all")
    {
      TDSet *mc_STtWch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STtWch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/STtWch_bar_Mu.root");
      mc_STtWch_bar->Process("Zprime.C+","sample=STtWch_bar");
    }

  if (sample=="MC"||sample=="WW"||sample=="all")
    {
      TDSet *mc_WW = new TDSet("top","*","/PATNtupleMaker");
      mc_WW->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WW_Mu.root");
      mc_WW->Process("Zprime.C+","sample=WW");
    }

  if (sample=="MC"||sample=="WZ"||sample=="all")
    {
      TDSet *mc_WZ = new TDSet("top","*","/PATNtupleMaker");
      mc_WZ->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WZ_Mu.root");
      mc_WZ->Process("Zprime.C+","sample=WZ");
    }

  if (sample=="MC"||sample=="ZJets"||sample=="all")
    {
      TDSet *mc_ZJets = new TDSet("top","*","/PATNtupleMaker");
      mc_ZJets->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/ZJets_Mu.root");
      mc_ZJets->Process("Zprime.C+","sample=ZJets");
    }


  if (sample=="MCsyst"||sample=="WJetsScaleUp"||sample=="all")
    {
      TDSet *mc_WJets_scaleup = new TDSet("top","*","/PATNtupleMaker");
      mc_WJets_scaleup->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WJets_scaledup_Mu.root");
      mc_WJets_scaleup->Process("Zprime.C+","sample=WJets_scaleup");
    }

  if (sample=="MCsyst"||sample=="WJetsScaleDown"||sample=="all")
    {
      TDSet *mc_WJets_scaledown = new TDSet("top","*","/PATNtupleMaker");
      mc_WJets_scaledown->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v8_1/WJets_scaledown_Mu.root");
      mc_WJets_scaledown->Process("Zprime.C+","sample=WJets_scaledown");
    }


  if (sample=="data"||sample=="all")
    {
      TDSet *data = new TDSet("top","*","/PATNtupleMaker");
      data->Add("/uscmst1b_scratch/lpc1/cmsroc/yumiceva/zprime_prod_Oct27/Data/Electrons/Run2011B-PromptReco-v1/Run2011B-PromptReco-v1_Ele.root");
      data->Process("Zprime.C+","sample=data");

      logList = p->GetManager()->GetSessionLogs()->GetListOfLogs();
      for (int i=1; i< logList->GetSize(); ++i)
	{
	  logElem = ( TProofLogElem* ) logList->At( i );
	  macro = logElem->GetMacro();
	  //macro->SaveSource("data_muons_"+TString(Form("%i",i))+".stdout");
	}
    }

}
