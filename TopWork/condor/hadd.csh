hadd /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/nominal/TopAnalysis_TTJets-madgraph_Fall08_all_all.root /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/TopAnalysis_TTJets-madgraph_Fall08_all_00*.root
hadd /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/nominal/TopAnalysis_WJets_madgraph_Fall08_all.root /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/TopAnalysis_WJets_madgraph_Fall08_00*.root
hadd /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/nominal/TopAnalysis_ZJets_madgraph_Fall08_all.root /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/TopAnalysis_ZJets_madgraph_Fall08_00*.root
hadd /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/nominal/TopAnalysis_InclusiveMuPt15_Summer08_all.root /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/TopAnalysis_InclusiveMuPt15_Summer08_00*.root

python ~/work/CMSSW_2_2_5/src/TopWork/scripts/counter.py /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/ TopAnalysis_TTJets-madgraph_Fall08_all_ /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/nominal/TopAnalysis_TTJets-madgraph_Fall08_all_all.root
python ~/work/CMSSW_2_2_5/src/TopWork/scripts/counter.py /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/ TopAnalysis_WJets_madgraph_Fall08_ /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/nominal/TopAnalysis_WJets_madgraph_Fall08_all.root
python ~/work/CMSSW_2_2_5/src/TopWork/scripts/counter.py /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/ TopAnalysis_ZJets_madgraph_Fall08_ /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/nominal/TopAnalysis_ZJets_madgraph_Fall08_all.root
python ~/work/CMSSW_2_2_5/src/TopWork/scripts/counter.py /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/ TopAnalysis_InclusiveMuPt15_Summer08_ /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_5/LowMass_analysisIP5/nominal/TopAnalysis_InclusiveMuPt15_Summer08_all.root

