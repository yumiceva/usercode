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

    tags = { 'Pre-Selection':1,
             'only jets p_{T}>65,40,40,40 GeV/c':5,
             'only muons p_{T}>30 GeV/c':6,
             'only isolated muons':7,
             'Njets>3, one isolated muon':8,
             '\Delta R(\mu,jets)>0.3':9 }
    
    if len(sys.argv) < 2:
        print" [usage] conter.py <directory> "
        sys.exit()

    directory = sys.argv[1]

    files = get_list_files(directory,"_all")

    data = {}

    sortedtags = tags.values()
    sortedtags.sort()
    
    for f in files:

        alist = []
        
        tf = TFile(f);

        h = ROOT.gDirectory.Get("counter")

        #h.Print("all")

        for iltags in sortedtags:

            for key,value in tags.items():

                if value == iltags:
                    
                    alist.append(h.GetBinContent(value+1))
        
        # extract name
        aname = tf.GetName()
        tmplist = aname.split('/')
        aname = tmplist[len(tmplist) -1]
        aname = aname.strip('.root')
        data[aname] = alist

    # print all

    # sort filenames
    thekeys = data.keys()
    thekeys.sort()
    
    line0 = '\\begin{tabular}{|l'
    for i in range(0,len(data)):
        line0 = line0 + "|c"
    else:
        line0 = line0 + "|} \hline"

    title = " Selection "
    for i in thekeys:
        title = title+" & " + i

    title = title + "\hline"
    
    print line0
    print title

    
    irow = 0
        
    for iltags in sortedtags:
    
        for i,value in tags.items():

            if value == iltags:
                
                row = i

                for j in thekeys:

                    row = row + " & " + str(data[j][irow])

                row = row + " \\\\"
                print row
                irow = irow +1
    

    endtable = '\end{tabular}'
    print endtable


if __name__ =='__main__':
    sys.exit(main())
