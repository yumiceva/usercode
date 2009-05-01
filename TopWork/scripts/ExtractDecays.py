
from ROOT import *

import sys,os

def main():

    file = TFile(sys.argv[1])

    h2d = ROOT.gDirectory.Get("/Generator/gen_top_decays_vsJets")

    events = []
    label = [ 'all jets','dileptons','e+jets','muon+jets','tau+jets']
    lines = []

    ylabel = [ 'Njets = 1','Njets = 2','Njets = 3','Njets$\\geq4$']

    
    for i in range(0,len(ylabel)):

	line = ylabel[i]

	h1 = h2d.ProjectionX("h1",i+1,i+1)
    
	total = h1.GetEntries()

	for j in range(1,len(label)+1):
	    rate = 100*h1.GetBinContent(j+1)/total
	    rate = round(rate,1)
	    events.append(rate)
	    
	    line += " \t & "+str(rate)
	    
	line += " \\\\"
	
	print line


if __name__ =='__main__':
    sys.exit(main())
