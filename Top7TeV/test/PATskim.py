### General setup ###

import string

events= 1000
#inputType = "DATA" # choose MC/DATA
inputType = "MC"

channel = "electron" # muon/electron

#eventtype="Jun14"
eventtype="TTJets"
#eventtype="Top-s"
#eventtype="Top-t"
#eventtype="Top-tW"
#eventtype="WJets"
#eventtype="ZJets"
#eventtype="Vqq"
#eventtype="Wc"
#eventtype="QCD"

out = "%s-*-PATskim.root"%eventtype
outfile = string.replace(out,"*","PAT")
outntuple = string.replace(out,"*","Ntuple")
source = "Source.PoolSource.%s-PoolSource"%eventtype

from PhysicsTools.PatAlgos.patTemplate_cfg import *

# Send all CMSSW output to stdout
process.load("FWCore.MessageService.MessageLogger_cfi")
#process.MessageLogger.cout = process.MessageLogger.cerr
#process.MessageLogger.cerr = cms.untracked.PSet(
#    placeholder = cms.untracked.bool( True )
#)
#process.MessageLogger.cerr_stats.output = 'cout'
#process.MessageLogger.cout.FwkReport.reportEvery = 500
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.options.wantSummary = True
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.GlobalTag.globaltag = 'GR_R_38X_V13::All' # for all
if inputType=="MC":
    process.GlobalTag.globaltag = 'START38_V12::All'

### Input ###
# My typical PoolSource
#process.load(source)
#process.maxEvents.input = events
#process.source.skipEvents = 0
process.source.fileNames = cms.untracked.vstring(
#    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/6A86E9F7-EF7A-DF11-A7AF-0017A4770C34.root',
#    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/584B5E90-F37A-DF11-A8D5-0017A4770024.root',
#    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/48CE97B5-F47A-DF11-9BF4-001E0B4A0EFC.root',
#    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/40ACF3E7-EF7A-DF11-A840-0017A4770430.root',
#    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/3E6B78EA-F17A-DF11-AEF9-0017A4771030.root',
#    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/30212CEB-EF7A-DF11-9EAE-0017A4770828.root',
#    '/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/2EC3A00D-F07A-DF11-9B09-00237DA41368.root'

    # or MC files
    '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0016/6E7C4631-9D47-DF11-96CE-003048C69288.root'
#    '/store/mc/Spring10/WJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0014/84872812-1F4B-DF11-8A07-00151796C158.root'   
)
process.maxEvents.input = events

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(outntuple)
)


### PAT setup ###
process.load("PhysicsTools.PatAlgos.patSequences_cff")
# Since these are re-RECO samples, the ak5GenJets collection is missing
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePATFAQs#Running_PAT_in_3_3_X_on_a_sa_AN1
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
if inputType=="MC":
    from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
    run36xOn35xInput(process, "ak5GenJets" )
    
# switch to 8e29 trigger menu and add PAT trigger to patDefaultSequence
#from PhysicsTools.PatAlgos.tools.trigTools import *
#switchOnTrigger( process )
#from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
# 35X MC re-reco labeled newest trigger REDIGI
#process.patTriggerEvent.processName = 'HLT'
#process.patTrigger.processName = 'HLT'
# shrink the jet content
# Keep the b-tag discriminators, just not the info used to calculate them,
# such as tracks and vertices.
# need to do this for other jet collections
setTagInfos(
    process,
    "patJets",
    cms.vstring("secondaryVertexTagInfos")
)
# Running on real data, we want to turn off all MC matching
from PhysicsTools.PatAlgos.tools.coreTools import *
#removeMCMatching(process, ['All'])

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
#process.load("RecoEgamma.EgammaTools.correctedElectronsProducer_cfi")
#process.patElectrons.electronSource = "gsfElectrons::PAT"
#process.patElectrons.addElectronID = False
# If using electron ID (we don't):
#process.load('RecoEgamma.ElectronIdentification.electronIdSequence_cff')
#process.eidTight.src = "gsfElectrons::PAT"
#process.eidLoose.src = "gsfElectrons::PAT"
#process.eidRobustTight.src = "gsfElectrons::PAT"
#process.eidRobustHighEnergy.src = "gsfElectrons::PAT"
#process.eidRobustLoose.src = "gsfElectrons::PAT"
#process.patElectrons.addElectronID = False
#process.patElectrons.isoDeposits = cms.PSet()
#process.patElectrons.embedGsfTrack = False
#process.patElectrons.embedSuperCluster = False
process.patElectrons.usePV = False
# Embed user data into the PAT objects
#process.patElectronsUserData = cms.EDProducer(
#    "PATElectronUserData",
#    src = cms.InputTag("patElectrons"),
#    ecalRecHitsEBsrc = cms.InputTag("reducedEcalRecHitsEB")
#)
# Insert the user data process into the PAT sequence
#process.makePatElectrons.replace(getattr(process,"patElectrons"),getattr(process,"patElectrons")*process.patElectronsUserData)
# Tell selectedPatElectrons to use the new user collection as input
#process.selectedPatElectrons.src = "patElectronsUserData"
process.selectedPatElectrons.cut = "et>15 & abs(eta)<2.5 " #& userFloat('SwissCross')<0.95"
#if inputType=="MC" or eventtype=="Prompt_MuB" or eventtype=="Prompt_ElB":
#    process.selectedPatElectrons.cut = "et>15 & abs(eta)<2.5"

