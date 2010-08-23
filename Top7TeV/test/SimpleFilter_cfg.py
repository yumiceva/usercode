import FWCore.ParameterSet.Config as cms

process = cms.Process("Simple")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
    '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0016/6E7C4631-9D47-DF11-96CE-003048C69288.root'
    )
                            )

import xaa
process.source = xaa.source

process.SimpleFilter = cms.EDFilter(
    'SimpleFilter',
    JetIDParams = cms.PSet(
    useRecHits = cms.bool(True),
    hbheRecHitsColl = cms.InputTag("hbhereco"),
    hoRecHitsColl   = cms.InputTag("horeco"),
    hfRecHitsColl   = cms.InputTag("hfreco"),
    ebRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEB"),
    eeRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEE")
    )
)



process.Out = cms.OutputModule("PoolOutputModule",
                               SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('p')
    ),
    fileName = cms.untracked.string('RECO_xaa.root')
)


process.p = cms.Path(
    #process.ak5CaloJetsL2L3 * process.metCorSequence *
    process.SimpleFilter)

process.outpath = cms.EndPath(process.Out)
