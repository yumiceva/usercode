
python replace.py JESUp on TopAnalysis_TTJets-madgraph_Fall08_all.py
python replace.py JESUp on TopAnalysis_WJets_madgraph_Fall08.py
python replace.py JESUp on TopAnalysis_ZJets_madgraph_Fall08.py
python replace.py JESUp on TopAnalysis_InclusiveMuPt15_Summer08.py
python replace.py JESUp on TopAnalysis_ST_s.py
python replace.py JESUp on TopAnalysis_ST_t.py
python replace.py JESUp on TopAnalysis_ST_tW.py

submitToCondor.py -s -c TopAnalysis_TTJets-madgraph_Fall08_all.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_TTJets_madgraph_Fall08.py -n 20 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_JESUp/

submitToCondor.py -s -c TopAnalysis_WJets_madgraph_Fall08.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_WJets_madgraph_Fall08.py -n 20 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_JESUp/

submitToCondor.py -s -c TopAnalysis_ZJets_madgraph_Fall08.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_ZJets_madgraph_Fall08.py -n 20 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_JESUp/

submitToCondor.py -s -c TopAnalysis_InclusiveMuPt15_Summer08.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_InclusiveMuPt15.py -n 45 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_JESUp/

submitToCondor.py -s -c TopAnalysis_ST_s.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_ST_s.py -n 20 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_JESUp/

submitToCondor.py -s -c TopAnalysis_ST_t.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_ST_t.py -n 20 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_JESUp/

submitToCondor.py -s -c TopAnalysis_ST_tW.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_ST_tW.py -n 20 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_JESUp/

#submitToCondor.py -s -c TopAnalysis_TauolaTTbar.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_TauolaTTbar.py -n 6 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_JESUp/

#submitToCondor.py -s -c TopAnalysis_VVJets.py -l /uscms/home/yumiceva/work/CMSSW_2_2_6/src/TopQuarkAnalysis/TopPairBSM/python/PatTuple_VVJets.py -n 6 -o /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_JESUp/

python replace.py JESUp off TopAnalysis_TTJets-madgraph_Fall08_all.py
python replace.py JESUp off TopAnalysis_WJets_madgraph_Fall08.py
python replace.py JESUp off TopAnalysis_ZJets_madgraph_Fall08.py
python replace.py JESUp off TopAnalysis_InclusiveMuPt15_Summer08.py
python replace.py JESUp off TopAnalysis_ST_s.py
python replace.py JESUp off TopAnalysis_ST_t.py
python replace.py JESUp off TopAnalysis_ST_tW.py

