#! /usr/bin/env python

"""
   top.py

   Script to create histograms and cut flow table for lepton+jets.
   
   usage: %prog 
   -b, --batch : run in batch mode without graphics.
   -f, --flavor = FLAVOR: flavor for Wjets/Vqq samples. 1 = b(b), 2 = c(c), 3 = light, 4 = discarded paths
   -I, --Initial = INITIAL: initial entry.
   -N, --Nevents = Nevents: Maximum entry to run.
   -o, --outfilename = OUTFILENAME: name of output file.
   -O, --OutputDir = OUTPUTDIR: name of output directory
   -s, --sample = SAMPLE: Ntuple sample: data, TTbar, Wjets, Zjets, QCD, etc.
   -v, --verbose : verbose output.
      
   Francisco Yumiceva (yumiceva@fnal.gov)
   Fermilab 2010
   
"""


from ROOT import *

import sys
import os
import math
import re

#________CONFIG_______OPTIONS________________
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

#____________END______CONFIG____OPTIONS________________________________________
                                                                                                                                

# Options
dataType = "data"
dataTypeSuffix = ""
verbose = False
OutputDir = "./"
Flavor = 0
FlavorStr = ""
InitialEntry = 0
FinalEntry = 0
outfilename = "newfile.root"

# check options
option,args = parse(__doc__)
#if option.help:
#    exit()

if not option.sample:
    exit()

if option.batch:
    ROOT.gROOT.SetBatch()
    print "running ROOT in batch mode."
if option.verbose:
    verbose = True

if option.sample:
    dataType = option.sample
print "sample: "+dataType

if option.OutputDir:
    OutputDir = option.OutputDir+"/"
print "output directory: " + OutputDir

if option.outfilename:
    outfilename = OutputDir+option.outfilename
else:
    outfilename = OutputDir+outfilename
print "output filename: "+outfilename

if option.flavor:
    Flavor = int(option.flavor)
    if Flavor == 1: FlavorStr = "bb"
    if Flavor == 2: FlavorStr = "cc"
    if Flavor == 3: FlavorStr = "light"
    if Flavor == 4: FlavorStr = "discarded"
    print "Selecting MC sample with flavor "+FlavorStr

if option.Initial:
    InitialEntry = int(option.Initial)
    print "Initial entry = "+str(InitailEntry)
if option.Nevents:
    FinalEntry = int(option.Nevents)
    print "Final entry = "+str(FinalEntry)


gROOT.Reset()

# libraries
gSystem.Load('libYumicevaTop7TeV.so')

        
cutmap = {}
cutmap['Processed'] = 0
cutmap['CleanFilters'] = 0
cutmap['HLT'] = 0
cutmap['GoodPV'] = 0
if Flavor != 0:
    cutmap['Flavor'] = 0
cutmap['MET'] = 0
cutmap['OneIsoMu'] = 0
cutmap['LooseMuVeto'] = 0
cutmap['ElectronVeto'] = 0
cutmap['1Jet'] = 0
cutmap['2Jet'] = 0
cutmap['3Jet'] = 0
cutmap['4Jet'] = 0


# Setup TChain
top = TChain('/PATNtupleMaker/top')

# input files
data_repo = "/uscms_data/d3/ttmuj/Documents/NtupleMaker/"

sigma_Wjets = 24170. # LO

if dataType=="Wjets":
    #top.Add(data_repo+"MC/V00-01-05/WJets_Z2_Mu.root")
    top.Add(data_repo+"MC/V00-01-06/WJets_D6T_Mu.root")
    top.Add(data_repo+"MC/V00-01-06/Vqq_D6T_Mu.root")
if dataType=="Zjets":
#    top.Add(data_repo+"MC/V00-01-05/ZJets_Z2_Mu.root")
    top.Add(data_repo+"MC/V00-01-06/ZJets_D6T_Mu.root")
    top.Add(data_repo+"MC/V00-01-06/Vqq_D6T_Mu.root")
