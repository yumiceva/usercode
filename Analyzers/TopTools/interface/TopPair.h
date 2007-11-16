#ifndef TopPair_h
#define TopPair_h

/**_________________________________________________________________
   class:   TopPair.h
   package: Analyzers/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopPair.h,v 1.1 2007/10/31 19:34:56 yumiceva Exp $

________________________________________________________________**/

#include "TLorentzVector.h"
#include "TMatrixD.h"
#include <string>
#include <iostream>

class TopPair {

  public:
	TopPair(){
		fittedp_(0);
		fittedq_(0);
		fittedHadb_(0);
		fittedLepb_(0);
		fittedl_(0);
		fittedn_(0);
		candp_(0);
		candq_(0);
		candHadb_(0);
		candLepb_(0);
		candl_(0);
		candn_(0);
		ndf_= 0;
		chi2_ = 0.;
		resEp_ = resEq_ = resEhadb_ = resElepb_ = resEl_ = resEn_ = 0;
		Vp.ResizeTo(4,4);
		Vq.ResizeTo(4,4);
		Vbh.ResizeTo(4,4);
		Vbl.ResizeTo(4,4);
		//Vp.ResizeTo(3,3);
		//Vq.ResizeTo(3,3);
		//Vbh.ResizeTo(3,3);
		//Vbl.ResizeTo(3,3);
		
		Vl.ResizeTo(3,3);
		Vn.ResizeTo(3,3);
	};
	
	~TopPair() {};

	void SetCand(TString name, TLorentzVector vector) {
		if ( name == "p" ) candp_ = vector;
		else if ( name == "q" ) candq_ = vector;
		else if ( name == "lepton" ) candl_ = vector;
		else if ( name == "neutrino" ) candn_ = vector;
		else if ( name == "hadbjet" ) candHadb_ = vector;
		else if ( name == "lepbjet" ) candLepb_ = vector;
		else {
			std::cout << "don't know about this name " << name <<
				"options are: p, q, lepton, neutrino, hadbjet, lepbjet"<< std::endl;
		}
	}
	TLorentzVector GetCand(TString name) {
		if ( name == "p" ) return candp_;
		else if ( name == "q" ) return candq_;
		else if ( name == "lepton" ) return candl_;
		else if ( name == "neutrino" ) return candn_;
		else if ( name == "hadbjet" ) return candHadb_;
		else if ( name == "lepbjet" ) return candLepb_;
		else {
			std::cout << "don't know about this name " << name
					  << "options are: p, q, lepton, neutrino, hadbjet, lepbjet"<< std::endl;
			TLorentzVector p(0,0,0,0);
			return p;
		}
	}
	void SetFitted(TString name, TLorentzVector vector) {
		if ( name == "p" ) fittedp_ = vector;
		else if ( name == "q" ) fittedq_ = vector;
		else if ( name == "lepton" ) fittedl_ = vector;
		else if ( name == "neutrino" ) fittedn_ = vector;
		else if ( name == "hadbjet" ) fittedHadb_ = vector;
		else if ( name == "lepbjet" ) fittedLepb_ = vector;
		else {
			std::cout << "don't know about this name " << name <<
				"options are: p, q, lepton, neutrino, hadbjet, lepbjet"<< std::endl;
		}
	}
	TLorentzVector GetFitted(TString name) {
		if ( name == "p" ) return fittedp_;
		else if ( name == "q" ) return fittedq_;
		else if ( name == "lepton" ) return fittedl_;
		else if ( name == "neutrino" ) return fittedn_;
		else if ( name == "hadbjet" ) return fittedHadb_;
		else if ( name == "lepbjet" ) return fittedLepb_;
		else {
			std::cout << "don't know about this name " << name
					  << "options are: p, q, lepton, neutrino, hadbjet, lepbjet"<< std::endl;
			TLorentzVector p(0,0,0,0);
			return p;
		}
	}
	void SetRes(TString name, double E) {
		if ( name == "p" ) resEp_ = E;
		else if ( name == "q" ) resEq_ = E;
		else if ( name == "lepton" ) resEl_ = E;
		else if ( name == "neutrino" ) resEn_ = E;
		else if ( name == "hadbjet" ) resEhadb_ = E;
		else if ( name == "lepbjet" ) resElepb_ = E;
		else {
			std::cout << "don't know about this name " << name <<
				"options are: p, q, lepton, neutrino, hadbjet, lepbjet"<< std::endl;
		}
	}
	double GetRes(TString name) {
		if ( name == "p" ) return resEp_;
		else if ( name == "q" ) return resEq_;
		else if ( name == "lepton" ) return resEl_;
		else if ( name == "neutrino" ) return resEn_;
		else if ( name == "hadbjet" ) return resEhadb_;
		else if ( name == "lepbjet" ) return resElepb_;
		else {
			std::cout << "don't know about this name " << name <<
				"options are: p, q, lepton, neutrino, hadbjet, lepbjet"<< std::endl;
			return 0;
		}
	}
	void SetHadWJet(int im, TLorentzVector vector) {
		if (im==0) candp_ = vector;
		if (im==1) candq_ = vector;
	}
	void SetLepW(int im, TLorentzVector vector) {
	  if (im==0) candl_ = vector;
	  if (im==1) candn_ = vector;
        }
	void SetHadbJet( TLorentzVector vector ) {
	  candHadb_ = vector;
	}
	void SetLepbJet( TLorentzVector vector ) {
	  candLepb_ = vector;
	}
	void SetCovHadWJet(int im, TMatrixD cov ) {
		if (im==0) Vp = cov;
		if (im==1) Vq = cov;
	}
	void SetCovLepW( int im, TMatrixD cov ) {
		if (im==0) Vl = cov;
		if (im==1) Vn = cov;
	}
	void SetCovHadbJet( TMatrixD cov ) { Vbh = cov; }
	void SetCovLepbJet( TMatrixD cov ) { Vbl = cov; }

