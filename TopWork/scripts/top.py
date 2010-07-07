#! /usr/bin/env python

import ROOT
from ROOT import gROOT, TFile, TCanvas, TH1F, TH1I, TLegend, TH2F, gPad

import sys,os, math
sys.path.append( os.environ['HOME'])
from RooAlias import *

gROOT.Reset()
# defined style
SetStyle()

# define histos
histo = {}
histo['njets'] = TH1I("njets","njets",6,0,6)
histo['mu_pt'] = TH1F("mu_pt","mu_pt",20,10,80)
histo['mu_pt_QCDMC'] = TH1F("mu_pt_QCDMC","mu_pt_QCDMC",20,10,80)
histo['mu_pt_QCDdata'] = TH1F("mu_pt_QCDdata","mu_pt_QCDdata",20,10,80)

histo['mu_reliso'] = TH1F("mu_reliso","mu_reliso",40,0.,1.05)
histo['mu_d0'] = TH1F("mu_d0","mu_d0",40,0,0.3)
histo['mu_d0_QCDMC'] = TH1F("mu_d0_QCDMC","mu_d0_QCDMC",40,0,0.3)
histo['mu_d0_WMC'] = TH1F("mu_d0_WMC","mu_d0_WMC",40,0,0.3)
histo['d0_vs_reliso'] = TH2F("d0_vs_reliso","d0_vs_reliso",40,0,1,40,0,0.11)
histo['W_mt_data'] = TH1F("W_mt","W_mt",25,0,160)
histo['W_mt_QCDdata'] = TH1F("W_mt_QCDdata","W_mt_QCDdata",25,0,160)
histo['W_mt_QCDMC'] = TH1F("W_mt_QCDMC","W_mt_QCDMC",25,0,160)
histo['W_mt_WMC'] = TH1F("W_mt_WMC","W_mt_WMC",25,0,160)
histo['met_data'] = TH1F("met_data","met_data",25,0,160)
histo['met_QCDMC'] = TH1F("met_QCDMC","met_QCDMC",25,0,160)
histo['met_WMC'] = TH1F("met_WMC","met_WMC",25,0,160)

for key in histo.keys():
    if key!="d0_vs_reliso":
	histo[key].Sumw2()

# input files
tfile = TFile("ABCD_data_new.root")
tfile.cd()

fchain = ROOT.gDirectory.Get( 'top' )
entries = fchain.GetEntriesFast()

Ndata = 0
Ndata50 = 0
Ndata50_pt20 = 0

mu_pt_cut = 20.
isTrkMuon_cut = 1.

Nqcd = 4. #322. 
Nqcderr = 3. #73. 
Nsig = 45. #46. #61.


for jentry in xrange( entries ):
    # get the next tree in the chain
    ientry = fchain.LoadTree(jentry)
    if ientry < 0:
	break

    # verify file/tree/chain integrity
    nb = fchain.GetEntry( jentry )
    if nb <= 0 or not hasattr( fchain, 'njets' ):
	continue

    # histogram main vertex
    histo['njets'].Fill( fchain.njets )

    # get number of muons candidates
    #n_mu = int(fchain.n_mu)
    
    mu_pt = float(fchain.muon_pt)
    mu_reliso = float(fchain.muon_old_reliso)
    mu_d0 = math.fabs( float(fchain.muon_d0) )
    mu_d0Err = float(fchain.muon_d0Error)
    isTrkMu = int(fchain.TrackerMu)
    met = float( fchain.met )
    deltaR = float ( fchain.muon_jet_dr )
    muHits = float ( fchain.muon_muonhits)
    trkHits = float( fchain.muon_trackerhits)
    chi2 = float ( fchain.muon_chi2 )
    eta = math.fabs( float(fchain.muon_eta) )
    #histo['mu_d0'].Fill( mu_d0 )
    #histo['mu_pt'].Fill( mu_pt )
    histo['mu_reliso'].Fill( mu_reliso )

    if isTrkMu==1:
	histo['d0_vs_reliso'].Fill(mu_reliso, mu_d0 )
    
    if mu_reliso > 0.95 and  mu_d0<0.02:
	histo['mu_pt'].Fill( mu_pt )

    passV2 = False
    if mu_reliso > 0.95 and mu_d0<0.02 and mu_pt>20. and isTrkMu==1 and deltaR>0.3 and muHits>0 and trkHits>10 and chi2<10 and eta<2.1:
	passV2 = True

    if passV2:
    #if mu_reliso > 0.95 and  mu_d0<0.02 and mu_pt>mu_pt_cut:
