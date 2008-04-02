#ifndef TopEventProducers_MEzCalculator_h
#define TopEventProducers_MEzCalculator_h

/**_________________________________________________________________
   class:   MEzCalculator.h

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: MEzCalculator.h,v 1.1 2008/03/17 15:26:17 yumiceva Exp $

________________________________________________________________**/

//#include "FWCore/Framework/interface/Event.h"
//#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Particle.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "TLorentzVector.h"

class MEzCalculator {

  public:
	/// constructor
	MEzCalculator();
		//MEzCalculator(const edm::ParameterSEt& iConf);
	/// destructor
	~MEzCalculator();
	/// Set MET
	void SetMET(const pat::MET &MET) { MET_ = MET; } ;
	void SetMET(TLorentzVector MET) {
		pat::Particle::LorentzVector p(MET.Px(),MET.Py(),MET.Pz(),MET.E());
		MET_.setP4(p);
	}
    /// Set Muon
	void SetMuon(const pat::Particle &lepton) { lepton_ = lepton; };
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
	double Calculate(int type = 0);
    /// check for complex root
	bool IsComplex() const { return isComplex_; };
		
  private:

	bool isComplex_;
	pat::Particle lepton_;
	pat::MET MET_;
	
};

#endif
