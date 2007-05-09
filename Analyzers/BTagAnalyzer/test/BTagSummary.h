#ifndef BTagSummary_h
#define BTagSummary_h

/**_________________________________________________________________
   class:   BTagSummary.h
   package: Analyzers/BTagAnalyzer
   

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: BTagSummary.h,v 1.3 2007/04/25 03:53:52 yumiceva Exp $

________________________________________________________________**/


// C++ standard

// ROOT
#include "TObject.h"
#include "TMatrixDSym.h"


class BTagSummary : public TObject {

  public:

	BTagSummary();
	~BTagSummary();

	void Reset();
	
	Int_t event; // event number
	Int_t run;   // run number

		
	// mc

	// reco
	Int_t njets;    // number of jets
	Int_t nmuons;   // number of muons
	Int_t nvertex;  // number of vertices
	Int_t ngenjets; // number of generated jets

	std::vector< double > jet_p;
	std::vector< double > jet_pt;
	std::vector< double > jet_eta;
	std::vector< double > jet_phi;
	std::vector< double > jet_et;
	std::vector< double > jet_vx;
	std::vector< double > jet_vy;
	std::vector< double > jet_vz;
	std::vector< double > jet_deltar;
	std::vector< double > jet_ptrel;
	std::vector< int > jet_flavour_phy;
	std::vector< int > jet_flavour_alg;
	std::vector< int > jet_btagged;
	
	std::vector< double > jetcorr_p;
	std::vector< double > jetcorr_pt;
	std::vector< double > jetcorr_et;
	std::vector< double > jetcorr_ptrel;
	std::vector< int > jetcorr_btagged;

	std::vector< double > genjet_p;
	std::vector< double > genjet_pt;
	std::vector< double > genjet_eta;
	std::vector< double > genjet_phi;
	std::vector< double > genjet_et;
	std::vector< double > genjet_vx;
	std::vector< double > genjet_vy;
	std::vector< double > genjet_vz;


	std::vector< double > muon_pt;
	std::vector< double > muon_eta;
	std::vector< double > muon_phi;
	std::vector< double > muon_charge;
	std::vector< double > muon_p;
	std::vector< double > muon_trkchi2;
	std::vector< double > muon_trkndof;
	std::vector< double > muon_chi2;
	std::vector< double > muon_ndof;
	std::vector< int > muon_SArechits;
	std::vector< int > muon_trkrechits;
	std::vector< double > muon_d0;
	std::vector< double > muon_d0sigma;
	std::vector< double > muon_vx;
	std::vector< double > muon_vy;
	std::vector< double > muon_vz;

	std::vector< double > muon_chi2_hits_mc;
	std::vector< double > muon_pt_hits_mc;
	std::vector< double > muon_eta_hits_mc;
	std::vector< double > muon_phi_hits_mc;
	std::vector< double > muon_charge_hits_mc;
	std::vector< double > muon_p_hits_mc;
	std::vector< int > muon_pdgid_hits_mc;
	std::vector< double > muon_vx_hits_mc;
	std::vector< double > muon_vy_hits_mc;
	std::vector< double > muon_vz_hits_mc;
	
	std::vector< double > muon_chi2_chi2_mc;
	std::vector< double > muon_pt_chi2_mc;
	std::vector< double > muon_eta_chi2_mc;
	std::vector< double > muon_phi_chi2_mc;
	std::vector< double > muon_charge_chi2_mc;
	std::vector< double > muon_p_chi2_mc;
	std::vector< double > muon_vx_chi2_mc;
	std::vector< double > muon_vy_chi2_mc;
	std::vector< double > muon_vz_chi2_mc;
	std::vector< int > muon_pdgid_chi2_mc;
	std::vector< int > muon_mother_pdgid_chi2_mc;

	std::vector< double > btag_discriminator0;
	std::vector< double > btag_discriminator1;
	std::vector< double > btag_discriminator2;

	std::vector< double > otherjet_pt;
	std::vector< double > otherjet_eta;
	std::vector< double > otherjet_phi;
	std::vector< double > otherjet_et;
	std::vector< int > otherjet_flavour;
	std::vector< int > otherjet_btagged;
	std::vector< double > otherbtag_discriminator0;
	std::vector< double > otherbtag_discriminator1;
	std::vector< double > otherbtag_discriminator2;

	ClassDef(BTagSummary,2);

};

#endif
