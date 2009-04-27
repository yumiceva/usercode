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
        print" [usage] conter.py <directory> <pattern> <root file destination> <on/off=write labels>"
        sys.exit()

    directory = sys.argv[1]
    pattern = sys.argv[2]
    out = sys.argv[3]
    writeLabels = "off"
    if len(sys.argv)==5:
        writeLabels = sys.argv[4]
    try:
        labelfile = open("labels")
    except:
        labelfile = open("labels","w")

    if writeLabels == "on":
        labelfile.close()
        labelfile = open("labels","w")
    
    outfile = TFile(out, "UPDATE")

    files = get_list_files(directory, pattern)
    
    nfiles = 0

    hall = TH1F("hall","hall",10,0,1)

    labelsize = 0
    #labels = 0

    for f in files:

	print "\n number of file: " + str(nfiles)
        
        tf = TFile(f);

        h = ROOT.gDirectory.Get("counter")

        #h.Print("all")

	
	if nfiles == 0:
	    labels = h.GetXaxis().GetLabels()
	    labelsize = len(labels)
	    hall = h.Clone("hall")
	    hall.Reset()
	    hall.SetDirectory(0)
	    
	    for il in labels:
		print il
                if writeLabels == "on":
                    labelfile.write(il.GetName())
                    labelfile.write('\n')

            if writeLabels!="on":
                labels = []
                for line in labelfile:
                    labels.append(line)
                ib = 1
                for al in labels:
                    al = al.strip('\n')
                    print al
                    hall.GetXaxis().SetBinLabel(ib,al)
                    ib = ib+1
            
	#hall.Print("all")
	
	ibin = 1
	# loop over labels
	for il in hall.GetXaxis().GetLabels():

	    axislabel = h.GetXaxis().GetLabels()
	    jbin = 1
	    for j in axislabel:
		if j == il:
		    # got the bin
		    hall.SetBinContent( ibin, (hall.GetBinContent(ibin) + h.GetBinContent(jbin)) )
		jbin = jbin + 1
	    ibin = ibin + 1


#	for ibin in range(1, h.GetNbinsX() ):


	    #print ibin
#	    hall.SetBinContent( ibin, (hall.GetBinContent(ibin) + h.GetBinContent(ibin)) )

#	    axislabel = h.GetXaxis().GetLabels()
#	    if ibin < labelsize:

#		if labels[ibin] != axislabel[ibin]:
#		    print "no matched labels!!"
		
	nfiles = nfiles+1
	tf.Close()

    finallabels = hall.GetXaxis().GetLabels()
    print "total number of files read = " + str(nfiles)

    ihall = 1;
    for i in finallabels:
	print str(ihall) + " " + str(i) + " = " + str( hall.GetBinContent(ihall) )
	ihall = ihall+1

    outfile.cd()
    hall.SetName("CounterGood")
    hall.Write()
    

if __name__ =='__main__':
    sys.exit(main())
