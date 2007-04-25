#ifndef TopEvent_h
#define TopEvent_h

/**_________________________________________________________________
   class:   TopEvent.h
   package: Analyzers/TopAnalyzer
   

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopEvent.h,v 1.2 2007/03/26 20:42:14 yumiceva Exp $

________________________________________________________________**/


// C++ standard

// ROOT
#include "TObject.h"
#include "TMatrixDSym.h"


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
	Int_t nvertex;      // number of vertices
	//_____ total number of MC objects ______________
	Int_t ngenjets;     // number of generated jets
	Int_t ngenleptons;  // number of generated leptons

	//_____ jets ____________________________________
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
	std::vector< int > jet_flavour;

	//_____ MC jets _________________________________
	std::vector< double > genjet_p;
	std::vector< double > genjet_pt;
	std::vector< double > genjet_eta;
	std::vector< double > genjet_phi;
	std::vector< double > genjet_et;
	std::vector< double > genjet_vx;
	std::vector< double > genjet_vy;
	std::vector< double > genjet_vz;
	std::vector< int >    genjet_flavour;
	
	//_____ leptons _________________________________
	std::vector< double > lepton_pt;
	std::vector< double > lepton_eta;
	std::vector< double > lepton_phi;
	std::vector< double > lepton_charge;
	std::vector< double > lepton_p;
	std::vector< double > lepton_chi2;
	std::vector< double > lepton_ndof;
	std::vector< double > lepton_d0;
	std::vector< double > lepton_d0sigma;
	std::vector< double > lepton_vx;
	std::vector< double > lepton_vy;
	std::vector< double > lepton_vz;
	std::vector< int >    lepton_pid;
	//_____ MC leptons ______________________________
	std::vector< double > lepton_chi2_hits_mc;
	std::vector< double > lepton_pt_hits_mc;
	std::vector< double > lepton_eta_hits_mc;
	std::vector< double > lepton_phi_hits_mc;
	std::vector< double > lepton_charge_hits_mc;
	std::vector< double > lepton_p_hits_mc;
	std::vector< int > lepton_pdgid_hits_mc;
	
	std::vector< double > lepton_chi2_chi2_mc;
	std::vector< double > lepton_pt_chi2_mc;
	std::vector< double > lepton_eta_chi2_mc;
	std::vector< double > lepton_phi_chi2_mc;
	std::vector< double > lepton_charge_chi2_mc;
	std::vector< double > lepton_p_chi2_mc;
	std::vector< double > lepton_vx_chi2_mc;
	std::vector< double > lepton_vy_chi2_mc;
	std::vector< double > lepton_vz_chi2_mc;
	std::vector< int > lepton_pdgid_chi2_mc;
	
	std::vector< int > jet_btagged;
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

	ClassDef(TopEvent,1);

};

#endif
