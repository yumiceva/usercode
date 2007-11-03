/**_________________________________________________________________
   class:   TopCombos.cc
   package: Analyzer/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopCombos.cc,v 1.4 2007/10/31 19:34:56 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/TopTools/interface/TopCombos.h"

#include "TMatrixD.h"
#include "TMath.h"
#include <iostream>

// includes for kinematic fit
#include "PhysicsTools/KinFitter/interface/TFitParticleESpher.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleMCPInvSpher.h"
#include "PhysicsTools/KinFitter/interface/TAbsFitParticle.h"
#include "PhysicsTools/KinFitter/interface/TFitConstraintM.h"
#include "PhysicsTools/KinFitter/interface/TFitConstraintEp.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleESpher.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleSpher.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleEtEtaPhi.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleEMomDev.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleEScaledMomDev.h"
#include "PhysicsTools/KinFitter/interface/TKinFitter.h"

#include "Analyzers/TopTools/interface/SimpleKinFit.h"

//_______________________________________________________________
TopCombos::TopCombos() {

	this->Clear();

	minInvMass_ = -999999.;
	maxInvMass_ = 999999.;
	minPhi_ = -1.;
	doKinFit_ = false;
	muon_(0);
	nu_(0);
	candHadb_(0);
	candLepb_(0);
	removeDuplicates_ = true;
}

//_______________________________________________________________
TopCombos::~TopCombos() {
}

//_______________________________________________________________
void TopCombos::Clear() {
	cand1_.clear();
	cand2_.clear();
	cand3_.clear();
	composites_.clear();
	filterlist_.clear();
	listcombos_.clear();
	vectors_.clear();
	fittedTops_.clear();
}


//_______________________________________________________________
std::vector< TopComposite > TopCombos::GetComposites() {

	if ( cand1_.size()==0 && cand3_.size()==0 ) {

		if ( composites1_.size() == 0 ||  cand2_.size() == 0 ) {
			std::cout << "TopCombos: empty input vectors, cand1= "<< cand1_.size()
					  << " composites1= " << composites1_.size() << " cand2= " << cand2_.size() << std::endl;
		}
		return this->TwoCombosMixed();
	}
	if ( composites1_.size() == 0 && cand3_.size()==0 ) {

		if ( cand1_.size() == 0 ||  cand2_.size() == 0 ) {
			std::cout << "TopCombos: empty input vectors, cand1= "<< cand1_.size()
					  << " composites1= " << composites1_.size() << " cand2= " << cand2_.size() << std::endl;
		}
		return this->TwoCombos();
	}
/*
	if ( (cand1_.size()==0 && composites1_.size()!=0 ) || ( cand1_.size() != 0 && composites1_.size() ==0) || cand2_.size() == 0 ) {

		std::cout << "TopCombos: empty input vector, size 1= " << cand1_.size() << " size 2= " << cand2_.size() << std::endl;
		return this->TwoCombos();
	}
*/
	
	if ( cand3_.size() != 0 ) {

		return this->ThreeCombos();
	}
}

//_______________________________________________________________
std::vector< TopComposite > TopCombos::TwoCombosMixed() {

	//std::cout << " running two combos mixed" << std::endl;
	
	//std::vector< TLorentzVector > composites;
	composites_.clear();
	
	//std::cout << "TC: cand1="<< cand1_.size() << " cand2=" << cand2_.size() << std::endl;
	for ( unsigned icand1 = 0; icand1 < composites1_.size(); ++icand1 ) {

		for ( unsigned icand2 = 0; icand2 < cand2_.size(); ++icand2 ) {

			
			TLorentzVector cand11 = composites1_[icand1].GetMon(0);//daughters of composite
			TLorentzVector cand12 = composites1_[icand1].GetMon(1);

			TLorentzVector cand1 = cand11 + cand12;// this is composite
			TLorentzVector cand2 = cand2_[icand2];// single candidate

			//std::cout << "masses:/n" << "cand11= " << cand11.M() << "/ncand12= " << cand12.M() << "/ncand2 
			if ( cand2 != cand11 && cand2 != cand12 ) {

				TLorentzVector acomp = cand1 + cand2;
				//std::cout << "TC: mass= " << acomp.M() << std::endl;
				
				if ( acomp.M() > minInvMass_ && acomp.M() < maxInvMass_ ) {


					Double_t adelta = TMath::Abs(cand1.Phi() - cand2.Phi());
					if ( adelta > TMath::Pi() ) adelta = TMath::Abs(2.*TMath::Pi() - adelta);
					
					if ( adelta > minPhi_ ) {

						// check for duplicates
						bool isduplicate = false;
						for ( unsigned ic = 0; ic < composites_.size(); ++ic) {
							//std::cout << "ci comp M= " << composites_[ic].M() << std::endl;
							if ( TMath::Abs(composites_[ic].M() - acomp.M())<1.0e-4) { isduplicate = true; break; }

						}
						//std::cout << "composites M= " << acomp123.M() << std::endl;

						if ( removeDuplicates_ && isduplicate ) continue;
						
						//if (!isduplicate) {
							//std::cout << "no duplicate" << std::endl;
							
							TopComposite tcomp;
							tcomp.SetMon(0,cand1);
							tcomp.SetMon(1,cand2);
							//tcomp.SetMon(2,cand2);
							
							if (doKinFit_) {
								// look for other jet
								TLorentzVector ocand;
								for ( unsigned iocand = 0; iocand < cand2_.size(); ++iocand ) {
									TLorentzVector tmpcand = cand2_[iocand];
									
									if ( (cand11 != tmpcand) && (cand12 != tmpcand) && (cand2 != tmpcand ) ) ocand = tmpcand;
										
								}

								TopPair *apair = new TopPair();
								apair->SetCand("p",cand11);
								apair->SetCand("q",cand12);
								apair->SetCand("hadbjet",cand2);
								apair->SetCand("lepton",muon_);
								apair->SetCand("neutrino",nu_);
								apair->SetCand("lepbjet",ocand);

								//std::cout << "[TopCombos] run KinFit" << std::endl;
								KinFit(apair);
								//my KinFit
								//MyKinFit(apair);

								tcomp.SetKFChi2( chi2_ );
								tcomp.SetKFndf( ndf_ );

								fittedTops_.push_back(*apair);

								
								

							}
							
							composites_.push_back(tcomp);
							vectors_.push_back(acomp);
							std::string atmp = itoa(icand1);
							atmp += itoa(icand2);
							listcombos_.push_back(atmp);
							//}
					}
				}
			}
		}
	}

	return composites_;
	
}


