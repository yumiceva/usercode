/**_________________________________________________________________
   class:   TopCombos.cc
   package: Analyzer/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopCombos.cc,v 1.2 2007/06/04 16:02:04 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/TopTools/interface/TopCombos.h"

#include "TMath.h"
#include <iostream>

//_______________________________________________________________
TopCombos::TopCombos() {

	this->Clear();

	minInvMass_ = -99999.;
	maxInvMass_ = 99999.;
	minPhi_ = 0.;
	
}

//_______________________________________________________________
TopCombos::~TopCombos() {
}

//_______________________________________________________________
void TopCombos::Clear() {
	cand1_.clear();
	cand2_.clear();
	cand3_.clear();
}


//_______________________________________________________________
std::vector< TLorentzVector > TopCombos::GetComposites() {

	if ( cand1_.size() == 0 || cand2_.size() == 0 ) {

		std::cout << "TopCombos: empty input vector, size 1= " << cand1_.size() << " size 2= " << cand2_.size() << std::endl;
		return this->TwoCombos();
	}

	if ( cand3_.size() == 0 ) {

		return this->TwoCombos();
		
	} else {

		return this->ThreeCombos();
	}
}

//_______________________________________________________________
std::vector< TLorentzVector > TopCombos::TwoCombos() {

	std::vector< TLorentzVector > composites;

	//std::cout << "TC: cand1="<< cand1_.size() << " cand2=" << cand2_.size() << std::endl;
	for ( unsigned icand1 = 0; icand1 < cand1_.size(); ++icand1 ) {

		for ( unsigned icand2 = icand1; icand2 < cand2_.size(); ++icand2 ) {

			TLorentzVector cand1 = cand1_[icand1];
			TLorentzVector cand2 = cand2_[icand2];
			
			if ( cand2 != cand1 ) {

				TLorentzVector acomp = cand1 + cand2;
				//std::cout << "TC: mass= " << acomp.M() << std::endl;
				
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


//_______________________________________________________________
std::vector< TLorentzVector > TopCombos::ThreeCombos() {

	std::vector< TLorentzVector > composites;

	
	for ( unsigned icand1 = 0; icand1 < cand1_.size(); ++icand1 ) {

		for ( unsigned icand2 = icand1; icand2 < cand2_.size(); ++icand2 ) {

			TLorentzVector cand1 = cand1_[icand1];
			TLorentzVector cand2 = cand2_[icand2];
						
			if ( cand2 != cand1 ) {
				for ( unsigned icand3 = 0; icand3 < cand3_.size(); ++icand3 ) {
					
					TLorentzVector cand3 = cand3_[icand3];
			
					if ( cand3 != cand1 && cand3 != cand2 ) {

						TLorentzVector acomp12 = cand1 + cand2;
						TLorentzVector acomp123 = cand1 + cand2 + cand3;
						
				
						if ( acomp12.M() > minInvMass_ && acomp12.M() < maxInvMass_ ) {


							Double_t adelta = TMath::Abs(acomp12.Phi() - cand3.Phi());
							if ( adelta > TMath::Pi() ) adelta = TMath::Abs(2.*TMath::Pi() - adelta);
					
							if ( adelta > minPhi_ ) {

							  // check for duplicates
							  bool isduplicate = false;
							  for ( unsigned ic = 0; ic < composites.size(); ++ic) {
							    //std::cout << "ci comp M= " << composites[ic].M() << std::endl;
							    if ( TMath::Abs(composites[ic].M() - acomp123.M())<1.0e-4) { isduplicate = true; break; }

							  }
							  //std::cout << "composites M= " << acomp123.M() << std::endl;
							  if (!isduplicate) composites.push_back(acomp123);
							}
						}
					}
				}
			}
		}
	}

	return composites;

}
