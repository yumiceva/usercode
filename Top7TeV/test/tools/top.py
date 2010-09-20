#! /usr/bin/env python

from ROOT import *

import sys
import os
import math

import histograms

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

gROOT.Reset()
# Plot Style
gROOT.ProcessLine('.L tdrstyle.C')
ROOT.setTDRStyle()
gROOT.ProcessLine('.L CMSTopStyle.cc+')
style = CMSTopStyle()
style.setupICHEPv1()
# libraries
gSystem.Load('libYumicevaTop7TeV.so')
gROOT.ProcessLine('.L METzCalculator.cc+')
METzCalculator = METzCalculator()
gROOT.ProcessLine('.L JetCombinatorics.cc+')
gROOT.ProcessLine('.L LoadTLV.C+')

# output txt file
txtfile = file("4jetevents_"+JetType+".txt","w")

# input files

datafilename = "NtupleMaker/ttmuj_data_Sep3.root"
#"/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/1.34pb-1/ttmuj_data_Aug25.root"
if dataType=="TTbar":
    datafilename = "NtupleMaker/v5/ttmuj_TTbar.root"
if dataType=="Wjets":
    datafilename = "NtupleMaker/v5/ttmuj_Wjets.root"
if dataType=="Zjets":
    datafilename = "NtupleMaker/v5/ttmuj_Zjets.root"
if dataType=="QCD":
    datafilename = "NtupleMaker/v5/ttmuj_QCD.root"
