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

# decaychain for madgraph samples
#from TopQuarkAnalysis.TopEventProducers.producers.TopDecaySubset_cfi import *
#process.decaySubset.genType = cms.uint32(1)

# setup path
#process.p = cms.Path( process.TopAnalysisMuFilter ) # with muonic generator filter
#process.p = cms.Path( process.TopAnalysisNoMuFilter ) # with generator filter on all but muonci decays
process.p = cms.Path( process.TopAnalysis ) # no generator filter at all

# change defaults
process.TopAnalyzer.IsMCTop      = cms.bool( False )
process.TopAnalyzer.rootFilename = '{OUTPUT_FILENAME}'
process.BooTopHLTFilter.HLTPaths = [''] # do not filter
#process.TopAnalyzer.writeAscii = cms.bool( True )
process.TopAnalyzer.asciiFilename = cms.string('{OUTPUT_FILENAME}.txt')

# source
#from TopQuarkAnalysis.TopPairBSM.L1_WJets_madgraph_Fall08 import source

#process.source = source
#process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1) )