if dataType=="Wc":
    top.Add(data_repo+"MC/V00-01-06/Wc_D6T_Mu.root")
if dataType=="Wjets_matchingup":
    sigma_Wjets = 25950
    dataType="Wjets"
    dataTypeSuffix = "matchingup"
    top.Add(data_repo+"MC/V00-01-05/WJets_matchingup_Mu.root")
    top.Add(data_repo+"MC/V00-01-06/Vqq_D6T_Mu.root")
if dataType=="Wjets_matchingdown":
    sigma_Wjets = 24990
    dataType="Wjets"
    dataTypeSuffix = "matchingdown"
    top.Add(data_repo+"MC/V00-01-05/WJets_matchingdown_Mu.root")
    top.Add(data_repo+"MC/V00-01-06/Vqq_D6T_Mu.root")
if dataType=="Wjets_scaleup":
    sigma_Wjets = 27230
    dataType="Wjets"
    dataTypeSuffix = "scaleup"
    top.Add(data_repo+"MC/V00-01-06/WJets_scaleup_Mu.root")
    top.Add(data_repo+"MC/V00-01-06/Vqq_scaleup_Mu.root")
if dataType=="Wjets_scaledown":
    sigma_Wjets = 26530 # LO
    dataType="Wjets"
    dataTypeSuffix = "scaledown"
    top.Add(data_repo+"MC/V00-01-06/WJets_scaledown_Mu.root")
    top.Add(data_repo+"MC/V00-01-06/Vqq_scaledown_Mu.root")
if dataType=="Zjets_matchingup":
    dataType="Zjets"
    dataTypeSuffix = "matchingup"
    top.Add(data_repo+"MC/V00-01-05/ZJets_matchingup_Mu.root")
if dataType=="Zjets_matchingdown":
    dataType="Zjets"
    dataTypeSuffix = "matchingdown"
    top.Add(data_repo+"MC/V00-01-05/ZJets_matchingdown_Mu.root")
if dataType=="Zjets_scaleup":
    dataType="Zjets"
    dataTypeSuffix = "scaleup"
    top.Add(data_repo+"MC/V00-01-05/ZJets_scaleup_Mu.root")
if dataType=="Zjets_scaledown":
    dataType="Zjets"
    dataTypeSuffix = "scaledown"
    top.Add(data_repo+"MC/V00-01-05/ZJets_scaledown_Mu.root")


print "files opened:"
tmplistfiles = top.GetListOfFiles()

N_Vqq = 0
N_Wjets = 0

for iifile in tmplistfiles:
    tmptfilename = iifile.GetTitle()
    print tmptfilename
    # read number of events passing trigger
    tmptfile = TFile(tmptfilename)
    tmptfile.cd()
    TrigHist = ROOT.gDirectory.Get('/triggerFilter/eventCount')
    if TrigHist:
        cutmap['CleanFilters'] = TrigHist.GetBinContent( 1 )
        cutmap['HLT'] = TrigHist.GetBinContent( 2 )
        print "N processed = " + str(cutmap['CleanFilters'])
        if tmptfilename.find("Vqq")!=-1:
            N_Vqq = float(cutmap['CleanFilters'])
        else:
            N_Wjets = float(cutmap['CleanFilters'])
            
    tmptfile.cd()
    PVHist = gDirectory.Get('/PATNtupleMaker/cutflow')
    if PVHist:
        cutmap['GoodPV'] = PVHist.GetBinContent( 3 )
    tmptfile.Close()
    del(tmptfile)
    
#del(tmplistfiles)

entries = top.GetEntriesFast()

# setup ntuple object
evt = TopEventNtuple()
top.SetBranchAddress('top.', AddressOf(evt))

# output ntuple
newfile = TFile(outfilename,"RECREATE")

newtop = TTree("top","top")
newtop.Branch("top.","TopEventNtuple",AddressOf(evt),64000,1)


