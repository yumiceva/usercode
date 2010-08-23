#ifndef TopGenJetEvent_h
#define TopGenJetEvent_h

/**_________________________________________________________________
   class:   TopGenJetEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopGenJetEvent.h,v 1.1 2010/08/18 21:13:05 yumiceva Exp $

________________________________________________________________**/

// ROOT
#include "TObject.h"


class TopGenJetEvent : public TObject
{

 public:

  TopGenJetEvent() {
    Reset();
  }
  ~TopGenJetEvent() {}
  
  void Reset();
  
  Float_t pt;
  Float_t eta;
  Float_t phi;
  Float_t e;

  Int_t flavor;
  
  ClassDef(TopGenJetEvent,1);
    
};

#endif
