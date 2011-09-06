#! /usr/bin/env python

from ROOT import *
from decimal import Decimal
import sys,os, math

path = "./"
IsMC = False
Lumi = 10.93 # in pb-1
JetType = "PF" #calo

if len(sys.argv) < 2:
    print "usage: mkCutFlowTables.py <path to files> <Lumi for MC>"
    sys.exit()

if len(sys.argv) > 1:

    path = sys.argv[1]
    print " path: "+ path
    if len(sys.argv)>2:
        IsMC = True
        Lumi = float(sys.argv[2])
        print " Luminosity for MC samples: "+str(Lumi)

Nevents = {}
xsec = {} # xsection in pb
file = {}
label = {}
# MC files
if IsMC:
    file['ttbar'] = path+'/results_ttbar.root'
    file['QCD']   = path+'/results_QCD.root'
    file['Wjets'] = path+'/results_WJets.root'
    file['Zjets'] = path+'/results_ZJets.root'
    file['tch']   = path+'/results_STtch.root'
    file['tWch']  = path+'/results_STtWch.root'
    file['sch']   = path+'/results_STsch.root'
    file['tch_bar']   = path+'/results_STtch_bar.root'
    file['tWch_bar']  = path+'/results_STtWch_bar.root'
    file['sch_bar']   = path+'/results_STsch_bar.root'
    file['WW']    = path+'/results_WW.root'
    file['WZ']    = path+'/results_WZ.root'
    file['Wprime800'] = path+'/results_Wprime_800.root'
    file['Wprime1000'] = path+'/results_Wprime_1000.root'
    file['Wprime1200'] = path+'/results_Wprime_1200.root'
    file['Wprime1500'] = path+'/results_Wprime_1500.root'
    file['Wprime2000'] = path+'/results_Wprime_2000.root'
    
    xsec['ttbar'] = 157.5
    xsec['QCD']   = 84679.3
    xsec['Wjets'] = 31314.0
    xsec['Zjets'] = 3048.0
    xsec['tch']   = 41.92
    xsec['tWch']  = 7.87
    xsec['sch']   = 3.19
    xsec['tch_bar']   = 22.65
    xsec['tWch_bar']  = 7.87
    xsec['sch_bar']   = 1.44
    xsec['WW']    = 43.0
    xsec['WZ']    = 18.0
    xsec['Wprime800'] = 5.26
    xsec['Wprime1000'] = 1.93
    xsec['Wprime1200'] = 0.773
    xsec['Wprime1500'] = 0.228
    xsec['Wprime2000'] = 0.034

    Nevents["Wprime800"] = 107347. 
    Nevents["Wprime1000"] = 109204.0
    Nevents["Wprime1200"] = 108990.0
    Nevents["Wprime1500"] = 108733.0
    Nevents["Wprime2000"] = 106657.0
    
    Nevents['ttbar'] = 3683795.0
    Nevents['QCD']   = 24575292.0
    Nevents['Wjets'] = 49335978.0
    Nevents['Zjets'] = 32475188.0
    Nevents['tch']   = 3897643.0
    Nevents['tWch']  = 813629.0
    Nevents['sch']   = 259762.0
    Nevents['tch_bar']   = 1943821.0 
    Nevents['tWch_bar']  = 809293.0
    Nevents['sch_bar']   = 137916.0            
    Nevents['WW']    = 4223922.0
    Nevents['WZ']    = 4263076.0
    
    label['ttbar'] = '$t\\overline{t}$'
    label['QCD'] = 'QCD'
    label['Wjets'] = '$W\\rightarrow l\\nu$'
    label['Zjets'] = '$Z\\rightarrow l^{+}l^{-}$'
    label['tch'] = 'top t-ch'
    label['tWch'] = 'top tW-ch'
    label['sch'] = 'top s-ch'
    label['tch_bar'] = 'anti-top t-ch'
    label['tWch_bar'] = 'anti-top tW-ch'
    label['sch_bar'] = 'anti-top s-ch'
    label['WW'] = 'WW'
    label['WZ'] = 'WZ'

    label['Wprime800'] = "W' 0.8 TeV"
    label['Wprime1000'] = "W' 1 TeV"
    label['Wprime1200'] = "W' 1.2 TeV"
    label['Wprime1500'] = "W' 1.5 TeV"
    label['Wprime2000'] = "W' 2 TeV"
    
    label['Total'] = 'Total MC'
