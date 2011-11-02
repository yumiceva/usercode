/**_________________________________________________________________
   class:   JetCombinatorics.cc
   package: Analyzer/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: JetCombinatorics.cc,v 1.1 2011/09/01 19:31:55 yumiceva Exp $

________________________________________________________________**/


#include "Yumiceva/TreeAnalyzer/interface/JetCombinatorics.h"
#include "TMath.h"

#include <iostream>
#include "Yumiceva/TreeAnalyzer/interface/combination.h"

std::string itoa(int i) {
  char temp[20];
  sprintf(temp,"%d",i);
  return((std::string)temp);
}



//_______________________________________________________________
JetCombinatorics::JetCombinatorics() {

	this->Clear();
		
	minMassLepW_ = -999999.;
	maxMassLepW_ = 999999.;
	minMassHadW_ = -999999.;
	maxMassHadW_ = 999999.;
	minMassLepTop_ = -999999.;
	maxMassLepTop_ = 999999.;
	
	minPhi_ = -1.;
	removeDuplicates_ = true;
	maxNJets_ = 9999;
	verbosef = false;
	UsebTagging_ = false;
	UseMtop_ = true;
	SigmasTypef = 0;
	UseFlv_ = false;
	
	Template4jCombos_ = NestedCombinatorics(); // 12 combinations
	Template5jCombos_ = Combinatorics(4,5); // 5 combinations of 4 combos
	Template6jCombos_ = Combinatorics(4,6); // 15 combinations of 4 combos
	Template7jCombos_ = Combinatorics(4,7); // xx combinations of 4 combos
	
}

//_______________________________________________________________
JetCombinatorics::~JetCombinatorics() {
  this->Clear();
}

//_______________________________________________________________
void JetCombinatorics::Clear() {

	allCombos_.clear();
	allCombosSumEt_.clear();
	allCombosDeltaR_.clear();
	//Template4jCombos_.clear();
	//Template5jCombos_.clear();
	//Template6jCombos_.clear();
	//Template7jCombos_.clear();
	//cand1_.clear();
	
}
	

//_______________________________________________________________
std::map< int, std::string > JetCombinatorics::Combinatorics(int n, int max) {

	// find a combinatorics template
	// This is a simple stupid function to make algebratic combinatorics

	int kcombos = n;
	int maxcombos = max;

	std::string list;

	for ( int m=0; m<maxcombos; m++) { list = list + (itoa(m));}

	std::string seed;
	for ( int m=0; m<kcombos; m++) { seed = seed + (itoa(m));}

	
	std::map< int, std::string > aTemplateCombos;
	aTemplateCombos.clear();
	
	aTemplateCombos[0] = seed;

	int i = 0;
	int totalmatches = seed.size();
	int totalIte = list.size();

	for ( int ite = 0; ite < ((int)totalIte); ite++) {

		//cout << "iteration " << ite << endl;
		//i = 0;
		//for ( Itevec = seed.begin(); Itevec != seed.end(); ++Itevec) {
		for ( i=0; i< (int) totalmatches; i++) {

			std::string newseed = aTemplateCombos[ite];
			std::string newseed2;
			/*
			cout << "newseed size= " << newseed.size() << " : ";
			for (std::vector< std::string>::iterator iite = newseed.begin();
				 iite != newseed.end(); ++iite) {

				cout << *iite << " ";
			}
			cout << endl;
			*/
			for ( int itemp=0; itemp<(int)newseed.size(); itemp++) {
				if (itemp!=i) newseed2 = newseed2 + (newseed[itemp]);
			}
			/*
			cout << "newseed2: ";
			for (std::vector< std::string>::iterator iite = newseed2.begin();
				 iite != newseed2.end(); ++iite) {

				cout << *iite << " ";
			}
			cout << endl;
			*/
			for ( int j=0; j<(int) list.size(); j++) {
				//cout << " j = " << j << endl;
				bool Isnewelement = true;
				std::string newelement = "0";
				//bool Isnewcombo = true;
				for (int k=0; k< (int)newseed2.size(); k++) {
					if ( list[j] == newseed2[k] ) Isnewelement = false;
				}
				if (Isnewelement) {

					newelement = list[j];
					//cout << "new element: " << newelement << endl;

					std::string candseed = newseed2;
					candseed = candseed + newelement;

					bool IsnewCombo = true;
					for (int ic=0; ic<(int)aTemplateCombos.size(); ++ic ) {

						int nmatch = 0;
						for ( int ij=0; ij<(int)(aTemplateCombos[ic]).size(); ij++) {

							for (int ik=0; ik<(int)candseed.size(); ik++) {
								if ( candseed[ik] == aTemplateCombos[ic][ij] ) nmatch++;
							}
						}
						if (nmatch == (int)totalmatches)
							IsnewCombo = false;

					}
					if (IsnewCombo) {
						//cout << "new combo"<< " before combo size=" << aTemplateCombos.size() << endl;
						aTemplateCombos[(int)aTemplateCombos.size()] = candseed;
						//cout << " after size = " << aTemplateCombos.size() << endl;
					}
				}

			}
		}
	}//close iterations

	// debug info
	
	//std::cout << " build templates for total combos = " << aTemplateCombos.size() << std::endl;
	//std::cout << " template combos: " << std::endl;
	//for (size_t ic=0; ic != aTemplateCombos.size(); ++ic) {

	//std::cout << aTemplateCombos[ic] << std::endl;
	//}
	
	return aTemplateCombos;
	
	
	
}


