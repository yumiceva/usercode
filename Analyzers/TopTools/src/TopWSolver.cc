/**_________________________________________________________________
   class:   TopWSolver.cc
   package: Analyzer/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopWSolver.cc,v 1.1 2007/04/25 03:53:54 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/TopTools/interface/TopWSolver.h"

#include "TMath.h"

//_______________________________________________________________
TopWSolver::TopWSolver() {

	complex_ = false;
	pzNu_ = 0.;
	type_=0;
	
}

//_______________________________________________________________
TopWSolver::~TopWSolver() {	
}


//_______________________________________________________________
Double_t TopWSolver::GetPzNu(int type) {

	// solve for a W using muon and MET
	
	double M_W  = 80.4;
	double M_mu =  0.10566;
	double emu = p4Muon_.E();
	double pxmu = p4Muon_.Px();
	double pymu = p4Muon_.Py();
	double pzmu = p4Muon_.Pz();
	double pxnu = p4MET_.Px();
	double pynu = p4MET_.Py();
	double pznu = 0.;
		
	double a = M_W*M_W - M_mu*M_mu + 2.0*pxmu*pxnu + 2.0*pymu*pynu;
	double A = 4.0*(emu*emu - pzmu*pzmu);
	double B = -4.0*a*pzmu;
	double C = 4.0*emu*emu*(pxnu*pxnu + pynu*pynu) - a*a;

	double tmproot = B*B - 4.0*A*C;
		
	if (tmproot<0) {
		complex_= true;
		if (type==0) pznu = - B/(2*A); // take real part of complex roots
		
		//std::cout<< "complex sol. tmproot=" << tmproot << " pznu=" << pznu << std::endl;
	}
	else {
		double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2.0*A);
		double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2.0*A);

		if (type == 0 ) {
			// two real roots, pick one with pz closest to muon
			if (TMath::Abs(tmpsol2-pzmu) < TMath::Abs(tmpsol1-pzmu)) { pznu = tmpsol2;}
			else pznu = tmpsol1;
		}
	}

	type_ = type;
	return pznu;
	
		
}

