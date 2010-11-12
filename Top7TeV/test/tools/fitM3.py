#! /usr/bin/env python

"""
   fit.py

   Script for template fits
   
   usage: %prog 
   -b, --batch : run in batch mode without graphics.
   -Q, --QCDMC : Use MC QCD shape for templates.
   -w, --wait : Pause script after plotting a new superpositin of histograms.
      
   Francisco Yumiceva (yumiceva@fnal.gov)
   Fermilab 2010
   
"""

from ROOT import *
from ROOT import RooFit

import sys
import os
import math
import re

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


### ROOT style
style = None
if os.path.isfile('tdrstyle.C'):
    gROOT.ProcessLine('.L tdrstyle.C')
    ROOT.setTDRStyle()
    print "Found tdrstyle.C file, using this style."
    
    if os.path.isfile('CMSTopStyle.cc'):
        gROOT.ProcessLine('.L CMSTopStyle.cc+')
        style = CMSTopStyle()
        style.setupICHEPv1()
        print "Found CMSTopStyle.cc file, use TOP style if requested in xml file."


# check options
option,args = parse(__doc__)


UseQCDMCShape = False

if option.batch:
    ROOT.gROOT.SetBatch()
    print "running ROOT in batch mode."

if option.QCDMC:
    UseQCDMCShape = True
    print "use QCD MC shape."

# labels
labels = ["data", "dataReverse", "TTbar", "Wjets", "Zjets", "QCD", "STtch","STtWch"]

# scale
Lumi = 35.
scale = {}
scale["data"] = 1.
scale["dataReverse"] = 1.
scale["TTbar"] = 0.0001061747170696587
scale["Wjets"] = 0.0031099738352619627 
scale["Zjets"] = 0.0028094211467931765
scale["QCD"] = 0.018205299430890203
scale["STtch"] = 3.9595681365436169e-05
scale["STtWch"] = 2.2725469891968262e-05

eff = 0.

# read histograms
histFile = {}
histName = {}

for ilabel in labels:

    histFile[ilabel] = TFile("res_35pb-1/PF/top_plots_"+ilabel+"_PF.root")
    histName["M3_3jet_"+ilabel] = gDirectory.Get("/M3/M3_3jet_"+ilabel+"PF")
    histName["M3_4jet_"+ilabel] = gDirectory.Get("/M3/M3_4jet_"+ilabel+"PF")
    histName["Njets_"+ilabel] = gDirectory.Get("/jets/Njets_"+ilabel+"PF")

    if ilabel=="TTbar":
        eff = (histName["M3_3jet_TTbar"].Integral(-1,-1)+histName["M3_4jet_TTbar"].Integral(-1,-1))/1483404.

    if ilabel!="data" and ilabel!="dataReverse":
        scale[ilabel] = scale[ilabel] * Lumi
        #print ilabel
        #print str(scale[ilabel])
        histName["M3_3jet_"+ilabel].Scale(scale[ilabel])
        histName["M3_4jet_"+ilabel].Scale(scale[ilabel])
        histName["Njets_"+ilabel].Scale(scale[ilabel])

# scale data reverse (antisolation) with MM predictions
NQCDMC_3jet = histName["M3_3jet_QCD"].Integral(-1,-1)
NQCDMC_4jet = histName["M3_4jet_QCD"].Integral(-1,-1)
NQCD_3jet = 4.8*histName["M3_3jet_QCD"].Integral(-1,-1)
NQCD_4jet = 8.8*histName["M3_4jet_QCD"].Integral(-1,-1)

histName["M3_3jet_dataReverse"].Scale(NQCD_3jet/histName["M3_3jet_dataReverse"].Integral(-1,-1))
histName["M3_4jet_dataReverse"].Scale(NQCD_4jet/histName["M3_4jet_dataReverse"].Integral(-1,-1))
histName["Njets_dataReverse"].Scale((NQCD_3jet+NQCD_4jet)/histName["Njets_dataReverse"].Integral(-1,-1))

