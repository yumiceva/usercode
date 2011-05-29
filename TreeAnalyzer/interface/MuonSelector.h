
/**_________________________________________________________________
   class:   MuonSelector.cc

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: MuonSelector.cc,v 1.2 2010/10/08 13:34:27 yumiceva Exp $

 ________________________________________________________________**/
#include<iostream>
#include "Yumiceva/Top7TeV/interface/TopEventNtuple.h"
#include "TLorentzVector.h"

#include<vector>

class MuonSelector {

public:
  MuonSelector() 
  { 
    _version = 4;
    _Pz = 0.;
  }

  void SetPz( double Pz) { _Pz = Pz; }
  
  virtual ~MuonSelector() {}

  void Version(int version) {
    _version = version;
  }

  //bool Pass( TopMuonEvent muon, vector< TopJetEvent > jets, float PVz = -9999 ) {
  //  bool pass = false;
  //  if (_version == 4 ) pass = PassV4( muon, jets, PVz );
  //  return pass;
  //}

  bool MuonLoose( TopMuonEvent muon ) {
    
    bool pass = false;
    if ( muon.pt > 10. && fabs(muon.eta)< 2.5 && muon.reliso03<0.2 ) pass = true;

    return pass;

  }

  bool MuonTight( TopMuonEvent muon, float PVz ) {
    bool pass = false;

    if (muon.pt>20. &&
        fabs(muon.eta)<2.1 &&
	muon.IsTrackerMuon==1 &&
        fabs(muon.d0)<0.02 &&
        muon.muonhits>0 &&
        muon.normchi2<10 &&
        muon.trackerhits>=11 &&
	muon.reliso03 <0.05 &&
	muon.muonstations > 1 &&
	muon.pixelhits >= 1 &&
	fabs(muon.vz - PVz) < 1.
        ) pass = true;
    
    return pass;
  }

private:

  int _version;
  float _Pz;

};

