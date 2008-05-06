#!/usr/bin/env python
############################################################################
#
# Author: Francisco Yumiceva
# yumiceva@fnal.gov
#
# Fermilab, 2007
############################################################################

"""
   Submit jobs to condor. It will create directories in the output path
   to store configuration files and log files.
   
   e.g. submitToCondor.py -c ZprimeEventsReco_full.cfg -l datasets/zp1Tev.txt -n 10 -o /uscms_data/d1/yumiceva/CMSSW_1_3_6/TQAFAnalyzer/

   where ZprimeEventsReco_full.cfg is the configuration file. The configuration file
   needs to be modified a bit: (1) remove all input files in the PoolSource and add a
   keyword {FILENAME} you need also the brackets. The filename of the output file should
   be changed to another keyword called {OUTPUT_FILENAME}.
   
   To Generate MC: In your cfg file you need to add also the keywords {SEED1},...,{SEED6} for
   the random seeds which are going to be modified by the script using PYTHON random numbers,
   you also need to change the output filename to {OUTPUT_FILENAME} All keywords need to have
   brackets.
   
   usage: %prog
   -m, --mc   : to generate MC. it will handle random numbers.
   -e, --events = EVENTS: number of events to generate MC. If -m flag is turned on.
   -c, --cfg    = CFG: configuration file
   -l, --list   = LIST: file with list of files (dataset)
   -n, --njobs  = NJOBS: number of jobs.
   -o, --output = OUTPUT: output path
   -i, --initial= INITIAL:  number of initial job
   -f, --final= FINAL: number of final job
   -s, --short : for short jobs that need high priority
   -t, --test : do not submit anything just show what I would do

   Author: Francisco Yumiceva (yumiceva@fnal.gov)
"""


import os,sys
try:
    import random
except:
    # now fix PYTHONPATH for the damn cmslpc python configuration
    atemp = os.getenv("PYTHONPATH")
    atempl = atemp.split(":")
    natemp = ""
    for iatemp in atempl:
        if iatemp.find("python2.3") == -1:
            natemp = natemp + iatemp
    print "\n FIX PYTHONPATH environment variable, try:\n"
    print "setenv PYTHONPATH "+ natemp
    sys.exit()
    #os.putenv("PYTHONPATH",natemp)
    # PYTHONPATH fixed

import string, re
from time import gmtime, localtime, strftime

#_______________OPTIONS__________________________________
import optparse
USAGE = re.compile(r'(?s)\s*usage: (.*?)(\n[ \t]*\n|$)')
def nonzero(self): # will become the nonzero method of optparse.Values
    "True if options were given"
    for v in self.__dict__.itervalues():
        if v is not None: return True
    return False

optparse.Values.__nonzero__ = nonzero # dynamically fix optparse.Values

class ParsingError(Exception): pass

optionstring=""

def exit(msg=""):
    raise SystemExit(msg or optionstring.replace("%prog",sys.argv[0]))

def parse(docstring, arglist=None):
    global optionstring
    optionstring = docstring
    match = USAGE.search(optionstring)
    if not match: raise ParsingError("Cannot find the option string")
    optlines = match.group(1).splitlines()
    try:
        p = optparse.OptionParser(optlines[0])
        for line in optlines[1:]:
            opt, help=line.split(':')[:2]
            short,long=opt.split(',')[:2]
            if '=' in opt:
                action='store'
                long=long.split('=')[0]
            else:
                action='store_true'
            p.add_option(short.strip(),long.strip(),
                         action = action, help = help.strip())
    except (IndexError,ValueError):
        raise ParsingError("Cannot parse the option string correctly")
    return p.parse_args(arglist)

#______________________________________________________________________

#
# path to this scripts and the used template file:
#
TemplatesPATH =  os.environ['CMSSW_BASE']+"/src/submit2uaf/"
#
# path to the output that is created by this script
#
#main_path   = "/uscmst1b_scratch/lpc1/cmsroc/yumiceva/CMSSW_1_2_0_pre3/src/RecoVertex/BeamSpotProducer/test/"
main_path = os.getcwd() + "/"

out_path      = main_path+"condorjobs/"

scripts_path  = main_path+"condorjobs/"
cfg_path      = scripts_path+"cfg/"
#condor_path   = scripts_path+"condor/"
csh_path      = scripts_path+"csh/"
logs_path     = scripts_path+"logs/"


istest = 0
isMC   = 0
Nevents = "1"
isshort = 0

#
# Path to the input/output data:
#

cfg_tags        = ["{FILENAME}",
                   "{OUTPUT_FILENAME}"]

