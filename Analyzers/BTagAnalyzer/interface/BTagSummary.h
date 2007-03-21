#ifndef BTagSummary_h
#define BTagSummary_h

/**_________________________________________________________________
   class:   BTagSummary.h
   package: Analyzers/BTagAnalyzer
   

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: BTagSummary.h,v 1.1 2006/12/15 20:00:37 yumiceva Exp $

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
	Int_t ntracks;   // number of tracks
	Int_t nvertex;  // number of vertices

	std::vector< double > jet_pt;
	std::vector< double > jet_eta;
	std::vector< double > jet_phi;
	std::vector< double > jet_et;
	std::vector< double > jet_deltar;

	std::vector< double > muon_pt;
	std::vector< double > muon_eta;
	std::vector< double > muon_phi;
	std::vector< double > muon_charge;
	std::vector< double > muon_p;
	
	std::vector< double > jet_ptrel;
	std::vector< int > jet_flavour;
	
	std::vector< int > jet_btagged;
	std::vector< double > btag_discriminator0;
	std::vector< double > btag_discriminator1;
	std::vector< double > btag_discriminator2;
	

	ClassDef(BTagSummary,1);

};

#endif
