

class Combo:
        
    def __init__(self):
        self.Wp = None
        self.Wq = None
        self.HadW = None
        self.Hadb = None
        self.HadTop = None
        self.LepW = None
        self.Lepb = None
        self.LepTop = None
        self.TopPair = None

        self.SumPt = 0.
        self.chi2 = 0.
        self.Ndof = 0.

        self.MW = 84.2
        self.Mtop_h = 172.
        self.Mtop_l = 172.

        self.sigmaHadW = 10.5
        self.sigmaHadt = 19.2
        self.sigmaLept = 24.2

        self.usebtag = False
        self.useMTop = True
        self.IsGoodbTagEvent = False
        self.Wp_istag = self.Wq_istag = self.Hadb_istag = self.Lepb_istag = False
                                            
    def UsebTagging( self, option):
        self.usebtag = option
        
    def SetWp( self, Wp ):
        self.Wp = Wp
        
    def SetWq( self, Wq ):
        self.Wq = Wq

    def SetHadb( self, Hadb ):
        self.Hadb = Hadb

    def SetLepW( self, LepW ):
        self.LepW = LepW

    def SetLepb( self, Lepb ):
        self.Lepb = Lepb

    def SetWp_tag( self, tag):
        self.Wp_istag = tag

    def SetWq_tag( self, tag):
        self.Wq_istag = tag

    def SetHadb_tag( self, tag):
        self.Hadb_istag = tag

    def SetLepb_tag( self, tag):
        self.Lepb_istag = tag

    def GetWp(self): return self.Wp
    def GetWq(self): return self.Wq
    def GetHadb(self): return self.Hadb
    def GetLepb(self): return self.Lepb
    def GetHadW(self): return self.HadW
    def GetLepW(self): return self.LepW
    def GetHadTop(self): return self.HadTop
    def GetLepTop(self): return self.LepTop
    def GetTopPair(self): return self.TopPair
    def GetChi2(self): return self.chi2
    def GetNdof(self): return self.Ndof
    def GetSumPt(self): return self.SumPt
    
    def analyze(self):

        self.HadW = self.Wp + self.Wq
        self.HadTop = self.HadW + self.Hadb
        self.LepTop = self.LepW + self.Lepb
        self.TopPair = self.HadTop + self.LepTop

        chiHadW = (self.HadW.M() - self.MW)/self.sigmaHadW
        chiHadt = (self.HadTop.M() - self.Mtop_h)/self.sigmaHadt
        chiLept = (self.LepTop.M() - self.Mtop_l)/self.sigmaLept

        if self.useMTop:
            self.chi2 = chiHadW*chiHadW + chiHadt*chiHadt + chiLept*chiLept
            self.Ndof = 3
        else:
            self.chi2 = chiHadW*chiHadW + (self.HadTop.M() - self.LepTop.M())*(self.HadTop.M() - self.LepTop.M())/(self.sigmaHadt*self.sigmaHadt+self.sigmaLept*self.sigmaLept)
            self.Ndof = 2

        self.SumPt = self.HadTop.Pt()

        if self.usebtag:

            if self.Wp_istag or self.Wq_istag or self.Hadb_istag or self.Lepb_istag:

                self.IsGoodbTagEvent = True

                

import itertools
 
class JetCombinatorics:


    def __init__(self):
        self.theLepW = None
        self.theOtherLepW = None
        self.UsebTags = False
        self.UseMtop = True
        self.combinations = []
        self.tagCombinations = []
    def Clear(self):
        self.combinations = []
        self.tagCombinations = []
        self.UsebTags = False
        self.UseMtop = True
        self.theOtherLepW = None
        
    def UsebTagging(self, option=True):
        self.UsebTags = option
    def UseMtopConstraint(self, option):
        self.UseMtop = option
    def SetLeptonicW( self, LepW ):
        self.theLepW = LepW

    def SetOtherLeptonicW( self, LepW):
        self.theOtherLepW

    def GetCombinationSumEt( self, n):
        #print self.combinations
        #tmp = sorted( self.combinations, key=lambda combo: combo.GetSumPt() )
        #print tmp[n].GetSumPt()
        #print tmp[1].GetSumPt()
        #tmp.reverse()
        #print tmp[n].GetSumPt()
        tmplist = self.combinations
        if self.UsebTags:
            tmplist = self.tagCombinations

        if len(tmplist)==0:
            return None
        else:
            return tmplist[n]

    def GetCombination( self, n ):
        tmplist = sorted( self.combinations, key=lambda combo: combo.GetChi2() )
        if self.UsebTags:
            tmplist = sorted( self.tagCombinations, key=lambda combo: combo.GetChi2() )

        if len(tmplist)==0:
            return None
        else:
            return tmplist[n]
    
    def FourJetsCombinations( self, jets, tags ):

        #print jets
        #print tags
        biglist = list(itertools.permutations( jets, 4))
        #print biglist
        biglisttag = list(itertools.permutations( tags, 4))
        #print biglisttag
        # loop over two neutrino solutions
        for ineutrino in range(0,2):
            
            aLepW = self.theLepW
            if self.theOtherLepW!=None and ineutrino==1 and self.theLepW.Pz()!=self.theOtherLepW.Pz():
                aLepW = self.theOtherLepW
            elif ineutrino==1:
                continue
            #print "neutrino = " +str(ineutrino)
            for i in range(0,len(biglist)):
                
                aset = biglist[i]
                asettag = biglisttag[i]
            
                aCombo = Combo()
                aCombo.SetWp( aset[0] )
                aCombo.SetWq( aset[1] )
                aCombo.SetHadb( aset[2] )
                aCombo.SetLepW( aLepW )
                aCombo.SetLepb( aset[3] )
                aCombo.useMtop = self.UseMtop
                if self.UsebTags:
                    aCombo.UsebTagging(True)
                    aCombo.SetWp_tag( asettag[0] )
                    aCombo.SetWq_tag( asettag[1] )
                    aCombo.SetHadb_tag( asettag[2] )
                    aCombo.SetLepb_tag( asettag[3] )
                    
                aCombo.analyze()

                #print "Combination "+str(i)+" chi2= "+str(aCombo.GetChi2())
                self.combinations.append( aCombo )
                if self.UsebTags:
                    if aCombo.IsGoodbTagEvent:
                        self.tagCombinations.append( aCombo )
        # sort in sum pt
        tmplist = sorted( self.combinations, key=lambda combo: combo.GetSumPt() )
        
        last = tmplist[-1]
        for i in range(len(tmplist)-2, -1, -1):
            if last.GetSumPt() == tmplist[i].GetSumPt():
                del tmplist[i]
            else:
                last = tmplist[i]
                
        self.combinations = tmplist

        if not self.UsebTags or len(self.tagCombinations)==0:
            return
        
        tmplist = sorted( self.tagCombinations, key=lambda combo: combo.GetSumPt() )

        last = tmplist[-1]
        for i in range(len(tmplist)-2, -1, -1):
            if last.GetSumPt() == tmplist[i].GetSumPt():
                del tmplist[i]
            else:
                last = tmplist[i]

        self.tagCombinations = tmplist
        
        
