#ifndef TopTrigger_h
#define TopTrigger_h

/**_________________________________________________________________
   class:   TopTrigger.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopTrigger.h,v 1.1 2011/05/04 22:37:19 yumiceva Exp $

 ________________________________________________________________**/


#include "TObject.h"
#include <string>

class TopTrigger : public TObject
{

 public:

  TopTrigger() 
    {
      Reset();
    }
  ~TopTrigger() {}

  void Reset();

  //___
  std::string name;
  UInt_t prescale;
  Bool_t accept;

  ClassDef(TopTrigger,1);

};

#endif