#process.countPatElectrons.maxNumber = 0
process.electronMatch.checkCharge = False

### Jets ###
# Running on Summer09 data using 7TeV JEC
# Use default patJetCorrFactors in 36X
#process.patJetCorrFactors.corrSample = 'Summer09_7TeV_ReReco332'
# get the 7 TeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Spring10")

process.selectedPatJets.cut = 'pt > 20. & abs(eta) < 2.4'# & emEnergyFraction>0.01 & jetID.n90Hits>1 & jetID.fHPD<0.98'
#process.countPatJets.minNumber = 1
process.patJets.embedGenJetMatch = False

### New SSVHP b-tag ###
# https://hypernews.cern.ch/HyperNews/CMS/get/btag/525/2.html
# for 36x MC
if inputType=="MC":
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


#addJetCollection(process,cms.InputTag('ak5PFJets'),
#                 'AK5', 'PF',
#                 doJTA = False,
#                 doBTagging = True,
#                 jetCorrLabel = ('AK5','PF'),
#                 doType1MET = False,
#                 doL1Cleaning = False,
#                 doL1Counters = False,
#                 genJetCollection=cms.InputTag("ak5GenJets"),
#                 doJetID = False
#                 )


# Embed user data into the PAT objects
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoJPTJets_cff')
#process.patJPTJetUserData = cms.EDProducer(
#    "PATJPTJetUserData",
#    src = cms.InputTag("patJetsAK5JPT")
#)
# Insert the user data process into the PAT sequence
#process.makePatJets.replace(getattr(process,"patJetsAK5JPT"),getattr(process,"patJetsAK5JPT")*process.patJPTJetUserData)
# replace JPT sample by costumized collection
#process.selectedPatJetsAK5JPT.src = "patJPTJetUserData"
# Apply jet ID
process.selectedPatJets.cut = 'pt > 20. & abs(eta) < 2.4'
#process.selectedPatJetsAK5JPT = 'pt > 20. & abs(eta) < 2.4'
#process.selectedPatJetsAK5PF = 'pt > 20. & abs(eta) < 2.4'
#process.selectedPatJetsAK4PF.cut = 'pt > 20. & abs(eta) < 2.4 & neutralHadronEnergyFraction() < 0.99 & neutralEmEnergyFraction() < 0.99 & nConstituents() > 1 & chargedHadronEnergyFraction() > 0.0 & chargedMultiplicity() > 0.0 & chargedEmEnergyFraction() < 0.99'

# needed for AOD
#process.patJets.addTagInfos = cms.bool(False)

# = MET
#from PhysicsTools.PatAlgos.tools.metTools import *
#addTcMET(process)
#addPfMET(process)

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

### Triggers ###

# muon trigger
process.triggerFilter = cms.EDFilter("MyHLTSummaryFilter",
                                     summary = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
                                     member  = cms .InputTag("HLTMu9","","HLT"),
                                     cut     = cms.string(""),
                                     minN    = cms.int32(1)
                                     )

