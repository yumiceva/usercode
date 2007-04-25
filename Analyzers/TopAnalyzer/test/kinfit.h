#ifndef kinfit_h
#define kinfit_h

#include "TMatrixD.h"
#include "TLorentzVector.h"

class kinfit {

  public:

	kinfit();
	virtual ~kinfit();

	void iteration(int i, int nf, int nm, int ny,
				   TMatrixD f, TMatrixD dfdm, TMatrixD dfdy, TMatrixD C, TMatrixD GI);

	void Fit();

	void SetPlepton(TLorentzVector pv) { pl_ = pv; }
	void SetPbjet1(TLorentzVector pv) { pb1_ = pv; }
	void SetPbjet2(TLorentzVector pv) { pb2_ = pv; }
	void SetPjet1(TLorentzVector pv) { pj1_ = pv; }
	void SetPjet2(TLorentzVector pv) { pj2_ = pv; }

	void SetEleptonErr(Double_t err) { elErr_ = err; }
	void SetEbjet1Err(Double_t err) { eb1Err_ = err; }
	void SetEbjet2Err(Double_t err) { eb2Err_ = err; }
	void SetEjet1Err(Double_t err) { ej1Err_ = err; }
	void SetEjet2Err(Double_t err) { ej2Err_ = err; }

	Double_t GetFitNuPz() { return fitNuPz_; }
	Double_t GetFitNuPzErr() { return fitNuPzErr_; }
	Double_t GetFitEbjet1() { return fitEbjet1_; }
	Double_t GetFitEbjet1Err() { return fitEbjet1Err_; }
	Double_t GetFitEbjet2() { return fitEbjet2_; }
	Double_t GetFitEbjet2Err() { return fitEbjet2Err_; }
	Double_t GetFitEjet1() { return fitEjet1_; }
	Double_t GetFitEjet1Err() { return fitEjet1Err_; }
	Double_t GetFitEjet2() { return fitEjet2_; }
	Double_t GetFitEjet2Err() { return fitEjet2Err_; }
		
  private:

	TLorentzVector pl_;
	TLorentzVector pb1_;
	TLorentzVector pb2_;
	TLorentzVector pj1_;
	TLorentzVector pj2_;

	Double_t elErr_;
	Double_t eb1Err_;
	Double_t eb2Err_;
	Double_t ej1Err_;
	Double_t ej2Err_;
	
	Double_t fitNuPz_;
	Double_t fitNuPzErr_;
	Double_t fitEbjet1_;
	Double_t fitEbjet1Err_;
	Double_t fitEbjet2_;
	Double_t fitEbjet2Err_;
	Double_t fitEjet1_;
	Double_t fitEjet1Err_;
	Double_t fitEjet2_;
	Double_t fitEjet2Err_;
	
	
	TMatrixD Cp_;
	TMatrixD chi2_;
	TMatrixD GmI_;

};

#endif