histName["M3_3jet_QCD"].Scale(NQCD_3jet/histName["M3_3jet_QCD"].Integral(-1,-1))
histName["M3_4jet_QCD"].Scale(NQCD_4jet/histName["M3_4jet_QCD"].Integral(-1,-1))
histName["Njets_QCD"].Scale((NQCD_3jet+NQCD_4jet)/histName["Njets_QCD"].Integral(-1,-1))

# combine ST MC samples
histName["M3_3jet_ST"] = histName["M3_3jet_STtch"].Clone("M3_3jet_ST")
histName["M3_4jet_ST"] = histName["M3_4jet_STtch"].Clone("M3_4jet_ST")
histName["Njets_ST"] = histName["Njets_STtch"].Clone("Njets_ST")
histName["M3_3jet_ST"].Add(histName["M3_3jet_STtWch"])
histName["M3_4jet_ST"].Add(histName["M3_4jet_STtWch"])
histName["Njets_ST"].Add(histName["Njets_STtWch"])

# print yields
print "Yields for 3jet sample:"
print "TTbar = "+str(histName["M3_3jet_TTbar"].Integral(-1,-1))
print "Wjets = "+str(histName["M3_3jet_Wjets"].Integral(-1,-1))
print "Zjets = "+str(histName["M3_3jet_Zjets"].Integral(-1,-1))
print "QCD   = "+str(histName["M3_3jet_QCD"].Integral(-1,-1))
print "ST    = "+str(histName["M3_3jet_ST"].Integral(-1,-1))
print "Rev.  = "+str(histName["M3_3jet_dataReverse"].Integral(-1,-1))
print "data  = "+str(histName["M3_3jet_data"].Integral(-1,-1))
print "Yields for 4jet sample:"
print "TTbar = "+str(histName["M3_4jet_TTbar"].Integral(-1,-1))
print "Wjets = "+str(histName["M3_4jet_Wjets"].Integral(-1,-1))
print "Zjets = "+str(histName["M3_4jet_Zjets"].Integral(-1,-1))
print "QCD   = "+str(histName["M3_4jet_QCD"].Integral(-1,-1))
print "ST    = "+str(histName["M3_4jet_ST"].Integral(-1,-1))
print "Rev.  = "+str(histName["M3_4jet_dataReverse"].Integral(-1,-1))
print "data  = "+str(histName["M3_4jet_data"].Integral(-1,-1))



Nbins = histName["M3_3jet_data"].GetNbinsX()
xlow = histName["M3_3jet_data"].GetXaxis().GetBinLowEdge(1)
xup = histName["M3_3jet_data"].GetXaxis().GetBinUpEdge(Nbins)


mass = RooRealVar("mass","M3",xlow,xup,"GeV/c^{2}")
mass.setBins(Nbins)
Njets = RooRealVar("Njets","jet multiplicity",3,5,"Jet Multiplicity")
Njets.setBins(2)

N = {}
labels2 = ["data", "dataReverse", "TTbar", "Wjets", "Zjets", "QCD", "ST"]
dataHist = {}
histPdf = {}
rfvN = {}

