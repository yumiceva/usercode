#ifndef BEAMFIT_FCTS_H
#define BEAMFIT_FCTS_H

#include <TROOT.h>

void zfcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *params, Int_t iflag);
Double_t zdis(Double_t z, Double_t sigma, Double_t *parms);

#endif   // BEAMFIT_FCTS
