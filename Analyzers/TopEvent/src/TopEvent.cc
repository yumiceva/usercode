/**_________________________________________________________________
   class:   TopEvent.cc
   package: Analyzer/TopEvent
   

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopEvent.cc,v 1.2 2007/03/26 20:42:14 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/TopEvent/interface/TopEvent.h"

// C++ standard

// ROOT

//_______________________________________________________________
TopEvent::TopEvent() {

	this->Reset();
}

//_______________________________________________________________
TopEvent::~TopEvent() {
}

//_______________________________________________________________
void TopEvent::Reset() {

  // event information
	event   = -1;
	run     = -1;

	// total number of objects
	njets       = -1;
	nleptons    = -1;
        nvertex     = -1;
	// MC
	ngenjets    = -1;
	ngenleptons = -1;

	// jets
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
	jet_flavour.clear();
	
	jet_btagged.clear();
	
	genjet_p.clear();
	genjet_pt.clear();
	genjet_eta.clear();
	genjet_phi.clear();
	genjet_et.clear();
	genjet_flavour.clear();
	
	lepton_pt.clear();
	lepton_eta.clear();
	lepton_phi.clear();
	lepton_charge.clear();
	lepton_p.clear();
	lepton_chi2.clear();
	lepton_ndof.clear();
	lepton_d0.clear();
	lepton_d0sigma.clear();
	lepton_vx.clear();
	lepton_vy.clear();
	lepton_vz.clear();
	lepton_pid.clear();
	// MC
	lepton_chi2_hits_mc.clear();
	lepton_pt_hits_mc.clear();
	lepton_eta_hits_mc.clear();
	lepton_phi_hits_mc.clear();
	lepton_charge_hits_mc.clear();
	lepton_p_hits_mc.clear();
	lepton_pdgid_hits_mc.clear();
	
	lepton_chi2_chi2_mc.clear();
	lepton_pt_chi2_mc.clear();
	lepton_eta_chi2_mc.clear();
	lepton_phi_chi2_mc.clear();
	lepton_charge_chi2_mc.clear();
	lepton_p_chi2_mc.clear();
	lepton_pdgid_chi2_mc.clear();

	
	btag_discriminator0.clear();
	btag_discriminator1.clear();
	btag_discriminator2.clear();
	
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

