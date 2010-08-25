#! /usr/bin/env python

from ROOT import *

class Hist:

    def __init__(self):

        self.muons = {}
        self.electrons = {}
        self.jets = {}
        self.M3 = {}
        self.M3chi2 = {}
        self.all = {}

    def Create(self, name):

        self.CreateMuon(name)
        self.all = self.muons.copy()
        self.all.update( self.electrons)
        self.all.update(self.jets)
        self.all.update(self.M3 )
        self.all.update(self.M3chi2)

        for key in self.all.keys():
            self.all[key].SetXTitle(self.all[key].GetTitle())

    def Write(self):
        for key in self.all.keys():
            self.all[key].Write()
    
    def CreateMuon(self, name):
        name = "_"+name
        self.muons['pt'] = TH1F("muon_pt"+name,"p_{T}^{#mu}", 25, 0, 100)
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
                