cfg_tags_mc     = ["{SEED1}",
                   "{SEED2}",
                   "{SEED3}",
                   "{SEED4}",
                   "{SEED5}",
                   "{SEED6}",
                   "{OUTPUT_FILENAME}"]

scripts_tags    = ["{PATHTOOUT}",
                   "{CONFIGFILE}",
                   "{OUTFILE}","{PATHTOBASE}","{OUTDIR}"]

condor_tags     = ["{EXECUTABLE}",
                   "{OUTPUT}",
                   "{ERROR}",
                   "{LOG}",
                   "{NOTIFY}"]


template_fnames = {}
template_fnames["cfg"]          =  TemplatesPATH+"/reco_template.cfg"
template_fnames["condor"]       =  TemplatesPATH+"/template.condor"
template_fnames["csh"]          =  TemplatesPATH+"/runcms_new2.csh"


def _mkdir(newdir):
    """works the way a good mkdir should :)
        - already exists, silently complete
        - regular file in the way, raise an exception
        - parent directory(ies) does not exist, make them as well
    """
    if os.path.isdir(newdir):
        pass
    elif os.path.isfile(newdir):
        raise OSError("a file with the same name as the desired " \
                      "dir, '%s', already exists." % newdir)
    else:
        head, tail = os.path.split(newdir)
        if head and not os.path.isdir(head):
            _mkdir(head)
        if tail:
            os.mkdir(newdir)

def change(infile,outfile,changearray,filearray):
    fin  = open(infile)
    fout = open(outfile,"w")
    for line in fin.readlines():
        # make sure we run over all events in each job
        if line.find("untracked")!=-1 and line.find("PSet")!=-1 and line.find("maxEvents")!=-1:
            line = "untracked PSet maxEvents = {untracked int32 input = "+str(Nevents)+"}\n"
                    
        for change in changearray:
            if change[0] == "{FILENAME}" and line.find(change[0])!=-1:
                line=line.replace(change[0] ,"")
                itmp=0
                for ifile in filearray:
                    ifile = ifile.strip("\n")
                    itmp = itmp + 1
                    suffix = "\","
                    if itmp == len(filearray):
                        suffix = "\""
                    fout.write("\""+ifile+suffix+"\n")
                    
            else:
                
                line=line.replace(change[0] ,change[1])
        
        fout.write(line)


def submit_jobs(njob,array,ini_cfgfile,output_path):
    
    cfgfile           = ini_cfgfile.replace("_template","")

    njobstr = str(njob)
    while len(njobstr)<4:
        njobstr = "0"+njobstr
    
    outfilename_cfg   = cfg_path  + cfgfile.replace(".cfg","_"+njobstr+".cfg")
    outfilename_root = output_path+ cfgfile.replace(".cfg","_"+njobstr+".root")
    outfilename_log   = logs_path + cfgfile.replace(".cfg","_"+njobstr+".log")
    outfilename_csh   = csh_path  + cfgfile.replace(".cfg","_"+njobstr+".csh")
    outfilename_condor= csh_path  + cfgfile.replace(".cfg","_"+njobstr+".condor")
    
    if  os.path.isfile(outfilename_root):
        print "Not submitting into condor batch farm since rootoutput already exists"
        print "delete or rename: "+outfilename_root+"\n to resubmit"
        sys.exit(main())
            
    #
    # First create config (.cfg) file:
    #
        
    changearray=[]

    if isMC:
        random.seed(njob)
        changearray.append((cfg_tags_mc[0],str(random.randint(1,987654321) ) ))
        changearray.append((cfg_tags_mc[1],str(random.randint(1,987654321) ) ))
        changearray.append((cfg_tags_mc[2],str(random.randint(1,987654321) ) ))
        changearray.append((cfg_tags_mc[3],str(random.randint(1,987654321) ) ))
        changearray.append((cfg_tags_mc[4],str(random.randint(1,987654321) ) ))
        changearray.append((cfg_tags_mc[5],str(random.randint(1,987654321) ) ))
        changearray.append((cfg_tags_mc[6],outfilename_root))
        change(template_fnames["cfg"],outfilename_cfg,changearray,array)
        print outfilename_cfg + " has been written.\n"

    else:
        changearray.append((cfg_tags[0],""))
        changearray.append((cfg_tags[1],outfilename_root))
        change(template_fnames["cfg"],outfilename_cfg,changearray,array)
        print outfilename_cfg + " has been written.\n"
    
    
    #
    # now create the script to process the file:
    #
    changearray=[]
    changearray.append((scripts_tags[0],output_path+"/"))
    changearray.append((scripts_tags[1],outfilename_cfg))
    changearray.append((scripts_tags[2],outfilename_root))
    changearray.append((scripts_tags[3],os.getenv("CMSSW_BASE")+"/src" ))
    changearray.append((scripts_tags[4],output_path))
    change(template_fnames["csh"],outfilename_csh,changearray,0)
    os.chmod(outfilename_csh,0775)
    print outfilename_csh + " has been written.\n"
    
    #
    # finally create the condor job description file:
    #
    stdout         = outfilename_log.replace(".log",".stdout")
    stderr         = outfilename_log.replace(".log",".stderr")
    logfiles       = outfilename_log.replace(".log",".log")
    changearray    =[]
    changearray.append((condor_tags[0],outfilename_csh))
    changearray.append((condor_tags[1],stdout))
    changearray.append((condor_tags[2],stderr))
    changearray.append((condor_tags[3],logfiles))
    changearray.append((condor_tags[4],os.getenv("USER")+"@fnal.gov"))
    change(template_fnames["condor"],outfilename_condor,changearray,0)
    if isshort:
        fout = open(outfilename_condor,"a")
        fout.write("+LENGTH=\"SHORT\"\n")
        fout.close()
    
    print outfilename_condor + " has been written.\n"

    submitcommand ="/opt/condor/bin/condor_submit  "+outfilename_condor
    print submitcommand+"\n"
    
    if not istest:
        child   = os.popen(submitcommand)
    else:
        print "Not submitting into condor batch farm. This is only a test"
        


