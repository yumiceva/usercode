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
// Francisco Yumiceva, Fermilab
//         Created:  Fri Jun 11 12:14:21 CDT 2010
// $Id: PATNtupleMaker.cc,v 1.23 2011/05/04 22:37:19 yumiceva Exp $
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

#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"

#include "Yumiceva/Top7TeV/src/TopElectronSelector.cc"
#include "Yumiceva/Top7TeV/interface/PATNtupleMaker.h"

using namespace std;
using namespace edm;
using namespace reco;

PATNtupleMaker::PATNtupleMaker(const edm::ParameterSet& iConfig)
{

  hltTag_ = iConfig.getParameter< InputTag > ("hltTag");
  hltList_ = iConfig.getParameter< vector< string > > ("hltList");
  electronTag_ = iConfig.getParameter< InputTag >("ElectronTag");
  PFelectronTag_ = iConfig.getParameter< InputTag >("PFElectronTag");
  muonTag_ = iConfig.getParameter< InputTag >("MuonTag");
  jetIdLoose_ = iConfig.getParameter<edm::ParameterSet>("jetIdLoose");
  PFjetTag_ = iConfig.getParameter< InputTag >("PFJetTag");
  RhojetTag_ = iConfig.getParameter< InputTag >("RhoJetTag");
  ntuplefile_ = iConfig.getParameter<std::string> ("ntupleFile");
  pfjetIdLoose_ = iConfig.getParameter<edm::ParameterSet>("pfjetIdLoose");
  PFMETTag_ = iConfig.getParameter< InputTag >("PFMETTag");
  PVTag_ = iConfig.getParameter< InputTag >("PVTag");
  _verbose = iConfig.getParameter< bool >("Verbose");
  _isDataInput = "DATA" == iConfig.getParameter<std::string>("inputType");

  cout << "[PATNtupleMaker] Using " << (_isDataInput ? "DATA" : "MC")
       << " input" << endl;

  _ntuple = new TopEventNtuple();
  
}


