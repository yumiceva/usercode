/**_________________________________________________________________
   class:   BTagSummary.cc
   package: Validation/RecoVertex
   

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: BTagSummary.cc,v 1.4 2007/05/09 03:32:21 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/BTagAnalyzer/interface/BTagSummary.h"

// C++ standard

// ROOT

//_______________________________________________________________
BTagSummary::BTagSummary() {

	this->Reset();
	
}

//_______________________________________________________________
BTagSummary::~BTagSummary() {
}

//_______________________________________________________________
void BTagSummary::Reset() {

	event   = -1;
	run     = -1;

		
	// mc

	// reco
	njets       = -1;
	nmuons      = -1;
	
	nvertex     = -1;
	ngenjets    = -1;

	jet_p.clear();
	jet_pt.clear();
	jet_eta.clear();
	jet_phi.clear();
	jet_et.clear();
	jet_deltar.clear();
	jet_vx.clear();
	jet_vy.clear();
	jet_vz.clear();
	jet_ptrel.clear();
	jet_flavour_alg.clear();
	jet_flavour_phy.clear();
	jet_btagged.clear();

	jetcorrection.clear();
	
	//jetcorr_p.clear();
	//jetcorr_pt.clear();
	//jetcorr_et.clear();
	//jetcorr_ptrel.clear();
	//jetcorr_btagged.clear();
	
	genjet_p.clear();
	genjet_pt.clear();
	genjet_eta.clear();
	genjet_phi.clear();
	genjet_et.clear();
	genjet_vx.clear();
	genjet_vy.clear();
	genjet_vz.clear();
	
	muon_pt.clear();
	muon_eta.clear();
	muon_phi.clear();
	muon_charge.clear();
	muon_p.clear();
	muon_trkchi2.clear();
	muon_trkndof.clear();
	muon_chi2.clear();
	muon_ndof.clear();
	muon_SArechits.clear();
	muon_trkrechits.clear();
	muon_d0.clear();
	muon_d0sigma.clear();
	muon_vx.clear();
	muon_vy.clear();
	muon_vz.clear();
	
	muon_chi2_hits_mc.clear();
	muon_pt_hits_mc.clear();
	muon_eta_hits_mc.clear();
	muon_phi_hits_mc.clear();
	muon_charge_hits_mc.clear();
	muon_p_hits_mc.clear();
	muon_pdgid_hits_mc.clear();
	muon_vx_hits_mc.clear();
	muon_vy_hits_mc.clear();
	muon_vz_hits_mc.clear();
	
	muon_chi2_chi2_mc.clear();
	muon_pt_chi2_mc.clear();
	muon_eta_chi2_mc.clear();
	muon_phi_chi2_mc.clear();
	muon_charge_chi2_mc.clear();
	muon_p_chi2_mc.clear();
	muon_pdgid_chi2_mc.clear();
	muon_vx_chi2_mc.clear();
	muon_vy_chi2_mc.clear();
	muon_vz_chi2_mc.clear();
	muon_mother_pdgid_chi2_mc.clear();

	btag_discriminator0.clear();
	btag_discriminator1.clear();
	btag_discriminator2.clear();

	otherjetcorrection.clear();
	otherjet_pt.clear();
	otherjet_eta.clear();
	otherjet_phi.clear();
	otherjet_et.clear();
	otherjet_flavour.clear();
	otherjet_btagged.clear();
	otherbtag_discriminator0.clear();
	otherbtag_discriminator1.clear();
	otherbtag_discriminator2.clear();
	
}


