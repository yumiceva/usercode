// -*- C++ -*-
//
// Package:    PATNtupleMaker
// Class:      PATNtupleMaker
// 
/**\class PATNtupleMaker PATNtupleMaker.cc Exercise/PATNtupleMaker/src/PATNtupleMaker.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  "Jian Wang"
//        Modified:  Samvel Khalatian, Francisco Yumiceva
//         Created:  Fri Jun 11 12:14:21 CDT 2010
// $Id: PATNtupleMaker.cc,v 1.13 2010/08/26 20:38:54 yumiceva Exp $
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
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"

#include "Yumiceva/Top7TeV/interface/PATNtupleMaker.h"

using namespace std;
using namespace edm;
using namespace reco;

PATNtupleMaker::PATNtupleMaker(const edm::ParameterSet& iConfig):
  ntuplefile_(iConfig.getParameter<std::string> ("ntupleFile")),
  hltTag_(iConfig.getParameter< InputTag >("hltTag")),
  muonTag_(iConfig.getParameter< InputTag >("MuonTag")),
  electronTag_(iConfig.getParameter< InputTag >("ElectronTag")),
  jetTag_(iConfig.getParameter< InputTag >("JetTag")),
  metTag_(iConfig.getParameter< InputTag >("METTag"))

{
  //now do what ever initialization is needed

  //jetID = new helper::JetIDHelper(iConfig.getParameter<ParameterSet>("JetIDParams"));
  _isDataInput = "DATA" == iConfig.getParameter<std::string>("inputType");

  cout << "[PATNtupleMaker] Using " << (_isDataInput ? "DATA" : "MC")
       << " input" << endl;

  _ntuple = new TopEventNtuple();
  
}


PATNtupleMaker::~PATNtupleMaker()
{
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

  delete _ntuple;
}


//
// member functions
//

// ------------ method called to for each event  ------------
bool
PATNtupleMaker::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using pat::ElectronCollection;
  using pat::JetCollection;
  using pat::MuonCollection;
  using pat::METCollection;

  bool pass_event = true; // for the moment DO not filter

   // reset ntuple containers
   _ntuple->Reset();

    // -[ Trigger ]-
    //Handle<TriggerResults> hlt;
    //iEvent.getByLabel(hltTag_,hlt);
    //const TriggerNames & hltNames_ = iEvent.triggerNames(*hlt);

   // get handle for IP tools
   edm::ESHandle<TransientTrackBuilder> trackBuilder;
   iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);
   reco::Vertex primaryVertex;

    // -[ Muons ]-
    Handle<MuonCollection> muons;
    iEvent.getByLabel(muonTag_,muons);

    // -[ Jets ]-
    Handle<JetCollection> calojets;
    iEvent.getByLabel(jetTag_,calojets);
    
    Handle<JetCollection> jptjets;
    iEvent.getByLabel("patJPTJetUserData",jptjets); //patJetsAK5JPT

    Handle<JetCollection> pfjets;
    iEvent.getByLabel("patJetsAK5PF",pfjets);
    
    // -[ Primary Vertices ]-
    Handle<VertexCollection> pvtx;
    iEvent.getByLabel("offlinePrimaryVertices",pvtx);

    // -[ Beam Spot ]-
    BeamSpot beamSpot;
    Handle<BeamSpot> beamSpotHandle;
    iEvent.getByLabel("offlineBeamSpot", beamSpotHandle);
    beamSpot = *beamSpotHandle;
    math::XYZPoint point(beamSpot.x0(),beamSpot.y0(), beamSpot.z0());

    // -[ Electrons ]-
    Handle<ElectronCollection> electrons;
    iEvent.getByLabel(electronTag_,electrons);

    // -[ MET ]-
    const pat::MET *cmet;
    Handle<METCollection> cmetHandle;
    iEvent.getByLabel(metTag_, cmetHandle);
    //const CaloMETCollection *cmetCol = cmetHandle.product();
    const METCollection *cmetCol = cmetHandle.product();
    cmet = &(cmetCol->front());

    // tcMET
    const pat::MET *tcmet;
    Handle<METCollection> tcmetHandle;
    iEvent.getByLabel("patMETsTC", tcmetHandle);
    const METCollection *tcmetCol = tcmetHandle.product();
    tcmet = &(tcmetCol->front());

    // PFMET
    const pat::MET *pfmet;
    Handle<METCollection> pfmetHandle;
    iEvent.getByLabel("patMETsPF", pfmetHandle);
    const METCollection *pfmetCol = pfmetHandle.product();
    pfmet = &(pfmetCol->front());

    
    // MC stuff
    //__________________
    //Event Flavor History Flag
    Handle< unsigned int > flavorhistoryHandle;
    if ( !_isDataInput && iEvent.getByLabel("flavorHistoryFilter",flavorhistoryHandle) ) {
       _ntuple->flavorHistory = *flavorhistoryHandle;
       //cout << "FlavorHistory variable = " << *flavorhistoryHandle << "  eventFlavor =  " << eventFlavor << endl;
    }
    // for ttbar MC events
    Handle<TtGenEvent > genEventHandle;
    TtGenEvent genevent;
    if ( !_isDataInput && iEvent.getByLabel("genEvt", genEventHandle) ) {
      genevent = *genEventHandle;
      TopMyGenEvent mygen;
      mygen.isSemiLeptonic = int(genevent.isSemiLeptonic());
      mygen.isDilepton = int(genevent.isFullLeptonic());
      mygen.LeptonicChannel = genevent.semiLeptonicChannel();
      mygen.t_pt =  (genevent.top())->pt();
      mygen.t_eta = (genevent.top())->eta();
      mygen.t_phi = (genevent.top())->phi();
      mygen.t_e =   (genevent.top())->energy();
      mygen.tbar_pt =  (genevent.topBar())->pt();
      mygen.tbar_eta = (genevent.topBar())->eta();
      mygen.tbar_phi = (genevent.topBar())->phi();
      mygen.tbar_e =   (genevent.topBar())->energy();
      if ( genevent.isSemiLeptonic() ) {
	mygen.nu_pt =  (genevent.singleNeutrino())->pt();
	mygen.nu_eta = (genevent.singleNeutrino())->eta();
	mygen.nu_phi = (genevent.singleNeutrino())->phi();
	mygen.nu_e =   (genevent.singleNeutrino())->energy();
	mygen.mu_pt =  (genevent.singleLepton())->pt();
	mygen.mu_eta = (genevent.singleLepton())->eta();
	mygen.mu_phi = (genevent.singleLepton())->phi();
	mygen.mu_e =   (genevent.singleLepton())->energy();
	mygen.Wp_pt =  (genevent.hadronicDecayQuark())->pt();
	mygen.Wp_eta = (genevent.hadronicDecayQuark())->eta();
	mygen.Wp_phi = (genevent.hadronicDecayQuark())->phi();
	mygen.Wp_e =   (genevent.hadronicDecayQuark())->energy();
	mygen.Wq_pt =  (genevent.hadronicDecayQuarkBar())->pt();
        mygen.Wq_eta = (genevent.hadronicDecayQuarkBar())->eta();
        mygen.Wq_phi = (genevent.hadronicDecayQuarkBar())->phi();
        mygen.Wq_e =   (genevent.hadronicDecayQuarkBar())->energy();
	mygen.bHad_pt =  (genevent.hadronicDecayB())->pt();
        mygen.bHad_eta = (genevent.hadronicDecayB())->eta();
        mygen.bHad_phi = (genevent.hadronicDecayB())->phi();
        mygen.bHad_e =   (genevent.hadronicDecayB())->energy();
	mygen.bLep_pt =  (genevent.leptonicDecayB())->pt();
        mygen.bLep_eta = (genevent.leptonicDecayB())->eta();
        mygen.bLep_phi = (genevent.leptonicDecayB())->phi();
        mygen.bLep_e =   (genevent.leptonicDecayB())->energy();
      }
      _ntuple->gen = mygen;
    }
    //_cutflow->Fill(0);

    //bool hlt_mu_ = false; 
    //for(size_t itrig = 0; itrig != hlt->size(); itrig++){
    //    string hltName = hltNames_.triggerName(itrig);
    //    if(hltName == "HLT_Mu9"&&hlt->accept(itrig))
    //    {
    //        hlt_mu_ = true;
    //        break;
    //    }
    //}
    //if(!hlt_mu_)
    //    return false;

    _cutflow->Fill(1);

    if(pvtx->size()<1)
        return false;
        
    // check PVs
    int ngoodPVs = 0; 
    int npvs = 0;
    bool IsFirstPVGood = false;
    float cutPVz = 15.;
    if (_isDataInput) cutPVz = 24.;

    for(VertexCollection::const_iterator pv = pvtx->begin(); pv != pvtx->end(); ++pv ) {

      TopVertexEvent topvtx;
      topvtx.vx = pv->x();
      topvtx.vy = pv->y();
      topvtx.vz = pv->z();
      topvtx.ndof = pv->ndof();
      topvtx.rho = pv->position().Rho();
      topvtx.IsGood = 0.;
      // PV quality cuts
      if( !pv->isFake()
	 &&pv->ndof()>4
	 &&fabs(pv->z())< cutPVz
	 &&fabs(pv->position().Rho())<2.0 ) {

	ngoodPVs++;
	topvtx.IsGood = 1;
	// check if first PV (hardest) is a good PV
	if (npvs==0) {
	  primaryVertex = *pv;
	  IsFirstPVGood = true;
	}
      }
      _ntuple->vertices.push_back(topvtx);
      npvs++;
    }
    if ( ! IsFirstPVGood ) return false;

    _cutflow->Fill(2);

    // Event ID
    _ntuple->event = iEvent.id().event();
    _ntuple->run   = iEvent.id().run();
    _ntuple->lumi  = iEvent.id().luminosityBlock();

    //math::XYZPoint PVpoint( _ntuple->vertices[0].vx,
    //			    _ntuple->vertices[0].vy,
    //		    _ntuple->vertices[0].vz);

    size_t n_loose=0;
    size_t n_tight=0;
    size_t n_muon=0;
    size_t n_electron=0;
    
    //if (_debug) cout << "begin loop over muons" << endl;

    for(MuonCollection::const_iterator mu = muons->begin(); mu != muons->end();  ++mu) {
        
      // relative isolation
      double reliso = (mu->isolationR03().hadEt+mu->isolationR03().emEt+mu->isolationR03().sumPt)/mu->pt(); //

      // calculate deltaR
      double CaloDeltaR = 99.;
      double JPTDeltaR = 99.;
      double PFDeltaR = 99.;

      for(JetCollection::const_iterator jet = calojets->begin(); jet != calojets->end(); ++jet){
          
	if (jet->pt()>30.
	    &&abs(jet->eta())<2.4
	    &&jet->emEnergyFraction() >0.01
	    &&jet->jetID().n90Hits>1
	    &&jet->jetID().fHPD<0.98)
          {
	    double dr = deltaR(mu->eta(), mu->phi(), jet->eta(), jet->phi());
	    if(dr<CaloDeltaR)
	      CaloDeltaR=dr;
          }
      }
      //if (_debug) cout << "got calo deltaR" << endl;
      for(JetCollection::const_iterator jet = jptjets->begin(); jet != jptjets->end(); ++jet){

	//double emf = jet->emEnergyFraction();
	double emf = jet->userFloat("MyemEnergyFraction");
	double fhpd = jet->userFloat("MyfHPD");
	int n90 = jet->userFloat("Myn90");

	if (jet->pt()>30.
	    &&abs(jet->eta())<2.4
	    && emf >0.01
	    && n90>1
	    && fhpd<0.98 )
	  {
	    double dr = deltaR(mu->eta(), mu->phi(), jet->eta(), jet->phi());
	    if(dr<JPTDeltaR)
	      JPTDeltaR=dr;
	  }
      }
      
      //if (_debug) cout << "got jpt deltaR" << endl;
      for(JetCollection::const_iterator jet = pfjets->begin(); jet != pfjets->end(); ++jet){

	// cache some variables 
        double chf = (jet->correctedJet("RAW")).chargedHadronEnergyFraction();
        double cef = (jet->correctedJet("RAW")).chargedEmEnergyFraction();
	double nef = (jet->correctedJet("RAW")).neutralEmEnergyFraction();
	int    nch = (jet->correctedJet("RAW")).chargedMultiplicity();
        double nhf = 0.;//((jet->correctedJet("RAW")).neutralHadronEnergy() + (jet->correctedJet("RAW")).HFHadronEnergy() ) / (jet->correctedJet("RAW")).energy();
	int nconstituents = (jet->correctedJet("RAW")).numberOfDaughters();
	
        if (jet->pt()>20.
            &&abs(jet->eta())<2.4
            && nhf < 0.99 && nef < 0.99&& nconstituents > 1
            && chf > 0.&& nch > 0. && cef < 0.99
            )
          {
	    double dr = deltaR(mu->eta(), mu->phi(), jet->eta(), jet->phi());
	    if(dr<PFDeltaR)
	      PFDeltaR=dr;
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
          topmuon.eta = mu->eta();
          topmuon.phi = mu->phi();
          topmuon.e  = mu->energy();
	  topmuon.vx = mu->vx();
	  topmuon.vy = mu->vy();
	  topmuon.vz = mu->vz();
	  topmuon.charge = mu->charge();
	  // IP wrt to beam spot
          topmuon.d0 = -1.* mu->innerTrack()->dxy(point);
          topmuon.d0err = sqrt( mu->innerTrack()->d0Error()*mu->innerTrack()->d0Error() + 0.5* beamSpot.BeamWidthX()*beamSpot.BeamWidthX() + 0.5* beamSpot.BeamWidthY()*beamSpot.BeamWidthY() );
	  // IP wrt to hardest PV
	  reco::TransientTrack tt = trackBuilder->build(mu->innerTrack());
	  std::pair<bool,Measurement1D> IPresult = IPTools::absoluteTransverseImpactParameter(tt, primaryVertex);
	  topmuon.d0wrtPV2d = IPresult.second.value();
	  topmuon.d0wrtPV2derr = IPresult.second.error();
	  
	  topmuon.muonhits = mu->globalTrack()->hitPattern().numberOfValidMuonHits();
	  topmuon.trackerhits = mu->innerTrack()->numberOfValidHits();
	  topmuon.muonstations = mu->numberOfMatches();
          topmuon.normchi2 = mu->globalTrack()->normalizedChi2();
	  
	  topmuon.iso03_track = mu->isolationR03().sumPt;
	  topmuon.iso03_ecal = mu->isolationR03().emEt;
	  topmuon.iso03_hcal = mu->isolationR03().hadEt;
	  topmuon.iso03_ecalveto = mu->isolationR03().emVetoEt;
	  topmuon.iso03_hcalveto = mu->isolationR05().hadVetoEt;
	  topmuon.reliso03 = reliso;
	  topmuon.CalodeltaR = CaloDeltaR;
	  topmuon.JPTdeltaR = JPTDeltaR;
	  topmuon.PFdeltaR = PFDeltaR;

	  topmuon.IsTrackerMuon = mu->isTrackerMuon();
	  topmuon.IsLooseIsoMuon = IsLooseIsoMuon;

	  topmuon.CalEhad = mu->calEnergy().had;
	  topmuon.CalEho = mu->calEnergy().ho;
	  topmuon.CalEem = mu->calEnergy().em;
	  topmuon.CaloCompatibility = mu->caloCompatibility();

	  //_muon_iso05_track = mu->isolationR05().sumPt;
	  //_muon_iso05_ecal = mu->isolationR05().emEt;
	  //_muon_iso05_hcal = mu->isolationR05().hadEt;
	  //_muon_iso05_ecal_veto = mu->isolationR05().emVetoEt;
	  //_muon_iso05_hcal_veto = mu->isolationR05().hadVetoEt;

	  // https://twiki.cern.ch/twiki/bin/view/CMS/VbtfWmunuBaselineSelection

	  //                double w_et = cmet->et()+ mu->pt();
	  //    double w_px = cmet->px()+ mu->px();
	  //    double w_py = cmet->py()+ mu->py();
	  //    w_mt_ = sqrt(w_et*w_et-w_px*w_px-w_py*w_py);
        
	  // Tight Isolated muons
	  if (mu->isGlobalMuon()&&mu->isTrackerMuon()
	      &&abs(mu->eta())<2.1
	      &&mu->pt()>20.
	      &&reliso<0.05
	      &&mu->innerTrack()->numberOfValidHits()>=11
	      &&mu->globalTrack()->normalizedChi2()<10.
	      &&mu->globalTrack()->hitPattern().numberOfValidMuonHits()>0
	      &&CaloDeltaR>0.3
	      &&abs(mu->innerTrack()->dxy(point))<0.02)
	    {
	      n_tight++;
	      IsTightIsoMuon = 1;
	      topmuon.IsTightIsoMuon = 1;
	    }
	  
	  // MC stuff
	  if (!_isDataInput) {
	    if (mu->genLepton() != 0 ) {
	      topmuon.mc.pdgId = (mu->genLepton())->pdgId();
	      if (mu->genLepton()->mother() != 0) {
		topmuon.mc.mom_pdgId = ((mu->genLepton())->mother())->pdgId();
		topmuon.mc.mom_pt = ((mu->genLepton())->mother())->pt();
		topmuon.mc.mom_eta =((mu->genLepton())->mother())->eta();
		topmuon.mc.mom_phi =((mu->genLepton())->mother())->phi();
	      }
	    }
	  }
	
	// store muons                                                                                                   
	_ntuple->muons.push_back( topmuon );

       }
    }

    // keep at least one loose muon not isolated
    if ( n_muon == 0 ) return false;

    //if(_debug) cout << "look at jets now" << endl;

    if (1 == n_tight)
        _cutflow->Fill(3);

    if (1 == n_tight &&
        1 == n_loose)
    {
            _cutflow->Fill(4);
    }

    // Electrons
    for(ElectronCollection::const_iterator elec=electrons->begin(); elec!=electrons->end(); ++elec )
    {
        double RelIso = (elec->dr03TkSumPt()+elec->dr03EcalRecHitSumEt()+elec->dr03HcalTowerSumEt())/elec->et();
        if(elec->et()>15.&&abs(elec->eta())<2.5&&RelIso<0.2) {
	 
	  n_electron++;
	  TopElectronEvent topele;
	  
	  topele.eta = elec->eta();
	  topele.phi = elec->phi();
	  topele.pt = elec->pt();
	  topele.e  = elec->energy();
	  topele.vx = elec->vx();
	  topele.vy = elec->vy();
	  topele.vz = elec->vz();

	  topele.reliso03 = RelIso;

	  _ntuple->electrons.push_back( topele );

	}
    }

    // Calo Jets
    int ncalojets = 0;
    for(JetCollection::const_iterator jet = calojets->begin(); jet != calojets->end(); ++jet)
    {
      if (jet->pt()>30.
            &&abs(jet->eta())<2.4
            &&jet->emEnergyFraction()>0.01
            &&jet->jetID().n90Hits>1
            &&jet->jetID().fHPD<0.98)
        {

	  TopJetEvent topjet;

	  topjet.eta = jet->eta();
	  topjet.phi = jet->phi();
	  topjet.pt = jet->pt();
	  topjet.e  = jet->energy();
	  ++ncalojets;
	  
	  topjet.ntracks = jet->associatedTracks().size();
	  topjet.ndaughters =jet->numberOfDaughters();
	  topjet.btag_TCHE  = jet->bDiscriminator( "trackCountingHighEffBJetTags" );
	  topjet.btag_TCHP  = jet->bDiscriminator( "trackCountingHighPurBJetTags" );
	  topjet.btag_SSVHE = jet->bDiscriminator( "simpleSecondaryVertexHighEffBJetTags" );
	  topjet.btag_SSVHP = jet->bDiscriminator( "simpleSecondaryVertexHighPurBJetTags" );

	  if (! _isDataInput ) {
	    topjet.mc.flavor = jet->partonFlavour();
	  }
	  // store jets
	  _ntuple->Calojets.push_back( topjet );

        }
    }

    // JPT Jets
    int njptjets = 0;
    for(JetCollection::const_iterator jet = jptjets->begin(); jet != jptjets->end(); ++jet)
      {
	double emf = jet->userFloat("MyemEnergyFraction");
        double fhpd = jet->userFloat("MyfHPD");
        int n90 = jet->userFloat("Myn90");
	
	//cout << "got one JPT" << endl;
	//cout << "emf = " << emf << endl;
	//cout << "n90 = " << n90 << endl;
	//cout << "fhpd = "<< fhpd << endl;
	//cout << "pt = " << jet->pt() << endl;
        if (jet->pt()>25.
            &&abs(jet->eta())<2.4
	    && emf >0.01
            && n90>1
            && fhpd<0.98 )
	  {
	    //cout << "pass jet ID for jpt jets" << endl;

	    TopJetEvent topjet;

	    topjet.eta = jet->eta();
	    topjet.phi = jet->phi();
	    topjet.pt = jet->pt();
	    topjet.e  = jet->energy();
	    ++njptjets;

	    topjet.ntracks = jet->associatedTracks().size();
	    topjet.ndaughters = jet->numberOfDaughters();
	    topjet.btag_TCHE  = jet->bDiscriminator( "trackCountingHighEffBJetTags" );
	    topjet.btag_TCHP  = jet->bDiscriminator( "trackCountingHighPurBJetTags" );
	    topjet.btag_SSVHE = jet->bDiscriminator( "simpleSecondaryVertexHighEffBJetTags" );
	    topjet.btag_SSVHP = jet->bDiscriminator( "simpleSecondaryVertexHighPurBJetTags" );

	    if (! _isDataInput ) {
	      topjet.mc.flavor = jet->partonFlavour();
	    }
	    // store jets
	    _ntuple->JPTjets.push_back( topjet );

	  }
      }

    // PF Jets
    int npfjets = 0;
    for(JetCollection::const_iterator jet = pfjets->begin(); jet != pfjets->end(); ++jet)
      {
	// cache some variables
        double chf = (jet->correctedJet("RAW")).chargedHadronEnergyFraction();
        double cef = (jet->correctedJet("RAW")).chargedEmEnergyFraction();
        double nef = (jet->correctedJet("RAW")).neutralEmEnergyFraction();
        int    nch = (jet->correctedJet("RAW")).chargedMultiplicity();
        double nhf = 0.;//((jet->correctedJet("RAW")).neutralHadronEnergy() + (jet->correctedJet("RAW")).HFHadronEnergy() ) / (jet->correctedJet("RAW")).energy();
        int nconstituents = (jet->correctedJet("RAW")).numberOfDaughters();

        if (jet->pt()>20.
            &&abs(jet->eta())<2.4
            && nhf < 0.99 && nef < 0.99 && nconstituents > 1
	    && chf > 0. && nch > 0. && cef < 0.99
            )
          {

            TopJetEvent topjet;

            topjet.eta = jet->eta();
            topjet.phi = jet->phi();
            topjet.pt = jet->pt();
            topjet.e  = jet->energy();
            ++npfjets;

	    topjet.id_neutralEmE = (jet->correctedJet("RAW")).neutralEmEnergy();
	    topjet.id_chargedEmE = (jet->correctedJet("RAW")).chargedEmEnergy();
	    topjet.id_muonMultiplicity = (jet->correctedJet("RAW")).muonMultiplicity();

	    topjet.ntracks = jet->associatedTracks().size();
            topjet.ndaughters = nconstituents;
            topjet.btag_TCHE  = jet->bDiscriminator( "trackCountingHighEffBJetTags" );
            topjet.btag_TCHP  = jet->bDiscriminator( "trackCountingHighPurBJetTags" );
            topjet.btag_SSVHE = jet->bDiscriminator( "simpleSecondaryVertexHighEffBJetTags" );
            topjet.btag_SSVHP = jet->bDiscriminator( "simpleSecondaryVertexHighPurBJetTags" );

            if (! _isDataInput ) {
              topjet.mc.flavor = jet->partonFlavour();
            }
            // store jets
	    _ntuple->PFjets.push_back( topjet );
	    
          }
      }


    
    if (1 == n_tight &&
        1 == n_loose && n_electron==0)
    {
        _cutflow->Fill(5);

        if (ncalojets)
            _cutflow->Fill(6);

        if (1 < ncalojets)
            _cutflow->Fill(7);

        if (2 < ncalojets)
            _cutflow->Fill(8);

        if (3 < ncalojets) {
	  _cutflow->Fill(9);
	  pass_event = true;
	}
    }

    


    _ntuple->CaloMET = cmet->et();
    //_ntuple->CaloMETeta = cmet->eta();
    _ntuple->CaloMETphi = cmet->phi();
    _ntuple->CaloHt = cmet->sumEt();

    _ntuple->tcMET = tcmet->et();
    _ntuple->tcMETphi = tcmet->phi();
    _ntuple->tcHt = tcmet->sumEt();

    _ntuple->PFMET = pfmet->et();
    _ntuple->PFMETphi = pfmet->phi();
    _ntuple->PFHt = pfmet->sumEt();

    ftree->Fill();

    return pass_event;
    
}


// ------------ method called once each job just before starting event loop  ------------
void 
PATNtupleMaker::beginJob()
{
  //theFile = new TFile(ntuplefile_.c_str(), "RECREATE");
  //ftree = new TTree("top","top");
  //ftree->AutoSave();
    
  //_cutflow = new TH1I("cutflow", "Cutflow", 10, 0, 10);

  edm::Service<TFileService> tfileservice;
  tfileservice->file().cd("/");

  ftree = tfileservice->make<TTree>("top","top");
  ftree->Branch("top.","TopEventNtuple",&_ntuple,64000,1);

  _cutflow = tfileservice->make<TH1I>("cutflow","Number of events", 10, 0, 10);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
PATNtupleMaker::endJob() {
  //theFile->cd();
  //ftree->Write();
  //_cutflow->Write();

  //delete ftree;
}

//define this as a plug-in
DEFINE_FWK_MODULE(PATNtupleMaker);
