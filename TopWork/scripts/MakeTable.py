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
        print" [usage] MakeTable.py <directory> <weight=true/false>"
        print " e.g. MakeTable.py /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_IPsig3_Iso95/nominal/ true"
        sys.exit()

    path = sys.argv[1]
    applyWeight = sys.argv[2]
    
    #files = get_list_files(directory, pattern)
    #path = '/uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_IPsig3_Iso95/nominal/'
    
    files = []
    files.append(path+'TopAnalysis_TTJets-madgraph_Fall08_all_all.root')
    files.append(path+'TopAnalysis_WJets_madgraph_Fall08_all.root')
    files.append(path+'TopAnalysis_ZJets_madgraph_Fall08_all.root')
    files.append(path+'TopAnalysis_InclusiveMuPt15_Summer08_all.root')
    files.append(path+'TopAnalysis_ST_t.root')
    files.append(path+'TopAnalysis_ST_s.root')
    files.append(path+'TopAnalysis_ST_tW.root')
    
    nfiles = 0

    weightList = [0.0101, 0.0883, 0.0744, 0.3907, 0.003, 0.0027, 0.0034]
    
    hlist = []
    
    
    for f in files:

	print "\n Open File: " + f
        
        tf = TFile(f);

        h = ROOT.gDirectory.Get("CounterGood")
        h.SetDirectory(0)
        #h.Print("all")
        hlist.append(h)


    select = ['Processed','HLT_Mu9','GlobalMuons','GoodMuons','GoodMuonsID','GoodIsoMuons','GoodOneIsoMuon','LooseMuonsID','muonVetoCone','NoElectrons','NoLooseElectrons','Njets=1','Njets=2','Njets=3','Njets>=4','ComplexPz']

    selectLabel = ['Processed','HLT','Global muons','$p^{\\mu}_T>20$,$|\eta^{\\mu}|<2.1$','muon ID','RelIso$>0.95$','1 isolated muon','1 loose muon','Veto $E_T$ iso-cone','Veto electrons','1 loose electron','Njets=1','Njets=2','Njets=3','Njets>=4','ComplexPz']
    
    print "Selection \t & \\ttbarjets \t & \\Wjets \t & \\Zjets \t & QCD \t & Single top t \t & Single top s \t & Single top tW \\\\ \\hline"
    
    for cut in select:

        #print "\n get events for cut: " + cut
        
        events = []
        i_hlist = 0
        
        for h in hlist:
            weight = 1.
            
            if applyWeight =="true":
                weight = weightList[i_hlist]
            i_hlist+=1
            labels = h.GetXaxis().GetLabels()
            labelsize = len(labels)
            ibin = 1
            for il in labels:
                if il == cut:
                    #print il
                    events.append(weight*h.GetBinContent(ibin))
                    
                ibin = ibin + 1

        line = cut
        
        for ev in events:
            
            line = line + "\t & " + str(int(round(ev,0)))
            


        print line + " \\\\"
    

if __name__ =='__main__':
    sys.exit(main())
