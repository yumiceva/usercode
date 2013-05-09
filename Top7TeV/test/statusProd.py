#! /usr/bin/env python

import sys
import os
import commands
import subprocess

# List of colors.  Replace digit [0; with [1; to get light color version.
black  = "\033[0;30m"
yellow   = "\033[1;33m"
blue  = "\033[0;34m"
green  = "\033[0;32m"
cyan  = "\033[0;36m"
red  = "\033[0;31m"
purple  = "\033[0;35m"
brown  = "\033[0;33m"
blue  = "\033[0;34m"
green  = "\033[1;32m"
cyan  = "\033[0;36m"
red  = "\033[1;31m"
purple  = "\033[0;35m"
reset    = "\033[0m"


#________________________________________________________________
def get_list_files(directory):

    dir = []
    
    dir = os.listdir(directory)
    
    lfiles = []
    for f in dir:

        if f.endswith(".root"):
            
            lfiles.append(directory+"/"+f)
                
    return lfiles

                                    
#________________________________________________________________
def my_getLumi(adir):

    cmdlumi = "pixelLumiCalc.py overview -i "
    totallumi = 0

    if not os.path.isdir(path+"/"+adir): return totallumi
    print "calculating luminosity for "+adir
    thejson = path+"/"+adir+"/"+adir+"/res/lumiSummary.json"
    if not os.path.isfile(thejson):
        print " no lumiSummary.json file for this folder. skip dataset."
        return totallumi
    output = commands.getstatusoutput(cmdlumi+thejson)
    lines = output[1].split('\n')
    theline = lines[len(lines)-2]
    tmplist = theline.split()
    alumi = tmplist[len(tmplist)-2]
    alumi = float(alumi) #/1.e6
    print "==> recorded in pb-1 "+str(round(alumi,2))
    totallumi = alumi
    return totallumi
#________________________________________________________________


if len(sys.argv) < 2:
    print " usage: statusProd.py path-directory <status/getoutput/report/lumi>"
    sys.exit()

if subprocess.call(['which','crab']):
    print "Executable crab was not found in path"
    sys.exit()
    
path = sys.argv[1]
current = os.getcwd()
cmdcrab = "-status -c "
doLumi = False
if len(sys.argv)>=3:
    cmdcrab = "-"+sys.argv[2]+" all -c "
    if sys.argv[2]=="lumi":
        doLumi = True

dirs = os.listdir(path)

# only look at input directory
if len(sys.argv)==4:
    if sys.argv[3].find(",")!=-1:
        dirs = sys.argv[3].split(',')
    else:
        dirs = [sys.argv[3]]

if doLumi:

    cmdlumi = "pixelLumiCalc.py overview -i "
    totallumi = 0
    
    for adir in dirs:
        if not os.path.isdir(path+"/"+adir): continue
        print "calculating luminosity for "+adir
        thejson = path+"/"+adir+"/"+adir+"/res/lumiSummary.json"
        if not os.path.isfile(thejson):
            print " no lumiSummary.json file for this folder. skip dataset."
            continue
        output = commands.getstatusoutput(cmdlumi+thejson)
        lines = output[1].split('\n')
        theline = lines[len(lines)-2]
        tmplist = theline.split()
        alumi = tmplist[len(tmplist)-2]
        alumi = float(alumi) #/1.e6
        print "==> recorded in pb-1 "+str(round(alumi,2))
        totallumi += alumi

    print "==> total recorded luminosity in pb-1 "+str(round(totallumi,2))
    sys.exit()

    
