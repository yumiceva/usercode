#! /usr/bin/env python

import sys
import os
import fileinput

if len(sys.argv) < 2:
    print "\n usage: setupProd.py <path> \n"
    print "   <path> = path to directory where the crab configuration files will be stored.\n"
    print "   <channel> = default both electron and muon otherwise specify here either \"electron\" or \"muon\""
    sys.exit()

# configutation
path = sys.argv[1]
DoMuons = False
DoElectrons = False
if len(sys.argv)==3:
    if sys.argv[2]=="electrons": DoElectrons = True
    if sys.argv[2]=="muons": DoMuons = True
else:
    DoMuons = DoElectrons = True

current = os.getcwd() # current path
py_cfg = "PATskim.py" # cfg config file
crab_cfg = "crab.py" # crab config file

# Dictionaries
directoriesMuData = {}
directoriesElData = {}
directoriesMuMC = {}
directoriesElMC = {}
jsonFilesMu = {}
jsonFilesEl = {}

######## Muons
directoriesMuData['Run2010A_Mu9'] = '/Mu/Run2010A-Nov4ReReco_v1/AOD'  # 135821   144114
directoriesMuData['Run2010B_Mu9'] = '/Mu/Run2010B-Nov4ReReco_v1/AOD'  # 146240   147195 
directoriesMuData['Run2010B_Mu15'] = '/Mu/Run2010B-Nov4ReReco_v1/AOD' # 147196   149711

jsonFilesMu['Run2010A_Mu9']  = 'Cert_136033-149442_7TeV_Nov4ReReco_Collisions10_JSON.txt'
jsonFilesMu['Run2010B_Mu9']  = 'Cert_146240-147195_7TeV_Nov4ReReco.txt'
jsonFilesMu['Run2010B_Mu15'] = 'Cert_147196-149442_7TeV_Nov4ReReco.txt'

## Attention:
## if a dataset name has ":g" it means that for that sample the jobs will be sent to Nebraska
## using condor_g

directoriesMuMC['TTbar_D6T_Mu'] = '/TTJets_TuneD6T_7TeV-madgraph-tauola/Fall10-START38_V12-v2/AODSIM'
directoriesMuMC['WJets_D6T_Mu'] = '/WJetsToLNu_TuneD6T_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM'
directoriesMuMC['ZJets_D6T_Mu'] ='/DYJetsToLL_TuneD6T_M-50_7TeV-madgraph-tauola/Fall10-START38_V12-v2/AODSIM'
directoriesMuMC['Wc_D6T_Mu'] = '/WCJetsToLNu_TuneD6T_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM:g'
directoriesMuMC['Vqq_D6T_Mu'] = '/VQQJetsToLL_TuneD6T_7TeV-madgraph-tauola/Fall10-START38_V12-v2/AODSIM:g'

directoriesMuMC['TTbar_Z2_Mu'] = '/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall10-START38_V12-v3/AODSIM:g'
directoriesMuMC['WJets_Z2_Mu'] = '/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM'
directoriesMuMC['ZJets_Z2_Mu'] = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall10-START38_V12-v2/AODSIM:g'
directoriesMuMC['STsch_Z2_Mu'] = '/TToBLNu_TuneZ2_s-channel_7TeV-madgraph/Fall10-START38_V12-v1/AODSIM'
directoriesMuMC['STtch_Z2_Mu'] = '/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/Fall10-START38_V12-v2/AODSIM'
directoriesMuMC['STtWch_Z2_Mu'] = '/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph/Fall10-START38_V12-v2/AODSIM'
directoriesMuMC['QCD_Z2_Mu'] = '/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/AODSIM'
directoriesMuMC['WW_Z2_Mu'] = '/WWtoAnything_TuneZ2_7TeV-pythia6-tauola/Fall10-START38_V12-v1/AODSIM' # missing at fnal/unl
directoriesMuMC['WZ_Z2_Mu'] = '/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/Fall10-START38_V12-v1/AODSIM'

