
import string
import os

events= 1000
#inputType = "DATA" # choose MC/DATA
inputType = "DATA"

#channel = "muon" # muon/electron

#eventtype="Run2010A_Mu9"
eventtype="TTJets"
#eventtype="Top-s"
#eventtype="Top-t"
#eventtype="Top-tW"
#eventtype="WJets"
#eventtype="ZJets"
#eventtype="Vqq"
#eventtype="Wc"
#eventtype="TPrime_400M_Mu"

#jsonfile = "json/Cert_160404-163369_7TeV_PromptReco_Collisions11_JSON_MuonPhys.txt"
jsonfile = ''

out = "%s-*-PATskim.root"%eventtype
outfile = string.replace(out,"*","PAT")
outntuple = string.replace(out,"*","Tuple")

import FWCore.ParameterSet.Config as cms
process = cms.Process("UFO")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Ntuple maker
process.load("Yumiceva.Top7TeV.PATNtupleMaker_cfi")
process.PATNtupleMaker.inputType = inputType
process.PATNtupleMaker.ntupleFile = outntuple

process.PATNtupleMaker.hltList = 'HLT_Mu24_v1','HLT_Mu30_v1','HLT_IsoMu24_v1'
# electrons: HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1 HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v1

process.PATNtupleMaker.MuonTag = 'selectedPatMuonsPFlow'
process.PATNtupleMaker.ElectronTag = 'selectedPatElectrons'
process.PATNtupleMaker.PFJetTag = 'goodPatJetsPFlow'
process.PATNtupleMaker.PFMETTag = 'patMETsPFlow'
#process.PATNtupleMaker.Verbose = True

## Input PAT files
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
#    '/store/user/srappocc/TTJets_TuneZ2_7TeV-madgraph-tauola/ttbsm_v1_Spring11-PU_S1_START311_V1G1-v1/3900271fc94df2539ec6b7c2deffc3db/ttbsm_413_9_1_0vd.root',
#    '/store/user/srappocc/TTJets_TuneZ2_7TeV-madgraph-tauola/ttbsm_v1_Spring11-PU_S1_START311_V1G1-v1/3900271fc94df2539ec6b7c2deffc3db/ttbsm_413_8_1_1Pn.root',
#    '/store/user/srappocc/TTJets_TuneZ2_7TeV-madgraph-tauola/ttbsm_v1_Spring11-PU_S1_START311_V1G1-v1/3900271fc94df2539ec6b7c2deffc3db/ttbsm_413_7_1_zlE.root'
'/store/user/yumiceva/SingleMu/ttbsm_v2_SingleMu2011A_v1/84471d8a18e499e217065966b63862b9/ttbsm_414_data_9_1_5is.root',
'/store/user/yumiceva/SingleMu/ttbsm_v2_SingleMu2011A_v1/84471d8a18e499e217065966b63862b9/ttbsm_414_data_99_1_bCM.root',
'/store/user/yumiceva/SingleMu/ttbsm_v2_SingleMu2011A_v1/84471d8a18e499e217065966b63862b9/ttbsm_414_data_98_1_dWx.root'

))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( events ))

## JSON Filter
import PhysicsTools.PythonAnalysis.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
if os.path.exists(jsonfile) and inputData=="DATA":
    print "Using JSON file "+jsonfile
    myLumis = LumiList.LumiList(filename = jsonfile).getCMSSWString().split(',')
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    process.source.lumisToProcess.extend(myLumis)

    
process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(outntuple)
)


### GEN decay channel, for tT MC events only ###
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")
process.load("PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi")
process.cFlavorHistoryProducer.matchedSrc = "ak5GenJets"
process.bFlavorHistoryProducer.matchedSrc = "ak5GenJets"
    

### The process path ###
process.p = cms.Path(

    process.PATNtupleMaker

)
            


                    

