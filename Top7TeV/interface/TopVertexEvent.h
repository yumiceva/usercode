#ifndef TopVertexEvent_h
#define TopVertexEvent_h

/**_________________________________________________________________
   class:   TopVertexEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopVertexEvent.h,v 1.1 2010/08/18 16:28:42 yumiceva Exp $

________________________________________________________________**/

#include <vector>

// ROOT
#include "TObject.h"

#ifdef NOSCRAMV
#include "TopMuonEvent.h"
#include "TopElectronEvent.h"
//#include "TopJetEvent.h"
//#include "TopVertexEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopMuonEvent.h"
#include "Yumiceva/Top7TeV/interface/TopElectronEvent.h"
#include "Yumiceva/Top7TeV/interface/TopJetEvent.h"
//#include "Yumiceva/Top7TeV/interface/TopVertexEvent.h"
#endif


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