# and met>5:
	Ndata += 1
	histo['W_mt_data'].Fill( fchain.w_mt )
	histo['met_data'].Fill( fchain.met )
	if fchain.w_mt > 50. :
	    Ndata50 += 1
	    if mu_pt>20: Ndata50_pt20 += 1

    if mu_reliso < 0.95 and mu_d0<0.02:
	histo['mu_pt_QCDdata'].Fill( mu_pt )
    # reverse isolation .7, 0.95
    if mu_reliso < 0.95 and mu_d0<0.02  and mu_pt>mu_pt_cut:
# and met>5:
	histo['W_mt_QCDdata'].Fill( fchain.w_mt )
	histo['mu_d0'].Fill( mu_d0 )
	

    #for k in range( nc ):
       # check e-ID flag, 0:e, >=1:jet
    #     if fchain.eg_IsEM[k] != 0:
    #        continue

       # load properties into an electron candidate data object
    #ecan.match  = fchain.eg_trkmatch_X[k]
    #     ecan.PtInvV = fchain.eg_trkpTV_X[k]
    #     ecan.Et     = fchain.eg_et[k]
    #     ecan.eta    = fchain.eg_eta[k]
    #     ecan.phi    = fchain.eg_phi[k]


# define canvas and make plots
cv = {}

ytitle = "Number of Events"

cv['njets'] = TCanvas("njets","njets",600,600)
histo['njets'].SetYTitle(ytitle)
histo['njets'].SetXTitle("number of jets")
histo['njets'].Draw("p")

cv['mu_pt'] = TCanvas("mu_pt","mu_pt",600,600)
histo['mu_pt'].SetYTitle(ytitle)
histo['mu_pt'].SetXTitle("muon p_{T} [GeV]")
histo['mu_pt'].Draw("p")

cv['mu_reliso'] = TCanvas("mu_reliso","mu_reliso",600,600)
histo['mu_reliso'].SetYTitle(ytitle)
histo['mu_reliso'].SetXTitle("muon Relative Isolation")
histo['mu_reliso'].Draw("p")

cv['d0_vs_reliso'] = TCanvas("d0_vs_reliso","d0_vs_reliso",600,600)
histo['d0_vs_reliso'].SetYTitle("absolute impact parameter [cm]")
histo['d0_vs_reliso'].SetXTitle("muon Relative Isolation")
histo['d0_vs_reliso'].Draw("box")
aleg = TLegend(0.4,0.5,0.8,0.7)
aleg.SetMargin(0.12)
aleg.SetTextSize(0.035)
aleg.SetFillColor(10)
aleg.SetBorderSize(0)
aleg.SetHeader("CMS Preliminary #sqrt{s}=7 TeV")
aleg.AddEntry(histo['d0_vs_reliso'],'data L = 11.4 nb^{-1}','p')
aleg.Draw()

cv['W_mt'] = TCanvas("W_mt","W_mt",600,600)
histo['W_mt_data'].SetYTitle(ytitle)
histo['W_mt_data'].SetXTitle("W transverse mass [GeV/c^{2}]")
histo['W_mt_data'].Draw("p")
histo['W_mt_QCDdata'].SetLineColor(4)
histo['W_mt_QCDdata'].SetMarkerColor(4)
ntotal = histo['W_mt_QCDdata'].Integral()
histo['W_mt_QCDdataP'] = histo['W_mt_QCDdata'].Clone("W_mt_QCDdataP")
histo['W_mt_QCDdataM'] = histo['W_mt_QCDdata'].Clone("W_mt_QCDdataM")

