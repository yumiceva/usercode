#! /usr/bin/env python

from ROOT import *

import sys,os, math


L = 0.84 # Luminosity in 1/pb
IsMC = True

xsec = {} # xsection in pb
file = {}
# MC files
if IsMC:
    file['ttbar'] = '/uscms_data/d2/samvel/Public/2Francisco/ttmuj_ntuple.root'
    #'NtupleMaker/MC/v2/TTbar/ttmuj_ntuple.root'
    #file['QCD']   = 'NtupleMaker/MC/v2/InclusiveMu15/ttmuj_ntuple.root'
    #file['WJets'] = 'NtupleMaker/MC/v2/WJets/ttmuj_ntuple.root'
    #file['ZJets'] = 'NtupleMaker/MC/v2/WJets/ttmuj_ntuple.root'

    xsec['ttbar'] =   157.5
    #xsec['QCD']   = 79688.
    #xsec['WJets'] = 31314.
    #xsec['ZJets'] =  3048.
else:
# data files
    file['Jun14'] = 'NtupleMaker/Data/0.84pb-1/Jun14/ttmuj_ntuple.root'
    file['MB']    = 'NtupleMaker/Data/0.84pb-1/MinimumBias/ttmuj_ntuple.root'
    file['Jul16'] = 'NtupleMaker/Data/0.84pb-1/Jul16/ttmuj_ntuple.root'
    file['Prompt']= 'NtupleMaker/Data/0.84pb-1/PromptReco/ttmuj_ntuple.root'

keys = file.keys()
cutflow = {}
cutlabel = {}
cutlabel[1] = 'Processed'
cutlabel[2] = 'HLT'
cutlabel[3] = 'PV'
cutlabel[4] = '1 iso mu'
cutlabel[5] = 'loose mu veto'
cutlabel[6] = 'electron veto'
cutlabel[7] = 'jets>=1'
cutlabel[8] = 'jets>=2'
cutlabel[9] = 'jets>=3'
cutlabel[10] = 'jets>=4'

allmap = {}

for sample in keys:

    print " processing " + sample
    afile = TFile(file[sample])
    h = gDirectory.Get('cutflow')

    cutmap = {}

    for acut in range(1, h.GetNbinsX() + 1 ):
        cutmap[ acut ] = h.GetBinContent( acut );

    scale = 1.
    if IsMC:
        scale = ( L * xsec[ sample ] / cutmap[1] )

    for key in cutmap.keys():

        cutmap[ key ] = scale * cutmap[ key]
        print " cut "+str(key) + " ("+cutlabel[key]+") "+" = "+str( round(cutmap[key],1) )
        if allmap.has_key(key):
            allmap[ key ] += cutmap[ key ]
        else:
            allmap[ key ] = cutmap[ key ]

    cutflow[ sample ] = cutmap

print " TOTAL"
for key in allmap.keys():
    print " cut "+str(key) + " ("+cutlabel[key]+") "+" = "+str( round(allmap[key],1) )

    

