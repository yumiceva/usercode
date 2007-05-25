#ifndef TopWSolver_h
#define TopWSolver_h

/**_________________________________________________________________
   class:   TopWSolver.h
   package: Analyzers/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopJetComb.h,v 1.1 2007/04/25 03:53:54 yumiceva Exp $

________________________________________________________________**/

#include "TLorentzVector.h"


class TopWSolver {

  public:

	TopWSolver();
	~TopWSolver();

	void SetP4Muon( TLorentzVector muon ) { p4Muon_ = muon; };
	void SetP4MET( TLorentzVector met ) { p4MET_ = met; };

	Double_t GetPzNu(int type=0);
	bool IsComplex() { return complex_; };

  private:

	bool complex_;
	Double_t pzNu_;
	Int_t type_;
	TLorentzVector p4Muon_;
	TLorentzVector p4MET_;
	
};

#endif

	
