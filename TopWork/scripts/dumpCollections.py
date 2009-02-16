from ROOT import *
from PhysicsTools.PythonAnalysis import *

gSystem.Load("libFWCoreFWLite.so")
AutoLibraryLoader.enable()

file = ROOT.TDCacheFile("/pnfs/cms/WAX/11/store/mc/Summer08/TauolaTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/16AAC418-218A-DD11-AC33-001F2908F0E4.root")

events = file.Get("Events")

events.Print()