if dataType=="STtch":
    datafilename = "NtupleMaker/v5/ttmuj_STtch.root"

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
    vertices = evt.vertices
    muons = evt.muons
    electrons = evt.electrons
    jets  = evt.Calojets
    if JetType == "JPT": jets = evt.JPTjets
    if JetType == "PF": jets = evt.PFjets

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
    nPVs = 0
    nloosemuons = 0
    ntightmuons = 0
    nelec = 0
    njets = 0
    p4muon = TLorentzVector()
    p4jets = []
    bdisc = {}
    bdisc['TCHP'] = []
    bdisc['SSVHE'] = []
    isTagb = {}
    isTagb['TCHPL'] = []
    isTagb['SSVHEM'] = []
    muVz = 0.
    PVz = 0.
    for pv in vertices:
        if nPVs == 0: PVz = pv.vz
        nPVs += 1
    
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
                    if not ApplyDeltaR:
                        ntightmuons += 1
                        p4muon.SetPtEtaPhiE( mu.pt, mu.eta, mu.phi, mu.e )
                        muonVz = mu.vz                                                
                    elif aDeltaR>0.3:
                        ntightmuons += 1
                        p4muon.SetPtEtaPhiE( mu.pt, mu.eta, mu.phi, mu.e )
                        muonVz = mu.vz
    
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
    hist.muons['dz'].Fill( math.fabs(muonVz - PVz))
    Wpt = p4muon.Pt() + p4MET.Pt()
    Wpx = p4muon.Px() + p4MET.Px()
    Wpy = p4muon.Py() + p4MET.Py()
    WMt = math.sqrt(Wpt*Wpt-Wpx*Wpx-Wpy*Wpy)
    hist.Mt['Mt'].Fill( WMt )
    hist.MET['MET'].Fill( p4MET.Pt() )
    hist.MET['phi'].Fill( p4MET.Phi() )
    hist.MET['Ht'].Fill( Ht )
    hist.MET['Htlep'].Fill(p4MET.Pt() + p4muon.Pt())
    
    # estimate Pz of neutrino
    METzCalculator.SetMET(p4MET)
    METzCalculator.SetMuon(p4muon)
    pzNu = METzCalculator.Calculate()
    p4Nu = TLorentzVector()
    p4OtherNu = TLorentzVector()
    p4Nu.SetPxPyPzE(p4MET.Px(), p4MET.Py(), pzNu,math.sqrt(p4MET.Px()*p4MET.Px()+p4MET.Py()*p4MET.Py()+pzNu*pzNu))
    #print "pzNu = " +str(pzNu)
    #print "p4Nu =("+str(p4Nu.Px())+","+str(p4Nu.Py())+","+str(p4Nu.Pz())+","+str(p4Nu.E())
    pzOtherNu = METzCalculator.getOther()
    p4OtherNu.SetPxPyPzE( p4MET.Px(), p4MET.Py(),pzOtherNu,math.sqrt(p4MET.Px()*p4MET.Px()+p4MET.Py()*p4MET.Py()+pzOtherNu*pzOtherNu))
    
    hist.MET['PzNu'].Fill(pzNu) #change this to 2d with two sol and as a function of jets
    p4LepW = p4muon + p4Nu
    p4OtherLepW = p4muon + p4OtherNu
    
    hist.MET['LepWmass'].Fill(p4LepW.M())
    if METzCalculator.IsComplex(): hist.MET['LepWmassComplex'].Fill(p4LepW.M())

    hist.jets['Njets'].Fill(0)
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
            bdisc['TCHP'].append( jet.btag_TCHP)
            bdisc['SSVHE'].append( jet.btag_SSVHE)
            if jet.btag_TCHP > 1.19:
                isTagb['TCHPL'].append(True)
            else:
                isTagb['TCHPL'].append(False)
            if jet.btag_SSVHE > 1.74:
                isTagb['SSVHEM'].append(True)
            else:
                isTagb['SSVHEM'].append(False)
                
            hist.jets['pt'].Fill( jet.pt)
            hist.jets['eta'].Fill(jet.eta)
            hist.jets['phi'].Fill(jet.phi)

            #if njets == 1:
            #hist.jets['1_pt'].Fill( jet.pt)
            #if njets == 2:
            #hist.jets['2_pt'].Fill( jet.pt)
            if njets == 3:
                hist.jets['1st_pt_N3j'].Fill(p4jets[0].Pt())
                hist.jets['2nd_pt_N3j'].Fill(p4jets[1].Pt())
                hist.jets['3th_pt_N3j'].Fill(p4jets[2].Pt())
                p4HadTop = TLorentzVector()
                p4HadTop = p4jets[0] + p4jets[1] + p4jets[2]
                hist.MET['LepWmass_3jet'].Fill(p4LepW.M())
                if METzCalculator.IsComplex(): hist.MET['LepWmassComplex_3jet'].Fill(p4LepW.M())
                hist.M3['3jet'].Fill( p4HadTop.M() )
                ntagjets = 0
                for itag in isTagb['SSVHEM']:
                    if itag: ntagjets += 1
                if ntagjets>0 and njets==3:
                    hist.M3['3jet_SSVHEM_1b'].Fill(p4HadTop.M())
                                            
            if njets >=4:
                hist.jets['1st_pt_N4j'].Fill(p4jets[0].Pt())
                hist.jets['2nd_pt_N4j'].Fill(p4jets[1].Pt())
                hist.jets['3th_pt_N4j'].Fill(p4jets[2].Pt())
                hist.jets['4th_pt_N4j'].Fill(p4jets[3].Pt())
                hist.MET['LepWmass_4jet'].Fill(p4LepW.M())
                if METzCalculator.IsComplex(): hist.MET['LepWmassComplex_4jet'].Fill(p4LepW.M())

    if njets==1:
        hist.jets['Njets'].Fill(1)
    if njets == 2:
        hist.jets['Njets'].Fill(2)
    if njets == 3:
        hist.jets['Njets'].Fill(3)
    if njets >= 4:
        hist.jets['Njets'].Fill(4)
    
    if njets > 0:
        cut['1jet'] += 1
        hist.Mt['Mt_1jet'].Fill( WMt )
    if njets > 1:
        cut['2jet'] += 1
        hist.Mt['Mt_2jet'].Fill( WMt )
    if njets > 2:
        cut['3jet'] += 1
        hist.Mt['Mt_3jet'].Fill( WMt )
        ntagjets = 0
        for itag in isTagb['TCHPL']:
            if itag: ntagjets += 1
        hist.jets['Nbjets_TCHPL_N3j'].Fill(ntagjets)
        ntagjets = 0
        for itag in isTagb['SSVHEM']:
            if itag: ntagjets += 1
        hist.jets['Nbjets_SSVHEM_N3j'].Fill(ntagjets)
        
    if njets > 3:
        cut['4jet'] += 1
        hist.Mt['Mt_4jet'].Fill( WMt )
        ntagjets = 0
        for itag in isTagb['TCHPL']:
            if itag: ntagjets += 1
        hist.jets['Nbjets_TCHPL_N4j'].Fill(ntagjets)
        ntagjets = 0
        for itag in isTagb['SSVHEM']:
            if itag: ntagjets += 1
        hist.jets['Nbjets_SSVHEM_N4j'].Fill(ntagjets)
        if ntagjets > 0:
            hist.M3['4jet_SSVHEM_1b'].Fill(M3_hadTopP4.M())
        # jet combination
        myCombi = JetCombinatorics()
        myCombi.SetLeptonicW(p4LepW)
        if p4OtherNu.E() != 0: myCombi.SetOtherLeptonicW(p4OtherLepW)
        maxNjets = 7
        if njets <= maxNjets: maxNjets = njets
        vectorjets = std.vector('TLorentzVector')(maxNjets)
        for ij in xrange(0,maxNjets):
            vectorjets[ij] = p4jets[ij]
        vectorbjets = std.vector('double')(0) 
        myCombi.FourJetsCombinations(vectorjets,vectorbjets);
        M3Combo = myCombi.GetCombinationSumEt(0);
        M3_hadWP4 = M3Combo.GetHadW();
        M3_hadTopP4 = M3Combo.GetHadTop();
        M3_lepTopP4 = M3Combo.GetLepTop();
        hist.M3['4jet'].Fill(M3_hadTopP4.M())
        # M3Prime
        myCombi.Clear()
        myCombi = JetCombinatorics()
        #top mass constraint
        myCombi.UseMtopConstraint(True);
        # choose sigmas
        #myCombi.SetSigmas(0);
        myCombi.SetLeptonicW(p4LepW)
        if p4OtherNu.E() != 0: myCombi.SetOtherLeptonicW(p4OtherLepW)
        myCombi.FourJetsCombinations(vectorjets, vectorbjets ); #// pass the b-tag dicriminators
        bestCombo = Combo()

        bestCombo = myCombi.GetCombination(0)
        M3p_hadWP4 = bestCombo.GetHadW()
        M3p_hadTopP4 = bestCombo.GetLepTop()
        M3p_lepTopP4 = bestCombo.GetLepTop()
        hist.M3['M3chi2_hadW_4jet'].Fill( M3p_hadWP4.M() )
        hist.M3['M3chi2_hadTop_4jet'].Fill( M3p_hadTopP4.M() )
        hist.M3['M3chi2_hadTop_lepTop_4jet'].Fill( M3p_hadTopP4.M(), M3p_lepTopP4.M() )
        hist.M3['M3chi2_normchi2_1st'].Fill( bestCombo.GetChi2()/3. )
        if M3p_hadTopP4.M() < 50.:
            print "top mass too low"
            print "len vect:"+str(len(vectorjets))
            print "Combo:"
            bestCombo.Print()
        for icc in range(1,4):
            nextCombo = myCombi.GetCombination(icc)
            if icc==1:
                hist.M3['M3chi2_normchi2_2nd'].Fill( nextCombo.GetChi2()/3. )
            if icc==2:
                hist.M3['M3chi2_normchi2_3th'].Fill( nextCombo.GetChi2()/3. )
            if icc==3:
                hist.M3['M3chi2_normchi2_4th'].Fill( nextCombo.GetChi2()/3. )
        
        MttbarP4 = M3p_hadTopP4 + M3p_lepTopP4
        hist.M3['Mttbar_chi2'].Fill(MttbarP4.M())
        # printout txt file with run,lumi,event
        line = str(evt.run)+":"+str(evt.lumi)+":"+str(evt.event)+"\n"
        #txtfile.write(line)
        # printout txt file
        #line = "-15 "+str(p4muon.Pt())+" "+str(p4muon.Eta())+' '+str(p4muon.Phi())+' 0\n'
        #txtfile.write(line)
        #line = "-5 "+str(p4MET.Pt())+' 0 '+str(p4MET.Phi())+' 0\n'
        #txtfile.write(line)
        #ij = 0
        #for iijet in p4jets:
            #line= str(iijet.E()) +' '+str(iijet.Pt)+' '+str(iijet.Eta())+' '+str(iijet.Phi()) +' '+str(bdisc['TCHP'][ij]) 
            #txtfile.write(line+'\n')
            #ij += 1
print "done."
print "Cut flow Table"
for key in cutkeys:
    print key + " " + str(cut[key])

#cv1 = TCanvas('muon_pt','muon_pt',600,600)
#hist.muons['pt'].Draw()
#cv2 = TCanvas('muon_eta','muon_eta',600,600)
#hist.muons['eta'].Draw()

outname = "top_plots_"+dataType+"_"+JetType+".root"
if not ApplyDeltaR:
    outname = "top_plots_"+dataType+"_"+JetType+"_NoDeltaR.root"
outroot = TFile(outname,"RECREATE")
hist.SetTFile(outroot)
hist.Write()

print "File "+outname+" has been written."
#raw_input ("Enter to quit:")
