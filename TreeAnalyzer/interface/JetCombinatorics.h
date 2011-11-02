#ifndef JetCombinatorics_h
#define JetCombinatorics_h

/**_________________________________________________________________
   class:   JetCombinatorics.h
   package: 


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: JetCombinatorics.h,v 1.1 2011/09/01 19:31:54 yumiceva Exp $

________________________________________________________________**/

#include "TLorentzVector.h"
#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include "TMath.h"
#include <map>
#include <vector>
#include <iostream>

class Combo {


  public:

	Combo() {
		
	  MW = 80.4;//79.8;
	  Mtop_h = 172.;//175.;
	  Mtop_l = 172.;

	  sigmaHadW = 10.5;//2.*7.6;
	  sigmaHadt = 19.2;//2.*12.5;
	  sigmaLept = 24.2;//2.*15.6;

	  chi2_ = 0.;
	  SumEt_ = 0.;
	  deltaRl_ = 999;
	  deltaRtlth_ = 999;
	  usebtag_ = false;
	  useMtop_ = true;
	  IsGoodbTagEvent_ = false;
	  Wp_istag_ = Wq_istag_ = Hadb_istag_ = Lepb_istag_ = false;

	  useFlv_ = false;
	  Wp_flv_ = Wq_flv_ = Hadb_flv_ = Lepb_flv_ = 1.;
	  
	}
	~Combo(){};

	void SetWp(TLorentzVector Wp) { Wp_ = Wp; }
	void SetWq(TLorentzVector Wq) { Wq_ = Wq; }
	void SetHadb(TLorentzVector Hadb) { Hadb_ = Hadb; }
	void SetLepW(TLorentzVector LepW) { LepW_ = LepW; }
	void SetLepb(TLorentzVector Lepb) { Lepb_ = Lepb; }
	// flavor corrections
	void ApplyFlavorCorrections(bool option=true){ useFlv_ = option;}
	void SetFlvCorrWp( double corr ) { Wp_flv_ = corr; }
	void SetFlvCorrWq( double corr ) { Wq_flv_ = corr; }
	void SetFlvCorrHadb( double corr ) { Hadb_flv_ = corr; }
	void SetFlvCorrLepb( double corr ) { Lepb_flv_ = corr; }
	// b tagging
	void SetWp_tag(bool tag) { Wp_istag_ = tag;}
	void SetWq_tag(bool tag) { Wq_istag_ = tag;}
	void SetHadb_tag(bool tag) { Hadb_istag_ = tag;}
	void SetLepb_tag(bool tag) { Lepb_istag_ = tag;}

