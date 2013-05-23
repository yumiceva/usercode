{

  gSystem->Load("libYumicevaTop7TeV.so");
  gSystem->Load("libYumicevaTreeAnalyzer.so");
  gSystem->Load("libCondFormatsJetMETObjects.so");
  gSystem->Load("libPhysicsToolsUtilities.so");
  //gSystem->SetIncludePath("-I$CMSSW_BASE/src/ -I$CMSSW_RELEASE_BASE/src/ -I/uscmst1/prod/sw/cms/slc5_amd64_gcc462/external/boost/1.47.0/include/");
  gSystem->SetIncludePath("-I$CMSSW_BASE/src/"); // -I$CMSSW_RELEASE_BASE/src/"); 
}
