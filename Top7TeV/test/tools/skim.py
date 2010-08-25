#! /usr/bin/env python

from ROOT import *

import sys,os, math

import histograms

gROOT.Reset()

gSystem.Load('libYumicevaTop7TeV.so')
#gROOT.LoadMacro('Yumiceva/Top7TeV/src/TopEventNtupleLinkDef.h+')

# input files
tfile = TFile("/uscms/home/samvel/Code/ttmuj/NtupleMaker/CMSSW_3_6_3/src/Top/Production/test/prod_25_Aug_2010_1/ntuple_patskim.root")
#TFile("/uscmst1b_scratch/lpc1/cmsroc/yumiceva/tmp/allntuple.root") 
tfile.cd()

# get tree
top = ROOT.gDirectory.Get( '/PATNtupleMaker/top' )
entries = top.GetEntriesFast()
# loop over entries

evt = TopEventNtuple()
top.SetBranchAddress('top.', evt)
cut = {}
#cut['processed'] = cut['OneIsoMuon'] = cut['VetoElectron'] = 0
#cut['1jet'] = cut['2jet'] = cut['3jet'] = cut['4jet'] = 0
cutkeys = ['processed','OneIsoMuon','VetoElectron','1jet','2jet','3jet','4jet']
for i in cutkeys:
    cut[i] = 0

hmuon = {}
hmuon['pt'] = TH1F("muon_pt","p_{T}^{#mu}", 25, 0, 100)
hmuon['pt_1jet'] = TH1F("muon_pt_1jet","p_{T}^{#mu}", 25, 20, 100)
hmuon['pt_2jet'] = TH1F("muon_pt_2jet","p_{T}^{#mu}", 25, 20, 100)
hmuon['pt_3jet'] = TH1F("muon_pt_3jet","p_{T}^{#mu}", 25, 20, 100)
hmuon['pt_4jet'] = TH1F("muon_pt_4jet","p_{T}^{#mu}", 25, 20, 100)
hmuon['eta'] = TH1F("muon_eta","#eta^{#mu}", 20, -2.1, 2.1)
hmuon['eta_1jet'] = TH1F("muon_eta_1jet","#eta^{#mu}", 20, -2.1, 2.1)
hmuon['eta_2jet'] = TH1F("muon_eta_2jet","#eta^{#mu}", 20, -2.1, 2.1)
hmuon['eta_3jet'] = TH1F("muon_eta_3jet","#eta^{#mu}", 20, -2.1, 2.1)
hmuon['eta_4jet'] = TH1F("muon_eta_4jet","#eta^{#mu}", 20, -2.1, 2.1)

hM3 = {}
hM3['3jet'] = TH1F("M3","M3 [GeV/c^{2}]", 20, 0, 500)

                   
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
    hmuon['pt'].Fill( p4muon.Pt() )
    hmuon['eta'].Fill( p4muon.Eta() )
    
    for ele in electrons:
        nelec += 1

    if nelec > 0 : continue
    cut['VetoElectron'] += 1

    p4jets = []
    
    for jet in jets:

        if jet.pt > 30. :
            p4jets.append( TLorentzVector() )
            p4jets[njets].SetPtEtaPhiE(jet.pt,jet.eta,jet.phi,jet.e)
            njets += 1
            
    if njets > 0:
        cut['1jet'] += 1
    if njets > 1:
        cut['2jet'] += 1
    if njets > 2:
        cut['3jet'] += 1
        p4HadTop = TLorentzVector()
        p4HadTop = p4jets[0] + p4jets[1] + p4jets[2]
        hM3['3jet'].Fill( p4HadTop.M() )
        
    if njets > 3:

        cut['4jet'] += 1
        
        #print "-15 "+str(mupt)+" "+str(mueta)+' '+str(muphi)
        #print "-5 "+str(evt.MET)+" "+str(evt.METeta)+' '+str(evt.METphi)
        #passed += 1
        #for jet in jets:
        #    print str(jet.e) +' '+str(jet.pt)+' '+str(jet.eta)+' '+str(jet.phi) 

print "done."
print "Cut flow Table"
for key in cutkeys:
    print key + " " + str(cut[key])

cv1 = TCanvas('muon_pt','muon_pt',600,600)
hmuon['pt'].Draw()
cv2 = TCanvas('muon_eta','muon_eta',600,600)
hmuon['eta'].Draw()

raw_input ("Enter to quit:")
