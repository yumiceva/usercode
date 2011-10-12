#! /usr/bin/env python

#from ROOT import *

import sys,os, math
import subprocess

if len(sys.argv) < 2:
    print "usage: calcLimits.py mass=<800 or 1000 or other> observed/expected muons/electrons/combined"
    print " e.g. run expected limits for mass 1200: calcLimits.py 1200 expected muons"
    print " e.g. run observed limits for all the masses: calcLimits.py -1 observed combined"
    sys.exit()


Mass = sys.argv[1]
IsObserved = True
if sys.argv[2]=="expected": IsObserved = False
Channel = sys.argv[3]

obs_cfg = "Wp_obs_limits.cfg"
exp_cfg = "Wp_exp_limit.cfg"
templates_cfg = "Wp_templates.cfg"

# copy original files
masses = [ '800', '900', '1000', '1100','1200','1300','1400', '1500','1600','2000' ]

#prefix = "Wprime_" # for pythia
prefix = "WpRH"+"_"+Channel # comphep

for OP in masses:

    if OP==Mass or Mass=="-1":

        # create dir
        os.system("mkdir "+prefix+OP)
        dir = prefix+OP+"/"

        if Channel=="muons" or Channel=="combined":
            os.system("ln -s ../commonWp.cfg "+dir+"commonWp.cfg")
            os.system("ln -s ../templates.root "+dir+"templates.root")
            os.system("ln -s ../templates_btag_syst.root "+dir+"templates_btag_syst.root")
            os.system("ln -s ../templates_jes_up.root "+dir+"templates_jes_up.root")
            os.system("ln -s ../templates_jes_down.root "+dir+"templates_jes_down.root")
        if Channel=="electrons" or Channel=="combined":
            os.system("ln -s ../templates_e.root "+dir+"templates_e.root")
            os.system("ln -s ../templates_e_btagUp.root "+dir+"templates_e_btagUp.root")
            os.system("ln -s ../templates_e_btagDown.root "+dir+"templates_e_btagDown.root")
            os.system("ln -s ../templates_e_jesUp.root "+dir+"templates_e_jesUp.root")
            os.system("ln -s ../templates_e_jesDown.root "+dir+"templates_e_jesDown.root")
                
        os.system("cp "+obs_cfg+" "+dir+obs_cfg) 
        os.system("cp "+exp_cfg+" "+dir+exp_cfg)
        os.system("cp "+templates_cfg+" "+dir+templates_cfg)
        
        # modify files and run
        cmd = "perl -pi -e 's/\@wprime800-mu0t/\@wprime"+str(OP)+"-mu0t/g' "+dir+templates_cfg
        os.system(cmd)
        cmd = "perl -pi -e 's/\@wprime800-e0t/\@wprime"+str(OP)+"-e0t/g' "+dir+templates_cfg
        os.system(cmd)
        cmd = "perl -pi -e 's/results_observed.root/results_observed_"+str(OP)+".root/g' "+dir+obs_cfg
        os.system(cmd)
        cmd = "perl -pi -e 's/results_expected.root/results_expected_"+str(OP)+".root/g' "+dir+exp_cfg
        os.system(cmd)
        cmd = "perl -pi -e 's/lib/..\/lib/g' "+dir+"commonWp.cfg"
        os.chdir(dir)
        
        if IsObserved:
            #cmd = "../../bin/theta "+obs_cfg
            p = subprocess.Popen(['../../bin/theta',obs_cfg])
            #os.system(cmd)
            #print "a"
        else:
            #cmd = "../../bin/theta "+exp_cfg
            #os.system(cmd)
            p = subprocess.Popen(['../../bin/theta',exp_cfg])
            #print "b"
        os.chdir('..')

