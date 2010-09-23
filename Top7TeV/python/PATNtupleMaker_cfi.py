import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.jetIDSelector_cfi import jetIDSelector
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector

PATNtupleMaker = cms.EDFilter(
        'PATNtupleMaker',

        hltTag = cms.InputTag("TriggerResults::REDIGI"),
        MuonTag = cms.InputTag("selectedPatMuons"),
        ElectronTag = cms.InputTag("selectedPatElectrons"),
        caloJetTag= cms.InputTag("selectedPatJets"),
        JPTJetTag= cms.InputTag("selectedPatJetsAK5JPT"),
        PFJetTag= cms.InputTag("selectedPatJetsPFlow"),
        caloMETTag= cms.InputTag("patMETs"),
        tcMETTag= cms.InputTag("patMETsTC"),
        PFMETTag= cms.InputTag("patMETsPFlow"),
        
        jetIdLoose = jetIDSelector.clone(),
        pfjetIdLoose = pfJetIDSelector.clone(),
        
        ntupleFile = cms.string("NtupleMaker.root"),
        inputType = cms.string("DATA")

)
