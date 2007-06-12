#ifndef TopEvent_h
#define TopEvent_h

/**_________________________________________________________________
   class:   TopEvent.h
   package: Analyzers/TopAnalyzer
   

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopEvent.h,v 1.2 2007/05/25 19:35:34 yumiceva Exp $

________________________________________________________________**/


// C++ standard

// ROOT
#include "TObject.h"
#include "TMatrixDSym.h"
#include "TLorentzVector.h"

class TopEvent : public TObject {

  public:

	TopEvent();
	~TopEvent();

	void Reset();
	
    	//_______ event ID_______________________________
	Int_t event; // event number
	Int_t run;   // run number

	//_____ total number of reco objects _____________
	Int_t njets;        // number of jets
	Int_t nleptons;     // number of leptons
	Int_t nvertices;      // number of vertices
	//_____ total number of MC objects ______________
	Int_t ngenjets;     // number of generated jets
	Int_t ngenleptons;  // number of generated leptons

	//_____ jets ____________________________________
	std::vector< TLorentzVector > twobody;
	std::vector< TLorentzVector > threebody;
	std::vector< double > jet_px;
	std::vector< double > jet_py;
	std::vector< double > jet_pz;
	std::vector< double > jet_e;


	std::vector< double >      jet_btag_discriminant;

	std::vector< double > alljet_px;
	std::vector< double > alljet_py;
	std::vector< double > alljet_pz;
	std::vector< double > alljet_e;
	std::vector< double > alljet_et;
	std::vector< double > alljet_eta;
	std::vector< double > alljet_phi;
	
	std::vector< int >    jet_flavour;
	std::vector< double > jet_correction;
	
	//_____ generator _______________________________
	std::vector< double > gentop_px;
	std::vector< double > gentop_py;
	std::vector< double > gentop_pz;
	std::vector< double > gentop_e;
	std::vector< double > gentop_charge;
	std::vector< int > gentop_hadronic;
	std::vector< double > gennu_px;
	std::vector< double > gennu_py;
	std::vector< double > gennu_pz;
	std::vector< int > gennu_pdg;

	//_____ MC jets _________________________________
	std::vector< double > genjet_p;
	std::vector< double > genjet_pt;
	std::vector< double > genjet_eta;
	std::vector< double > genjet_phi;
	std::vector< double > genjet_et;
	std::vector< double > genjet_vx;
	std::vector< double > genjet_vy;
	std::vector< double > genjet_vz;
	
	//_____ leptons _________________________________
	std::vector< double > muon_px;
	std::vector< double > muon_py;
	std::vector< double > muon_pz;
	std::vector< double > muon_e;
	std::vector< double > allmuon_pt;
	std::vector< double > allmuon_normchi2;
	std::vector< double > allmuon_d0;
	std::vector< double > allmuon_d0Error;
	std::vector< int > allmuon_nrechits;
	
	//_____ MC leptons ______________________________
	std::vector< int > muon_mc_pdgid;
	std::vector< int > allmuon_mc_pdgid;
	
	std::vector< double > met;
	


	ClassDef(TopEvent,1);

};

#endif
