/**_________________________________________________________________
   class:   TopHistograms.cc
   package: Analyzer/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopHistograms.cc,v 1.8 2007/06/07 14:10:24 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/TopTools/interface/TopHistograms.h"
#include "TF1.h"

Double_t Background(Double_t *x, Double_t *par)
  // The background function
{

	Double_t val = par[0] + par[1]*x[0] + par[2]*x[0]*x[0] + par[3]*x[0]*x[0]*x[0];
	//Double_t val = 
	return val;
}

Double_t Signal(Double_t *x, Double_t *par)
  // The signal function: a gaussian
{
  Double_t arg = 0;
  if (par[2]) arg = (x[0] - par[1])/par[2];

  Double_t sig = par[0]*TMath::Exp(-0.5*arg*arg);
  return sig;
}

Double_t Total(Double_t *x, Double_t *par)
  // Combined background + signal
{
  Double_t tot = Background(x,par) + Signal(x,&par[4]);
  return tot;
}


//_______________________________________________________________
TopHistograms::TopHistograms() {

}

//_______________________________________________________________
TopHistograms::~TopHistograms() {

}

//_______________________________________________________________
void TopHistograms::Init(TString type, TString suffix1, TString suffix2) {

	if (suffix1 != "") suffix1 = "_" + suffix1;
	if (suffix2 != "") suffix1 += "_" + suffix2;

	if ( type == "generator") {
		h2["gentop_rapidities"] = new TH2D("gentop_rapidities","y_{t} vs y_{T}",50,-3,3,50,-3,3);
	}
	else if ( type == "jets") {
		h1["jets"+suffix1]     = new TH1D("jets"+suffix1,"Number of jets",50,0,50);
		h1["jet_et"+suffix1]  = new TH1D("jet_et"+suffix1,"Jet E_{T} [GeV/c]",50,0,100);
		h1["jet_eta"+suffix1] = new TH1D("jet_eta"+suffix1,"Jet #eta",50,-5,5);
		h1["jet_phi"+suffix1] = new TH1D("jet_phi"+suffix1,"Jet #phi",50,-3.15,3.15);
	}
	else if ( type == "muons") {
		h1["muons"+suffix1]         = new TH1D("muons"+suffix1,"Number of muons",8,0,8);
		h1["muon_normchi2"+suffix1] = new TH1D("muon_normchi2"+suffix1,"#chi^{2}/ndof",40,0,30);
		h1["muon_pt"+suffix1]       = new TH1D("muon_pt"+suffix1,"Muon p_{T} [GeV/c]",50,0.0,100.0);
	}
	else if ( type == "MET") {
		h1["MET"+suffix1] = new TH1D("MET"+suffix1,"MET [GeV]",50,0.0,150.0);
		h1["METcomplex"+suffix1] = new TH1D("METcomplex"+suffix1,"MET [GeV]",50,0.0,150.0);
	}
	else {

		h1["phi_Wb"+suffix1] = new TH1D("phi_Wb"+suffix1,"Phi Difference of W(u+v) and J",50,0.0,3.2);    // phi balance t->W+b
	
		h1["kinfit_chi2"+suffix1] = new TH1D("kinfit_chi2"+suffix1,"#chi^{2} Probability",50,0.,1.);
	
		h1["nu_pz"+suffix1] = new TH1D("nu_pz"+suffix1,"Neutrino p_{z} [GeV/c]",50,-500.0,500.0);
	
		h1["WToMuNu"+suffix1] = new TH1D("WToMuNu"+suffix1,"(#mu + #nu) mass [GeV/c]",80,0.0,300.0);
		h1["tToWlnuj"+suffix1] = new TH1D("tToWlnuj"+suffix1,"(W_{l#nu} + jet) mass [GeV/c]",50,0.0,500.0);
		h1["tToWlnub"+suffix1] = new TH1D("tToWlnub"+suffix1,"(W_{l#nu} + jet) mass [GeV/c]",50,0.0,500.0);
		
		h1["WTojj"+suffix1] = new TH1D("WTojj"+suffix1,"(jet+jet) mass [GeV/c]",80,0.0,300.0);
		h1["tToWj"+suffix1] = new TH1D("tToWj"+suffix1,"(W_{jj} + jet) mass [GeV/c]",50,0.0,500.0);
		h1["WTojj_nob"+suffix1] = new TH1D("WTojj_nob"+suffix1,"(jet+jet) mass [GeV/c]",80,0.0,300.0);
		h1["tToWb"+suffix1] = new TH1D("tToWb"+suffix1,"(W_{jj} + jet) mass [GeV/c]",50,0.0,500.0);
	}

	for(std::map<TString,TH1* >::const_iterator ih=h1.begin(); ih!=h1.end(); ++ih){
		TH1 *htemp = ih->second;
		htemp->SetXTitle( htemp->GetTitle() );
	}
	for(std::map<TString,TH2* >::const_iterator ih=h2.begin(); ih!=h2.end(); ++ih){
		TH2 *htemp = ih->second;
		htemp->SetXTitle( htemp->GetTitle() );
	}
}

//_______________________________________________________________
void TopHistograms::Fill1d(TString name, Double_t x) {

	h1[name]->Fill(x);
}

//_______________________________________________________________
void TopHistograms::Fill2d(TString name, Double_t x, Double_t y) {

	h2[name]->Fill(x,y);
	
}

//_______________________________________________________________
void TopHistograms::Fit(TString name, Double_t mean) {

	Int_t npar = 7;
	Double_t params[7] = {700,mean,50.,350,6,0.5};
	TF1 *simple = new TF1("simple",Total,0,10,npar);
	simple->SetParameters(params);

	h1[name]->Fit("simple");
	
}


//_______________________________________________________________
void TopHistograms::Print(TString extension, TString tag) {

	if ( tag != "" ) tag = "_"+tag;
                
	for(std::map<TString,TCanvas*>::const_iterator icv=cv_map.begin(); icv!=cv_map.end(); ++icv){

		TString tmpname = icv->first;
		TCanvas *acv = icv->second;
		acv->Print(TString(tmpname+tag+"."+extension));
	}               

}

//_______________________________________________________________
void TopHistograms::SaveToFile(TString filename) {

	foutfile = new TFile(filename,"RECREATE");
	for(std::map<TString,TH1* >::const_iterator ih=h1.begin(); ih!=h1.end(); ++ih){
		TH1 *htemp = ih->second;
		htemp->Write();
	}
	for(std::map<TString,TH2* >::const_iterator ih=h2.begin(); ih!=h2.end(); ++ih){
		TH2 *htemp = ih->second;
		htemp->Write();
	}
                
	foutfile->Write();
	foutfile->Close();
	
}