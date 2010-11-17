#! /usr/bin/env python

"""
   fit.py

   Script for template fits
   
   usage: %prog 
   -b, --batch : run in batch mode without graphics.
   -Q, --QCDMC : Use MC QCD shape for templates.
   -s, --scale : Fit for scale systematic uncertainty.
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
IsSyst = False
TypeSyst = "scale"

if option.batch:
    ROOT.gROOT.SetBatch()
    print "running ROOT in batch mode."

if option.QCDMC:
    UseQCDMCShape = True
    print "use QCD MC shape."

if option.scale:
    IsSyst = True
    TypeSyst = "scale"
    print "estimate scale systematic uncertainty"

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

# systematic samples
labelsSyst = {}
NgenTot = {}
if IsSyst:
    labelsSyst = ["TTbar"]#,"Wjets_scaleup","Wjets_scaledown"]
    NgenTot["TTbar_scaleup"] = 788124.
    NgenTot["TTbar_scaledown"] = 928608.0
    #NgenTot["Wjets_scaleup"] = 1603225.0
    #NgenTot["Wjets_scaledown"] = 5666311.0
    #NgenTot["Zjets_scaleup"] = 
    #NgenTot["Zjets_scaledown"] = 1438687.0

    for ilabel in NgenTot.keys():
        histFile[ilabel] = TFile("res_35pb-1/PF/top_plots_"+ilabel+"_PF.root")
        tmpname = ilabel.split("_")[0]
        histName["M3_3jet_"+ilabel] = gDirectory.Get("/M3/M3_3jet_"+tmpname+"PF")
        histName["M3_4jet_"+ilabel] = gDirectory.Get("/M3/M3_4jet_"+tmpname+"PF")
        histName["Njets_"+ilabel] = gDirectory.Get("/jets/Njets_"+tmpname+"PF")

        #if ilabel.find("TTbar")!=-1:
        #    eff = (histName["M3_3jet_"+ilabel].Integral(-1,-1)+histName["M3_4jet_"+ilabel].Integral(-1,-1))/NgenTot[ilabel]

        if ilabel!="data" and ilabel!="dataReverse":
            ascale = 157.5 * Lumi/NgenTot[ilabel]
        #print ilabel                                                                                                                                                
        #print str(scale[ilabel])                                                                                                                                    
            histName["M3_3jet_"+ilabel].Scale(ascale)
            histName["M3_4jet_"+ilabel].Scale(ascale)
            histName["Njets_"+ilabel].Scale(ascale)


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
mass.setBins(Nbins,"cache")
Njets = RooRealVar("Njets","jet multiplicity",3,5,"Jet Multiplicity")
Njets.setBins(2)#,"cache")

N = {}
labels2 = ["data", "dataReverse", "TTbar", "Wjets", "Zjets", "QCD", "ST"]
dataHist = {}
histPdf = {}
rfvN = {}

for ilabel in labels2:
    
    tmp3j = histName["M3_3jet_"+ilabel].Integral(-1,-1)
    tmp4j = histName["M3_4jet_"+ilabel].Integral(-1,-1)
    N["M3_3jet_"+ilabel] = RooRealVar("rrv_M3_3jet_"+ilabel,"number of "+ilabel+" events", tmp3j, -100., 1000.)
    N["M3_4jet_"+ilabel] = RooRealVar("rrv_M3_4jet_"+ilabel,"number of "+ilabel+" events", tmp4j, -100., 1000.)
    #N[ilabel] = RooRealVar("rrv_"+ilabel,"number of "+ilabel+" events", tmp3j+tmp4j  , -100., 800.)
    dataHist["M3_3jet_"+ilabel] = RooDataHist("rdh_M3_3jet_"+ilabel, "rdh_M3_3jet_"+ilabel, RooArgList(mass), histName["M3_3jet_"+ilabel])
    dataHist["M3_4jet_"+ilabel] = RooDataHist("rdh_M3_4jet_"+ilabel, "rdh_M3_4jet_"+ilabel, RooArgList(mass), histName["M3_4jet_"+ilabel])
    dataHist["Njets_"+ilabel] = RooDataHist("rdh_Njets_"+ilabel,ilabel, RooArgList(Njets), histName["Njets_"+ilabel])
    histPdf["M3_3jet_"+ilabel] = RooHistPdf("rhp_M3_3jet_"+ilabel,"rhp_M3_3jet_"+ilabel, RooArgSet(mass), dataHist["M3_3jet_"+ilabel], 0 )
    histPdf["M3_4jet_"+ilabel] = RooHistPdf("rhp_M3_4jet_"+ilabel,"rhp_M3_4jet_"+ilabel, RooArgSet(mass), dataHist["M3_4jet_"+ilabel], 0 )
    histPdf["Njets_"+ilabel] = RooHistPdf("rhp_Njets_"+ilabel,"rhp_Njets_"+ilabel, RooArgSet(Njets), dataHist["Njets_"+ilabel], 0 )
    rfvN[ilabel] = RooFormulaVar("rfv_"+ilabel ,"rfv_"+ilabel ,"@0+@1",RooArgList(N["M3_3jet_"+ilabel],N["M3_4jet_"+ilabel]) )


# systematic p.d.fs
alpha = {}
if IsSyst:
    
    for ilabel in NgenTot.keys():
        print ilabel
        tmp3j = histName["M3_3jet_"+ilabel].Integral(-1,-1)
        tmp4j = histName["M3_4jet_"+ilabel].Integral(-1,-1)
        N["M3_3jet_"+ilabel] = RooRealVar("rrv_M3_3jet_"+ilabel,"number of "+ilabel+" events", tmp3j, -100., 1000.)
        N["M3_4jet_"+ilabel] = RooRealVar("rrv_M3_4jet_"+ilabel,"number of "+ilabel+" events", tmp4j, -100., 1000.)
        dataHist["M3_3jet_"+ilabel] = RooDataHist("rdh_M3_3jet_"+ilabel, "rdh_M3_3jet_"+ilabel, RooArgList(mass), histName["M3_3jet_"+ilabel])
        dataHist["M3_4jet_"+ilabel] = RooDataHist("rdh_M3_4jet_"+ilabel, "rdh_M3_4jet_"+ilabel, RooArgList(mass), histName["M3_4jet_"+ilabel])
        dataHist["Njets_"+ilabel] = RooDataHist("rdh_Njets_"+ilabel,ilabel, RooArgList(Njets), histName["Njets_"+ilabel])
        histPdf["M3_3jet_"+ilabel] = RooHistPdf("rhp_M3_3jet_"+ilabel,"rhp_M3_3jet_"+ilabel, RooArgSet(mass), dataHist["M3_3jet_"+ilabel], 0 )
        histPdf["M3_4jet_"+ilabel] = RooHistPdf("rhp_M3_4jet_"+ilabel,"rhp_M3_4jet_"+ilabel, RooArgSet(mass), dataHist["M3_4jet_"+ilabel], 0 )
        histPdf["Njets_"+ilabel] = RooHistPdf("rhp_Njets_"+ilabel,"rhp_Njets_"+ilabel, RooArgSet(Njets), dataHist["Njets_"+ilabel], 0 )
        rfvN[ilabel] = RooFormulaVar("rfv_"+ilabel ,"rfv_"+ilabel ,"@0+@1",RooArgList(N["M3_3jet_"+ilabel],N["M3_4jet_"+ilabel]) )

    alpha["TTbar"] = RooRealVar("alpha_TTbar","alpha_TTbar",0,1.)
    alpha["TTbar"].setBins(10,"cache")
    N["M3_3jet_TTbar_"+TypeSyst] = RooRealVar("rrv_M3_3jet_TTbar_"+TypeSyst,"number of "+ilabel+" events", tmp3j, -100., 1000.)
    #histPdf["M3_3jet_TTbar_scale"] = RooMomentMorph("TTbar_morph","TTbar_morph",
    histPdf["M3_3jet_TTbar_"+TypeSyst] = RooIntegralMorph("TTbar_morph","TTbar_morph",histPdf["M3_3jet_TTbar_"+TypeSyst+"down"],histPdf["M3_3jet_TTbar_"+TypeSyst+"up"],mass,alpha["TTbar"])
    histPdf["M3_3jet_TTbar_"+TypeSyst].setInterpolationOrder(0)
    #histPdf["M3_3jet_TTbar_scale"].setCacheAlpha(True)

    cv = TCanvas("morph","morph",700,700)

    frametmp = mass.frame()
    histPdf["M3_3jet_TTbar_scaledown"].plotOn(frametmp)
    histPdf["M3_3jet_TTbar_scaleup"].plotOn(frametmp)

    alpha["TTbar"].setVal(0.2)
    histPdf["M3_3jet_TTbar_scale"].plotOn(frametmp,RooFit.LineColor(2))
    alpha["TTbar"].setVal(0.8)
    histPdf["M3_3jet_TTbar_scale"].plotOn(frametmp,RooFit.LineColor(2))

    frametmp.Draw()
    #raw_input( 'Press ENTER to continue\n ' )
    histPdf["M3_3jet_TTbar_scale"].setCacheAlpha(True)

# fit models
listPdfs_3jet = RooArgList(histPdf["M3_3jet_TTbar"],\
                               histPdf["M3_3jet_Wjets"],\
                               histPdf["M3_3jet_dataReverse"],\
                               histPdf["M3_3jet_ST"])
listCoefPdfs_3jet = RooArgList(N["M3_3jet_TTbar"],\
                                   N["M3_3jet_Wjets"],\
                                   N["M3_3jet_dataReverse"],\
                                   N["M3_3jet_ST"])


if IsSyst:
    listPdfs_3jet = RooArgList(histPdf["M3_3jet_TTbar_scale"],\
                               histPdf["M3_3jet_Wjets"],\
                               histPdf["M3_3jet_dataReverse"],\
                               histPdf["M3_3jet_ST"])
    listCoefPdfs_3jet = RooArgList(N["M3_3jet_TTbar_scale"],\
                                   N["M3_3jet_Wjets"],\
                                   N["M3_3jet_dataReverse"],\
                                   N["M3_3jet_ST"])

model_M3_3jet = RooAddPdf("model_M3_3jet","model_M3_3jet",listPdfs_3jet, listCoefPdfs_3jet)

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
Ntoterr = {}
Ntotal["TTbar"] = rfvN["TTbar"].getVal()
Ntoterr["TTbar"] = rfvN["TTbar"].getPropagatedError(simfitres) 
print 50*"="
print "Results:"
print "TTbar = "+str( Ntotal["TTbar"] ) + " \pm " + str( Ntoterr["TTbar"] )
print "Wjets = "+str( rfvN["Wjets"].getVal() ) + " \pm " + str(rfvN["Wjets"].getPropagatedError(simfitres) )
#print "Zjets = "+str( rfvN["Wjets"].getVal() ) + " \pm " + str(rfvN["Wjets"].getPropagatedError(simfitres) )
print "efficiency = "+str(eff)
print "xsection = "+str(Ntotal["TTbar"]/(eff*Lumi)) + " \pm " + str( Ntoterr["TTbar"]/(eff*Lumi))
print 50*"="

cv = {}

cv["M3_3jet"] = TCanvas("M3_3jet","M3_3jet",700,700)

frame1 = mass.frame(RooFit.Title("3jet"))
CombData.plotOn(frame1,RooFit.Cut("sample==sample::3jet"))

simPdf.plotOn(frame1,RooFit.Name("3jet_dataReverse"),RooFit.Slice(sample,"3jet"),RooFit.Components("rhp_M3_3jet_dataReverse"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.FillColor(style.QCDColor),RooFit.LineColor(1),RooFit.DrawOption("F"),RooFit.MoveToBack())

simPdf.plotOn(frame1,RooFit.Name("3jet_ST"),RooFit.Slice(sample,"3jet"),RooFit.Components("rhp_M3_3jet_ST"),RooFit.AddTo("3jet_dataReverse"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.FillColor(style.SingleTopColor),RooFit.DrawOption("F"),RooFit.MoveToBack())

simPdf.plotOn(frame1,RooFit.Name("3jet_Wjets"),RooFit.Slice(sample,"3jet"),RooFit.Components("rhp_M3_3jet_Wjets"),RooFit.AddTo("3jet_ST"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.FillColor(style.WJetsColor),RooFit.DrawOption("F"),RooFit.MoveToBack())

simPdf.plotOn(frame1,RooFit.Name("3jet_TTbar"),RooFit.Slice(sample,"3jet"),RooFit.Components("rhp_M3_3jet_TTbar"),RooFit.AddTo("3jet_Wjets"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.FillColor(style.TtbarColor),RooFit.DrawOption("F"),RooFit.MoveToBack())



frame1.Draw()

#raw_input( 'Press ENTER to continue\n ' )


cv["M3_4jet"] = TCanvas("M3_4jet","M3_4jet",700,700)

frame2 = mass.frame(RooFit.Title("4jet"))
CombData.plotOn(frame2,RooFit.Cut("sample==sample::4jet"))

simPdf.plotOn(frame2,RooFit.Name("4jet_dataReverse"),RooFit.Slice(sample,"4jet"),RooFit.Components("rhp_M3_4jet_dataReverse"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.FillColor(style.QCDColor),RooFit.LineColor(1),RooFit.DrawOption("F"))

simPdf.plotOn(frame2,RooFit.Name("4jet_ST"),RooFit.Slice(sample,"4jet"),RooFit.Components("rhp_M3_4jet_ST"),RooFit.AddTo("4jet_dataReverse"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.FillColor(style.SingleTopColor),RooFit.DrawOption("F"),RooFit.MoveToBack())

simPdf.plotOn(frame2,RooFit.Name("4jet_Wjets"),RooFit.Slice(sample,"4jet"),RooFit.Components("rhp_M3_4jet_Wjets"),RooFit.AddTo("4jet_ST"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.FillColor(style.WJetsColor),RooFit.DrawOption("F"),RooFit.MoveToBack())

simPdf.plotOn(frame2,RooFit.Name("4jet_TTbar"),RooFit.Slice(sample,"4jet"),RooFit.Components("rhp_M3_4jet_TTbar"),RooFit.AddTo("4jet_Wjets"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.FillColor(style.TtbarColor),RooFit.DrawOption("F"),RooFit.MoveToBack())

frame2.Draw()

cv["Njets"] = TCanvas("Njets","Njets",700,700)

frame3 = Njets.frame(RooFit.Title("Njets"))
CombData.plotOn(frame3,RooFit.Cut("sample==sample::Njets"))

simPdf.plotOn(frame3,RooFit.Name("rp_Njets_dataReverse"),RooFit.Slice(sample,"Njets"),RooFit.Components("rhp_Njets_dataReverse"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.FillColor(style.QCDColor),RooFit.LineColor(1),RooFit.DrawOption("F"))

simPdf.plotOn(frame3,RooFit.Name("rp_Njets_ST"),RooFit.Slice(sample,"Njets"),RooFit.Components("rhp_Njets_ST"),RooFit.AddTo("rp_Njets_dataReverse"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.FillColor(style.SingleTopColor),RooFit.DrawOption("F"),RooFit.MoveToBack())

simPdf.plotOn(frame3,RooFit.Name("rp_Njets_Wjets"),RooFit.Slice(sample,"Njets"),RooFit.Components("rhp_Njets_Wjets"),RooFit.AddTo("rp_Njets_ST"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.FillColor(style.WJetsColor),RooFit.DrawOption("F"),RooFit.MoveToBack())

simPdf.plotOn(frame3,RooFit.Name("rp_Njets_TTbar"),RooFit.Slice(sample,"Njets"),RooFit.Components("rhp_Njets_TTbar"),RooFit.AddTo("rp_Njets_Wjets"),RooFit.ProjWData(RooArgSet(sample),CombData),RooFit.FillColor(style.TtbarColor),RooFit.DrawOption("F"),RooFit.MoveToBack())

frame3.Draw()

cv["CovMatrix"] = TCanvas("CovMatrix","CovMatrix",700,700)

gStyle.SetPalette(1) 
hcorr = simfitres.correlationHist()
hcorr.Draw("colz")

if IsSyst:

    cv["syst"] = TCanvas("syst","syst",700,700)
    
    frame4 = mass.frame()
    histPdf["M3_3jet_TTbar_scaledown"].plotOn(frame4)
    histPdf["M3_3jet_TTbar_scaleup"].plotOn(frame4)
    
    frame4.Draw()
    #frame4 = mass.frame(RooFit.Title("3jet"))


raw_input( 'Press ENTER to continue\n ' )

