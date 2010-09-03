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

events= 1000
inputType = "DATA" # choose MC/DATA

eventtype="Jun14"

out = "%s-*-PATskim.root"%eventtype
outfile = string.replace(out,"*","PAT")
outntuple = string.replace(out,"*","Ntuple")
source = "Source.PoolSource.%s-PoolSource"%eventtype

from PhysicsTools.PatAlgos.patTemplate_cfg import *

# Send all CMSSW output to stdout
process.MessageLogger.cout = process.MessageLogger.cerr
process.MessageLogger.cerr = cms.untracked.PSet(
    placeholder = cms.untracked.bool( True )
)
process.MessageLogger.cerr_stats.output = 'cout'
#process.MessageLogger.cout.FwkReport.reportEvery = 500
process.options.wantSummary = True
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
#process.GlobalTag.globaltag = 'GR_R_36X_V12A::All' # For the Jun 14 re-reco
#process.GlobalTag.globaltag = 'GR_R_36X_V12B::All' # For the Jul 16 re-reco
#process.GlobalTag.globaltag = 'GR10_P_V7::All' # For PromptReco
process.GlobalTag.globaltag = 'GR_R_36X_V12::All' # for all

### Input ###
# My typical PoolSource
#process.load(source)
#process.maxEvents.input = events
#process.source.skipEvents = 0
# tT+jet input file
process.source.fileNames = cms.untracked.vstring(
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0006/B0222EAB-B07B-DF11-AE0A-001E0B5FE542.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/F25FF224-F47A-DF11-AE84-0017A4770828.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/EEBDBF25-F47A-DF11-9E8F-0017A477083C.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/E6F21093-EF7A-DF11-9985-0017A4770C38.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/DED36991-F17A-DF11-A857-0017A477141C.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/CC6AA9E2-EF7A-DF11-A84D-001E0B5FC422.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/C06427E0-EF7A-DF11-861D-0017A4771004.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/B25323B8-F47A-DF11-932C-0017A4770028.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/B0A68691-F37A-DF11-920E-00237DA14F92.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/A8DD8DF0-EF7A-DF11-9176-001E0BEACAB8.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/94C12994-EF7A-DF11-91F9-001E0B5FC422.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/94B4688F-EF7A-DF11-9512-0017A4770C10.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/84070BDC-EF7A-DF11-BA3C-00237DA13CAC.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/72BE83F5-EF7A-DF11-9669-001CC443B76C.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/720CA171-F47A-DF11-BF4D-0017A477041C.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/6AC5E58D-F17A-DF11-AAC3-0017A4770004.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/6A86E9F7-EF7A-DF11-A7AF-0017A4770C34.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/584B5E90-F37A-DF11-A8D5-0017A4770024.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/48CE97B5-F47A-DF11-9BF4-001E0B4A0EFC.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/40ACF3E7-EF7A-DF11-A840-0017A4770430.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/3E6B78EA-F17A-DF11-AEF9-0017A4771030.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/30212CEB-EF7A-DF11-9EAE-0017A4770828.root',
    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/2EC3A00D-F07A-DF11-9B09-00237DA41368.root'
    
)
process.maxEvents.input = events

### Speed up I/O using patches ###
# https://twiki.cern.ch/twiki/bin/view/Main/CmsIOWork
#process.source.cacheSize = cms.untracked.uint32(20*1024*1024)
#process.AdaptorConfig = cms.Service(
#    "AdaptorConfig",
#    enable=cms.untracked.bool(True),
#    stats = cms.untracked.bool(True),
#    cacheHint = cms.untracked.string("auto-detect"),
#    readHint = cms.untracked.string("direct-unbuffered")
#)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(outntuple)
)


### PAT setup ###
process.load("PhysicsTools.PatAlgos.patSequences_cff")
# Since these are re-RECO samples, the ak5GenJets collection is missing
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePATFAQs#Running_PAT_in_3_3_X_on_a_sa_AN1
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
# switch to 8e29 trigger menu and add PAT trigger to patDefaultSequence
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
# 35X MC re-reco labeled newest trigger REDIGI
process.patTriggerEvent.processName = 'HLT'
process.patTrigger.processName = 'HLT'
# shrink the jet content
# Keep the b-tag discriminators, just not the info used to calculate them,
# such as tracks and vertices.
setTagInfos(
    process,
    "patJets",
    cms.vstring("secondaryVertexTagInfos")
)
# Running on real data, we want to turn off all MC matching
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])

