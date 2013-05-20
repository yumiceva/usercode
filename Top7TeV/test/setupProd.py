#! /usr/bin/env python

import sys
import os
import fileinput

if len(sys.argv) < 2:
    print "\n usage: setupProd.py <path> \n"
    print "   <path> = path to directory where the crab configuration files will be stored.\n"
    print "   <channel> = default both electron and muon otherwise specify here either \"electrons\" or \"muons\""
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
py_cfg = "TuplesFromPAT.py" # cfg config file
crab_cfg = "crab.py" # crab config file

# Dictionaries
directoriesMuData = {}
directoriesElData = {}
directoriesMuMC = {}
directoriesElMC = {}
dbsURL = {}
jsonFilesMu = {}
jsonFilesEl = {}

######## Muons
directoriesMuData['Run2012A-12Jul2012-v1'] = '/SingleMu/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/USER'  # 
directoriesMuData['Run2012A-recover-06Aug2012-v1'] = '/SingleMu/StoreResults-Run2012A-recover-06Aug2012-v1_TLBSM_53x_v2-e3fb55b810dc7a0811f4c66dfa2267c9/USER'
directoriesMuData['Run2012B-13Jul2012-v1'] = '/SingleMu/StoreResults-Run2012B-13Jul2012-v1_TLBSM_53x_v2-e3fb55b810dc7a0811f4c66dfa2267c9/USER'
directoriesMuData['Run2012C-24Aug2012-v1'] = '/SingleMu/StoreResults-Run2012C-24Aug2012-v1_TLBSM_53x_v2-e3fb55b810dc7a0811f4c66dfa2267c9/USER'
directoriesMuData['Run2012C-PromptReco-v2'] = '/SingleMu/StoreResults-Run2012C-PromptReco-v2_TLBSM_53x_v2-e3fb55b810dc7a0811f4c66dfa2267c9/USER'
directoriesMuData['Run2012C-PromptReco-v2-p2'] = '/SingleMu/StoreResults-Run2012C-PromptReco-v2_TLBSM_53x_v2-646f7563e9ae6f48814faa1c250f042a/USER'
#missing:
#directoriesMuData['Run2012C-PromptReco-v2-ext1'] = '/SingleMu/StoreResults-Run2012C-PromptReco-v2_TLBSM_53x_v2_extension_v1-e3fb55b810dc7a0811f4c66dfa2267c9/USER'
directoriesMuData['Run2012C-EcalRecover_11Dec2012-v1'] = '/SingleMu/jpilot-Run2012C-EcalRecover_11Dec2012-v1_TLBSM_53x_v2-e3fb55b810dc7a0811f4c66dfa2267c9/USER'
directoriesMuData['Run2012D-PromptReco-v1'] = '/SingleMu/StoreResults-Run2012D-PromptReco-v1_TLBSM_53x_v2-e3fb55b810dc7a0811f4c66dfa2267c9/USER'
directoriesMuData['Run2012D-PromptReco-v1-p2'] = '/SingleMu/StoreResults-Run2012D-PromptReco-v1_TLBSM_53x_v2_extension_v1-e3fb55b810dc7a0811f4c66dfa2267c9/USER'
directoriesMuData['Run2012D-PromptReco-v1-p3'] = '/SingleMu/StoreResults-Run2012D-PromptReco-v1_TLBSM_53x_v2_extension_v2-e3fb55b810dc7a0811f4c66dfa2267c9/USER'

jsonFilesMu['Run2012A-12Jul2012-v1']  = 'Cert_190456-203742_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
jsonFilesMu['Run2012A-recover-06Aug2012-v1'] = 'Cert_190456-203742_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
jsonFilesMu['Run2012B-13Jul2012-v1'] = 'Cert_190456-203742_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
jsonFilesMu['Run2012C-24Aug2012-v1'] = 'Cert_190456-203742_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
jsonFilesMu['Run2012C-PromptReco-v2'] = 'Cert_190456-203742_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
jsonFilesMu['Run2012C-PromptReco-v2-p2'] = 'Cert_190456-203742_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
jsonFilesMu['Run2012C-EcalRecover_11Dec2012-v1'] = 'Cert_190456-203742_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
jsonFilesMu['Run2012D-PromptReco-v1'] = 'Cert_190456-203742_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
jsonFilesMu['Run2012D-PromptReco-v1-p2'] = 'Cert_190456-203742_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
jsonFilesMu['Run2012D-PromptReco-v1-p3'] = 'Cert_190456-203742_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'

## Attention:
## if a dataset name has ":g" it means that for that sample the jobs will be sent to Nebraska
## using condor_g

directoriesMuMC['TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola'] = '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/meloam-Summer12_DR53X-PU_S10_START53_V7A-v1_TLBSM_53x_v2-c04f3b4fa74c8266c913b71e0c74901d/USER'

