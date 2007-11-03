#!/usr/bin/env python2
############################################################################
#
# Author: Francisco Yumiceva
# yumiceva@fnal.gov
#
############################################################################
import os, string, re,sys
from time import gmtime, localtime, strftime
#
# path to this scripts and the used template file:
#
TemplatesPATH =  os.getcwd()
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

if not os.path.exists(cfg_path):
    os.makedir(cfg_path)
if not os.path.exists(csh_path):
    os.mkdir(csh_path)
if not os.path.exists(logs_path):
    os.mkdir(logs_path)


istest = "0"

#
# Path to the input/output data:
#

cfg_tags        = ["{FILENAME}",
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

def usage():
    print '\n'
    print 'Usage: '+sys.argv[0]+'  <cfg filename> <initial run> <last run> <only test script: y/n>'
    print 'e.g.:  '+sys.argv[0]+'  myconfig.cfg 100 150'


def main():
    
                
    if len(sys.argv) < 3:
        usage()
        return 1

    ini_cfgfile = sys.argv[1]
    template_fnames["cfg"]          =  TemplatesPATH+"/"+ini_cfgfile

    list_of_files = sys.argv[2]
    
    number_of_jobs = sys.argv[3]
    
    run = 0
    fin_run = 0
    
    istest = "0"
    output_path = ""
    if len(sys.argv)>=5:
        output_path = sys.argv[4]+"/"
    
    if len(sys.argv) == 6:
        istest = sys.argv[5]
        


    inputfile = open(list_of_files)
    totalfiles = len(inputfile.readlines())
    inputfile.seek(0,0)

    filesperjob = float(totalfiles)/float(number_of_jobs)
    filesperjob = int(filesperjob)

    subset = []
    njob = 0
    
    for ifile in inputfile:
        if len(subset) == filesperjob:
            njob = njob + 1
            submit_jobs(njob,subset,ini_cfgfile,output_path)
            subset = []

        subset.append(ifile)

    if len(subset)>0:
        njob = njob + 1
        submit_jobs(njob,subset,ini_cfgfile,output_path)
        

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
    #changearray.append((cfg_tags[0],sirun))     
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
    changearray.append((condor_tags[4],"yumiceva@fnal.gov"))
    change(template_fnames["condor"],outfilename_condor,changearray,0)
    print outfilename_condor + " has been written.\n"

    submitcommand ="/opt/condor/bin/condor_submit  "+outfilename_condor
    print submitcommand+"\n"
    
    if istest == "0":
        child   = os.popen(submitcommand)
    else:
        print "Not submitting into condor batch farm. This is only a test"
        


    
if __name__ =='__main__':
    sys.exit(main())
