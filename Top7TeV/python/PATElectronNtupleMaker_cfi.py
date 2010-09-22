import FWCore.ParameterSet.Config as cms

PATElectronNtupleMaker = cms.EDFilter(
        'PATElectronNtupleMaker',

        hltTag = cms.InputTag("TriggerResults::REDIGI"),
        MuonTag = cms.InputTag("selectedPatMuons"),
        ElectronTag = cms.InputTag("selectedPatElectrons"),
        JetTag= cms.InputTag("selectedPatJets"),
        METTag= cms.InputTag("patMETs"),

        ntupleFile = cms.string("NtupleMaker.root"),
        inputType = cms.string("DATA")

)
