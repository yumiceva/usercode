### Modeled from
### TopQuarkAnalysis/TopEventProducers/test/tqaf_cfg.py
### PhysicsTools/PatAlgos/test/patLayer1_fromAOD_full_cfg.py
### PhysicsTools/PatAlgos/python/patTemplate_cfg.py
### UserCode/LJMet/Production/test/PatSkim_lite_cfg.py
### https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideTQAFRecipes
### https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePATRecipes
### https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions

### General setup ###

import string

events= 10
inputType = "MC" # choose MC/DATA

eventtype="TTJets"
#eventtype="Top-s"
#eventtype="Top-t"
#eventtype="Top-tW"
#eventtype="WJets"
#eventtype="ZJets"
#eventtype="Vqq"
#eventtype="Wc"
#eventtype="QCD-Mu15"
#eventtype="QCD-100to250"
#eventtype="QCD-250to500"
#eventtype="QCD-500to1000"
#eventtype="QCD-1000toInf"
#eventtype="QCD-Mu5-Pt30"
out = "%s-*-PATskim.root"%eventtype
outfile = string.replace(out,"*","PAT")
outntuple = string.replace(out,"*","Ntuple")
source = "Source.PoolSource.%s-AODSIM-PoolSource"%eventtype

from PhysicsTools.PatAlgos.patTemplate_cfg import *

# Send all CMSSW output to stdout
process.MessageLogger.cout = process.MessageLogger.cerr
process.MessageLogger.cerr = cms.untracked.PSet(
    placeholder = cms.untracked.bool( True )
)
process.MessageLogger.cerr_stats.output = 'cout'
process.options.wantSummary = True
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.GlobalTag.globaltag = 'START36_V10::All' # Newest 36X conditions for 7TeV startup
#process.GlobalTag.globaltag = 'START3X_V26::All' # Used for MC samples re-reco'ed in 357

### Input ###
# My typical PoolSource
#process.load(source)
#process.maxEvents.input = events
#process.source.skipEvents = 0
# tT+jet input file
process.source.fileNames = cms.untracked.vstring(
    '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0016/6E7C4631-9D47-DF11-96CE-003048C69288.root'
)
process.maxEvents.input = events

### Speed up I/O using patches ###
# https://twiki.cern.ch/twiki/bin/view/Main/CmsIOWork
process.source.cacheSize = cms.untracked.uint32(20*1024*1024)
process.AdaptorConfig = cms.Service(
    "AdaptorConfig",
    enable=cms.untracked.bool(True),
    stats = cms.untracked.bool(True),
    cacheHint = cms.untracked.string("auto-detect"),
    readHint = cms.untracked.string("direct-unbuffered")
)


### PAT setup ###
process.load("PhysicsTools.PatAlgos.patSequences_cff")
# Since these are re-RECO samples, the ak5GenJets collection is missing
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePATFAQs#Running_PAT_in_3_3_X_on_a_sa_AN1
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
run36xOn35xInput(process, "ak5GenJets" )
# switch to 8e29 trigger menu and add PAT trigger to patDefaultSequence
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
# 35X MC re-reco labeled newest trigger REDIGI
process.patTriggerEvent.processName = 'REDIGI'
process.patTrigger.processName = 'REDIGI'
# shrink the jet content
# Keep the b-tag discriminators, just not the info used to calculate them,
# such as tracks and vertices.
setTagInfos(
    process,
    "patJets",
    cms.vstring("secondaryVertexTagInfos")
)

### Muons ###
process.patMuons.isoDeposits = cms.PSet()
# Embed user data into the PAT objects
process.patMuonsUserData = cms.EDProducer(
    "PATMuonUserData",
    src = cms.InputTag("patMuons")
)
# Insert the user data process into the PAT sequence
process.makePatMuons.replace(getattr(process,"patMuons"),getattr(process,"patMuons")*process.patMuonsUserData)
# Use BeamSpot instead of primary vertex
process.patMuons.usePV = False
# Tell selectedPatMuons to use the new user collection as input
process.selectedPatMuons.src = "patMuonsUserData"
process.selectedPatMuons.cut = "pt>10. & abs(eta)<2.5 & muonID('AllGlobalMuons')"
#process.countPatMuons.minNumber = 1
process.muonMatch.checkCharge = False

### Electrons ###
process.patElectrons.addElectronID = False
process.patElectrons.isoDeposits = cms.PSet()
process.patElectrons.embedGsfTrack = False
process.patElectrons.embedSuperCluster = False
# Embed user data into the PAT objects
process.patElectronsUserData = cms.EDProducer(
    "PATElectronUserData",
    src = cms.InputTag("patElectrons"),
    ecalRecHitsEBsrc = cms.InputTag("reducedEcalRecHitsEB")
)
# Insert the user data process into the PAT sequence
process.makePatElectrons.replace(getattr(process,"patElectrons"),getattr(process,"patElectrons")*process.patElectronsUserData)
# Tell selectedPatElectrons to use the new user collection as input
process.selectedPatElectrons.src = "patElectronsUserData"
process.selectedPatElectrons.cut = "et>15 & abs(eta)<2.5 & userFloat('CombRelIso')<0.2 & userFloat('SwissCross')<0.95"
#process.countPatElectrons.maxNumber = 0
process.electronMatch.checkCharge = False

