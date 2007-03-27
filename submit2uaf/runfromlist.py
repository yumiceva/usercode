#!/usr/bin/env python2
#################################################

import os, sys


def main():

    pwddir = os.getcwd()
    cfgfile = sys.argv[1]
    filelist = open(sys.argv[2])
    #dir = sys.argv[4]
    dir = ""
    if len(sys.argv)>=5:
        dir = sys.argv[4]
        dir = pwddir+"/"+dir
    totaljobs = sys.argv[3]
    outdir = sys.argv[4]
    
    ijob = 0
    total = int(totaljobs)
    for iline in filelist:

        if ijob==total:
            break
        iline = iline.strip("\n")
        command = "./submit_jobs_new.py "+cfgfile+" "+iline+" "

        tmparr = iline.split("/")
        tmpfile = tmparr[len(tmparr)-1]

        command += tmpfile + " " + dir
        print command+"\n"
        os.system(command)
        
        ijob += 1

if __name__ =='__main__':
        sys.exit(main())
