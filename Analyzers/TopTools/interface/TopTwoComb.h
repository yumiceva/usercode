#ifndef TopTwoComb_h
#define TopTwoComb_h

/**_________________________________________________________________
   class:   TopTwoComb.h
   package: Analyzers/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopJetComb.h,v 1.1 2007/04/25 03:53:54 yumiceva Exp $

________________________________________________________________**/

#include "TLorentzVector.h"


class TopTwoComb {

  public:

	TopTwoComb();
	~TopTwoComb();

	void SetCandidate1( std::vector< TLorentzVector > candidates ) {
		cand1_ = candidates;
	}
	void SetCandidate2( std::vector< TLorentzVector > candidates ) {
		cand1_ = candidates;
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

	
};

#endif

