#! /usr/bin/env python

from ROOT import *

import sys,os, math


# Luminosity in 1/pb
L = 0.58

file = {}

file['ttbar'] = 'NtupleMaker/MC/v2/TTbar/ttmuj_ntuple.root'
file['QCD']   = 'NtupleMaker/MC/v2/InclusiveMu15/ttmuj_ntuple.root'
file['WJets'] = 'NtupleMaker/MC/v2/WJets/ttmuj_ntuple.root'
file['ZJets'] = 'NtupleMaker/MC/v2/WJets/ttmuj_ntuple.root'

# xsection in pb
xsec = {}
xsec['ttbar'] =   157.5
xsec['QCD']   = 79688.
xsec['WJets'] = 31314.
xsec['ZJets'] =  3048.

keys = file.keys()
cutflow = {}

for sample in keys:

    print " processing " + sample
    afile = TFile(file[sample])
    h = gDirectory.Get('cutflow')

    cutmap = {}

    for acut in range(1, h.GetNbinsX() + 1 ):
        cutmap[ acut ] = h.GetBinContent( acut );

    scale = ( L * xsec[ sample ] / cutmap[1] )

    for key in cutmap.keys():

        cutmap[ key ] = scale * cutmap[ key]
        print " cut "+str(key) +" = "+str( round(cutmap[key],1) )

    cutflow[ sample ] = cutmap


    

