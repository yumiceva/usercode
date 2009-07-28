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
       '/store/mc/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0012/FE3DD746-1A6F-DE11-82E9-00304854D980.root',
       '/store/mc/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0012/F662D080-9D6E-DE11-9A01-00304865C2CA.root',
       '/store/mc/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0012/F4503BB4-F46E-DE11-AF9A-001E682F2108.root',
       '/store/mc/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0012/F205EDAA-586E-DE11-BE1C-00304854D980.root',
       '/store/mc/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0012/EAB3E19E-5B6E-DE11-BC24-0002B3E92671.root',
       '/store/mc/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0012/EA3FB7CB-BD6E-DE11-8ED2-00304865C29A.root',
       '/store/mc/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0012/E298BB39-B96E-DE11-9385-00144FCA22C8.root',
       '/store/mc/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0012/E0F29673-A16E-DE11-A78C-00144FCAACE0.root',
       '/store/mc/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0012/DC832CEF-996E-DE11-97DE-00304865C330.root',
       '/store/mc/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V2_preproduction_311-v1/0012/D6D1FE25-806E-DE11-B337-00304865C254.root'
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
    fileName = cms.untracked.string('InclusiveMu15_311-v1_Summer09.root'),
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
