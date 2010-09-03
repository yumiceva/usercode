#! /usr/bin/env python

from ROOT import *

import sys
import os
import math

import histograms

JetType = "calo"
dataType = "data"
if len(sys.argv)>1:
    if sys.argv[1] == "JPT": JetType = "JPT"
    if sys.argv[1] == "PF": JetType = "PF"

    if len(sys.argv)>2: dataType = sys.argv[2]
        
gROOT.Reset()
gSystem.Load('libYumicevaTop7TeV.so')
gROOT.ProcessLine('.L METzCalculator.cc+')
METzCalculator = METzCalculator()
gROOT.ProcessLine('.L JetCombinatorics.cc+')

# Plot Style
gROOT.ProcessLine('.L tdrstyle.C')
ROOT.setTDRStyle()
gROOT.ProcessLine('.L CMSTopStyle.cc+')
style = CMSTopStyle()
style.setupICHEPv1()

# output txt file
txtfile = file("4jetevents_"+JetType+".txt","w")

# input files
datafilename = "/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.79pb-1/ttmuj_data_all.root"
"/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/1.34pb-1/ttmuj_data_Aug25.root"
if dataType=="TTbar":
    datafilename = "/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v4/ttmuj_TTbar.root"
if dataType=="Wjets":
    datafilename = "/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v4/ttmuj_Wjets.root"
if dataType=="Zjets":
    datafilename = "/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v4/ttmuj_Zjets.root"
if dataType=="QCD":
    datafilename = "/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v4/ttmuj_QCD.root"
tfile = TFile(datafilename)
print "read file "+datafilename
print "use "+JetType+" collections"
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
cutkeys = ['processed','NoDeltaR','OneIsoMuon','VetoLooseMu','VetoElectron','1jet','2jet','3jet','4jet']
for i in cutkeys:
    cut[i] = 0


