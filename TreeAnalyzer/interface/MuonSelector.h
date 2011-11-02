
/**_________________________________________________________________
   class:   MuonSelector.cc

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: MuonSelector.h,v 1.4 2011/09/01 19:31:54 yumiceva Exp $

 ________________________________________________________________**/
#include<iostream>
#include "Yumiceva/Top7TeV/interface/TopEventNtuple.h"
#include "TLorentzVector.h"

#include<vector>

using namespace std;

class MuonSelector {

public:
  MuonSelector() 
  { 
    _version = 4;
    _Pz = 0.;
    _deltaR = -1;
  }

  void SetPz( double Pz) { _Pz = Pz; }
  
  virtual ~MuonSelector() {}

  void Version(int version) {
    _version = version;
  }

  double GetDeltaR() { return _deltaR; }

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

    if (muon.pt > 35. &&
        fabs(muon.eta)<2.1 &&
	muon.IsTrackerMuon==1 &&
        fabs(muon.d0)<0.02 &&
        muon.muonhits>0 &&
        muon.normchi2<10 &&
        muon.trackerhits>=11 &&
	muon.reliso03 < 0.1 && // was 0.125
	muon.muonstations > 1 &&
	muon.pixelhits >= 1 &&
	fabs(muon.vz - PVz) < 1.
        ) pass = true;
    
    return pass;
  }

  bool MuonTightDeltaR( TopMuonEvent muon, float PVz, vector< TopJetEvent > jets ) {
    bool pass = false;

    if ( MuonTight( muon, PVz ) )
      {
	double deltaR = 999.;
	TLorentzVector tmpp4Mu;
	tmpp4Mu.SetPtEtaPhiE(muon.pt, muon.eta, muon.phi, muon.e );

	for ( vector< TopJetEvent>::iterator ijet=jets.begin(); ijet != jets.end(); ++ijet)
	  {

	    TopJetEvent jet = *ijet;

	    if ( jet.pt > 35. && fabs(jet.eta) < 2.4 )
	      {
		
		TLorentzVector tmpp4Jet;
		tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e );
		double tmpdeltaR = tmpp4Mu.DeltaR(tmpp4Jet);
		if ( tmpdeltaR < deltaR ) deltaR = tmpdeltaR;

	      } // jet ID
	  } // jet loop
	_deltaR = deltaR; // store value
	if ( deltaR > 0.3 ) pass = true;

      } // muon ID
    return pass;
  }

  bool MuonNoIsoTight( TopMuonEvent muon, float PVz, vector< TopJetEvent > jets ) {
    bool pass = false;

    if ( muon.pt > 35. &&
	 fabs(muon.eta)<2.1 &&
	 muon.IsTrackerMuon==1 &&
	 fabs(muon.d0)<0.02 &&
	 muon.muonhits>0 &&
	 muon.normchi2<10 &&
	 muon.trackerhits>=11 &&
	 muon.muonstations > 1 &&
	 muon.pixelhits >= 1 &&
	 fabs(muon.vz - PVz) < 1.
	 )
      {
        double deltaR = 999.;
	double ptrel = -99;
        TLorentzVector tmpp4Mu;
        tmpp4Mu.SetPtEtaPhiE(muon.pt, muon.eta, muon.phi, muon.e );

        for ( vector< TopJetEvent>::iterator ijet=jets.begin(); ijet != jets.end(); ++ijet)
          {

            TopJetEvent jet = *ijet;

            if ( jet.pt > 50. && fabs(jet.eta) < 2.4 )
              {

                TLorentzVector tmpp4Jet;
                tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e );
                double tmpdeltaR = tmpp4Mu.DeltaR(tmpp4Jet);
		if ( tmpdeltaR < 0.3 ) continue;

                if ( tmpdeltaR < deltaR ) {
		  deltaR = tmpdeltaR;
		  ptrel = tmpp4Mu.Perp( tmpp4Jet.Vect() );
		}
              } // jet ID                                                                                                                                                                                           
          } // jet loop                                                                                                                                                                                             
        _deltaR = deltaR; // store value                                                                                                                                                                            
        if ( deltaR > 0.5 && ptrel > 25. ) pass = true;

      } // muon ID
    return pass;
  }



  bool MuonRelax02IsoQCD( TopMuonEvent muon, float PVz, vector< TopJetEvent > jets )
  {

    bool pass = false;

    if ( muon.pt > 35. &&
	 fabs(muon.eta)<2.1 &&
	 muon.IsTrackerMuon==1 &&
	 fabs(muon.d0)<0.02 &&
	 muon.muonhits>0 &&
	 muon.normchi2<10 &&
	 muon.trackerhits>=11 &&
	 muon.reliso03 < 0.5 && // HERE
	 muon.reliso03 > 0.2 && // and HERE
	 muon.muonstations > 1 &&
	 muon.pixelhits >= 1 &&
	 fabs(muon.vz - PVz) < 1.
	 )
      {
        double deltaR = 999.;
        TLorentzVector tmpp4Mu;
        tmpp4Mu.SetPtEtaPhiE(muon.pt, muon.eta, muon.phi, muon.e );
	
        for ( vector< TopJetEvent>::iterator ijet=jets.begin(); ijet != jets.end(); ++ijet)
          {
	    
            TopJetEvent jet = *ijet;

            if ( jet.pt > 35. && fabs(jet.eta) < 2.4 )
              {

                TLorentzVector tmpp4Jet;
                tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e );
                double tmpdeltaR = tmpp4Mu.DeltaR(tmpp4Jet);
                if ( tmpdeltaR < deltaR ) deltaR = tmpdeltaR;
		
              } // jet ID                                                                                                                                                                                                             
          } // jet loop                                                                                                                                                                                                               
        _deltaR = deltaR; // store value                                                                                                                                                                                              
        if ( deltaR > 0.3 ) pass = true;

      } // muon ID                                                                                                                                                                                                                    
    return pass;

  }

private:

  int _version;
  float _Pz;
  double _deltaR;
};

