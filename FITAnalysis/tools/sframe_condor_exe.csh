#! /bin/csh
# Source version of CMSSW associated to SFRAME
source /uscmst1/prod/sw/cms/cshrc uaf
cd /uscms/home/yumiceva/work/sframe/CMSSW_5_3_3/src
eval `scramv1 runtime -csh`
cd /uscms/home/yumiceva/work/sframe/CMSSW_5_3_3/src/SFrame 
source setup.csh
setenv FASTJETDIR /uscms/home/yumiceva/work/sframe/fastjet/3.0.2-install/lib
setenv LD_LIBRARY_PATH ${FASTJETDIR}/lib:${LD_LIBRARY_PATH}
setenv LD_LIBRARY_PATH ${SFRAME_DIR}/SFrameTools/JetMETObjects/lib:${LD_LIBRARY_PATH}
# Run the sframe job
cd /uscms/home/yumiceva/work/sframe/CMSSW_5_3_3/src/SFrame/FITAnalysis/config
sframe_main $1
