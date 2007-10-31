
#include "Analyzers/TopTools/interface/SimpleKinFit.h"
#include <iostream>


SimpleKinFit::SimpleKinFit() {
	
	// initial values
	MW_ = 80.4;
	Mt_ = 175.0;

	cp_.ResizeTo(4,1);
	dx_.ResizeTo(1,1);
	chi_.ResizeTo(1,1);
	gxi_.ResizeTo(1,1);
	gmfi_.ResizeTo(4,4);

	cp_.Zero(); dx_.Zero(); chi_.Zero(); gxi_.Zero(); gmfi_.Zero();
	
	elErr_ = eb1Err_ = eb2Err_ = ej1Err_ = ej2Err_ = 0.;
	fitNuPz_ = fitEb1_ = fitEb2_ = fitEj1_ = fitEj2_ = 0.;
	
}

SimpleKinFit::~SimpleKinFit() {
}

void SimpleKinFit::iteration(int i, int nf, int nm, int nx,
					   TMatrixD f, TMatrixD a, TMatrixD b, TMatrixD C, TMatrixD GI) {


	//std::cout << " GI in iteration: " << std::endl;
	//GI.Print();

	if (i ==0 ) {
		TMatrixD R;
		R.ResizeTo(f);
		R = f - b * C;
		TMatrixD dum;
		dum.ResizeTo(f);
		dum = R;
		
		TMatrixD bt(4,4);
		bt.Transpose(b);
		//std::cout << "KinFit::iteration got transpose of b" << std::endl;
		
		TMatrixD h(4,4);
		h = b * GI * bt;
		
		TMatrixD si(4,4);
		//std::cout << "h = " << std::endl;
		//h.Print();
		//std::cout << "det(h)= " << h.Determinant() << std::endl;
		Double_t deth;
		si = h;
		si.Invert(&deth);
		//std::cout << "si inverted" << std::endl;
		
		if (nx==0) {
			TMatrixD at(1,4);
			at.Transpose(a);
			TMatrixD hh(1,1);
			hh =  at * si * a;
			Double_t dethh;
			gxi_ = hh;
			gxi_.Invert(&dethh);
			dx_ = (-1.0)*gxi_*at*si*R;
			dum = R + a * dx_;
			
		}
		
		TMatrixD al(4,1);
		al = si*dum;
		TMatrixD alt(1,4);
		alt.Transpose(al);
		chi_ = alt * dum;
		cp_ = (-1.0)*GI * bt * al;
		gmfi_.UnitMatrix();
				
	} else {
		TMatrixD R;
		R.ResizeTo(f);
		R = f - b * C;
		
		TMatrixD bt(4,4);
		bt.Transpose(b);
		
		TMatrixD h(4,4);
		h = b * GI * bt;
		
		TMatrixD si(4,4);
		Double_t deth;
		si = h;
		si.Invert(&deth);
						
		TMatrixD hhh(4,4);
		hhh = GI * bt * si * b * GI;
		
		gmfi_ = GI - hhh;
		
		if (nx == 0 ) {
			TMatrixD at(1,4);
			at.Transpose(a);
			TMatrixD hh(1,1);
			hh =  at * si * a;
			Double_t detgi;
			gxi_ = GI;
			gxi_.Invert(&detgi);
			TMatrixD dum(4,4);
			dum = GI * bt * si * a;
			TMatrixD dumt(4,4);
			dumt.Transpose(dum);
			gmfi_ = gmfi_ + dum*gxi_*dumt;
		}
		
	}
	
}

