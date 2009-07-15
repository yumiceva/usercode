#!/usr/bin/env python
#____________________________________________________________
#
#  cuy
#
# A very simple way to make plots with ROOT via an XML file
#
# Francisco Yumiceva
# yumiceva@fnal.gov
#
# Fermilab, 2008
#
#____________________________________________________________

"""
   cuy

    A very simple way to make plots with ROOT via an XML file.

   usage: %prog -x <XML configuration file>
   -c, --create  = CREATE: create XML configuration file from a ROOT file.
   -e, --example = EXAMPLE: generate an example xml file.
   -f, --flag    = FLAG: create a baneer
   -l, --list    = LIST: list of objects in the ROOT file. 
   -p, --prt     = PRT: print canvas in the format specified png, ps, eps, pdf, etc.
   -t, --tag     = TAG: tag name for XML configuration file.
   -w, --wait : Pause script after plotting a new superposition of histograms.
   -x, --xml     = XML: xml configuration file.
   
   Francisco Yumiceva (yumiceva@fnal.gov)
   Fermilab 2008
   
"""


import os, string, re, sys, math

try:
    import ROOT
except:
    print "\nCannot load PYROOT, make sure you have setup ROOT in the path"
    print "and pyroot library is also defined in the variable PYTHONPATH, try:\n"
    if (os.getenv("PYTHONPATH")):
	print " setenv PYTHONPATH ${PYTHONPATH}:$ROOTSYS/lib\n"
    else:
	print " setenv PYTHONPATH $ROOTSYS/lib\n"
    sys.exit()

from ROOT import TFile
from ROOT import TCanvas
from ROOT import TLegend
from ROOT import SetOwnership
from ROOT import THStack
from ROOT import TLatex

from xml.sax import saxutils, make_parser, handler
from xml.sax.handler import feature_namespaces

import Inspector
import Style

#_______________OPTIONS________________
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

#______________________________________________________________________

class ValElement:
    def __init__(self):
	self.type = ""
	self.filename = ""
	self.release = ""
	self.histos = {}
	self.TH1s = {}
	self.weight = None

class divideElement:
    def __init__(self):
	self.name = ""
	self.numerator = None
	self.denominator = None

class plotElement:
    def __init__(self):
	self.name = ""
	self.title = ""
	self.color = ""

class additionElement:
    def __init__(self):
	self.name = ""
	self.title = ""
	self.SetLogy = ""
	self.SetGrid = ""
	self.histos = []
	self.weight = []
	
class superimposeElement:
    def __init__(self):
	self.name = ""
	self.title = ""
	self.SetLogy = ""
	self.SetGrid = ""
	self.histos = []
	self.color = []
	self.marker = []
	self.legend = []
	self.weight = []

