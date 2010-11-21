/**_________________________________________________________________
   class:   TopGenPdfInfo.cc
   package: 

________________________________________________________________**/

#ifdef NOSCRAMV
#include "TopGenPdfInfo.h"
#else
#include "Yumiceva/Top7TeV/interface/TopGenPdfInfo.h"
#endif

#include <math.h>

ClassImp(TopGenPdfInfo)

//_______________________________________________________________
void TopGenPdfInfo::Reset()
{
  id1 = -99;
  id2 = -99;
  x1 = -99.;
  x2 = -99.;
  xPdf1 = -99.;
  xPdf2 = -99.;
  scalePDF = -99.;
}
