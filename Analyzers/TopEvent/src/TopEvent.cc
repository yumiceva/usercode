/**_________________________________________________________________
   class:   TopEvent.cc
   package: Analyzer/TopEvent
   

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopEvent.cc,v 1.3 2007/06/12 11:15:40 yumiceva Exp $

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

	passfilter = -1;
	
	// jets
	twobody.clear();
	threebody.clear();
	jet_px.clear();
	jet_py.clear();
	jet_pz.clear();
	jet_e.clear();
	jet_correction.clear();
	jet_btag_discriminant.clear();

	//alljet_px.clear();
	//alljet_py.clear();
	//alljet_pz.clear();
	//alljet_e.clear();
	//alljet_et.clear();
	//alljet_eta.clear();
	//alljet_phi.clear();
	
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
	gennu_e.clear();
	gennu_pdg.clear();

	genmuon_px.clear();
	genmuon_py.clear();
	genmuon_pz.clear();
	genmuon_e.clear();
	genmuon_pdg.clear();

	genjet_px.clear();
	genjet_py.clear();
	genjet_pz.clear();
	genjet_e.clear();
	genjet_pdg.clear();
	genjet_mother.clear();
	
	//genjet_p.clear();
	//genjet_pt.clear();
	//genjet_eta.clear();
	//genjet_phi.clear();
	//genjet_et.clear();

	muon_px.clear();
	muon_py.clear();
	muon_pz.clear();
	muon_e.clear();
	//allmuon_pt.clear();
	muon_normchi2.clear();
	muon_d0.clear();
	muon_d0Error.clear();
	muon_nSArechits.clear();
	muon_nTrkrechits.clear();
	
	met.clear();
	
	// MC
	muon_mc_pdgid.clear();
	//allmuon_mc_pdgid.clear();
	
}


