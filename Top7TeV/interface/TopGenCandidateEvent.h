#ifndef TopGenCandidateEvent_h
#define TopGenCandidateEvent_h

/**_________________________________________________________________
   class:   TopGenCandidateEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopGenCandidateEvent.h,v 1.2 2010/08/18 21:13:05 yumiceva Exp $

________________________________________________________________**/

#include "TObject.h"

class TopGenCandidateEvent : public TObject
{

public:

  TopGenCandidateEvent()
    {
      Reset();
    }
  ~TopGenCandidateEvent() {}

  virtual void Reset();
  
  //___
  Int_t pdgId;
  Int_t mom_pdgId;
  
  Float_t mom_pt;
  Float_t mom_eta;
  Float_t mom_phi;

  ClassDef(TopGenCandidateEvent,1);

};

#endif

