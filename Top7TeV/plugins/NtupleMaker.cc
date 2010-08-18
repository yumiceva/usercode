// -*- C++ -*-
//
// Package:    NtupleMaker
// Class:      NtupleMaker
// 
/**\class NtupleMaker NtupleMaker.cc Exercise/NtupleMaker/src/NtupleMaker.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  "Jian Wang"
//        Modified:  Samvel Khalatian
//         Created:  Fri Jun 11 12:14:21 CDT 2010
// $Id: NtupleMaker.cc,v 1.1 2010/08/17 15:22:04 samvel Exp $
//
//


// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "RecoJets/JetAlgorithms/interface/JetIDHelper.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"

#include "Yumiceva/Top7TeV/interface/NtupleMaker.h"

using namespace std;
using namespace edm;
using namespace reco;

NtupleMaker::NtupleMaker(const edm::ParameterSet& iConfig):
  hltTag_(iConfig.getParameter< InputTag >("hltTag"))
{
    //now do what ever initialization is needed
    jetID = new helper::JetIDHelper(iConfig.getParameter<ParameterSet>("JetIDParams"));
    _isDataInput = "DATA" == iConfig.getParameter<std::string>("inputType");

    cout << "[NtupleMaker] Using " << (_isDataInput ? "DATA" : "MC")
        << " input" << endl;

    _ntuple = new TopEventNtuple();

}


NtupleMaker::~NtupleMaker()
{
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called to for each event  ------------
bool
NtupleMaker::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  bool pass_event = false;
   // reset ntuple containers
   _ntuple->Reset();

    // -[ Trigger ]-
    Handle<TriggerResults> hlt;
    iEvent.getByLabel(hltTag_,hlt);
    const TriggerNames & hltNames_ = iEvent.triggerNames(*hlt);

    // -[ Muons ]-
    Handle<MuonCollection> muons;
    iEvent.getByLabel("muons",muons);

    // -[ Jets ]-
    Handle<CaloJetCollection> jets;
    iEvent.getByLabel("ak5CaloJetsL2L3",jets);

    // -[ Primary Vertices ]-
    Handle<vector<reco::Vertex> > pvtx;
    iEvent.getByLabel("offlinePrimaryVertices",pvtx);

    // -[ Beam Spot ]-
    BeamSpot beamSpot;
    Handle<BeamSpot> beamSpotHandle;
    iEvent.getByLabel("offlineBeamSpot", beamSpotHandle);
    beamSpot = *beamSpotHandle;
    math::XYZPoint point(beamSpot.x0(),beamSpot.y0(), beamSpot.z0());

    // -[ Electrons ]-
    Handle<GsfElectronCollection> electrons;
    iEvent.getByLabel("gsfElectrons",electrons);

    // -[ MET ]-
    const CaloMET *cmet;
    Handle<CaloMETCollection> cmetHandle;
    iEvent.getByLabel("metMuonJESCorAK5", cmetHandle);
    const CaloMETCollection *cmetCol = cmetHandle.product();
    cmet = &(cmetCol->front());

    _cutflow->Fill(0);

    bool hlt_mu_ = false; 
    for(size_t itrig = 0; itrig != hlt->size(); itrig++){
        string hltName = hltNames_.triggerName(itrig);
        if(hltName == "HLT_Mu9"&&hlt->accept(itrig))
        {
            hlt_mu_ = true;
            break;
        }
    }
    if(!hlt_mu_)
        return false;

    _cutflow->Fill(1);

    if(pvtx->size()<1)
        return false;
        
    //_npvs = pvtx->size();

    Vertex const & pv = pvtx->at(0);
    if(!(!pv.isFake()
        &&pv.ndof()>4
        &&fabs(pv.z())< _isDataInput ? 24 : 15.
        &&fabs(pv.position().Rho())<2.0))
        return false;

    _cutflow->Fill(2);

    _ntuple->event = iEvent.id().event();
    _ntuple->run   = iEvent.id().run();
    _ntuple->lumi  = iEvent.id().luminosityBlock();


    //_pv_coord[0] = pv.x();
    //_pv_coord[1] = pv.y();
    //_pv_coord[2] = pv.z();

    math::XYZPoint primaryVertex(pv.x(), pv.y(), pv.z());

    size_t n_loose=0;
    size_t n_tight=0;
    size_t n_muon=0;
    size_t n_electron=0;

    for(MuonCollection::const_iterator mu = muons->begin(); mu != muons->end();  ++mu) {
        
      // relative isolation
       double reliso = (mu->isolationR03().hadEt+mu->isolationR03().emEt+mu->isolationR03().sumPt)/mu->pt(); //

       // calculate deltaR
       double DeltaR = 3.;
       for(CaloJetCollection::const_iterator jet = jets->begin(); jet != jets->end(); ++jet){
          jetID->calculate(iEvent, *jet);
          if (jet->pt()>30.
              &&abs(jet->eta())<2.4
              &&jet->emEnergyFraction()>0.01
              &&jetID->n90Hits()>1
              &&jetID->fHPD()<0.98)
          {
             double dr = deltaR(mu->eta(), mu->phi(), jet->eta(), jet->phi());
             if(dr<DeltaR)
                DeltaR=dr;
          }
       }


       // Loose muons
       if(
          mu->isGlobalMuon()
          &&abs(mu->eta())<2.5
          &&mu->pt()>10.)
       {
        
          // Loose Isolated muons
          int IsLooseIsoMuon = 0;
	  int IsTightIsoMuon = 0;

          if(reliso<0.2) {
             n_loose++;
             IsLooseIsoMuon = 1;
          }
           
          n_muon++;
	  
          TopMuonEvent topmuon;

          topmuon.pt = mu->pt();
          topmuon.px = mu->px();
          topmuon.py = mu->py();
          topmuon.e  = mu->energy();

          topmuon.d0 = -1.* mu->innerTrack()->dxy(point);
          topmuon.d0err = sqrt( mu->innerTrack()->d0Error() * mu->innerTrack()->d0Error() + beamSpot.BeamWidthX()*beamSpot.BeamWidthX());
          topmuon.d0wrtPV2d = -1. * mu->innerTrack()->dxy(primaryVertex);
          //topmuon.d0wrtPV2derr
          topmuon.normchi2 = mu->globalTrack()->normalizedChi2();

	  /*
          if(n_tight==0)
            {
             
                muon_d0_ = -1.* mu->innerTrack()->dxy(point);
                _muon_d0pv2d = -1. * mu->innerTrack()->dxy(primaryVertex);

                muon_d0Error_ = sqrt( mu->innerTrack()->d0Error() * mu->innerTrack()->d0Error() + beamSpot.BeamWidthX()*beamSpot.BeamWidthX());
                muon_old_reliso_=( mu->pt()/(mu->pt() + mu->isolationR03().sumPt+mu->isolationR03().emEt+mu->isolationR03().hadEt) );
                muon_pt_ = mu->pt();
                muon_eta_ = mu->eta();
                muon_phi_ = mu->phi();
                muon_chi2_ = mu->globalTrack()->normalizedChi2();
                muon_muonhits_ = mu->globalTrack()->hitPattern().numberOfValidMuonHits();
                muon_trackerhits_ = mu->innerTrack()->numberOfValidHits();
                _muon_iso03_track = mu->isolationR03().sumPt;
                _muon_iso03_ecal = mu->isolationR03().emEt;
                _muon_iso03_hcal = mu->isolationR03().hadEt;
                _muon_iso03_ecal_veto = mu->isolationR03().emVetoEt;
                _muon_iso03_hcal_veto = mu->isolationR03().hadVetoEt;

                _muon_iso05_track = mu->isolationR05().sumPt;
                _muon_iso05_ecal = mu->isolationR05().emEt;
                _muon_iso05_hcal = mu->isolationR05().hadEt;
                _muon_iso05_ecal_veto = mu->isolationR05().emVetoEt;
                _muon_iso05_hcal_veto = mu->isolationR05().hadVetoEt;

                _muon_coord[0] = mu->vx();
                _muon_coord[1] = mu->vy();
                _muon_coord[2] = mu->vz();
                // https://twiki.cern.ch/twiki/bin/view/CMS/VbtfWmunuBaselineSelection
                _muon_mustations = mu->numberOfMatches();
                TrackerMu_ = mu->isTrackerMuon();
                GlobalMu_ = mu->isGlobalMuon();
                muon_jet_dr_ = DeltaR;
                double w_et = cmet->et()+ mu->pt();
                double w_px = cmet->px()+ mu->px();
                double w_py = cmet->py()+ mu->py();
                w_mt_ = sqrt(w_et*w_et-w_px*w_px-w_py*w_py);
            }
        }
	  */
	  // Tight Isolated muons
        if (mu->isGlobalMuon()&&mu->isTrackerMuon()
            &&abs(mu->eta())<2.1
            &&mu->pt()>20.
            &&reliso<0.05
            &&mu->innerTrack()->numberOfValidHits()>=11
            &&mu->globalTrack()->normalizedChi2()<10.
            &&mu->globalTrack()->hitPattern().numberOfValidMuonHits()>0
            &&DeltaR>0.3
            &&abs(mu->innerTrack()->dxy(point))<0.02)
        {
            n_tight++;
	    IsTightIsoMuon = 1;
        }
	
	// store muons                                                                                                   
	_ntuple->muons.push_back( topmuon );

       }
    }

    if (1 == n_tight)
        _cutflow->Fill(3);

    if (1 == n_tight &&
        1 == n_loose)
    {
            _cutflow->Fill(4);
    }

    for(GsfElectronCollection::const_iterator elec=electrons->begin(); elec!=electrons->end(); ++elec )
    {
        double RelIso = (elec->dr03TkSumPt()+elec->dr03EcalRecHitSumEt()+elec->dr03HcalTowerSumEt())/elec->et();
        if(elec->et()>15.&&abs(elec->eta())<2.5&&RelIso<0.2)
            n_electron++;
    }

    //    if(n_electron!=0)
    //  return;

    int njets = 0;
    for(CaloJetCollection::const_iterator jet = jets->begin(); jet != jets->end(); ++jet)
    {
        jetID->calculate(iEvent, *jet);
        if (jet->pt()>30.
            &&abs(jet->eta())<2.4
            &&jet->emEnergyFraction()>0.01
            &&jetID->n90Hits()>1
            &&jetID->fHPD()<0.98)
        {

	  TopJetEvent topjet;

	  topjet.px = jet->px();
	  topjet.py = jet->py();
	  topjet.pz = jet->pz();
	  topjet.e  = jet->energy();
	  ++njets;

	  // store jets
	  _ntuple->jets.push_back( topjet );

        }
    }

    if (1 == n_tight &&
        1 == n_loose)
    {
        _cutflow->Fill(5);

	pass_event = true;

        if (njets)
            _cutflow->Fill(6);

        if (1 < njets)
            _cutflow->Fill(7);

        if (2 < njets)
            _cutflow->Fill(8);

        if (3 < njets)
            _cutflow->Fill(9);
    }


    _ntuple->MET = cmet->et();

    ftree->Fill();

    return pass_event;
    
}


// ------------ method called once each job just before starting event loop  ------------
void 
NtupleMaker::beginJob()
{
    theFile = new TFile("NtupleMaker.root", "RECREATE");
    ftree = new TTree("top","top");
    ftree->AutoSave();

    ftree->Branch("top.","TopEventNtuple",&_ntuple,64000,1); 

    _cutflow = new TH1I("cutflow", "Cutflow", 10, 0, 10);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
NtupleMaker::endJob() {
    theFile->cd();
    ftree->Write();
    _cutflow->Write();

    delete ftree;
}

//define this as a plug-in
DEFINE_FWK_MODULE(NtupleMaker);
