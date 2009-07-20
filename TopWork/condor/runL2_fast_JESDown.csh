
python replace.py JESDown on TopAnalysis_TTJets-madgraph_Fall08_all.py
python replace.py JESDown on TopAnalysis_WJets_madgraph_Fall08.py
python replace.py JESDown on TopAnalysis_ZJets_madgraph_Fall08.py
python replace.py JESDown on TopAnalysis_InclusiveMuPt15_Summer08.py

submitToCondor.py -s -c TopAnalysis_TTJets-madgraph_Fall08_all.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_TTJets_Fast.py -n 20 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_JESDown_Fast/

submitToCondor.py -s -c TopAnalysis_WJets_madgraph_Fall08.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_WJets_Fast.py -n 45 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_JESDown_Fast/

submitToCondor.py -s -c TopAnalysis_ZJets_madgraph_Fall08.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_ZJets_Fast.py -n 20 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_JESDown_Fast/

submitToCondor.py -s -c TopAnalysis_InclusiveMuPt15_Summer08.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_QCDMu_Fast.py -n 45 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_JESDown_Fast/


python replace.py JESDown off TopAnalysis_TTJets-madgraph_Fall08_all.py
python replace.py JESDown off TopAnalysis_WJets_madgraph_Fall08.py
python replace.py JESDown off TopAnalysis_ZJets_madgraph_Fall08.py
python replace.py JESDown off TopAnalysis_InclusiveMuPt15_Summer08.py

