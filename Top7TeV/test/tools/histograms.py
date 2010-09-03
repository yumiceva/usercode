#! /usr/bin/env python

from ROOT import *

class Hist:

    def __init__(self):

        self.muons = {}
        self.electrons = {}
        self.jets = {}
        self.M3 = {}
        self.M3chi2 = {}
        self.MET = {}
        self.Mt = {}

        self.tfile = TFile()
        
    def SetTFile(self, tfile):
        self.tfile = tfile
    
    def Create(self, name):

        name = '_'+name
        self.CreateMuon(name)
        self.CreateMass(name)
        self.CreateMET(name)
        self.CreateMt(name)
        self.CreateJet(name)

    def Write(self):
        self.WriteMap(self.muons,"muons")
        self.WriteMap(self.electrons,"electrons")
        self.WriteMap(self.jets,"jets")
        self.WriteMap(self.M3,"M3")
        self.WriteMap(self.M3chi2,"M3")
        self.WriteMap(self.MET,"MET")
        self.WriteMap(self.Mt,"Mt")
        
    def WriteMap(self, map, name):
        if self.tfile.IsOpen():
            self.tfile.cd()
            if not self.tfile.GetKey(name):
                self.tfile.mkdir(name)
            self.tfile.cd(name)
            for key in map.keys():
                map[key].Write()
    
    def CreateMuon(self, name):
        
        self.muons['pt'] = TH1F("muon_pt"+name,"p_{T}^{#mu}", 25, 20, 100)
        self.muons['pt_1jet'] = TH1F("muon_pt_1jet"+name,"p_{T}^{#mu}", 25, 20, 100)
        self.muons['pt_2jet'] = TH1F("muon_pt_2jet"+name,"p_{T}^{#mu}", 25, 20, 100)
        self.muons['pt_3jet'] = TH1F("muon_pt_3jet"+name,"p_{T}^{#mu}", 25, 20, 100)
        self.muons['pt_4jet'] = TH1F("muon_pt_4jet"+name,"p_{T}^{#mu}", 25, 20, 100)
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
        self.muons['deltaR'] = TH1F("deltaR"+name,"#DeltaR(#mu,jet)",30, 0, 4)
        self.muons['d0_cut1'] = TH1F("d0_cut1"+name,"#mu Impact Parameter [cm]",20,-0.1,0.1)
        self.muons['pt_cut1'] = TH1F("muon_pt_cut1"+name,"p_{T}^{#mu}", 25, 0, 100)
        self.muons['pt_cut2'] = TH1F("muon_pt_cut2"+name,"p_{T}^{#mu}", 25, 0, 100)
        self.SetupXTitle(self.muons)
                        
    def CreateMass(self, name):
        
        self.M3['3jet'] = TH1F("M3_3jet"+name,"M3 [GeV/c^{2}]", 20, 0, 500)
        self.M3['4jet'] = TH1F("M3_4jet"+name,"M3 [GeV/c^{2}]", 20, 0, 500)
        self.SetupXTitle(self.M3)
                        
    def CreateMET(self, name):

        self.MET['MET'] = TH1F("MET"+name,"Missing Transverse Energy [GeV]", 20, 0, 110)
        self.MET['phi'] = TH1F("MET_phi"+name,"#phi Missing Transverse Energy [GeV]", 20, 0, 3.15)
        self.MET['Ht'] = TH1F("Ht"+name,"H_{T} [GeV]", 30, 0, 160)
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
        self.jets['1_pt'] = TH1F("jet1_pt"+name,"Leading jet p_{T} [GeV/c]",50, 30, 300)
        self.jets['2_pt'] = TH1F("jet2_pt"+name,"2nd jet p_{T} [GeV/c]",50, 30, 300)
        self.jets['3_pt'] = TH1F("jet3_pt"+name,"3rd jet p_{T} [GeV/c]",50, 30, 300)
        self.jets['4_pt'] = TH1F("jet4_pt"+name,"4th jet p_{T} [GeV/c]",50, 30, 300)
        self.jets['eta'] = TH1F("jet_eta"+name,"jet #eta",50, -2.4, 2.4)
        self.jets['phi'] = TH1F("jet_phi"+name,"jet #phi",50, 0, 3.15)
        self.SetupXTitle(self.jets)
        
    def SetupXTitle(self, map):
        for key in map.keys():
            map[key].SetXTitle(map[key].GetTitle())
                        