	void SetWp_disc(double disc) { Wp_disc_ = disc;}
	void SetWq_disc(double disc) { Wq_disc_= disc;}
	void SetHadb_disc(double disc) { Hadb_disc_= disc;}
	void SetLepb_disc(double disc) { Lepb_disc_= disc;}
	void SetbDiscPdf(TString filename) { 
	  pdffile_ = TFile::Open(filename);
	  hdisc_b_ = (TH1F*) gDirectory->Get("hdiscNorm_b");
	  hdisc_cl_ = (TH1F*) gDirectory->Get("hdiscNorm_cl");
	}
	void SetSigmas(int type=0,bool isMC=true) {

	  if (! isMC) {
	    MW = 80.4;
	    Mtop_h = Mtop_l = 173.3;
	    //leave default sigmas for the moment
	  }

	  // type == 0 take defaults
	  if (type==1 && isMC) {
	    // JES +10%
	    MW = 87.2;
	    Mtop_h = 193.2;
	    Mtop_l = 179.0;
	    sigmaHadW = 13.0;
	    sigmaHadt = 22.8;
	    sigmaLept = 26.3;
	  }
	  if (type==-1 && isMC) {
            // JES -10%
            MW = 81.6;
            Mtop_h = 169.3;
            Mtop_l = 171.4;
            sigmaHadW =8.9;
            sigmaHadt =17.9;
            sigmaLept =22.6;
	  }

	}
	void Usebtagging(bool option = true) { usebtag_ = option;}
	void SetMinMassLepW( double mass ) { minMassLepW_ = mass; }
	void SetMaxMassLepW( double mass ) { maxMassLepW_ = mass; }
	void SetMinMassHadW( double mass ) { minMassHadW_ = mass; }
	void SetMaxMassHadW( double mass ) { maxMassHadW_ = mass; }
	void SetMinMassLepTop( double mass ) { minMassLepTop_ = mass; }
	void SetMaxMassLepTop( double mass ) { maxMassLepTop_ = mass; }
	void UseMtopConstraint(bool option=true) { useMtop_ = option; }
	bool IsGoodbTagEvent() { return IsGoodbTagEvent_; }
	void analyze() {

		if ( useFlv_ ) {
			Wp_ = Wp_flv_ * Wp_;
			Wq_ = Wq_flv_ * Wq_;
			Hadb_ = Hadb_flv_ * Hadb_;
			Lepb_ = Lepb_flv_ * Lepb_;
		}

		HadW_ = Wp_ + Wq_;
		HadTop_ = HadW_ + Hadb_;
		LepTop_ = LepW_ + Lepb_;
		TopPair_ = HadTop_ + LepTop_;

		//double sigmaHadW = 10.5;//2.*7.6;
		//double sigmaHadt = 19.2;//2.*12.5;
		//double sigmaLept = 24.2;//2.*15.6;
		
		double chiHadW = (HadW_.M() - MW)/sigmaHadW;
		double chiHadt = (HadTop_.M() - Mtop_h)/sigmaHadt;
		double chiLept = (LepTop_.M() - Mtop_l)/sigmaLept;

		if ( useMtop_ ) {
			chi2_ = chiHadW*chiHadW + chiHadt*chiHadt + chiLept*chiLept;
			Ndof_ = 3;
		} else {
			chi2_ = chiHadW*chiHadW + (HadTop_.M() - LepTop_.M())*(HadTop_.M() - LepTop_.M())/(sigmaHadt*sigmaHadt+sigmaLept*sigmaLept);
			Ndof_ = 2;
		}
		
		SumEt_ = HadTop_.Pt();

		if ( usebtag_ ) {

		  if ( Wp_istag_ || Wq_istag_ || Hadb_istag_ || Lepb_istag_  ) {
		    
		    IsGoodbTagEvent_ = true;

		  }
		  
		}

		// delta R
		double deltaRbltl = Lepb_.DeltaR( LepTop_ );
		double deltaRnutl = LepW_.DeltaR( LepTop_ );
		//double deltaRltl =
		deltaRl_ = deltaRbltl + deltaRnutl;
		deltaRtlth_ = LepTop_.DeltaR( HadTop_);

	}

