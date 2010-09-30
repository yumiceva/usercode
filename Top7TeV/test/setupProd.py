#! /usr/bin/env python

import sys
import os
import fileinput

if len(sys.argv) < 2:
    print "\n usage: setupProd.py <path> \n"
    print "   <path> = path to directory where the crab configuration files will be stored.\n"
    sys.exit()

# configutation
path = sys.argv[1]
current = os.getcwd() # current path
py_cfg = "PATskim.py" # cfg config file
crab_cfg = "crab.py" # crab config file


directoriesMuData = ['Jun14MB_Mu','Jun14Mu_Mu','Jul16_Mu','Prompt_Mu']
datasetsMuData = ['/MinimumBias/Commissioning10-SD_Mu-Jun14thSkim_v1/RECO',\
                  '/Mu/Run2010A-Jun14thReReco_v1/RECO',\
                  '/Mu/Run2010A-Jul16thReReco-v1/RECO',\
                  '/Mu/Run2010A-PromptReco-v4/RECO']
directoriesMuMC = ['TTbar_Mu','Wjets_Mu','Zjets_Mu','STtch_Mu','STtWch','QCD_Mu','Wc_Mu','Vqq_Mu']
datasetsMuMC = ['/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO',\
                '/WJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO',\
                '/ZJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO',\
                '/SingleTop_tChannel-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO',\
                '/SingleTop_tWChannel-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO',\
                '/InclusiveMu15/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO',\
                '/WCJets_7TeV-madgraph/Spring10-START3X_V26-v1/GEN-SIM-RECO',\
                '/VqqJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO']
directoriesElData = ['Jun14MB_El','Jun14EG_El','Jul16_El','Prompt_El']
datasetsElData = ['/MinimumBias/Commissioning10-SD_EG-Jun14thSkim_v1/RECO',\
                  '/EG/Run2010A-Jun14thReReco_v1/RECO',\
                  '/EG/Run2010A-Jul16thReReco-v2/RECO',\
                  '/EG/Run2010A-PromptReco-v4/RECO']
directoriesElMC =['TTbar_El','Wjets_El','Zjets_El','STtch_El','QCD_El','Wc_El','Vqq_El']
datasetsElMC = ['/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO',\
                '/WJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO',\
                '/ZJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO',\
                '/SingleTop_tChannel-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO',\
                '/SingleTop_tWChannel-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO',\
                '/QCD_EMEnriched_Pt80to170/Spring10-START3X_V26_S09-v3/GEN-SIM-RECO',\
                '/WCJets_7TeV-madgraph/Spring10-START3X_V26-v1/GEN-SIM-RECO',\
                '/VqqJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO']

jsonFiles = ['Cert_132440-137028_7TeV_June14thReReco_Collisions10_JSON_v2_TopFiltered_135059-137028.txt',\
             'Cert_132440-137028_7TeV_June14thReReco_Collisions10_JSON_v2_TopFiltered_135059-137028.txt',\
             'Cert_139779-1340159_7TeV_July16thReReco_Collisions10_JSON.txt',\
             'Cert_132440-144114_7TeV_StreamExpress_Collisions10_JSON_v2_TopSep11_Filtered_138564-144114.txt']

print "==> make directories"
alldirs = directoriesMuData + directoriesMuMC + directoriesElData + directoriesElMC

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

print "===> for muon channel"
for dir in directoriesMuData:

    afile = path+"/"+dir+"/"+py_cfg
    os.system("cp "+py_cfg+" "+afile)

    for line in fileinput.FileInput(afile, inplace=1):
        line = line.replace("inputType = \"MC\"","inputType = \"DATA\"")
        line = line.replace("channel = \"electron\"","channel = \"muon\"")
        line = line.replace("eventtype=\"TTJets\"","eventtype=\""+dir+"\"")
        print line,
    
