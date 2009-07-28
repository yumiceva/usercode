import FWCore.ParameterSet.Config as cms

#-------------------------------------------------
# test cfg file for tqaflayer1 production from
# fullsim
#-------------------------------------------------
process = cms.Process("BooTopPat")

## add message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

#-------------------------------------------------
# process configuration
#-------------------------------------------------

## define input
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0009/9885EAE5-876D-DE11-A0EE-001A9243D640.root',
       '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0006/DCAE7AE1-2A6D-DE11-9A56-001A9254460C.root',
       '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0006/D2FE0B42-2F6D-DE11-9FF3-001A9227D383.root',
       '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0006/C2F0B1DF-2C6D-DE11-8849-001A9243D62A.root',
       '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0006/9CC08AE1-2A6D-DE11-96C9-001E8CCCE140.root',
       '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0005/E893E3BA-246D-DE11-9C26-001A9227D32D.root',
       '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0005/E04DD0BC-246D-DE11-A774-001E8CCCE148.root',
       '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0005/CC6C4DC1-246D-DE11-8DDB-00304858A675.root'
    )
    , duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

## define maximal number of events to loop over
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

## configure process options
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)


## configure geometry
process.load("Configuration.StandardSequences.Geometry_cff")
## configure conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('MC_31X_V2::All')
## load magnetic field
process.load("Configuration.StandardSequences.MagneticField_cff")


#-------------------------------------------------
# patTuple configuration
#-------------------------------------------------

## std sequence for tqaf layer1
process.load("TopQuarkAnalysis.TopPairBSM.BooTopPatTuple")


#-------------------------------------------------
# pat tuple event content; first ALL objects
# are dropped in this process; then patTuple
# content is added
#-------------------------------------------------

## define pat tuple event content
from PhysicsTools.PatAlgos.patEventContent_cff import *

##----------------------------------
## Switch jet collection to SC5
##----------------------------------
from PhysicsTools.PatAlgos.tools.jetTools import *

switchJetCollection(process, 
                    cms.InputTag('sisCone5CaloJets'),             # jet collection; must be already in the event when patLayer0 sequence is executed
                    doJTA        = True,            # run jet-track association & JetCharge
                    doBTagging   = True,            # run b-tagging
                    jetCorrLabel = ('SC5','Calo'), # example jet correction name; set to None for no JEC
                    doType1MET   = False,             # recompute Type1 MET using these jets
		    genJetCollection=cms.InputTag("sisCone5GenJets")
                    )


#--------------------------------------
# PRE SELECTION
#

process.selectedLayer1Electrons.cut  = cms.string('pt > 15. & abs(eta) < 2.4')
process.selectedLayer1Muons.cut      = cms.string('pt > 15. & abs(eta) < 2.4')
process.selectedLayer1Jets.cut       = cms.string('pt > 20. & abs(eta) < 2.4 & nConstituents > 0')
#process.selectedLayer1METs.cut               = cms.string('et >= 0.') #no met selector at the moment GYJ 2009/07/28
#process.countLayer1Leptons.minNumber = 1
process.countLayer1Jets.minNumber      = 1
process.countLayer1Muons.minNumber     = 1

#-----------------------------------------
# Add JPT collection

process.load("TopQuarkAnalysis.TopPairBSM.ZPT_cff")
process.ZSPJetCorJetIcone5.src = cms.InputTag("sisCone5CaloJets")

addJetCollection(process,
                 cms.InputTag('JetPlusTrackZSPCorJetIcone5'),
                 'JPT',
                 doJTA=True,
                 doBTagging=True,
                 jetCorrLabel=None,
                 doType1MET=False,
                 doL1Counters=False,
                 genJetCollection=cms.InputTag("sisCone5GenJets")
		 )


#--------------------------------------------------
# PATH

process.p = cms.Path(process.recoJPTJets+
                     process.BooTopPatTuple)


#-------------------------------------------------
# process output; first the event selection is
# defined: only those events that have passed the
# full production path are selected and written
# to file; the event content has been defined
# above
#-------------------------------------------------

## define event selection
process.EventSelection = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p')
    )
)

## configure output module
process.out = cms.OutputModule("PoolOutputModule",
    process.EventSelection,
    verbose = cms.untracked.bool(True),
    outputCommands = cms.untracked.vstring('drop *'),
    dropMetaDataForDroppedData = cms.untracked.bool(True),                           
    fileName = cms.untracked.string('TauolaTTbar_311-v1_Summer09.root'),
    dataset = cms.untracked.PSet(
            dataTier = cms.untracked.string('USER'),
            filterName = cms.untracked.string('')
                )
)
#process.out.outputCommands += patEventContent
process.out.outputCommands += patTriggerEventContent
process.out.outputCommands += patExtraAodEventContent
process.out.outputCommands += patEventContentNoLayer1Cleaning

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.4 $'),
    annotation = cms.untracked.string('PAT tuple creation'),
    name = cms.untracked.string('$Source: /local/projects/CMSSW/rep/UserCode/Yumiceva/TopWork/crab/BooTopPatTuple_TTJets_cfg.py,v $')
)



#-------------------------------------------------
# output paths; in order not to write the
# persistent output to file comment the output
# path
#-------------------------------------------------

#process.dump=cms.EDAnalyzer('EventContentAnalyzer')
#process.px=cms.Path(process.dump)

## output
process.outpath = cms.EndPath(process.out)
