
import math
from ROOT import *

class Selector:

    def __init__(self):

        self.muons = []
        self.electrons = []
        self.jets = []
        self.hist = None
        self.IsoCut = 0.1
        self.PVz = 0
        self.ApplyDeltaR = True
        self.MinJetPt = 25.
        self.LeadingJetPtCut = 25.
        self.p4MET = None
    def Jets(self, jets): self.jets = jets
    def Muons(self,muons): self.muons = muons
    def Electrons(self,electrons): self.electrons = electrons
    def SetPVz(self,value): self.PVz = value
    def SetIso(self,value): self.IsoCut = value
    def SetMinJetPt(self,value): self.MinJetPt = value
    def SetLeadingJetPt(self,value): self.LeadingJetPtCut = value
    def Setp4MET(self,value): self.p4MET = value
        
    def ElectronLoose(self, ele):
        accepted = False
        if ele.reliso03<0.2:
            accepted = True
        return accepted
    
    def ElectronTight(self, ele):

        accepted = False
        
        eta_sc = ele.etasc
        # check loose iso electrons
        if ele.reliso03<0.2:
            
            if ele.pt>30. and ( (eta_sc > -2.5 and eta_sc < -1.566) or (math.fabs(eta_sc)<1.4442) or (eta_sc > 1.566 and eta_sc< 2.5) ):

                if not self.hist:
                    self.hist.electrons['pt_cut1'].Fill(ele.pt)
                    self.hist.electrons['d0_cut1'].Fill(ele.d0)
            
                if math.fabs(ele.d0)<0.02 and ele.pass70==1:

                    if not self.hist:
                        self.hist.electrons['pt_cut2'].Fill(ele.pt)
                        self.hist.electrons['reliso'].Fill(ele.reliso03)
                
                    if ele.reliso03 < self.IsoCut:

                        eleVz = ele.vz
                        if not self.hist: self.hist.electrons['dz'].Fill( math.fabs(eleVz - self.PVz))
                    
                        if math.fabs(eleVz - self.PVz) < 1.:
                            accepted = True

        return accepted

    def ElectronZveto(self, p4ele):
        isZevent = False
        nloose95electrons = 0
        for ele in self.electrons:

            eta_sc = ele.etasc
            pass95 = ele.pass95

            tmpp4ele = TLorentzVector()
            tmpp4ele.SetPtEtaPhiE( ele.pt, ele.eta, ele.phi, ele.e)
            if tmpp4ele.Et()>20. and ( math.fabs(eta_sc)< 1.4442 or math.fabs(eta_sc)>1.566 ) and math.fabs(ele.eta)<2.5 and ele.reliso03<1.0 and math.fabs(eta_sc)<2.5:
                
                if pass95:
                    p4secondElec = TLorentzVector()
                    p4secondElec.SetPtEtaPhiE( ele.pt, ele.eta, ele.phi, ele.e)
                    nloose95electrons += 1
                    Zp4 = (p4ele + p4secondElec)
                    Zmass = Zp4.M()
                    if Zmass >= 76. and Zmass <= 106.:
                        isZevent = true
        return isZevent
                                                                                                        
    def MuonReverse(self, mu):
        accepted = False

        theLeadingPt = TLorentzVector()
        
        aDeltaR = 999
        if len(self.jets)>0: theLeadingPt = self.jets[0].pt
        tmpp4Mu = TLorentzVector()
        tmpp4Mu.SetPtEtaPhiE(mu.pt, mu.eta, mu.phi, mu.e )
        
        for jet in self.jets:
            if jet.pt>self.MinJetPt and theLeadingPt>self.LeadingJetPtCut:
                tmpp4Jet= TLorentzVector()
                tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e )
                tmpdeltaR = tmpp4Mu.DeltaR(tmpp4Jet)
                #if tmpdeltaR < 0.1 and JetType=="JPT": continue
                #if tmpdeltaR < aDeltaR and tmpdeltaR>0.01: aDeltaR = tmpdeltaR
                if tmpdeltaR < aDeltaR: aDeltaR = tmpdeltaR
                del(tmpp4Jet)
        aDeltaPhi =tmpp4Mu.DeltaPhi(self.p4MET)
        del(tmpp4Mu)
        if mu.pt>20. and math.fabs(mu.eta)<2.1 and mu.IsTrackerMuon==1 and \
               mu.muonhits>0 and mu.normchi2<10 and \
               mu.trackerhits>=11 and mu.muonstations> 1 and \
               mu.pixelhits >= 1 and \
               math.fabs(mu.vz - self.PVz) < 1. and aDeltaR>0.3 and \
               ( math.fabs(mu.d0)>0.025 or (mu.reliso03<0.7 and mu.reliso03>0.15) ) and \
               math.fabs( aDeltaPhi ) < 2:
                    
            #ntightmuons += 1
            #p4muon.SetPtEtaPhiE( mu.pt, mu.eta, mu.phi, mu.e )
            if not self.hist: self.hist.muons['reliso'].Fill(mu.reliso03)
            accepted = True
            

        return accepted
                
    def MuonLoose(self, mu):
        accepted = False
        if mu.pt>10. and math.fabs(mu.eta)<2.5 and mu.reliso03<0.2:
            accepted = True
        return accepted

    def MuonTight(self, mu):
        accepted = False

        theLeadingPt = 0
                        
        if mu.pt>20. and math.fabs(mu.eta)<2.1 and mu.IsTrackerMuon==1:

            if not self.hist:
                self.hist.muons['pt_cut1'].Fill(mu.pt)
                self.hist.muons['d0_cut1'].Fill(mu.d0)
            
            if math.fabs(mu.d0)<0.02 and \
                   mu.muonhits>0 and \
                   mu.normchi2<10 and \
                   mu.trackerhits>=11 and \
                   mu.muonstations> 1 and \
                   mu.pixelhits >= 1:

                if not self.hist:
                    self.hist.muons['pt_cut2'].Fill(mu.pt)
                    self.hist.muons['reliso'].Fill(mu.reliso03)
                
                if mu.reliso03 < self.IsoCut:
                    
                    muonVz = mu.vz
                    if not self.hist: self.hist.muons['dz'].Fill( math.fabs(muonVz - self.PVz))
                    
                    if math.fabs(muonVz - self.PVz) >= 1.:
                        return accepted
                    
                    if not self.hist: self.hist.muons['pt_cut3'].Fill(mu.pt)

                    aDeltaR = 999
                    if len(self.jets)>0: theLeadingPt = self.jets[0].pt

                    tmpp4Mu = TLorentzVector()
                    tmpp4Mu.SetPtEtaPhiE(mu.pt, mu.eta, mu.phi, mu.e )
                    
                    for jet in self.jets:
                        
                        if jet.pt>self.MinJetPt and theLeadingPt>self.LeadingJetPtCut:
                            tmpp4Jet= TLorentzVector()
                            tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e )
                            tmpdeltaR = tmpp4Mu.DeltaR(tmpp4Jet)
                            #if tmpdeltaR < 0.1 and JetType=="JPT": continue
                            #if tmpdeltaR < aDeltaR and tmpdeltaR>0.01: aDeltaR = tmpdeltaR
                            if tmpdeltaR < aDeltaR: aDeltaR = tmpdeltaR
                            del(tmpp4Jet)
                    del(tmpp4Mu)
                    if aDeltaR < 999: self.hist.muons['deltaR'].Fill(aDeltaR)
                            
                    if not self.ApplyDeltaR:
                        accepted = True
                        #ntightmuons += 1
                        #p4muon.SetPtEtaPhiE( mu.pt, mu.eta, mu.phi, mu.e )
                    elif aDeltaR>0.3:
                        accepted = True
                        #ntightmuons += 1
                        #p4muon.SetPtEtaPhiE( mu.pt, mu.eta, mu.phi, mu.e )

        return accepted

