import FWCore.ParameterSet.Config as cms

NtupleMaker = cms.EDFilter(
        'NtupleMaker',

        hltTag = cms.InputTag("TriggerResults::REDIGI"),

        inputType = cms.string("DATA"),

        JetIDParams = cms.PSet(
    useRecHits = cms.bool(True),
    hbheRecHitsColl = cms.InputTag("hbhereco"),
    hoRecHitsColl   = cms.InputTag("horeco"),
    hfRecHitsColl   = cms.InputTag("hfreco"),
    ebRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEB"),
    eeRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEE")
    )
)