for adir in dirs:

    if os.path.isdir(path+"/"+adir):
        cmd = "cd "+path+"/"+adir
        os.chdir(path+"/"+adir)
        print cmd

        if sys.argv[2]!="hadd":
            cmd = "crab "+cmdcrab+adir
            print cmd
            output = commands.getstatusoutput(cmd)
            lines = output[1].split('\n')

            totaljobs = 0
            donejobs = 0
            retrievedjobs = 0
            createdjobs = 0
            for line in lines:
                if line.find("Total Jobs")!=-1:
                    totaljobs = line.split()[1]
                if line.find("Jobs Done")!=-1:
                    donejobs = line.split()[1]
                if line.find("Jobs with Wrapper Exit Code : 0")!=-1:
                    retrievedjobs = line.split()[1]
                if line.find("Jobs Created")!=-1:
                    createdjobs = line.split()[1]
                    
            if sys.argv[2]=="status":
                print " Total Jobs "+str(totaljobs)
                if createdjobs != 0:
                    print " Jobs Created  "+str(createdjobs)
                if donejobs != 0:
                    print " Jobs Done  "+str(donejobs)
                if retrievedjobs != 0:
                    print " Retrieved Jobs "+str(retrievedjobs)
                if totaljobs == retrievedjobs:
                    print green+"===> DONE"+reset
                if donejobs>0:
                    print yellow+"===> need to getoutput"+reset
                if totaljobs > donejobs and totaljobs > retrievedjobs:
                    print purple+"===> incomplete"+reset
                if totaljobs == 0 or createdjobs > 0:
                    print red+"===> Zero jobs! need to submit them?"+reset
        #if len(sys.argv)>2 and (sys.argv[2]=="getoutput" or sys.argv[2]=="hadd"):
        if len(sys.argv)>2 and sys.argv[2]=="hadd":
            if not os.path.isdir(adir+"/res/"):
                print " no output files skip this dataset."
                os.chdir(current)
                continue
            if os.path.isfile(adir+".root"):
                print " merged root file already exits. we will not merge again. The file is:"
                print green+path+"/"+adir+"/"+adir+".root"+reset
                os.chdir(current)
                continue
            listofroot = get_list_files(adir+"/res/")
            nfiles = 0
            tmplistoffiles = []
            atmplist = []
            maxfiles = 500
            if len(listofroot) > maxfiles:
                print " total number of root files = "+str(len(listofroot))
                                                           
                for iroot in listofroot:
                    if iroot.find(".root")==-1: continue
                    if nfiles <= maxfiles:
                        atmplist.append(iroot)
                    if nfiles == maxfiles:
                        tmplistoffiles.append(atmplist)
                        print " block of files to add = "+str(len(tmplistoffiles)) +" with "+str(len(atmplist))+" files"
                        nfiles = 0
                        atmplist = []                        
                    else: nfiles += 1
                if nfiles!= 0:
                    tmplistoffiles.append(atmplist)
                    print " block of files to add = "+str(len(tmplistoffiles)) +" with "+str(len(atmplist))+" files"
                    nfiles = 0
                                            
                istrfile = 1

                for ii in tmplistoffiles:
                    linefiles = ""
                    for jj in ii:
                        linefiles = linefiles +" "+jj
                    haddedfile = adir+"_"+str(istrfile)+".root"
                    cmd = "hadd "+haddedfile + linefiles
                    #print cmd
                    print "hadd block "+str(istrfile)
                    output = commands.getstatusoutput(cmd)
                    print output[0]
                    #print output[1]
                    istrfile += 1
                linefiles = ""
                for kk in range(1,istrfile):
                    linefiles = linefiles + " " +adir+"_"+str(kk)+".root"
                cmd = "hadd "+adir+".root "+linefiles
                print cmd
                print green+path+"/"+adir+"/"+adir+".root"+reset
                output = commands.getstatusoutput(cmd)
                print output[1]
                for kk in range(1,istrfile):
                    cmd = "rm "+adir+"_"+str(kk)+".root"
                    print cmd
                    output = commands.getstatusoutput(cmd)
            else:    
                cmd = "hadd "+adir+".root "+adir+"/res/*.root"
                print cmd
                output = commands.getstatusoutput(cmd)
                print output[0]
                
    else:
        print "no status because crab was not run in directory "+adir
    os.chdir(current)
    
os.chdir(current)
