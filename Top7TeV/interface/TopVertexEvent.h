#ifndef TopVertexEvent_h
#define TopVertexEvent_h

/**_________________________________________________________________
   class:   TopVertexEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopVertexEvent.h,v 1.2 2010/08/23 01:48:56 yumiceva Exp $

________________________________________________________________**/

// ROOT
#include "TObject.h"


class TopVertexEvent : public TObject
{

 public:

  TopVertexEvent() {
    Reset();
  }
  ~TopVertexEvent() {}
  
  void Reset();
  
  Float_t vx;
  Float_t vy;
  Float_t vz;
  Float_t ndof;
  Float_t rho;
  Int_t IsGood;

  ClassDef(TopVertexEvent,1);
    
};

#endif
