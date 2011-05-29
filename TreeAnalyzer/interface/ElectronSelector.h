
/**_________________________________________________________________
   class:   ElectronSelector.cc

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: ElectronSelector.cc,v 1.2 2010/10/08 13:34:27 yumiceva Exp $

 ________________________________________________________________**/
#include<iostream>
#include "Yumiceva/Top7TeV/interface/TopEventNtuple.h"
#include "TLorentzVector.h"

#include<vector>

class ElectronSelector {

public:
  ElectronSelector() 
  { 
    _version = 4;
    _Pz = 0.;
  }

  void SetPz( double Pz) { _Pz = Pz; }
  
  virtual ~ElectronSelector() {}

  void Version(int version) {
    _version = version;
  }

  bool ElectronLoose( TopElectronEvent electron ) {
    
    bool pass = false;

    if ( electron.reliso03<0.2 ) pass = true;

    return pass;

  }

  bool ElectronTight( TopElectronEvent electron, float PVz ) {
    bool pass = false;
    
    float eta_sc = electron.etasc;

    if (electron.pt>30. &&
	( (eta_sc > -2.5 && eta_sc < -1.566) || (fabs(eta_sc)<1.4442) || (eta_sc > 1.566 && eta_sc< 2.5) ) &&
        fabs(electron.d0)<0.02 &&
        electron.pass70==1 &&
	electron.reliso03 < 0.1 &&
	fabs(electron.vz - PVz) < 1.
        ) pass = true;
    
    return pass;
  }

  bool ElectronZveto( TLorentzVector p4ele, vector<TopElectronEvent> *collection ) {

    bool isZevent = false;
    
    for (size_t iele = 0; iele < collection->size(); ++iele) 
      {
	TopElectronEvent tmpele = (*collection)[iele];

	float eta_sc = tmpele.etasc;
	bool pass95 = tmpele.pass95;

	TLorentzVector tmpp4ele;
	tmpp4ele.SetPtEtaPhiE( tmpele.pt, tmpele.eta, tmpele.phi, tmpele.e);

	if ( tmpp4ele.Et()>20. &&
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

};

