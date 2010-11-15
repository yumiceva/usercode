#! /usr/bin/env python

"""
   top.py

   Script to create histograms and cut flow table for lepton+jets.
   
   usage: %prog 
   -b, --batch : run in batch mode without graphics.
   -d, --deltaR : enable/dissable deltaR(muon,jet) cut. Default is enable.
   -f, --flavor = FLAVOR: flavor for Vqq samples. 1 = bb, 2 = c(c), 3 = light
   -I, --Initial = INITIAL: initial entry.
   -i, --isolation = ISO: lepton isolation cut.
   -j, --jet = JET: Jet and MET type: calo, JPT, PF
   -J, --JES = JES: JES scale factor.
   -l, --listofruns : write text file with run,lumi,event of those passing the selection.
   -M, --MET = MET: MET threshold.
   -m, --mttbar = MTTBAR: Mttbar cut.
   -N, --Nevents = Nevents: Maximum entry to run.
   -n, --new : write text file with run,lumi,event for Mttbar>900.
   -O, --OutputDir = OUTPUTDIR: name of output directory
   -p, --jetpt = JETPT: jet pT threhold.
   -s, --sample = SAMPLE: Ntuple sample: data, TTbar, Wjets, Zjets, QCD, etc.
   -t, --txt : write text file with events with > 1 jet.
   -v, --verbose : verbose output.
   -w, --wait : Pause script after plotting a new superpositin of histograms.
      
   Francisco Yumiceva (yumiceva@fnal.gov)
   Fermilab 2010
   
"""


from ROOT import *

import sys
import os
import math
import re
import histograms

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
JetType = "PF"
dataType = "data"
dataTypeSuffix = ""
ApplyDeltaR = True
METCut = 20.
verbose = False
MinJetPt = 25.
IsoCut = 0.1
printlistofruns = False
printtxtfile = False
OutputDir = "./"
Flavor = 0
FlavorStr = ""
doReverseIso = False
MttbarCut = 900.
printnewlistofruns = False
JES = 1
InitialEntry = 0
FinalEntry = 0

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

if option.listofruns:
    printlistofruns = True
    print "We will write a file with run,event,lumi information"

if option.txt:
    printtxtfile = True
    print "we will write a txt file with jet kinematics"

if option.deltaR:
    ApplyDeltaR = option.deltaR
    if not ApplyDeltaR: print "deltaR cut will be removed."
    
if option.jet:
    JetType = option.jet
print "jet collection: " + JetType
    
if option.MET:
    METCut = float(option.MET)
print "MET > "+str(METCut)

if option.isolation:
    IsoCut = float(option.isolation)
print "Isolation > " +str(IsoCut)

if option.jetpt:
    MinJetPt = float(option.jetpt)
print "jet pT > "+str(MinJetPt)

if option.JES:
    JES = float(option.JES)
    print "Use JES factor of "+str(JES)
    if JES>1: dataTypeSuffix = "JESup"
    if JES<1: dataTypeSuffix = "JESdown"
if option.sample:
    dataType = option.sample
print "sample: "+dataType

if option.OutputDir:
    OutputDir = option.OutputDir+"/"
print "output directory: " + OutputDir

if option.mttbar:
    MttbarCut = float(option.mttbar)
print "Mttbar cut > "+str(MttbarCut)

if option.new:
    printnewlistofruns = True
    print "Write txt file with events mttbar>900"
    
if option.flavor:
    Flavor = int(option.flavor)
    if Flavor == 1: FlavorStr = "bb"
    if Flavor == 2: FlavorStr = "cc"
    if Flavor == 3: FlavorStr = "light"
    print "Selecting MC sample with flavor "+FlavorStr

if option.Initial:
    InitialEntry = int(option.Initial)
    print "Initial entry = "+str(InitailEntry)
if option.Nevents:
    FinalEntry = int(option.Nevents)
    print "Final entry = "+str(FinalEntry)


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
txtfilename = "2jetevents_"+JetType+"_"+dataType+".txt"
txtfile = None
if printtxtfile:
    txtfile = file(OutputDir+"/"+txtfilename,"w")