# create friend ntuple for weights
gROOT.ProcessLine(
    "struct weight {\
    Float_t   weight;\
    Int_t     isVqq;\
    Int_t     isTrashPath;\
    };" );

wntuple = weight()
            
wtree = TTree("weight","weight")
wtree.Branch('weight', AddressOf( wntuple, 'weight'),'weight/F')
wtree.Branch('isVqq', AddressOf( wntuple, 'isVqq'),'isVqq/I')
wtree.Branch('isTrashPath', AddressOf( wntuple, 'isTrashPath'),'isTrashPath/I')

#sigma_Wjets = 24170. # LO
#sigma_Zjets = 
sigma_Vqq = 35.8 # LO
kfactor = 1.3

# loop over events
for jentry in xrange( entries ):

    # get the next tree in the chain
    ientry = top.LoadTree(jentry)
    if ientry < 0:
        break

    #if ientry <= 100: break
    if option.Initial:
        if ientry < InitialEntry : continue
    if option.Nevents:
        if ientry > FinalEntry : break
    
    # verify file/tree/chain integrity
    nb = top.GetEntry( jentry )
            
    if nb <= 0 or not hasattr( evt, 'run' ):
        continue

    tmpfile = top.GetFile()
    tmpfilename = tmpfile.GetName()
    del(tmpfile)
    
    isVqq = 0
    isTrashPath = 0
    weight = 1.
    if tmpfilename.find("Vqq")!=-1:
        isVqq = 1
        weight = kfactor * sigma_Vqq / N_Vqq
    else:
        weight = kfactor * sigma_Wjets / N_Wjets

    #print "w = "+str(weight)
    #cut['processed'] += 1

    if jentry%50000 == 0:
        print "Processing entry = "+str(jentry)

    # check if flavor is requested
    if Flavor != 0:
        passFlavor = False
        
        # check if we have a W event in Vqq sample
        if dataType.find("Wjets")!=-1 and isVqq==1 and evt.gen.isWevent!=1:
            continue
        
        path = evt.flavorHistory
        #print "Vqq="+str(isVqq)+" path="+str(path)
        #if dataType=="Wc" and Flavor==2 and path==4: passFlavor = True

        # check b(b) selection
        if isVqq==1 and Flavor==1 and path==1: passFlavor = True
        if isVqq==0 and Flavor==1 and path==2: passFlavor = True
        if isVqq==0 and Flavor==1 and path==5: passFlavor = True
        # check c(c) selection
        if isVqq==1 and Flavor==2 and path==3: passFlavor = True
        if isVqq==0 and Flavor==2 and (path==4 or path==6): passFlavor = True
        # check light selection
        if isVqq==0 and Flavor==3 and path==11: passFlavor = True
        # chech discarted paths
        if isVqq==0 and Flavor==4 and (path>=7 and path<=10):
            passFlavor = True
            isTrashPath = 1
            
        if not passFlavor:
            continue
        cutmap['Flavor'] += 1

    # fill new tree
    #print "weight = "+str(weight)
    wntuple.weight = weight
    wntuple.isVqq = isVqq
    wntuple.isTrashPath = isTrashPath
    wtree.Fill()
    newtop.Fill()

print "loop done."
print "Selected events in skim: " + str(cutmap['Flavor'])

newfile.cd()
newfile.mkdir("PATNtupleMaker")
newfile.cd("PATNtupleMaker")
newtop.Write()
wtree.Write()

print "end."

#print "Cut flow Table"
#cutmapkeys =[ "CleanFilters","HLT","GoodPV","OneIsoMu","LooseMuVeto","ElectronVeto","MET","1Jet","2Jet","3Jet","4Jet"]
#if Flavor != 0:
#    cutmapkeys =[ "CleanFilters","HLT","GoodPV","Flavor","OneIsoMu","LooseMuVeto","ElectronVeto","MET","1Jet","2Jet","3Jet","4Jet"]
    
#for key in cutmapkeys:
#    print key + " " + str(cutmap[key])