//______________________________________________________________
std::map< int, std::string > JetCombinatorics::NestedCombinatorics() {

	// build by hand 12 combinations for semileptonic top decays

	std::map< int, std::string > aTemplateCombos;
	aTemplateCombos.clear();
	
	aTemplateCombos[0] = "0123";
	aTemplateCombos[1] = "0132";
	aTemplateCombos[2] = "0213";
	aTemplateCombos[3] = "0231";
	aTemplateCombos[4] = "0312";
	aTemplateCombos[5] = "0321";
	aTemplateCombos[6] = "1203";
	aTemplateCombos[7] = "1230";
	aTemplateCombos[8] = "1302";
	aTemplateCombos[9] = "1320";
	aTemplateCombos[10] = "2301";
	aTemplateCombos[11] = "2310";
		
	return aTemplateCombos;

}

//______________________________________________________________
void JetCombinatorics::FourJetsCombinations(std::vector<TLorentzVector> *jets, std::vector<int> *btags ) {


	int n = 0; // total number of combos
	std::map< Combo, int, minChi2 > allCombos;
	std::map< Combo, int, maxSumEt > allCombosSumEt;
	std::map< Combo, int, minDeltaR > allCombosDeltaR;

	for ( int iw = 0; iw < 2; iw++ ) 
	  { // loop over two neutrino solutions                                                                                                                                                     
	  
	    TLorentzVector aLepW;

	    if ( iw == 0 ) aLepW = theLepW_;
	    else aLepW = theOtherLepW_;
	    
	    if ( aLepW.E() == 0 ) continue;
	    
	    if (verbosef) {
	      std::cout <<"[JetCombinatorics] Neutrino hypothesis # " << iw << std::endl;
	      std::cout<< "[JetCombinatorics] with these 4 jets, make 12 combinations: " <<std::endl;
	    }

	    std::vector<TLorentzVector> tmpFourJetCombo;
	    for( std::vector<TLorentzVector>::iterator itejet = jets->begin(); itejet != jets->end(); ++itejet )
	      tmpFourJetCombo.push_back( *itejet );
	    
	    do 
	      {

		Combo acombo;
		
		acombo.SetWp( tmpFourJetCombo[0] );
		acombo.SetWq( tmpFourJetCombo[1] );
		acombo.SetHadb( tmpFourJetCombo[2] );
		acombo.SetLepb( tmpFourJetCombo[3] );
		acombo.SetLepW( aLepW );
		
		//acombo.SetIdWp( the4Ids[atoi((a4template.substr(0,1)).c_str())] );
		//acombo.SetIdWq( the4Ids[atoi((a4template.substr(1,1)).c_str())] );
		//acombo.SetIdHadb( the4Ids[atoi((a4template.substr(2,1)).c_str())] );
		//acombo.SetIdLepb( the4Ids[atoi((a4template.substr(3,1)).c_str())] );
		
		acombo.UseMtopConstraint(UseMtop_);
		// choose value of sigmas                                                                                                                                                                                                      
		acombo.SetSigmas(SigmasTypef);

		acombo.analyze();

		if (verbosef) {

		  std::cout << "[JetCombinatorics] ==> combination done:" << std::endl;
		  acombo.Print();
		}


		// invariant mass cuts                                                                                                                                                                                                         
		TLorentzVector aHadWP4 = acombo.GetHadW();
		TLorentzVector aLepWP4 = acombo.GetLepW();
		TLorentzVector aLepTopP4=acombo.GetLepTop();

		if ( ( aHadWP4.M() > minMassHadW_ && aHadWP4.M() < maxMassHadW_ ) &&
		     ( aLepWP4.M() > minMassLepW_ && aLepWP4.M() < maxMassLepW_ ) &&
		     ( aLepTopP4.M() > minMassLepTop_ && aLepTopP4.M() < maxMassLepTop_) ) {

		  allCombos[acombo] = n;
		  allCombosSumEt[acombo] = n;
		  allCombosDeltaR[acombo] = n;
		  n++;
		}

	      }
	    while (stdcomb::next_combination( jets->begin(), jets->end(), tmpFourJetCombo.begin(), tmpFourJetCombo.end() ) );

	  }

	allCombos_ = allCombos;
	allCombosSumEt_ = allCombosSumEt;
	allCombosDeltaR_ = allCombosDeltaR;
	allCombos.clear();
	allCombosSumEt.clear();
	allCombosDeltaR.clear();

}

Combo JetCombinatorics::GetCombination(int n) {

	int j = 0;
	Combo a;
	
	if ( ! UsebTagging_ ) {

	  for ( std::map<Combo,int,minChi2>::const_iterator ite=allCombos_.begin();
		ite!=allCombos_.end(); ++ite) {

		if (j == n) a = ite->first;
		j++;

	  }

	  
	} else {


	  for ( std::map<Combo,int,minChi2>::const_iterator ite=allCombos_.begin();
		ite!=allCombos_.end(); ++ite) {

	    Combo tmp = ite->first;
	    if ( tmp.IsGoodbTagEvent() ) {
	    
	      if (j == n) a = tmp;
	      j++;
	    }
	  
	  }
	}

	return a;

	
}

Combo JetCombinatorics::GetCombinationSumEt(int n) {

	int j = 0;
	Combo a;
	for ( std::map<Combo,int,maxSumEt>::const_iterator ite=allCombosSumEt_.begin();
		  ite!=allCombosSumEt_.end(); ++ite) {
		
		if (j == n) a = ite->first;
		j++;
	}

	return a;

	
}

Combo JetCombinatorics::GetCombinationDeltaR(int n) {

  int j = 0;
  Combo a;
  for ( std::map<Combo,int,minDeltaR>::const_iterator ite=allCombosDeltaR_.begin();
	ite!=allCombosDeltaR_.end(); ++ite) {

    if (j == n) a = ite->first;
    j++;
  }

  return a;


}
