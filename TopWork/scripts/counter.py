from ROOT import *

import sys,os


#________________________________________________________________
def get_list_files(directory,pattern = ""):

    dir = []

    dir = os.listdir(directory)
    
    lfiles = []
    for f in dir:

        if f.find(pattern) != -1 and f.endswith("root"):
            
            lfiles.append(directory+"/"+f)

    lfiles.sort()
    return lfiles


def main():
    
    if len(sys.argv) < 2:
        print" [usage] conter.py <directory> <pattern> <root file destination>"
        sys.exit()

    directory = sys.argv[1]
    pattern = sys.argv[2]
    out = sys.argv[3]

    outfile = TFile(out, "UPDATE")

    files = get_list_files(directory, pattern)
    
    nfiles = 0

    hall = TH1F("hall","hall",10,0,1)

    for f in files:

	#print nfiles
        
        tf = TFile(f);

        h = ROOT.gDirectory.Get("counter")

        #h.Print("all")

	labelsize = 0

	if nfiles == 0:
	    labels = h.GetXaxis().GetLabels()
	    labelsize = len(labels)
	    hall = h.Clone("hall")
	    hall.Reset()
	    hall.SetDirectory(0)

	#hall.Print("all")

	for ibin in range(1, h.GetNbinsX() ):

	    #print ibin
	    hall.SetBinContent( ibin, (hall.GetBinContent(ibin) + h.GetBinContent(ibin)) )

	    axislabel = h.GetXaxis().GetLabels()
	    if ibin < labelsize:

		if labels[ibin] != axislabel[ibin]:
		    print "no matched labels!!"

	nfiles = nfiles+1
	tf.Close()

    finallabels = hall.GetXaxis().GetLabels()
    print "total number of files read = " + str(nfiles)

    ihall = 1;
    for i in finallabels:
	print str(i) + " = " + str( hall.GetBinContent(ihall) )
	ihall = ihall+1

    outfile.cd()
    hall.SetName("CounterGood")
    hall.Write()
    

if __name__ =='__main__':
    sys.exit(main())
