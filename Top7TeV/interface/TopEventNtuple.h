#ifndef TopEventNtuple_h
#define TopEventNtuple_h

/**_________________________________________________________________
   class:   TopEventNtuple.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopEventNtuple.h,v 1.3 2010/08/18 21:20:15 yumiceva Exp $

________________________________________________________________**/

#include <vector>

// ROOT
#include "TObject.h"

#ifdef NOSCRAMV
#include "TopMuonEvent.h"
#include "TopElectronEvent.h"
#include "TopJetEvent.h"
#include "TopVertexEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopMuonEvent.h"
#include "Yumiceva/Top7TeV/interface/TopElectronEvent.h"
#include "Yumiceva/Top7TeV/interface/TopJetEvent.h"
#include "Yumiceva/Top7TeV/interface/TopVertexEvent.h"
#endif


class TopEventNtuple : public TObject
{

public:

    TopEventNtuple();
    ~TopEventNtuple();

    void                 Reset();

	//_______ event ID_______________________________
    Int_t event;       // event number
    Int_t run;         // run number
    Int_t lumi;        // luminosity section
    Int_t dataType;    // type of data: MC, colisions,
	
    //_______ event counters_________________________
    //Int_t njets;       // number of jets
    //Int_t nmuons;      // number of muons
    //Int_t nelectrons;  // number of electrons
    //Int_t nvertices;   // number of vertices

    //______ muons ________________________________
    std::vector< TopMuonEvent > muons;
    
    //_____ jets __________________________________
    std::vector< TopJetEvent > jets;
    
    //_____ electrons _____________________________
    std::vector< TopElectronEvent > electrons;
    
    //_____ vertices ______________________________
    std::vector< TopVertexEvent > vertices;
    
    //_____ Event variables ______________________
    Float_t MET;
    Float_t METeta;
    Float_t METphi;
    Float_t Ht;
    
    //_____ jet combinations _______________________
    Float_t M3;
    Float_t chi2sorting;
    
    
    ClassDef(TopEventNtuple,1);

};

#endif
