### General setup ###

import string

events= 1000
inputType = "DATA" # choose MC/DATA
#inputType = "MC"

channel = "muon" # muon/electron

eventtype="Run2010A_Mu9"
#eventtype="TTJets"
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
process.GlobalTag.globaltag = 'GR_R_38X_V15::All' # for all
if inputType=="MC":
    process.GlobalTag.globaltag = 'START38_V14::All' # MC_38Y_V14, START38_V14, DESIGN_38_V14

### Input ###
# My typical PoolSource
#process.load(source)
#process.maxEvents.input = events
#process.source.skipEvents = 0
process.source.fileNames = cms.untracked.vstring(

    # data files
    #'/store/data/Run2010B/Mu/RECO/Nov4ReReco_v1/0008/80ED4E8B-D2EA-DF11-90CF-90E6BA0D09EC.root'
    '/store/data/Run2010B/Mu/AOD/Nov4ReReco_v1/0006/067DC752-D2EA-DF11-90D6-001EC9D87221.root',
    '/store/data/Run2010B/Mu/AOD/Nov4ReReco_v1/0000/00A75A80-93EA-DF11-A874-E0CB4E55365F.root',
    '/store/data/Run2010B/Mu/AOD/Nov4ReReco_v1/0000/00A0D70F-11EA-DF11-A94C-485B39800BD5.root',
    '/store/data/Run2010B/Mu/AOD/Nov4ReReco_v1/0000/009C2D2D-42EA-DF11-9E21-E0CB4E553648.root',
    '/store/data/Run2010B/Mu/AOD/Nov4ReReco_v1/0000/0037C237-F4E9-DF11-A5FC-0030487C6A32.root',
    '/store/data/Run2010B/Mu/AOD/Nov4ReReco_v1/0000/00309820-0FEA-DF11-AE59-E0CB4E1A118E.root'
    
    # or MC files
    #'/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0006/06B4F65D-9EE4-DF11-8B5F-003048D4767A.root',
    #'/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0006/0690C90B-8DE4-DF11-8FF4-0015172C0934.root',
    #'/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0006/04C46047-23E5-DF11-85EC-00221952AA1F.root',
    #'/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0006/029459F3-B0E4-DF11-B45A-003048CDCC2A.root',
    #'/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0006/00BD2378-D6E4-DF11-825B-003048C99F9C.root'
    # Wjets
    #'/store/mc/Fall10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v1/0043/F22AD025-46FC-DF11-8FE0-0002C90A3426.root',
    #'/store/mc/Fall10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v1/0043/E66C2612-3CFC-DF11-8134-0002C90A3414.root',
    #'/store/mc/Fall10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v1/0043/E2E2716E-C2FC-DF11-A60F-0002C90B7428.root'
    # ttbar AODSIM
    #'/store/mc/Fall10/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0014/BAF995DB-85E1-DF11-B1DF-00A0D1EEE660.root',
    #'/store/mc/Fall10/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0014/A44094D0-89E1-DF11-AFD8-00A0D1EEA838.root',
    #'/store/mc/Fall10/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0014/32374770-8AE1-DF11-A270-00A0D1EEE5CC.root',
    #'/store/mc/Fall10/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0014/2CC6A3A8-8AE1-DF11-A9DF-001D0967C649.root',
    #'/store/mc/Fall10/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0014/04C894E5-32E1-DF11-939F-001EC9ED4A0A.root'
    # QCD
    #'/store/mc/Fall10/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0000/22E2557D-33C8-DF11-980B-001A92971AEC.root',
    #'/store/mc/Fall10/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0000/1AA9BF44-39C8-DF11-8C91-00261894391D.root',
    #'/store/mc/Fall10/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0000/1A84CD4D-38C8-DF11-8A67-0026189438F7.root',
    #'/store/mc/Fall10/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0000/14A46E6C-54C8-DF11-A3C7-002618943958.root',
    #'/store/mc/Fall10/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0000/14137D23-48C8-DF11-9CBA-002618943867.root'
)
process.maxEvents.input = events

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(outntuple)
)


### PAT setup ###
process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
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
#switchJECSet( process, "Spring10")

process.selectedPatJets.cut = 'pt > 20. & abs(eta) < 2.4'# & emEnergyFraction>0.01 & jetID.n90Hits>1 & jetID.fHPD<0.98'
#process.countPatJets.minNumber = 1
process.patJets.embedGenJetMatch = False

### New SSVHP b-tag ###
# https://hypernews.cern.ch/HyperNews/CMS/get/btag/525/2.html
# for 36x MC
#if inputType=="MC":
#    process.load("RecoBTag.SecondaryVertex.simpleSecondaryVertex3TrkES_cfi")
#    process.load("RecoBTag.SecondaryVertex.simpleSecondaryVertexHighPurBJetTags_cfi")
#    process.patJets.discriminatorSources += [cms.InputTag("simpleSecondaryVertexHighPurBJetTags")]

### Photons ###
process.patPhotons.isoDeposits = cms.PSet()
process.patPhotons.embedSuperCluster = False

### Taus ###
process.patTaus.isoDeposits = cms.PSet()

# -[ Add Custom Collections ]-
#
# = Jets
#process.load('RecoJets.Configuration.RecoJPTJets_cff')
# JPT corrections
jptcorrections = ""

if inputType == "MC":

    switchJetCollection( process,
                         jetCollection=cms.InputTag('ak5CaloJets'),
                         jetCorrLabel=('AK5Calo', ['L2Relative', 'L3Absolute']))
    jptcorrections = ['L1JPTOffset','L2Relative', 'L3Absolute']
    