//_______________________________________________________________
std::vector< TopComposite > TopCombos::TwoCombos() {

	//std::cout << " running two combos" << std::endl;
	
	//std::vector< TLorentzVector > composites;
	composites_.clear();
	
	//std::cout << "TC: cand1="<< cand1_.size() << " cand2=" << cand2_.size() << std::endl;
	for ( unsigned icand1 = 0; icand1 < cand1_.size(); ++icand1 ) {

		for ( unsigned icand2 = 0; icand2 < cand2_.size(); ++icand2 ) {

			bool pass = true;
			// filter candidates
			if ( filterlist_.size() >0) {
				TString astr = TString(filterlist_[icand1] );
				TString c1st(astr[0]);
				TString c2st(astr[1]);
				unsigned c1 = c1st.Atoi();
				unsigned c2 = c2st.Atoi();

				//std::cout << " filterlist_["<< icand1 << "] = " << astr <<
				//	" c1= " << c1 << " c2= " << c2 << std::endl;
				
				if ( (c1 == icand2) || (c2 == icand2) ) pass = false;

			}

			if (!pass) continue;
			
			TLorentzVector cand1 = cand1_[icand1];
			TLorentzVector cand2 = cand2_[icand2];
			
			if ( cand2 != cand1 ) {

				TLorentzVector acomp = cand1 + cand2;
				//std::cout << "TC: mass= " << acomp.M() << std::endl;
				
				if ( acomp.M() > minInvMass_ && acomp.M() < maxInvMass_ ) {


					Double_t adelta = TMath::Abs(cand1.Phi() - cand2.Phi());
					if ( adelta > TMath::Pi() ) adelta = TMath::Abs(2.*TMath::Pi() - adelta);
					
					if ( adelta > minPhi_ ) {

						// check for duplicates
						bool isduplicate = false;
						for ( unsigned ic = 0; ic < composites_.size(); ++ic) {
							//std::cout << "ci comp M= " << composites_[ic].M() << std::endl;
							if ( TMath::Abs(composites_[ic].M() - acomp.M())<1.0e-4) { isduplicate = true; break; }

						}
						//std::cout << "composites M= " << acomp123.M() << std::endl;
						if (!isduplicate) {
							TopComposite tcomp;
							tcomp.SetMon(0,cand1);
							tcomp.SetMon(1,cand2);

							if (doKinFit_) {
								// get other jet
								TLorentzVector ocand;
								TLorentzVector cand11;
								TLorentzVector cand12;
								for ( unsigned iocand = 0; iocand < cand2_.size(); ++iocand ) {
									TLorentzVector tmpcand = cand2_[iocand];
									if ( filterlist_.size() >0) {
										TString astr = TString(filterlist_[icand1] );
										TString c1st(astr[0]);
										TString c2st(astr[1]);
										unsigned c1 = c1st.Atoi();
										unsigned c2 = c2st.Atoi();
										
										if ( (c1 != iocand) && (c2 != iocand) && cand2!=tmpcand ) {
											ocand = tmpcand;
											cand11 = cand2_[c1];
											cand12 = cand2_[c2];
										}
									}
								}
								
								KinFit(cand11,cand12,cand2,ocand,muon_,nu_);
								tcomp.SetKFChi2( chi2_ );
								tcomp.SetKFndf( ndf_ );
							}
							
							composites_.push_back(tcomp);
							vectors_.push_back(acomp);
							std::string atmp = itoa(icand1);
							atmp += itoa(icand2);
							listcombos_.push_back(atmp);
						}
					}
				}
			}
		}
	}

	return composites_;

}


//_______________________________________________________________
std::vector< TopComposite > TopCombos::ThreeCombos() {

	//std::vector< TLorentzVector > composites;
	composites_.clear();
	
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
							  for ( unsigned ic = 0; ic < composites_.size(); ++ic) {
							    //std::cout << "ci comp M= " << composites_[ic].M() << std::endl;
							    if ( TMath::Abs(composites_[ic].M() - acomp123.M())<1.0e-4) { isduplicate = true; break; }

							  }
							  //std::cout << "composites M= " << acomp123.M() << std::endl;
							  if (!isduplicate) {
								  TopComposite tcomp;
								  tcomp.SetMon(0,cand1);
								  tcomp.SetMon(1,cand2);
								  tcomp.SetMon(2,cand3);
								  
								  if (doKinFit_) {
									  KinFit(cand1,cand2,cand3,candLepb_,muon_,nu_);
									  tcomp.SetKFChi2( chi2_ );
									  tcomp.SetKFndf( ndf_ );
									  // tcomp.SetKFChi2( KinFitChi2(cand1,cand2,cand3,candLepb_,muon_,nu_) );
								  }
								  composites_.push_back(tcomp);
								  vectors_.push_back(acomp123);
							  }
							}
						}
					}
				}
			}
		}
	}

	return composites_;

}


