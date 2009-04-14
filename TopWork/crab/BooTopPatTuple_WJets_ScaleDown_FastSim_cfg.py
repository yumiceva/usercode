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
       '/store/mc/Winter09/WJets_ScaleDown-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V12_FastSim_v2/0001/025981FD-BB20-DE11-8CE7-00163E1124E9.root',
       '/store/mc/Winter09/WJets_ScaleDown-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V12_FastSim_v2/0001/08B9968A-0420-DE11-B0FF-001E0BC1EDA0.root',
       '/store/mc/Winter09/WJets_ScaleDown-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V12_FastSim_v2/0001/0CA42301-721F-DE11-8CC7-00163E1124D1.root',
       '/store/mc/Winter09/WJets_ScaleDown-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V12_FastSim_v2/0001/104C9C44-BD20-DE11-8E59-00163E1124CC.root',
       '/store/mc/Winter09/WJets_ScaleDown-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V12_FastSim_v2/0001/1ACB96AA-0021-DE11-9037-00163E1124DE.root',
       '/store/mc/Winter09/WJets_ScaleDown-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V12_FastSim_v2/0001/1C29E1FD-C120-DE11-B99E-00163E1124E8.root',
       '/store/mc/Winter09/WJets_ScaleDown-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V12_FastSim_v2/0001/1CACC2F4-E820-DE11-8915-00163E1124CD.root',
       '/store/mc/Winter09/WJets_ScaleDown-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V12_FastSim_v2/0001/207B3A51-ED1F-DE11-BDF3-001E0BC198E0.root',
       '/store/mc/Winter09/WJets_ScaleDown-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V12_FastSim_v2/0001/26D538B8-E11F-DE11-B0C5-001E0BC199C2.root',
       '/store/mc/Winter09/WJets_ScaleDown-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V12_FastSim_v2/0001/2EDCF01F-DA20-DE11-ADA4-00163E1124D1.root'
    )
    , duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

## define maximal number of events to loop over
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(500)
)

## configure process options
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)


## configure geometry
process.load("Configuration.StandardSequences.Geometry_cff")
## configure conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('IDEAL_V12::All')
## load magnetic field
process.load("Configuration.StandardSequences.MagneticField_cff")


#-------------------------------------------------
# patTuple configuration
#-------------------------------------------------

## std sequence for tqaf layer1
process.load("TopQuarkAnalysis.TopPairBSM.BooTopPatTuple")

## necessary fixes to run 2.2.X on 2.1.X data
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import run22XonSummer08AODSIM
run22XonSummer08AODSIM(process)

## switch from clusters to rec hits in ECAL electron isolation
#from PhysicsTools.PatAlgos.recoLayer0.electronIsolation_cff import useElectronRecHitIsolation
#useElectronRecHitIsolation(process)
## switch from clusters to rec hits in ECAL photon isolation
#from PhysicsTools.PatAlgos.recoLayer0.photonIsolation_cff import usePhotonRecHitIsolation
#usePhotonRecHitIsolation(process)



#-------------------------------------------------
# pat tuple event content; first ALL objects
# are dropped in this process; then patTuple
# content is added
#-------------------------------------------------

## define pat tuple event content
from TopQuarkAnalysis.TopObjectProducers.patTuple_EventContent_cff import *
makePatTupleEventContent(process)

##----------------------------------
## Switch jet collection to SC5
##----------------------------------
from PhysicsTools.PatAlgos.tools.jetTools import *

switchJetCollection(process, 
                    'sisCone5CaloJets',             # jet collection; must be already in the event when patLayer0 sequence is executed
                    layers       = [0,1],           # if you're not running patLayer1, set 'layers=[0]' 
                    runCleaner   = "CaloJet",       # =None if not to clean
                    doJTA        = True,            # run jet-track association & JetCharge
                    doBTagging   = True,            # run b-tagging
                    jetCorrLabel = ('SC5','Calo'), # example jet correction name; set to None for no JEC
                    doType1MET   = False             # recompute Type1 MET using these jets
                    )

# if you need to change JEC use the following
# FOR WINTER09 FASTSIM samples comment out the following line

switchJECSet(process, newName='Winter09', oldName='Summer08Redigi')


#--------------------------------------
# PRE SELECTION
#

process.selectedLayer1Electrons.cut  = cms.string('pt > 15. & abs(eta) < 2.4')
process.selectedLayer1Muons.cut      = cms.string('pt > 15. & abs(eta) < 2.4')
process.selectedLayer1Jets.cut       = cms.string('pt > 20. & abs(eta) < 2.4 & nConstituents > 0')
process.selectedLayer1METs.cut       = cms.string('et >= 0.')
#process.countLayer1Leptons.minNumber = 1
process.minLayer1Jets.minNumber      = 2
process.minLayer1Muons.minNumber     = 1


#--------------------------------------------------
# PATH

process.p = cms.Path( process.BooTopPatTuple_reduced )


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
    process.patTupleEventContent,
    verbose = cms.untracked.bool(True),
    dropMetaDataForDroppedData = cms.untracked.bool(True),                           
    fileName = cms.untracked.string('WJets_ScaleDown_madgraph_Winter09_FastSim.root'),
    dataset = cms.untracked.PSet(
            dataTier = cms.untracked.string('USER'),
            filterName = cms.untracked.string('')
                )
)

process.out.outputCommands.extend(["keep *_selectedLayer1Jets*_*_*"])
process.out.outputCommands.extend(["keep *_selectedLayer1METs*_*_*"])

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1.2.1 $'),
    annotation = cms.untracked.string('PAT tuple creation'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/TopQuarkAnalysis/TopPairBSM/test/Attic/BooTopPatTuple_FastSim_cfg.py,v $')
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
