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

import sys
import ROOT
from ROOT import TFile


class Inspector:

    def SetFilename(self, value):
	self.Filename = value

    def Loop(self, value):
		
	alist = self.dir.GetListOfKets()

	for i in alist:
	    aobj = i.ReadObj()
	    if obj.IsA().InheritsFrom("TDirectory"):
		print 'y'
    
    def GetListObjects(self):
	
	afile = TFile(self.Filename)
	
	if afile.IsZombie():
	    print " error trying to open file: " + self.Filename
	    sys.exit()
	
	#alist = afile.GetListOfKeys()
	self.dir = ROOT.gDirectory
	
	alist = self.dir.GetListOfKeys()

	for i in alist:
	    aobj = i.ReadObj()
	    if aobj.IsA().InheritsFrom("TDirectory"):
		print ' found directory: '+i.GetName()
		
		bdir = self.dir
		afile.GetObject(i.GetName(),bdir)
		blist = bdir.GetListOfKeys()
		for j in blist:
		    bobj = j.ReadObj()
		    if bobj.IsA().InheritsFrom(ROOT.TH1.Class()):
			print '  --> found TH1: '+j.GetName()
    
	#loop(dir)

    


