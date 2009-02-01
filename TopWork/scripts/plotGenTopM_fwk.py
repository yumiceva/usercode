from ROOT import *
from PhysicsTools.PythonAnalysis import *

gSystem.Load("libFWCoreFWLite.so")
AutoLibraryLoader.enable()

events = EventTree("dcache:/pnfs/cms/WAX/11/store/mc/Summer08/TauolaTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/16AAC418-218A-DD11-AC33-001F2908F0E4.root")

h1 = TH1F("h1","h1",100,50,250)

entry = 0;

for event in events:
    #entry += 1
    #print "Event ", count
    #gen = event.getProduct("recoGenParticles_genParticles__HLT.obj")
    muons = event.getProduct("recoMuons_muons__RECO.obj")
    #for p in gen:
        #print p.pdgId()
    #    if abs( int(p.pdgId()) ) == 6:
            #print p.mass()
    #        h1.Fill(float(p.mass()))
    


cv = TCanvas("cv","cv",700,700)
h1.Draw()
cv.Print("topmass.png")