	TLorentzVector GetWp() { return Wp_; }
	TLorentzVector GetWq() { return Wq_; }
	TLorentzVector GetHadW() { return HadW_; }
	TLorentzVector GetLepW() { return LepW_; }
	TLorentzVector GetHadb() { return Hadb_; }
	TLorentzVector GetLepb() { return Lepb_; }
	TLorentzVector GetHadTop() { return HadTop_; }
	TLorentzVector GetLepTop() { return LepTop_; }
	TLorentzVector GetTopPair() { return TopPair_; }
	double GetChi2() { return chi2_; }
	double GetNdof() { return Ndof_; }
	double GetSumEt() { return SumEt_; }
	double GetDeltaRl() { return deltaRl_; }
	double GetDeltaRtlth() { return deltaRtlth_; }
	int GetIdHadb() { return IdHadb_;}
	int GetIdWp() { return IdWp_; }
	int GetIdWq() { return IdWq_; }
	int GetIdLepb() { return IdLepb_;}
	void SetIdHadb(int id) { IdHadb_ = id;}
	void SetIdWp(int id) { IdWp_ = id; }
	void SetIdWq(int id) { IdWq_ = id; }
	void SetIdLepb(int id) { IdLepb_ = id;}
	void Print() {
	  std::cout << " jet Wp  : px = " << Wp_.Px() << " py = " <<  Wp_.Py() << " pz = " << Wp_.Pz() << " e = " << Wp_.E() << " tag = " << Wp_istag_ << std::endl;
	  std::cout << " jet Wq  : px = " << Wq_.Px() << " py = " <<  Wq_.Py() << " pz = " << Wq_.Pz() << " e = "<< Wq_.E() << " tag = " << Wq_istag_ << std::endl;
	  std::cout << " jet Hadb: px = " << Hadb_.Px() << " py = " <<  Hadb_.Py() <<" pz = " << Hadb_.Pz() <<" e = "<< Hadb_.E() << " tag = " << Hadb_istag_ << std::endl;
	  std::cout << " jet Lepb: px = " << Lepb_.Px() << " py = " <<  Lepb_.Py() <<" pz = " << Lepb_.Pz() <<" e = "<< Lepb_.E() << " tag = " << Lepb_istag_ << std::endl;
	  std::cout << " chi-squared = " << chi2_ << " sumEt = " << SumEt_ << std::endl;
	}
	double getPdfValue(std::string flavor, double disc) {
	  double pdf= 0;
	  TH1F *hpdf;
	  if ( flavor == "b" ) hpdf = hdisc_b_;
	  else hpdf = hdisc_cl_;
	  int bin = hpdf->GetXaxis()->FindBin( disc );
	  pdf = hpdf->GetBinContent( bin );
	  if ( disc < -10 || disc >50 ) return 0;
	  //if ( pdf == 0 ) return 1.e-7;
	  return pdf;
	}
	
  private:
	
	TLorentzVector Wp_;
	TLorentzVector Wq_;
	TLorentzVector HadW_;
	TLorentzVector Hadb_;
	TLorentzVector HadTop_;
	TLorentzVector LepW_;
	TLorentzVector Lepb_;	
	TLorentzVector LepTop_;
	TLorentzVector TopPair_;
	
	bool usebtag_;
	bool useMtop_;
	bool Wp_istag_;
	bool Wq_istag_;
	bool Hadb_istag_;
	bool Lepb_istag_;
	bool IsGoodbTagEvent_;

	double Wp_disc_;
	double Wq_disc_;
	double Hadb_disc_;
	double Lepb_disc_;
	TFile *pdffile_;
	TH1F *hdisc_b_;
	TH1F *hdisc_cl_;

	double Wp_flv_, Wq_flv_, Hadb_flv_, Lepb_flv_;
	bool useFlv_;
	double chi2_;
	double Ndof_;
	double SumEt_;
	double deltaRl_;
	double deltaRtlth_;
	double minMassLepW_;
	double maxMassLepW_;
	double minMassHadW_;
	double maxMassHadW_;
	
	double minMassLepTop_;
	double maxMassLepTop_;

	double MW;
	double Mtop_h;
	double Mtop_l;
	double sigmaHadW;
	double sigmaHadt;
	double sigmaLept;


	int IdHadb_;
	int IdWp_;
	int IdWq_;
	int IdLepb_;
	
};

struct minChi2
{
  bool operator()(Combo s1, Combo s2) const
  {
    return s1.GetChi2() <= s2.GetChi2();
  }
};

struct maxSumEt
{
  bool operator()(Combo s1, Combo s2) const
  {
    return s1.GetSumEt() >= s2.GetSumEt();
  }
};

struct minDeltaR
{
  bool operator()(Combo s1, Combo s2) const
  {
    return ( s1.GetDeltaRl() <= s2.GetDeltaRl() ) && ( s1.GetDeltaRtlth() >= s2.GetDeltaRtlth() );
  }
};


