
/**_________________________________________________________________
   class:   MuonSelector.cc

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: MuonSelector.cc,v 1.2 2010/09/10 22:33:13 yumiceva Exp $

 ________________________________________________________________**/
#include<iostream>
#include "TopEventNtuple.h"
#include "TLorentzVector.h"

#include<vector>

class MuonSelector {

public:
  MuonSelector() { _version = 4; }
  virtual ~MuonSelector() {}

  void Version(int version) {
    _version = version;
  }
  bool Pass( TopMuonEvent muon, vector< TopJetEvent > jets, float PVz = -9999 ) {
    bool pass = false;
    if (_version == 3 ) pass = PassV3( muon, jets );
    if (_version == 4 ) pass = PassV4( muon, jets, PVz );
    return pass;
  }
  bool PassV3( TopMuonEvent muon, vector< TopJetEvent > jets ) {
    bool pass = false;

    // loop over jets in the event and calculate deltaR
    float deltaR = 999;

    for ( size_t ijet=0; ijet < jets.size(); ++ijet) {

      TopJetEvent ajet = jets[ijet];

      if ( ajet.pt > 30. ) {
	TLorentzVector muP4;
	TLorentzVector jetP4;
	muP4.SetPtEtaPhiE(muon.pt,muon.eta,muon.phi,muon.e);
	jetP4.SetPtEtaPhiE(ajet.pt,ajet.eta,ajet.phi,ajet.e);
	float tmpdeltaR = muP4.DeltaR(jetP4);
	if ( tmpdeltaR < 0.1 ) continue;
	if ( tmpdeltaR < deltaR ) deltaR = tmpdeltaR;
      }
    }

    if (muon.pt>20. &&
	fabs(muon.eta)<2.1 &&
	muon.IsTrackerMuon==1 &&
	fabs(muon.d0)<0.02 &&
	muon.muonhits>0 &&
	muon.normchi2<10 &&
	muon.trackerhits>10 &&
	muon.reliso03<0.05 &&
	deltaR > 0.3
	) pass = true;

    return pass;
  }
  bool PassV4( TopMuonEvent muon, vector< TopJetEvent > jets, float PVz ) {
    bool pass = false;

    // loop over jets in the event and calculate deltaR                                                                                                       
    float deltaR = 999;

    for ( size_t ijet=0; ijet < jets.size(); ++ijet) {

      TopJetEvent ajet = jets[ijet];

      if ( ajet.pt > 30. ) {
	TLorentzVector muP4;
	TLorentzVector jetP4;
	muP4.SetPtEtaPhiE(muon.pt,muon.eta,muon.phi,muon.e);
	jetP4.SetPtEtaPhiE(ajet.pt,ajet.eta,ajet.phi,ajet.e);
	float tmpdeltaR = muP4.DeltaR(jetP4);
	if ( tmpdeltaR < 0.1 ) continue;
        if ( tmpdeltaR < deltaR ) deltaR = tmpdeltaR;
      }
    }

    if (muon.pt>20. &&
        fabs(muon.eta)<2.1 &&
	muon.IsTrackerMuon==1 &&
        fabs(muon.d0)<0.02 &&
        muon.muonhits>1 &&
        muon.normchi2<10 &&
        muon.trackerhits>=11 &&
	muon.reliso03 <0.05 &&
	muon.muonstations > 1 &&
	muon.pixelhits >= 1 &&
	deltaR > 0.3 &&
	fabs(muon.vz - PVz) < 1.
        ) pass = true;
    
    return pass;
  }

private:
  int _version;

};

