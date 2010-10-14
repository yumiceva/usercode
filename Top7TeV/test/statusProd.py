#! /usr/bin/env python

import sys
import os
import commands


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
        if not os.path.isdir(path+"/"+adir): continue
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
    
    if os.path.isdir(path+"/"+adir):
        cmd = "cd "+path+"/"+adir
        os.chdir(path+"/"+adir)
        print cmd
                
        cmd = "crab "+cmdcrab+adir
        print cmd
        output = commands.getstatusoutput(cmd)
        lines = output[1].split('\n')
        #print lines
        if len(sys.argv)>2 and sys.argv[2]=="getoutput":

            listofroot = get_list_files(adir+"/res/")
            nfiles = 0
            tmplistoffiles = []
            atmplist = []
            maxfiles = 500
            if len(listofroot) > maxfiles:
                for iroot in listofroot:
                    if iroot.find(".root")==-1: continue
                    if nfiles <= maxfiles:
                        atmplist.append(iroot)
                    if nfiles == maxfiles:
                        tmplistoffiles.append(atmplist)
                        nfiles = 0
                        atmplist = []
                    else: nfiles += 1
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
                    print output[1]
                    istrfile += 1
                linefiles = ""
                for kk in range(1,istrfile):
                    linefiles = linefiles + " " +adir+"_"+str(kk)+".root"
                cmd = "hadd "+adir+".root "+linefiles
                print cmd
                output = commands.getstatusoutput(cmd)
                print output[1]
                for kk in range(1,istrfile+1):
                    cmd = "rm "+adir+"_"+str(kk)+".root"
                    print cmd
                    output = commands.getstatusoutput(cmd)
            else:    
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