void SimpleKinFit::Fit() {

	int nf = 4; // mass of W and t
	int nm = 4; // only energies poorly measured
	int nx = 1; // longitudinal neutrino momentum
	int kfl = 0; //failure flag for max steps, nstep > 10

	// initial unknown wiggles and measured wiggles
	//TMatrixD dx(nx,1);
	dx_.Zero();
	TMatrixD C(nm,1);

	TMatrixD emo(4,1);
	emo(0,0) = pj1_.E();
	emo(1,0) = pj2_.E();
	emo(2,0) = pb2_.E();
	emo(3,0) = pb1_.E();

	// direction
	TLorentzVector Ab1 = (1./pb1_.E()) * pb1_;
	TLorentzVector Ab2 = (1./pb2_.E()) * pb2_;
	TLorentzVector Aj1 = (1./pj1_.E()) * pj1_;
	TLorentzVector Aj2 = (1./pj2_.E()) * pj2_;
	
	// error matrix
	//double acal = 1.3;
	//double bcal = 0.1;
	//ej1Err_ = (bcal * bcal * pj1_.E() * pj1_.E() + acal * acal * pj1_.E() );
	//ej2Err_ = (bcal * bcal * pj2_.E() * pj2_.E() + acal * acal * pj2_.E() );
	//eb2Err_ = (bcal * bcal * pb2_.E() * pb2_.E() + acal * acal * pb2_.E() );
	//eb1Err_ = (bcal * bcal * pb1_.E() * pb1_.E() + acal * acal * pb1_.E() );
	
	TMatrixD GI(nm,nm);
	GI(0,0) = ej1Err_;
	GI(1,1) = ej2Err_;
	GI(2,2) = eb2Err_;
	GI(3,3) = eb1Err_;
	//std::cout << " GI= " <<std::endl;
	//GI.Print();

	/*
	// starting value of MET
	TLorentzVector p4MET;
	p4MET += - TLorentzVector(pb1_.Px(),pb1_.Py(),0.,0.);
	p4MET += - TLorentzVector(pb2_.Px(),pb2_.Py(),0.,0.);
	p4MET += - TLorentzVector(pj1_.Px(),pj1_.Py(),0.,0.);
	p4MET += - TLorentzVector(pj2_.Px(),pj2_.Py(),0.,0.);
	p4MET += - TLorentzVector(pl_.Px(), pl_.Py(),0.,0.);

	//std::cout << "KinFit:: p4METx= "<< p4MET.Px() << " p4METy= " << p4MET.Py()
	//<< " p4METz= " << p4MET.Pz() << std::endl;
	
	TLorentzVector p4Nu;
	p4Nu.SetPz( GetPzNu(pl_, p4MET) );
	p4Nu.SetE( TMath::Sqrt(p4Nu.Px()*p4Nu.Px() + p4Nu.Py()*p4Nu.Py() + p4Nu.Pz()*p4Nu.Pz()) );

	*/
	
	TMatrixD x(nx,1);
	x(0,0) = pznu_; //p4Nu.Pz();

	//TMatrixD gxi(nx,nx);
	gxi_.Zero();
	TMatrixD si(nf,nf);

	int nstep = 0;
	double flim = 0.20;
	double fabs = 1.0;
	
	TMatrixD em(4,1);
	TMatrixD f(nf,1);
	TMatrixD a(nf,nx);
	TMatrixD b(nf,nm);
	
	//std::cout << "KinFit::Fit start while loop " << std::endl;
	
	while ( fabs > flim ) {
	  // wiggles measures and unkonws by corrections
	  em = emo + C;
	  x = x + dx_;

	  TLorentzVector tmpvecj1 = em(0,0)*Aj1;
	  TLorentzVector tmpvecj2 = em(1,0)*Aj2;
	  TLorentzVector tmpvecb2 = em(2,0)*Ab2;
	  TLorentzVector tmpvecb1 = em(3,0)*Ab1;
	  TLorentzVector tmpvecnu = (-1.0)* (pl_ + tmpvecj1 + tmpvecj2 + tmpvecb2 + tmpvecb1 );
	  tmpvecnu.SetPz(x(0,0));
	  tmpvecnu.SetE(tmpvecnu.P());
	  
	  TLorentzVector tmpvec;
	  tmpvec = tmpvecj1 + tmpvecj2;
	  f(0,0) = tmpvec.M() - MW_;
	  tmpvec = tmpvecj1 + tmpvecj2 + tmpvecb2;
	  f(1,0) = tmpvec.M() - Mt_;
	  tmpvec = pl_ + tmpvecnu;
	  f(2,0) = tmpvec.M() - MW_;
	  tmpvec = pl_ + tmpvecnu + tmpvecb1;
	  f(3,0) = tmpvec.M() - Mt_;

	  fabs = 0;
	  for ( int j = 0; j<nf; ++j) {
	    fabs = fabs + TMath::Abs( f(j,0) );
	  }
	  nstep++;
	  if ( nstep > 100 ) break;
	  
	  // loadup derivative matrices
	  b.Zero();
	  a.Zero();
	  //TMatrixD b(nf,nm); // dfdm(nf,nm)
	  //TMatrixD a(nf,nx); // df/dx(nf,nx)

	  // evaluate derivatives of f wrt em and x numberically

	  TMatrixD h(1,1);
	  h(0,0) = 0.1; // 100 MeV step

	  x = x + h;
	  tmpvecnu = (-1.0)* (pl_ + tmpvecj1 + tmpvecj2 + tmpvecb2 + tmpvecb1 );
	  tmpvecnu.SetPz(x(0,0));
	  tmpvecnu.SetE(tmpvecnu.P());
	  
	  double f00 = f(0,0);
	  double f10 = f(1,0);
	  tmpvec = pl_ + tmpvecnu;
	  double f20 = tmpvec.M() - MW_;
	  tmpvec = pl_ + tmpvecnu + tmpvecb1;
	  double f30 = tmpvec.M() - Mt_;

	  x = x - h;
	  a(0,0) = (f00 - f(0,0))/h(0,0);
	  a(1,0) = (f10 - f(1,0))/h(0,0);
	  a(2,0) = (f20 - f(2,0))/h(0,0);
	  a(3,0) = (f30 - f(3,0))/h(0,0);

	  for ( int i = 0; i<nm; ++i ) {
		  em(i,0) = em(i,0) + h(0,0);	
	    
		  tmpvecj1 = em(0,0)*Aj1;
		  tmpvecj2 = em(1,0)*Aj2;
		  tmpvecb2 = em(2,0)*Ab2;
		  tmpvecb1 = em(3,0)*Ab1;		  
		  tmpvecnu = (-1.0)* (pl_ + tmpvecj1 + tmpvecj2 + tmpvecb2 + tmpvecb1 );
		  tmpvecnu.SetPz(x(0,0));
		  tmpvecnu.SetE(tmpvecnu.P());
		  
		  tmpvec = tmpvecj1 + tmpvecj2;
		  f00 = tmpvec.M() - MW_;
		  tmpvec = tmpvecj1 + tmpvecj2 + tmpvecb2;
		  f10 = tmpvec.M() - Mt_;
		  tmpvec = pl_ + tmpvecnu;
		  f20 = tmpvec.M() - MW_;
		  tmpvec = pl_ + tmpvecnu + tmpvecb1;
		  f30 = tmpvec.M() - Mt_;
		  		  
		  em(i,0) = em(i,0) - h(0,0);

		  b(0,i) = (f00 - f(0,0))/h(0,0);
		  b(1,i) = (f10 - f(1,0))/h(0,0);
		  b(2,i) = (f20 - f(2,0))/h(0,0);
		  b(3,i) = (f30 - f(3,0))/h(0,0);

	  }
	  
	  //std::cout << "KinFit::Fit() calling iteration() " << std::endl;
	  iteration( 0, nf, nm, nx,
			  f, a, b, C, GI);
 
	  C = cp_;

	}/// while loop
	
	if ( nstep > 10 ) {
		kfl = 1;
	}
	
	iteration(1,nf, nm, nx, f, a , b , C, GI);
	
	fitEj1_ = em(0,0);
	fitEj2_ = em(1,0);
	fitEb2_ = em(2,0);
	fitEb1_ = em(3,0);
	fitNuPz_ = x(0,0);

	//fitNuPzErr_ = TMath::Sqrt( gxi_(0,0) );
	fitEj1Err_ = TMath::Sqrt( gmfi_(0,0) );
	fitEj2Err_ = TMath::Sqrt( gmfi_(1,0) );
	fitEb2Err_ = TMath::Sqrt( gmfi_(2,0) );
	fitEb1Err_ = TMath::Sqrt( gmfi_(3,0) );
	
}

/*
Double_t KinFit::GetPzNu(TLorentzVector p4Muon,  TLorentzVector p4MET  ) {
  // solve for a W using muon and MET
  double M_W  = 80.4;
  double M_mu =  0.105658;
  double emu = p4Muon.E();
  double pxmu = p4Muon.Px();
  double pymu = p4Muon.Py();
  double pzmu = p4Muon.Pz();
  double pxnu = p4MET.Px();
  double pynu = p4MET.Py();
  double pznu = 0.;

  double a = M_W*M_W - M_mu*M_mu + 2.*pxmu*pxnu + 2.*pymu*pynu;
  double A = 4.*(emu*emu - pzmu*pzmu);
  double B = -4.*a*pzmu;
  double C = 4.*emu*emu*(pxnu*pxnu + pynu*pynu) - a*a;

  double tmproot = B*B - 4.*A*C;
  if (tmproot<0) pznu = - B/(2*A); // take real part of complex roots
  else {
    double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2*A);
    double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2*A);
    // two real roots, pick one with pz closest to muon
    if (TMath::Abs(tmpsol2-pzmu) < TMath::Abs(tmpsol1-pzmu)) pznu = tmpsol2;
    else pznu = tmpsol1;
  }

  return pznu;
};

*/
	

		

