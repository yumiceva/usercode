#ifndef TopCombos_h
#define TopCombos_h

/**_________________________________________________________________
   class:   TopCombos.h
   package: Analyzers/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopCombos.h,v 1.1 2007/06/04 15:54:20 yumiceva Exp $

________________________________________________________________**/

#include "TLorentzVector.h"


class TopCombos {

  public:

	TopCombos();
	~TopCombos();

	void SetCandidate1( std::vector< TLorentzVector > candidates ) {
		cand1_ = candidates;
	}
	void SetCandidate2( std::vector< TLorentzVector > candidates ) {
		cand2_ = candidates;
	}
	void SetCandidate3( std::vector< TLorentzVector > candidates ) {
		cand3_ = candidates;
	}
	
	void SetMinInvMass( Double_t mass ) { minInvMass_ = mass; }
	void SetMaxInvMass( Double_t mass ) { maxInvMass_ = mass; }
	void SetMinDeltaPhi( Double_t angle ) { minPhi_ = angle; }
	
	std::vector< TLorentzVector > GetComposites();
	std::vector< TLorentzVector > TwoCombos();
	std::vector< TLorentzVector > ThreeCombos();
	
	void Clear();
	
  private:
	Double_t minInvMass_;
	Double_t maxInvMass_;
	Double_t minPhi_;

	std::vector< TLorentzVector > cand1_;
	std::vector< TLorentzVector > cand2_;
	std::vector< TLorentzVector > cand3_;
	
};

#endif

