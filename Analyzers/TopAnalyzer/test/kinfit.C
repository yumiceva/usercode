
#include "kinfit.h"



kinfit::kinfit() {
}

kinfit::~kinfit() {
}

void kinfit::iteration(int i, int nf, int nm, int ny,
					   TMatrixD f, TMatrixD dfdm, TMatrixD dfdy, TMatrixD C, TMatrixD GI) {

	if (i ==0 ) {
		TMatrixD R;
		R.ResizeTo(f);
		R = f + dfdy * C;
		TMatrixD Rtmp = R;
		
		TMatrixD dfdmt;
		dfdmt.Transpose(dfdm);
		
		TMatrixD S;
		S = dfdm * GI * dfdmt;
	
		if (ny==0) {
			
		}
		
		TMatrixD SI;
		SI = S.Invert();
		
		TMatrixD lam;
		lam = SI * Rtmp;
		
		TMatrixD lamt;
		lamt.Transpose(lam);
	
		TMatrixD chi2;
		chi2 = lamt * Rtmp;

		TMatrixD Cp;
		Cp = GI * dfdmt * lam;
		Cp = (-1.) * Cp;

		// store values
		Cp_ = Cp;
		chi2_ = chi2;
		GmI_.UnitMatrix();
		
	} else {

	}
	
}

void kinfit::Fit() {

	int nf = 4; // mass of W and t
	int nm = 4; // only energies poorly measured
	int nx = 1; // longitudinal neutrino momentum
	int kfl = 0; //failure flag for max steps, nstep > 10

	// initial unknown wiggles and measured wiggles
	TMatrixD dx(nx,1);
	TMatrixD C(nm,1);

	TMatrixD GI(nm,nm);

	GI(0,0) = ej1Err_;
	GI(1,1) = ej2Err_;
	GI(2,2) = eb1Err_;
	GI(3,3) = eb2Err_;

	
}



	

		