//_______________________________________________________________
void TopCombos::KinFit(TLorentzVector p4Hadp, TLorentzVector p4Hadq,
					   TLorentzVector p4Hadb, TLorentzVector p4Lepb,
					   TLorentzVector p4Muon, TLorentzVector p4Nu) {

	//double theFitterChi2 = 0.;
	
////// Kin Fitter

		TKinFitter *theFitter = new TKinFitter("TtFit", "TtFit");
		TMatrixD empty3(3,3); TMatrixD empty4(4,4);
		TAbsFitParticle *fitHadb = new TFitParticleEtEtaPhi("Jet1", "Jet1", 0, &empty3);
		TAbsFitParticle *fitHadp = new TFitParticleEtEtaPhi("Jet2", "Jet2", 0, &empty3);
		TAbsFitParticle *fitHadq = new TFitParticleEtEtaPhi("Jet3", "Jet3", 0, &empty3);
		TAbsFitParticle *fitLepb = new TFitParticleEtEtaPhi("Jet4", "Jet4", 0, &empty3);
		TAbsFitParticle *fitLepl = new TFitParticleEtEtaPhi("Lepton", "Lepton", 0, &empty3);
		TAbsFitParticle *fitLepn = new TFitParticleEtEtaPhi("Neutrino", "Neutrino", 0, &empty3);

		TFitConstraintM  *cons1 = new TFitConstraintM("MassConstraint", "Mass-Constraint", 0, 0 , 80.35);
		cons1->addParticles1(fitHadp, fitHadq);
		TFitConstraintM  *cons2 = new TFitConstraintM("MassConstraint", "Mass-Constraint", 0, 0 , 80.35);
		cons2->addParticles1(fitLepl, fitLepn);
		TFitConstraintM  *cons3 = new TFitConstraintM("MassConstraint", "Mass-Constraint", 0, 0, 175.);
		cons3->addParticles1(fitHadp, fitHadq, fitHadb);
		TFitConstraintM  *cons4 = new TFitConstraintM("MassConstraint", "Mass-Constraint", 0, 0, 175.);
		cons4->addParticles1(fitLepl, fitLepn, fitLepb);
		TFitConstraintM  *cons5 = new TFitConstraintM("MassConstraint", "Mass-Constraint", 0, 0, 0.);
		cons5->addParticle1(fitLepn);

		theFitter->addConstraint(cons1);
		theFitter->addConstraint(cons2);
		theFitter->addConstraint(cons3);
		theFitter->addConstraint(cons4);
		theFitter->addConstraint(cons5);
		
		theFitter->addMeasParticle(fitHadb);
		theFitter->addMeasParticle(fitHadp);
		theFitter->addMeasParticle(fitHadq);
		theFitter->addMeasParticle(fitLepb);
		theFitter->addMeasParticle(fitLepl);
		//theFitter->addUnmeasParticle(fitLepl);
		theFitter->addMeasParticle(fitLepn);
 
		theFitter->setMaxNbIter(200);
		theFitter->setMaxDeltaS(5.e-5);
		theFitter->setMaxF(1.e-4);
		theFitter->setVerbosity(0);

		TMatrixD m0(3,3), m1(3,3), m2(3,3), m3(3,3), m4(3,3), m5(3,3);
		m0.Zero(); m1.Zero(); m2.Zero(); m3.Zero(); m4.Zero(); m5.Zero(); 

		// error matrix
        double acal = 1.0;//1.3;//1.0;
        double bcal = 0.08;//0.1;//0.08;
        double ejErr[4] = {0};
		ejErr[0] = (bcal * bcal * p4Hadp.E() * p4Hadp.E() + acal * acal * p4Hadp.E() );
        ejErr[1] = (bcal * bcal * p4Hadq.E() * p4Hadq.E() + acal * acal * p4Hadq.E() );
        ejErr[2] = (bcal * bcal * p4Hadb.E() * p4Hadb.E() + acal * acal * p4Hadb.E() );
        ejErr[3] = (bcal * bcal * p4Lepb.E() * p4Lepb.E() + acal * acal * p4Lepb.E() );

		double small= 1.e-5;
		m0(0,0) = ejErr[0]; m0(1,1) = small; m0(2,2) = small; 
		m1(0,0) = ejErr[1]; m1(1,1) = small; m1(2,2) = small;
		m2(0,0) = ejErr[2]; m2(1,1) = small; m2(2,2) = small;
		m3(0,0) = ejErr[3]; m3(1,1) = small; m3(2,2) = small;

		m4(0,0) = m4(1,1) = m4(2,2) = small;
		m5(0,0) = m5(1,1) = m5(2,2) = small;
		
		// resolutions

		fitHadp->setIni4Vec(&p4Hadp); fitHadp->setCovMatrix(&m0);
		fitHadq->setIni4Vec(&p4Hadq); fitHadq->setCovMatrix(&m1);
		fitHadb->setIni4Vec(&p4Hadb); fitHadb->setCovMatrix(&m2);
		fitLepb->setIni4Vec(&p4Lepb); fitLepb->setCovMatrix(&m3);
		fitLepl->setIni4Vec(&p4Muon); fitLepl->setCovMatrix(&m4);
		fitLepn->setIni4Vec(&p4Nu); fitLepn->setCovMatrix(&m5);

		theFitter->fit();

		
		
		if (theFitter->getStatus() == 0) {

			chi2_ = theFitter->getS();
			ndf_ = theFitter->getNDF();
			//std::cout << " fit chi2 = " << theFitterChi2 << std::endl;
			TopPair tmppair;
			TLorentzVector tmp;
			tmp.SetXYZT(fitHadp->getCurr4Vec()->X(), fitHadp->getCurr4Vec()->Y(), fitHadp->getCurr4Vec()->Z(), fitHadp->getCurr4Vec()->E() );
			tmppair.SetHadWJet(0,tmp);
			tmp.SetXYZT(fitHadq->getCurr4Vec()->X(), fitHadq->getCurr4Vec()->Y(), fitHadq->getCurr4Vec()->Z(), fitHadq->getCurr4Vec()->E() );
			tmppair.SetHadWJet(1,tmp);
			tmp.SetXYZT(fitHadb->getCurr4Vec()->X(), fitHadb->getCurr4Vec()->Y(), fitHadb->getCurr4Vec()->Z(), fitHadb->getCurr4Vec()->E() );
			tmppair.SetHadbJet(tmp);
			tmp.SetXYZT(fitLepb->getCurr4Vec()->X(), fitLepb->getCurr4Vec()->Y(), fitLepb->getCurr4Vec()->Z(), fitLepb->getCurr4Vec()->E() );
			tmppair.SetLepbJet(tmp);
			tmp.SetXYZT(fitLepl->getCurr4Vec()->X(), fitLepl->getCurr4Vec()->Y(), fitLepl->getCurr4Vec()->Z(), fitLepl->getCurr4Vec()->E() );
			tmppair.SetLepW(0, tmp);
			tmp.SetXYZT(fitLepn->getCurr4Vec()->X(), fitLepn->getCurr4Vec()->Y(), fitLepn->getCurr4Vec()->Z(), fitLepn->getCurr4Vec()->E() );
			tmppair.SetLepW(1, tmp);
			//fittedTops_.push_back(tmppair);
			
		}
		else {
			chi2_ = 999999.;
			ndf_ = theFitter->getNDF();
		}

}

