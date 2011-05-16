#ifndef TopEventNtuple_h
#define TopEventNtuple_h

/**_________________________________________________________________
   class:   TopEventNtuple.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopEventNtuple.h,v 1.11 2011/05/04 22:37:19 yumiceva Exp $

________________________________________________________________**/

#include <vector>

// ROOT
#include "TObject.h"

#ifdef NOSCRAMV
#include "TopMuonEvent.h"
#include "TopElectronEvent.h"
#include "TopJetEvent.h"
#include "TopVertexEvent.h"
#include "TopMyGenEvent.h"
#include "TopGenPdfInfo.h"
#include "TopTrigger.h"
#else
#include "Yumiceva/Top7TeV/interface/TopMuonEvent.h"
#include "Yumiceva/Top7TeV/interface/TopElectronEvent.h"
#include "Yumiceva/Top7TeV/interface/TopJetEvent.h"
#include "Yumiceva/Top7TeV/interface/TopVertexEvent.h"
#include "Yumiceva/Top7TeV/interface/TopMyGenEvent.h"
#include "Yumiceva/Top7TeV/interface/TopGenPdfInfo.h"
#include "Yumiceva/Top7TeV/interface/TopTrigger.h"
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
    
    std::vector< TopTrigger > triggers;

    //_______ event counters_________________________
    //Int_t njets;       // number of jets
    //Int_t nmuons;      // number of muons
    //Int_t nelectrons;  // number of electrons
    //Int_t nvertices;   // number of vertices

    //______ muons ________________________________
    std::vector< TopMuonEvent > muons;
    
    //_____ jets __________________________________
    //std::vector< TopJetEvent > Calojets;
    //std::vector< TopJetEvent > JPTjets;
    std::vector< TopJetEvent > PFjets;
    //std::vector< TopJetEvent > PFlowjets;

    //_____ electrons _____________________________
    std::vector< TopElectronEvent > electrons;
    std::vector< TopElectronEvent > PFelectrons;

    //_____ vertices ______________________________
    std::vector< TopVertexEvent > vertices;
    
    //_____ Event variables ______________________
    //Float_t CaloMET;
    //Float_t CaloMETphi;
    //Float_t CaloHt;
    //Float_t tcMET;
    //Float_t tcMETphi;
    //Float_t tcHt;
    Float_t PFMET;
    Float_t PFMETphi;
    Float_t PFHt;
    //Float_t PFlowMET;
    //Float_t PFlowMETphi;
    //Float_t PFlowHt;

    Int_t IsZevent; // for e+jets

    Double_t rho;
    Double_t rmsrho;

    Int_t flavorHistory; // MC flag
    TopMyGenEvent gen; // MC generator info for ttbar
    TopGenPdfInfo genPdf; // MC PDF info

    //_____ jet combinations _______________________
    //Float_t M3;
    //Float_t chi2sorting;
    
    
    ClassDef(TopEventNtuple,1);

};

#endif
