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
cut = {}
cut['processed'] = cut['OneIsoMuon'] = cut['VetoElectron'] = 0
cut['1jet'] = cut['2jet'] = cut['3jet'] = cut['4jet'] = 0

hmuon['pt'] = TH1F("muon_pt","p_{T}^{#mu}", 25, 0, 100)
hmuon['pt_1jet'] = TH1F("muon_pt_1jet","p_{T}^{#mu}", 25,0, 100)
hmuon['pt_2jet'] = TH1F("muon_pt_2jet","p_{T}^{#mu}", 25,0, 100)
hmuon['pt_3jet'] = TH1F("muon_pt_3jet","p_{T}^{#mu}", 25,0, 100)
hmuon['pt_4jet'] = TH1F("muon_pt_4jet","p_{T}^{#mu}", 25,0, 100)
hmuon['eta'] = TH1F("muon_eta","p_{T}^{#mu}", 25,0, 100)
hmuon['eta_1jet'] = TH1F("muon_eta_1jet","#eta^{#mu}", 25,0, 100)
hmuon['eta_2jet'] = TH1F("muon_eta_2jet","#eta^{#mu}", 25,0, 100)
hmuon['eta_3jet'] = TH1F("muon_eta_3jet","#eta^{#mu}", 25,0, 100)
hmuon['eta_4jet'] = TH1F("muon_eta_4jet","#eta^{#mu}", 25,0, 100)


                   
for jentry in xrange( entries ):

    # get the next tree in the chain
    ientry = top.LoadTree(jentry)
    if ientry < 0:
        break

    # verify file/tree/chain integrity
    nb = top.GetEntry( jentry )
            
    if nb <= 0 or not hasattr( evt, 'run' ):
        continue

    cut['processed'] += 1

    if jentry%500 == 0:
        print "Processing entry = "+str(jentry)

    # get collections
    muons = evt.muons
    jets  = evt.Calojets
    electrons = evt.electrons
    
    nmuons = 0
    nelec = 0
    njets = 0
    p4muon = TLorentzVector()
    
    for mu in muons:

        if mu.IsTightIsoMuon == 1:

            nmuons += 1
            p4muon.SetPtEtaPhiE( mu.pt, mu.eta, mu.phi, mu.e )
                        
    if nmuons != 1:
        continue
    cut['OneIsoMuon'] += 1
    
    for ele in electrons:
        nelec += 1

    if nelec > 0 : continue
    cut['VetoElectron'] += 1
    
    for jet in jets:

        if jet.pt > 30. :

            njets += 1

    if njets > 0:
        cut['1jet'] += 1
    if njets > 1:
        cut['2jet'] += 1
    if njets > 2:
        cut['3jet'] += 1
    if nejts > 3:

        cut['4jet'] += 1
        
        print "-15 "+str(mupt)+" "+str(mueta)+' '+str(muphi)
        print "-5 "+str(evt.MET)+" "+str(evt.METeta)+' '+str(evt.METphi)
        passed += 1
        for jet in jets:

            print str(jet.e) +' '+str(jet.pt)+' '+str(jet.eta)+' '+str(jet.phi) 

print "done."
print "Cut flow"
for key in cut.keys():
    print key + " " + str(cut[key])