class FindIssue(handler.ContentHandler):
    def __init__(self):
	self.data = {}
	self.divide = {}
	self.addition = {}
	self.superimpose = {}
	self.tmpaddname = ""
	self.plot = {}
	self.size = 0
	self.atype = ""
	self.tmpsupername = ""

    def startElement(self, name, attrs):
        if name == 'validation':
	    self.size = self.size + 1
	    self.atype = attrs.get('type',None)
	    self.data[self.atype] = ValElement()
	    self.data[self.atype].type = attrs.get('type',None)
	    self.data[self.atype].filename = attrs.get('file',None)
	    self.data[self.atype].release = attrs.get('release',None)
	    self.data[self.atype].weight = attrs.get('weight','')
	if name == 'TH1':
	    self.data[self.atype].histos[attrs.get('name',None)] = attrs.get('source',None)
	    #print attrs.get('name',None)
	    #print attrs.get('source',None)
	if name == 'divide':
	    aname = attrs.get('name',None)
	    self.divide[aname] = divideElement()
	    self.divide[aname].name = aname
	    self.divide[aname].numerator = attrs.get('numerator',None)
	    self.divide[aname].denominator = attrs.get('denominator',None)
	    self.divide[aname].DivideOption = attrs.get('DivideOption',None)
	    self.divide[aname].Option = attrs.get('Option',None)
	if name == 'addition':
	    aname = attrs.get('name',None)
	    self.addition[aname] = additionElement()
	    self.tmpaddname = aname
	    self.addition[aname].name = aname
	    self.addition[aname].title = attrs.get('title',None)
	    self.addition[aname].YTitle = attrs.get('YTitle',None)
	    self.addition[aname].XTitle = attrs.get('XTitle',None)
	    self.addition[aname].Option = attrs.get('Option',None)
	    self.addition[aname].Weight = attrs.get('Wight',None)
	    self.addition[aname].Normalize = attrs.get('Normalize',None)
	    self.addition[aname].SetGrid = attrs.get('SetGrid',None)
	if name == 'additionItem':
	    #print "in element: " + self.tmpsupername
	    self.addition[self.tmpaddname].histos.append(attrs.get('name',None))
	    self.addition[self.tmpaddname].weight.append(attrs.get('weight',None))
	if name == 'superimpose':
	    aname = attrs.get('name',None)
	    self.superimpose[aname] = superimposeElement()
	    self.superimpose[aname].name = aname
	    self.superimpose[aname].title = attrs.get('title',None)
	    self.superimpose[aname].SetLogy = attrs.get('SetLogy',None)
	    self.superimpose[aname].SetGrid = attrs.get('SetGrid',None)
	    self.superimpose[aname].Normalize = attrs.get('Normalize',None)
	    self.superimpose[aname].Stack     = attrs.get('Stack',None)
	    self.superimpose[aname].YTitle = attrs.get('YTitle',None)
	    self.superimpose[aname].XTitle = attrs.get('XTitle',None)
	    self.superimpose[aname].projection = attrs.get('Projection',None)
	    self.superimpose[aname].bin = attrs.get('bin',None)
	    self.superimpose[aname].profile = attrs.get('Profile',None)
	    self.superimpose[aname].Fill = attrs.get('Fill',None)
	    self.superimpose[aname].Option = attrs.get('Option',None)
	    self.superimpose[aname].Weight = attrs.get('Weight',None)
	    self.superimpose[aname].Maximum = attrs.get('Maximum',None)
	    self.superimpose[aname].Labels = attrs.get('Labels',None)
	    self.tmpsupername = aname
	if name == 'superimposeItem':
	    #print "in element: " + self.tmpsupername
	    self.superimpose[self.tmpsupername].histos.append(attrs.get('name',None))
	    self.superimpose[self.tmpsupername].color.append(attrs.get('color',None))
	    self.superimpose[self.tmpsupername].marker.append(attrs.get('MarkerStyle',None))
	    self.superimpose[self.tmpsupername].legend.append(attrs.get('legend',None))
	    #self.superimpose[self.tmpsupername].weight.append(attrs.get('weight',None))