#________________________________________________________________    
if __name__ =='__main__':


    #import optionparse
    option,args = parse(__doc__)
    if not args and not option: exit()

    if not option.mc and (not option.cfg or not option.njobs or not option.list):
        print " you need to provide configuration file, list of files, number of jobs"
        optionparse.exit()
        
    ini_cfgfile = option.cfg
    
    template_fnames["cfg"]          =  ini_cfgfile

    if not option.mc:
        list_of_files = option.list
    else:
        Nevents = str(option.events)
                      
    number_of_jobs = option.njobs
    
    ini_run = 0
    fin_run = 0

    if option.initial:
        ini_run = int(option.initial)
    if option.final:
        fin_run = int(option.final)
    
    istest = option.test
    isMC   = option.mc
    isshort = option.short
    
    output_path = ""
    
    if option.output:
        output_path = option.output+"/"
    
    cfg_path  = output_path+ "cfg/"
    csh_path  = output_path+ "csh/"
    logs_path = output_path+ "logs/"

    subset = []
    njob = 0

    #make directories
    if not os.path.exists(cfg_path):
        _mkdir(cfg_path)
    if not os.path.exists(csh_path):
        _mkdir(csh_path)
    if not os.path.exists(logs_path):
        _mkdir(logs_path)
    
    
    #filesperjob = 0
    #inputfile = []
    if not option.mc:
        inputfile = open(list_of_files)
        totalfiles = len(inputfile.readlines())
        inputfile.seek(0,0)
        filesperjob = float(totalfiles)/float(number_of_jobs)
        filesperjob = int(filesperjob)
    
    
        for ifile in inputfile:
            ignoreline = 0
            if ifile.find("replace")!=-1 or ifile.find("}")!=-1 or ifile.find("{")!=-1:
                ignoreline = 1
            
            if ignoreline==0:
                ifile = ifile.strip("'")
                ifile = ifile.strip('\',\n')
                #ifile = ifile.strip("'")
                if len(subset) == filesperjob:
                    njob = njob + 1
                    if fin_run == 0 and njob >= ini_run:
                        submit_jobs(njob,subset,ini_cfgfile,output_path)
                    elif fin_run > 0 and njob <= fin_run and njob >= ini_run:
                        submit_jobs(njob,subset,ini_cfgfile,output_path)
            
                    subset = []

                subset.append(ifile)
        
        if len(subset)>0:
            njob = njob + 1
            if fin_run == 0 and njob >= ini_run:
                submit_jobs(njob,subset,ini_cfgfile,output_path)
            elif fin_run > 0 and njob <= fin_run and njob >= ini_run:
                submit_jobs(njob,subset,ini_cfgfile,output_path)
        
        #submit_jobs(njob,subset,ini_cfgfile,output_path)

    else:
        ijob = 1
        while ijob <= int(number_of_jobs):
            submit_jobs(ijob,subset,ini_cfgfile,output_path)
            ijob = ijob + 1



