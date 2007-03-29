
#include "TF1.h"
#include "TCanvas.h"
#include "TString.h"


Double_t beamwidth( Double_t *x, Double_t *parm ) {

	//const double emmitance = 3.75e-8;
	//const double betastar = 55.;
	//const double z0 = 0.;
	//double x = sqrt(emmitance*(betastar+(((z-z0)*(z-z0))/betastar)));
	//return x;
	return sqrt(parm[0]*(parm[1]+(x[0]*x[0]/parm[1])));
}


TF1* get_beamwidth(double betastar,TString name="f1") {

	TF1* f1 = new TF1(name,beamwidth,-30,30,2);

	f1->SetParameter(0,3.75e-8);
	f1->SetParameter(1,betastar);

	return f1;
	//TCanvas* cv1 = new TCanvas("cv1","cv1",700,700);

	//f1->Draw(option);
	

}