runfilename = "2jetList_"+JetType+"_"+dataType+".txt"
runfile= None
if printlistofruns:
    runfile = file(OutputDir+"/"+runfilename,"w")

newrunfilename = "newList_"+JetType+"_"+dataType+".txt"
newrunfile = None
if printnewlistofruns:
    newrunfile = file(OutputDir+"/"+newrunfilename,"w")
        
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
# additioanl counter
N500gev3j = 0
N500gev4j = 0

# Setup TChain
top = TChain('/PATNtupleMaker/top')

# input files
data_repo = "/uscms_data/d3/ttmuj/Documents/NtupleMaker/"
#datafilename = "Data/21.89pb-1/ttmuj_21.89pb-1_Oct29.root"
#datafilename = "/uscmst1b_scratch/lpc1/cmsroc/yumiceva/top_prod_Nov3/TrigC/TrigC.root"

if dataType=="data" or dataType=="dataReverse":
    top.Add(data_repo+"Data/21.89pb-1/ttmuj_21.89pb-1_Oct29.root")
    top.Add(data_repo+"Data/34.72pb-1/ttmuj_12.83pb-1_Nov5.root") #total 34.72pb-1
if dataType=="dataReverse":
    doReverseIso = True
    print "Apply reverse isolation"
if dataType=="TTbar":
    top.Add(data_repo+"MC/V00-01-04-07/TTbar_Mu.root")
if dataType=="sync":
    top.Add("/uscmst1b_scratch/lpc1/cmsroc/yumiceva/top_prod_Oct5/TTJets_syncv4.root")
if dataType=="Wjets":
    top.Add(data_repo+"MC/V00-01-04-07/WJets_Mu.root")
if dataType=="Zjets":
    top.Add(data_repo+"MC/V00-01-04-07/ZJets_Mu.root")
if dataType=="QCD":
    top.Add(data_repo+"MC/V00-01-04-07/QCD_Mu.root")
if dataType=="STtch":
    top.Add(data_repo+"MC/V00-01-04-07/STtch_Mu.root")
if dataType=="STtWch":
    top.Add(data_repo+"MC/V00-01-04-07/STtWch_Mu.root")
if dataType=="Wc":
    top.Add(data_repo+"MC/V00-01-04-07/Wc_Mu.root")
if dataType=="Vqq":
    top.Add(data_repo+"MC/V00-01-04-07/Vqq_Mu.root")
if dataType=="TTbar_matchingup":
    dataType="TTbar"
    dataTypeSuffix = "matchingup"
    top.Add(data_repo+"MC/V00-01-04-07/TTbar_matchingup_Mu.root")
if dataType=="TTbar_matchingdown":
    dataType="TTbar"
    dataTypeSuffix = "matchingdown"
    top.Add(data_repo+"MC/V00-01-04-07/TTbar_matchingdown_Mu.root")
if dataType=="TTbar_scaleup":
    dataType="TTbar"
    dataTypeSuffix = "scaleup"
    top.Add(data_repo+"MC/V00-01-04-07/TTbar_scaleup_Mu.root")
if dataType=="TTbar_scaledown":
    dataType="TTbar"
    dataTypeSuffix = "scaledown"
    top.Add(data_repo+"MC/V00-01-04-07/TTbar_scaledown_Mu.root")
if dataType=="Wjets_matchingup":
    dataType="Wjets"
    dataTypeSuffix = "matchingup"
    top.Add(data_repo+"MC/V00-01-04-07/WJets_matchingup_Mu.root")
if dataType=="Wjets_matchingdown":
    dataType="Wjets"
    dataTypeSuffix = "matchingdown"
    top.Add(data_repo+"MC/V00-01-04-07/WJets_matchingdown_Mu.root")
if dataType=="Wjets_scaleup":
    dataType="Wjets"
    dataTypeSuffix = "scaleup"
    top.Add(data_repo+"MC/V00-01-04-07/WJets_scaleup_Mu.root")
if dataType=="Wjets_scaledown":
    dataType="Wjets"
    dataTypeSuffix = "scaledown"
    top.Add(data_repo+"MC/V00-01-04-07/WJets_scaledown_Mu.root")
