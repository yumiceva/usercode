#ifndef TopMyGenEvent_h
#define TopMyGenEvent_h

/**_________________________________________________________________
   class:   TopMyGenEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopMyGenEvent.h,v 1.3 2011/10/15 03:25:29 yumiceva Exp $

________________________________________________________________**/

#include "TObject.h"

class TopMyGenEvent : public TObject
{

public:

  TopMyGenEvent()
    {
      Reset();
    }
  ~TopMyGenEvent() {}

  virtual void Reset();
  
  //___
  Int_t isSemiLeptonic;
  Int_t isDilepton;
  Int_t LeptonicChannel;
  Int_t isWevent; //only used for W+jets and Vqq sample for the moment

  Int_t Bx_minus1;
  Int_t Bx_0;
  Int_t Bx_plus1;

  Float_t t_pt;
  Float_t t_eta;
  Float_t t_phi;
  Float_t t_e;
  Float_t tbar_pt;
  Float_t tbar_eta;
  Float_t tbar_phi;
  Float_t tbar_e;
  Float_t nu_pt;
  Float_t nu_eta;
  Float_t nu_phi;
  Float_t nu_e;
  Float_t mu_pt;
  Float_t mu_eta;
  Float_t mu_phi;
  Float_t mu_e;
  Float_t Wp_pt;
  Float_t Wp_eta;
  Float_t Wp_phi;
  Float_t Wp_e;
  Float_t Wq_pt;
  Float_t Wq_eta;
  Float_t Wq_phi;
  Float_t Wq_e;
  Float_t bHad_pt;
  Float_t bHad_eta;
  Float_t bHad_phi;
  Float_t bHad_e;
  Float_t bLep_pt;
  Float_t bLep_eta;
  Float_t bLep_phi;
  Float_t bLep_e;
  Float_t MET;
  Float_t METPhi;

  ClassDef(TopMyGenEvent,1);

};

#endif

