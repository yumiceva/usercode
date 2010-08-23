#! /usr/bin/env python

from ROOT import *

import sys,os, math

gROOT.Reset()

gSystem.Load('libYumicevaTop7TeV.so')
#gROOT.LoadMacro('Yumiceva/Top7TeV/src/TopEventNtupleLinkDef.h+')

# input files
tfile = TFile("/uscmst1b_scratch/lpc1/cmsroc/yumiceva/tmp/allntuple.root") 
tfile.cd()

# get tree
top = ROOT.gDirectory.Get( 'top' )
entries = top.GetEntriesFast()
# loop over entries

evt = TopEventNtuple()
top.SetBranchAddress('top.', evt)
passed = 0

for jentry in xrange( entries ):

    # get the next tree in the chain
    ientry = top.LoadTree(jentry)
    if ientry < 0:
        break

    # verify file/tree/chain integrity
    nb = top.GetEntry( jentry )
            
    if nb <= 0 or not hasattr( evt, 'run' ):
        continue

    #print "entry = "+str(jentry)
    
    muons = evt.muons
    jets  = evt.jets
    electrons = evt.electrons
    
    nmuons = 0
    nelec = 0
    njets = 0
    mupt = 0.
    mueta = 0.
    muphi = 0.
    
    for mu in muons:

        if mu.IsTightIsoMuon == 1:

            nmuons += 1
            mupt = mu.pt
            mueta = mu.eta
            muphi = mu.phi
            
    if nmuons != 1:
        continue

    for ele in electrons:
        nelec += 1

    if nelec > 0 : continue
    
    for jet in jets:

        if jet.pt > 30. :

            njets += 1

    if njets >= 4:
        
        print "-15 "+str(mupt)+" "+str(mueta)+' '+str(muphi)
        print "-5 "+str(evt.MET)+" "+str(evt.METeta)+' '+str(evt.METphi)
        passed += 1
        for jet in jets:

            print str(jet.e) +' '+str(jet.pt)+' '+str(jet.eta)+' '+str(jet.phi) 

print "done."
print "total = "+str(passed)