	TMatrixD GetCovHadWJet(int im) {
		if (im==0) return Vp;
		if (im==1) return Vq;
	}

	TMatrixD GetCovLepW( int im) {
		if (im==0) return Vl;
		if (im==1) return Vn;
	}

	TMatrixD GetCovHadbJet() { return Vbh; }
	TMatrixD GetCovLepbJet() { return Vbl; }
		
	TLorentzVector GetHadWJet(int im) {
		if (im==0) return candp_;
		if (im==1) return candq_;
	}
	TLorentzVector GetLepW(int im) {
		if (im==0) return candl_;
		if (im==1) return candn_;
	}
	TLorentzVector GetHadbJet() {
		return candHadb_;
	}
	TLorentzVector GetLepbJet() {
		return candLepb_;
	}
	double HadtopM() {
		TLorentzVector top = candp_ + candq_ + candHadb_;
		return top.M();
	}
	double LeptopM() {
		TLorentzVector top = candl_ + candn_ + candLepb_;
		return top.M();
	}
	double TopPairM() {
		TLorentzVector pair = candp_ + candq_ + candHadb_;
		pair = pair + candl_ + candn_ + candLepb_;
		return pair.M();
	}
	double FittedTopPairM() {
		TLorentzVector pair = fittedp_ + fittedq_ + fittedHadb_;
		pair = pair + fittedl_ + fittedn_ + fittedLepb_;
		return pair.M();
	}
	void SetKFChi2(double chi2) {
		chi2_ = chi2;
	}
	void SetKFndf(int ndf) {
		ndf_ = ndf;
	}
	double GetKFChi2() { return chi2_; }
	int GetKFndf() { return ndf_; }

	
  private:
	TLorentzVector candp_;
	TLorentzVector candq_;
	TLorentzVector candHadb_;
	TLorentzVector candl_;
	TLorentzVector candn_;
	TLorentzVector candLepb_;
	TLorentzVector fittedp_;
	TLorentzVector fittedq_;
	TLorentzVector fittedHadb_;
	TLorentzVector fittedl_;
	TLorentzVector fittedn_;
	TLorentzVector fittedLepb_;
	TMatrixD Vp;
	TMatrixD Vq;
	TMatrixD Vbh;
	TMatrixD Vbl;
	TMatrixD Vl;
	TMatrixD Vn;
	
	double chi2_;
	int ndf_;
	double resEp_, resEq_, resEhadb_, resEl_, resEn_, resElepb_;
	
};
#endif
