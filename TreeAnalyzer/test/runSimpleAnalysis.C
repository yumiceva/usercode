void runSimpleAnalysis(TString sample="all", bool getLogs=false)  
{

  TString desdir = "/uscms/home/yumiceva/lpc1/4tops2013_templates/";

  TProof *p = TProof::Open("lite://", desdir ,desdir);

  //p->AddDynamicPath("");
  p->Exec("gSystem->Load(\"/uscms/home/yumiceva/work/CMSSW_5_3_3/lib/slc5_amd64_gcc462/libYumicevaTop7TeV.so\")");
  p->AddIncludePath("/uscms/home/yumiceva/work/CMSSW_5_3_3/src/");
  //p->AddIncludePath("-I/uscmst1/prod/sw/cms/slc5_amd64_gcc462/external/boost/1.47.0/include/");

  p->Archive(" ",desdir);

  //p->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION", "LOCAL"));

  if (sample=="MC"||sample=="ttbar"||sample=="all")
    {
      TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbar->Add("/uscms/home/yumiceva/lpc1/4tops2013_ntuples_v1/muons/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola.root"); 
      mc_ttbar->Process("SimpleAnalyzer.C+","sample=ttbar");
	
    }
  
  if (sample=="data"||sample=="all")
    {
      TDSet *data = new TDSet("top","*","/PATNtupleMaker");
      data->Add("/uscms/home/yumiceva/lpc1/4tops2013_ntuples_v1/muons/Run2012A-recover-06Aug2012-v1.root");
      //data->Add("/uscms/home/yumiceva/lpc1/4tops2013_ntuples_v1/muons/Run2012B-13Jul2012-v1.root");
      
      data->Process("SimpleAnalyzer.C+","verbose sample=data");
      // get log files
      if (getLogs)
	{
	  logList = p->GetManager()->GetSessionLogs()->GetListOfLogs();
	  for (int i=1; i< logList->GetSize(); ++i)
	    {
	      logElem = ( TProofLogElem* ) logList->At( i );
	      macro = logElem->GetMacro();
	      macro->SaveSource("data_muons_"+TString(Form("%i",i))+".stdout");
	    }
	}
    }
}
