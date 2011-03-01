#! /usr/bin/env python

from ROOT import *

class Hist:

    def __init__(self):

        self.muons = {}
        self.electrons = {}
        self.jets = {}
        self.M3 = {}
        self.MET = {}
        self.Mt = {}
        self.Various = {}
        
        self.tfile = TFile()
        
    def SetTFile(self, tfile):
        self.tfile = tfile
    
    def Create(self, name):

        name = '_'+name
        if name.find("Low")!=-1:
            self.CreateMttbar(name)
        elif name.find("High")!=-1:
            self.CreateMttbar(name)
        else:
            self.CreateMuon(name)
            self.CreateElectron(name)
            self.CreateMass(name)
            self.CreateMET(name)
            self.CreateMt(name)
            self.CreateJet(name)
            self.CreateVarious(name)
        
    def Write(self):
        self.WriteMap(self.muons,"muons")
        self.WriteMap(self.electrons,"electrons")
        self.WriteMap(self.jets,"jets")
        self.WriteMap(self.M3,"M3")
        self.WriteMap(self.MET,"MET")
        self.WriteMap(self.Mt,"Mt")
        self.WriteMap(self.Various,"various")
        
    def WriteMap(self, map, name):
        if self.tfile.IsOpen():
            self.tfile.cd()
            if not self.tfile.GetKey(name):
                self.tfile.mkdir(name)
            self.tfile.cd(name)
            for key in map.keys():
                if map[key].GetEntries>0:
                    map[key].Write()
                else:
                    print "Empty histogram: "+map[key].GetName()+" it will not be written to file."

    def CreateVarious(self, name):
        self.Various['flavor'] = TH1F("flavor"+name,"Flavor History",12,0,12)
        self.SetupXTitle(self.Various)
        
    def CreateMuon(self, name):
        
        self.muons['pt'] = TH1F("muon_pt"+name,"p_{T}^{#mu} [GeV/c]", 25, 20, 100)
        self.muons['pt_1jet'] = TH1F("muon_pt_1jet"+name,"p_{T}^{#mu} [GeV/c]", 25, 20, 100)
        self.muons['pt_2jet'] = TH1F("muon_pt_2jet"+name,"p_{T}^{#mu} [GeV/c]", 25, 20, 100)
        self.muons['pt_3jet'] = TH1F("muon_pt_3jet"+name,"p_{T}^{#mu} [GeV/c]", 25, 20, 100)
        self.muons['pt_4jet'] = TH1F("muon_pt_4jet"+name,"p_{T}^{#mu} [GeV/c]", 25, 20, 100)
        self.muons['eta'] = TH1F("muon_eta"+name,"#eta^{#mu}", 20, -2.1, 2.1)
        self.muons['eta_1jet'] = TH1F("muon_eta_1jet"+name,"#eta^{#mu}", 20, -2.1, 2.1)
        self.muons['eta_2jet'] = TH1F("muon_eta_2jet"+name,"#eta^{#mu}", 20, -2.1, 2.1)
        self.muons['eta_3jet'] = TH1F("muon_eta_3jet"+name,"#eta^{#mu}", 20, -2.1, 2.1)
        self.muons['eta_4jet'] = TH1F("muon_eta_4jet"+name,"#eta^{#mu}", 20, -2.1, 2.1)
        self.muons['phi'] = TH1F("muon_phi"+name,"#phi^{#mu}", 20, 0, 3.15)
        self.muons['phi_1jet'] = TH1F("muon_phi_1jet"+name,"#phi^{#mu}", 20, 0, 3.15)
        self.muons['phi_2jet'] = TH1F("muon_phi_2jet"+name,"#phi^{#mu}", 20, 0, 3.15)
        self.muons['phi_3jet'] = TH1F("muon_phi_3jet"+name,"#phi^{#mu}", 20, 0, 3.15)
        self.muons['phi_4jet'] = TH1F("muon_phi_4jet"+name,"#phi^{#mu}", 20, 0, 3.15)
        self.muons['reliso'] = TH1F("muon_reliso"+name,"Relative Isolation", 30, 0, 1.5)
        self.muons['reliso_1jet'] = TH1F("muon_reliso_1jet"+name,"Relative Isolation", 30, 0, 1.5)
        self.muons['reliso_2jet'] = TH1F("muon_reliso_2jet"+name,"Relative Isolation", 30, 0, 1.5)
        self.muons['reliso_3jet'] = TH1F("muon_reliso_3jet"+name,"Relative Isolation", 30, 0, 1.5)
        self.muons['reliso_4jet'] = TH1F("muon_reliso_4jet"+name,"Relative Isolation", 30, 0, 1.5)
        self.muons['deltaR_cut0'] = TH1F("deltaR_cut0"+name,"#DeltaR(#mu,jet)",80, 0, 4)
        self.muons['deltaR'] = TH1F("deltaR"+name,"#DeltaR(#mu,jet)",80, 0, 4)
        self.muons['d0_cut1'] = TH1F("d0_cut1"+name,"#mu Impact Parameter [cm]",20,-0.1,0.1)
        self.muons['pt_cut1'] = TH1F("muon_pt_cut1"+name,"p_{T}^{#mu} [GeV/c]", 25, 0, 100)
        self.muons['pt_cut2'] = TH1F("muon_pt_cut2"+name,"p_{T}^{#mu} [GeV/c]", 25, 0, 100)
        self.muons['pt_cut3'] = TH1F("muon_pt_cut3"+name,"p_{T}^{#mu} [GeV/c]", 25, 0, 100)
        self.muons['dz'] = TH1F('dz'+name,"|z(#mu) - z_{PV}| [cm]", 25, 0, 1.)
        self.SetupXTitle(self.muons)

    def CreateElectron(self, name):

        self.electrons['pt'] = TH1F("electron_pt"+name,"p_{T}^{#mu} [GeV/c]", 25, 20, 100)
        self.electrons['pt_1jet'] = TH1F("electron_pt_1jet"+name,"p_{T}^{#mu} [GeV/c]", 25, 20, 100)
        self.electrons['pt_2jet'] = TH1F("electron_pt_2jet"+name,"p_{T}^{#mu} [GeV/c]", 25, 20, 100)
        self.electrons['pt_3jet'] = TH1F("electron_pt_3jet"+name,"p_{T}^{#mu} [GeV/c]", 25, 20, 100)
        self.electrons['pt_4jet'] = TH1F("electron_pt_4jet"+name,"p_{T}^{#mu} [GeV/c]", 25, 20, 100)
        self.electrons['eta'] = TH1F("electron_eta"+name,"#eta^{#mu}", 20, -2.1, 2.1)
        self.electrons['eta_1jet'] = TH1F("electron_eta_1jet"+name,"#eta^{#mu}", 20, -2.1, 2.1)
        self.electrons['eta_2jet'] = TH1F("electron_eta_2jet"+name,"#eta^{#mu}", 20, -2.1, 2.1)
        self.electrons['eta_3jet'] = TH1F("electron_eta_3jet"+name,"#eta^{#mu}", 20, -2.1, 2.1)
        self.electrons['eta_4jet'] = TH1F("electron_eta_4jet"+name,"#eta^{#mu}", 20, -2.1, 2.1)
        self.electrons['phi'] = TH1F("electron_phi"+name,"#phi^{#mu}", 20, 0, 3.15)
        self.electrons['phi_1jet'] = TH1F("electron_phi_1jet"+name,"#phi^{#mu}", 20, 0, 3.15)
        self.electrons['phi_2jet'] = TH1F("electron_phi_2jet"+name,"#phi^{#mu}", 20, 0, 3.15)
        self.electrons['phi_3jet'] = TH1F("electron_phi_3jet"+name,"#phi^{#mu}", 20, 0, 3.15)
        self.electrons['phi_4jet'] = TH1F("electron_phi_4jet"+name,"#phi^{#mu}", 20, 0, 3.15)
        self.electrons['reliso'] = TH1F("electron_reliso"+name,"Relative Isolation", 30, 0, 1.5)
        self.electrons['reliso_1jet'] = TH1F("electron_reliso_1jet"+name,"Relative Isolation", 30, 0, 1.5)
        self.electrons['reliso_2jet'] = TH1F("electron_reliso_2jet"+name,"Relative Isolation", 30, 0, 1.5)
        self.electrons['reliso_3jet'] = TH1F("electron_reliso_3jet"+name,"Relative Isolation", 30, 0, 1.5)
        self.electrons['reliso_4jet'] = TH1F("electron_reliso_4jet"+name,"Relative Isolation", 30, 0, 1.5)
        self.electrons['deltaR_cut0'] = TH1F("electron_deltaR_cut0"+name,"#DeltaR(#mu,jet)",80, 0, 4)
        self.electrons['deltaR'] = TH1F("electron_deltaR"+name,"#DeltaR(#mu,jet)",80, 0, 4)
        self.electrons['d0_cut1'] = TH1F("electron_d0_cut1"+name,"#mu Impact Parameter [cm]",20,-0.1,0.1)
        self.electrons['pt_cut1'] = TH1F("electron_pt_cut1"+name,"p_{T}^{#mu} [GeV/c]", 25, 0, 100)
        self.electrons['pt_cut2'] = TH1F("electron_pt_cut2"+name,"p_{T}^{#mu} [GeV/c]", 25, 0, 100)
        self.electrons['dz'] = TH1F('electron_dz'+name,"|z(#mu) - z_{PV}| [cm]", 25, 0, 1.)
        self.SetupXTitle(self.electrons)
        
    def CreateMass(self, name):

        self.M3['M3'] = TH1F("M3"+name,"M3 [GeV/c^{2}]", 40, 0, 1000)
        self.M3['3jet'] = TH1F("M3_3jet"+name,"M3 [GeV/c^{2}]", 40, 0, 1000)
        self.M3['4jet'] = TH1F("M3_4jet"+name,"M3 [GeV/c^{2}]", 40, 0, 1000)
        self.M3['3jet_SSVHEM_1b'] = TH1F("M3_3jet_SSVHEM_1b"+name,"M3 [GeV/c^{2}]", 40, 0, 1000)
        self.M3['4jet_SSVHEM_1b'] = TH1F("M3_4jet_SSVHEM_1b"+name,"M3 [GeV/c^{2}]", 40, 0, 1000)
        self.M3['3jet_TCHPL_1b'] = TH1F("M3_3jet_TCHPL_1b"+name,"M3 [GeV/c^{2}]", 40, 0, 1000)
        self.M3['4jet_TCHPL_1b'] = TH1F("M3_4jet_TCHPL_1b"+name,"M3 [GeV/c^{2}]", 40, 0, 1000)
        self.M3['M3chi2_hadW_4jet'] = TH1F("M3chi2_hadW_4jet"+name,"hadronic W mass M3-chi2", 20, 0,150)
        self.M3['M3chi2_hadTop_4jet'] = TH1F("M3chi2_hadTop_4jet"+name,"M3-chi2 [GeV/c^{2}]", 40, 0, 800)
        self.M3['M3chi2_hadTop_0btag'] = TH1F("M3chi2_hadTop_0btag"+name,"M3-chi2 [GeV/c^{2}]", 40, 0, 800)
        self.M3['M3chi2_hadTop_1btag'] = TH1F("M3chi2_hadTop_1btag"+name,"M3-chi2 [GeV/c^{2}]", 40, 0, 800)
        self.M3['M3chi2_hadTop_2btag'] = TH1F("M3chi2_hadTop_2btag"+name,"M3-chi2 [GeV/c^{2}]", 40, 0, 800)
        self.M3['M3chi2_lepTop_4jet'] = TH1F("M3chi2_lepTop_4jet"+name,"M3-chi2 [GeV/c^{2}]", 40, 0, 800)
        self.M3['M3chi2_lepTop_0btag'] = TH1F("M3chi2_lepTop_0btag"+name,"M3-chi2 [GeV/c^{2}]", 40, 0, 800)
        self.M3['M3chi2_lepTop_1btag'] = TH1F("M3chi2_lepTop_1btag"+name,"M3-chi2 [GeV/c^{2}]", 40, 0, 800)
        self.M3['M3chi2_lepTop_2btag'] = TH1F("M3chi2_lepTop_2btag"+name,"M3-chi2 [GeV/c^{2}]", 40, 0, 800)                        
        self.M3['M3chi2_hadTop_lepTop_4jet'] = TH2F("M3chi2_hadTop_lepTop_4jet"+name,"hadronic vs leptonic top M3chi2 [GeV/c^{2}]", 40, 0, 1000,40,1,1000)
        self.M3['M3chi2_normchi2_1st'] = TH1F("M3chi2_normchi2_1st"+name,"#chi^{2}/ndof",30,0,30)
        self.M3['M3chi2_normchi2_2nd'] = TH1F("M3chi2_normchi2_2nd"+name,"#chi^{2}/ndof",30,0,30)
        self.M3['M3chi2_normchi2_3th'] = TH1F("M3chi2_normchi2_3th"+name,"#chi^{2}/ndof",30,0,30)
        self.M3['M3chi2_normchi2_4th'] = TH1F("M3chi2_normchi2_4th"+name,"#chi^{2}/ndof",30,0,30)
        self.M3['Mttbar_chi2'] = TH1F('Mttbar_chi2'+name,"m_{t#bar{t}} [GeV/c^{2}]",28,200,1600)
        self.M3['Mttbar_chi2_0btag'] = TH1F('Mttbar_chi2_0btag'+name,"m_{t#bar{t}} [GeV/c^{2}]",28,200,1600)
        self.M3['Mttbar_chi2_1btag'] = TH1F('Mttbar_chi2_1btag'+name,"m_{t#bar{t}} [GeV/c^{2}]",28,200,1600)
        self.M3['Mttbar_chi2_2btag'] = TH1F('Mttbar_chi2_2btag'+name,"m_{t#bar{t}} [GeV/c^{2}]",28,200,1600)
        self.M3['pt_ttbar'] = TH1F("pt_ttbar"+name,"p_{T}(t#bar{t}) [GeV]",50,0,1000)
        self.M3['pt_ttbar_0btag'] = TH1F("pt_ttbar_0btag"+name,"p_{T}(t#bar{t}) [GeV]",50,0,1000)
        self.M3['pt_ttbar_1btag'] = TH1F("pt_ttbar_1btag"+name,"p_{T}(t#bar{t}) [GeV]",50,0,1000)        
        self.M3['pt_ttbar_2btag'] = TH1F("pt_ttbar_2btag"+name,"p_{T}(t#bar{t}) [GeV]",50,0,1000)
        self.M3['lepTop_vs_Mttbar'] = TH2F("lepTop_vs_Mttbar"+name,"leptonic top mass vs m_{t#bar{t}} [GeV/c^{2}]",40,0,800,28,200,1600)
        self.M3['pt_vs_Mttbar'] = TH2F("pt_vs_Mttbar"+name,"p_{T}(t#bar{t}) vs m_{t#bar{t}}",50,0,1000,28,200,1600)
        self.M3['pt_vs_Mttbar_0btag'] = TH2F("pt_vs_Mttbar_0btag"+name,"p_{T}(t#bar{t}) vs m_{t#bar{t}}",50,0,1000,28,200,1600)
        self.M3['pt_vs_Mttbar_1btag'] = TH2F("pt_vs_Mttbar_1btag"+name,"p_{T}(t#bar{t}) vs m_{t#bar{t}}",50,0,1000,28,200,1600)
        self.M3['pt_vs_Mttbar_2btag'] = TH2F("pt_vs_Mttbar_2btag"+name,"p_{T}(t#bar{t}) vs m_{t#bar{t}}",50,0,1000,28,200,1600)
        self.M3['MET_vs_Mttbar'] = TH2F("MET_vs_Mttbar"+name,"MET vs m_{t#bar{t}}",20,0,110,28,200,1600)
        self.M3['MET_vs_Mttbar_0btag'] = TH2F("MET_vs_Mttbar_0btag"+name,"MET vs m_{t#bar{t}}",20,0,110,28,200,1600)
        self.M3['MET_vs_Mttbar_1btag'] = TH2F("MET_vs_Mttbar_1btag"+name,"MET vs m_{t#bar{t}}",20,0,110,28,200,1600)
        self.M3['MET_vs_Mttbar_2btag'] = TH2F("MET_vs_Mttbar_2btag"+name,"MET vs m_{t#bar{t}}",20,0,110,28,200,1600)
        self.SetupXTitle(self.M3)
                        
    def CreateMET(self, name):

        self.MET['MET'] = TH1F("MET"+name,"Missing Transverse Energy [GeV]", 20, 0, 110)
        self.MET['MET_1jet'] = TH1F("MET_1jet"+name,"Missing Transverse Energy [GeV]", 20, 0, 110)
        self.MET['MET_2jet'] = TH1F("MET_2jet"+name,"Missing Transverse Energy [GeV]", 20, 0, 110)
        self.MET['MET_3jet'] = TH1F("MET_3jet"+name,"Missing Transverse Energy [GeV]", 20, 0, 110)
        self.MET['MET_4jet'] = TH1F("MET_4jet"+name,"Missing Transverse Energy [GeV]", 20, 0, 110)
        self.MET['phi_cut0'] = TH1F("MET_phi_cut0"+name,"#phi Missing Transverse Energy [GeV]", 20, 0, 3.15)
        self.MET['phi'] = TH1F("MET_phi"+name,"#phi Missing Transverse Energy [GeV]", 20, 0, 3.15)
        self.MET['Ht'] = TH1F("Ht"+name,"H_{T} [GeV]", 50, 0, 1000)
        self.MET['Ht_1jet'] = TH1F("Ht_1jet"+name,"H_{T} [GeV]", 50, 0, 1000)
        self.MET['Ht_2jet'] = TH1F("Ht_2jet"+name,"H_{T} [GeV]", 50, 0, 1000)
        self.MET['Ht_3jet'] = TH1F("Ht_3jet"+name,"H_{T} [GeV]", 50, 0, 1000)
        self.MET['Ht_4jet'] = TH1F("Ht_4jet"+name,"H_{T} [GeV]", 50, 0, 1000)
        self.MET['Htlep'] = TH1F("Htlep"+name,"H_{T,lep} [GeV]", 50, 0, 1000)
        self.MET['Htlep_1jet'] = TH1F("Htlep_1jet"+name,"H_{T,lep} [GeV]", 50, 0, 1000)
        self.MET['Htlep_2jet'] = TH1F("Htlep_2jet"+name,"H_{T,lep} [GeV]", 50, 0, 1000)
        self.MET['Htlep_3jet'] = TH1F("Htlep_3jet"+name,"H_{T,lep} [GeV]", 50, 0, 1000)
        self.MET['Htlep_4jet'] = TH1F("Htlep_4jet"+name,"H_{T,lep} [GeV]", 50, 0, 1000)
        self.MET['deltaPhi_1jet'] = TH1F("deltaPhi_1jet"+name,"#Delta#phi(l,MET)", 50, 0, 3.15)
        self.MET['deltaPhi_2jet'] = TH1F("deltaPhi_2jet"+name,"#Delta#phi(l,MET)", 50, 0, 3.15)
        self.MET['deltaPhi_3jet'] = TH1F("deltaPhi_3jet"+name,"#Delta#phi(l,MET)", 50, 0, 3.15)
        self.MET['deltaPhi_4jet'] = TH1F("deltaPhi_4jet"+name,"#Delta#phi(l,MET)", 50, 0, 3.15)
        self.MET['LepWmass'] = TH1F("LepWmass"+name,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150)
        self.MET['LepWmass_1jet'] = TH1F("LepWmass_1jet"+name,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150)
        self.MET['LepWmass_2jet'] = TH1F("LepWmass_2jet"+name,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150)
        self.MET['LepWmass_3jet'] = TH1F("LepWmass_3jet"+name,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150)
        self.MET['LepWmass_4jet'] = TH1F("LepWmass_4jet"+name,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150)
        self.MET['LepWmassComplex']=TH1F("LepWmassComplex"+name,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150)
        self.MET['LepWmassComplex_1jet']=TH1F("LepWmassComplex_1jet"+name,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150)
        self.MET['LepWmassComplex_2jet']=TH1F("LepWmassComplex_2jet"+name,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150)
        self.MET['LepWmassComplex_3jet']=TH1F("LepWmassComplex_3jet"+name,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150)
        self.MET['LepWmassComplex_4jet']=TH1F("LepWmassComplex_4jet"+name,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150)
        self.MET['PzNu'] = TH1F("PzNu"+name,"p_{z} #nu [GeV]", 40, -300,300) 
        self.SetupXTitle(self.MET)
                        
    def CreateMt(self, name):

        self.Mt['Mt'] = TH1F("Mt"+name,"M_{T}(W) [GeV/c^{2}]", 20, 0, 150)
        self.Mt['Mt_1jet'] = TH1F("Mt_1jet"+name,"M_{T}(W) [GeV/c^{2}]", 20, 0, 150)
        self.Mt['Mt_2jet'] = TH1F("Mt_2jet"+name,"M_{T}(W) [GeV/c^{2}]", 20, 0, 150)
        self.Mt['Mt_3jet'] = TH1F("Mt_3jet"+name,"M_{T}(W) [GeV/c^{2}]", 20, 0, 150)
        self.Mt['Mt_4jet'] = TH1F("Mt_4jet"+name,"M_{T}(W) [GeV/c^{2}]", 20, 0, 150)
        self.SetupXTitle(self.Mt)

    def CreateJet(self, name):

        self.jets['pt'] = TH1F("jet_pt"+name,"jet p_{T} [GeV/c]",50, 30, 300)
        self.jets['1st_pt_N3j'] = TH1F("jet1_pt_N3j"+name,"Leading jet p_{T} [GeV/c]",50, 30, 300)
        self.jets['2nd_pt_N3j'] = TH1F("jet2_pt_N3j"+name,"2nd jet p_{T} [GeV/c]",50, 30, 300)
        self.jets['3th_pt_N3j'] = TH1F("jet3_pt_N3j"+name,"3th jet p_{T} [GeV/c]",50, 30, 300)
        self.jets['1st_pt_N4j'] = TH1F("jet1_pt_N4j"+name,"Leading jet p_{T} [GeV/c]",50, 30, 300)
        self.jets['2nd_pt_N4j'] = TH1F("jet2_pt_N4j"+name,"2nd jet p_{T} [GeV/c]",50, 30, 300)
        self.jets['3th_pt_N4j'] = TH1F("jet3_pt_N4j"+name,"3th jet p_{T} [GeV/c]",50, 30, 300)
        self.jets['4th_pt_N4j'] = TH1F("jet4_pt_N4j"+name,"4th jet p_{T} [GeV/c]",50, 30, 300)
        self.jets['eta'] = TH1F("jet_eta"+name,"jet #eta",50, -2.4, 2.4)
        self.jets['phi'] = TH1F("jet_phi"+name,"jet #phi",50, 0, 3.15)
        self.jets['Njets'] = TH1F("Njets"+name,"jet multiplicity",5,0,5)
        self.jets['Njets_btag'] = TH1F("Njets_btag"+name,"jet multiplicity",5,0,5)
        self.jets['Nbjets_TCHPL_N3j'] = TH1F("Nbjets_TCHPL_N3j"+name,"Tagged b-jets",3,0,3)
        self.jets['Nbjets_TCHPL_N4j'] = TH1F("Nbjets_TCHPL_N4j"+name,"Tagged b-jets",3,0,3)
        self.jets['Nbjets_SSVHEM_N3j'] = TH1F("Nbjets_SSVHEM_N3j"+name,"Tagged b-jets",3,0,3)
        self.jets['Nbjets_SSVHEM_N4j'] = TH1F("Nbjets_SSVHEM_N4j"+name,"Tagged b-jets",3,0,3)
        
        self.SetupXTitle(self.jets)

    def CreateMttbar(self, name):
        
        self.muons['pt_4jet'] = TH1F("muon_pt_4jet"+name,"p_{T}^{#mu} [GeV/c]", 25, 20, 100)
        self.muons['eta_4jet'] = TH1F("muon_eta_4jet"+name,"#eta^{#mu}", 20, -2.1, 2.1)
        self.muons['phi_4jet'] = TH1F("muon_phi_4jet"+name,"#phi^{#mu}", 20, 0, 3.15)                                                
        self.jets['Njets'] = TH1F("Njets"+name,"jet multiplicity",6,4,10)
        self.MET['MET_4jet'] = TH1F("MET_4jet"+name,"Missing Transverse Energy [GeV]", 20, 0, 110)
        self.electrons['pt_4jet'] = TH1F("electron_pt_4jet"+name,"p_{T}^{#mu} [GeV/c]", 25, 20, 100)
        self.MET['Ht_4jet'] = TH1F("Ht_4jet"+name,"H_{T} [GeV]", 50, 0, 1000)
        self.MET['Htlep_4jet'] = TH1F("Htlep_4jet"+name,"H_{T,lep} [GeV]", 50, 0, 1000)
        self.jets['jetdeltaR'] = TH1F("jetdeltaR"+name,"#DeltaR(jet1,jet2)",80, 0, 4)                                        
        self.SetupXTitle(self.muons)
        self.SetupXTitle(self.electrons)
        self.SetupXTitle(self.MET)
        self.SetupXTitle(self.jets)
        
    def SetupXTitle(self, map):
        for key in map.keys():
            map[key].SetXTitle(map[key].GetTitle())
            map[key].Sumw2()
