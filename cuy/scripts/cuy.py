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
   -b, --batch : run script in batch mode.
   -e, --example = EXAMPLE: generate an example xml file.
   -l, --list    = LIST: list of objects in the ROOT file. 
   -p, --prt     = PRT: print canvas in the format specified png, ps, eps, pdf, etc.
   -x, --xml     = XML: xml configuration file.

   Francisco Yumiceva (yumiceva@fnal.gov)
   Fermilab 2008
   
"""


import os, string, re, sys

try:
    import ROOT
except:
    print "Cannot load PYROOT, make sure you have setup ROOT in the path"
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

from xml.sax import saxutils, make_parser, handler
from xml.sax.handler import feature_namespaces

import Inspector

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

class DivideElement:
    def __init__(self):
	self.name = ""
	self.N = None
	self.D = None

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
	if name == 'TH1':
	    self.data[self.atype].histos[attrs.get('name',None)] = attrs.get('source',None)
	    #print attrs.get('name',None)
	    #print attrs.get('source',None)
	if name == 'divide':
	    aname = attrs.get('name',None)
	    #self.divide[aname] = divideElement()
	    self.divide[aname].name = aname
	    self.divide[aname].numerator = attrs.get('numerator',None)
	    self.divide[aname].denominator = attrs.get('denominator',None)
	if name == 'addition':
	    aname = attrs.get('name',None)
	    self.addition[aname] = additionElement()
	    self.tmpaddname = aname
	    self.addition[aname].name = aname
	    self.addition[aname].title = attrs.get('title',None)
	    self.addition[aname].YTitle = attrs.get('YTitle',None)
	    self.addition[aname].XTitle = attrs.get('XTitle',None)
	    self.addition[aname].Option = attrs.get('Option',None)
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
	    self.superimpose[aname].profile = attrs.get('Profile',None)
	    self.superimpose[aname].Fill = attrs.get('Fill',None)
	    self.superimpose[aname].Option = attrs.get('Option',None)
	    self.tmpsupername = aname
	if name == 'superimposeItem':
	    #print "in element: " + self.tmpsupername
	    self.superimpose[self.tmpsupername].histos.append(attrs.get('name',None))
	    self.superimpose[self.tmpsupername].color.append(attrs.get('color',None))
	    self.superimpose[self.tmpsupername].marker.append(attrs.get('MarkerStyle',None))
	    self.superimpose[self.tmpsupername].legend.append(attrs.get('legend',None))
	    self.superimpose[self.tmpsupername].weight.append(attrs.get('weight',None))

if __name__ == '__main__':

    # style
    ROOT.gStyle.SetFrameBorderMode(0)
    ROOT.gStyle.SetCanvasBorderMode(0)
    ROOT.gStyle.SetPadBorderMode(0)

    ROOT.gStyle.SetPadColor(0)
    ROOT.gStyle.SetCanvasColor(0)
    ROOT.gStyle.SetTitleColor(1)
    ROOT.gStyle.SetStatColor(0)

    # set the paper & margin sizes
    ROOT.gStyle.SetPaperSize(20,26)
    ROOT.gStyle.SetPadTopMargin(0.04)
    ROOT.gStyle.SetPadRightMargin(0.04)
    ROOT.gStyle.SetPadBottomMargin(0.14)
    ROOT.gStyle.SetPadLeftMargin(0.16)
    ROOT.gStyle.SetPadTickX(1)
    ROOT.gStyle.SetPadTickY(1)
    
    
    ROOT.gStyle.SetTextFont(132)
    ROOT.gStyle.SetTextSize(0.09)
    ROOT.gStyle.SetLabelFont(132,"xyz")
    ROOT.gStyle.SetTitleFont(132,"xyz")
    ROOT.gStyle.SetLabelSize(0.035,"xyz")
    ROOT.gStyle.SetTitleSize(0.045,"xyz")
    ROOT.gStyle.SetTitleOffset(1.5,"y")
    
    # use bold lines and markers
    ROOT.gStyle.SetMarkerStyle(8)
    ROOT.gStyle.SetHistLineWidth(2)
    ROOT.gStyle.SetLineWidth(1)
    #ROOT.gStyle.SetLineStyleString(2,"[12 12]") // postscript dashes

    # do not display any of the standard histogram decorations
    ROOT.gStyle.SetOptTitle(0)
    ROOT.gStyle.SetOptStat(0) #("m")
    ROOT.gStyle.SetOptFit(0)
    
    #ROOT.gStyle.SetPalette(1,0)
    ROOT.gStyle.cd()
    ROOT.gROOT.ForceStyle()

    printCanvas = False
    printFormat = "png"
    # check options
    option,args = parse(__doc__)
    if not args and not option: exit()

    if option.list:
	ins = Inspector.Inspector()
	ins.SetFilename(option.list)
	ins.GetListObjects()
	sys.exit()

    if not option.xml: exit()
    if option.prt: 
	printCanvas = True
	printFormat = option.prt

    

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
	arelease = thedata[ikey].release
	print "== filename: " + afilename
	print "== release:  " + arelease
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
		aweight = float(listweight[ihnameIt])
	    for jkey in thedata:
		tmpkeys = thedata[jkey].histos.keys()
		for tmpname in tmpkeys:
		    if tmpname == ihname:
			ath = thedata[jkey].TH1s[tmpname]
			if ath is None:
			    print "ERROR: histogram name \""+tmpname+"\" does not exist in file "+thedata[jkey].filename
			    exit(0)
			print "=== add histogram: "+ath.GetName() + " from " + thedata[jkey].filename + " mean = " + "%.2f" % round(ath.GetMean(),2) + " weight= " + str(aweight)

			if isFirst:
			    newth = ath.Clone()
			    newth.Scale(aweight)
			    isFirst = False
			else:
			    atmpth = ath.Clone()
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

	cv[theaddition[ikey].name].Update()

	# add new histogram to the list
	newth.SetName(theaddition[ikey].name)
	newTH1list.append(newth.GetName())
	thedata[newth.GetName()] = ValElement()
	thedata[newth.GetName()].TH1s[newth.GetName()] = newth
	thedata[newth.GetName()].histos[newth.GetName()] = newth.GetName()

    thesuper = dh.superimpose
    print "= Create superimpose histograms:"
    for ikey in thesuper:
	print "== plot name: \""+thesuper[ikey].name+"\" title: \""+thesuper[ikey].title+"\""
	listname = thesuper[ikey].histos
	listcolor = thesuper[ikey].color
	listmarker = thesuper[ikey].marker
	listlegend = thesuper[ikey].legend
	listweight = thesuper[ikey].weight
	dolegend = True
	for il in listlegend:
	    if il==None: dolegend = False
	print "dolegend = " +str(dolegend)
	doNormalize = False
	if thesuper[ikey].Normalize == "true":
	    doNormalize = True
	    print "normalize = " +str(doNormalize)
	projectAxis = "no"
	if thesuper[ikey].projection == "x": projectAxis = "x"
	if thesuper[ikey].projection == "y": projectAxis = "y"
	profileAxis = "no"
	if thesuper[ikey].profile == "x": profileAxis = "x"
	if thesuper[ikey].profile == "y": profileAxis = "y"
	doFill = False
	if thesuper[ikey].Fill == "true": doFill = True
	print doFill
	#create canvas
	cv[thesuper[ikey].name] = TCanvas(thesuper[ikey].name,thesuper[ikey].title,700,700)
	#legend
	aleg = TLegend(0.6,0.2,0.8,0.4)
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
			    newthpx = ath.ProjectionX(ath.GetName()+"_px",0,-1,"e")
			    newth = newthpx.Clone()
			if projectAxis == "y":
			    newthpy = ath.ProjectionY(ath.GetName()+"_py",0,-1,"e")
			    newth = newthpy.Clone()
			if profileAxis == "x":
			    newthpx = ath.ProfileX(ath.GetName()+"_px",0,-1,"e")
			    newth = newthpx.Clone()
			if profileAxis == "y":
			    newthpy = ath.ProfileY(ath.GetName()+"_py",0,-1,"e")
			    newth = newthpy.Clone()
			
			# get weight
			aweight = 1
			if listweight[ii]:
			    aweight = float( listweight[ii] )

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
			
			# stack histograms
			if doFill:
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
			#    newth.SetTitle(thesuper[ikey].title)
			#    if thesuper[ikey].YTitle != None:
			#	newth.SetYTitle(thesuper[ikey].YTitle)
			#    newth.Draw()
			#    isFirst=0
			#else:
			#    newth.Draw("same")
			if dolegend: aleg.AddEntry(newth,listlegend[ii],"P")
	    ii = ii + 1
	if thesuper[ikey].Stack != None:
	    astack.Draw()
	else:
	    astack.Draw("nostack")
	if thesuper[ikey].XTitle != None:
	    astack.GetHistogram().SetXTitle(thesuper[ikey].XTitle)
	if thesuper[ikey].YTitle != None:
	    astack.GetHistogram().SetYTitle(thesuper[ikey].YTitle)
	if dolegend: 
	    aleg.Draw()
	if thesuper[ikey].SetLogy == "true":
	    cv[thesuper[ikey].name].SetLogy()
	if thesuper[ikey].SetGrid == "true":
	    cv[thesuper[ikey].name].SetGrid()
	
	cv[thesuper[ikey].name].Update()
	#cv[thesuper[ikey].name].Print("test.png")

    if printCanvas:
	
	for ikey in theaddition:
	    cv[theaddition[ikey].name].Print(theaddition[ikey].name + "." + printFormat)
	for ikey in thesuper:
	    cv[thesuper[ikey].name].Print(thesuper[ikey].name + "." + printFormat)
	
    
    if not option.batch:
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
    