### Muons ###
process.patMuons.isoDeposits = cms.PSet()
# Embed user data into the PAT objects
#process.patMuonsUserData = cms.EDProducer(
#    "PATMuonUserData",
#    src = cms.InputTag("patMuons")
#)
# Insert the user data process into the PAT sequence
#process.makePatMuons.replace(getattr(process,"patMuons"),getattr(process,"patMuons")*process.patMuonsUserData)
# Use BeamSpot instead of primary vertex
process.patMuons.usePV = False
# Tell selectedPatMuons to use the new user collection as input
#process.selectedPatMuons.src = "patMuonsUserData"
process.selectedPatMuons.cut = "pt>10. & abs(eta)<2.5 & muonID('AllGlobalMuons')"
#process.countPatMuons.minNumber = 1
process.muonMatch.checkCharge = False

### Electrons ###
# We need electrons corrected for EE misalignment
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEgammaCorrectedElectrons
process.load("RecoEgamma.EgammaTools.correctedElectronsProducer_cfi")
process.patElectrons.electronSource = "gsfElectrons::PAT"
process.patElectrons.addElectronID = False
# If using electron ID (we don't):
#process.load('RecoEgamma.ElectronIdentification.electronIdSequence_cff')
#process.eidTight.src = "gsfElectrons::PAT"
#process.eidLoose.src = "gsfElectrons::PAT"
#process.eidRobustTight.src = "gsfElectrons::PAT"
#process.eidRobustHighEnergy.src = "gsfElectrons::PAT"
#process.eidRobustLoose.src = "gsfElectrons::PAT"
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
process.selectedPatJets.cut = 'pt > 20. & abs(eta) < 2.4'# & emEnergyFraction>0.01 & jetID.n90Hits>1 & jetID.fHPD<0.98'
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

# -[ Add Custom Collections ]-
#
# = Jets
process.load('RecoJets.Configuration.RecoJPTJets_cff')


addJetCollection(process,cms.InputTag('ak5PFJets'),
                 'AK5', 'PF',
                 doJTA = False,
                 doBTagging = True,
                 jetCorrLabel = ('AK5','PF'),
                 doType1MET = False,
                 doL1Cleaning = False,
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID = False
                 )

addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
                 'AK5', 'JPT',
                 doJTA = True,
                 doBTagging = True,
                 jetCorrLabel = ('AK5','JPT'),
                 doType1MET = False,
                 doL1Cleaning = False,
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID = True
                 )
# Embed user data into the PAT objects
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoJPTJets_cff')
process.patJPTJetUserData = cms.EDProducer(
    "PATJPTJetUserData",
    src = cms.InputTag("patJetsAK5JPT")
)
# Insert the user data process into the PAT sequence
process.makePatJets.replace(getattr(process,"patJetsAK5JPT"),getattr(process,"patJetsAK5JPT")*process.patJPTJetUserData)
# replace JPT sample by costumized collection
process.selectedPatJetsAK5JPT.src = "patJPTJetUserData"
# Apply jet ID
process.selectedPatJets.cut = 'pt > 20. & abs(eta) < 2.4 & emEnergyFraction>0.01 & jetID.n90Hits>1 & jetID.fHPD<0.98'
#process.selectedPatJetsAK4PF.cut = 'pt > 20. & abs(eta) < 2.4 & neutralHadronEnergyFraction() < 0.99 & neutralEmEnergyFraction() < 0.99 & nConstituents() > 1 & chargedHadronEnergyFraction() > 0.0 & chargedMultiplicity() > 0.0 & chargedEmEnergyFraction() < 0.99'
# = MET
from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process)
addPfMET(process)

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
    'TopHLTFilter',
    hltTag = cms.InputTag("TriggerResults::HLT")
)    
# pat based trigger    
#process.triggerFilter = cms.EDFilter(
#    'TriggerFilter',
#    menu = cms.string("patTriggerEvent"),
#    path = cms.string("HLT_Mu9")
#)

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


process.load("Yumiceva.Top7TeV.PATNtupleMaker_cfi")
process.PATNtupleMaker.inputType = inputType
process.PATNtupleMaker.ntupleFile = outntuple

### The process path ###
process.p = cms.Path(

    process.scrapingVeto *
    process.HBHENoiseFilter *
    process.triggerFilter *

    process.gsfElectrons *    
    process.simpleSecondaryVertexHighPurBJetTags *
    process.recoJPTJets *
    process.ak5JPTJetsL2L3 *
    
    process.patDefaultSequence *
#    process.triggerFilter *
    process.PATNtupleMaker

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
#process.out.outputCommands = ['keep *']

process.out.fileName = outfile

# No output
del(process.outpath)


