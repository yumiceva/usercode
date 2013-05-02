import string
import os

####### Parameters ############
###############################
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')

options.register ('useData',
                  False,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  "Run this on real data")

options.register ('channel',
                  'muon',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "channel muon or electron")

options.register ('events',
                  1000,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  "number of events to process")

options.register ('verbose',
                  False,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  "verbose")

options.parseArguments()

inputType = "DATA"
if not options.useData :
    inputType = "MC"

events= options.events
channel = options.channel 
eventtype = "UFO"

fileaddix = "_Mu.root"
if channel == "electron":
    fileaddix = "_El.root"
       
#jsonfile = "json/Cert_160404-163369_7TeV_PromptReco_Collisions11_JSON_MuonPhys.txt"
jsonfile = "Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON_v2.txt"


out = "%s-*-PATskim.root"%eventtype
outfile = string.replace(out,"*","PAT")
outntuple = string.replace(out,"*","Tuple")

print options

import FWCore.ParameterSet.Config as cms
process = cms.Process("UFO")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff')
process.MessageLogger.suppressWarning.append('patTrigger')
process.MessageLogger.cerr.FwkJob.limit=1
process.MessageLogger.cerr.ERROR = cms.untracked.PSet( limit = cms.untracked.int32(0) )

## Ntuple maker
process.load("Yumiceva.Top7TeV.PATNtupleMaker_cfi")
process.PATNtupleMaker.inputType = inputType
process.PATNtupleMaker.ntupleFile = outntuple

process.PATNtupleMaker.hltList = ["HLT_IsoMu24_eta2p1_v*"] # v12
#process.PATNtupleMaker.hltList = 'HLT_Mu24_v1','HLT_Mu24_v2','HLT_Mu24_v3','HLT_Mu24_v4','HLT_Mu24_v7'
if channel=="electron":
    #process.PATNtupleMaker.hltList = 'HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT','HLT_Ele32_CaloIdL_CaloIsoVL_SC17'
    process.PATNtupleMaker.hltList = 'HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2','HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1'
process.PATNtupleMaker.hltTag = cms.InputTag("patTriggerEvent")
process.PATNtupleMaker.MuonTag = 'selectedPatMuonsPFlow'
process.PATNtupleMaker.ElectronTag = 'selectedPatElectrons'
process.PATNtupleMaker.PFJetTag = 'goodPatJetsPFlow'
process.PATNtupleMaker.PFMETTag = 'patMETsPFlow'
process.PATNtupleMaker.PVTag = 'goodOfflinePrimaryVertices'
if options.verbose:
    process.PATNtupleMaker.Verbose = True

## Input PAT files for testing
if inputType=="DATA":
    process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(
        '/store/results/B2G/SingleMu/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/SingleMu/USER/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/0000/F85709B7-113B-E211-81DD-00261894386D.root',
        '/store/results/B2G/SingleMu/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/SingleMu/USER/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/0000/F62E1CBC-103B-E211-9BC6-00261894386D.root',
        '/store/results/B2G/SingleMu/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/SingleMu/USER/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/0000/F61F433A-163B-E211-A4C9-002590593878.root',
        '/store/results/B2G/SingleMu/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/SingleMu/USER/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/0000/F2E0741F-103B-E211-B1E4-0025905964A6.root',
        '/store/results/B2G/SingleMu/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/SingleMu/USER/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/0000/F067911D-163B-E211-8837-002590596484.root',
        '/store/results/B2G/SingleMu/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/SingleMu/USER/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/0000/EE22AE21-143B-E211-8B93-003048FFCB96.root',
        '/store/results/B2G/SingleMu/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/SingleMu/USER/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/0000/EC7B99DC-113B-E211-8F21-003048FFCB8C.root',
        '/store/results/B2G/SingleMu/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/SingleMu/USER/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/0000/E8B34F70-163B-E211-9AFF-00259059642E.root',
        '/store/results/B2G/SingleMu/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/SingleMu/USER/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/0000/E6DE7962-123B-E211-9F03-00248C55CC40.root',
        '/store/results/B2G/SingleMu/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/SingleMu/USER/StoreResults-SingleMu_Run2012A-13Jul2012-v1_TLBSM_53x_v2_jsonfix-e3fb55b810dc7a0811f4c66dfa2267c9/0000/E23AB9D5-163B-E211-8089-003048FFCC1E.root'
        
        ))
    process.GlobalTag.globaltag = cms.string( 'GR_P_V40_AN1::All' )
    
if inputType=="MC":
    process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(
#        '/store/user/srappocc/TTJets_TuneZ2_7TeV-madgraph-tauola/ttbsm_v1_Spring11-PU_S1_START311_V1G1-v1/3900271fc94df2539ec6b7c2deffc3db/ttbsm_413_9_1_0vd.root',
#        '/store/user/srappocc/TTJets_TuneZ2_7TeV-madgraph-tauola/ttbsm_v1_Spring11-PU_S1_START311_V1G1-v1/3900271fc94df2539ec6b7c2deffc3db/ttbsm_413_8_1_1Pn.root',
#        '/store/user/srappocc/TTJets_TuneZ2_7TeV-madgraph-tauola/ttbsm_v1_Spring11-PU_S1_START311_V1G1-v1/3900271fc94df2539ec6b7c2deffc3db/ttbsm_413_7_1_zlE.root'
#        '/store/user/lpctlbsm/weizou/WprimeToTBbar_M-1000_TuneZ2_7TeV-pythia6/ttbsm_v8_Summer11-PU_S4_-START42_V11-v1/2bcf344afee8f9cb5489a05cc32c05cf/ttbsm_42x_mc_1_1_prX.root'
        '/store/user/lpctlbsm/meloam/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1_TLBSM_53x_v2/c04f3b4fa74c8266c913b71e0c74901d/tlbsm_53x_v2_mc_9_1_HeW.root'
        ))
    process.GlobalTag.globaltag = cms.string( 'START53_V7E::All' )
    
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( events ))

## JSON Filter
if os.path.exists(jsonfile) and inputType=="DATA":
    import FWCore.PythonUtilities.LumiList as LumiList
    import FWCore.ParameterSet.Types as CfgTypes

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

process.load('PhysicsTools.PatAlgos.triggerLayer1.triggerEventProducer_cfi')


### The process path ###
process.p = cms.Path(

    process.patTriggerDefaultSequence * process.PATNtupleMaker

)