else:
# data files
    #file['Jun14'] = ''
    #file['MB']    = '/uscmst1b_scratch/lpc1/cmsroc/yumiceva/top_prod_Sep22/Jun14/ttmuj_Jun14_MB_PSep23.root'#'/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_MB_Sep3.root'
    #file['Jul16'] = '/uscmst1b_scratch/lpc1/cmsroc/yumiceva/top_prod_Sep22/Jul16/ttmuj_Jul16_PSep23.root'#'/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_Jul16_Sep3.root'
    #file['Prompt']= '/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_Prompt_Sep3.root'

    #label['Jun14'] = 'Jun 14th Mu'
    #label['MB'] = 'Jun 14th MinimumBias'
    #label['Jul16'] = 'Jul 16th'
    #label['Prompt'] = 'PromptReco'

    #if os.path.isfile(path+'/JPT/cutflow_JPT_data.txt'):
    #    file['dataJPT'] = path+'/cutflow_JPT_data.txt'
    #    label['dataJPT'] = 'JPT Ref. Sel.'
    #if os.path.isfile(path+'/calo/cutflow_calo_data.txt'):
    #    file['datacalo'] = path+'/cutflow_calo_data.txt'
    #    label['datacalo'] = 'Calo Ref. Sel.'
    #if os.path.isfile(path+'/PF/cutflow_PF_data.txt'):
    #    file['dataPF'] = path+'/cutflow_PF_data.txt'
    #    label['dataPF'] = 'PF Ref. Sel.'

    file['data'] = path+'/results_data.root';
    label['data'] = "Data";
    
                    
    
keys = file.keys()
cutflow = {}
cutflowerr = {}
cutlabel = {}
cutlabel['Processed'] = 'Processed'
cutlabel['CleanFilters'] = 'CleanFilters'
cutlabel['HLT'] = 'HLT'
cutlabel['GoodPV'] = 'Skim'
cutlabel['OneIsoMu'] = 'One Iso $\mu$'
cutlabel['LooseMuVeto'] = 'Loose $\mu$ veto'
cutlabel['ElectronVeto'] = 'Electron veto'
cutlabel['MET'] = '$E\!\!\!\!/_T>20$~GeV'
cutlabel['1Jet'] = 'jets $> 0$'
cutlabel['2Jet'] = 'jets $> 1$'
cutlabel['3Jet'] = 'jets $> 2$'
cutlabel['4Jet'] = 'jets $> 3$'
cutlabel['2Jet1b'] = 'jets $> 1$, btags $> 0$'
cutlabel['2Jet2b'] = 'jets $> 1$, btags $> 1$'

cutlabelvector = [ 'GoodPV', 'OneIsoMu', 'LooseMuVeto', 'ElectronVeto', 'MET', '1Jet', '2Jet', '3Jet', '4Jet','2Jet1b','2Jet2b']
SKIPCUTS = ['3Jet','4Jet']

allmap = {}
allmaperr = {}

weightmap = {}

for sample in keys:

    print " processing " + sample

    cutmap = {}
    cutmaperr = {}
    #txtfile = open(file[sample])
    roofile = TFile(file[sample])
    hcutflow = gDirectory.Get("cutflow")
    print "  entries in cutflow histogram: " + str(hcutflow.GetEntries())

    for ibin in xrange(1, hcutflow.GetNbinsX() +1 ):

        skipthiscut = False
        for skipcut in SKIPCUTS:
            if skipcut == cutlabelvector[ibin-1]: skipthiscut = True
        if skipthiscut:
            print "skip counting cut name: "+cutlabelvector[ibin-1]
            continue
        cutname = cutlabelvector[ibin-1]
        acut = hcutflow.GetBinContent(ibin)
        #print cutname
        #print acut
        #if sample=="data":
        if sample.find("data")!=-1:
            cutmap[ cutname ] = int(float(acut))
            cutmaperr[ cutname ] = math.sqrt( cutmap[cutname] )
        else:
            cutmap[ cutname ] = Decimal(str(acut))
            cutmaperr[ cutname ] = cutmap[cutname].sqrt() #math.sqrt( cutmap[cutname] )
            
    roofile.Close()
    
    scale = 1.
    if IsMC and Lumi>0:
        scale = Decimal( str( Lumi * xsec[ sample ] / Nevents[sample] ))
        print "sample weight = "+ str( xsec[ sample ] / Nevents[sample] )
        weightmap[sample] = xsec[ sample ] / Nevents[sample]
    ilabel = 0
    for key in cutmap.keys():

        cutmap[ key ] = scale * cutmap[ key]
        cutmaperr[key]= scale * cutmaperr[key] * scale * cutmaperr[key]
        
        print " cut "+str(key) + " ("+cutlabel[key]+") "+" = "+str( round(cutmap[key],1) )

        if allmap.has_key(key):
            allmap[ key ] += cutmap[ key ]
            allmaperr[ key ] += cutmaperr[key]
        else:
            allmap[ key ] = cutmap[ key ]
            allmaperr[ key ] = cutmap[key]
        ilabel += 1
    cutflow[ sample ] = cutmap
    cutflowerr[ sample ] = cutmaperr
    
print " TOTAL"
ilabel=0
for key in allmap.keys():
    print " cut "+str(key) + " ("+cutlabel[key]+") "+" = "+str( round(allmap[key],1) )
    ilabel += 1
    
cutflow["Total"] = allmap
cutflowerr["Total"] = allmaperr

# write latex
#sortedcutlist = ['CleanFilters','HLT','GoodPV','OneIsoMu','LooseMuVeto','ElectronVeto','MET','1Jet','2Jet','3Jet','4Jet']
sortedcutlist = ['GoodPV','OneIsoMu','LooseMuVeto','ElectronVeto']#,'MET','1Jet','2Jet','2Jet1b']
sortedcutlist2= ['MET','1Jet','2Jet','2Jet1b','2Jet2b']

