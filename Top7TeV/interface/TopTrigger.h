#ifndef TopTrigger_h
#define TopTrigger_h

/**_________________________________________________________________
   class:   TopTrigger.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopTrigger.h,v 1.5 2010/10/01 23:06:37 yumiceva Exp $

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
  Bool_t pass;

  ClassDef(TopTrigger,1);

};

#endif
