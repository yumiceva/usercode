#! /usr/bin/env python

from ROOT import *

import sys
import os
import math

import histograms

gROOT.Reset()

gSystem.Load('libYumicevaTop7TeV.so')
gROOT.ProcessLine('.L tdrstyle.C')
ROOT.setTDRStyle()
gROOT.ProcessLine('.L CMSTopStyle.cc+')
style = ROOT.CMSTopStyle()
style.setupICHEPv1()

#gROOT.LoadMacro('Yumiceva/Top7TeV/src/TopEventNtupleLinkDef.h+')

# input files
tfile = TFile("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/1.34pb-1/ttmuj_data_Aug25.root")
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
cutkeys = ['processed','OneIsoMuon','VetoLooseMu','VetoElectron','1jet','2jet','3jet','4jet']
for i in cutkeys:
    cut[i] = 0


# create histograms
h = {}
h[1] = TH1F("h1","Impact Parameter d_{0} [cm]",30,0,0.1);
h[2] = TH1F("h2","Impact Parameter d_{0} [cm]",30,0,0.1);
h[3] = TH1F("h3","Impact Parameter d_{0} [cm]",30,0,0.1);
h[4] = TH1F("h4","Impact Parameter d_{0} [cm]",30,0,0.1);
hscatter = TH2F("hscatter","",300,0,1,300,0,1000)
          
relisocut ={}
relisocut[1] = [0.,0.2]
relisocut[2] = [0.2,0.5]
relisocut[3] = [0.5,1.0]
relisocut[4] = [1.0,9999]
                   
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

    if jentry%1000 == 0:
        print "Processing entry = "+str(jentry)

    # get collections
    muons = evt.muons
    jets  = evt.Calojets
    electrons = evt.electrons
    
    nloosemuons = 0
    ntightmuons = 0
    nisotightmuons = 0
    nelec = 0
    njets = 0
    p4muon = TLorentzVector()
    thereliso = 0.
    thed0 = 0.
    
    for mu in muons:

        # check loose iso muons
        if mu.reliso03<0.2:
            nloosemuons +=1
        if mu.pt>20. and math.fabs(mu.eta)<2.1 and \
           mu.muonhits>0 and mu.normchi2<10 and \
           mu.trackerhits>=11 and math.fabs(mu.CalodeltaR)>0.3:

            ntightmuons += 1
            p4muon.SetPtEtaPhiE( mu.pt, mu.eta, mu.phi, mu.e )
            thereliso = mu.reliso03
            thed0 = mu.d0
            if math.fabs(thed0)<0.02 and thereliso<0.05:
                nisotightmuons += 1

    if nisotightmuons > 1:
        continue
    if nloosemuons > 1:
        continue
    if nisotightmuons !=1 and ntightmuons>1:
        continue
    
    for ele in electrons:
        nelec += 1

    if nelec > 0 : continue
    cut['VetoElectron'] += 1

    p4jets = []
    bdisc = []
    
    for jet in jets:

        if jet.pt > 30. :
            p4jets.append( TLorentzVector() )
            p4jets[njets].SetPtEtaPhiE(jet.pt,jet.eta,jet.phi,jet.e)
            #bdisc.append( jet.btag_TCHP)
            njets += 1

    for acut in relisocut.keys():
        inicut = relisocut[acut][0]
        fincut = relisocut[acut][1]
        #print str(inicut)+" "+str(fincut)
        
        if thereliso>= inicut and thereliso< fincut:
            #print str(thereliso)+" "+str(thed0)
            h[acut].Fill( math.fabs(thed0) )
    hscatter.Fill( thereliso, 10000.*math.fabs(thed0) )
                                
    if njets > 0:
        cut['1jet'] += 1
    if njets > 1:
        cut['2jet'] += 1
    if njets > 2:
        cut['3jet'] += 1
        p4HadTop = TLorentzVector()
        p4HadTop = p4jets[0] + p4jets[1] + p4jets[2]
                
    if njets > 3:

        cut['4jet'] += 1
        

print "done."
print "Cut flow Table"
for key in cutkeys:
    print key + " " + str(cut[key])

cv2 = TCanvas('scatter','scatter',600,600)
hscatter->Draw()
hscatter->SetXTitle("#mu Relative Isolation")
hscatter->SetYTitle("d_{0} [#mum]")

resultsfile = TFile("plot_ABCD_correlations.root","RECREATE")
for ih in h.keys():
    h[ih].Write()
hscatter.Write()

cv1 = TCanvas('muon_pt','muon_pt',600,600)
h[1].Draw()
h[2].Draw("same")
h[3].Draw("same")
h[4].Draw("same")


raw_input ("Enter to quit:")
