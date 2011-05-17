import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.jetIDSelector_cfi import jetIDSelector
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector

PATNtupleMaker = cms.EDFilter(
        'PATNtupleMaker',

        hltTag = cms.InputTag("patTriggerEventPFlow"),
        hltList = cms.vstring(""),
        ApplyTrigger = cms.bool(True),
        StoreTrigger = cms.bool(False),
        MuonTag = cms.InputTag("selectedPatMuons"),
        ElectronTag = cms.InputTag("selectedPatElectrons"),
        PFElectronTag = cms.InputTag("selectedPatElectronsPFlow"),
        PFJetTag= cms.InputTag("goodPatJetsPFlow"),
        RhoJetTag = cms.InputTag("kt6PFJetsPFlow"),
        AreaJetTag = cms.InputTag("pfJetsPFlow"),
        PFMETTag= cms.InputTag("patMETsPFlow"),
        PVTag = cms.InputTag("offlinePrimaryVertices"),
        
        jetIdLoose = jetIDSelector.clone(),
        pfjetIdLoose = pfJetIDSelector.clone(),
        
        ntupleFile = cms.string("NtupleMaker.root"),
        inputType = cms.string("DATA"),
        Verbose = cms.bool(False)
)
