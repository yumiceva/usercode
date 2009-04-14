hadd /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/nominal/ABCD_TTJets_all.root /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/ABCD_TTJets_0*.root
hadd /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/nominal/ABCD_WJets_all.root /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/ABCD_WJets_0*.root
hadd /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/nominal/ABCD_ZJets_all.root /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/ABCD_ZJets_0*.root
hadd /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/nominal/ABCD_MuPt15_all.root /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/ABCD_MuPt15_0*.root

python ~/work/CMSSW_2_2_6/src/TopWork/scripts/counter.py /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/ ABCD_TTJets_ /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/nominal/ABCD_TTJets_all.root
python ~/work/CMSSW_2_2_6/src/TopWork/scripts/counter.py /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/ ABCD_WJets_ /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/nominal/ABCD_WJets_all.root
python ~/work/CMSSW_2_2_6/src/TopWork/scripts/counter.py /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/ ABCD_ZJets_ /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/nominal/ABCD_ZJets_all.root
python ~/work/CMSSW_2_2_6/src/TopWork/scripts/counter.py /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/ ABCD_MuPt15_ /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/ABCD_analysis/nominal/ABCD_MuPt15_all.root