for ilabel in labels2:
    
    tmp3j = histName["M3_3jet_"+ilabel].Integral(-1,-1)
    tmp4j = histName["M3_4jet_"+ilabel].Integral(-1,-1)
    N["M3_3jet_"+ilabel] = RooRealVar("rrv_M3_3jet_"+ilabel,"number of "+ilabel+" events", tmp3j, -100., 800.)
    N["M3_4jet_"+ilabel] = RooRealVar("rrv_M3_4jet_"+ilabel,"number of "+ilabel+" events", tmp4j, -100., 800.)
    #N[ilabel] = RooRealVar("rrv_"+ilabel,"number of "+ilabel+" events", tmp3j+tmp4j  , -100., 800.)
    dataHist["M3_3jet_"+ilabel] = RooDataHist("rdh_M3_3jet_"+ilabel, ilabel, RooArgList(mass), histName["M3_3jet_"+ilabel])
    dataHist["M3_4jet_"+ilabel] = RooDataHist("rdh_M3_4jet_"+ilabel, ilabel, RooArgList(mass), histName["M3_4jet_"+ilabel])
    dataHist["Njets_"+ilabel] = RooDataHist("rdh_Njets_"+ilabel,ilabel, RooArgList(Njets), histName["Njets_"+ilabel])
    histPdf["M3_3jet_"+ilabel] = RooHistPdf("rhp_M3_3jet_"+ilabel,ilabel, RooArgSet(mass), dataHist["M3_3jet_"+ilabel], 0 )
    histPdf["M3_4jet_"+ilabel] = RooHistPdf("rhp_M3_4jet_"+ilabel,ilabel, RooArgSet(mass), dataHist["M3_4jet_"+ilabel], 0 )
    histPdf["Njets_"+ilabel] = RooHistPdf("rhp_Njets_"+ilabel,ilabel, RooArgSet(Njets), dataHist["Njets_"+ilabel], 0 )
    rfvN[ilabel] = RooFormulaVar("rfv_"+ilabel ,ilabel ,"@0+@1",RooArgList(N["M3_3jet_"+ilabel],N["M3_4jet_"+ilabel]) )

# fit models
model_M3_3jet = RooAddPdf("model_M3_3jet","model_M3_3jet",RooArgList(histPdf["M3_3jet_TTbar"],\
                                                         histPdf["M3_3jet_Wjets"],\
                                                         histPdf["M3_3jet_dataReverse"],\
                                                         histPdf["M3_3jet_ST"]),\
                              RooArgList(N["M3_3jet_TTbar"],\
                                             N["M3_3jet_Wjets"],\
                                             N["M3_3jet_dataReverse"],\
                                             N["M3_3jet_ST"]))

model_M3_4jet = RooAddPdf("model_M3_4jet","model_M3_4jet",RooArgList(histPdf["M3_4jet_TTbar"],\
                                                        histPdf["M3_4jet_Wjets"],\
                                                        histPdf["M3_4jet_dataReverse"],\
                                                        histPdf["M3_4jet_ST"]),\
                             RooArgList(N["M3_4jet_TTbar"],\
                                            N["M3_4jet_Wjets"],\
                                            N["M3_4jet_dataReverse"],\
                                            N["M3_4jet_ST"]))

model_Njets = RooAddPdf("model_Njets","model_Njets",RooArgList(histPdf["Njets_TTbar"],\
                                                     histPdf["Njets_Wjets"],\
                                                     histPdf["Njets_dataReverse"],\
                                                     histPdf["Njets_ST"]),\
                            RooArgList(rfvN["TTbar"],\
                                           rfvN["Wjets"],\
                                           rfvN["dataReverse"],\
                                           rfvN["ST"]))

# Constraints
NST_3jet = histName["M3_3jet_ST"].Integral(-1,-1)
NST_4jet = histName["M3_4jet_ST"].Integral(-1,-1)
STgauss_3jet =  RooGaussian("STgauss_3jet","STgauss_3jet", N["M3_3jet_ST"],RooFit.RooConst(NST_3jet),RooFit.RooConst(0.5*NST_3jet))
STgauss_4jet =  RooGaussian("STgauss_4jet","STgauss_4jet", N["M3_4jet_ST"],RooFit.RooConst(NST_4jet),RooFit.RooConst(0.5*NST_4jet))
QCDgauss_3jet = RooGaussian("QCDgauss_3jet","QCDgauss_3jet", N["M3_3jet_dataReverse"],RooFit.RooConst(NQCD_3jet),RooFit.RooConst(0.5*NQCD_3jet))
QCDgauss_4jet = RooGaussian("QCDgauss_4jet","QCDgauss_4jet", N["M3_4jet_dataReverse"],RooFit.RooConst(NQCD_4jet),RooFit.RooConst(0.5*NQCD_4jet))


N["M3_3jet_dataReverse"].setVal(NQCD_3jet)
N["M3_4jet_dataReverse"].setVal(NQCD_4jet)
N["M3_3jet_dataReverse"].setConstant(True)
N["M3_4jet_dataReverse"].setConstant(True)