#for i in directoriesMuMC:
#dbsURL[i] = 'http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet'



#directoriesMuMC['TTbar_D6T_Mu'] = '/TTJets_TuneD6T_7TeV-madgraph-tauola/Fall10-START38_V12-v2/AODSIM'
#directoriesMuMC['HToWW_M300_Mu'] = '/WH_ZH_TTH_HToWW_M-300_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO:lite'

########## Electrons
directoriesElData['TrigA'] = '/EG/Run2010A-Nov4ReReco_v1/AOD'       #trig:Ele15_LW, Oct 1st json

jsonFilesEl['TrigA'] = 'Cert_136033_140040_EL.txt'

directoriesElMC['TTbar_D6T_El'] = '/TTJets_TuneD6T_7TeV-madgraph-tauola/Fall10-START38_V12-v2/AODSIM'



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


# scheduler = remoteGlidein

crabData = '''
[CMSSW]
datasetpath = %(DATASET)s
pset = %(CFG)s
total_number_of_events = -1
lumi_mask = %(JSON)s
total_number_of_lumis = -1
lumis_per_job = 40
pycfg_params = noprint
#the line above is needed for VarParsing

[USER]
return_data = 1
copy_data = 0
ui_working_dir = %(DIR)s
storage_element = cmseos.fnal.gov
storage_path = /srm/v2/server?SFN=/eos/uscms/store/user/yumiceva
user_remote_dir = ntuples_4tops_v1
publish_data = 0
publish_data_name = Summer13
dbs_url_for_publication = https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet
check_user_remote_dir = 0

[CRAB]
jobtype = cmssw
scheduler = condor
use_server = 0
'''

crabMC = '''
[CMSSW]
datasetpath = %(DATASET)s
pset = %(CFG)s
total_number_of_events = -1
events_per_job = 25000
pycfg_params = noprint
#the line above is needed for VarParsing
dbs_url = %(DBSURL)s

[USER]
return_data = 1
copy_data = 0
ui_working_dir = %(DIR)s
storage_element = cmseos.fnal.gov
storage_path = /srm/managerv2?SFN=/11/
user_remote_dir = ntuples_4tops_v1
publish_data = 0
publish_data_name = Summer13
dbs_url_for_publication = https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet
check_user_remote_dir = 0

[CRAB]
jobtype = cmssw
scheduler = condor
use_server = 0
'''

crabMCg = '''
[CMSSW]
datasetpath = %(DATASET)s
pset = %(CFG)s
total_number_of_events = -1
events_per_job = 25000
pycfg_params = noprint
#the line above is needed for VarParsing

[USER]
return_data = 1
copy_data = 0
ui_working_dir = %(DIR)s
storage_element = cmseos.fnal.gov
storage_path = /srm/managerv2?SFN=/11/
user_remote_dir = ntuples_4tops_v1
publish_data = 0
publish_data_name = Summer10_363_v2
dbs_url_for_publication = https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet
check_user_remote_dir = 0

[CRAB]
jobtype = cmssw
scheduler = condor_g
use_server = 0

[EDG]
se_white_list = unl

'''

crabMCglite = '''
[CMSSW]
datasetpath = %(DATASET)s
pset = %(CFG)s
total_number_of_events = -1
events_per_job = 25000

[USER]
return_data = 1
copy_data = 0
ui_working_dir = %(DIR)s
storage_element = cmssrm.fnal.gov
storage_path = /srm/managerv2?SFN=/11/
user_remote_dir = ntuples_4tops_v1
publish_data = 0
publish_data_name = Summer10_363_v2
dbs_url_for_publication = https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet
check_user_remote_dir = 0

[CRAB]
jobtype = cmssw
scheduler = glite
use_server = 0

[EDG]
rb                      = CERN
proxy_server            = myproxy.cern.ch
virtual_organization    = cms
retry_count             = 4
lcg_catalog_type        = lfc
lfc_host                = lfc-cms-test.cern.ch
lfc_home                = /grid/cms
se_black_list           = 

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
        adict['CFG'] = py_cfg
        
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
        docondorglite = False
        adict['DATASET'] = directoriesMuMC[dir]
        if directoriesMuMC[dir].find(":g")!=-1:
            docondorg = True
            adict['DATASET'] = directoriesMuMC[dir].strip(":g")
        if directoriesMuMC[dir].find(":lite")!=-1:
            docondorglite = True
            adict['DATASET'] = directoriesMuMC[dir].strip(":lite")
                                    
        adict['DIR'] = dir
        adict['CFG'] = py_cfg
        adict['DBSURL'] = dbsURL[dir]
        
        acrab = crabMC%adict
        if docondorg:
            acrab = crabMCg%adict
        if docondorglite:
            acrab = crabMCglite%adict
            
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
        adict['CFG'] = py_cfg
        
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
        adict['CFG'] = py_cfg
        
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


                                                
