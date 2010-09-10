#ifndef METzCalculator_h
#define METzCalculator_h

/**_________________________________________________________________
   class:   METzCalculator.h

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: METzCalculator.h,v 1.1 2010/09/03 16:19:12 yumiceva Exp $

________________________________________________________________**/
#include<iostream>
#include "TLorentzVector.h"

class METzCalculator {

  public:
	/// constructor
	METzCalculator();
		//METzCalculator(const edm::ParameterSEt& iConf);
	/// destructor
	virtual ~METzCalculator();
	/// Set MET
	void SetMET(TLorentzVector MET) {
	  MET_ = MET;
	}
    /// Set Muon
	void SetMuon(TLorentzVector lepton) {
	  lepton_ = lepton;
	}
    /// Calculate MEz
	/// options to choose roots from quadratic equation:
	/// type = 0 (defalut): if real roots, pick the one nearest to
	///                     the lepton Pz except when the Pz so chosen
	///                     is greater than 300 GeV in which case pick
	///                     the most central root.
	/// type = 1: if real roots, choose the one closest to the lepton Pz
	///           if complex roots, use only the real part.
	/// type = 2: if real roots, choose the most central solution.
	///           if complex roots, use only the real part.
	/// type = 3: if real roots, pick the largest value of the cosine*
	double Calculate(int type = 0);
    /// check for complex root
	bool IsComplex() const { return isComplex_; };
	double getOther() const { return otherSol_; };
	void Print() {
		std::cout << " METzCalculator: pxmu = " << lepton_.Px() << " pzmu= " << lepton_.Pz() << std::endl;
		std::cout << " METzCalculator: pxnu = " << MET_.Px() << " pynu= " << MET_.Py() << std::endl;
	}
	
  private:

	bool isComplex_;
	TLorentzVector lepton_;
	TLorentzVector MET_;
	double otherSol_;
	
};

#endif
