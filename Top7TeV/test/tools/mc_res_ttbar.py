#! /usr/bin/env python

from ROOT import *

import sys
import os
import math

JetType = "calo"
dataType = "data"
ApplyDeltaR = True
if len(sys.argv)>1:
    if sys.argv[1] == "JPT": JetType = "JPT"
    if sys.argv[1] == "PF": JetType = "PF"

    if len(sys.argv)>2: dataType = sys.argv[2]
    if len(sys.argv)>3:
                ApplyDeltaR = False
                print "deltaR cut will be removed."


datafilename = "/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v5/ttmuj_TTbar.root"

tfile = TFile(datafilename)
print "read file "+datafilename
print "use "+JetType+" collections"


# get tree
top = ROOT.gDirectory.Get( '/PATNtupleMaker/top' )
entries = top.GetEntriesFast()
# loop over entries

for jentry in xrange( entries ):

    # get the next tree in the chain
    ientry = top.LoadTree(jentry)
    if ientry < 0:
        break
    
    # verify file/tree/chain integrity
    nb = top.GetEntry( jentry )
    
    if nb <= 0 or not hasattr( evt, 'run' ):
        continue

    if jentry%50000 == 0:
        print "Processing entry = "+str(jentry)
                
    jets  = evt.Calojets
    if JetType == "JPT": jets = evt.JPTjets
    if JetType == "PF": jets = evt.PFjets

    if evt.isSemiLeptonic != 1:
        continue
    if evt.LeptonicChannel != 2:
        continue

    if Wp_e == -99 or Wq_e == -99 or bHad_e == -99 or bLep_e == -99:
        print "no MC truth information for partons. continue."
        continue

    

print "done."

