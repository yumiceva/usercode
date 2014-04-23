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
   -b, --batch : run in batch mode without graphics.
   -c, --create  = CREATE: create XML configuration file from a ROOT file.
   -e, --example = EXAMPLE: generate an example xml file.
   -f, --flag    = FLAG: create a baneer
   -l, --list    = LIST: list of objects in the ROOT file.
   -O, --OutputFilename = OUTPUTFILENAME: name of the output ROOT file. 
   -o, --output  = OUTPUT: output directory
   -p, --prt     = PRT: print canvas in the format specified png, ps, eps, pdf, etc.
   -q, --quit  : quit after producing plots.
   -t, --tag     = TAG: tag name for XML configuration file.
   -v, --verbose : verbose output.
   -w, --wait : Pause script after plotting a new superposition of histograms.
   -x, --xml     = XML: xml configuration file.
   
   Francisco Yumiceva (yumiceva@fnal.gov)
   Fermilab 2008
   
"""


import os
import string
import re
import sys
import math
from array import array

try:
    from ROOT import *
except:
    print "\nCannot load PYROOT, make sure you have setup ROOT in the path"
    print "and pyroot library is also defined in the variable PYTHONPATH, try:\n"
    if (os.getenv("PYTHONPATH")):
	print " setenv PYTHONPATH ${PYTHONPATH}:$ROOTSYS/lib\n"
    else:
	print " setenv PYTHONPATH $ROOTSYS/lib\n"
    sys.exit()

#from ROOT import TFile
#from ROOT import TCanvas
#from ROOT import TLegend
#from ROOT import SetOwnership
#from ROOT import THStack
#from ROOT import TLatex

from xml.sax import saxutils, make_parser, handler
from xml.sax.handler import feature_namespaces

from usercode.cuy.Inspector import *
from usercode.cuy.Style import *


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

class additionArrayElement:
    def __init__(self):
        self.name = ""
        self.title = ""
        self.array = []
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
        self.SF = []
        self.Error = []
        self.Norm = []
        self.NoStack = []
        
class FindIssue(handler.ContentHandler):
    def __init__(self):
	self.data = {}
	self.divide = {}
	self.addition = {}
        self.additionArray = {}
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
            self.divide[aname].YTitle = attrs.get('YTitle',None)
            self.divide[aname].XTitle = attrs.get('XTitle',None)                        
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
            self.addition[aname].Lumi = attrs.get('Lumi',None)
	if name == 'additionItem':
	    #print "in element: " + self.tmpsupername
	    self.addition[self.tmpaddname].histos.append(attrs.get('name',None))
	    self.addition[self.tmpaddname].weight.append(attrs.get('weight',None))
        if name == 'additionArray':
            aname = attrs.get('name',None)
            self.additionArray[aname] = additionArrayElement()
            self.tmpaddname = aname
            self.additionArray[aname].name = aname
            self.additionArray[aname].title = attrs.get('title',None)
        if name == 'additionArrayItem':
            #print "in element: " + self.tmpsupername
            self.additionArray[self.tmpaddname].array.append(attrs.get('array',None))
            self.additionArray[self.tmpaddname].weight.append(attrs.get('weight',None))
                                                
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
	    self.superimpose[aname].Minimum = attrs.get('Minimum',None)
	    self.superimpose[aname].Labels = attrs.get('Labels',None)
            self.superimpose[aname].Lumi = attrs.get('Lumi',None)
            self.superimpose[aname].SubBanner = attrs.get('SubBanner',None)
            self.superimpose[aname].Ndivisions = attrs.get('Ndivisions',None)
            self.superimpose[aname].PlotDiff = attrs.get('PlotDiff',None)
            self.superimpose[aname].doKS = attrs.get('doKS',None)
	    self.tmpsupername = aname
	if name == 'superimposeItem':
	    #print "in element: " + self.tmpsupername
	    self.superimpose[self.tmpsupername].histos.append(attrs.get('name',None))
	    self.superimpose[self.tmpsupername].color.append(attrs.get('color',None))
	    self.superimpose[self.tmpsupername].marker.append(attrs.get('MarkerStyle',None))
	    self.superimpose[self.tmpsupername].legend.append(attrs.get('legend',None))
	    self.superimpose[self.tmpsupername].SF.append(attrs.get('SF',None))
            self.superimpose[self.tmpsupername].Error.append(attrs.get('Error',None))
            self.superimpose[self.tmpsupername].Norm.append(attrs.get('Normalize',None))
            self.superimpose[self.tmpsupername].NoStack.append(attrs.get('NoStack',None))
            
if __name__ == '__main__':



    # style
    thestyle = Style()

    HasCMSStyle = False
    style = None
    if os.path.isfile('tdrstyle.C'):
        gROOT.ProcessLine('.L tdrstyle.C')
        ROOT.setTDRStyle()
        print "Found tdrstyle.C file, using this style."
        HasCMSStyle = True
        if os.path.isfile('CMSTopStyle.cc'):
            gROOT.ProcessLine('.L CMSTopStyle.cc+')
            style = CMSTopStyle()
            style.setupICHEPv1()
            print "Found CMSTopStyle.cc file, use TOP style if requested in xml file."
    if not HasCMSStyle:
        print "Using default style defined in cuy package."
        thestyle.SetStyle()

    gROOT.ForceStyle()
    
    printCanvas = False
    printFormat = "png"
    printBanner = False
    Banner = "#splitline{CMS Preliminary}"
    verbose = False
    OutputDir = "./"
    
    # check options
    option,args = parse(__doc__)
    if not args and not option: exit()

    if option.batch:
	ROOT.gROOT.SetBatch()

    if option.verbose:
	verbose = True

    if option.output:
        OutputDir = option.output
        
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
    outputroot_name = "cuy.root"
    if option.OutputFilename: outputroot_name = option.OutputFilename
    
    outputroot = TFile(outputroot_name,"RECREATE")

    # new histograms
    newTH1list = []

    # extract histograms
    thedata = dh.data

    firstFilename = ''

    for ikey in thedata:
	if verbose : print "= Processing set called: " + ikey
	afilename = thedata[ikey].filename
	if firstFilename == '':
	    firstFilename = afilename
	arelease = ""
	if thedata[ikey].release != None:
	    arelease = thedata[ikey].release
	if verbose : print "== filename: " + afilename
	if verbose : print "== release:  " + arelease
	if verbose : print "== weight:   " + thedata[ikey].weight
	thehistos = thedata[ikey].histos
	afilelist[afilename] = TFile(afilename)
	if verbose : print "== get histograms: "
	histonamekeys = thehistos.keys()
	for ihname in histonamekeys:
	    if verbose : print "=== Histogram name: \""+ ihname + "\" source: \""+thehistos[ihname]+"\""
	    thedata[ikey].TH1s[ihname] = ROOT.gDirectory.Get(thehistos[ihname])
            if not thedata[ikey].TH1s[ihname]:
                print "ERROR: histogram name: "+thehistos[ihname]+" does not exist."
                
	    #SetOwnership(thedata[ikey].TH1s[ihname], 0)
	    # check if file exists
	    #print thedata[ikey].TH1s[ihname].GetEntries()


    # plot superimpose histograms
    #afilelist['../outputLayer2_ttbarmuonic_all.root'].cd()
    afilelist[firstFilename].cd()
    #print thedata['ttbar'].TH1s['gen_eta'].GetEntries()


    #### ADDITION ARRAY
    theadditionArray = dh.additionArray
    
    if verbose : print "= Create addition arrays:"

    for ikey in theadditionArray:
        if verbose : print "== block name: \""+theadditionArray[ikey].name+"\" title: \""+theadditionArray[ikey].title+"\""
        listarray = theadditionArray[ikey].array
        listweight = theadditionArray[ikey].weight
        #print listweight
        
        # get first list of histos to add
        tmplisthistos = thedata[ listarray[0] ].histos.keys()
        new_tmplisthistos = []
        for isuffix in range(0, len(tmplisthistos)):
            tmpname = ''
            if tmplisthistos[isuffix].find("__")!=-1:
                blist = tmplisthistos[isuffix].split('__')
                for iblist in range(0,len(blist)-1):
                    tmpname += blist[iblist] +"__"
            else:
                if tmplisthistos[isuffix].count('_') % 2 == 0:
                    tmpname = tmplisthistos[isuffix]
                    tmpname = tmpname[0:tmpname.rfind('_')]
                    tmpname += '_'
                else:
                    tmpname = tmplisthistos[isuffix]
                    tmpname = tmpname[0:tmpname.rfind('_')]
                    tmpname = tmpname[0:tmpname.rfind('_')]
                    tmpname += '_'
                #blist = tmplisthistos[isuffix].split('_')
                #for iblist in range(0,len(blist)-1):
                #    tmpname += blist[iblist] +"_"
            #tmpname = tmplisthistos[isuffix].strip(listarray[0])
            new_tmplisthistos.append( tmpname )
        tmplisthistos = new_tmplisthistos
        if verbose:
            print "  histogram names to be added: "
            print tmplisthistos
        
        for namehisto in tmplisthistos:

            isFirst = True
            # loop over samples
            iisample = 0
            for jsample in listarray:

                the_namehisto = namehisto+jsample
                tmp_weight = 1.
                if listweight[iisample]:
                    tmp_weight = float(listweight[iisample])
                #print tmp_weight
                # first check if SF/weight is given as a file and check the key
                the_big_weight = 1
                if type(thedata[jsample].weight) is not float and len(thedata[jsample].weight.split(":"))>1:
                    keySF = thedata[jsample].weight.split(":")[1]
                    if os.path.isfile(thedata[jsample].weight.split(":")[0]):
                        SFfile = open(thedata[jsample].weight.split(":")[0])
                        for sfline in SFfile:
                            if sfline.find("#")!=-1: continue
                            if sfline.find(keySF)!=-1:
                                the_big_weight = float(sfline.split()[1])/float(sfline.split()[2])
                                if verbose: print "SF computed as "+str(sfline.split()[1])+" / "+str(sfline.split()[2])+" = "+str(the_big_weight)
                                break
                else: the_big_weight = thedata[jsample].weight
                                                                                                        
                aweight = float( the_big_weight ) * tmp_weight
                if verbose: print " get histogram name: "+the_namehisto+" from sample: "+ jsample

                
                ath = thedata[jsample].TH1s[the_namehisto]
                if ath is None:
                    print "ERROR: histogram name \""+tmpname+"\" does not exist in file "+thedata[jkey].filename
                if verbose :
                    print "=== histogram from sample: "+jsample
                    print "=== add histogram: "+ath.GetName() + " from " + thedata[jsample].filename + " mean = " + "%.2f" % round(ath.GetMean(),2) + " weight= " + str(aweight)
                if isFirst:
                    newth = ath.Clone(namehisto + theadditionArray[ikey].name)
                    newth.SetName(namehisto + theadditionArray[ikey].name)
                    if verbose: print "=== new histogram name: "+ newth.GetName()
                    newth.Sumw2()
                    #newth.Scale(1/newth.Integral())
                    newth.Scale(aweight)
                    isFirst = False
                else:
                    atmpth = ath.Clone()
                    atmpth.Sumw2()
                    #if theaddition[ikey].Normalize == "true":
                    #    atmpth.Scale(1/atmpth.Integral())
                    atmpth.Scale(aweight)
                    newth.Add( atmpth )
                    
                iisample += 1
            # add new histogram to the list
            #newth.SetName(theadditionArray[ikey].name)
            
            newTH1list.append(newth.GetName())
            thedata[newth.GetName()] = ValElement()
            thedata[newth.GetName()].TH1s[newth.GetName()] = newth
            thedata[newth.GetName()].histos[newth.GetName()] = newth.GetName()
            thedata[newth.GetName()].weight = 1.
            thedata[newth.GetName()].filename = "memory"
            if verbose: print "=== new histogram added to list: "+ newth.GetName() + " integral: "+str(newth.Integral())

    # ADDITION HISTOGRAM
    theaddition = dh.addition
    tmp_listtheaddition = theaddition.keys()
    tmp_listtheaddition.sort()
    tmp_listtheaddition.reverse()
    if verbose : print "= Create addition histograms:"; print tmp_listtheaddition
    
    for ikey in tmp_listtheaddition:
	if verbose : print "== plot name: \""+theaddition[ikey].name+"\" title: \""+theaddition[ikey].title+"\""
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
                
                if theaddition[ikey].Lumi != None:
                    aweight = aweight * float(theaddition[ikey].Lumi)
                    # check if we have additianal SF
                    #aSF = 1.
                    #if listSF[ii] != None:
                    #    tmpSF = listSF[ii]
                    #    if tmpSF.find('*') != -1:
                    #        tmpSFlist = tmpSF.split('*')
                    #        for i_tmpSF in tmpSFlist:
                    #            aSF *= float(i_tmpSF)
                    #        else:
                    #            aSF = tmpSF
                    #            
                    #aweight = aweight * aSF
                                    
                                                                                                                                                                                                                                                                                                                                                                                            
	    for jkey in thedata:
		tmpkeys = thedata[jkey].histos.keys()
		for tmpname in tmpkeys:
		    if tmpname == ihname:
			ath = thedata[jkey].TH1s[tmpname]
			if ath is None:
			    print "ERROR: histogram name \""+tmpname+"\" does not exist in file "+thedata[jkey].filename
			    exit(0)
                                                    
			if verbose : print "=== add histogram: "+ath.GetName() + " from " + thedata[jkey].filename + " mean = " + "%.2f" % round(ath.GetMean(),2) + " weight= " + str(aweight) + " integral= " +str(ath.Integral())
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
        thedata[newth.GetName()].weight = 1.
	# write new histograms to file
	outputroot.cd()
	#newth.Write()
        if verbose: print "=== new histogram added to list: "+ newth.GetName() + " integral: "+str(newth.Integral())

                                                   
    # DIVIDE    
    if verbose : print "= Create ratio histograms:"
    
    thedivition = dh.divide
    for ikey in thedivition:
	if verbose : print "== plot name: \""+thedivition[ikey].name+"\" title: \""+"\""
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

        #numeratorth.Rebin(4)
        #denominatorth.Rebin(4)
        
	newth = numeratorth.Clone("ratio")
	newth.Clear()
	if thedivition[ikey].DivideOption is None:
	    newth.Divide(numeratorth,denominatorth)
        elif thedivition[ikey].DivideOption == "TGraph":
            newth = TGraphAsymmErrors( denominatorth )
            newth.Divide( numeratorth, denominatorth, "cl=0.683 b(1,1) mode pois")
	else:
	    newth.Divide(numeratorth,denominatorth,1.,1.,thedivition[ikey].DivideOption)

        if thedivition[ikey].DivideOption != "TGraph":
            if thedivition[ikey].XTitle != None:
                newth.SetXTitle(thedivition[ikey].XTitle)
            if thedivition[ikey].YTitle != None:
                newth.SetYTitle(thedivition[ikey].YTitle)

                
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
    if verbose : print "= Create superimpose histograms:"
    datahist = {}
    tmpNoStackhist = {}
    ratiohist = {}
    errorgraph = {}
    tex = {}
    for ikey in thesuper:
	if verbose : print "== plot name: \""+thesuper[ikey].name+"\" title: \""+thesuper[ikey].title+"\""
	listname = thesuper[ikey].histos
	listcolor = thesuper[ikey].color
	listmarker = thesuper[ikey].marker
	listlegend = thesuper[ikey].legend
	listSF = thesuper[ikey].SF
        listError = thesuper[ikey].Error
        doPlotError = False
        tmplistcolor = []
        tmplistlegend = []
        orglistlegend = listlegend
        for icolor in xrange(0,len(listcolor)):
            if listcolor[icolor] == "top" and listlegend[icolor]=="Data":
                tmplistcolor.append('1')
                tmplistlegend.append("Data")                                
            if listcolor[icolor] == "top" and listlegend[icolor]=="TTbar":
                tmplistcolor.append( style.TtbarColor)
                tmplistlegend.append(style.TtbarText)
            if listcolor[icolor] == "top" and listlegend[icolor]=="Wjets":
                tmplistcolor.append( style.WJetsColor)
                tmplistlegend.append(style.WJetsText)
            if listcolor[icolor] == "top" and listlegend[icolor]=="Zjets":
                tmplistcolor.append( style.DYZJetsColor)
                tmplistlegend.append(style.DYZJetsText)
            if listcolor[icolor] == "top" and listlegend[icolor]=="QCD":
                tmplistcolor.append( style.QCDColor)
                tmplistlegend.append(style.QCDText)
            if listcolor[icolor] == "top" and listlegend[icolor]=="ST":
                tmplistcolor.append( style.SingleTopColor)
                tmplistlegend.append(style.SingleTopText)
            if listcolor[icolor] == "top" and listlegend[icolor]=="STtch":
                tmplistcolor.append( style.ST_t_sColor)
                tmplistlegend.append(style.ST_t_sText)
            if listcolor[icolor] == "top" and listlegend[icolor]=="Dibosons":
                tmplistcolor.append( 10 ) #style.DibosonsColor)
                tmplistlegend.append(style.DibosonsText)
                #print "dibosons color ="+str(style.DibosonsColor)
            if listcolor[icolor] != "top":
                tmplistcolor.append( int(listcolor[icolor]) ) 
                tmplistlegend.append( listlegend[icolor])
                                
        if len(tmplistcolor)>0:
            listcolor = tmplistcolor
            listlegend = tmplistlegend
        
	dolegend = True
	for il in listlegend:
	    if il==None: dolegend = False
	if verbose : print "dolegend = " +str(dolegend)
	doNormalize = False
	if thesuper[ikey].Normalize == "true":
	    doNormalize = True
	    if verbose : print "normalize = " +str(doNormalize)
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
	if verbose : print "fill option:"+ str(doFill)
	#create canvas
	cv[thesuper[ikey].name] = TCanvas(thesuper[ikey].name,thesuper[ikey].title,700,700)
	#legend
	aleg = TLegend(0.66,0.65,0.93,0.93)
	SetOwnership( aleg, 0 ) 
	aleg.SetMargin(0.12)
        aleg.SetTextSize(0.035)
        aleg.SetFillColor(0)
        aleg.SetFillStyle(0)
	aleg.SetBorderSize(0)

        # check if diff plot requested
        makeDiffPlot = False
        doKS = False
        if thesuper[ikey].PlotDiff == "true":
            makeDiffPlot = True
            if thesuper[ikey].doKS == "true": doKS = True
            #cv[thesuper[ikey].name+"_diff"] = TCanvas(thesuper[ikey].name+"_diff",thesuper[ikey].title,700,700)
            #cv[thesuper[ikey].name].cd()
            pad1 = TPad("pad1","histogram",0.01,0.25,0.99,0.99)
            pad2 = TPad("pad2","ratio",0.01,0.01,0.99,0.24)
            pad1.Draw()
            pad2.Draw()
            pad1.cd()
            
	isFirst = 1
	ii = 0

	stacklist[thesuper[ikey].name] = THStack("astack"+thesuper[ikey].name,thesuper[ikey].title)
	astack = stacklist[thesuper[ikey].name]
        astack2 = None # subset of histograms to superimpose
        datahist[thesuper[ikey].name] = None
        tmpNoStackhist[thesuper[ikey].name] = []
        ratiohist[thesuper[ikey].name] = None # for plot diff
        errorgraph[thesuper[ikey].name] = None # error band
        thisistheMax = 0.
        defaultXTitle = ""
        xarray = array('d')
        yarray = array('d')
        xerr_array = array('d')
        yerr_array = array('d')

	for ihname in listname:
	
	    for jkey in thedata:
		tmpkeys = thedata[jkey].histos.keys()
		
		for tmpname in tmpkeys:
		
		    if tmpname == ihname:
			ath = thedata[jkey].TH1s[tmpname]
			if ath is None:
			    print "ERROR: histogram name \""+tmpname+"\" does not exist in file "+thedata[jkey].filename
			    exit(0)
			if verbose : print "=== superimpose histogram: "+ath.GetName() + " mean = " + "%.2f" % round(ath.GetMean(),2)
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
                        SFforallbins = {} #used for jet bins
                                                                                                                                    
			if thedata[jkey].weight != None and thesuper[ikey].Weight=="true":

                            # first check if SF/weight is given as a file and check the key
                            the_big_weight = 1
                            if type(thedata[jkey].weight) is not float and len(thedata[jkey].weight.split(":"))>1:
                                keySF = thedata[jkey].weight.split(":")[1]
                                if os.path.isfile(thedata[jkey].weight.split(":")[0]):
                                    SFfile = open(thedata[jkey].weight.split(":")[0])
                                    for sfline in SFfile:
                                        if sfline.find("#")!=-1: continue
                                        if sfline.find(keySF)!=-1:
                                            #the_big_weight = sfline.split()[1]
                                            the_big_weight = float(sfline.split()[1])/float(sfline.split()[2])
                                            if verbose: print "SF computed as "+str(sfline.split()[1])+" / "+str(sfline.split()[2])+" = "+str(the_big_weight)
                                            break
                            else: the_big_weight = thedata[jkey].weight
                            #print " WEIGHT==="+the_big_weight
			    aweight = float( the_big_weight )
                            
                            if thesuper[ikey].Lumi != None:
                                aweight = aweight * float(thesuper[ikey].Lumi)
                                # check if we have additianal SF
                                aSF = 1.
                                
                                if listSF[ii] != None:
                                    # SF enable
                                    tmpSF =listSF[ii]
                                    keySF = ""
                                    # first check if SF is given as a file and check the key
                                    if len(listSF[ii].split(":"))>1:
                                        keySF = listSF[ii].split(":")[1]
                                        if os.path.isfile(listSF[ii].split(":")[0]):
                                            SFfile = open(listSF[ii].split(":")[0])
                                            for sfline in SFfile:
                                                if sfline.find("#")!=-1: continue
                                                if keySF.find("all")!=-1:
                                                    tmpSF = "1"                                                    
                                                    # get SF for all jet bins
                                                    tmpallkey = keySF.strip("all")
                                                    
                                                    if sfline.find(orglistlegend[ii])!=-1 and sfline.find("1jet"+tmpallkey)!=-1: SFforallbins[2]=sfline.split()[2]
                                                    if sfline.find(orglistlegend[ii])!=-1 and sfline.find("2jet"+tmpallkey)!=-1: SFforallbins[3]=sfline.split()[2]
                                                    if sfline.find(orglistlegend[ii])!=-1 and sfline.find("3jet"+tmpallkey)!=-1: SFforallbins[4]=sfline.split()[2]
                                                    if sfline.find(orglistlegend[ii])!=-1 and sfline.find("4jet"+tmpallkey)!=-1: SFforallbins[5]=sfline.split()[2]
                                                
                                                elif sfline.find(orglistlegend[ii])!=-1 and sfline.find(keySF)!=-1:
                                               
                                                     tmpSF = sfline.split()[2]
                                                     break
                                    
                                    if tmpSF.find('*') != -1:
                                        tmpSFlist = tmpSF.split('*')
                                        for i_tmpSF in tmpSFlist:
                                            aSF *= float(i_tmpSF)
                                    else:
                                        aSF = float(tmpSF)

                                aweight = aweight * aSF
                                if verbose:
                                    print " with gloabl SF = "+str(aSF)
                                    print " with binned SF = "
                                    print SFforallbins
                                if listlegend[ii]=="Data": aweight = 1.
                                
                                #print "aweight="+str(aweight)
                                #if ath.GetName().find("data")!=-1: aweight = aSF
                                
			if verbose: print " with weight = " + str(aweight)
			#if listweight[ii]:
			 #   aweight = float( listweight[ii] )

			# clone original histogram
			if projectAxis == "no" and profileAxis == "no" : newth = ath.Clone()

                        if newth.InheritsFrom("TH1"):
                            newth.Sumw2()
                        listofnorm = thesuper[ikey].Norm
                        #print listofnorm
                        if listofnorm[ii] == "true":
                            newth.Scale(1./newth.Integral())
                            if verbose: print " histogram has been normalized before applying weight"
                            
                        if newth.InheritsFrom("TH1"): newth.Scale(aweight)
			
			# check if we have color
			if not listcolor[ii]:
			    listcolor[ii] = 1
			
			#newth.SetLineColor(int(listcolor[ii]))
			newth.SetMarkerColor(int(listcolor[ii]))
			
			if doFill: newth.SetFillColor(int(listcolor[ii]))
                        else:
                            newth.SetLineColor(int(listcolor[ii]))
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
                                # scale if binned SF
                                if SFforallbins.has_key(ib):
                                    if listofnorm[ii] == "true":
                                        tmpcontent = float(SFforallbins[ib])
                                    else: 
                                        tmpcontent = newth.GetBinContent(ib) * float(SFforallbins[ib])
                                    newth.SetBinContent(ib, tmpcontent)
				#if ib==1:
				    #newth.GetXaxis().SetBinLabel(ib,"")
				#newth.GetHistogram().GetXaxis().SetBinLabel(ib,ilabel)
				ib += 1

                        # get error band
                        if listError[ii] != None:
                            doPlotError = True
                            lxx = []
                            lyy = []
                            lerrX = []
                            lerrY = []
                            for iibin in range(1,newth.GetNbinsX()+1):
                                content = newth.GetBinContent(iibin)*float(listError[ii])
                                lxx.append( newth.GetBinCenter( iibin ) )
                                lyy.append( newth.GetBinContent(iibin) )
                                lerrX.append( 0.5*newth.GetBinWidth(iibin) )
                                lerrY.append( content )
                            
                            xarray.fromlist( lxx)
                            xerr_array.fromlist( lerrX )
                            yerr_array.fromlist( lerrY )
                            
                            
			# stack histograms
			if doFill:
			    if thesuper[ikey].XTitle != None:
				newth.SetXTitle("")
                            if listlegend[ii]=="Data": datahist[thesuper[ikey].name] = newth
                            elif thesuper[ikey].NoStack[ii] != None:
                                #print "HEEEEEEEEEEEERE: "
                                #print thesuper[ikey].NoStack[ii]
                                newth.SetFillColor(0)
                                tmpNoStackhist[thesuper[ikey].name].append(newth)
                            else:
                                astack.Add(newth,"HIST")
			elif thesuper[ikey].Option:
			    astack.Add(newth,thesuper[ikey].Option)
			else:
			    #newth.Fit("landau")
                            if listlegend[ii]=="Data": datahist[thesuper[ikey].name] = newth
			    astack.Add(newth)
			    
			astack.SetTitle(thesuper[ikey].title)
			
			if isFirst==1:
			    newth.GetPainter().PaintStat(ROOT.gStyle.GetOptStat(),0);
			    isFirst=0
			    tmpsumth = newth.Clone()
                            defaultXTitle = newth.GetXaxis().GetTitle()
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
                            if listlegend[ii]=="Data": aleg.AddEntry(newth,listlegend[ii],"PL")
                            else: aleg.AddEntry(newth,listlegend[ii],"F")
			elif dolegend:
			    aleg.AddEntry(newth,listlegend[ii],"P")

			if newth.GetMaximum()>= thisistheMax: thisistheMax=newth.GetMaximum()
			newth.SetName(tmpname)
			outputroot.cd()
			newth.Write()
	    ii = ii + 1
	if thesuper[ikey].Maximum != None:
	    astack.SetMaximum( float(thesuper[ikey].Maximum) )
        else: astack.SetMaximum( thisistheMax * 1.55 )
	if thesuper[ikey].Minimum != None:
	    astack.SetMinimum( float(thesuper[ikey].Minimum) )
	if thesuper[ikey].Stack == "true":
	    astack.Draw()
            if thesuper[ikey].Ndivisions !=None: astack.GetHistogram().GetXaxis().SetNdivisions( int(thesuper[ikey].Ndivisions) )
            astack.Draw()
	if thesuper[ikey].Stack == "false" or thesuper[ikey].Stack == None:
	    astack.Draw()
	    astack.Draw("nostack")
            if thesuper[ikey].Ndivisions !=None: astack.GetHistogram().GetXaxis().SetNdivisions( int(thesuper[ikey].Ndivisions) )
	if thesuper[ikey].XTitle != None:
	    astack.GetHistogram().SetXTitle(thesuper[ikey].XTitle)
            if datahist[thesuper[ikey].name] != None: datahist[thesuper[ikey].name].SetXTitle(thesuper[ikey].XTitle)
            for iitmp in tmpNoStackhist[thesuper[ikey].name]:
                iitmp.SetXTitle(thesuper[ikey].XTitle)
                                                
        else:
            astack.GetHistogram().SetXTitle(defaultXTitle)
            if datahist[thesuper[ikey].name] != None: datahist[thesuper[ikey].name].SetXTitle(defaultXTitle)
            for iitmp in tmpNoStackhist[thesuper[ikey].name]:
                iitmp.SetXTitle(defaultXTitle)
	if thesuper[ikey].YTitle != None:
	    astack.GetHistogram().SetYTitle(thesuper[ikey].YTitle)
            if datahist[thesuper[ikey].name] != None: datahist[thesuper[ikey].name].SetYTitle(thesuper[ikey].YTitle)
        else:
            astack.GetHistogram().SetYTitle("Events/"+str( int(astack.GetHistogram().GetBinWidth(1) ) ) +" GeV" )
            if datahist[thesuper[ikey].name] != None: datahist[thesuper[ikey].name].SetYTitle("Events/"+str( int(datahist[thesuper[ikey].name].GetBinWidth(1) ) ) +" GeV" )
	if doFill:
            if datahist[thesuper[ikey].name] != None:
#                if thesuper[ikey].Ndivisions !=None: datahist[thesuper[ikey].name].GetXaxis().SetNdivisions( int(thesuper[ikey].Ndivisions) )
                datahist[thesuper[ikey].name].Draw("p same")
                if thesuper[ikey].Ndivisions !=None: datahist[thesuper[ikey].name].GetXaxis().SetNdivisions( int(thesuper[ikey].Ndivisions) )
	    #astack.Draw("same") #sameaxis")
            #if thesuper[ikey].Ndivisions !=None: astack.GetHistogram().GetXaxis().SetNdivisions( int(thesuper[ikey].Ndivisions) )
            #if tmpNoStackhist[thesuper[ikey].name] != None:
            jjtmp = 0
            for iitmp in tmpNoStackhist[thesuper[ikey].name]:
                #tmpNoStackhist[thesuper[ikey].name].Draw("HIST same")
                iitmp.SetLineStyle(jjtmp+1)
                iitmp.Draw("HIST same")
                jjtmp+=1
        gPad.RedrawAxis()

        if doPlotError:
            lyy = []
            for iibin in range(1,astack.GetStack().Last().GetNbinsX()+1):
                content = astack.GetStack().Last().GetBinContent(iibin)
                lyy.append( content )
            yarray.fromlist( lyy)   
            errorgraph[thesuper[ikey].name] = TGraphErrors(astack.GetStack().Last().GetNbinsX(), xarray, yarray, xerr_array, yerr_array)
            errorgraph[thesuper[ikey].name].SetName(thesuper[ikey].name)
            errorgraph[thesuper[ikey].name].SetFillColor(920+3)
            errorgraph[thesuper[ikey].name].SetFillStyle(3354)
            errorgraph[thesuper[ikey].name].Draw("E2 same")
            
            aleg.AddEntry(errorgraph[thesuper[ikey].name],"QCD uncert.","F")
	#astack.GetHistogram().GetXaxis().SetTickLength(0)
         
        #astack.GetHistogram().GetXaxis().SetLabelOffset(999)
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
            if makeDiffPlot:
                pad1.SetLogy()
            else:
                cv[thesuper[ikey].name].SetLogy()
            #gPad.SetLogy()
	if thesuper[ikey].SetGrid == "true":
            if makeDiffPlot:
                pad1.SetGrid()
            else:
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
            newBanner = Banner
            if thesuper[ikey].SubBanner != None:
                newBanner = '#splitline{'+Banner+'}{'+thesuper[ikey].SubBanner+'}'
                if verbose: print "add sub banner"
	    tex[thesuper[ikey].name] = TLatex(0.28,0.85,newBanner)
	    tex[thesuper[ikey].name].SetNDC()
	    tex[thesuper[ikey].name].SetTextSize(0.035)
	    tex[thesuper[ikey].name].Draw()
	
	cv[thesuper[ikey].name].Update()

        if makeDiffPlot and datahist[thesuper[ikey].name] != None:
            
            ratiohist[thesuper[ikey].name+"_diff"] = datahist[thesuper[ikey].name].Clone(thesuper[ikey].name+"_diff")
            ratiohist[thesuper[ikey].name+"_diff"].Reset()
                    
            if astack.GetStack().Last().GetEntries() != 0:
                if len(astack.GetStack() ) ==2:
                    atemphist = astack.GetStack().Last()
                    atemphist.Add(datahist[thesuper[ikey].name],-1.)
                    ratiohist[thesuper[ikey].name+"_diff"].Divide(datahist[thesuper[ikey].name], atemphist )
                else:
                    # get KS value
                    if doKS: KSvalue = datahist[thesuper[ikey].name].KolmogorovTest(astack.GetStack().Last() )
                    tmpsubst = datahist[thesuper[ikey].name].Clone()
                    tmpsubst.Add(astack.GetStack().Last(),-1.)
                    ratiohist[thesuper[ikey].name+"_diff"].Divide(tmpsubst, astack.GetStack().Last() ) 
                    #ratiohist[thesuper[ikey].name+"_diff"].Divide(datahist[thesuper[ikey].name], astack.GetStack().Last() )
                    if (thesuper[ikey].name).find("Wprime_1btag") != -1:
                        Sdata = (datahist[thesuper[ikey].name]).Integral(datahist[thesuper[ikey].name].FindBin(1500),-1)
                        S = tmpNoStackhist[thesuper[ikey].name][1].Integral(tmpNoStackhist[thesuper[ikey].name][1].FindBin(1500),-1)
                    #S = S*0.1
                        B = astack.GetStack().Last().Integral(astack.GetStack().Last().FindBin(1500),-1)
                        print "  Data with mass>1.5 = "+str(Sdata)
                        print " Signal with mass>1.5 = " +str(S)
                        
                        print " Background  mass>1.5 = " +str(B)
                        #print "FOM for "+thesuper[ikey].name+" in "+str(tmpNoStackhist[thesuper[ikey].name][1].GetName())
                    #print " S/sqrt(B) = "+str(S/math.sqrt(B))
                    #print " S/sqrt(S+B) = "+str(S/math.sqrt(S+B))
                #cv[thesuper[ikey].name].Clear()
                pad2.cd()
                ratiohist[thesuper[ikey].name+"_diff"].SetYTitle("#frac{(Data - MC)}{MC}")
                ratiohist[thesuper[ikey].name+"_diff"].SetTitleSize(0.09,"Y")
                ratiohist[thesuper[ikey].name+"_diff"].SetTitleOffset(0.7,"Y")
                ratiohist[thesuper[ikey].name+"_diff"].Draw()
                ratiohist[thesuper[ikey].name+"_diff"].SetMaximum(2.)
                ratiohist[thesuper[ikey].name+"_diff"].SetMinimum(-2.)
                if doKS:
                    texKS = TLatex(0.25,0.82,"KS="+str(round(KSvalue,2)))
                    texKS.SetNDC()
                    texKS.SetTextSize(0.1)
                    texKS.Draw()
                                                        
                pad2.SetGrid()
                pad2.Update()

                #if doPlotError:
                #    lyy = []
                #    for iibin in range(0,astack.GetStack().Last().GetNbinsX()):
                #        content = astack.GetStack().Last().GetBinContent(iibin)
                #        lyy.append( content )
                #        yarray.fromlist( lyy)
                #        errorgraph[thesuper[ikey].name] = TGraphErrors(astack.GetStack().Last().GetNbinsX(), xarray, yarray, xerr_array, yerr_array)
                #        errorgraph[thesuper[ikey].name].SetFillColor(920+3);
                #        errorgraph[thesuper[ikey].name].SetFillStyle(3354);
                #        errorgraph[thesuper[ikey].name].Draw("E3 same")
            else:
                print "stack plot has zero entries for "+thesuper[ikey].name + " skip data/MC ratio plot"

        elif makeDiffPlot and len(astack.GetHists()) ==2:

            atemphist = astack.GetHists().At(0)
            atemphist1 = atemphist.Clone()
            ratiohist[thesuper[ikey].name+"_diff"] = atemphist1.Clone( thesuper[ikey].name+"_diff" )
            ratiohist[thesuper[ikey].name+"_diff"].Reset()

            atemphist2 = astack.GetHists().At(1)
            atemphist1.Add(atemphist2,-1.)
            ratiohist[thesuper[ikey].name+"_diff"].Divide(atemphist1, atemphist2 )
            # get KS value
            #if doKS: KSvalue = datahist[thesuper[ikey].name].KolmogorovTest(astack.GetStack().Last() )
            pad2.cd()
            ratiohist[thesuper[ikey].name+"_diff"].SetYTitle("#frac{(hist1 - hist2)}{hist2}")
            ratiohist[thesuper[ikey].name+"_diff"].SetTitleSize(0.09,"Y")
            ratiohist[thesuper[ikey].name+"_diff"].SetTitleOffset(0.7,"Y")
            ratiohist[thesuper[ikey].name+"_diff"].Draw()
            ratiohist[thesuper[ikey].name+"_diff"].SetMaximum(2.)
            ratiohist[thesuper[ikey].name+"_diff"].SetMinimum(-2.)
            #if doKS:
            #    texKS = TLatex(0.25,0.82,"KS="+str(round(KSvalue,2)))
            #    texKS.SetNDC()
            #    texKS.SetTextSize(0.1)
            #    texKS.Draw()
                
            pad2.SetGrid()
            pad2.Update()
                                                                                                                                                                                                                                
                                                                                                                                                                                                                                                                                                                                                                                                                                                
	#cv[thesuper[ikey].name].Print("test.png")
	#cv[thesuper[ikey].name].UseCurrentStyle()
        #gPad.SetRightMargin(4)
        if printCanvas:
            cv[thesuper[ikey].name].Print(OutputDir+thesuper[ikey].name + "." + printFormat)
        # pause
	if option.wait:
	    raw_input( 'Press ENTER to continue\n ' )
        
	    
    if printCanvas:
	
	for ikey in theaddition:
	    cv[theaddition[ikey].name].Print(OutputDir+theaddition[ikey].name + "." + printFormat)
	#for ikey in thesuper:
	#    cv[thesuper[ikey].name].Print(OutputDir+thesuper[ikey].name + "." + printFormat)
    
    
    #outputroot.Write()
    #outputroot.Close()

#    if not option.wait:
    if option.quit:
        sys.exit()
        
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

        
    print "exit"

    
		    