class JetCombinatorics {

  public:

	JetCombinatorics();
	~JetCombinatorics();

	void Verbose() {
	  verbosef = true;
	}

	std::map< int, std::string > Combinatorics(int k, int max = 6);
	std::map< int, std::string > NestedCombinatorics();

	//void FourJetsCombinations(std::vector<TLorentzVector> jets, std::vector<double> bdiscriminators);
	void FourJetsCombinations(std::vector<TLorentzVector> *jets, std::vector<int> *btags);
	void SetFlavorCorrections(std::vector<double > vector ) { flavorCorrections_ = vector; }
	void SetMaxNJets(int n) { maxNJets_ = n; }
	Combo GetCombination(int n=0);
	Combo GetCombinationSumEt(int n=0);
	Combo GetCombinationDeltaR(int n =0);
	int GetNumberOfCombos() { return ( (int)allCombos_.size() ); } 
	//void SetCandidate( std::vector< TLorentzVector > JetCandidates );

	void SetSigmas(int type = 0) {
	  SigmasTypef = type;
	}
	void SetLeptonicW( TLorentzVector LepW ) { theLepW_ = LepW; }
	void SetOtherLeptonicW(  TLorentzVector LepW ) { theOtherLepW_ = LepW; }
	void SetMinMassLepW( double mass ) { minMassLepW_ = mass; }
	void SetMaxMassLepW( double mass ) { maxMassLepW_ = mass; }
	void SetMinMassHadW( double mass ) { minMassHadW_ = mass; }
	void SetMaxMassHadW( double mass ) { maxMassHadW_ = mass; }
	void SetMinMassLepTop( double mass ) { minMassLepTop_ = mass; }
	void SetMaxMassLepTop( double mass ) { maxMassLepTop_ = mass; }

	void UsebTagging( bool option = true ) { UsebTagging_ = option; }
	void ApplyFlavorCorrection( bool option = true ) { UseFlv_ = option; }
	void UseMtopConstraint( bool option = true) { UseMtop_ = option; }
	void SetbTagPdf( TString name ) { bTagPdffilename_ = name; }
	void Clear();

	//std::vector< TLorentzVector > TwoCombos();
	//std::vector< TLorentzVector > ThreeCombos();

	void RemoveDuplicates( bool option) { removeDuplicates_ = option; }

	//std::vector< TLorentzVector > GetComposites();
	//void AnalyzeCombos();


  private:

	//int kcombos_;
	//int maxcombos_;
	int SigmasTypef;
	bool verbosef;
	std::map< int, std::string > Template4jCombos_;
	std::map< int, std::string > Template5jCombos_;
	std::map< int, std::string > Template6jCombos_;
	std::map< int, std::string > Template7jCombos_;

	std::vector< double > flavorCorrections_;
	int maxNJets_;
	bool UsebTagging_;
	bool UseMtop_;
	TString bTagPdffilename_;
	bool UseFlv_;
	
	TLorentzVector theLepW_;
	TLorentzVector theOtherLepW_;
	
	double minMassLepW_;
	double maxMassLepW_;
	double minMassHadW_;
	double maxMassHadW_;
	double minMassLepTop_;
	double maxMassLepTop_;
	
	std::map< Combo, int, minChi2 > allCombos_;
	std::map< Combo, int, maxSumEt > allCombosSumEt_;
	std::map< Combo, int, minDeltaR > allCombosDeltaR_;

	Double_t minPhi_;
	double chi2_;
	int ndf_;
	Double_t minDeltaR_;
	bool removeDuplicates_;
	
	//std::vector< TLorentzVector > cand1_;
	//std::vector< TLorentzVector > cand2_;
	//std::vector< TLorentzVector > cand3_;

	//int nLists_;
	
	//std::vector< TLorentzVector > composites_;
	
};

#endif