if __name__ == '__main__':

    # style
    thestyle = Style.Style()
    thestyle.SetStyle()

    printCanvas = False
    printFormat = "png"
    printBanner = False
    Banner = "CMS Preliminary"

    # check options
    option,args = parse(__doc__)
    if not args and not option: exit()

    if option.list:
	ins = Inspector.Inspector()
	ins.Verbose(True)
	ins.createXML(False)
	ins.SetFilename(option.list)
	ins.GetListObjects()
	sys.exit()

    if option.create:
	createXML = Inspector.Inspector()
	createXML.Verbose(False)
	createXML.createXML(True)
	if option.tag:
	    createXML.SetTag(option.tag)
	createXML.SetFilename(option.create)
	createXML.GetListObjects()
	sys.exit()

    if not option.xml: exit()
    if option.prt: 
	printCanvas = True
	printFormat = option.prt

    if option.flag:
	printBanner = True
	Banner = option.flag

    # check xml file
    try:
	xmlfile = open(option.xml)
	xmlfile.close()
    except:
	print " ERROR: xml file \"" + option.xml + "\" does not exist"
	sys.exit()
    
    # Create a parser
    parser = make_parser()

    # Tell the parser we are not interested in XML namespaces
    parser.setFeature(feature_namespaces, 0)

    # Create the handler
    dh = FindIssue()

    # Tell the parser to use our handler
    parser.setContentHandler(dh)

    # Parse the input
    parser.parse(option.xml)

    # list of canvas
    cv = {}
    afilelist = {}
    stacklist = {}

    # root output file
    outputroot = TFile("cuy.root","RECREATE")

    # new histograms
    newTH1list = []

    # extract histograms
    thedata = dh.data

    firstFilename = ''

    for ikey in thedata:
	print "= Processing set called: " + ikey
	afilename = thedata[ikey].filename
	if firstFilename == '':
	    firstFilename = afilename
	arelease = ""
	if thedata[ikey].release != None:
	    arelease = thedata[ikey].release
	print "== filename: " + afilename
	print "== release:  " + arelease
	print "== weight:   " + thedata[ikey].weight
	thehistos = thedata[ikey].histos
	afilelist[afilename] = TFile(afilename)
	print "== get histograms: "
	histonamekeys = thehistos.keys()
	for ihname in histonamekeys:
	    print "=== Histogram name: \""+ ihname + "\" source: \""+thehistos[ihname]+"\""
	    thedata[ikey].TH1s[ihname] = ROOT.gDirectory.Get(thehistos[ihname])
	    #SetOwnership(thedata[ikey].TH1s[ihname], 0)
	    # check if file exists
	    #print thedata[ikey].TH1s[ihname].GetEntries()


    # plot superimpose histograms
    #afilelist['../outputLayer2_ttbarmuonic_all.root'].cd()
    afilelist[firstFilename].cd()
    #print thedata['ttbar'].TH1s['gen_eta'].GetEntries()


    theaddition = dh.addition
    print "= Create addition histograms:"
    
    for ikey in theaddition:
	print "== plot name: \""+theaddition[ikey].name+"\" title: \""+theaddition[ikey].title+"\""
	listname = theaddition[ikey].histos
	listweight = theaddition[ikey].weight

	#create canvas
	cv[theaddition[ikey].name] = TCanvas(theaddition[ikey].name,theaddition[ikey].name,700,700)

	isFirst = True
	ihnameIt = 0
	for ihname in listname:
	    aweight = 1
	    if listweight[ihnameIt]:
	    #if thedata[jkey].weight != None and theaddition[ikey].Weight == "true":
		aweight = float(listweight[ihnameIt])
		#aweight = float(thedata[jkey].weight)
	    for jkey in thedata:
		tmpkeys = thedata[jkey].histos.keys()
		for tmpname in tmpkeys:
		    if tmpname == ihname:
			ath = thedata[jkey].TH1s[tmpname]
			if ath is None:
			    print "ERROR: histogram name \""+tmpname+"\" does not exist in file "+thedata[jkey].filename
			    exit(0)
			print "=== add histogram: "+ath.GetName() + " from " + thedata[jkey].filename + " mean = " + "%.2f" % round(ath.GetMean(),2) + " weight= " + str(aweight)
			#ath.Print("all")
			if isFirst:
			    newth = ath.Clone(theaddition[ikey].name)
			    newth.Sumw2()
			    if theaddition[ikey].Normalize == "true":
				newth.Scale(1/newth.Integral())
			    newth.Scale(aweight)
			    isFirst = False
			else:
			    atmpth = ath.Clone()
			    atmpth.Sumw2()
			    if theaddition[ikey].Normalize == "true":
				atmpth.Scale(1/atmpth.Integral())
			    atmpth.Scale(aweight)
			    newth.Add( atmpth )
	    ihnameIt = ihnameIt + 1

	if theaddition[ikey].XTitle != None:
	    newth.SetXTitle(theaddition[ikey].XTitle)
	if theaddition[ikey].YTitle != None:
	    newth.SetYTitle(theaddition[ikey].YTitle)

	if theaddition[ikey].Option:
	    newth.Draw(theaddition[ikey].Option)
	else:
	    newth.Draw()

	if theaddition[ikey].SetGrid == "true":
	    cv[theaddition[ikey].name].SetGrid()
	
	cv[theaddition[ikey].name].Update()

	# add new histogram to the list
	newth.SetName(theaddition[ikey].name)
	newTH1list.append(newth.GetName())
	thedata[newth.GetName()] = ValElement()
	thedata[newth.GetName()].TH1s[newth.GetName()] = newth
	thedata[newth.GetName()].histos[newth.GetName()] = newth.GetName()

	# write new histograms to file
	outputroot.cd()
	newth.Write()
	
    
    print "= Create ratio histograms:"
    
    thedivition = dh.divide
    for ikey in thedivition:
	print "== plot name: \""+thedivition[ikey].name+"\" title: \""+"\""
	numerator = thedivition[ikey].numerator
	denominator = thedivition[ikey].denominator

	#create canvas
	cv[thedivition[ikey].name] = TCanvas(thedivition[ikey].name,thedivition[ikey].name,700,700)

	for jkey in thedata:
	    tmpkeys = thedata[jkey].histos.keys()
	    for tmpname in tmpkeys:
		if tmpname == numerator:
		    numeratorth = thedata[jkey].TH1s[tmpname]
		    if numeratorth is None:
			print "ERROR: histogram name \""+tmpname+"\" does not exist in file "+thedata[jkey].filename
			exit(0)
			#print "=== numerator histogram: "+numeratorth.GetName() + " from " + thedata[jkey].filename + " mean = " + "%.2f" % round(numeratorth.GetMean(),2) + " weight= " + str(aweight)

		if tmpname == denominator:
		    denominatorth = thedata[jkey].TH1s[tmpname]
		    if denominatorth is None:
			print "ERROR: histogram name \""+tmpname+"\" does not exist in file "+thedata[jkey].filename
			exit(0)
			#print "=== denominator histogram: "+denominatorth.GetName() + " from " + thedata[jkey].filename + " mean = " + "%.2f" % round(denominatorth.GetMean(),2) + " weight= " + str(aweight)


	
	numeratorth.Sumw2()
	denominatorth.Sumw2()
	newth = numeratorth.Clone()
	newth.Clear()
	if thedivition[ikey].DivideOption is None:
	    newth.Divide(numeratorth,denominatorth)
	else:
	    newth.Divide(numeratorth,denominatorth,1.,1.,thedivition[ikey].DivideOption)
