import FWCore.ParameterSet.Config as cms

#-------------------------------------------------
# fullsim
#-------------------------------------------------
process = cms.Process("LowMass")

## add message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

## configure process options
process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True)
        )

## configure geometry
process.load("Configuration.StandardSequences.Geometry_cff")
## configure conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('IDEAL_V9::All')
## load magnetic field
process.load("Configuration.StandardSequences.MagneticField_cff")

# load sequences
process.load("TopQuarkAnalysis.TopPairBSM.TopAnalysis_sequences")


process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.printList = cms.EDFilter("ParticleListDrawer",
                                 src = cms.InputTag("genParticles"),
                                 maxEventsToPrint = cms.untracked.int32(-1)
                                 )

process.printTree = cms.EDFilter("ParticleTreeDrawer",
     src = cms.InputTag("genParticles"),
     printP4 = cms.untracked.bool(False),
     printPtEtaPhi = cms.untracked.bool(False),
     printVertex = cms.untracked.bool(True),
     printStatus = cms.untracked.bool(False),
     printIndex = cms.untracked.bool(False),
     status = cms.untracked.vint32(1, 2, 3)
 )

from TopQuarkAnalysis.TopEventProducers.producers.TopDecaySubset_cfi import *
process.decaySubset.genType = cms.uint32(1)


# setup path
#process.p = cms.Path( process.TopAnalysisMuFilter ) # with muonic generator filter
process.p = cms.Path(  process.TopAnalysisNoMuFilter ) # with generator filter on all but muonic decays
#process.p = cms.Path( process.TopAnalysis ) # no generator filter at all


# change defaults
process.TopAnalyzer.rootFilename = '/uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_1/LowMass_analysis//TopAnalysis_TTJets-madgraph_Fall08_other_Test0010.root'
process.BooTopHLTFilter.HLTPaths = [''] # do not filter

# source
#from TopQuarkAnalysis.TopPairBSM.L1_TTJets_madgraph_Summer08 import source

#process.source = source
#process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10) )



    

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(2) )

process.source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring(
"dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_1_v3/TTJets-madgraph_Fall08/TTJets_madgraph_Fall08_4.root",
"dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_1_v3/TTJets-madgraph_Fall08/TTJets_madgraph_Fall08_40.root",
"dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_1_v3/TTJets-madgraph_Fall08/TTJets_madgraph_Fall08_41.root",
"dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_1_v3/TTJets-madgraph_Fall08/TTJets_madgraph_Fall08_42.root",
"dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_1_v3/TTJets-madgraph_Fall08/TTJets_madgraph_Fall08_43.root"
)
, duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)
