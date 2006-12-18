#!/bin/csh
#
############################################################################
#
#
# Author: Francisco Yumiceva
# yumiceva@fnal.gov
#

echo "Configuring CMS environment...."

source /afs/fnal.gov/exp/cms/setup/cshrc uaf

cd /uscmst1b_scratch/lpc1/cmsroc/yumiceva/CMSSW_1_2_0_pre3/src/RecoVertex/BeamSpotProducer/test/scripts
eval `scramv1 runtime -csh`

if (${?_CONDOR_SCRATCH_DIR}) then 
   echo " change to Condor Batch system scratch disk"
   cd ${_CONDOR_SCRATCH_DIR}
else 
   echo " no Condor scratch directory! "
endif

if (-e  {PATHTOOUT}{OUTFILE}) then
  echo " {PATHTOOUT}{OUTFILE} already exists exiting"
  echo " to rerun delete or rename  first "
  exit 1 
endif 

cmsRun -p {CONFIGFILE}
if ( $? == 0 ) then 
    echo 'processing File went fine, now move everything back. '
    cp {OUTFILE} {PATHTOOUT}
    /bin/rm -f {OUTFILE}
    exit 0
else
      echo 'Error processing File cleaning up now '  
      /bin/rm -f {OUTFILE}
      exit 1
endif