#	if theaddition[ikey].XTitle != None:
#	    newth.SetXTitle(theaddition[ikey].XTitle)
#	if theaddition[ikey].YTitle != None:
#	    newth.SetYTitle(theaddition[ikey].YTitle)

	if thedivition[ikey].Option:
	    newth.Draw(thedivition[ikey].Option)
	else:
	    newth.Draw()

	cv[thedivition[ikey].name].Update()
	
	
	# pause
	if option.wait:
	    raw_input( 'Press ENTER to continue\n ' )

	# add new histogram to the list
	newth.SetName(thedivition[ikey].name)
	newTH1list.append(newth.GetName())
	thedata[newth.GetName()] = ValElement()
	thedata[newth.GetName()].TH1s[newth.GetName()] = newth
	thedata[newth.GetName()].histos[newth.GetName()] = newth.GetName()
	
	# write new histograms to file
	outputroot.cd()
	newth.Write()


    thesuper = dh.superimpose
    print "= Create superimpose histograms:"
    for ikey in thesuper:
	print "== plot name: \""+thesuper[ikey].name+"\" title: \""+thesuper[ikey].title+"\""
	listname = thesuper[ikey].histos
	listcolor = thesuper[ikey].color
	listmarker = thesuper[ikey].marker
	listlegend = thesuper[ikey].legend
	#listweight = thesuper[ikey].weight
	dolegend = True
	for il in listlegend:
	    if il==None: dolegend = False
	print "dolegend = " +str(dolegend)
	doNormalize = False
	if thesuper[ikey].Normalize == "true":
	    doNormalize = True
	    print "normalize = " +str(doNormalize)
	projectAxis = "no"
	projectBin = -1 #all
	if thesuper[ikey].projection == "x": projectAxis = "x"
	if thesuper[ikey].projection == "y": projectAxis = "y"
	if thesuper[ikey].bin != None: projectBin = thesuper[ikey].bin
	profileAxis = "no"
	if thesuper[ikey].profile == "x": profileAxis = "x"
	if thesuper[ikey].profile == "y": profileAxis = "y"
	doFill = False
	if thesuper[ikey].Fill == "true": doFill = True
	print doFill
	#create canvas
	cv[thesuper[ikey].name] = TCanvas(thesuper[ikey].name,thesuper[ikey].title,700,700)
	#legend
	aleg = TLegend(0.6,0.4,0.8,0.6)
	SetOwnership( aleg, 0 ) 
	aleg.SetMargin(0.12)
        aleg.SetTextSize(0.035)
        aleg.SetFillColor(10)
	aleg.SetBorderSize(0)

	isFirst = 1
	ii = 0

	stacklist[thesuper[ikey].name] = THStack("astack"+thesuper[ikey].name,thesuper[ikey].title)
	astack = stacklist[thesuper[ikey].name]
	for ihname in listname:
	
	    for jkey in thedata:
		tmpkeys = thedata[jkey].histos.keys()
		
		for tmpname in tmpkeys:
		
		    if tmpname == ihname:
			ath = thedata[jkey].TH1s[tmpname]
			if ath is None:
			    print "ERROR: histogram name \""+tmpname+"\" does not exist in file "+thedata[jkey].filename
			    exit(0)
			print "=== superimpose histogram: "+ath.GetName() + " mean = " + "%.2f" % round(ath.GetMean(),2)
			# project 2D histogram if requested
			if projectAxis == "x":
			    if projectBin == -1:
				newthpx = ath.ProjectionX(ath.GetName()+"_px",0,-1,"e")
			    else:
				newthpx = ath.ProjectionX(ath.GetName()+"_px",int(projectBin),int(projectBin),"e")
			    newth = newthpx.Clone()
			if projectAxis == "y":
			    if projectBin == -1:
				newthpy = ath.ProjectionY(ath.GetName()+"_py",0,-1,"e")
			    else:
				newthpx = ath.ProjectionY(ath.GetName()+"_py",int(projectBin),int(projectBin),"e")
			    newth = newthpy.Clone()
			if profileAxis == "x":
			    newthpx = ath.ProfileX(ath.GetName()+"_px",0,-1,"e")
			    newth = newthpx.Clone()
			if profileAxis == "y":
			    newthpy = ath.ProfileY(ath.GetName()+"_py",0,-1,"e")
			    newth = newthpy.Clone()
			
			# get weight
			aweight = 1
			if thedata[jkey].weight != None and thesuper[ikey].Weight=="true":
			    aweight = float( thedata[jkey].weight )
			print " with weight = " + str(aweight)
			#if listweight[ii]:
			 #   aweight = float( listweight[ii] )

			# clone original histogram
			if projectAxis == "no" and profileAxis == "no" : newth = ath.Clone()

                        newth.Sumw2()
			newth.Scale(aweight)
			
			# check if we have color
			if not listcolor[ii]:
			    listcolor[ii] = 1
			
			newth.SetLineColor(int(listcolor[ii]))
			newth.SetMarkerColor(int(listcolor[ii]))
			
			if doFill: newth.SetFillColor(int(listcolor[ii]))

			if listmarker[ii] != None:
			    newth.SetMarkerStyle(int(listmarker[ii]))
			# normalize
			if doNormalize:
			    newth.Scale(1./newth.Integral())
			#print "   "+listlegend[ii]
			
			if thesuper[ikey].Labels != None:
			    thelabels = thesuper[ikey].Labels.split(',')
			    ib = 1
			    #print thelabels

			    for ilabel in thelabels:
				newth.GetXaxis().SetBinLabel(ib,ilabel)
				#if ib==1:
				    #newth.GetXaxis().SetBinLabel(ib,"")
				#newth.GetHistogram().GetXaxis().SetBinLabel(ib,ilabel)
				ib += 1
			    #if aweight==0.0081:
			#	newth.SetBinContent(1, newth.GetBinContent(1) / 0.28756)
			 #   if aweight==0.0883:
				#newth.SetBinContent(1, newth.GetBinContent(1) / 0.01953)
			    #if aweight==0.0731:
				#newth.SetBinContent(1, newth.GetBinContent(1) / 0.0367)
			    #if aweight==0.4003:
				#newth.SetBinContent(1, newth.GetBinContent(1) / 0.5683)
			    #if aweight==0.003:
				#newth.SetBinContent(1, newth.GetBinContent(1) / 0.21173)
			    #if aweight==0.0027:
				#newth.SetBinContent(1, newth.GetBinContent(1) / 0.26394)
			    #if aweight==0.0034:
				#newth.SetBinContent(1, newth.GetBinContent(1) / 0.26394)


			# stack histograms
			if doFill:
			    if thesuper[ikey].XTitle != None:
				newth.SetXTitle("")
			    astack.Add(newth,"HIST")
			elif thesuper[ikey].Option:
			    astack.Add(newth,thesuper[ikey].Option)
			else:
			    #newth.Fit("landau")
			    astack.Add(newth)
			    
			astack.SetTitle(thesuper[ikey].title)
			
			if isFirst==1:
			    newth.GetPainter().PaintStat(ROOT.gStyle.GetOptStat(),0);
			    isFirst=0
			    tmpsumth = newth.Clone()
			else:
			    tmpsumth.Add(newth)
			#    newth.SetTitle(thesuper[ikey].title)
			#    if thesuper[ikey].YTitle != None:
			#	newth.SetYTitle(thesuper[ikey].YTitle)
			#    newth.Draw()
			#    isFirst=0
			#else:
			#    newth.Draw("same")
			if dolegend and doFill: 
			    aleg.AddEntry(newth,listlegend[ii],"F")
			elif dolegend:
			    aleg.AddEntry(newth,listlegend[ii],"P")
			
			newth.SetName(tmpname)
			outputroot.cd()
			newth.Write()
	    ii = ii + 1

	
	if thesuper[ikey].Maximum != None:
	    astack.SetMaximum( float(thesuper[ikey].Maximum) )
	if thesuper[ikey].Stack == "true":
	    astack.Draw()
	if thesuper[ikey].Stack == "false" or thesuper[ikey].Stack == None:
	    astack.Draw()
	    astack.Draw("nostack")
	if thesuper[ikey].XTitle != None:
	    astack.GetHistogram().SetXTitle(thesuper[ikey].XTitle)
	if thesuper[ikey].YTitle != None:
	    astack.GetHistogram().SetYTitle(thesuper[ikey].YTitle)
	if doFill:
	    astack.Draw("sameaxis")

	
	#thelabels = []
	#if thesuper[ikey].Labels != None:
	#    thelabels = thesuper[ikey].Labels.split(',')
	#    ib = 1
	#    print thelabels

	 #   for ilabel in thelabels:
	#	astack.GetXaxis().SetBinLabel(ib,ilabel)
		#astack.GetHistogram().GetXaxis().SetBinLabel(ib,ilabel)
		#ib += 1
	#    astack.Draw()
	#    astack.Draw("sameaxis")

	if dolegend: 
	    aleg.Draw()
	if thesuper[ikey].SetLogy == "true":
	    cv[thesuper[ikey].name].SetLogy()
	if thesuper[ikey].SetGrid == "true":
	    cv[thesuper[ikey].name].SetGrid()
	
	# test smearing
	#rn = ROOT.TRandom(12345)
	#for iibin in range(0,tmpsumth.GetNbinsX()):
	#    tmpsumth.SetBinContent(iibin, rn.Poisson(tmpsumth.GetBinContent(iibin)) )
	#    if tmpsumth.GetBinContent(iibin) == 0:
	#	tmpsumth.SetBinError(iibin, 0 )
	#    else:
	#	tmpsumth.SetBinError(iibin, 1/math.sqrt(tmpsumth.GetBinContent(iibin)) )
			
	#tmpsumth.Draw("same E1")

	
	if printBanner:
	    tex = TLatex(0.35,0.95,Banner)
	    tex.SetNDC()
	    tex.SetTextSize(0.05)
	    tex.Draw()
	
	cv[thesuper[ikey].name].Update()
	#cv[thesuper[ikey].name].Print("test.png")
	
	# pause
	if option.wait:
	    raw_input( 'Press ENTER to continue\n ' )
	    
    if printCanvas:
	
	for ikey in theaddition:
	    cv[theaddition[ikey].name].Print(theaddition[ikey].name + "." + printFormat)
	for ikey in thesuper:
	    cv[thesuper[ikey].name].Print(thesuper[ikey].name + "." + printFormat)
	
    
    #outputroot.Write()
    #outputroot.Close()

#    if not option.wait:
    rep = ''
    while not rep in [ 'q', 'Q', '.q', 'qq' 'p']:
	rep = raw_input( '\nenter: ["q",".q" to quit] ["p" or "print" to print all canvas]: ' )
	if 0<len(rep):
	    if rep=='quit': rep = 'q'
	    if rep=='p' or rep=='print':
		for ikey in theaddition:
		    cv[theaddition[ikey].name].Print(theaddition[ikey].name + "." + printFormat)
		for ikey in thesuper:
		    cv[thesuper[ikey].name].Print(thesuper[ikey].name + "." + printFormat) 

		    