if dataType=="Zjets_matchingup":
    dataType="Zjets"
    dataTypeSuffix = "matchingup"
    top.Add(data_repo+"MC/V00-01-04-07/ZJets_matchingup_Mu.root")
if dataType=="Zjets_matchingdown":
    dataType="Zjets"
    dataTypeSuffix = "matchingdown"
    top.Add(data_repo+"MC/V00-01-04-07/ZJets_matchingdown_Mu.root")
if dataType=="Zjets_scaleup":
    dataType="Zjets"
    dataTypeSuffix = "scaleup"
    top.Add(data_repo+"MC/V00-01-04-07/ZJets_scaleup_Mu.root")
if dataType=="Zjets_scaledown":
    dataType="Zjets"
    dataTypeSuffix = "scaledown"
    top.Add(data_repo+"MC/V00-01-04-07/ZJets_scaledown_Mu.root")


#tfile = TFile(data_repo+datafilename)
print "use "+JetType+" collections"
#tfile.cd()


print "files opened:"
tmplistfiles = top.GetListOfFiles()
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
    tmptfile.cd()
    PVHist = gDirectory.Get('/PATNtupleMaker/cutflow')
    if PVHist:
        cutmap['GoodPV'] = PVHist.GetBinContent( 3 )
    tmptfile.Close()
    del(tmptfile)
#del(tmplistfiles)


# get tree
#top = ROOT.gDirectory.Get( '/PATNtupleMaker/top' )

entries = top.GetEntriesFast()

# setup ntuple object
evt = TopEventNtuple()
top.SetBranchAddress('top.', AddressOf(evt))

#cut = {}
#cut['processed'] = cut['OneIsoMuon'] = cut['VetoElectron'] = 0
#cut['1jet'] = cut['2jet'] = cut['3jet'] = cut['4jet'] = 0
#cutkeys = ['processed','NoDeltaR','OneIsoMuon','VetoLooseMu','VetoElectron','1jet','2jet','3jet','4jet']
#for i in cutkeys:
#    cut[i] = 0


# create histograms
hist = histograms.Hist()
hist.Create(dataType+JetType)
histLow = histograms.Hist()
histLow.Create(dataType+JetType+"Low")
histHigh = histograms.Hist()
histHigh.Create(dataType+JetType+"High")

