#! /usr/bin/env python

from ROOT import *
from decimal import Decimal
import sys,os, math

path = "./"
IsMC = False
Lumi = 10.93 # in pb-1
JetType = "PF" #calo
showWprime = False
kFactor = 1.2

if len(sys.argv) < 2:
    print "usage: mkCutFlowTables.py <path to files> <Lumi for MC> < 1 =will produce table for signal Wprime>"
    sys.exit()

if len(sys.argv) > 1:

    path = sys.argv[1]
    print " path: "+ path
    if len(sys.argv)>2:
        IsMC = True
        Lumi = float(sys.argv[2])
        print " Luminosity for MC samples: "+str(Lumi)

        if len(sys.argv)>3:
            showWprime = True
            
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

    file['WpRH800'] = path+'/results_WpRH800.root'
    file['WpRH900'] = path+'/results_WpRH900.root'
    file['WpRH1000'] = path+'/results_WpRH1000.root'
    file['WpRH1100'] = path+'/results_WpRH1100.root'
    file['WpRH1200'] = path+'/results_WpRH1200.root'
    file['WpRH1300'] = path+'/results_WpRH1300.root'
    file['WpRH1400'] = path+'/results_WpRH1400.root'
    file['WpRH1500'] = path+'/results_WpRH1500.root'
    file['WpRH1600'] = path+'/results_WpRH1600.root'

    file['WpLH800'] = path+'/results_WpLH800.root'
    file['WpLH900'] = path+'/results_WpLH900.root'
    file['WpLH1000'] = path+'/results_WpLH1000.root'
    file['WpLH1100'] = path+'/results_WpLH1100.root'
    file['WpLH1200'] = path+'/results_WpLH1200.root'
    file['WpLH1300'] = path+'/results_WpLH1300.root'
    file['WpLH1400'] = path+'/results_WpLH1400.root'
    file['WpLH1500'] = path+'/results_WpLH1500.root'
    file['WpLH1600'] = path+'/results_WpLH1600.root'

    file['WpMix800'] = path+'/results_WpMix800.root'
    file['WpMix900'] = path+'/results_WpMix900.root'
    file['WpMix1000'] = path+'/results_WpMix1000.root'
    file['WpMix1100'] = path+'/results_WpMix1100.root'
    file['WpMix1200'] = path+'/results_WpMix1200.root'
    file['WpMix1300'] = path+'/results_WpMix1300.root'
    file['WpMix1400'] = path+'/results_WpMix1400.root'
    file['WpMix1500'] = path+'/results_WpMix1500.root'
    file['WpMix1600'] = path+'/results_WpMix1600.root'

#    file['WpRH2000'] = path+'/results_WpRH2000.root'
    
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
    xsec['WpRH800'] = 1.6697*kFactor
    xsec['WpRH900'] = 0.97609*kFactor
    xsec['WpRH1000'] = 0.58782*kFactor
    xsec['WpRH1100'] = 0.36266*kFactor
    xsec['WpRH1200'] = 0.22815*kFactor
    xsec['WpRH1300'] = 0.14584*kFactor
    xsec['WpRH1400'] = 0.094496*kFactor
    xsec['WpRH1500'] = 0.061946*kFactor
    xsec['WpRH1600'] = 0.04102*kFactor
    Nevents['WpRH800'] = 109734.0 
    Nevents['WpRH900'] = 109695.0
    Nevents['WpRH1000'] = 109613.0
    Nevents['WpRH1100'] = 109623.0
    Nevents['WpRH1200'] = 109592.0
    Nevents['WpRH1300'] = 109566.0
    Nevents['WpRH1400'] = 109558.0
    Nevents['WpRH1500'] = 109532.0
    Nevents['WpRH1600'] = 109529.0

    xsec['WpLH800'] = 2.3139*kFactor
    xsec['WpLH1000'] = 1.3529*kFactor
    xsec['WpLH1100'] = 1.1649*kFactor
    xsec['WpLH1200'] = 1.0587*kFactor
    xsec['WpLH1300'] = 0.9982*kFactor
    xsec['WpLH1400'] = 0.96318*kFactor
    xsec['WpLH1500'] = 0.94378*kFactor
    xsec['WpLH1600'] = 0.93355*kFactor
    Nevents['WpLH800'] = 105441.0
    Nevents['WpLH1000'] = 109762.0
    Nevents['WpLH1100'] = 109501.0
    Nevents['WpLH1200'] = 109116.0
    Nevents['WpLH1300'] = 107481.0
    Nevents['WpLH1400'] = 109846.0
    Nevents['WpLH1500'] = 108879.0
    Nevents['WpLH1600'] = 109880.0

    xsec['WpMix800'] = 3.9983*kFactor
    xsec['WpMix900'] = 2.6853*kFactor
    xsec['WpMix1000'] = 1.9589*kFactor
    xsec['WpMix1100'] = 1.5443*kFactor
    xsec['WpMix1200'] = 1.3008*kFactor
    xsec['WpMix1300'] = 1.1552*kFactor
    xsec['WpMix1400'] = 1.067*kFactor
    xsec['WpMix1500'] = 1.0132*kFactor
    xsec['WpMix1600'] = 0.98074*kFactor
    Nevents['WpMix800'] = 109681.0
    Nevents['WpMix900'] = 108706.0
    Nevents['WpMix1000'] = 108735.0
    Nevents['WpMix1100'] = 109498.0
    Nevents['WpMix1200'] = 109168.0
    Nevents['WpMix1300'] = 109799.0
    Nevents['WpMix1400'] = 109163.0
    Nevents['WpMix1500'] = 108854.0
    Nevents['WpMix1600'] = 109862.0
    
