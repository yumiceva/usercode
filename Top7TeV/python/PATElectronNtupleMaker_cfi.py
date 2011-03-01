import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.jetIDSelector_cfi import jetIDSelector
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector


PATElectronNtupleMaker = cms.EDFilter(
        'PATElectronNtupleMaker',

        hltTag = cms.InputTag("TriggerResults::REDIGI"),
        MuonTag = cms.InputTag("selectedPatMuons"),
        ElectronTag = cms.InputTag("selectedPatElectrons"),
        PFElectronTag = cms.InputTag("patElectronsPFlowLoose"),
        caloJetTag= cms.InputTag("selectedPatJets"),
        JPTJetTag= cms.InputTag("selectedPatJetsAK5JPT"),
        PFJetTag= cms.InputTag("selectedPatJetsAK5PF"),
        PFlowJetTag= cms.InputTag("selectedPatJetsPFlow"),
        caloMETTag= cms.InputTag("patMETs"),
        tcMETTag= cms.InputTag("patMETsTC"),
        PFMETTag= cms.InputTag("patMETsPF"),
        PFlowMETTag = cms.InputTag("patMETsPFlow"),
        
        jetIdLoose = jetIDSelector.clone(),
        pfjetIdLoose = pfJetIDSelector.clone(),
        
        ntupleFile = cms.string("NtupleMaker.root"),
        inputType = cms.string("DATA")

)