# jet combination
myCombi = JetCombinatorics()
        
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

    #cut['processed'] += 1

    if jentry%50000 == 0:
        print "Processing entry = "+str(jentry)

    # flavor history for MC V+jets
    if dataType=="Wjets" or dataType=="Zjets" or dataType=="Wc" or dataType=="Vqq":
        hist.Various['flavor'].Fill(evt.flavorHistory)
    # check if flavor is requested
    if Flavor != 0:
        passFlavor = False
        
        evtflavor = evt.flavorHistory

        if (dataType=="Wjets" or dataType=="Zjets") and Flavor==1 and evtflavor==5: passFlavor = True
        if (dataType=="Wjets" or dataType=="Zjets") and Flavor==2 and evtflavor==6: passFlavor = True
        if (dataType=="Wjets" or dataType=="Zjets") and Flavor==3 and evtflavor==11: passFlavor = True

        if dataType=="Wc" and Flavor==2 and evtflavor==4: passFlavor = True

        if dataType=="Vqq" and Flavor==1 and (evtflavor==1 or evtflavor==2): passFlavor = True
        if dataType=="Vqq" and Flavor==2 and (evtflavor==3 or evtflavor==4): passFlavor = True

        if not passFlavor:
            continue
        cutmap['Flavor'] += 1
    
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

        # check if reverse isolation requested
        if doReverseIso:

            tmpp4Mu = TLorentzVector()
            tmpp4Jet= TLorentzVector()
            aDeltaR = 999
            for jet in jets:
                
                if JES*jet.pt>MinJetPt:
                    tmpp4Mu.SetPtEtaPhiE(mu.pt, mu.eta, mu.phi, mu.e )
                    tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e )
                    tmpdeltaR = tmpp4Mu.DeltaR(tmpp4Jet)
                    if tmpdeltaR < 0.1 and JetType=="JPT": continue
                    #if tmpdeltaR < aDeltaR and tmpdeltaR>0.01: aDeltaR = tmpdeltaR
                    if tmpdeltaR < aDeltaR: aDeltaR = tmpdeltaR
            del(tmpp4Mu)
            del(tmpp4Jet)
            if mu.pt>20. and math.fabs(mu.eta)<2.1 and mu.IsTrackerMuon==1 and \
               mu.muonhits>0 and mu.normchi2<10 and \
               mu.trackerhits>=11 and mu.muonstations> 1 and \
               mu.pixelhits >= 1 and \
               math.fabs(mu.vz - PVz) < 1. and aDeltaR>0.3 and \
               ( math.fabs(mu.d0)>0.025 or (mu.reliso03<0.7 and mu.reliso03>0.15) ):

                ntightmuons += 1
                p4muon.SetPtEtaPhiE( mu.pt, mu.eta, mu.phi, mu.e )
                hist.muons['reliso'].Fill(mu.reliso03)
                continue                        

        if doReverseIso: continue
        
        # check loose iso muons
        if mu.reliso03<0.2:
            nloosemuons +=1
        if mu.pt>20. and math.fabs(mu.eta)<2.1 and mu.IsTrackerMuon==1:

            hist.muons['pt_cut1'].Fill(mu.pt)
            hist.muons['d0_cut1'].Fill(mu.d0)

            if math.fabs(mu.d0)<0.02 and \
                   mu.muonhits>0 and \
                   mu.normchi2<10 and \
                   mu.trackerhits>=11 and \
                   mu.muonstations> 1 and \
                   mu.pixelhits >= 1:

                hist.muons['pt_cut2'].Fill(mu.pt)
                hist.muons['reliso'].Fill(mu.reliso03)
                
                if mu.reliso03 < IsoCut:

                    muonVz = mu.vz
                    hist.muons['dz'].Fill( math.fabs(muonVz - PVz))
                    
                    if math.fabs(muonVz - PVz) >= 1.:
                        continue
                    
                    tmpp4Mu = TLorentzVector()
                    tmpp4Jet= TLorentzVector()
                    aDeltaR = 999
                    for jet in jets:
                        
                        if JES*jet.pt>MinJetPt:
                            tmpp4Mu.SetPtEtaPhiE(mu.pt, mu.eta, mu.phi, mu.e )
                            tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e )
                            tmpdeltaR = tmpp4Mu.DeltaR(tmpp4Jet)
                            if tmpdeltaR < 0.1 and JetType=="JPT": continue
                            #if tmpdeltaR < aDeltaR and tmpdeltaR>0.01: aDeltaR = tmpdeltaR
                            if tmpdeltaR < aDeltaR: aDeltaR = tmpdeltaR

                    del(tmpp4Mu)
                    del(tmpp4Jet)
                    if aDeltaR < 999: hist.muons['deltaR'].Fill(aDeltaR)
                    
                    if not ApplyDeltaR:
                        ntightmuons += 1
                        p4muon.SetPtEtaPhiE( mu.pt, mu.eta, mu.phi, mu.e )
                    elif aDeltaR>0.3:
                        ntightmuons += 1
                        p4muon.SetPtEtaPhiE( mu.pt, mu.eta, mu.phi, mu.e )
                            
    #print "loop over muons done"
    
    if ntightmuons != 1:
        continue
    cutmap['OneIsoMu'] +=1
    
    if nloosemuons > 1:
        continue
    cutmap['LooseMuVeto'] += 1
    for ele in electrons:
        nelec += 1
    if nelec > 0 : continue
    cutmap['ElectronVeto'] += 1

    # correct MET for effect of JES
    if JES != 1:
        deltaJES = TLorentzVector()
        tmpp4Jet= TLorentzVector()
        for jet in jets:
            tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e )
            deltaJES += (float(JES) - 1.) * tmpp4Jet
            
        p4MET = p4MET - deltaJES
        del(deltaJES)
        del(tmpp4Jet)

    hist.MET['phi_cut0'].Fill( p4MET.Phi() )
    
    # MET cut if requested
    if p4MET.Et() <= METCut:
        continue
    cutmap['MET'] +=1
                
    # inclusive
    hist.muons['pt'].Fill( p4muon.Pt() )
    hist.muons['eta'].Fill( p4muon.Eta() )
    hist.muons['phi'].Fill( p4muon.Phi() )
    #hist.muons['dz'].Fill( math.fabs(muonVz - PVz))
    Wpt = p4muon.Pt() + p4MET.Pt()
    Wpx = p4muon.Px() + p4MET.Px()
    Wpy = p4muon.Py() + p4MET.Py()
    WMt = math.sqrt(Wpt*Wpt-Wpx*Wpx-Wpy*Wpy)
    hist.Mt['Mt'].Fill( WMt )
    hist.MET['MET'].Fill( p4MET.Pt() )
    hist.MET['phi'].Fill( p4MET.Phi() )
    hist.MET['Ht'].Fill( Ht )
    hist.MET['Htlep'].Fill( Ht + p4muon.Pt())
    
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
        if JES*jet.pt>MinJetPt:
            tmpp4Jet = TLorentzVector()
            tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e )
            tmpdeltaR = p4muon.DeltaR(tmpp4Jet)
            if tmpdeltaR < 0.1 and JetType=="JPT": continue
            njets += 1

            p4jets.append( TLorentzVector() )
            p4jets[njets-1].SetPtEtaPhiE(JES*jet.pt,jet.eta,jet.phi,JES*jet.e)
                        
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
                hist.MET['LepWmass_3jet'].Fill(p4LepW.M())
                if METzCalculator.IsComplex(): hist.MET['LepWmassComplex_3jet'].Fill(p4LepW.M())
                ntagjets = 0
                for itag in isTagb['SSVHEM']:
                    if itag: ntagjets += 1
                if ntagjets>0 and njets==3:
                    p4HadTop = TLorentzVector()
                    p4HadTop = p4jets[0] + p4jets[1] + p4jets[2]
                    hist.M3['3jet_SSVHEM_1b'].Fill(p4HadTop.M())
                ntagjets = 0
                for itag in isTagb['TCHPL']:
                    if itag: ntagjets += 1
                if ntagjets>0 and njets==3:
                    p4HadTop = TLorentzVector()
                    p4HadTop = p4jets[0] + p4jets[1] + p4jets[2]
                    hist.M3['3jet_TCHPL_1b'].Fill(p4HadTop.M())
                                
            if njets >=4:
                hist.jets['1st_pt_N4j'].Fill(p4jets[0].Pt())
                hist.jets['2nd_pt_N4j'].Fill(p4jets[1].Pt())
                hist.jets['3th_pt_N4j'].Fill(p4jets[2].Pt())
                hist.jets['4th_pt_N4j'].Fill(p4jets[3].Pt())
                hist.MET['LepWmass_4jet'].Fill(p4LepW.M())
                if METzCalculator.IsComplex(): hist.MET['LepWmassComplex_4jet'].Fill(p4LepW.M())
            del(tmpp4Jet)

    if njets==1:
        cutmap['1Jet'] += 1
        hist.jets['Njets'].Fill(1)
        hist.muons['pt_1jet'].Fill( p4muon.Pt() )
        hist.Mt['Mt_1jet'].Fill( WMt )
        hist.MET['Ht_1jet'].Fill( Ht )
        hist.MET['Htlep_1jet'].Fill( Ht + p4muon.Pt())            
        hist.MET['MET_1jet'].Fill( p4MET.Pt() )
        hist.MET['deltaPhi_1jet'].Fill( p4MET.DeltaPhi( p4muon ) ) 
    if njets == 2:
        cutmap['2Jet'] += 1
        hist.jets['Njets'].Fill(2)
        hist.muons['pt_2jet'].Fill( p4muon.Pt() )
        hist.Mt['Mt_2jet'].Fill( WMt )
        hist.MET['Ht_2jet'].Fill( Ht )
        hist.MET['Htlep_2jet'].Fill( Ht + p4muon.Pt())        
        hist.MET['MET_2jet'].Fill( p4MET.Pt() )
        hist.MET['deltaPhi_2jet'].Fill(p4MET.DeltaPhi(p4muon ) )
    if njets == 3:
        cutmap['3Jet'] += 1
        hist.jets['Njets'].Fill(3)
        hist.muons['pt_3jet'].Fill( p4muon.Pt() )
        hist.Mt['Mt_3jet'].Fill( WMt )
        hist.MET['Ht_3jet'].Fill( Ht )
        hist.MET['Htlep_3jet'].Fill( Ht + p4muon.Pt())        
        hist.MET['MET_3jet'].Fill( p4MET.Pt() )
        hist.MET['deltaPhi_3jet'].Fill(p4MET.DeltaPhi(p4muon ) )
        p4HadTop = TLorentzVector()
        p4HadTop = p4jets[0] + p4jets[1] + p4jets[2]
        hist.M3['3jet'].Fill( p4HadTop.M() )
        if p4HadTop.M() > 500.:
            N500gev3j += 1

        ntagjets = 0
        for itag in isTagb['TCHPL']:
            if itag: ntagjets += 1
        hist.jets['Nbjets_TCHPL_N3j'].Fill(ntagjets)
        if ntagjets > 0:
            hist.M3['3jet_TCHPL_1b'].Fill(p4HadTop.M())
                
        ntagjets = 0
        for itag in isTagb['SSVHEM']:
            if itag: ntagjets += 1
        hist.jets['Nbjets_SSVHEM_N3j'].Fill(ntagjets)
        if ntagjets > 0:
            hist.M3['3jet_SSVHEM_1b'].Fill(p4HadTop.M())
                                                                        
    
    if njets > 1:
        #
        if printtxtfile:
            line = "-15 "+str(p4muon.Pt())+" "+str(p4muon.Eta())+' '+str(p4muon.Phi())+' 0\n'
            txtfile.write(line)
            line = "-5 "+str(p4MET.Pt())+' 0 '+str(p4MET.Phi())+' 0\n'
            txtfile.write(line)
            ij = 0
            for iijet in p4jets:
                line= str(iijet.E()) +' '+str(iijet.Pt())+' '+str(iijet.Eta())+' '+str(iijet.Phi()) +' '+str(bdisc['TCHP'][ij])
                txtfile.write(line+'\n')
                ij += 1
        #
        if printlistofruns:
            line = str(evt.run)+":"+str(evt.lumi)+":"+str(evt.event)+"\n"
            runfile.write(line)
                                            
    if njets > 3:

        maxNjets = 7 # select only the leading 7 jets for combinatorics
        
        ntagjetsTCHPL = 0
        for itag in isTagb['TCHPL']:
            if itag: ntagjetsTCHPL += 1
                    
        ntagjetsSSVHEM = 0
        for itag in isTagb['SSVHEM']:
            if itag: ntagjetsSSVHEM += 1
                                                                                
        # jet combination
        #myCombi = JetCombinatorics()
        myCombi.Clear()
        myCombi.UsebTagging(False)
        myCombi.SetLeptonicW(p4LepW)
        if p4OtherNu.E() != 0: myCombi.SetOtherLeptonicW(p4OtherLepW)
        
        if njets <= maxNjets: maxNjets = njets
        vectorjets = std.vector('TLorentzVector')(maxNjets)
        vectorbjets = std.vector('int')(maxNjets)
        minDeltaRjets = 9999.
        for ij in xrange(0,maxNjets):
            vectorjets[ij] = p4jets[ij]
            vectorbjets[ij] = int(isTagb['TCHPL'][ij])
            if ij < maxNjets - 1:
                tmpdeltaRjets = p4jets[ij].DeltaR( p4jets[ij+1] )
                if tmpdeltaRjets < minDeltaRjets: minDeltaRjets = tmpdeltaRjets
                
        myCombi.FourJetsCombinations(vectorjets,vectorbjets)
        M3Combo = myCombi.GetCombinationSumEt(0);
        M3_hadWP4 = M3Combo.GetHadW();
        M3_hadTopP4 = M3Combo.GetHadTop();
        M3_lepTopP4 = M3Combo.GetLepTop();
        hist.M3['4jet'].Fill(M3_hadTopP4.M())
        if M3_hadTopP4.M() > 500.:
            N500gev4j += 1

        hist.jets['Nbjets_TCHPL_N4j'].Fill(ntagjetsTCHPL)
        if ntagjetsTCHPL > 0:
            hist.M3['4jet_TCHPL_1b'].Fill(M3_hadTopP4.M())

        hist.jets['Nbjets_SSVHEM_N4j'].Fill(ntagjetsSSVHEM)
        if ntagjetsSSVHEM > 0:
            hist.M3['4jet_SSVHEM_1b'].Fill(M3_hadTopP4.M())
                        
                                
        # M3Prime
        myCombi.Clear()
        #del(myCombi)
        #myCombi = JetCombinatorics()
        #top mass constraint
        myCombi.UsebTagging(False)
        myCombi.UseMtopConstraint(True)
        # choose sigmas
        #myCombi.SetSigmas(0);
        myCombi.SetLeptonicW(p4LepW)
        if p4OtherNu.E() != 0: myCombi.SetOtherLeptonicW(p4OtherLepW)
        myCombi.FourJetsCombinations(vectorjets, vectorbjets ) # 
        bestCombo = Combo()

        bestCombo = myCombi.GetCombination(0)
        M3p_hadWP4 = bestCombo.GetHadW()
        M3p_hadTopP4 = bestCombo.GetHadTop()
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
        hist.M3['pt_ttbar'].Fill(MttbarP4.Pt())
        hist.M3['lepTop_vs_Mttbar'].Fill( M3p_lepTopP4.M(), MttbarP4.M() )
        hist.M3['pt_vs_Mttbar'].Fill( MttbarP4.Pt(), MttbarP4.M() )
        
        # printout txt file with run,lumi,event
        if printnewlistofruns and MttbarP4.M()>MttbarCut:
            line = str(evt.run)+":"+str(evt.lumi)+":"+str(evt.event)+"\n"
            newrunfile.write(line)

        # inclusive
        cutmap['4Jet'] += 1
        hist.jets['Njets'].Fill(4)
        hist.Mt['Mt_4jet'].Fill( WMt )
        hist.MET['Ht_4jet'].Fill( Ht )
        hist.MET['Htlep_4jet'].Fill( Ht + p4muon.Pt())
        hist.MET['MET_4jet'].Fill( p4MET.Pt() )
        hist.MET['deltaPhi_4jet'].Fill(p4MET.DeltaPhi(p4muon ) )
        hist.muons['pt_4jet'].Fill( p4muon.Pt() )
                                                                        
        # apply b-tagging
        # M3Prime
        myCombi.Clear()
        #del(myCombi)
        #myCombi = JetCombinatorics()
        #top mass constraint
        #myCombi.Verbose()
        myCombi.UseMtopConstraint(True)
        myCombi.UsebTagging()
        myCombi.SetLeptonicW(p4LepW)
        if p4OtherNu.E() != 0: myCombi.SetOtherLeptonicW(p4OtherLepW)
        myCombi.FourJetsCombinations(vectorjets, vectorbjets ) # pass the b-tag dicriminators
        bestCombo = Combo()
        
        bestCombo = myCombi.GetCombination(0)
        M3p_hadWP4 = bestCombo.GetHadW()
        M3p_hadTopP4 = bestCombo.GetHadTop()
        M3p_lepTopP4 = bestCombo.GetLepTop()
        MttbarP4 = M3p_hadTopP4 + M3p_lepTopP4
        hist.M3['Mttbar_chi2_1btag'].Fill(MttbarP4.M())
        #hist.M3['pt_ttbar'].Fill(MttbarP4.Pt())
        #hist.M3['lepTop_vs_Mttbar'].Fill( M3p_lepTopP4.M(), MttbarP4.M() )
        hist.M3['pt_vs_Mttbar_1btag'].Fill( MttbarP4.Pt(), MttbarP4.M() )

        # Cut on Mttbar
        if MttbarP4.M() > MttbarCut:
            histHigh.muons['pt_4jet'].Fill( p4muon.Pt() )
            histHigh.muons['eta_4jet'].Fill( p4muon.Eta() )
            histHigh.muons['phi_4jet'].Fill( p4muon.Phi() )
            histHigh.jets['Njets'].Fill( njets )
            histHigh.MET['MET_4jet'].Fill( p4MET.Pt() )
            #histHigh.electrons['pt_4jet'].Fill( p4electron.Pt() )
            histHigh.MET['Ht_4jet'].Fill( Ht )
            histHigh.MET['Htlep_4jet'].Fill( Ht + p4muon.Pt() )
            histHigh.jets['jetdeltaR'].Fill( minDeltaRjets )
        else:
            histLow.muons['pt_4jet'].Fill( p4muon.Pt() )
            histLow.muons['eta_4jet'].Fill( p4muon.Eta() )
            histLow.muons['phi_4jet'].Fill( p4muon.Phi() )                        
            histLow.jets['Njets'].Fill( njets )
            histLow.MET['MET_4jet'].Fill( p4MET.Pt() )
            #histLow.electrons['pt_4jet'].Fill( p4electron.Pt() )
            histLow.MET['Ht_4jet'].Fill( Ht )
            histLow.MET['Htlep_4jet'].Fill( Ht + p4muon.Pt() )
            histLow.jets['jetdeltaR'].Fill( minDeltaRjets )
        myCombi.Clear()
        #del(myCombi)
        del(vectorjets)
        del(vectorbjets)
    
    del(p4jets)                                                                