PATNtupleMaker::~PATNtupleMaker()
{
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

  if (_verbose) cout << "analyzing event" << endl;

  // get handle for IP tools
  //edm::ESHandle<TransientTrackBuilder> trackBuilder;
  //iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);
  
  // -[ Trigger ]-
  Handle< pat::TriggerEvent > triggerEvent;
  iEvent.getByLabel( hltTag_, triggerEvent );
  
  // -[ Muons ]-
  Handle<MuonCollection> muons;
  iEvent.getByLabel( muonTag_, muons);

  // -[ Jets ]-
  Handle<JetCollection> pfjets;
  iEvent.getByLabel( PFjetTag_, pfjets); //selectedPatJetsPFlow
  Handle<double> h_rho;
  iEvent.getByLabel(InputTag(RhojetTag_.label(), "rho"), h_rho);
 
  // -[ Primary Vertices ]-
  reco::Vertex primaryVertex;
  Handle<VertexCollection> pvtx;
  iEvent.getByLabel( PVTag_, pvtx);

  // -[ Beam Spot ]-
  //BeamSpot beamSpot;
  //Handle<BeamSpot> beamSpotHandle;
  //iEvent.getByLabel("offlineBeamSpot", beamSpotHandle);
  //beamSpot = *beamSpotHandle;
  //math::XYZPoint point(beamSpot.x0(),beamSpot.y0(), beamSpot.z0());

  // -[ Electrons ]-
  Handle<ElectronCollection> electrons;
  iEvent.getByLabel(electronTag_,electrons);

  Handle<ElectronCollection> PFelectrons;
  iEvent.getByLabel(PFelectronTag_,PFelectrons);

  // -[ MET ]-
  const pat::MET *pfmet;
  Handle<METCollection> pfmetHandle;
  iEvent.getByLabel(PFMETTag_, pfmetHandle); //patMETsPFlow
  const METCollection *pfmetCol = pfmetHandle.product();
  pfmet = &(pfmetCol->front());

  if (_verbose) cout << "got all collections" << endl;

  //////////////////////
  // T R I G G E R 
  /////////////////////
  bool passORTriggers = false;
  

  /// debug triggers, print the whole erreggrssf list of triggers
  //cout << "wasRun = "<< triggerEvent->wasRun() << endl;
  //const pat::TriggerPathCollection& trigPaths = *(triggerEvent->paths());
  //unsigned int nTrig = trigPaths.size();
  //for (unsigned int i=0; i < nTrig; i++) {
  //  if (trigPaths[i].wasRun()) {
  //    cout << "trigger ran with name = "<< trigPaths[i].name() << endl;
  //  }
  //}

  for (unsigned int itrig=0; itrig < hltList_.size(); itrig++) {

    string aTrigger = hltList_[itrig];
    if (aTrigger == "") { passORTriggers = true; continue; }
    unsigned int prescale = 1.;
    bool tpass = true;
    if (_verbose) cout << "checking trigger name = " << aTrigger << endl;
    if ( !( triggerEvent->path(aTrigger) ) ) {
      tpass = false;
      if (_verbose) cout << " trigger does not exist" << endl;
    } else if (!(triggerEvent->path(aTrigger)->wasRun() && triggerEvent->path(aTrigger)->wasAccept()) ) {
      if (_verbose) cout << "event fail trigger: wasRun="<<triggerEvent->path(aTrigger)->wasRun() << " accept=" << triggerEvent->path(aTrigger)->wasAccept() << endl;
      tpass = false;
    }
    
    if (tpass) prescale = triggerEvent->path(aTrigger)->prescale();
    passORTriggers = passORTriggers || tpass;

    if (_verbose) {
      cout << "trigger pass = " << passORTriggers << endl;
      cout << "trigger prescale = " << prescale << endl;
    }
    //store trigger
    TopTrigger ttrig;
    ttrig.name = aTrigger;
    ttrig.pass = tpass;
    ttrig.prescale = prescale;
    _ntuple->triggers.push_back(ttrig);
  }
  if (_verbose) cout << "triggers analyzed" << endl;
  if (! passORTriggers ) return false;

  // PDF stuff
  edm::Handle<GenEventInfoProduct> pdfstuff;

  if ((!iEvent.isRealData()) && iEvent.getByLabel("generator", pdfstuff)) {
    if (pdfstuff->hasPDF()) {
      TopGenPdfInfo tmpPDF;
      tmpPDF.scalePDF = pdfstuff->pdf()->scalePDF;
      tmpPDF.id1 = pdfstuff->pdf()->id.first;
      tmpPDF.id2 = pdfstuff->pdf()->id.second;
      tmpPDF.x1 = pdfstuff->pdf()->x.first;
      tmpPDF.x2 = pdfstuff->pdf()->x.second;
      tmpPDF.xPdf1 = pdfstuff->pdf()->xPDF.first;
      tmpPDF.xPdf2 = pdfstuff->pdf()->xPDF.second;
      _ntuple->genPdf = tmpPDF;
    }
    if (_verbose) cout << "got generator pdf information" << endl;
  }
  

  // MC stuff
  //__________________
  //Event Flavor History Flag
  Handle< unsigned int > flavorhistoryHandle;
  if ( !_isDataInput && iEvent.getByLabel("flavorHistoryFilter",flavorhistoryHandle) ) {
    _ntuple->flavorHistory = *flavorhistoryHandle;
    //cout << "FlavorHistory variable = " << *flavorhistoryHandle << "  eventFlavor =  " << eventFlavor << endl;
  }

  // for ttbar MC events
    /*
    Handle<TtGenEvent > genEventHandle;
    TtGenEvent genevent;
    Handle<reco::GenParticleCollection> genParticles;
    
    if ( !_isDataInput ) {
      // MC
      TopMyGenEvent mygen; // mostly for ttbar MC 
      // for ttbar MC
      if ( iEvent.getByLabel("genEvt", genEventHandle) ) {
	genevent = *genEventHandle;
	
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
      } else if ( iEvent.getByLabel("genParticles", genParticles) ) { // for W MC
    reco::GenParticleCollection::const_iterator mcIter ;
    for( mcIter=genParticles->begin() ; mcIter!=genParticles->end() ; mcIter++ ) {
      // find a W
      if ( fabs(mcIter->pdgId()) == 24 && mcIter->energy() > 10.0) {
	    
	mygen.Wp_pt = mcIter->pt();
	mygen.isWevent = 1;
      }
      // find a neutrino
      if ( fabs(mcIter->pdgId()) == 12 || fabs(mcIter->pdgId()) == 14 || fabs(mcIter->pdgId()) == 16  ) {

	if (fabs( (mcIter->mother())->pdgId() ) == 24) {
	  mygen.nu_pt = mcIter->pt();
	  mygen.nu_e = mcIter->energy();
	  mygen.nu_eta = mcIter->eta();
	  mygen.nu_phi = mcIter->phi();
	}
      }
    }
    
  }
  
      _ntuple->gen = mygen;
    }
    */

//_cutflow->Fill(0); // count events processed

    // setup jet ID selectors
    pat::strbitset bitset_for_caloJPTjets = jetIdLoose_.getBitTemplate();
    pat::strbitset bitset_for_PFjets = pfjetIdLoose_.getBitTemplate();

    // count number of processed events by analyzer
    _cutflow->Fill(1);

    // PRIMARY VERTEX
    //__________________
    if (pvtx->size()<1) {
      
      if (_verbose) cout << "no PVs found. skip event" << endl;
      return false;
        
    }
    // check PVs
    int ngoodPVs = 0; 
    int npvs = 0;
    bool IsFirstPVGood = false;
    float cutPVz = 24.;
    if (_isDataInput) cutPVz = 24.;
    float refPVz = 0;

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
	  refPVz = pv->z();
	}
      }
      _ntuple->vertices.push_back(topvtx);
      npvs++;
    }
    if (_verbose) cout << "got PVs" << endl;

    if ( ! IsFirstPVGood ) return false;

    // count PV cut
    _cutflow->Fill(2);

    // keep Event ID
    _ntuple->event = iEvent.id().event();
    _ntuple->run   = iEvent.id().run();
    _ntuple->lumi  = iEvent.id().luminosityBlock();

    //math::XYZPoint PVpoint( _ntuple->vertices[0].vx,
    //			    _ntuple->vertices[0].vy,
    //		    _ntuple->vertices[0].vz);

    int nmu_loose = 0;
    int nele_loose = 0;
    int nPFele_loose = 0;
    if (_verbose) cout << "loop over muons" << endl;

    for(MuonCollection::const_iterator mu = muons->begin(); mu != muons->end();  ++mu) {
        
      // relative isolation
      double reliso = (mu->isolationR03().hadEt+mu->isolationR03().emEt+mu->isolationR03().sumPt)/mu->pt(); //

      // calculate deltaR
      double PFDeltaR = 99.;

      for(JetCollection::const_iterator jet = pfjets->begin(); jet != pfjets->end(); ++jet){

	bool passJetID = false;
        passJetID = pfjetIdLoose_(*jet, bitset_for_PFjets);

        if (jet->pt()>20.
            && fabs(jet->eta())<2.4
            && passJetID )
	  {
	    double dr = deltaR(mu->eta(), mu->phi(), jet->eta(), jet->phi());
	    if(dr<PFDeltaR)
	      PFDeltaR=dr;
	  }
      }
      
      nmu_loose++;
	  
      TopMuonEvent topmuon;

      if (_verbose) cout << "filling ntuple with muon kinematics" << endl;
      topmuon.pt = mu->pt();
      topmuon.eta = mu->eta();
      topmuon.phi = mu->phi();
      topmuon.e  = mu->energy();
      topmuon.vx = mu->vx();
      topmuon.vy = mu->vy();
      topmuon.vz = mu->vz();
      topmuon.charge = mu->charge();

      if (_verbose) cout << "filling ntuple with muon IP data" << endl;
      topmuon.d0 = mu->dB();
      topmuon.d0err = mu->edB();

	  //          topmuon.d0 = -1.* mu->innerTrack()->dxy(point);
          //topmuon.d0err = sqrt( mu->innerTrack()->d0Error()*mu->innerTrack()->d0Error() + 0.5* beamSpot.BeamWidthX()*beamSpot.BeamWidthX() + 0.5* beamSpot.BeamWidthY()*beamSpot.BeamWidthY() );
	  
          // IP wrt to hardest PV
	  //reco::TransientTrack tt = trackBuilder->build(mu->innerTrack());
	  //std::pair<bool,Measurement1D> IPresult = IPTools::absoluteTransverseImpactParameter(tt, primaryVertex);
	  //topmuon.d0wrtPV2d = IPresult.second.value();
	  //topmuon.d0wrtPV2derr = IPresult.second.error();

      if (_verbose) cout << "filling muon track data" << endl;
      topmuon.muonhits = mu->numberOfValidHits();
      topmuon.trackerhits = mu->track()->numberOfValidHits();
      topmuon.muonstations = mu->numberOfMatches();
      topmuon.normchi2 = mu->normChi2();
      topmuon.pixelhits = mu->track()->hitPattern().pixelLayersWithMeasurement();
      // storing isolation variables
      topmuon.iso03_track = mu->isolationR03().sumPt;
      topmuon.iso03_ecal = mu->isolationR03().emEt;
      topmuon.iso03_hcal = mu->isolationR03().hadEt;
      topmuon.iso03_ecalveto = mu->isolationR03().emVetoEt;
      topmuon.iso03_hcalveto = mu->isolationR03().hadVetoEt;
      topmuon.reliso03 = reliso;
      topmuon.PFdeltaR = PFDeltaR;

      topmuon.IsTrackerMuon = mu->isTrackerMuon();
      //topmuon.IsLooseIsoMuon = IsLooseIsoMuon;

      topmuon.CalEhad = mu->calEnergy().had;
      topmuon.CalEho = mu->calEnergy().ho;
      topmuon.CalEem = mu->calEnergy().em;
      topmuon.CaloCompatibility = mu->caloCompatibility();

      // https://twiki.cern.ch/twiki/bin/view/CMS/VbtfWmunuBaselineSelection
      
      //                double w_et = cmet->et()+ mu->pt();
      //    double w_px = cmet->px()+ mu->px();
      //    double w_py = cmet->py()+ mu->py();
      //    w_mt_ = sqrt(w_et*w_et-w_px*w_px-w_py*w_py);
      
      // MC stuff
      if (!_isDataInput) {
	if (mu->genLepton() != 0 ) {
	  topmuon.mc.pdgId = (mu->genLepton())->pdgId();
	  //if (mu->genLepton()->mother() != 0) {
	  //  topmuon.mc.mom_pdgId = ((mu->genLepton())->mother())->pdgId();
	  //  topmuon.mc.mom_pt = ((mu->genLepton())->mother())->pt();
	  //  topmuon.mc.mom_eta =((mu->genLepton())->mother())->eta();
	  //  topmuon.mc.mom_phi =((mu->genLepton())->mother())->phi();
	  // }
	}
      }

      _ntuple->muons.push_back( topmuon );

       
    } // close muon loop

    ////////////////////////////////
    // GSF   E L E C T R O N S
    ////////////////////////////////

    TopElectronSelector patEle70(TopElectronSelector::wp70, false);
    TopElectronSelector patEle95(TopElectronSelector::wp95, false);

    bool isConversion = false;

    pat::Electron p4leadingElec;
    pat::Electron p4secondElec;
    if (_verbose) cout << "loop over electrons" << endl;

    for(ElectronCollection::const_iterator elec=electrons->begin(); elec!=electrons->end(); ++elec )
    {

        double RelIso = (elec->dr03TkSumPt()+elec->dr03EcalRecHitSumEt()+elec->dr03HcalTowerSumEt())/elec->et();
	double eta_sc = elec->superCluster()->eta();

        bool pass70 = patEle70(*elec);
        bool pass95 = patEle95(*elec);
        bool IsTight = false;

        bool tmpIsConversion = false;

	if ( (eta_sc <= -2.5 || eta_sc >= -1.566)&&(fabs(eta_sc)>=1.4442)&&(eta_sc <= 1.566 || eta_sc >= 2.5) ) continue;

	if ( elec->et()>30 && fabs( elec->dB() ) < 0.02 && pass70 && RelIso < 0.1 ) {
	  //nel_tight++;
	  p4leadingElec = *elec;
	  //IsTight = true;

	  double nhits = elec->gsfTrack()->trackerExpectedHitsInner().numberOfHits();

	  double e1_dist = elec->convDist();
	  double e1_dcot = elec->convDcot();
	  bool ise1Conv =  fabs(e1_dist) < 0.02 && fabs(e1_dcot) < 0.02 ;

	  if ( ise1Conv || nhits>0 ) {
	    isConversion = true;
	    tmpIsConversion = true;
	  }
	}

	nele_loose++;

	TopElectronEvent topele;
	if (_verbose) cout << "filling electron kinematics" << endl;

	topele.eta = elec->eta();
	topele.phi = elec->phi();
	topele.pt = elec->pt();
	topele.e  = elec->energy();
	topele.vx = elec->vx();
	topele.vy = elec->vy();
	topele.vz = elec->vz();
	topele.charge = elec->charge();
	topele.d0 = elec->dB();
	topele.d0err = elec->edB();

	topele.reliso03 = RelIso;
	topele.IsTight = int(IsTight);
	topele.pass70 = int(pass70);
	topele.pass95 = int(pass95);
	topele.missingHits = elec->gsfTrack()->trackerExpectedHitsInner().numberOfHits();
	topele.convDist = elec->convDist();
	topele.convDcot = elec->convDcot();
	topele.IsConversion = int(tmpIsConversion);

	topele.etasc = eta_sc;
	topele.sigmaIetaIeta = elec->sigmaIetaIeta();
	topele.HoE = elec->hadronicOverEm();
	topele.deltaphisc = patEle70.DeltaPhi();
	topele.deltaetasc = patEle70.DeltaEta();

	// MC stuff 
	//if (!_isDataInput) {
	//if (elec->genLepton() != 0 ) {
	//  topele.mc.pdgId = (elec->genLepton())->pdgId();
	    //cout << "gen electron pdgid = "<< topele.mc.pdgId << endl;
	    // this is crashing for the moment
	    //if (elec->genLepton()->mother() != 0) {
	    //  cout << "mother" << endl;
	    //  topele.mc.mom_pdgId = ((elec->genLepton())->mother())->pdgId();
	    //  cout << " mom pdgid = " << topele.mc.mom_pdgId << endl;
	    //  topele.mc.mom_pt = ((elec->genLepton())->mother())->pt();
	    //  topele.mc.mom_eta =((elec->genLepton())->mother())->eta();
	    //  topele.mc.mom_phi =((elec->genLepton())->mother())->phi();
	    //}
	//}
	//}

	_ntuple->electrons.push_back( topele );

    }
    
    ////////////////////////////////                                                                                                                                                                                                        
    // PF   E L E C T R O N S 
    ////////////////////////////////

    isConversion = false;
    if (_verbose) cout << "loop over pf electrons" << endl;
    for(ElectronCollection::const_iterator elec=PFelectrons->begin(); elec!=PFelectrons->end(); ++elec )
      {

        double RelIso = (elec->dr03TkSumPt()+elec->dr03EcalRecHitSumEt()+elec->dr03HcalTowerSumEt())/elec->et();
        double eta_sc = elec->superCluster()->eta();

        bool pass70 = patEle70(*elec);
        bool pass95 = patEle95(*elec);
        bool IsTight = false;

        bool tmpIsConversion = false;

        if ( (eta_sc <= -2.5 || eta_sc >= -1.566)&&(fabs(eta_sc)>=1.4442)&&(eta_sc <= 1.566 || eta_sc >= 2.5) ) continue;

        if ( elec->et()>30 && fabs( elec->dB() ) < 0.02 && pass70 && RelIso < 0.1 ) {
          //nel_tight++;                                                                                                                                                                                                                       
          p4leadingElec = *elec;
          //IsTight = true;                                                                                                                                                                                                                    

          double nhits = elec->gsfTrack()->trackerExpectedHitsInner().numberOfHits();

          double e1_dist = elec->convDist();
          double e1_dcot = elec->convDcot();
          bool ise1Conv =  fabs(e1_dist) < 0.02 && fabs(e1_dcot) < 0.02 ;

          if ( ise1Conv || nhits>0 ) {
            isConversion = true;
            tmpIsConversion = true;
          }
        }

	nPFele_loose++;

	TopElectronEvent topele;
	if (_verbose) cout << "filling electron kinematics" << endl;

        topele.eta = elec->eta();
        topele.phi = elec->phi();
        topele.pt = elec->pt();
        topele.e  = elec->energy();
        topele.vx = elec->vx();
        topele.vy = elec->vy();
        topele.vz = elec->vz();
        topele.charge = elec->charge();
        topele.d0 = elec->dB();
        topele.d0err = elec->edB();

        topele.reliso03 = RelIso;
        topele.IsTight = int(IsTight);
        topele.pass70 = int(pass70);
        topele.pass95 = int(pass95);
        topele.missingHits = elec->gsfTrack()->trackerExpectedHitsInner().numberOfHits();
        topele.convDist = elec->convDist();
        topele.convDcot = elec->convDcot();
        topele.IsConversion = int(tmpIsConversion);

        topele.etasc = eta_sc;
        topele.sigmaIetaIeta = elec->sigmaIetaIeta();
        topele.HoE = elec->hadronicOverEm();
        topele.deltaphisc = patEle70.DeltaPhi();
        topele.deltaetasc = patEle70.DeltaEta();

	// MC stuff
        if (!_isDataInput) {
          if (elec->genLepton() != 0 ) {
            topele.mc.pdgId = (elec->genLepton())->pdgId();
            //if (elec->genLepton()->mother() != 0) {
            //  topele.mc.mom_pdgId = ((elec->genLepton())->mother())->pdgId();
            //  topele.mc.mom_pt = ((elec->genLepton())->mother())->pt();
            //  topele.mc.mom_eta =((elec->genLepton())->mother())->eta();
            //  topele.mc.mom_phi =((elec->genLepton())->mother())->phi();
            //}
	  }
        }

        _ntuple->PFelectrons.push_back( topele );

      }



    // check if at least one lepton passes
    if ( nele_loose==0 && nPFele_loose == 0 && nmu_loose==0 ) return false;

    // PF Jets
    int npfjets = 0;
    if (_verbose) cout << "loop over jets" << endl;

    for(JetCollection::const_iterator jet = pfjets->begin(); jet != pfjets->end(); ++jet)
      {
	bool passJetID = false;
        passJetID = pfjetIdLoose_(*jet, bitset_for_PFjets);

        if (jet->pt()>20.
            && fabs(jet->eta())<2.4
            && passJetID)
	  {

            TopJetEvent topjet;

            topjet.eta = jet->eta();
            topjet.phi = jet->phi();
            topjet.pt = jet->pt();
            topjet.e  = jet->energy();
            ++npfjets;

	    topjet.id_neutralEmE = (jet->correctedJet("Uncorrected")).neutralEmEnergy();
	    topjet.id_chargedEmE = (jet->correctedJet("Uncorrected")).chargedEmEnergy();
	    topjet.id_muonMultiplicity = (jet->correctedJet("Uncorrected")).muonMultiplicity();

	    topjet.ntracks = jet->associatedTracks().size();
	    //const reco::SecondaryVertexTagInfo & svTagInfo = *(jet->tagInfoSecondaryVertex());
	    //topjet.nSVs = svTagInfo.nVertices();
            topjet.ndaughters = (jet->correctedJet("Uncorrected")).numberOfDaughters();
            topjet.btag_TCHE  = jet->bDiscriminator( "trackCountingHighEffBJetTags" );
            topjet.btag_TCHP  = jet->bDiscriminator( "trackCountingHighPurBJetTags" );
            topjet.btag_SSVHE = jet->bDiscriminator( "simpleSecondaryVertexHighEffBJetTags" );
            topjet.btag_SSVHP = jet->bDiscriminator( "simpleSecondaryVertexHighPurBJetTags" );
	    
            if (! _isDataInput ) {
              topjet.mc.flavor = jet->partonFlavour();
	      if (jet->genJet()) {
		topjet.mc.eta = jet->genJet()->eta();
		topjet.mc.phi = jet->genJet()->phi();
		topjet.mc.pt = jet->genJet()->pt();
		topjet.mc.e  = jet->genJet()->energy();
	      }
            }
	    
            // store jets
	    _ntuple->PFjets.push_back( topjet );
	    
          }
      }

    //_ntuple->rho = *h_rho;

    //////////////////////////
    // M E T
    /////////////////////////
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