//_______________________________________________________________
void TopCombos::KinFit(TopPair *apair) {


	TLorentzVector p4Hadp = apair->GetCand("p");
	TLorentzVector p4Hadq = apair->GetCand("q");
	TLorentzVector p4Hadb = apair->GetCand("hadbjet");
	TLorentzVector p4Lepb = apair->GetCand("lepbjet");
	TLorentzVector p4Muon = apair->GetCand("lepton");
	TLorentzVector p4Nu = apair->GetCand("neutrino");
	//double theFitterChi2 = 0.;
	//std::cout << "[TopCombos] p4Hadp E = " << p4Hadp.E() << std::endl;
////// Kin Fitter

		TKinFitter *theFitter = new TKinFitter("TtFit", "TtFit");
		TMatrixD empty3(3,3); TMatrixD empty4(4,4);

		// EMom
		
		TAbsFitParticle *fitHadb = new TFitParticleEMomDev("Jet1", "Jet1", 0, &empty4);
		TAbsFitParticle *fitHadp = new TFitParticleEMomDev("Jet2", "Jet2", 0, &empty4);
		TAbsFitParticle *fitHadq = new TFitParticleEMomDev("Jet3", "Jet3", 0, &empty4);
		TAbsFitParticle *fitLepb = new TFitParticleEMomDev("Jet4", "Jet4", 0, &empty4);
		TAbsFitParticle *fitLepl = new TFitParticleEScaledMomDev("Lepton", "Lepton", 0, &empty3);
		TAbsFitParticle *fitLepn = new TFitParticleEScaledMomDev("Neutrino", "Neutrino", 0, &empty3);
		
		// EtEtaPhi parametrization
			/*
		TAbsFitParticle *fitHadb = new TFitParticleEtEtaPhi("Jet1", "Jet1", 0, &empty3);
		TAbsFitParticle *fitHadp = new TFitParticleEtEtaPhi("Jet2", "Jet2", 0, &empty3);
		TAbsFitParticle *fitHadq = new TFitParticleEtEtaPhi("Jet3", "Jet3", 0, &empty3);
		TAbsFitParticle *fitLepb = new TFitParticleEtEtaPhi("Jet4", "Jet4", 0, &empty3);
		TAbsFitParticle *fitLepl = new TFitParticleEtEtaPhi("Lepton", "Lepton", 0, &empty3);
		TAbsFitParticle *fitLepn = new TFitParticleEtEtaPhi("Neutrino", "Neutrino", 0, &empty3);
			*/
		// ESpher parametrization
			/*
		TAbsFitParticle *fitHadb = new TFitParticleESpher("Jet1", "Jet1", 0, &empty4);
		TAbsFitParticle *fitHadp = new TFitParticleESpher("Jet2", "Jet2", 0, &empty4);
		TAbsFitParticle *fitHadq = new TFitParticleESpher("Jet3", "Jet3", 0, &empty4);
		TAbsFitParticle *fitLepb = new TFitParticleESpher("Jet4", "Jet4", 0, &empty4);
		TAbsFitParticle *fitLepl = new TFitParticleESpher("Lepton", "Lepton", 0, &empty4);
		TAbsFitParticle *fitLepn = new TFitParticleESpher("Neutrino", "Neutrino", 0, &empty4);		
			*/
		
		TFitConstraintM  *cons1 = new TFitConstraintM("MassConstraint", "Mass-Constraint", 0, 0 , 80.35);
		cons1->addParticles1(fitHadp, fitHadq);
		TFitConstraintM  *cons2 = new TFitConstraintM("MassConstraint", "Mass-Constraint", 0, 0 , 80.35);
		cons2->addParticles1(fitLepl, fitLepn);
		TFitConstraintM  *cons3 = new TFitConstraintM("MassConstraint", "Mass-Constraint", 0, 0, 175.);
		cons3->addParticles1(fitHadp, fitHadq, fitHadb);
		TFitConstraintM  *cons4 = new TFitConstraintM("MassConstraint", "Mass-Constraint", 0, 0, 175.);
		cons4->addParticles1(fitLepl, fitLepn, fitLepb);
		TFitConstraintM  *cons5 = new TFitConstraintM("MassConstraint", "Mass-Constraint", 0, 0, 0.);
		cons5->addParticle1(fitLepn);

        //TFitConstraintEp *LepPxCons = new TFitConstraintEp("LepPx", "Px constraint", 0, TFitConstraintEp::pX, p4Muon.X() );
		//LepPxCons->addParticle(fitLepl);
		//TFitConstraintEp *LepPyCons = new TFitConstraintEp("LepPy", "Py constraint", 0, TFitConstraintEp::pY, p4Muon.Y() );
		//LepPyCons->addParticle(fitLepl);
		//TFitConstraintEp *LepPzCons = new TFitConstraintEp("LepPz", "Pz constraint", 0, TFitConstraintEp::pZ, p4Muon.Z() );
		//LepPzCons->addParticle(fitLepl);
		//TFitConstraintEp *LepECons = new TFitConstraintEp("LepE", "E constraint", 0, TFitConstraintEp::E, p4Muon.E() );
		//LepECons->addParticle(fitLepl);
		

		theFitter->addConstraint(cons1);
		theFitter->addConstraint(cons2);
		theFitter->addConstraint(cons3);
		theFitter->addConstraint(cons4);
		//theFitter->addConstraint(cons5);

        //theFitter->addConstraint(LepPxCons);
		//theFitter->addConstraint(LepPyCons);
		//theFitter->addConstraint(LepPzCons);
		//theFitter->addConstraint(LepECons);
		
		theFitter->addMeasParticle(fitHadb);
		theFitter->addMeasParticle(fitHadp);
		theFitter->addMeasParticle(fitHadq);
		theFitter->addMeasParticle(fitLepb);
		theFitter->addMeasParticle(fitLepl);
		theFitter->addMeasParticle(fitLepn);
 
		theFitter->setMaxNbIter(200);
		theFitter->setMaxDeltaS(5.e-5);
		theFitter->setMaxF(1.e-4);
		theFitter->setVerbosity(0);

		TMatrixD mHadp3(3,3), mHadq3(3,3), mHadb3(3,3), mLepb3(3,3), mMuon3(3,3), mNu3(3,3);
		mHadp3.Zero(); mHadq3.Zero(); mHadb3.Zero(); mLepb3.Zero(); mMuon3.Zero(); mNu3.Zero(); 
		
		TMatrixD mHadp4(4,4), mHadq4(4,4), mHadb4(4,4), mLepb4(4,4), mMuon4(4,4), mNu4(4,4);
		mHadp4.Zero(); mHadq4.Zero(); mHadb4.Zero(); mLepb4.Zero(); mMuon4.Zero(); mNu4.Zero();

		// get resolutions

		// EMom parametrization
		//std::cout << "[TopCombos] get resolutions" << std::endl;
		int abin = -1;
		abin = ResJets_->getEtaBin( p4Hadp.Eta() );
		double tiny = 1.e-7;
		double blow= 1.;
		//std::cout << "[TopCombos] bin " << abin << ": " << pow( ResJets_->getObsRes(0, abin, p4Hadp.Et() ) , 2 ) << std::endl;
		//mHadp4(0,0) = pow( ResJets_->getObsRes(6, abin, p4Hadp.Et() )*blow, 2 ); //Et
		mHadp4(0,0) = pow( ResJets_->getObsRes(0, abin, p4Hadp.Et() ) , 2 ); //A
		mHadp4(1,1) = pow( ResJets_->getObsRes(1, abin, p4Hadp.Et() ) , 2 ); //B
		mHadp4(2,2) = pow( ResJets_->getObsRes(2, abin, p4Hadp.Et() ) , 2 ); //C
		//mHadp4(3,3) = tiny;//pow( ResJets_->getObsRes(3, abin, p4Hadp.Et() )*blow, 2 ); //Et
		mHadp4(3,3) = pow( ResJets_->getObsRes(3, abin, p4Hadp.Et() ) , 2 ); //D
		mHadp3(0,0) = pow( ResJets_->getObsRes(6, abin, p4Hadp.Et() )*blow, 2 ); //Et
		//std::cout << " et res2= " << mHadp3(0,0) << " eta res2= " <<pow( ResJets_->getObsRes(7, abin, p4Hadp.Et() ) , 2 ) << " phi res2= "<< pow( ResJets_->getObsRes(5, abin, p4Hadp.Et() ) , 2 ) << std::endl;
		mHadp3(1,1) = tiny;//pow( ResJets_->getObsRes(7, abin, p4Hadp.Et() )*blow, 2 ); //eta
		mHadp3(2,2) = tiny;//pow( ResJets_->getObsRes(5, abin, p4Hadp.Et() )*blow, 2 ); //phi
		

		abin = ResJets_->getEtaBin( p4Hadq.Eta() );
		//std::cout << "[TopCombos] bin " << abin << ": " << pow( ResJets_->getObsRes(0, abin, p4Hadp.Et() ) , 2 ) << std::endl;
		//mHadq4(0,0) = pow( ResJets_->getObsRes(6, abin, p4Hadq.Et() )*blow, 2 ); //Et
		mHadq4(0,0) = pow( ResJets_->getObsRes(0, abin, p4Hadq.Et() ) , 2 ); //A
		mHadq4(1,1) = pow( ResJets_->getObsRes(1, abin, p4Hadq.Et() ) , 2 ); //B
		mHadq4(2,2) = pow( ResJets_->getObsRes(2, abin, p4Hadq.Et() ) , 2 ); //C
		//mHadq4(3,3) = tiny;//pow( ResJets_->getObsRes(3, abin, p4Hadq.Et() )*blow, 2 ); //Et
		mHadq4(3,3) = pow( ResJets_->getObsRes(3, abin, p4Hadq.Et() ) , 2 ); //D
		mHadq3(0,0) = pow( ResJets_->getObsRes(6, abin, p4Hadq.Et() )*blow, 2 ); //Et
		//std::cout << " et res2= " << mHadq3(0,0) << " eta res2= " <<pow( ResJets_->getObsRes(7, abin, p4Hadp.Et() ) , 2 ) << std::endl;
		mHadq3(1,1) = tiny;//pow( ResJets_->getObsRes(7, abin, p4Hadq.Et() )*blow, 2 ); //eta
		mHadq3(2,2) = tiny;//pow( ResJets_->getObsRes(5, abin, p4Hadq.Et() )*blow, 2 ); //phi
		//std::cout << "[TopCombos] eta= "<< p4Hadb.Eta() << std::endl;
		abin = ResbJets_->getEtaBin( p4Hadb.Eta() );
		//std::cout << "[TopCombos] bin " << abin << std::endl;
		//std::cout << ": " << pow( ResJets_->getObsRes(0, abin, p4Hadp.Et() ) , 2 ) << std::endl;
		//mHadb4(0,0) = pow( ResbJets_->getObsRes(6, abin, p4Hadb.Et() )*blow, 2 ); //
		mHadb4(0,0) = pow( ResbJets_->getObsRes(0, abin, p4Hadb.Et() ) , 2 ); //A
		mHadb4(1,1) = pow( ResbJets_->getObsRes(1, abin, p4Hadb.Et() ) , 2 ); //B
		mHadb4(2,2) = pow( ResbJets_->getObsRes(2, abin, p4Hadb.Et() ) , 2 ); //C
		//mHadb4(3,3) = tiny;//pow( ResbJets_->getObsRes(3, abin, p4Hadb.Et() )*blow, 2 ); //
		mHadb4(3,3) = pow( ResbJets_->getObsRes(3, abin, p4Hadb.Et() ) , 2 ); //D
		mHadb3(0,0) = pow( ResbJets_->getObsRes(6, abin, p4Hadb.Et() )*blow, 2 ); //
		//std::cout << " et res2= " << mHadb3(0,0) << " eta res2= " <<pow( ResbJets_->getObsRes(7, abin, p4Hadb.Et() ) , 2 ) << std::endl;
		mHadb3(1,1) = tiny;//pow( ResbJets_->getObsRes(7, abin, p4Hadb.Et() )*blow, 2 ); //
		mHadb3(2,2) = tiny;//pow( ResbJets_->getObsRes(5, abin, p4Hadb.Et() )*blow, 2 ); //
		abin = ResbJets_->getEtaBin( p4Lepb.Eta() );
		//std::cout << "[TopCombos] bin " << abin << ": " << pow( ResJets_->getObsRes(0, abin, p4Hadp.Et() ) , 2 ) << std::endl;
		//mLepb4(0,0) = pow( ResbJets_->getObsRes(6, abin, p4Lepb.Et() )*blow, 2 ); //
		mLepb4(0,0) = pow( ResbJets_->getObsRes(0, abin, p4Lepb.Et() ) , 2 ); //A
		mLepb4(1,1) = pow( ResbJets_->getObsRes(1, abin, p4Lepb.Et() ) , 2 ); //B
		mLepb4(2,2) = pow( ResbJets_->getObsRes(2, abin, p4Lepb.Et() ) , 2 ); //C
		//mLepb4(3,3) = tiny;//pow( ResbJets_->getObsRes(3, abin, p4Lepb.Et() )*blow, 2 ); //
		//mLepb4(3,3) = pow( ResbJets_->getObsRes(6, abin, p4Lepb.Et() )*blow, 2 ); //
		mLepb4(3,3) = pow( ResbJets_->getObsRes(3, abin, p4Lepb.Et() ) , 2 ); //D
		mLepb3(0,0) = pow( ResbJets_->getObsRes(6, abin, p4Lepb.Et() )*blow, 2 ); //
		//std::cout << " et res2= " << mLepb3(0,0) << " eta res2= " <<pow( ResbJets_->getObsRes(7, abin, p4Lepb.Et() ) , 2 ) << " phi res2= "<< pow( ResbJets_->getObsRes(5, abin, p4Lepb.Et() ) , 2 ) << std::endl;
		mLepb3(1,1) = tiny;//pow( ResbJets_->getObsRes(7, abin, p4Lepb.Et() )*blow, 2 ); //
		mLepb3(2,2) = tiny;//pow( ResbJets_->getObsRes(5, abin, p4Lepb.Et() )*blow, 2 ); //
		abin = ResMuons_->getEtaBin( p4Muon.Eta() );
		//std::cout << "[TopCombos] bin " << abin << ": " << pow( ResJets_->getObsRes(0, abin, p4Hadp.Et() ) , 2 ) << std::endl;
		mMuon3(0,0) = pow( ResMuons_->getObsRes(0, abin, p4Muon.Et() ) , 2 ); //A
		mMuon3(1,1) = pow( ResMuons_->getObsRes(1, abin, p4Muon.Et() ) , 2 ); //B
		mMuon3(2,2) = pow( ResMuons_->getObsRes(2, abin, p4Muon.Et() ) , 2 ); //C
		//mMuon3(0,0) = pow( ResMuons_->getObsRes(6, abin, p4Muon.Et() )*blow, 2 ); //Et
		//std::cout << " et res2= " << mMuon3(0,0) << " eta res2= " <<pow( ResMuons_->getObsRes(7, abin, p4Muon.Et() ) , 2 ) << " phi res2= "<< pow( ResMuons_->getObsRes(5, abin, p4Muon.Et() ) , 2 ) << std::endl;
		//mMuon3(1,1) = pow( ResMuons_->getObsRes(7, abin, p4Muon.Et() )*blow, 2 ); //eta
		//mMuon3(2,2) = pow( ResMuons_->getObsRes(5, abin, p4Muon.Et() )*blow, 2 ); //phi
		mMuon4(0,0) = pow( ResMuons_->getObsRes(6, abin, p4Muon.Et() )*blow, 2 ); //Et
		mMuon4(1,1) = tiny;
		mMuon4(2,2) = tiny;
		mMuon4(3,3) = tiny;//pow( ResMuons_->getObsRes(3, abin, p4Muon.Et() )*blow, 2 ); //Et
		abin = ResMET_->getEtaBin( p4Nu.Eta() );
		//std::cout << "[TopCombos] bin " << abin << ": " << pow( ResJets_->getObsRes(0, abin, p4Hadp.Et() ) , 2 ) << std::endl;
		mNu3(0,0) = pow( ResMET_->getObsRes(0, abin, p4Nu.Et() ) , 2 ); //A
		mNu3(1,1) = pow( ResMET_->getObsRes(1, abin, p4Nu.Et() ) , 2 ); //B
		mNu3(2,2) = pow( ResMET_->getObsRes(2, abin, p4Nu.Et() ) , 2 ); //C
		//mNu3(0,0) = pow( ResMET_->getObsRes(6, abin, p4Nu.Et() )*blow, 2 ); //Et
		//std::cout << " et res2= " << mNu3(0,0) << " eta res2= " <<pow( ResMET_->getObsRes(7, abin, p4Nu.Et() ) , 2 ) << " phi res2= "<< pow( ResMET_->getObsRes(5, abin, p4Nu.Et() ) , 2 ) << std::endl;
		//mNu3(1,1) = pow( ResMET_->getObsRes(7, abin, p4Nu.Et() )*blow, 2 ); //eta
		//mNu3(2,2) = pow( ResMET_->getObsRes(5, abin, p4Nu.Et() )*blow, 2 ); //phi
		mNu4(0,0) = pow( ResMET_->getObsRes(6, abin, p4Nu.Et() )*blow, 2 ); //Et
		mNu4(1,1) = tiny;
		mNu4(2,2) = tiny;
		mNu4(3,3) = tiny;//pow( ResMET_->getObsRes(3, abin, p4Nu.Et() )*blow, 2 ); //Et
		//std::cout << "[TopCombos] got all resolutions" << std::endl;
		
		// error matrix
		/*
        double acal = 1.3;//1.0;
        double bcal = 0.1;//0.08;
        double ejErr[4] = {0};
		ejErr[0] = (bcal * bcal * p4Hadp.E() * p4Hadp.E() + acal * acal * p4Hadp.E() );
        ejErr[1] = (bcal * bcal * p4Hadq.E() * p4Hadq.E() + acal * acal * p4Hadq.E() );
        ejErr[2] = (bcal * bcal * p4Hadb.E() * p4Hadb.E() + acal * acal * p4Hadb.E() );
        ejErr[3] = (bcal * bcal * p4Lepb.E() * p4Lepb.E() + acal * acal * p4Lepb.E() );

		double small= 1.e-5;
		mHadp3(0,0) = ejErr[0]; mHadp3(1,1) = small; mHadp3(2,2) = small; 
		mHadq3(0,0) = ejErr[1]; mHadq3(1,1) = small; mHadq3(2,2) = small;
		mHadb3(0,0) = ejErr[2]; mHadb3(1,1) = small; mHadb3(2,2) = small;
		mLepb3(0,0) = ejErr[3]; mLepb3(1,1) = small; mLepb3(2,2) = small;

		mMuon3(0,0) = mMuon3(1,1) = mMuon3(2,2) = small;
		mNu3(0,0) = mNu3(1,1) = mNu3(2,2) = small;
		*/
		
		// initialize
		fitHadp->setIni4Vec(&p4Hadp); 
		fitHadq->setIni4Vec(&p4Hadq); 
		fitHadb->setIni4Vec(&p4Hadb); 
		fitLepb->setIni4Vec(&p4Lepb); 
		fitLepl->setIni4Vec(&p4Muon); 
		fitLepn->setIni4Vec(&p4Nu); 

		// resolutions
		fitHadp->setCovMatrix(&mHadp4);
		fitHadq->setCovMatrix(&mHadq4);
		fitHadb->setCovMatrix(&mHadb4);
		fitLepb->setCovMatrix(&mLepb4);
		//fitHadp->setCovMatrix(&mHadp3);
		//fitHadq->setCovMatrix(&mHadq3);
		//fitHadb->setCovMatrix(&mHadb3);
		//fitLepb->setCovMatrix(&mLepb3);
		fitLepl->setCovMatrix(&mMuon3);
		fitLepn->setCovMatrix(&mNu3);

		//std::cout << "[TopCombos] KinFit all setup" << std::endl;
		
		theFitter->fit();		
		
		if (theFitter->getStatus() == 0) {

			TMatrixD Vp(4,4);  Vp  = (*fitHadp->getCovMatrixFit()); 
			TMatrixD Vq(4,4);  Vq  = (*fitHadq->getCovMatrixFit()); 
			TMatrixD Vbh(4,4); Vbh = (*fitHadb->getCovMatrixFit()); 
			TMatrixD Vbl(4,4); Vbl = (*fitLepb->getCovMatrixFit());
			//TMatrixD Vl(4,4); Vl = (*fitLepl->getCovMatrixFit());
			//TMatrixD Vn(4,4); Vn = (*fitLepn->getCovMatrixFit());

			//TMatrixD Vp(3,3);  Vp  = (*fitHadp->getCovMatrixFit()); 
			//TMatrixD Vq(3,3);  Vq  = (*fitHadq->getCovMatrixFit()); 
			//TMatrixD Vbh(3,3); Vbh = (*fitHadb->getCovMatrixFit()); 
			//TMatrixD Vbl(3,3); Vbl = (*fitLepb->getCovMatrixFit());
			TMatrixD Vl(3,3); Vl = (*fitLepl->getCovMatrixFit());
			TMatrixD Vn(3,3); Vn = (*fitLepn->getCovMatrixFit());
			
			chi2_ = theFitter->getS();
			ndf_ = theFitter->getNDF();
			//std::cout << " fit chi2 = " << chi2_ << std::endl;
			TLorentzVector tmp;
			tmp.SetXYZT(fitHadp->getCurr4Vec()->X(), fitHadp->getCurr4Vec()->Y(), fitHadp->getCurr4Vec()->Z(), fitHadp->getCurr4Vec()->E() );
			apair->SetFitted("p",tmp);
			apair->SetRes("p", sqrt(Vp(0,0)) );
			apair->SetCovHadWJet(0, Vp );
			
			//std::cout << "[TopCombos] fitted Hadp E= " << tmp.E() << std::endl;
			tmp.SetXYZT(fitHadq->getCurr4Vec()->X(), fitHadq->getCurr4Vec()->Y(), fitHadq->getCurr4Vec()->Z(), fitHadq->getCurr4Vec()->E() );
			apair->SetFitted("q",tmp);
			apair->SetRes("q", sqrt(Vq(0,0)) );
			apair->SetCovHadWJet(1, Vq );
			
			tmp.SetXYZT(fitHadb->getCurr4Vec()->X(), fitHadb->getCurr4Vec()->Y(), fitHadb->getCurr4Vec()->Z(), fitHadb->getCurr4Vec()->E() );
			apair->SetFitted("hadbjet",tmp);
			apair->SetRes("hadbjet", sqrt(Vbh(0,0)) );
			apair->SetCovHadbJet( Vbh );
			
			tmp.SetXYZT(fitLepb->getCurr4Vec()->X(), fitLepb->getCurr4Vec()->Y(), fitLepb->getCurr4Vec()->Z(), fitLepb->getCurr4Vec()->E() );
			apair->SetFitted("lepbjet",tmp);
			apair->SetRes("lepbjet", sqrt(Vbl(0,0)) );
			apair->SetCovLepbJet( Vbl );
			
			tmp.SetXYZT(fitLepl->getCurr4Vec()->X(), fitLepl->getCurr4Vec()->Y(), fitLepl->getCurr4Vec()->Z(), fitLepl->getCurr4Vec()->E() );
			apair->SetFitted("lepton", tmp);
			apair->SetRes("lepton", sqrt(Vl(0,0)) );
			
			tmp.SetXYZT(fitLepn->getCurr4Vec()->X(), fitLepn->getCurr4Vec()->Y(), fitLepn->getCurr4Vec()->Z(), fitLepn->getCurr4Vec()->E() );
			apair->SetFitted("neutrino", tmp);
			apair->SetRes("neutrino", sqrt(Vn(0,0)) );
			
		}
		else {
			chi2_ = 999999.;
			ndf_ = theFitter->getNDF();
		}
		apair->SetKFChi2(chi2_);
		apair->SetKFndf(ndf_);

		//std::cout << "[TopCombos] KF chi2 = " << chi2_ << std::endl;

		delete theFitter;
		delete fitHadb;
		delete fitHadp;
		delete fitHadq;
		delete fitLepb;
		delete fitLepl;
		delete fitLepn;
		delete cons1;
		delete cons2;
		delete cons3;
		delete cons4;//, LepPxCons, LepPyCons, LepPzCons, LepECons;
		
}