histo['W_mt_QCDdata'].Scale(Nqcd/ntotal)
histo['W_mt_QCDdataP'].Scale( (Nqcd+Nqcderr)/ntotal)
histo['W_mt_QCDdataM'].Scale( (Nqcd+Nqcderr)/ntotal)
histo['W_mt_QCDdataErr'] = histo['W_mt_QCDdata'].Clone("W_mt_QCDdataErr")
for ibin in range(0,histo['W_mt_QCDdata'].GetNbinsX()):
    error = histo['W_mt_QCDdataP'].GetBinContent(ibin) - histo['W_mt_QCDdata'].GetBinContent(ibin)
    histo['W_mt_QCDdataErr'].SetBinError( ibin, error )
histo['W_mt_QCDdata'].SetFillColor(4)
histo['W_mt_QCDdata'].Draw("histo same")
histo['W_mt_QCDdataErr'].SetLineColor(2)
histo['W_mt_QCDdataErr'].SetFillColor(2)
histo['W_mt_QCDdataErr'].SetFillStyle(3013)
histo['W_mt_QCDdataErr'].Draw("E3 same")
#tmphisto = histo['W_mt_QCDdata'].DrawClone("E4 same")
#tmphisto.SetFillColor(4)
#histo['W_mt_QCDdata'].Draw("E4 same")
histo['W_mt_data'].Draw("P same")
aleg = TLegend(0.4,0.5,0.8,0.7)
aleg.SetMargin(0.12)
aleg.SetTextSize(0.035)
aleg.SetFillColor(10)
aleg.SetBorderSize(0)
aleg.SetHeader("CMS Preliminary #sqrt{s}=7 TeV")
aleg.AddEntry(histo['W_mt_data'],'data L = 11.4 nb^{-1}','p')
aleg.AddEntry(histo['W_mt_QCDdata'],'background data-driven','f')
aleg.AddEntry(histo['W_mt_QCDdataErr'],'background data-driven error','f')
aleg.Draw()
ROOT.gPad.RedrawAxis()

#cv['W_mt'].Print("qcd_data-driven.png")

Ndata = histo['W_mt_data'].Integral()
#Ndata50 = histo['W_mt_data'].Integral()

print "data events = " + str(Ndata)
print "data events mt>50 = " +str(Ndata50)
print "data events mt>50 and pt>20 = " + str(Ndata50_pt20)

raw_input ("Enter to quit:")

histo['W_mt_QCDdataErr'].SetDirectory(0)

#################################################
print "read ppMuX MC sample"
#tfile = TFile("ABCD_ppMuX_new.root")
tfile = TFile("ppMuX_corrected.root")
tfile.cd()

fchain = ROOT.gDirectory.Get( 'top' )
entries = fchain.GetEntriesFast()

for jentry in xrange( entries ):
    # get the next tree in the chain
    ientry = fchain.LoadTree(jentry)
    if ientry < 0:
	break

    # verify file/tree/chain integrity
    nb = fchain.GetEntry( jentry )
    if nb <= 0 or not hasattr( fchain, 'njets' ):
	continue

    # histogram main vertex
    #histo['njets'].Fill( fchain.njets )

    # get number of muons candidates
    #n_mu = int(fchain.n_mu)
    
    mu_pt = float(fchain.muon_pt)
    mu_reliso = float(fchain.muon_old_reliso)
    mu_d0 = math.fabs( float(fchain.muon_d0) )
    mu_d0Err = float(fchain.muon_d0Error)
    isTrkMu = int(fchain.TrackerMu)
    met = float( fchain.met )
    deltaR = float ( fchain.muon_jet_dr )
    muHits = float ( fchain.muon_muonhits)
    trkHits = float( fchain.muon_trackerhits)
    chi2 = float ( fchain.muon_chi2 )
    eta = math.fabs( float(fchain.muon_eta) )
    #histo['mu_d0_QCDMC'].Fill( mu_d0 )
    #histo['mu_pt'].Fill( mu_pt )
    #histo['mu_reliso'].Fill( mu_reliso )
    #histo['d0_vs_reliso'].Fill(mu_reliso, mu_d0 )
    
    passV2 = False
    if mu_reliso > 0.95 and mu_d0<0.02 and mu_pt>20. and isTrkMu==1 and deltaR>0.3 and muHits>0 and trkHits>10 and chi2<10 and eta<2.1:
	passV2 = True
    
    if passV2:
    #if mu_reliso > 0.95 and  mu_d0<0.02 and mu_pt>mu_pt_cut:
#and met>5.:
	histo['W_mt_QCDMC'].Fill( fchain.w_mt )
	histo['met_QCDMC'].Fill( fchain.met )
	histo['mu_d0_QCDMC'].Fill( mu_d0 )
	histo['mu_pt_QCDMC'].Fill( mu_pt )

#####################################################
print "read W MC sample"
tfile = TFile("W_corrected.root")
tfile.cd()

fchain = ROOT.gDirectory.Get( 'top' )
entries = fchain.GetEntriesFast()

for jentry in xrange( entries ):
    # get the next tree in the chain
    ientry = fchain.LoadTree(jentry)
    if ientry < 0:
	break

    # verify file/tree/chain integrity
    nb = fchain.GetEntry( jentry )
    if nb <= 0 or not hasattr( fchain, 'njets' ):
	continue

    # histogram main vertex
    #histo['njets'].Fill( fchain.njets )

    # get number of muons candidates
    #n_mu = int(fchain.n_mu)
    
    mu_pt = float(fchain.muon_pt)
    mu_reliso = float(fchain.muon_old_reliso)
    mu_d0 = math.fabs( float(fchain.muon_d0) )
    mu_d0Err = float(fchain.muon_d0Error)
    isTrkMu = int(fchain.TrackerMu)
    met = float( fchain.met )
    deltaR = float ( fchain.muon_jet_dr )
    muHits = float ( fchain.muon_muonhits)
    trkHits = float( fchain.muon_trackerhits)
    chi2 = float ( fchain.muon_chi2 )
    eta = math.fabs( float(fchain.muon_eta) )
    #histo['mu_d0_QCDMC'].Fill( mu_d0 )
    #histo['mu_pt'].Fill( mu_pt )
    #histo['mu_reliso'].Fill( mu_reliso )
    #histo['d0_vs_reliso'].Fill(mu_reliso, mu_d0 )
    
    passV2 = False
    if mu_reliso > 0.95 and mu_d0<0.02 and mu_pt>20. and isTrkMu==1 and deltaR>0.3 and muHits>0 and trkHits>10 and chi2<10 and eta<2.1:
	passV2 = True

    if passV2:
    #if mu_reliso > 0.95 and  mu_d0<0.02 and mu_pt>mu_pt_cut:
# and met>5.:
	histo['W_mt_WMC'].Fill( fchain.w_mt )
	histo['met_WMC'].Fill( fchain.met )
	histo['mu_d0_WMC'].Fill( mu_d0 )



#W_weight = (8.525*10e7 * 1.e3 )/10418911.
QCD_weight = (85254. )/8000000. #nb
#QCD_weight = (28000. * 1.e3 )/10068895.
#QCD_weight = (28000. * 1.e3 )/7189041.
W_weight = (2.8 )/7000000. #nb

ntotal = histo['W_mt_WMC'].Integral()
histo['W_mt_WMC'].Scale(W_weight*11.4)
ntotal = histo['W_mt_WMC'].Integral()
print "total MC W events for lumi "+str(ntotal)

cv['W_mtCompare'] = TCanvas("W_mtCompare","W_mtCompare",600,600)
histo['W_mt_QCDMC'].SetYTitle("Arbitrary Units")
histo['W_mt_QCDMC'].SetXTitle("W transverse mass [GeV/c^{2}]")

