/**_________________________________________________________________
   class:   BTagSummary.cc
   package: Validation/RecoVertex
   

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: MySummary.cc,v 1.1 2006/12/15 20:00:37 yumiceva Exp $

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
	ntracks     = -1;
	
	jet_pt.clear();
	jet_eta.clear();
	jet_phi.clear();
	jet_et.clear();
	jet_deltar.clear();

	muon_pt.clear();
	muon_eta.clear();
	muon_phi.clear();
	muon_charge.clear();
	muon_p.clear();
	
	jet_ptrel.clear();
	jet_flavour.clear();
	
	jet_btagged.clear();
	btag_discriminator0.clear();
	btag_discriminator1.clear();
	btag_discriminator2.clear();
	
	
}