### Jets ###
# Running on Summer09 data using 7TeV JEC
# Use default patJetCorrFactors in 36X
#process.patJetCorrFactors.corrSample = 'Summer09_7TeV_ReReco332'
process.selectedPatJets.cut = 'pt > 20. & abs(eta) < 2.4' # & emEnergyFraction>0.01 & jetID.n90Hits>1 & jetID.fHPD<0.98'
#process.countPatJets.minNumber = 1
process.patJets.embedGenJetMatch = False

### New SSVHP b-tag ###
# https://hypernews.cern.ch/HyperNews/CMS/get/btag/525/2.html
process.load("RecoBTag.SecondaryVertex.simpleSecondaryVertex3TrkES_cfi")
process.load("RecoBTag.SecondaryVertex.simpleSecondaryVertexHighPurBJetTags_cfi")
process.patJets.discriminatorSources += [cms.InputTag("simpleSecondaryVertexHighPurBJetTags")]

### Photons ###
process.patPhotons.isoDeposits = cms.PSet()
process.patPhotons.embedSuperCluster = False

### Taus ###
process.patTaus.isoDeposits = cms.PSet()

### Filters used ###

### Veto scraping events ###
process.scrapingVeto = cms.EDFilter(
    "FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
)

### Remove events with anomalous HCAL noise ###
process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')

### Trigger ###
process.triggerFilter = cms.EDFilter(
    'TriggerFilter',
    menu = cms.string("patTriggerEvent"),
    path = cms.string("HLT_Mu9")
)

### Primary vertex ###
# The below, provided from DPGAnalysis/Skims, doesn't include a
# requirement on isFake(), so we use our own home-brewed filter
#process.primaryVertexFilter = cms.EDFilter(
#    "GoodVertexFilter",
#    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
#    minimumNDOF = cms.uint32(4),
#    maxAbsZ = cms.double(15),
#    maxd0 = cms.double(2)
#)
# Because the top group keeps fiddling with this, keep the vertex
# collection for subsequent filtering instead.
#process.pvFilter = cms.EDFilter(
#    "PVFilter",
#    source = cms.string("offlinePrimaryVertices")
#)

### GEN decay channel, for tT MC events only ###
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")

### Flavor history, for W+jet, Z+jet, Vqq, & Wc MC events only ###
process.load("PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi")
process.cFlavorHistoryProducer.matchedSrc = "ak5GenJets"
process.bFlavorHistoryProducer.matchedSrc = "ak5GenJets"

### V flavor history plots, for W+jet, Z+jet, Vqq, & Wc events only ###
process.vFlavor = cms.EDAnalyzer(
    'VFlavor'
)

### Prune the GEN particle collection ###
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.prunedGenParticles = cms.EDProducer("GenParticlePruner",
    src = cms.InputTag("genParticles"),
    select = cms.vstring(
    "drop  *",
     #keeps all particles from the hard matrix element
    "keep status = 3",
     #keeps all stable muons and electrons and their (direct) mothers.
    "+keep (abs(pdgId) = 11 | abs(pdgId) = 13) & status = 1"
    )
)

process.load("Yumiceva.Top7TeV.PATNtupleMaker_cfi")
process.PATNtupleMaker.inputType = inputType
process.PATNtupleMaker.ntupleFile = outntuple

### The process path ###
process.p = cms.Path(
#    process.eventsProcessed *
    process.simpleSecondaryVertexHighPurBJetTags *
    process.patDefaultSequence *
    #process.flavorHistorySeq * # Use only for W+jet, Z+jet, Vqq, Wc (MC) events
    #process.vFlavor *          # Use only for W+jet, Z+jet, Vqq, Wc (MC) events
    process.scrapingVeto *
#    process.eventsPassingScrapingVeto *
    process.HBHENoiseFilter *
#    process.eventsPassingHCALNoiseFilter *
#    process.triggerFilter *
#    process.eventsPassingTrigger *
    #process.pvFilter *
    process.makeGenEvt *        # Use only for tT (MC) events
    process.prunedGenParticles *
    process.PATNtupleMaker
#    process.eventsPassed
)

### Output ###
# For a PAT Skim
process.out.outputCommands += ['drop *_cleanPat*_*_*',
			       'keep *_flavorHistoryFilter_*_*',
                               'keep *_prunedGenParticles_*_*',
                               'keep *_offlinePrimaryVertices_*_*',
                               'keep *_selectedPat*_*_*',
			       'keep *_decaySubset_*_*',
			       'keep *_initSubset_*_*',
			       'keep *_offlineBeamSpot_*_*',
                               'keep *_genEvt_*_*',
                               'drop *_selectedPatTaus_*_*']
# Keep all PAT trigger collections
#process.out.outputCommands += patTriggerStandAloneEventContent

# Keep everything, including input
process.out.outputCommands = ['keep *']

process.out.fileName = outfile

# No output
#del(process.outpath)