else:
    switchJetCollection( process,
                         jetCollection=cms.InputTag('ak5CaloJets'),
                         jetCorrLabel=('AK5Calo', ['L2Relative', 'L3Absolute', 'L2L3Residual']))
    jptcorrections = ['L1JPTOffset','L2Relative', 'L3Absolute','L2L3Residual']


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
triggerprocess = "HLT"
if inputType == "MC" and eventtype=="QCD":
    triggerprocess = "REDIGI38X" #
#if eventtype.find("_scale")!=-1 or eventtype.find("_matching")!=-1\
#       or eventtype.find("ISRFSR")!=-1 or eventtype.find("_PU")!=-1:
#    triggerprocess = "HLT"

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

if channel=="electron":
    elefilter = "hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter"
    triggercut = ""
    if eventtype == 'TrigB':
        elefilter = "hltL1NonIsoHLTNonIsoSingleElectronEt15PixelMatchFilter"
    if eventtype == 'TrigC' or eventtype == 'PromptRecoTrigC':
        elefilter = "hltL1NonIsoHLTNonIsoSingleElectronEt15CaloEleIdPixelMatchFilter"
    if eventtype == 'TrigD':
        elefilter = "hltL1NonIsoHLTNonIsoSingleElectronEt17CaloEleIdPixelMatchFilter"
    if eventtype == 'TrigE':
        elefilter = "hltL1NonIsoHLTNonIsoSingleElectronEt17TightEleIdDphiFilter"
    if eventtype == 'TrigF':
        elefilter = "hltL1NonIsoHLTNonIsoSingleElectronEt22TighterEleIdDphiFilter"
                
    # the trigger filter will anyways be removed for MC     
    if inputType == "MC":
        elefilter = "hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter"
    
    process.triggerFilter = cms.EDFilter("MyHLTSummaryFilter",
                                         summary = cms.InputTag("hltTriggerSummaryAOD","",triggerprocess),
                                         member  = cms .InputTag(elefilter,"",triggerprocess),
                                         cut     = cms.string(triggercut),
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

postfixLoose = "PFlowLoose"
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=brunOnMC, postfix=postfixLoose)
getattr(process, "patElectrons"+postfixLoose).embedGenMatch = brunOnMC
getattr(process, "patMuons"+postfixLoose).embedGenMatch = brunOnMC
process.patElectronsPFlowLoose.pfElectronSource = 'pfAllElectronsPFlowLoose'
process.patElectronsPFlowLoose.isoDeposits = cms.PSet()
process.patElectronsPFlowLoose.isolationValues = cms.PSet()
    
# turn to false when running on data
getattr(process, "patElectrons"+postfix).embedGenMatch = brunOnMC
getattr(process, "patMuons"+postfix).embedGenMatch = brunOnMC

## ---
## PAT trigger matching
## --
process.muonTriggerMatchHLTMuons = cms.EDProducer(
        "PATTriggerMatcherDRLessByR"
            , src     = cms.InputTag( 'cleanPatMuons' )
            , matched = cms.InputTag( 'patTrigger' )
            , andOr          = cms.bool( False )
            , filterIdsEnum  = cms.vstring( 'TriggerMuon' )
            , filterIds      = cms.vint32( 0 )
            , filterLabels   = cms.vstring( '*' )
            , pathNames      = cms.vstring( patTriggerName )
            , collectionTags = cms.vstring( '*' )
            , maxDPtRel   = cms.double( 0.5 ) # no effect here
            , maxDeltaR   = cms.double( 0.5 )
            , maxDeltaEta = cms.double( 0.2 ) # no effect here
            , resolveAmbiguities    = cms.bool( True )
            , resolveByMatchQuality = cms.bool( True )
        )


## --
## Switch on PAT trigger
## --
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process ) # This is optional and can be omitted.
switchOnTriggerMatching( process, triggerMatchers = [ 'muonTriggerMatchHLTMuons' ] )
if inputType=="MC" and eventtype=="QCD":
    process.patTrigger.processName = "REDIGI38X"
    process.patTriggerEvent.processName = "REDIGI38X"
    
if inputType=="DATA":
    removeMCMatching(process, ['All'])
#process.patseq.remove( process.flavorHistorySeq )
#process.patseq.remove( process.patJetPartonMatchAK5JPTPFlow )

addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
                 'AK5', 'JPT',
                 doJTA = True,
                 doBTagging = True,
                 jetCorrLabel = ('AK5JPT', jptcorrections),                                 
                 doType1MET = False,
                 doL1Cleaning = False,
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID = True,
                 jetIdLabel = "ak5"
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
#    process.simpleSecondaryVertexHighPurBJetTags *
#    process.recoJPTJets *
#    process.ak5JPTJetsL2L3 *

    getattr(process,"patPF2PATSequence"+postfix)*
    getattr(process,"patPF2PATSequencePFlowLoose")*
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
    process.p.remove( process.patPF2PATSequencePFlowLoose )
else:
    process.p.remove( process.PATNtupleMaker )
            
if inputType=="MC":
    process.p.remove( process.scrapingVeto )
    process.p.remove( process.HBHENoiseFilter )
    if channel == "electron":
        process.p.remove( process.triggerFilter )
    if eventtype != "TTJets":
        process.p.remove( process.makeGenEvt )
    if eventtype != "WJets" and eventtype!="ZJets" and eventtype!="Vqq" and eventtype!="Wc":
        process.p.remove( process.flavorHistorySeq )
        
else:
    #process.p.remove( process.simpleSecondaryVertexHighPurBJetTags )
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


