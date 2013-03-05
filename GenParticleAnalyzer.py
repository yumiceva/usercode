#! /usr/bin/env python
#################################
# Python script to plot nice histos  
#################################
import sys,getopt
import operator
from ROOT import *
from DataFormats.FWLite import Events, Handle

gROOT.SetStyle("Plain")
gStyle.SetOptStat(111111)
gStyle.SetHistFillColor(kBlue)

PDG =  {'d':1, 'dbar':-1, 'down':1,
	'u':2, 'ubar':-2, 'up':2,
	's':3, 'sbar':-3, 'strange':3,
	'c':4, 'cbar':-4, 'charm':4,
	'b':5, 'bbar':-5, 'bottom':5, 'beauty':5,
	't':6, 'tbar':-6, 'top':6,
	'electron':11, 'positron':-11,
	'nu_e':12, 'nu_ebar':-12,
	'muon':13, 'mu+':-13,
	'nu_mu':14, 'nu_mubar':-14,
	'tau':15, 'tau+':-15,
	'nu_tau':16, 'nu_taubar':-16,
	'gluon':21, 'photon':22, 'Z0':23, 
	'W+':24, 'W-':-24, 
	'Higgs0':25,
	'gh': 1000021, 'GH':1000021}

######################################
def analyze(infile, outfile):
	##############################

	print "Begin analyze"
	
	particle = 'top'
	
	# open file and load events
	events = Events (infile)
	# define collection type and names 
        handleGen = Handle ("vector<reco::GenParticle>")
        label = "genParticles"

	# create histograms
	hpt = TH1F(particle+' pt', particle+' pt',500,0,1000)
	hpt.SetXTitle(particle+' pt [GeV]')
	heta = TH1F(particle+' eta', particle+' eta',100,-5,5)
	heta.SetXTitle(particle+' eta')
	print "Histos created"
	
	entry = 0
	for event in events:
		entry += 1
		
		print "Event ", entry

		# open collections
		event.getByLabel (label, handleGen)
		gens = handleGen.product()

		# loop over content of collection
		for p in gens:
			#print 'id',p.pdgId(), p.status(), 'status'
			# select top quarks
			if abs( int(p.pdgId()) ) == int(PDG[particle]):
				#print p.pdgId()
				#print p.status()
				#if p.status() == 22:     # 22 for ttbar, W and gh, 23 for 'final' particles
				#nparticles += 1
				hpt.Fill(float(p.pt())) 
				heta.Fill(float(p.eta())) 

	# draw histograms
	hpt.Draw()
	c1.SaveAs(outfile+'_'+particle+'_pt.png')
	heta.Draw()
	c1.SaveAs(outfile+'_'+particle+'_eta.png')


#######################################
def main(argv):
#######################################
	infile = None
	infile2 = None
	outfile = None
	try: 
		opts, args = getopt.getopt(argv, 'hbi:p:draec:', ['help','input=', 'particle='])
		if not opts:
			print 'No options supplied. Please insert input -i and pdgID -p'
	except getopt.GetoptError,e:
		print e
		sys.exit(2)

	for opt, arg in opts:
		if opt in ('-h', '--help'):
			sys.exit(2)
		elif opt == '-b':
			continue
		elif opt in ('-i', '--input'):
			infile = arg
#		elif opt in ('-o', '--output'):
			outfile = infile.replace('.root','')

			analyze(infile,outfile)
			
		else:
			sys.exit(2)

if __name__ == '__main__':
	main(sys.argv[1:])
