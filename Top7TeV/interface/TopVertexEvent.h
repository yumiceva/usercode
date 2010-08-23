#ifndef TopVertexEvent_h
#define TopVertexEvent_h

/**_________________________________________________________________
   class:   TopVertexEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopVertexEvent.h,v 1.1 2010/08/18 21:13:05 yumiceva Exp $

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
    
  ClassDef(TopVertexEvent,1);
    
};

#endif