void TopCombos::MyKinFit(TopPair *apair) {
  
  SimpleKinFit afit;
  afit.SetPj1( apair->GetCand("p") );
  afit.SetPj2( apair->GetCand("q") );
  afit.SetPbj2( apair->GetCand("hadbjet") );
  afit.SetPbj1( apair->GetCand("lepbjet") );
  afit.SetPl( apair->GetCand("lepton") );
  afit.SetPzNu( apair->GetCand("neutrino").Pz() );
  
  // error matrix

  double acal = 1.0;//12.0;//3.0;//3.0;//1.0;
  double bcal = 0.08;//1.2;//0.3;//0.3;//0.08;
  afit.SetEj1Err( (bcal * bcal * pow( (apair->GetCand("p")).Et(), 2) + acal * acal * (apair->GetCand("p")).Et() ) );
  afit.SetEj2Err( (bcal * bcal * pow( (apair->GetCand("q")).Et(), 2) + acal * acal * (apair->GetCand("q")).Et() ) );
  afit.SetEbj2Err( (bcal * bcal * pow( (apair->GetCand("hadbjet")).Et(), 2) + acal * acal * (apair->GetCand("hadbjet")).Et() ) );
  afit.SetEbj1Err( (bcal * bcal * pow( (apair->GetCand("lepbjet")).Et(), 2) + acal * acal * (apair->GetCand("lepbjet")).Et() ) );

  afit.Fit();

  chi2_ = afit.GetChi2();
  ndf_ = 3;

  if ( chi2_ != 0 ) {
	  TLorentzVector tmpp = (apair->GetCand("p"));
	  tmpp= ( afit.GetFitEj1() / tmpp.E() )*tmpp;
	  apair->SetFitted("p", tmpp  );
	  apair->SetRes("p", afit.GetFitEj1Err() );
	  tmpp = (apair->GetCand("q"));
	  tmpp= ( afit.GetFitEj2() / tmpp.E() )*tmpp;
	  apair->SetFitted("q", tmpp );
	  apair->SetRes("q", afit.GetFitEj2Err() );
	  tmpp = (apair->GetCand("lepbjet"));
	  tmpp= ( afit.GetFitEbj1() / tmpp.E() )*tmpp;
	  apair->SetFitted("lepbjet", tmpp  );
	  apair->SetRes("lepbjet", afit.GetFitEbj1Err() );
	  tmpp = (apair->GetCand("hadbjet"));
	  tmpp= ( afit.GetFitEbj2() / tmpp.E() )*tmpp;
	  apair->SetFitted("hadbjet", tmpp );
	  apair->SetRes("hadbjet", afit.GetFitEbj2Err() );

	  tmpp.SetPxPyPzE( (apair->GetCand("neutrino")).Px() ,(apair->GetCand("neutrino")).Py(), afit.GetFitNuPz(),
					   sqrt( pow((apair->GetCand("neutrino")).Px(),2)+pow((apair->GetCand("neutrino")).Py(),2)+pow(afit.GetFitNuPz(),2) )  );
	  apair->SetFitted("neutrino", tmpp );
	  apair->SetFitted("lepton", apair->GetCand("lepton") );
	  
  }
  
  //apair->SetFitted("neutrino", afit.GetFitEbj1() );
  //apair->SetRes("lepbjet", afit.GetFitEbj1Err() );




}
