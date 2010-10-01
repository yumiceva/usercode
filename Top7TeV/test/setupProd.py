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

directoriesMuData['Jun14MB_Mu'] = '/MinimumBias/Commissioning10-SD_Mu-Jun14thSkim_v1/RECO' #'/MinimumBias/Run2010A-Sep17ReReco_v2/RECO'
directoriesMuData['Jun14Mu_Mu'] = '/Mu/Run2010A-Jun14thReReco_v1/RECO'
directoriesMuData['Jul16_Mu'] = '/Mu/Run2010A-Jul16thReReco-v1/RECO'
directoriesMuData['Prompt_MuA'] = '/Mu/Run2010A-PromptReco-v4/RECO' # 137437-144114
directoriesMuData['Prompt_MuB'] = '/Mu/Run2010B-PromptReco-v2/RECO' # 146240-146946...

directoriesMuMC['TTbar_Mu'] = '/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesMuMC['Wjets_Mu'] = '/WJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesMuMC['Zjets_Mu'] = '/ZJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesMuMC['STtch_Mu'] = '/SingleTop_tChannel-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesMuMC['STtWch_Mu'] = '/SingleTop_tWChannel-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesMuMC['QCD_Mu'] = '/InclusiveMu15/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesMuMC['Wc_Mu'] = '/WCJets_7TeV-madgraph/Spring10-START3X_V26-v1/GEN-SIM-RECO'
directoriesMuMC['Vqq_Mu'] = '/VqqJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'

directoriesElData['Jun14MB_El']= '/MinimumBias/Commissioning10-SD_EG-Jun14thSkim_v1/RECO'
directoriesElData['Jun14EG_El']= '/EG/Run2010A-Jun14thReReco_v1/RECO'
directoriesElData['Jul16_El'] ='/EG/Run2010A-Jul16thReReco-v1/RECO'
directoriesElData['Prompt_ElA'] = '/EG/Run2010A-PromptReco-v4/RECO'
directoriesElData['Prompt_ElB'] = '/EG/Run2010A-PromptReco-v4/RECO'

directoriesElMC['TTbar_El'] = '/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['Wjets_El'] = '/WJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['Zjets_El'] = '/ZJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['STtch_El'] = '/SingleTop_tChannel-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['STtWch_El']= '/SingleTop_tWChannel-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'
directoriesElMC['QCD_El'] = '/QCD_EMEnriched_Pt80to170/Spring10-START3X_V26_S09-v3/GEN-SIM-RECO'
directoriesElMC['Wc_El'] = '/WCJets_7TeV-madgraph/Spring10-START3X_V26-v1/GEN-SIM-RECO'
directoriesElMC['Vqq_El'] = '/VqqJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO'

jsonFilesMu = {}
jsonFilesEl = {}
jsonFilesMu['Jun14MB_Mu'] = jsonFilesEl['Jun14MB_El'] = 'Cert_132440-137028_7TeV_June14thReReco_Collisions10_JSON_v2_TopFiltered_135059-137028.txt'
jsonFilesMu['Jun14Mu_Mu'] = jsonFilesEl['Jun14EG_El'] = 'Cert_132440-137028_7TeV_June14thReReco_Collisions10_JSON_v2_TopFiltered_135059-137028.txt'
jsonFilesMu['Jul16_Mu'] = jsonFilesEl['Jul16_El'] = 'Cert_139779-1340159_7TeV_July16thReReco_Collisions10_JSON.txt'
jsonFilesMu['Prompt_MuA'] = jsonFilesEl['Prompt_ElA'] = 'Cert_132440-146729_7TeV_StreamExpress_Collisions10_JSON.txt'
jsonFilesMu['Prompt_MuB'] = jsonFilesEl['Prompt_ElB'] = 'Cert_132440-146729_7TeV_StreamExpress_Collisions10_JSON.txt'

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

print "==> soft link \"production\" created in current directory to production directory"
if not os.path.islink("production"):
    os.system("ln -s "+path+" production")
else:
    print "link \"production\" already exists."
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

fcrab_data = open("launch_crab_data.csh","w")
fcrab_mc = open("launch_crab_mc.csh","w")

if DoMuons:
    
    for dir in directoriesMuData.keys():

        ajson = jsonFilesMu[dir]
    
        os.system("cp json/"+ajson+" "+path+"/"+dir)
        
        adict = {}
        adict['DATASET'] = datasetsMuData[dir]
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
    

    for dir in directoriesMuMC.keys():

        adict = {}
        adict['DATASET'] = datasetsMuMC[dir]
        adict['DIR'] = dir
    
        acrab = crabMC%adict
    
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
        adict['DATASET'] = datasetsMuData[dir]
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
        adict['DATASET'] = datasetsMuMC[dir]
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


                                                
