#ifndef TopComposite_h
#define TopComposite_h

/**_________________________________________________________________
   class:   TopComposite.h
   package: Analyzers/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopComposite.h,v 1.2 2007/06/04 16:02:03 yumiceva Exp $

________________________________________________________________**/

#include "TLorentzVector.h"
#include<string>

class TopComposite {

  public:
	TopComposite(){
		cand0_(0);
		cand1_(0);
		cand2_(0);
		chi2_ = 0.;
		ndf_ = 0;
	};
	
	~TopComposite() {};

	void SetMon(int im, TLorentzVector vector) {
		if (im==0) cand0_ = vector;
		if (im==1) cand1_ = vector;
		if (im==2) cand2_ = vector;
	}
	TLorentzVector GetMon(int im) {
		if (im==0) return cand0_;
		if (im==1) return cand1_;
		if (im==2) return cand2_;
	}
	double M() {
		TLorentzVector tot = cand0_ + cand1_ + cand2_;
		return tot.M();
	}
	TLorentzVector P4() {
		TLorentzVector tot = cand0_ + cand1_ + cand2_;
		return tot;
	}
	void SetKFChi2(double chi2) {
		chi2_ = chi2;
	}
	void SetKFndf(int ndf) {
		ndf_ = ndf;
	}
	double GetKFChi2() { return chi2_; }
	int GetKFndf() { return ndf_; }
	
  private:
	TLorentzVector cand0_;
	TLorentzVector cand1_;
	TLorentzVector cand2_;
	double chi2_;
	int ndf_;
	
};
#endif
