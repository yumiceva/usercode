/**_________________________________________________________________
   class:   TopTwoComb.cc
   package: Analyzer/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopTwoComb.cc,v 1.1 2007/05/25 19:47:13 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/TopTools/interface/TopTwoComb.h"

#include "TMath.h"
#include <iostream>

//_______________________________________________________________
TopTwoComb::TopTwoComb() {

	this->Clear();

	minInvMass_ = -99999.;
	maxInvMass_ = 99999.;
	minPhi_ = 0.;
	
}

//_______________________________________________________________
TopTwoComb::~TopTwoComb() {
}

//_______________________________________________________________
void TopTwoComb::Clear() {
	cand1_.clear();
	cand2_.clear();
}

//_______________________________________________________________
std::vector< TLorentzVector > TopTwoComb::GetComposites() {

	std::vector< TLorentzVector > composites;

	std::cout << "TC: cand1="<< cand1_.size() << " cand2=" << cand2_.size() << std::endl;
	for ( unsigned icand1 = 0; icand1 < cand1_.size(); ++icand1 ) {

		for ( unsigned icand2 = 0; icand2 < cand2_.size(); ++icand2 ) {

			TLorentzVector cand1 = cand1_[icand1];
			TLorentzVector cand2 = cand2_[icand2];
			
			if ( cand2 != cand1 ) {

				TLorentzVector acomp = cand1 + cand2;
				std::cout << "TC: mass= " << acomp.M() << std::endl;
				
				if ( acomp.M() > minInvMass_ && acomp.M() < maxInvMass_ ) {


					Double_t adelta = TMath::Abs(cand1.Phi() - cand2.Phi());
					if ( adelta > TMath::Pi() ) adelta = TMath::Abs(2.*TMath::Pi() - adelta);
					
					if ( adelta > minPhi_ ) composites.push_back(acomp);
				}
			}
		}
	}

	return composites;

}