directoriesMuMC['TTbar_scaleup_Mu'] = '/TTJets_TuneD6T_scaleup_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM'
directoriesMuMC['TTbar_scaledown_Mu'] = '/TTJets_TuneD6T_scaledown_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM'
directoriesMuMC['TTbar_matchingup_Mu'] = '/TTJets_TuneD6T_matchingup_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM'
directoriesMuMC['TTbar_matchingdown_Mu'] = '/TTJets_TuneD6T_matchingdown_7TeV-madgraph-tauola/Fall10-START38_V12-v2/AODSIM'
directoriesMuMC['TTbar_smallerISRFSR_Mu'] = '/TTJets_TuneD6T_smallerISRFSR_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM'
directoriesMuMC['TTbar_largerISRFSR_Mu'] = '/TTJets_TuneD6T_largerISRFSR_7TeV-madgraph-tauola/Fall10-START38_V12-v2/AODSIM'
directoriesMuMC['TTbar_0Jetalpgen_Mu'] = '/TT0Jets_40GeVTh_TuneZ2_7TeV-alpgen-tauola/Fall10-START38_V12-v1/AODSIM:g'
directoriesMuMC['TTbar_1Jetalpgen_Mu'] = '/TT1Jets_40GeVTh_TuneZ2_7TeV-alpgen-tauola/Fall10-START38_V12-v1/AODSIM:g'
directoriesMuMC['TTbar_2Jetalpgen_Mu'] = '/TT2Jets_40GeVTh_TuneZ2_7TeV-alpgen-tauola/Fall10-START38_V12-v1/AODSIM:g'
directoriesMuMC['TTbar_3Jetalpgen_Mu'] = '/TT3Jets_40GeVTh_TuneZ2_7TeV-alpgen-tauola/Fall10-START38_V12-v1/AODSIM:g'
directoriesMuMC['TTbar_4Jetalpgen_Mu'] = '/TT4Jets_40GeVTh_TuneZ2_7TeV-alpgen-tauola/Fall10-START38_V12-v1/AODSIM:g'
directoriesMuMC['TTbar_PU_Mu'] = '/TTJets_TuneD6T_7TeV-madgraph-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/AODSIM' # missing

directoriesMuMC['WJets_scaleup_Mu'] = '/WJets_TuneD6T_scaleup_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM'
directoriesMuMC['WJets_scaledown_Mu'] = '/WJets_TuneD6T_scaledown_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM:g'
directoriesMuMC['WJets_matchingdown_Mu'] = '/WJets_TuneD6T_matchingdown_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM:g'
directoriesMuMC['WJets_matchingup_Mu'] = '/WJets_TuneD6T_matchingup_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM'

directoriesMuMC['ZJets_scaleup_Mu'] = '/DYJetsToLL_TuneD6T_scaleup_7TeV-madgraph-tauola/Fall10-START38_V12-v2/AODSIM'
directoriesMuMC['ZJets_scaledown_Mu'] = '/DYJetsToLL_TuneD6T_scaledown_7TeV-madgraph-tauola/Fall10-START38_V12-v2/AODSIM'
directoriesMuMC['ZJets_matchingup_Mu'] = '/DYJetsToLL_TuneD6T_matchingup_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM'
directoriesMuMC['ZJets_matchingdown_Mu'] = '/DYJetsToLL_TuneD6T_matchingdown_7TeV-madgraph-tauola/Fall10-START38_V12-v2/AODSIM'

########## Electrons
directoriesElData['TrigA'] = '/EG/Run2010A-Sep17ReReco_v2/RECO'       #trig:Ele15_LW, Oct 1st json
directoriesElData['TrigB'] = '/EG/Run2010A-Sep17ReReco_v2/RECO'       #trig:Ele15_LW, July16 json
directoriesElData['TrigC'] = '/EG/Run2010A-Sep17ReReco_v2/RECO'       #trig:Ele15_LW, Oct 1st json
directoriesElData['PromptRecoTrigC'] = '/Electron/Run2010B-PromptReco-v2/RECO'
directoriesElData['TrigD'] = '/Electron/Run2010B-PromptReco-v2/RECO'
directoriesElData['TrigE'] = '/Electron/Run2010B-PromptReco-v2/RECO'  #trig:Ele17_SW_CaloID, Oct 1st json
directoriesElData['TrigF'] = '/Electron/Run2010B-PromptReco-v2/RECO'

jsonFilesEl['TrigA'] = 'Cert_132440-140040_El_Oct15_JSON.txt'
jsonFilesEl['TrigB'] = 'Cert_140041-143962_El_Oct15_JSON.txt'
jsonFilesEl['TrigC'] = 'Cert_143963-146427_El_Oct15_JSON.txt'
jsonFilesEl['PromptRecoTrigC'] = 'Cert_143963-146427_El_Oct15_JSON.txt'
jsonFilesEl['TrigD'] = 'Cert_146428-147116_El_Oct15_JSON.txt'
jsonFilesEl['TrigE'] = 'Cert_147117-148818_El_Oct29_JSON.txt'
jsonFilesEl['TrigF'] = 'Cert_148819-149442_El_Nov5_JSON.txt'#'Cert_148819-148864_El_Oct29_JSON.txt'


