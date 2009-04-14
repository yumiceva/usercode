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
       '/store/mc/Winter09/Wjets-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/0085D4BD-0FD1-DD11-8774-00163691DBB2.root',
       '/store/mc/Winter09/Wjets-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/00A7840D-DFD0-DD11-9B8F-001B243DEF3F.root',
       '/store/mc/Winter09/Wjets-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/00BBE591-08D1-DD11-AB38-0016368E0CF0.root',
       '/store/mc/Winter09/Wjets-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/00FB9BDB-B8D1-DD11-8745-001EC9AAA350.root',
       '/store/mc/Winter09/Wjets-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/0200020B-FAD0-DD11-90FA-001EC9AA9E0A.root',
       '/store/mc/Winter09/Wjets-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/021688F9-FBD0-DD11-831F-0016368E0A9C.root',
       '/store/mc/Winter09/Wjets-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/0234DF1D-A8D1-DD11-BE8C-00304865C2C0.root',
       '/store/mc/Winter09/Wjets-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/0276F6BA-DFD0-DD11-B74C-00163691D7C2.root',
       '/store/mc/Winter09/Wjets-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/02B57010-EAD0-DD11-AAE0-00163691D762.root',
       '/store/mc/Winter09/Wjets-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/02C63B2B-F3D0-DD11-8F39-001EC9AA93B0.root',
       '/store/mc/Winter09/Wjets-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/0475E6ED-E8D0-DD11-B213-00163691D6C2.root',
       '/store/mc/Winter09/Wjets-madgraph/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/06ACA37A-EDD0-DD11-B9D4-00163691D6C2.root'
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
process.GlobalTag.globaltag = cms.string('IDEAL_V11::All')
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
    fileName = cms.untracked.string('Wjets_madgraph_Winter09_FastSim.root'),
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