print "done."
print "M3 3jets events with > 500 GeV: "+str(N500gev3j)
print "M3 4jets events with > 500 GeV: "+str(N500gev4j)

print "Cut flow Table"
cutmapkeys =[ "CleanFilters","HLT","GoodPV","OneIsoMu","LooseMuVeto","ElectronVeto","MET","1Jet","2Jet","3Jet","4Jet"]
if Flavor != 0:
    cutmapkeys =[ "CleanFilters","HLT","GoodPV","Flavor","OneIsoMu","LooseMuVeto","ElectronVeto","MET","1Jet","2Jet","3Jet","4Jet"]
    
for key in cutmapkeys:
    print key + " " + str(cutmap[key])

txtname = "cutflow_"+JetType+"_"+dataType+".txt"
if dataTypeSuffix != "":
    txtname = "cutflow_"+JetType+"_"+dataType+"_"+dataTypeSuffix+".txt"
if Flavor != 0:
    txtname = "cutflow_"+JetType+"_"+dataType+"_"+FlavorStr+".txt"
    if dataTypeSuffix !="":
        txtname = "cutflow_"+JetType+"_"+dataType+"_"+dataTypeSuffix+"_"+FlavorStr+".txt"

filecut = open(OutputDir+txtname,"w")
for key in cutmapkeys:
    filecut.write(key + " " + str(cutmap[key])+"\n")
