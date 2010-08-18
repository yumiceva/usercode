#ifndef TopCandidateEvent_h
#define TopCandidateEvent_h

/**_________________________________________________________________
   class:   TopCandidateEvent.h
   package:


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopCandidateEvent.h,v 1.1 2010/08/18 16:28:42 yumiceva Exp $

________________________________________________________________**/


// ROOT
#include "TObject.h"

class TopCandidateEvent : public TObject
{

public:

    TopCandidateEvent()
    {
        Reset();
    }
    ~TopCandidateEvent() {}

    virtual void Reset();

	//___ kinematics _______________________________
    Float_t eta;
    Float_t phi;
    Float_t pt;
    Float_t e;

    Float_t vx;
    Float_t vy;
    Float_t vz;

    ClassDef(TopCandidateEvent,1);

};

#endif