#    xsec['WpRH800'] = 6.838
#    xsec['WpRH1000'] = 2.509
#    xsec['WpRH1200'] = 1.1595
#    xsec['WpRH1500'] = 0.2964
    xsec['WpRH2000'] = 7.935e-3 #0.0442
#    Nevents["WpRH800"] = 107347. 
#    Nevents["WpRH1000"] = 109204.0
#    Nevents["WpRH1200"] = 108990.0
#    Nevents["WpRH1500"] = 108733.0
    Nevents["WpRH2000"] = 106657.0
    
    Nevents['ttbar'] = 3697476.0 #3683795.0
    Nevents['QCD']   = 24909653.0 
    Nevents['Wjets'] = 76978604.0 #49335978.0
    Nevents['Zjets'] = 36236728.0 #32475188.0
    Nevents['tch']   = 3897643.0 
    Nevents['tWch']  = 813629.0 
    Nevents['sch']   = 259762.0
    Nevents['tch_bar']  = 1943821.0 #1943821.0 
    Nevents['tWch_bar'] = 809293.0
    Nevents['sch_bar']  = 137916.0 #137916.0            
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

    label['WpRH800'] = "W' RH 0.8 TeV"
    label['WpRH900'] = "W' RH 0.9 TeV"
    label['WpRH1000'] = "W' RH 1 TeV"
    label['WpRH1100'] = "W' RH 1.1 TeV"
    label['WpRH1200'] = "W' RH 1.2 TeV"
    label['WpRH1300'] = "W' RH 1.3 TeV"
    label['WpRH1400'] = "W' RH 1.4 TeV"
    label['WpRH1500'] = "W' RH 1.5 TeV"
    label['WpRH1600'] = "W' RH 1.6 TeV"
    label['WpRH2000'] = "W' RH 2 TeV"

    label['WpLH800'] = "W' LH 0.8 TeV"
    label['WpLH900'] = "W' LH 0.9 TeV"
    label['WpLH1000'] = "W' LH 1 TeV"
    label['WpLH1100'] = "W' LH 1.1 TeV"
    label['WpLH1200'] = "W' LH 1.2 TeV"
    label['WpLH1300'] = "W' LH 1.3 TeV"
    label['WpLH1400'] = "W' LH 1.4 TeV"
    label['WpLH1500'] = "W' LH 1.5 TeV"
    label['WpLH1600'] = "W' LH 1.6 TeV"

    label['WpMix800'] = "W' Mix 0.8 TeV"
    label['WpMix900'] = "W' Mix 0.9 TeV"
    label['WpMix1000'] = "W' Mix 1 TeV"
    label['WpMix1100'] = "W' Mix 1.1 TeV"
    label['WpMix1200'] = "W' Mix 1.2 TeV"
    label['WpMix1300'] = "W' Mix 1.3 TeV"
    label['WpMix1400'] = "W' Mix 1.4 TeV"
    label['WpMix1500'] = "W' Mix 1.5 TeV"
    label['WpMix1600'] = "W' Mix 1.6 TeV"
                                    
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
cutlabel['MaxJets'] = 'max jets'
cutlabel['phi'] = 'delta phi'
cutlabel['topmass'] = 'top mass'

cutlabelvector = [ 'GoodPV', 'OneIsoMu', 'LooseMuVeto', 'ElectronVeto', 'MET', '1Jet', '2Jet', '3Jet', '4Jet','2Jet1b','2Jet2b','MaxJets','phi','topmass']
SKIPCUTS = ['3Jet','4Jet','MaxJets','phi','topmass']

allmap = {}
allmaperr = {}

weightmap = {}

tablelist = ['ttbar','Wjets','Zjets','QCD','tch','tch_bar','tWch','tWch_bar','sch','sch_bar','WW','WZ','Total']
if showWprime:
    tablelist = ['WpRH800','WpRH900','WpRH1000','WpRH1100','WpRH1200','WpRH1300','WpRH1400','WpRH1500','WpRH1600'] #,'WpRH2000']
#tablelist = ['WpRH800','WpRH1000','WpRH1200','WpRH1500','WpRH2000']
if Lumi<=0:
    tablelist = ['ttbar','Wjets','Zjets','QCD','tch','tch_bar','tWch','tWch_bar','sch','sch_bar','WW','WZ']

if not IsMC:
    tablelist = ['data']

                            
for sample in tablelist:

    if sample=="Total": continue
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

    
texname = "cutflow_"+JetType+"_data.tex"

if IsMC:
    texname = "cutflow_"+JetType+"_MC.tex"
    if showWprime: texname = "cutflow_"+JetType+"_MC_WpRH.tex"
    
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


    