# create histograms
hist = histograms.Hist()
hist.Create(dataType+JetType)

                   
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

    if jentry%50000 == 0:
        print "Processing entry = "+str(jentry)

    # get collections
    muons = evt.muons
    jets  = evt.Calojets
    if JetType == "JPT": jets = evt.JPTjets
    if JetType == "PF": jets = evt.PFjets
    electrons = evt.electrons

    p4MET = TLorentzVector()
    p4MET.SetPtEtaPhiE( evt.CaloMET, 0, evt.CaloMETphi, evt.CaloMET )
    Ht = evt.CaloHt
    if JetType == "JPT":
        p4MET.SetPtEtaPhiE(evt.tcMET, 0,evt.tcMETphi,evt.tcMET )
        Ht = evt.tcHt
    if JetType == "PF":
        p4MET.SetPtEtaPhiE(evt.PFMET, 0,evt.PFMETphi,evt.PFMET )
        Ht = evt.PFHt
    # some counters
    nloosemuons = 0
    ntightmuons = 0
    nelec = 0
    njets = 0
    p4muon = TLorentzVector()
    p4jets = []
    bdisc = []
    
    for mu in muons:

        #deltaR = mu.CalodeltaR
        #if JetType == "JPT": deltaR = mu.JPTdeltaR
        #if JetType == "PF": deltaR = mu.PFdeltaR
        
        # check loose iso muons
        if mu.reliso03<0.2:
            nloosemuons +=1
        if mu.pt>20. and math.fabs(mu.eta)<2.1 and mu.IsTrackerMuon==1:

            hist.muons['pt_cut1'].Fill(mu.pt)
            hist.muons['d0_cut1'].Fill(mu.d0)

            if math.fabs(mu.d0)<0.02 and mu.muonhits>0 and mu.normchi2<10 and \
               mu.trackerhits>10:

                hist.muons['pt_cut2'].Fill(mu.pt)
                hist.muons['reliso'].Fill(mu.reliso03)
                
                if mu.reliso03<0.05:

                    cut['NoDeltaR'] += 1
                    tmpp4Mu = TLorentzVector()
                    tmpp4Jet= TLorentzVector()
                    aDeltaR = 999
                    for jet in jets:
                        
                        if jet.pt>30.:
                            tmpp4Mu.SetPtEtaPhiE(mu.pt, mu.eta, mu.phi, mu.e )
                            tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e )
                            tmpdeltaR = tmpp4Mu.DeltaR(tmpp4Jet)
                            if tmpdeltaR < 0.001: continue
                            #if tmpdeltaR < aDeltaR and tmpdeltaR>0.01: aDeltaR = tmpdeltaR
                            if tmpdeltaR < aDeltaR: aDeltaR = tmpdeltaR
                    if aDeltaR < 999: hist.muons['deltaR'].Fill(aDeltaR)
                    if aDeltaR>0.3:
                        ntightmuons += 1
                        p4muon.SetPtEtaPhiE( mu.pt, mu.eta, mu.phi, mu.e )
                        
    if ntightmuons != 1:
        continue
    cut['OneIsoMuon'] += 1
    
    if nloosemuons > 1:
        continue
    cut['VetoLooseMu'] += 1
    for ele in electrons:
        nelec += 1
    if nelec > 0 : continue
    cut['VetoElectron'] += 1

    # inclusive
    hist.muons['pt'].Fill( p4muon.Pt() )
    hist.muons['eta'].Fill( p4muon.Eta() )
    hist.muons['phi'].Fill( p4muon.Phi() )
    Wpt = p4muon.Pt() + p4MET.Pt()
    Wpx = p4muon.Px() + p4MET.Px()
    Wpy = p4muon.Py() + p4MET.Py()
    WMt = sqrt(Wpt*Wpt-Wpx*Wpx-Wpy*Wpy)
    hist.Mt['Mt'].Fill( WMt )
    hist.MET['MET'].Fill( p4MET.Pt() )
    hist.MET['phi'].Fill( p4MET.Phi() )
    hist.MET['Ht'].Fill( Ht )
    
    # estimate Pz of neutrino
    METzCalculator.SetMET(p4MET)
    METzCalculator.SetMuon(p4muon)
    pzNu = METzCalculator.Calculate()
    p4Nu = TLorentzVector()
    p4Nu.SetPxPyPzE( p4MET.Px(), p4MET.Py(),pzNu,sqrt(p4MET.Pt()*p4MET.Pt()+pzNu*pzNu))
    hist.MET['PzNu'].Fill(pzNu)
    p4LepW = p4MET + p4Nu
    hist.MET['LepWmass'].Fill(p4LepW.M())
    if METzCalculator.IsComplex(): hist.MET['LepWmassComplex'].Fill(p4LepW.M())

    #count again jets
    njets = 0
    for jet in jets:
        if jet.pt>30:
            tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e )
            tmpdeltaR = p4muon.DeltaR(tmpp4Jet)
            if tmpdeltaR < 0.001: continue
            njets += 1

            p4jets.append( TLorentzVector() )
            p4jets[njets-1].SetPtEtaPhiE(jet.pt,jet.eta,jet.phi,jet.e)
            bdisc.append( jet.btag_TCHP)
            
            hist.jets['pt'].Fill( jet.pt)
            hist.jets['eta'].Fill(jet.eta)
            hist.jets['phi'].Fill(jet.phi)

            if njets == 1:
                hist.jets['1_pt'].Fill( jet.pt)
            if njets == 2:
                hist.jets['2_pt'].Fill( jet.pt)
            if njets == 3:
                hist.jets['3_pt'].Fill( jet.pt)
                p4HadTop = TLorentzVector()
                p4HadTop = p4jets[0] + p4jets[1] + p4jets[2]
                hist.M3['3jet'].Fill( p4HadTop.M() )
            if njets >=4:
                hist.jets['4_pt'].Fill( jet.pt)
                hist.MET['LepWmass_4jet'].Fill(p4LepW.M())
                if METzCalculator.IsComplex(): hist.MET['LepWmassComplex_4jet'].Fill(p4LepW.M())
                
    if njets > 0:
        cut['1jet'] += 1
        hist.Mt['Mt_1jet'].Fill( WMt )
    if njets > 1:
        cut['2jet'] += 1
        hist.Mt['Mt_2jet'].Fill( WMt )
    if njets > 2:
        cut['3jet'] += 1
        hist.Mt['Mt_3jet'].Fill( WMt )
    if njets > 3:
        cut['4jet'] += 1
        hist.Mt['Mt_4jet'].Fill( WMt )
        # printout txt file
        line = "-15 "+str(p4muon.Pt())+" "+str(p4muon.Eta())+' '+str(p4muon.Phi())+' 0\n'
        txtfile.write(line)
        line = "-5 "+str(p4MET.Pt())+' 0 '+str(p4MET.Phi())+' 0\n'
        txtfile.write(line)
        ij = 0
        for iijet in p4jets:
            line= str(iijet.E()) +' '+str(iijet.Pt)+' '+str(iijet.Eta())+' '+str(iijet.Phi()) +' '+str(bdisc[ij]) 
            txtfile.write(line+'\n')
            ij += 1
print "done."
print "Cut flow Table"
for key in cutkeys:
    print key + " " + str(cut[key])

cv1 = TCanvas('muon_pt','muon_pt',600,600)
hist.muons['pt'].Draw()
#cv2 = TCanvas('muon_eta','muon_eta',600,600)
#hist.muons['eta'].Draw()

outname = "top_plots_"+dataType+"_"+JetType+".root"
outroot = TFile(outname,"RECREATE")
hist.SetTFile(outroot)
hist.Write()

print "File "+outname+" has been written."
#raw_input ("Enter to quit:")
