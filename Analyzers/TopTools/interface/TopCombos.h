#ifndef TopCombos_h
#define TopCombos_h

/**_________________________________________________________________
   class:   TopCombos.h
   package: Analyzers/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopCombos.h,v 1.2 2007/05/31 21:29:07 yumiceva Exp $

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

