#! /usr/bin/env python

import sys
import os
import commands

#________________________________________________________________


path = sys.argv[1]
current = os.getcwd()

dirs = os.listdir(path)

for adir in dirs:

    cmd = "cd "+path+"/"+adir
    os.chdir(path+"/"+adir)
    print cmd
    if os.path.isdir(adir):
        cmd = "crab -status -c "+adir
        output = commands.getstatusoutput(cmd)
        print cmd
        lines = output[1].split('\n')
        #print lines
        totaljobs = 0
        donejobs = 0
        for line in lines:
            if line.find("Total Jobs")!=-1:
                totaljobs = line.split()[1]
            if line.find("Jobs Done")!=-1:
                donejobs = line.split()[1]
        print " Total Jobs "+str(totaljobs)
        print " Jobs Done  "+str(donejobs)
    else:
        print "no status because crab was not run in directory "+adir
    os.chdir(current)
    
os.chdir(current)
