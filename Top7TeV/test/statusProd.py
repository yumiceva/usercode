#! /usr/bin/env python

import sys
import os
import commands

#________________________________________________________________

if len(sys.argv) < 2:
    print " usage: statusProd.py path-directory <getoutput>"
    sys.exit()


path = sys.argv[1]
current = os.getcwd()
cmdcrab = "-status -c "
if len(sys.argv)==3:
    cmdcrab = "-getoutput all -c "

dirs = os.listdir(path)

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
