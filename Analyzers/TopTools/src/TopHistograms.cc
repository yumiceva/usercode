/**_________________________________________________________________
   class:   TopHistograms.cc
   package: Analyzer/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopHistograms.cc,v 1.1 2007/05/25 19:47:13 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/TopTools/interface/TopHistograms.h"


//_______________________________________________________________
TopHistograms::TopHistograms() {

}

//_______________________________________________________________
TopHistograms::~TopHistograms() {

}

//_______________________________________________________________
TopHistograms::Init() {

	h1["jets"]    = new TH1D("jets","Number of jets",50,0,50);

	h1["muons"] = new TH1D("muons","Number of muons",8,0,8);
	h1["muons_normchi2"] = new TH1D("allmuon_normchi2","#chi^{2}/ndof",40,0,30);
	h1["muons_normchi2_MC"] = new TH1D("allmuon_normchi2_MC","#chi^{2}/ndof",40,0,30);
	
	h1["allmuon_pt"] = new TH1D("allmuon_pt","Muon p_{T} [GeV/c]",50,0,100);
	h1["allmuon_pt_MC"] = new TH1D("allmuon_pt_MC","Muon p_{T} [GeV/c]",50,0,100);
	h1["allmuon_d0"] = new TH1D("allmuon_d0","Muon d_{0} [cm]",35,-1,1);
	h1["allmuon_d0_MC"] = new TH1D("allmuon_d0_MC","Muon d_{0} [cm]",35,-1,1);
	h1["allmuon_ips"] = new TH1D("allmuon_ips","Muon d_{0}/#sigma_{d0}",50,-60,60);
	h1["allmuon_ips_MC"] = new TH1D("allmuon_ips_MC","Muon d_{0}/#sigma_{d0}",50,-60,60);
	
	h1["alljet_et"] = new TH1D("alljet_et","Jet E_{T} [GeV/c]",50,0,100);
	h1["alljet_eta"] = new TH1D("alljet_eta","Jet #eta",50,-5,5);
	h1["alljet_phi"] = new TH1D("alljet_phi","Jet #phi",50,-3.15,3.15);
	

	h1["MET"] = new TH1D("MET","MET [GeV]",50,0.0,150.0);
	h1["METcomplex"] = new TH1D("METcomplex","MET [GeV]",50,0.0,150.0);
	h1["phi_Wb"] = new TH1D("phi_Wb","Phi Difference of W(u+v) and J",50,0.0,3.2);    // phi balance t->W+b
	
	h1["kinfit_chi2"] = new TH1D("kinfit_chi2","#chi^{2} Probability",50,0.,1.);
	
	h1["muon_pt"] = new TH1D("muon_pt","Muon p_{T} [GeV/c]",50,0.0,100.0);
	h1["nu_pz"] = new TH1D("nu_pz","Neutrino p_{z} [GeV/c]",50,-500.0,500.0);
	
	h1["mu_nu"] = new TH1D("mu_nu","(#mu + #nu) mass [GeV/c]",80,0.0,300.0);

}

//_______________________________________________________________
TopHistograms::fill1d(TString name, Double_t x) {

	h1[name]->Fill(x);
}

//_______________________________________________________________
TopHistograms::fill1d(TString name, Double_t x) {

	h1[name]->Fill(x);
}

//_______________________________________________________________
TopHistograms::fill2d(TString name, Double_t x, Double_t y) {

	h2[name]->Fill(x,y);
	
}