if IsMC:
    cutlabel['CleanFilters'] = 'Processed'

         
#tablelist = ['ttbar','Wjets','Zjets','QCD','tch','tch_bar','tWch','tWch_bar','sch','sch_bar','WW','WZ','Total']
tablelist = ['Wprime800','Wprime1000','Wprime1200','Wprime1500','Wprime2000']
if Lumi<=0:
    tablelist = ['ttbar','Wjets','Zjets','QCD','tch','tch_bar','tWch','tWch_bar','sch','sch_bar','WW','WZ']
    
if not IsMC:
    #tablelist = ['MB','Jun14','Jul16','Prompt','Total']
    #tablelist = ['dataJPT','datacalo','dataJPTMET','datacaloMET']
    tablelist = ['data']
    #tablelist = ['dataPFMET']
    
texname = "cutflow_"+JetType+"_data.tex"

if IsMC:
    texname = "cutflow_"+JetType+"_MC.tex"

outtex = open(texname,"w")

sss = " & "

# Write Header
outtex.write('''
\\begin{table}[h]
\\begin{centering}
''')
if IsMC:
    aline = '\\begin{tabular}{|l|'+'c|'*(len(sortedcutlist)+1) +'} \hline \n'
    if Lumi<=0:
        aline = '\\begin{tabular}{|l|'+'c|'*len(sortedcutlist) +'} \hline \n'
    outtex.write(aline)
    #outtex.write('Cut & ttbar & Wjets & Zjets & QCD & STtch \hline')
    line = "Sample"
    for icut in sortedcutlist:
        line += sss+cutlabel[icut]
    outtex.write(line+" \\\ \hline \n")
else:
    aline = '\\begin{tabular}{|l|r|r|r|} \hline \n'
    
    outtex.write(aline)
    line = "Sample"
    for icut in sortedcutlist:
        line += sss+cutlabel[icut]
    outtex.write(line+" \\\ \hline \n")

ilabel = 0                   
#for key in allmap.keys():
# Write content
for sample in tablelist:
    
    line = label[sample]
    
    for key in sortedcutlist:
        cutmap = cutflow[sample]
        cutmaperr = cutflowerr[sample]
        acut = cutmap[key]
        acuterr = cutmaperr[key]
        stracut = str(int(acut))
        stracuterr = str(round(math.sqrt(acuterr),1))
        #stracut = stracut.strip('.0')
        if IsMC:
            acut = round(cutmap[key],1)
            stracut = str(acut)
            line += sss + stracut + " $\\pm$ " + stracuterr
        else:
            line += sss + stracut
    if sample=="Total":line = '\\hline \n'+line
    outtex.write(line+" \\\ \n")
    ilabel += 1
    
outtex.write(''' \hline
\end{tabular}
%\caption{MC cutflow}\label{tab:tab}
\end{centering}
\end{table}
''')


if len(sortedcutlist2) > 0:

    
    # Write Header
    outtex.write('''
\\begin{table}[h]
\\begin{centering}
    ''')
    if IsMC:
        aline = '\\begin{tabular}{|l|'+'c|'*(len(sortedcutlist2)+1) +'} \hline \n'
        if Lumi<=0:
            aline = '\\begin{tabular}{|l|'+'c|'*len(sortedcutlist2) +'} \hline \n'
        outtex.write(aline)
        #outtex.write('Cut & ttbar & Wjets & Zjets & QCD & STtch \hline')
        line = "Sample"
        for icut in sortedcutlist2:
            line += sss+cutlabel[icut]
        outtex.write(line+" \\\ \hline \n")
    else:
        aline = '\\begin{tabular}{|l|r|r|r|} \hline \n'
            
        outtex.write(aline)
        line = "Sample"
        for icut in sortedcutlist2:
            line += sss+cutlabel[icut]
        outtex.write(line+" \\\ \hline \n")

    ilabel = 0
    #for key in allmap.keys():
    # Write content
    for sample in tablelist:
        
        line = label[sample]

        for key in sortedcutlist2:
            cutmap = cutflow[sample]
            cutmaperr = cutflowerr[sample]
            acut = cutmap[key]
            acuterr = cutmaperr[key]
            stracut = str(int(acut))
            stracuterr = str(round(math.sqrt(acuterr),1))
            #stracut = stracut.strip('.0')
            if IsMC:
                acut = round(cutmap[key],1)
                stracut = str(acut)
                line += sss + stracut + " $\\pm$ " + stracuterr
            else:
                line += sss + stracut
        if sample=="Total":line = '\\hline \n'+line
        outtex.write(line+" \\\ \n")
        ilabel += 1
            
    outtex.write(''' \hline
\end{tabular}
%\caption{MC cutflow}\label{tab:tab}
\end{centering}
\end{table}
    ''')
    

print "file "+texname+ " has been written."

if IsMC:
    
    print "\n MC Weights"
    
    for sample in weightmap.keys():

        print sample+" "+str(weightmap[sample])


    

