#! /usr/bin/env python

import sys
import os
import commands

#________________________________________________________________

if len(sys.argv) < 2:
    print " usage: statusProd.py path-directory <getoutput/report/lumi>"
    sys.exit()


path = sys.argv[1]
current = os.getcwd()
cmdcrab = "-status -c "
doLumi = False
if len(sys.argv)==3:
    cmdcrab = "-"+sys.argv[2]+" all -c "
    if sys.argv[2]=="lumi":
        doLumi = True

dirs = os.listdir(path)

if doLumi:

    cmdlumi = "lumiCalc.py -c frontier://LumiProd/CMS_LUMI_PROD --nowarning overview -i "
    totallumi = 0
    
    for adir in dirs:
        print "calculating luminosity for "+adir
        thejson = path+"/"+adir+"/"+adir+"/res/lumiSummary.json"
        output = commands.getstatusoutput(cmdlumi+thejson)
        lines = output[1].split('\n')
        theline = lines[len(lines)-2]
        tmplist = theline.split()
        alumi = tmplist[len(tmplist)-2]
        alumi = float(alumi)/1.e6
        print "==> recorded in pb-1 "+str(round(alumi,2))
        totallumi += alumi

    print "==> total recorded luminosity in pb-1 "+str(round(totallumi,2))
    sys.exit()

    
for adir in dirs:

    cmd = "cd "+path+"/"+adir
    os.chdir(path+"/"+adir)
    print cmd
    if os.path.isdir(adir):
        cmd = "crab "+cmdcrab+adir
        print cmd
        output = commands.getstatusoutput(cmd)
        lines = output[1].split('\n')
        #print lines
        if len(sys.argv)==3:
            cmd = "hadd "+adir+".root "+adir+"/res/*.root"
            print cmd
            output = commands.getstatusoutput(cmd)
            
        totaljobs = 0
        donejobs = 0
        for line in lines:
            if line.find("Total Jobs")!=-1:
                totaljobs = line.split()[1]
            if line.find("Jobs Done")!=-1:
                donejobs = line.split()[1]
        if len(sys.argv)<3:
            print " Total Jobs "+str(totaljobs)
            print " Jobs Done  "+str(donejobs)
        
    else:
        print "no status because crab was not run in directory "+adir
    os.chdir(current)
    
os.chdir(current)