if channel=="electron":
    elefilter = "hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter"
    triggerptcut = ""
    if eventtype == 'Sep17_135821-139459_El':
        elefilter = "hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter"
    if eventtype == 'Sep17_139779-140159_El':
        elefilter = "hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter"
    if eventtype == 'Sep17_140160-141881_El':
        elefilter = "hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter"
    if eventtype == 'Sep17_141956-143962_El':
        elefilter = "hltL1NonIsoHLTNonIsoSingleElectronEt15PixelMatchFilter"
    if eventtype == 'Sep17_144011-144114_El':
        elefilter = "hltL1NonIsoHLTNonIsoSingleElectronEt15CaloEleIdPixelMatchFilter"
    if eventtype == "Sep17_146428-146644_El":
        elefilter = "hltL1NonIsoHLTNonIsoSingleElectronEt17CaloEleIdPixelMatchFilter"
    if inputType == "MC":
        elefilter = "HLT_Ele15_SW_L1R"
        triggerptcut = "pt>17"
    process.triggerFilter = cms.EDFilter("MyHLTSummaryFilter",
                                         summary = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
                                         member  = cms .InputTag(elefilter,"","HLT"),
                                         cut     = cms.string(triggerptcut),
                                         minN    = cms.int32(1)
                                         )
# pat based trigger    
#process.triggerFilter = cms.EDFilter(
#    'TriggerFilter',
#    menu = cms.string("patTriggerEvent"),
#    path = cms.string("HLT_Mu9")
#)


# PF2PAT
#
from PhysicsTools.PatAlgos.tools.pfTools import *
# An empty postfix means that only PF2PAT is run,
# otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
# collections have standard names + postfix (e.g. patElectronPFlow)
postfix = "PFlow"
brunOnMC = False
if inputType=="MC": brunOnMC = True

usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=brunOnMC, postfix=postfix)

# turn to false when running on data
getattr(process, "patElectrons"+postfix).embedGenMatch = brunOnMC
getattr(process, "patMuons"+postfix).embedGenMatch = brunOnMC

if inputType=="DATA":
    removeMCMatching(process, ['All'])
#process.patseq.remove( process.flavorHistorySeq )
#process.patseq.remove( process.patJetPartonMatchAK5JPTPFlow )

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

# = MET
from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process)

# muon+jets ntuple
process.load("Yumiceva.Top7TeV.PATNtupleMaker_cfi")
process.PATNtupleMaker.inputType = inputType
process.PATNtupleMaker.ntupleFile = outntuple
# electron+jets ntuple
process.load("Yumiceva.Top7TeV.PATElectronNtupleMaker_cfi")
process.PATElectronNtupleMaker.inputType = inputType
process.PATElectronNtupleMaker.ntupleFile = outntuple

### GEN decay channel, for tT MC events only ###
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")
process.load("PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi")
process.cFlavorHistoryProducer.matchedSrc = "ak5GenJets"
process.bFlavorHistoryProducer.matchedSrc = "ak5GenJets"
    
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

### The process path ###
process.p = cms.Path(

    process.scrapingVeto *
    process.HBHENoiseFilter *
    process.triggerFilter *

    #process.gsfElectrons *    
    process.simpleSecondaryVertexHighPurBJetTags *
    process.recoJPTJets *
#    process.ak5JPTJetsL2L3 *

    getattr(process,"patPF2PATSequence"+postfix)*
    
    process.patDefaultSequence *
    
    process.makeGenEvt *
    process.flavorHistorySeq *
    process.prunedGenParticles *
    
#    process.triggerFilter *
    process.PATNtupleMaker *
    process.PATElectronNtupleMaker
)

if channel == "muon":
    process.p.remove( process.PATElectronNtupleMaker )
else:
    process.p.remove( process.PATNtupleMaker )
            
if inputType=="MC":
    process.p.remove( process.scrapingVeto )
    process.p.remove( process.HBHENoiseFilter )
    if eventtype != "TTJets":
        process.p.remove( process.makeGenEvt )
    if eventtype != "WJets" and eventtype!="ZJets" and eventtype!="Vqq" and eventtype!="Wc":
        process.p.remove( process.flavorHistorySeq )
else:
    process.p.remove( process.simpleSecondaryVertexHighPurBJetTags )
    process.p.remove( process.makeGenEvt )
    process.p.remove( process.flavorHistorySeq )
    process.p.remove( process.prunedGenParticles )

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