for dir in directoriesMuMC:

    afile = path+"/"+dir+"/"+py_cfg
    os.system("cp "+py_cfg+" "+afile)

    type = dir.strip("_Mu")
    
    for line in fileinput.FileInput(afile, inplace=1):
        line = line.replace("inputType = \"DATA\"","inputType = \"MC\"")
        line = line.replace("channel = \"electron\"","channel = \"muon\"")
        line = line.replace("eventtype=\"TTJets\"","eventtype=\""+type+"\"")
        print line,
print "===> for electron channel"
for dir in directoriesElData:

    afile = path+"/"+dir+"/"+py_cfg
    os.system("cp "+py_cfg+" "+afile)

    for line in fileinput.FileInput(afile, inplace=1):
        line = line.replace("inputType = \"MC\"","inputType = \"DATA\"")
        line = line.replace("channel = \"muon\"","channel = \"electron\"")
        line =line.replace("eventtype=\"TTJets\"","eventtype=\""+dir+"\"")
        print line,
        
for dir in directoriesElMC:

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

idir = 0
for dir in directoriesMuData:

    ajson = jsonFiles[idir]
    
    os.system("cp json/"+ajson+" "+path+"/"+dir)

    adict = {}
    adict['DATASET'] = datasetsMuData[idir]
    adict['JSON'] = ajson
    adict['DIR'] = dir
    
    acrab = crabData%adict

    fileout = open(path+"/"+dir+"/crab.cfg","w")
    fileout.writelines(acrab)
    fileout.close()
    idir += 1

    fcrab_data.write("cd "+path+"/"+dir+"\n")
    fcrab_data.write("crab -create -cfg crab.cfg"+"\n")
    if dir.find("Prompt")!= -1:
        fcrab_data.write("crab -submit 500 -c "+dir+"\n")
        fcrab_data.write("crab -submit 500 -c "+dir+"\n")
        fcrab_data.write("crab -submit 500 -c "+dir+"\n")
    else:
        fcrab_data.write("crab -submit all -c "+dir+"\n")
    
idir = 0
for dir in directoriesMuMC:

    adict = {}
    adict['DATASET'] = datasetsMuMC[idir]
    adict['DIR'] = dir
    
    acrab = crabMC%adict
    
    fileout = open(path+"/"+dir+"/crab.cfg","w")
    fileout.writelines(acrab)
    fileout.close()
    idir += 1

    fcrab_mc.write("cd "+path+"/"+dir+"\n")
    fcrab_mc.write("crab -create -cfg crab.cfg -submit all"+"\n")
    
idir = 0
for dir in directoriesElData:

    ajson = jsonFiles[idir]

    os.system("cp json/"+ajson+" "+path+"/"+dir+"\n")

    adict = {}
    adict['DATASET'] = datasetsMuData[idir]
    adict['JSON'] = ajson
    adict['DIR'] = dir

    acrab = crabData%adict

    fileout = open(path+"/"+dir+"/crab.cfg","w")
    fileout.writelines(acrab)
    fileout.close()
    idir += 1

    fcrab_data.write("cd "+path+"/"+dir+"\n")
    fcrab_data.write("crab -create -cfg crab.cfg"+"\n")
    if dir.find("Prompt")!= -1:
        fcrab_data.write("crab -submit 500 -c "+dir+"\n")
        fcrab_data.write("crab -submit 500 -c "+dir+"\n")
        fcrab_data.write("crab -submit 500 -c "+dir+"\n")
    else:
        fcrab_data.write("crab -submit all -c "+dir+"\n")
                                            
idir = 0
for dir in directoriesElMC:

    adict = {}
    adict['DATASET'] = datasetsMuMC[idir]
    adict['DIR'] = dir

    acrab = crabMC%adict

    fileout = open(path+"/"+dir+"/crab.cfg","w")
    fileout.writelines(acrab)
    fileout.close()
    idir += 1

    fcrab_mc.write("cd "+path+"/"+dir+"\n")
    fcrab_mc.write("crab -create -cfg crab.cfg -submit all"+"\n")


fcrab_data.close()
fcrab_mc.close()

print "done."


                                                
