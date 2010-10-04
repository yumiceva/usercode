#! /usr/bin/env python

from ROOT import *

import sys,os, math

IsMC = False
Lumi = 4.54 # in pb-1
JetType = "calo"

if len(sys.argv)>1:
    JetType = sys.argv[1]

if len(sys.argv)>2:
    IsMC = True
    Lumi = sys.argv[2]


Nevents = {}
xsec = {} # xsection in pb
file = {}
label = {}
# MC files
if IsMC:
    file['ttbar'] = 'cutflow_'+JetType+'_TTbar.txt'
    file['QCD']   = 'cutflow_'+JetType+'_QCD.txt'
    file['Wjets'] = 'cutflow_'+JetType+'_Wjets.txt'
    file['Zjets'] = 'cutflow_'+JetType+'_Zjets.txt'
    file['tch'] = 'cutflow_'+JetType+'_STtch.txt'

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
    #file['Jun14'] = ''
    #file['MB']    = '/uscmst1b_scratch/lpc1/cmsroc/yumiceva/top_prod_Sep22/Jun14/ttmuj_Jun14_MB_PSep23.root'#'/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_MB_Sep3.root'
    #file['Jul16'] = '/uscmst1b_scratch/lpc1/cmsroc/yumiceva/top_prod_Sep22/Jul16/ttmuj_Jul16_PSep23.root'#'/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_Jul16_Sep3.root'
    #file['Prompt']= '/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_Prompt_Sep3.root'

    #label['Jun14'] = 'Jun 14th Mu'
    #label['MB'] = 'Jun 14th MinimumBias'
    #label['Jul16'] = 'Jul 16th'
    #label['Prompt'] = 'PromptReco'
    file['data'] = 'cutflow_'+JetType+'_data.txt'
    label['data'] = 'Data'
    
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

    cutmap = {}
    txtfile = open(file[sample])
    for line in txtfile:
        tmplist = line.split()
        cutname = tmplist[0]
        acut = tmplist[1]
        #print cutname
        #print acut
        if sample=="data":
            cutmap[ cutname ] = int(float(acut))
        else:
            cutmap[ cutname ] = float(acut)
    txtfile.close()
    
    scale = 1.
    if IsMC:
        scale = ( L * xsec[ sample ] / cutmap[0] )

    ilabel = 0
    for key in cutmap.keys():

        cutmap[ key ] = scale * cutmap[ key]
        print " cut "+str(key) + " ("+cutlabel[ilabel]+") "+" = "+str( round(cutmap[key],1) )
        if allmap.has_key(key):
            allmap[ key ] += cutmap[ key ]
        else:
            allmap[ key ] = cutmap[ key ]
        ilabel += 1
    cutflow[ sample ] = cutmap

print " TOTAL"
ilabel=0
for key in allmap.keys():
    print " cut "+str(key) + " ("+cutlabel[ilabel]+") "+" = "+str( round(allmap[key],1) )
    ilabel += 1
    
cutflow["Total"] = allmap

# write latex
tablelist = ['ttbar','Wjets','Zjets','QCD','tch','Total']
if not IsMC:
    #tablelist = ['MB','Jun14','Jul16','Prompt','Total']
    tablelist = ['data']

texname = "cutflow_"+JetType+"_data.tex"

if IsMC:
    texname = "cutflow_"+JetType+"_MC.tex"

outtex = open(texname,"w")

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
    #outtex.write('\\begin{tabular}{|l|c|c|c|c|c|} \hline \n')
    outtex.write('\\begin{tabular}{|l|c|} \hline \n')
    line = "Cut"
    for sample in tablelist:
        line += sss+label[sample]
    outtex.write(line+" \\\ \hline \n")

ilabel = 0                   
for key in allmap.keys():

    
    line = cutlabel[ilabel]
    
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
    ilabel += 1
    
outtex.write(''' \hline
\end{tabular}
%\caption{MC cutflow}\label{tab:tab}
\end{centering}
\end{table}
''')

print "cutflow.tex written."

    

