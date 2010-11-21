#ifndef TopGenPdfInfo_h
#define TopGenPdfInfo_h

/**_________________________________________________________________
   class:   TopGenPdfInfo.h
   package:

________________________________________________________________**/
#include "TObject.h"

class TopGenPdfInfo : public TObject
{

 public:

  TopGenPdfInfo() {
    Reset();
  }
  ~TopGenPdfInfo() {}
  virtual void Reset();

  int id1,id2;
  double x1,x2,xPdf1,xPdf2,scalePDF;

  ClassDef(TopGenPdfInfo,1);

};

#endif

