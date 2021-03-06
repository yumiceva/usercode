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
process.GlobalTag.globaltag = cms.string('IDEAL_V11::All')
## load magnetic field
process.load("Configuration.StandardSequences.MagneticField_cff")

# load sequences
process.load("TopQuarkAnalysis.TopPairBSM.TopAnalysis_sequences")

# decaychain for madgraph samples
#from TopQuarkAnalysis.TopEventProducers.producers.TopDecaySubset_cfi import *
#process.decaySubset.genType = cms.uint32(1)

# setup path
process.p = cms.Path( process.TopAnalysisMuFilter ) # with muonic generator filter
#process.p = cms.Path( process.TopAnalysisNoMuFilter ) # with generator filter on all but muonci decays
#process.p = cms.Path( process.TopAnalysis ) # no generator filter at all

# change defaults
#process.TopAnalyzer.muonCuts.MinPt = cms.double(30.)
#process.TopAnalyzer.jetCuts.ApplyAsymmetricCuts = cms.bool(True)
#process.TopAnalyzer.METCuts.Recalculate  = cms.bool(True)
#process.TopAnalyzer.jetSource      = cms.InputTag('selectedLayer1JetsJPT')
#process.TopAnalyzer.UsebTagging  = cms.bool(True)
#process.TopAnalyzer.IsMCTop      = cms.bool( False )
process.TopAnalyzer.rootFilename = 'TopAnalysis_TTJets-madgraph_Fall08_Mu_0001.root'
process.BooTopHLTFilter.HLTPaths = [''] # do not filter
#process.TopAnalyzer.writeAscii = cms.bool( True )
process.TopAnalyzer.asciiFilename = cms.string('/uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/CMSSW_2_2_6/LowMass_analysis_IPsig3_Iso95//TopAnalysis_TTJets-madgraph_Fall08_all_0001.root.txt')

# source
#from TopQuarkAnalysis.TopPairBSM.L1_WJets_madgraph_Fall08 import source

#process.source = source
#process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1) )



    

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring(
"",
"",
"",
"dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_6/TTJets-madgraph_Fall08//TTJets_madgraph_Fall08_1.root",
"dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_6/TTJets-madgraph_Fall08//TTJets_madgraph_Fall08_10.root",
"dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_6/TTJets-madgraph_Fall08//TTJets_madgraph_Fall08_100.root"
)
, duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)
