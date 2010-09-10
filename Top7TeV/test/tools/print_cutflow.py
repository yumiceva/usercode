#! /usr/bin/env python

from ROOT import *

import sys,os, math

gSystem.Load('libYumicevaTop7TeV.so')

L = 2.88 # Luminosity in 1/pb
IsMC = False

Nevents = {}
xsec = {} # xsection in pb
file = {}
label = {}
# MC files
if IsMC:
    file['ttbar'] = '/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v5/ttmuj_TTbar.root'
    file['QCD']   = '/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v5/ttmuj_QCD.root'
    file['Wjets'] = '/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v5/ttmuj_Wjets.root'
    file['Zjets'] = '/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v5/ttmuj_Zjets.root'
    file['tch'] = '/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v5/ttmuj_STtch.root'

    xsec['ttbar'] =   157.5
    xsec['QCD']   = 79688.
    xsec['Wjets'] = 31314.
    xsec['Zjets'] =  3048.
    xsec['tch'] = 20.93

    Nevents['ttbar'] = 1483404.
    Nevents['QCD']   = 4377187.
    Nevents['Wjets'] = 10068895.
    Nevents['Zjets'] = 1084921.
    Nevents['tch'] = 528593.

    label['ttbar'] = '$t\\overline{t}$'
    label['QCD'] = 'QCD'
    label['Wjets'] = '$W\\rightarrow l\\nu$'
    label['Zjets'] = '$Z\\rightarrow l\\nu$'
    label['tch'] = 't channel'
    label['Total'] = 'Total'
else:
# data files
    file['Jun14'] = '/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_Jun14_Mu_Sep3.root'
    file['MB']    = '/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_MB_Sep3.root'
    file['Jul16'] = '/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_Jul16_Sep3.root'
    file['Prompt']= '/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_Prompt_Sep3.root'

    label['Jun14'] = 'Jun 14th Mu'
    label['MB'] = 'Jun 14th MinimumBias'
    label['Jul16'] = 'Jul 16th'
    label['Prompt'] = 'PromptReco'
    label['Total'] = 'Total'
    
keys = file.keys()
cutflow = {}
cutlabel = {}
cutlabel[0] = 'Processed'
cutlabel[1] = 'CleanFilters'
cutlabel[2] = 'HLT'
cutlabel[3] = 'Good PV'
cutlabel[4] = 'One Iso mu'
cutlabel[5] = 'Loose mu veto'
cutlabel[6] = 'Electron veto'
cutlabel[7] = 'Jets$\\geq$1'
cutlabel[8] = 'Jets$\\geq$2'
cutlabel[9] = 'Jets$\\geq$3'
cutlabel[10] = 'Jets$\\geq$4'

allmap = {}

for sample in keys:

    print " processing " + sample
    afile = TFile(file[sample])
    h = gDirectory.Get('/triggerFilter/eventCount')

    cutmap = {}
    if IsMC: cutmap[ 0 ] = Nevents[sample]
    cutmap[ 1 ] = h.GetBinContent( 1 )
    cutmap[ 2 ] = h.GetBinContent( 2 )

    h2 = gDirectory.Get('/PATNtupleMaker/cutflow')

    for acut in range(3, h2.GetNbinsX()+1 ):
        cutmap[ acut ] = h2.GetBinContent( acut );

    scale = 1.
    if IsMC:
        scale = ( L * xsec[ sample ] / cutmap[0] )

    for key in cutmap.keys():

        cutmap[ key ] = scale * cutmap[ key]
        print " cut "+str(key) + " ("+cutlabel[key]+") "+" = "+str( round(cutmap[key],1) )
        if allmap.has_key(key):
            allmap[ key ] += cutmap[ key ]
        else:
            allmap[ key ] = cutmap[ key ]

    cutflow[ sample ] = cutmap

print " TOTAL"
for key in allmap.keys():
    print " cut "+str(key) + " ("+cutlabel[key]+") "+" = "+str( round(allmap[key],1) )

cutflow["Total"] = allmap

# write latex
tablelist = ['ttbar','Wjets','Zjets','QCD','tch','Total']
if not IsMC:
    tablelist = ['MB','Jun14','Jul16','Prompt','Total']

outtex = open("cutflow.tex","w")
sss = " & "

outtex.write('''
\\begin{table}[h]
\\begin{centering}
''')
if IsMC:
    outtex.write('\\begin{tabular}{|l|c|c|c|c|c|c|} \hline \n')
    #outtex.write('Cut & ttbar & Wjets & Zjets & QCD & STtch \hline')
    line = "Cut"
    for sample in tablelist:
        line += sss+label[sample]
    outtex.write(line+" \\\ \hline \n")
else:
    outtex.write('\\begin{tabular}{|l|c|c|c|c|c|} \hline \n')
    line = "Cut"
    for sample in tablelist:
        line += sss+label[sample]
    outtex.write(line+" \\\ \hline \n")
                    
for key in allmap.keys():
    
    line = cutlabel[key]

    for sample in tablelist:
        cutmap = cutflow[sample]
        acut = cutmap[key]
        stracut = str(acut)
        stracut = stracut.strip('.0')
        if IsMC:
            acut = round(cutmap[key],1)
            stracut = str(acut)
        line += sss + stracut
    outtex.write(line+" \\\ \n")

outtex.write(''' \hline
\end{tabular}
%\caption{MC cutflow}\label{tab:tab}
\end{centering}
\end{table}
''')
print "cutflow.tex written."

    

