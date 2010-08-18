#ifndef TopBaseEvent_h
#define TopBaseEvent_h

/**_________________________________________________________________
   class:   TopBaseEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopBaseEvent.h,v 1.3 2009/07/30 06:02:19 jengbou Exp $

________________________________________________________________**/


// ROOT
#include "TObject.h"

class TopBaseEvent : public TObject
{

public:

    TopBaseEvent()
    {
        Reset();
    }
    ~TopBaseEvent() {}

    virtual void Reset();

	//___ kinematics _______________________________
    Float_t px;
    Float_t py;
    Float_t pz;
    Float_t pt;
    Float_t e;

    ClassDef(TopBaseEvent,1);

};

#endif

