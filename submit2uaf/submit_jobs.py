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
main_path   = "/uscmst1b_scratch/lpc1/cmsroc/yumiceva/CMSSW_1_2_0_pre3/src/RecoVertex/BeamSpotProducer/test/"

out_path      = main_path+"reco_files/"
scripts_path  = main_path+"scripts/"

cfg_path      = scripts_path+"cfg/"
condor_path   = scripts_path+"condor/"
csh_path      = scripts_path+"csh/"
logs_path     = scripts_path+"logs/"


#
# Path to the input/output data:
#

cfg_tags        = ["{SEED1}",
                   "{SEED2}",
                   "{OUTPUT_FILENAME}"]

scripts_tags    = ["{PATHTOOUT}",
                   "{CONFIGFILE}",
                   "{OUTFILE}"]

condor_tags     = ["{EXECUTABLE}",
                   "{OUTPUT}",
                   "{ERROR}",
                   "{LOG}",
                   "{NOTIFY}"]


template_fnames = {}
template_fnames["cfg"]          =  TemplatesPATH+"/reco_d0_phi_test_template.cfg"
template_fnames["condor"]       =  TemplatesPATH+"/template.condor"
template_fnames["csh"]          =  TemplatesPATH+"/runcms.csh"


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

def change(infile,outfile,changearray):
    fin  = open(infile)
    fout = open(outfile,"w")
    for line in fin.readlines():
        for change in changearray:
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
    run = sys.argv[2]
    fin_run = sys.argv[3]
    
    istest = "0"
    if len(sys.argv) == 5:
        istest = sys.argv[4]

    for irun in range(int(run),int(fin_run)+1):

        sirun = str(irun)
        seed2 = int(irun) + 12345

        mypath    = os.getcwd()
        cfgpath   = mypath.replace("scripts","cfg")
        outpath   = mypath.replace("scripts","reco_files")
        logpath   = mypath.replace("scripts","logs")
        cshpath   = mypath.replace("scripts","csh")

        cfgfile           = ini_cfgfile.replace("_template","")
        outfilename_cfg   = cfg_path  + cfgfile.replace(".cfg","_"+sirun+".cfg")
        outfilename_root  = cfgfile.replace(".cfg","_"+sirun+".root")
        outfilename_log   = logs_path + cfgfile.replace(".cfg","_"+sirun+".log")
        outfilename_csh   = csh_path  + cfgfile.replace(".cfg","_"+sirun+".csh")
        outfilename_condor= csh_path  + cfgfile.replace(".cfg","_"+sirun+".condor")
    
        if  os.path.isfile(outpath+"/"+outfilename_root):
            print "Not submitting into condor batch farm since rootoutput already exists"
            print "delete or rename: "+rootoutput+"\n to resubmit"
            sys.exit(main())
            
            #
            # First create config (.cfg) file:
            #
        
        changearray=[]
        changearray.append((cfg_tags[0],sirun))     
        changearray.append((cfg_tags[1],str(seed2)))
        changearray.append((cfg_tags[2],outfilename_root))
        change(template_fnames["cfg"],outfilename_cfg,changearray)
        print outfilename_cfg + " has been written.\n"
    
        #
        # now create the script to process the file:
        #
        changearray=[]
        changearray.append((scripts_tags[0],outpath+"/"))
        changearray.append((scripts_tags[1],outfilename_cfg))
        changearray.append((scripts_tags[2],outfilename_root))
        change(template_fnames["csh"],outfilename_csh,changearray)
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
        change(template_fnames["condor"],outfilename_condor,changearray)
        print outfilename_condor + " has been written.\n"
    
        if istest == "0":
            submitcommand ="/opt/condor/bin/condor_submit  "+outfilename_condor
            child   = os.popen(submitcommand)
        else:
            print "Not submitting into condor batch farm. This is only a test"


    
if __name__ =='__main__':
    sys.exit(main())