directoriesElMC['TTbar_El'] = '/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['Wjets_El'] = '/WJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['Zjets_El'] = '/ZJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['STtch_El'] = '/SingleTop_tChannel-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['STtWch_El']= '/SingleTop_tWChannel-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['QCDEMEn2030_El'] = '/QCD_EMEnriched_Pt20to30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['QCDEMEn3080_El'] = '/QCD_EMEnriched_Pt30to80/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['QCDEMEn80170_El'] = '/QCD_EMEnriched_Pt80to170/Spring10-START3X_V26_S09-v3/GEN-SIM-RECO'
directoriesElMC['QCDBCtoE2030_El'] = '/QCD_BCtoE_Pt20to30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['QCDBCtoE3080_El'] = '/QCD_BCtoE_Pt30to80/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['QCDBCtoE80170_El'] = '/QCD_BCtoE_Pt80to170/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['PhoJet40100_El'] = '/PhotonJets_Pt40to100-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['PhoJet100200_El'] = '/PhotonJets_Pt100to200-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['PhoJet200Inf_El'] = '/PhotonJets_Pt200toInf-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['Wc_El'] = '/WCJets_7TeV-madgraph/Spring10-START3X_V26-v1/GEN-SIM-RECO'
directoriesElMC['Vqq_El'] = '/VqqJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'


print "==> make directories"
alldirs = []
if DoMuons:
    alldirs = directoriesMuData.keys() + directoriesMuMC.keys()
if DoElectrons:
    alldirs = alldirs + directoriesElData.keys() + directoriesElMC.keys()


if not os.path.isdir(path):
    os.mkdir(path)
    print path

for dir in alldirs:
    apath = path+'/'+dir
    if os.path.isdir(apath):
        print apath+" already exists."
    else:
        os.mkdir(apath)
        print apath

print "==> create soft link \"production\" in current directory to production directory"
if not os.path.islink("production"):
    os.system("ln -s "+path+" production")
else:
    print "link \"production\" already exists, remove current link and create a new one"
    print "old link:"
    os.system("ls -l production")
    os.system("rm production")
    os.system("ln -s "+path+" production")
    print "new link:"
os.system("ls -l production")
                
print "==> create cfg files based in "+py_cfg+"\n"

if DoMuons:
    
    print "===> for muon channel"
    for dir in directoriesMuData.keys():

        afile = path+"/"+dir+"/"+py_cfg
        os.system("cp "+py_cfg+" "+afile)

        for line in fileinput.FileInput(afile, inplace=1):
            line = line.replace("inputType = \"MC\"","inputType = \"DATA\"")
            line = line.replace("channel = \"electron\"","channel = \"muon\"")
            line = line.replace("eventtype=\"TTJets\"","eventtype=\""+dir+"\"")
            print line,
    
    for dir in directoriesMuMC.keys():

        afile = path+"/"+dir+"/"+py_cfg
        os.system("cp "+py_cfg+" "+afile)

        type = dir.strip("_Mu")
        
        for line in fileinput.FileInput(afile, inplace=1):
            line = line.replace("inputType = \"DATA\"","inputType = \"MC\"")
            line = line.replace("channel = \"electron\"","channel = \"muon\"")
            line = line.replace("eventtype=\"TTJets\"","eventtype=\""+type+"\"")
            print line,

if DoElectrons:
    
    print "===> for electron channel"
    for dir in directoriesElData.keys():

        afile = path+"/"+dir+"/"+py_cfg
        os.system("cp "+py_cfg+" "+afile)

        for line in fileinput.FileInput(afile, inplace=1):
            line = line.replace("inputType = \"MC\"","inputType = \"DATA\"")
            line = line.replace("channel = \"muon\"","channel = \"electron\"")
            line =line.replace("eventtype=\"TTJets\"","eventtype=\""+dir+"\"")
            print line,
        
    for dir in directoriesElMC.keys():

        afile = path+"/"+dir+"/"+py_cfg
        os.system("cp "+py_cfg+" "+afile)
    
        type = dir.strip("_El")
    
        for line in fileinput.FileInput(afile, inplace=1):
            line = line.replace("inputType = \"DATA\"","inputType = \"MC\"")
            line = line.replace("channel = \"muon\"","channel = \"electron\"")
            line = line.replace("eventtype=\"TTJets\"","eventtype=\""+type+"\"")
            print line,


print "==> create crab configuration files"

crabData = '''
[CMSSW]
datasetpath = %(DATASET)s
pset = PATskim.py
total_number_of_events = -1
lumi_mask = %(JSON)s
total_number_of_lumis = -1
lumis_per_job = 20

[USER]
return_data = 1
copy_data = 0
ui_working_dir = %(DIR)s
storage_element = cmssrm.fnal.gov
storage_path = /srm/managerv2?SFN=/11/
user_remote_dir = /store/user/ttmuj/
publish_data = 0
publish_data_name = Summer10_363_v2
dbs_url_for_publication = https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet
check_user_remote_dir = 0

[CRAB]
jobtype = cmssw
scheduler = condor
'''

