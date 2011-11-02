
/**_________________________________________________________________
   class:   ElectronSelector.cc

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: ElectronSelector.h,v 1.3 2011/07/29 23:12:37 yumiceva Exp $

 ________________________________________________________________**/
#include<iostream>
#include "Yumiceva/Top7TeV/interface/TopEventNtuple.h"
#include "TLorentzVector.h"

#include<vector>

using namespace std;

class ElectronSelector {

public:
  ElectronSelector() 
  { 
    _version = 4;
    _Pz = 0.;
    _deltaR = 9999;
    _ptrel = -99;
    _elepT = 35.;
    _jetpT = 35.;
  }

  void SetElectronPt(float pT) { _elepT = pT; }
  void SetJetPt(float pT) { _jetpT = pT; }

  double GetDeltaR() { return _deltaR; }
  double GetPtrel() { return _ptrel;}

  void SetPz( double Pz) { _Pz = Pz; }
  
  virtual ~ElectronSelector() {}

  void Version(int version) {
    _version = version;
  }

  bool ElectronLoose( TopElectronEvent electron ) {
    
    bool pass = false;

    if ( electron.pt > 15. && 
	 fabs(electron.eta) < 2.5 &&
	 electron.reliso03< 0.2 ) pass = true;

    return pass;

  }

  bool ElectronNoIsoTight(TopElectronEvent electron, float PVz, vector< TopJetEvent > jets ) {
    bool pass = false;

    float eta_sc = electron.etasc;

    if (electron.pt > _elepT &&
        ( (eta_sc > -2.5 && eta_sc < -1.566) || (fabs(eta_sc)<1.4442) || (eta_sc > 1.566 && eta_sc< 2.5) ) &&
	fabs(electron.d0)<0.02 &&
        electron.pass70==1 &&
        fabs(electron.vz - PVz) < 1.
        ) {

      double deltaR = 999.;
      double ptrel = -99;
      TLorentzVector tmpp4Ele;
      tmpp4Ele.SetPtEtaPhiE(electron.pt, electron.eta, electron.phi, electron.e );

      for ( vector< TopJetEvent>::iterator ijet=jets.begin(); ijet != jets.end(); ++ijet)
	{

	  TopJetEvent jet = *ijet;

	  if ( jet.pt > _jetpT && fabs(jet.eta) < 2.4 )
	    {
	      TLorentzVector tmpp4Jet;
	      tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e );
	      double tmpdeltaR = tmpp4Ele.DeltaR(tmpp4Jet);
	      if ( tmpdeltaR < 0.3 ) continue;
	      if ( tmpdeltaR < deltaR ) {
		deltaR = tmpdeltaR;
		ptrel = tmpp4Ele.Perp( tmpp4Jet.Vect() );
	      }
	    }// jet ID
	}// jet loop
      
      _deltaR = deltaR;
      _ptrel = ptrel;

      if ( deltaR > 0.5 && ptrel > 25. ) pass= true;
    }

    return pass;
  }

  bool ElectronTight( TopElectronEvent electron, float PVz ) {
    bool pass = false;
    
    float eta_sc = electron.etasc;

    if (electron.pt > _elepT &&
	( (eta_sc > -2.5 && eta_sc < -1.566) || (fabs(eta_sc)<1.4442) || (eta_sc > 1.566 && eta_sc< 2.5) ) &&
        fabs(electron.d0)<0.02 &&
        electron.pass70==1 &&
	electron.reliso03 < 0.1 &&
	fabs(electron.vz - PVz) < 1.
        ) pass = true;
    
    return pass;
  }

  bool ElectronZveto( TLorentzVector p4ele, vector<TopElectronEvent> electrons ) {

    bool isZevent = false;
    
    for ( vector<TopElectronEvent>::iterator jelectron = electrons.begin(); jelectron != electrons.end(); ++jelectron)
      {
	TopElectronEvent tmpele = *jelectron;

	float eta_sc = tmpele.etasc;
	bool pass95 = tmpele.pass95;

	TLorentzVector tmpp4ele;
	tmpp4ele.SetPtEtaPhiE( tmpele.pt, tmpele.eta, tmpele.phi, tmpele.e);

	if ( tmpp4ele.Et()> _elepT &&
	     ( (eta_sc > -2.5 && eta_sc < -1.566) || (fabs(eta_sc)<1.4442) || (eta_sc > 1.566 && eta_sc< 2.5) ) &&
	     tmpele.reliso03<1.0 &&
	     fabs(eta_sc)<2.5 ) {

	  if (pass95) {
	    TLorentzVector p4secondElec;
	    p4secondElec.SetPtEtaPhiE( tmpele.pt, tmpele.eta, tmpele.phi, tmpele.e);
	    //nloose95electrons += 1
	    TLorentzVector Zp4 = (p4ele + p4secondElec);
	    float Zmass = Zp4.M();
	    if ( Zmass >= 76. and Zmass <= 106. )
	      isZevent = true;

	  }
	}
      }

    return isZevent;
  }
private:

  int _version;
  float _Pz;
  double _deltaR;
  double _ptrel;
  float _elepT;
  float _jetpT;
};

