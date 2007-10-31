#ifndef TopCombos_h
#define TopCombos_h

/**_________________________________________________________________
   class:   TopCombos.h
   package: Analyzers/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopCombos.h,v 1.2 2007/06/04 16:02:03 yumiceva Exp $

________________________________________________________________**/

#include "TopComposite.h"
#include "TopPair.h"
#include "TLorentzVector.h"
#include "GetResolutions.h"
#include<string>

class TopCombos {

  public:

	TopCombos();
	~TopCombos();

	void SetCandidate1( std::vector< TLorentzVector > candidates ) {
		cand1_ = candidates;
	}
	void SetCandidate1( std::vector< TopComposite > candidates ) {
		composites1_ = candidates;
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
	void SetFilterList( std::vector< std::string > list ) {
		filterlist_ = list;
	}
	std::vector< std::string > GetList() { return listcombos_; }
	std::vector< TopComposite > GetComposites();
	std::vector< TopPair > GetFittedTops() { return fittedTops_;}
	std::vector< TLorentzVector > GetVectors() { return vectors_; }
	std::vector< TopComposite > TwoCombosMixed();
	std::vector< TopComposite > TwoCombos();
	std::vector< TopComposite > ThreeCombos();
	
	void Clear();

	void RemoveDuplicates( bool option) { removeDuplicates_ = option; }
	void DoKinFit() { doKinFit_ = true; }
	void SetMuon( TLorentzVector muon) {
		muon_ = muon;
	}
	void SetNu( TLorentzVector nu ) {
		nu_ = nu;
	}
	void SetHadb( TLorentzVector bjet ) {
		candHadb_ = bjet;
	}
	void SetLepb( TLorentzVector bjet ) {
		candLepb_ = bjet;
	}
	void KinFit(TLorentzVector p4Jet0, TLorentzVector p4Jet1,
				TLorentzVector p4Jet2, TLorentzVector p4Jet3,
				TLorentzVector p4Muon, TLorentzVector p4Nu);
	void KinFit(TopPair *apair);
	void MyKinFit(TopPair *apair);

	std::string itoa(int i) {
		char temp[20];
		sprintf(temp,"%d",i);
		return((std::string)temp);
	};
	void SetResJets( GetResolutions *res ) { ResJets_ = res; }
	void SetResbJets( GetResolutions *res ) { ResbJets_ = res; }
	void SetResMuons( GetResolutions *res ) { ResMuons_ = res; }
	void SetResMET( GetResolutions *res ) { ResMET_ = res; }
	

  private:
	Double_t minInvMass_;
	Double_t maxInvMass_;
	Double_t minPhi_;
	double chi2_;
	int ndf_;
	bool removeDuplicates_;
	
	std::vector< TLorentzVector > cand1_;
	std::vector< TLorentzVector > cand2_;
	std::vector< TLorentzVector > cand3_;
	std::vector< TopComposite > composites1_;
	
	std::vector< TLorentzVector > vectors_;
	std::vector< TopComposite > composites_;
	std::vector< TopPair > fittedTops_;
	std::vector< std::string > listcombos_;
	std::vector< std::string > filterlist_;
	//std::map<int , TString> mapcomp_;
	bool doKinFit_;
	TLorentzVector muon_;
	TLorentzVector nu_;
	TLorentzVector candHadb_;
	TLorentzVector candLepb_;
	TLorentzVector fittedcand_;

	GetResolutions *ResJets_;
	GetResolutions *ResbJets_;
	GetResolutions *ResMuons_;
	GetResolutions *ResMET_;
	
};

#endif

