import FWCore.ParameterSet.Config as cms

process = cms.Process("Ntuple")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
    '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0016/6E7C4631-9D47-DF11-96CE-003048C69288.root'
    )
                            )

process.load("Yumiceva.Top7TeV.NtupleMaker_cfi")
process.NtupleMaker.inputType = "MC"

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
from JetMETCorrections.Type1MET.MetType1Corrections_cff import metJESCorAK5CaloJet
process.metMuonJESCorAK5 = metJESCorAK5CaloJet.clone()
process.metMuonJESCorAK5.inputUncorMetLabel = "corMetGlobalMuons"
process.metCorSequence = cms.Sequence(process.metMuonJESCorAK5)

process.p = cms.Path(process.ak5CaloJetsL2L3 * process.metCorSequence * process.NtupleMaker)