ntotal = histo['W_mt_QCDMC'].Integral()
#histo['W_mt_QCDMC'].Scale(1./ntotal)
histo['W_mt_QCDMC'].Scale(QCD_weight*11.4)
ntotal = histo['W_mt_QCDMC'].Integral()
print "total MC QCD events for lumi"+str(ntotal)

histo['W_mt_QCDMC'].Scale(1./ntotal)
ntotal = histo['W_mt_QCDdata'].Integral()
histo['W_mt_QCDdata'].Scale(1./ntotal)
histo['W_mt_QCDMC'].Draw()
histo['W_mt_QCDdata'].Draw("same")
aleg = TLegend(0.4,0.5,0.8,0.7)
aleg.SetFillColor(10)
aleg.SetBorderSize(0)
aleg.SetHeader("CMS Preliminary #sqrt{s}=7 TeV")
aleg.AddEntry(histo['W_mt_QCDMC'],'QCD MC','p')
aleg.AddEntry(histo['W_mt_QCDdata'],'data-driven shape','p')
aleg.Draw()
ROOT.gPad.RedrawAxis()
raw_input ("Enter to quit:")

cv['mu_ptCompare'] = TCanvas("mu_ptCompare","mu_ptCompare",600,600)
ntotal = histo['mu_pt_QCDMC'].Integral()
histo['mu_pt_QCDMC'].Scale(1./ntotal)
ntotal = histo['mu_pt_QCDdata'].Integral()
histo['mu_pt_QCDdata'].Scale(1./ntotal)
histo['mu_pt_QCDMC'].Draw()
histo['mu_pt_QCDdata'].Draw("same")

cv['mu_d0'] = TCanvas("mu_d0","mu_d0",600,600)
histo['mu_d0'].SetYTitle(ytitle)
histo['mu_d0'].SetXTitle("muon absolute impact parameter [cm]")
ntotal = histo['mu_d0'].Integral()
histo['mu_d0'].Scale(1./ntotal)
ntotal = histo['mu_d0_QCDMC'].Integral()
histo['mu_d0_QCDMC'].Scale(1./ntotal)
histo['mu_d0'].Draw()
histo['mu_d0_QCDMC'].SetLineColor(2)
histo['mu_d0_QCDMC'].SetMarkerColor(2)
histo['mu_d0_QCDMC'].Draw("same")

cv['W_mt_withWMC'] = TCanvas("W_mt_withWMC","W_mt_withWMC",600,600)
ntotal = histo['W_mt_WMC'].Integral()
# signal
histo['W_mt_WMC'].Scale(Nsig/ntotal)

histo['W_mt_QCDdata'].Scale(Nqcd)
histo['W_mt_WMC'].Add(histo['W_mt_QCDdata']) # add qcd to make stack plot
histo['W_mt_WMC'].SetLineColor(400)
histo['W_mt_WMC'].SetFillColor(400)
histo['W_mt_WMC'].SetYTitle( ytitle )
histo['W_mt_WMC'].SetXTitle("W transverse mass [GeV/c^{2}]")
histo['W_mt_WMC'].Draw("hist")
histo['W_mt_QCDdata'].Draw("hist same")
histo['W_mt_QCDdataErr'].Draw("E3 same")
histo['W_mt_data'].Draw("p same")
aleg = TLegend(0.4,0.5,0.8,0.7)
aleg.SetFillColor(10)
aleg.SetBorderSize(0)
aleg.SetHeader("CMS Preliminary #sqrt{s}=7 TeV")
aleg.AddEntry(histo['W_mt_data'],'data L = 11.4 nb^{-1}','p')
aleg.AddEntry(histo['W_mt_QCDdata'],'background data-driven','f')
aleg.AddEntry(histo['W_mt_QCDdataErr'],'background data-driven error','f')
aleg.AddEntry(histo['W_mt_WMC'],'W+jets MC','f')
aleg.Draw()
ROOT.gPad.RedrawAxis()
raw_input ("Enter to quit:")