filecut.close()
print "cut flow save in file "+txtname

#cv1 = TCanvas('muon_pt','muon_pt',600,600)
#hist.muons['pt'].Draw()
#cv2 = TCanvas('muon_eta','muon_eta',600,600)
#hist.muons['eta'].Draw()

outname = "top_plots_"+dataType+"_"+JetType+".root"
if dataTypeSuffix !="":
    outname = "top_plots_"+dataType+"_"+dataTypeSuffix+"_"+JetType+".root"
if Flavor != 0:
    outname = "top_plots_"+dataType+"_"+JetType+"_"+FlavorStr+".root"
    if dataTypeSuffix !="":
        outname = "top_plots_"+dataType+"_"+dataTypeSuffix+"_"+FlavorStr+"_"+JetType+".root"

if not ApplyDeltaR:
    outname = "top_plots_"+dataType+"_"+JetType+"_NoDeltaR.root"
outroot = TFile(OutputDir+"/"+outname,"RECREATE")
hist.SetTFile(outroot)
hist.Write()
outroot.Close()

outrootHigh = TFile(OutputDir+"/High_"+outname,"RECREATE")
histHigh.SetTFile(outrootHigh)
histHigh.Write()
outrootHigh.Close()

outrootLow = TFile(OutputDir+"/Low_"+outname,"RECREATE")
histLow.SetTFile(outrootLow)
histLow.Write()
outrootLow.Close()


print "File "+outname+" has been written."

if printtxtfile:
    print "flat file "+txtfilename +" has been written."
if printlistofruns:
    print "flat file "+runfilename +" has been written."
    
#raw_input ("Enter to quit:")
