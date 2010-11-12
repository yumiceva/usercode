#! /usr/bin/env python

from ROOT import *

import sys,os, math

path = "./"
IsMC = False
Lumi = 10.93 # in pb-1
JetType = "PF" #calo

if len(sys.argv) < 2:
    print "usage: mkCutFlowTables.py <path to files> <JPT/calo> <Lumi for MC>"
    sys.exit()

if len(sys.argv) > 1:

    path = sys.argv[1]
    
    if len(sys.argv)>2:
        JetType = sys.argv[2]

        if len(sys.argv)>3:
            IsMC = True
            Lumi = float(sys.argv[3])


Nevents = {}
xsec = {} # xsection in pb
file = {}
label = {}
# MC files
if IsMC:
    file['ttbar'] = path+'/cutflow_'+JetType+'_TTbar.txt'
    file['QCD']   = path+'/cutflow_'+JetType+'_QCD.txt'
    file['Wjets'] = path+'/cutflow_'+JetType+'_Wjets.txt'
    file['Zjets'] = path+'/cutflow_'+JetType+'_Zjets.txt'
    file['tch'] = path+'/cutflow_'+JetType+'_STtch.txt'
    file['tWch'] = path+'/cutflow_'+JetType+'_STtWch.txt'
    
    xsec['ttbar'] =   157.5
    xsec['QCD']   = 79688.
    xsec['Wjets'] = 31314.
    xsec['Zjets'] =  3048.
    xsec['tch'] = 20.93
    xsec['tWch'] = 10.6
    
    Nevents['ttbar'] = 1483404.
    Nevents['QCD']   = 4377187.
    Nevents['Wjets'] = 10068895.
    Nevents['Zjets'] = 1084921.
    Nevents['tch'] = 528593.
    Nevents['tWch'] = 466437.
    
    label['ttbar'] = '$t\\overline{t}$'
    label['QCD'] = 'QCD'
    label['Wjets'] = '$W\\rightarrow l\\nu$'
    label['Zjets'] = '$Z\\rightarrow l\\nu$'
    label['tch'] = 't channel'
    label['tWch'] = 'tW channel'
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

    #if os.path.isfile(path+'/JPT/cutflow_JPT_data.txt'):
    #    file['dataJPT'] = path+'/cutflow_JPT_data.txt'
    #    label['dataJPT'] = 'JPT Ref. Sel.'
    #if os.path.isfile(path+'/calo/cutflow_calo_data.txt'):
    #    file['datacalo'] = path+'/cutflow_calo_data.txt'
    #    label['datacalo'] = 'Calo Ref. Sel.'
    #if os.path.isfile(path+'/PF/cutflow_PF_data.txt'):
    #    file['dataPF'] = path+'/cutflow_PF_data.txt'
    #    label['dataPF'] = 'PF Ref. Sel.'
    if os.path.isfile(path+'/JPT/cutflow_JPT_data.txt'):
        file['dataJPTMET'] = path+'/JPT/cutflow_JPT_data.txt'
        label['dataJPTMET'] = 'JPT MET$>$20, Iso$<$0.1'
    if os.path.isfile(path+'/calo/cutflow_calo_data.txt'):    
        file['datacaloMET'] = path+'/calo/cutflow_calo_data.txt'
        label['datacaloMET'] = 'Calo MET$>$20, Iso$<$0.1'
    if os.path.isfile(path+'/PF/cutflow_PF_data.txt'):
        file['dataPFMET'] = path+'/PF/cutflow_PF_data.txt'
        label['dataPFMET'] = 'PF MET$>$20, Iso$<$0.1, p^{jet}_{T}>25'

                    
    
keys = file.keys()
cutflow = {}
cutlabel = {}
cutlabel['Processed'] = 'Processed'
cutlabel['CleanFilters'] = 'CleanFilters'
cutlabel['HLT'] = 'HLT'
cutlabel['GoodPV'] = 'Good PV'
cutlabel['OneIsoMu'] = 'One Iso mu'
cutlabel['LooseMuVeto'] = 'Loose mu veto'
cutlabel['ElectronVeto'] = 'Electron veto'
cutlabel['MET'] = 'MET'
cutlabel['1Jet'] = '1 jet'
cutlabel['2Jet'] = '2 jets'
cutlabel['3Jet'] = '3 jets'
cutlabel['4Jet'] = '$\geq$4 jets'

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
        #if sample=="data":
        if sample.find("data")!=-1:
            cutmap[ cutname ] = int(float(acut))
        else:
            cutmap[ cutname ] = float(acut)
    txtfile.close()
    
    scale = 1.
    if IsMC and Lumi>0:
        scale = ( Lumi * xsec[ sample ] / cutmap['CleanFilters'] )

    ilabel = 0
    for key in cutmap.keys():

        cutmap[ key ] = scale * cutmap[ key]
        print " cut "+str(key) + " ("+cutlabel[key]+") "+" = "+str( round(cutmap[key],1) )
        if allmap.has_key(key):
            allmap[ key ] += cutmap[ key ]
        else:
            allmap[ key ] = cutmap[ key ]
        ilabel += 1
    cutflow[ sample ] = cutmap

print " TOTAL"
ilabel=0
for key in allmap.keys():
    print " cut "+str(key) + " ("+cutlabel[key]+") "+" = "+str( round(allmap[key],1) )
    ilabel += 1
    
cutflow["Total"] = allmap

# write latex
sortedcutlist = ['CleanFilters','HLT','GoodPV','OneIsoMu','LooseMuVeto','ElectronVeto','MET','1Jet','2Jet','3Jet','4Jet']
if IsMC:
    cutlabel['CleanFilters'] = 'Processed'

         
tablelist = ['ttbar','Wjets','Zjets','QCD','tch','tWch','Total']
if Lumi<=0:
    tablelist = ['ttbar','Wjets','Zjets','QCD','tch','tWch']
    
if not IsMC:
    #tablelist = ['MB','Jun14','Jul16','Prompt','Total']
    #tablelist = ['dataJPT','datacalo','dataJPTMET','datacaloMET']
    tablelist = ['datacaloMET','dataJPTMET','dataPFMET']
    
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
    aline = '\\begin{tabular}{|l|r|r|r|r|r|r|r|} \hline \n'
    if Lumi<=0:
        aline = '\\begin{tabular}{|l|r|r|r|r|r|r|} \hline \n'
    outtex.write(aline)
    #outtex.write('Cut & ttbar & Wjets & Zjets & QCD & STtch \hline')
    line = "Cut"
    for sample in tablelist:
        line += sss+label[sample]
    outtex.write(line+" \\\ \hline \n")
else:
    aline = '\\begin{tabular}{|l|r|r|r|} \hline \n'
    
    outtex.write(aline)
    line = "Cut"
    for sample in tablelist:
        line += sss+label[sample]
    outtex.write(line+" \\\ \hline \n")

ilabel = 0                   
#for key in allmap.keys():
for key in sortedcutlist:
    
    line = cutlabel[key]
    
    for sample in tablelist:
        cutmap = cutflow[sample]
        acut = cutmap[key]
        stracut = str(int(acut))
        #stracut = stracut.strip('.0')
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

print "file "+texname+ " has been written."

    

