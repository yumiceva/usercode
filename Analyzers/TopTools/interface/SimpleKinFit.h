#ifndef SimpleKinFit_h
#define SimpleKinFit_h

#include "TMatrixD.h"
#include "TLorentzVector.h"


class SimpleKinFit {

  public:

	SimpleKinFit();
	virtual ~SimpleKinFit();

	void iteration(int i, int nf, int nm, int ny,
				   TMatrixD f, TMatrixD dfdm, TMatrixD dfdy, TMatrixD C, TMatrixD GI);

	void Fit();

	void SetPl(TLorentzVector pv) { pl_ = pv; }
	void SetPbj1(TLorentzVector pv) { pb1_ = pv; }
	void SetPbj2(TLorentzVector pv) { pb2_ = pv; }
	void SetPj1(TLorentzVector pv) { pj1_ = pv; }
	void SetPj2(TLorentzVector pv) { pj2_ = pv; }
	void SetMET(TLorentzVector pv) { p4met_ = pv; }
	void SetPzNu(Double_t pznu) { pznu_ = pznu; }
	
	void SetElErr(Double_t err) { elErr_ = err; }
	void SetEbj1Err(Double_t err) { eb1Err_ = err; }
	void SetEbj2Err(Double_t err) { eb2Err_ = err; }
	void SetEj1Err(Double_t err) { ej1Err_ = err; }
	void SetEj2Err(Double_t err) { ej2Err_ = err; }

	Double_t GetFitNuPz() { return fitNuPz_; }
	Double_t GetFitNuPzErr() { return fitNuPzErr_; }
	Double_t GetFitEbj1() { return fitEb1_; }
	Double_t GetFitEbj1Err() { return fitEb1Err_; }
	Double_t GetFitEbj2() { return fitEb2_; }
	Double_t GetFitEbj2Err() { return fitEb2Err_; }
	Double_t GetFitEj1() { return fitEj1_; }
	Double_t GetFitEj1Err() { return fitEj1Err_; }
	Double_t GetFitEj2() { return fitEj2_; }
	Double_t GetFitEj2Err() { return fitEj2Err_; }
	
	//Double_t GetPzNu(TLorentzVector p4Muon, TLorentzVector p4MET);

	Double_t GetChi2() {
		return chi_(0,0);
	}
  private:

	Double_t MW_;
	Double_t Mt_;
		
	TLorentzVector pl_;
	TLorentzVector pb1_;
	TLorentzVector pb2_;
	TLorentzVector pj1_;
	TLorentzVector pj2_;
	TLorentzVector p4met_;
	
	Double_t pznu_;
	
	Double_t elErr_;
	Double_t eb1Err_;
	Double_t eb2Err_;
	Double_t ej1Err_;
	Double_t ej2Err_;
	
	Double_t fitNuPz_;
	Double_t fitNuPzErr_;
	Double_t fitEb1_;
	Double_t fitEb1Err_;
	Double_t fitEb2_;
	Double_t fitEb2Err_;
	Double_t fitEj1_;
	Double_t fitEj1Err_;
	Double_t fitEj2_;
	Double_t fitEj2Err_;
	
	
	TMatrixD cp_;
	TMatrixD dx_;
	TMatrixD chi_;
	TMatrixD gxi_;
	TMatrixD gmfi_;

};

#endif

