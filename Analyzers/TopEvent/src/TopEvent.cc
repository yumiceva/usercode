/**_________________________________________________________________
   class:   TopEvent.cc
   package: Analyzer/TopEvent
   

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopEvent.cc,v 1.2 2007/05/25 19:35:35 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/TopEvent/interface/TopEvent.h"

ClassImp(TopEvent)
	
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
	nvertices     = -1;
	// MC
	ngenjets    = -1;
	ngenleptons = -1;

	// jets
	twobody.clear();
	threebody.clear();
	jet_px.clear();
	jet_py.clear();
	jet_pz.clear();
	jet_e.clear();
	jet_correction.clear();
	jet_btag_discriminant.clear();

	alljet_px.clear();
	alljet_py.clear();
	alljet_pz.clear();
	alljet_e.clear();
	alljet_et.clear();
	alljet_eta.clear();
	alljet_phi.clear();
	
	jet_flavour.clear();
	
	//_____ generator _______________________________
	gentop_px.clear();
	gentop_py.clear();
	gentop_pz.clear();
	gentop_e.clear();
	gentop_charge.clear();
	gentop_hadronic.clear();
	gennu_px.clear();
        gennu_py.clear();
        gennu_pz.clear();
        gennu_pdg.clear();

	genjet_p.clear();
	genjet_pt.clear();
	genjet_eta.clear();
	genjet_phi.clear();
	genjet_et.clear();

	muon_px.clear();
	muon_py.clear();
	muon_pz.clear();
	muon_e.clear();
	allmuon_pt.clear();
	allmuon_normchi2.clear();
	allmuon_d0.clear();
	allmuon_d0Error.clear();
	allmuon_nrechits.clear();
	
	met.clear();
	
	// MC
	muon_mc_pdgid.clear();
	allmuon_mc_pdgid.clear();
	
}