crabMC = '''
[CMSSW]
datasetpath = %(DATASET)s
pset = PATskim.py
total_number_of_events = -1
events_per_job = 25000

[USER]
return_data = 1
copy_data = 0
ui_working_dir = %(DIR)s
storage_element = cmssrm.fnal.gov
storage_path = /srm/managerv2?SFN=/11/
user_remote_dir = /store/user/ttmuj/
publish_data = 0
publish_data_name = Summer10_363_v2
dbs_url_for_publication = https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet
check_user_remote_dir = 0

[CRAB]
jobtype = cmssw
scheduler = condor
'''

crabMCg = '''
[CMSSW]
datasetpath = %(DATASET)s
pset = PATskim.py
total_number_of_events = -1
events_per_job = 25000

[USER]
return_data = 1
copy_data = 0
ui_working_dir = %(DIR)s
storage_element = cmssrm.fnal.gov
storage_path = /srm/managerv2?SFN=/11/
user_remote_dir = /store/user/ttmuj/
publish_data = 0
publish_data_name = Summer10_363_v2
dbs_url_for_publication = https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet
check_user_remote_dir = 0

[CRAB]
jobtype = cmssw
scheduler = condor_g

[EDG]
se_white_list = unl

'''


fcrab_data = open("launch_crab_data.csh","w")
fcrab_mc = open("launch_crab_mc.csh","w")

if DoMuons:
    
    for dir in directoriesMuData.keys():

        ajson = jsonFilesMu[dir]
    
        os.system("cp json/"+ajson+" "+path+"/"+dir)
        
        adict = {}
        adict['DATASET'] = directoriesMuData[dir]
        adict['JSON'] = ajson
        adict['DIR'] = dir
        
        acrab = crabData%adict

        fileout = open(path+"/"+dir+"/crab.cfg","w")
        fileout.writelines(acrab)
        fileout.close()
        
        fcrab_data.write("cd "+path+"/"+dir+"\n")
        fcrab_data.write("crab -create -cfg crab.cfg"+"\n")
        if dir.find("Prompt")!= -1 or dir.find("Sep17")!=-1:
            fcrab_data.write("crab -submit 500 -c "+dir+"\n")
            fcrab_data.write("crab -submit 500 -c "+dir+"\n")
            fcrab_data.write("crab -submit 500 -c "+dir+"\n")
        else:
            fcrab_data.write("crab -submit all -c "+dir+"\n")
    

    for dir in directoriesMuMC.keys():

        adict = {}
        docondorg = False
        adict['DATASET'] = directoriesMuMC[dir]
        if directoriesMuMC[dir].find(":g")!=-1:
            docondorg = True
            adict['DATASET'] = directoriesMuMC[dir].strip(":g")
        
        adict['DIR'] = dir
    
        acrab = crabMC%adict
        if docondorg:
            acrab = crabMCg%adict
            
        fileout = open(path+"/"+dir+"/crab.cfg","w")
        fileout.writelines(acrab)
        fileout.close()

        fcrab_mc.write("cd "+path+"/"+dir+"\n")
        fcrab_mc.write("crab -create -cfg crab.cfg -submit all"+"\n")
        
if DoElectrons:
    
    for dir in directoriesElData.keys():

        ajson = jsonFilesEl[dir]
        
        os.system("cp json/"+ajson+" "+path+"/"+dir+"\n")
        
        adict = {}
        adict['DATASET'] = directoriesElData[dir]
        adict['JSON'] = ajson
        adict['DIR'] = dir
        
        acrab = crabData%adict
        
        fileout = open(path+"/"+dir+"/crab.cfg","w")
        fileout.writelines(acrab)
        fileout.close()

        fcrab_data.write("cd "+path+"/"+dir+"\n")
        fcrab_data.write("crab -create -cfg crab.cfg"+"\n")
        if dir.find("Prompt")!= -1:
            fcrab_data.write("crab -submit 500 -c "+dir+"\n")
            fcrab_data.write("crab -submit 500 -c "+dir+"\n")
            fcrab_data.write("crab -submit 500 -c "+dir+"\n")
        else:
            fcrab_data.write("crab -submit all -c "+dir+"\n")
                                            

    for dir in directoriesElMC.keys():

        adict = {}
        adict['DATASET'] = directoriesElMC[dir]
        adict['DIR'] = dir

        acrab = crabMC%adict
        
        fileout = open(path+"/"+dir+"/crab.cfg","w")
        fileout.writelines(acrab)
        fileout.close()

        fcrab_mc.write("cd "+path+"/"+dir+"\n")
        fcrab_mc.write("crab -create -cfg crab.cfg -submit all"+"\n")


fcrab_data.close()
fcrab_mc.close()

print "helper files: launch_crab_data.csh and launch_crab_mc.csh have been written."
print "check status with statusProd.py"

print "done."


                                                
