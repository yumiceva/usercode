
import string

events= 100
#inputType = "DATA" # choose MC/DATA
inputType = "DATA"

channel = "muon" # muon/electron

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

out = "%s-*-PATskim.root"%eventtype
outfile = string.replace(out,"*","PAT")
outntuple = string.replace(out,"*","Tuple")

import FWCore.ParameterSet.Config as cms
process = cms.Process("UFO")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("Yumiceva.Top7TeV.PATNtupleMaker_cfi")
process.PATNtupleMaker.inputType = inputType
process.PATNtupleMaker.ntupleFile = outntuple
process.load("Yumiceva.Top7TeV.PATElectronNtupleMaker_cfi")
process.PATElectronNtupleMaker.inputType = inputType
process.PATElectronNtupleMaker.ntupleFile = outntuple

process.PATNtupleMaker.MuonTag = 'selectedPatMuonsPFlow'
process.PATNtupleMaker.ElectronTag = 'selectedPatElectrons'
process.PATNtupleMaker.PFJetTag = 'goodPatJetsPFlow'
process.PATNtupleMaker.PFMETTag = 'patMETsPFlow'
process.PATNtupleMaker.Verbose = True

### Input ###
# My typical PoolSource
#process.load(source)
#process.maxEvents.input = events
#process.source.skipEvents = 0
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

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(outntuple)
)


### Triggers ###

# muon trigger
triggerprocess = "HLT"
patTriggerName = "HLT_Mu9"

if channel=="muon":
    mufilter = "hltSingleMu9L3Filtered9"
    if eventtype == "Run2010B_Mu15":
        mufilter = "hltSingleMu15L3Filtered15"
        patTriggerName = "HLT_Mu15"
        
    process.triggerFilter = cms.EDFilter("MyHLTSummaryFilter",
                                         summary = cms.InputTag("hltTriggerSummaryAOD","",triggerprocess),
                                         member  = cms .InputTag(mufilter,"",triggerprocess),
                                         cut     = cms.string(""),
                                         minN    = cms.int32(1)
                                         )


### GEN decay channel, for tT MC events only ###
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")
process.load("PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi")
process.cFlavorHistoryProducer.matchedSrc = "ak5GenJets"
process.bFlavorHistoryProducer.matchedSrc = "ak5GenJets"
    

### The process path ###
process.p = cms.Path(

    #process.triggerFilter *

#    process.makeGenEvt *
#    process.flavorHistorySeq *
    
    process.PATNtupleMaker *
    process.PATElectronNtupleMaker
)

if channel == "muon":
    process.p.remove( process.PATElectronNtupleMaker )
else:
    process.p.remove( process.PATNtupleMaker )
            
if inputType=="MC":
    process.p.remove( process.triggerFilter )
    if eventtype.find("TTJets")==-1:
    	process.p.remove( process.makeGenEvt )
    if eventtype.find("WJets")==-1 and eventtype.find("ZJets")==-1 and eventtype.find("Vqq")==-1 and eventtype.find("Wc")==-1:
        process.p.remove( process.flavorHistorySeq )

else:
    process.p.remove( process.makeGenEvt )
    process.p.remove( process.flavorHistorySeq )

                    