# Define categories
sample = RooCategory("sample","sample")
sample.defineType("3jet")
sample.defineType("4jet")
sample.defineType("Njets")

CombData = RooDataHist("CombData","combined data",RooArgList(mass,Njets),RooFit.Index(sample),\
                           RooFit.Import("3jet",dataHist["M3_3jet_data"]),RooFit.Import("4jet",dataHist["M3_4jet_data"]),\
                           RooFit.Import("Njets",dataHist["Njets_data"]) )

simPdf = RooSimultaneous("simPdf","simPdf",sample)
simPdf.addPdf(model_M3_3jet,"3jet")
simPdf.addPdf(model_M3_4jet,"4jet")
simPdf.addPdf(model_Njets,"Njets")

# Fit
constraints = RooArgSet(STgauss_3jet,STgauss_4jet) #,QCDgauss_3jet,QCDgauss_4jet)

simfitres = simPdf.fitTo(CombData,RooFit.InitialHesse(True),RooFit.Minos(True),\
                             RooFit.ExternalConstraints(constraints),\
                             RooFit.Extended(True),RooFit.Save(True),RooFit.SumW2Error(True) )

resultsList = simfitres.floatParsFinal()

Ntotal = {}
Ntotal["TTbar"] = rfvN["TTbar"].getVal()
print 50*"="
print "Results:"
print "TTbar = "+str( Ntotal["TTbar"] ) + " \pm " + str( rfvN["TTbar"].getPropagatedError(simfitres) )
print "Wjets = "+str( rfvN["Wjets"].getVal() ) + " \pm " + str(rfvN["Wjets"].getPropagatedError(simfitres) )
#print "Zjets = "+str( rfvN["Wjets"].getVal() ) + " \pm " + str(rfvN["Wjets"].getPropagatedError(simfitres) )
print "efficiency = "+str(eff)
print "xsection = "+str(Ntotal["TTbar"]/(eff*Lumi))
print 50*"="

cv = {}

cv["M3_3jet"] = TCanvas("M3_3jet","M3_3jet",700,700)

frame1 = mass.frame(RooFit.Title("3jet"))
CombData.plotOn(frame1,RooFit.Cut("sample==sample::3jet"))

simPdf.plotOn(frame1,RooFit.Slice(sample,"3jet"),RooFit.Components("rhp_M3_3jet_dataReverse"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.LineColor(style.QCDColor))

#simPdf.plotOn(frame1,RooFit.Slice(sample,"3jet"),RooFit.Components(RooArgSet(histPdf["M3_3jet_dataReverse"],histPdf["M3_3jet_ST"])),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.LineColor(style.SingleTopColor))

#simPdf.plotOn(frame1,RooFit.Slice(sample,"3jet"),RooFit.Components(RooArgSet(histPdf["M3_3jet_dataReverse"],histPdf["M3_3jet_ST"],histPdf["M3_3jet_Wjets"])),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.LineColor(style.WJetsColor))

frame1.Draw()

cv["M3_4jet"] = TCanvas("M3_4jet","M3_4jet",700,700)

frame2 = mass.frame(RooFit.Title("4jet"))
CombData.plotOn(frame2,RooFit.Cut("sample==sample::4jet"))

simPdf.plotOn(frame1,RooFit.Slice(sample,"4jet"),RooFit.Components("rhp_M3_4jet_dataReverse"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.LineColor(style.QCDColor))

frame2.Draw()

cv["Njets"] = TCanvas("Njets","Njets",700,700)

frame3 = Njets.frame(RooFit.Title("Njets"))
CombData.plotOn(frame3,RooFit.Cut("sample==sample::Njets"))

simPdf.plotOn(frame3,RooFit.Slice(sample,"Njets"),RooFit.Components("rhp_Njets_dataReverse"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.LineColor(style.QCDColor))

frame3.Draw()



raw_input( 'Press ENTER to continue\n ' )

