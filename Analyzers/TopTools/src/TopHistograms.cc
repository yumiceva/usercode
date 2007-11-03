/**_________________________________________________________________
   class:   TopHistograms.cc
   package: Analyzer/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopHistograms.cc,v 1.10 2007/10/31 19:34:56 yumiceva Exp $

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

	this->DeleteHisto();
	
}

//_______________________________________________________________
void TopHistograms::Init(TString type, TString suffix1, TString suffix2) {

	if (suffix1 != "") suffix1 = "_" + suffix1;
	if (suffix2 != "") suffix1 += "_" + suffix2;

	if ( type == "generator") {
		h2["gentop_rapidities"] = new TH2D("gentop_rapidities","y_{t} vs y_{T}",50,-3,3,50,-3,3);
		h1["gen_toppair_mass"] = new TH1D("gen_toppair_mass","top pair Mass [Gev/c^{2}]",100,100,4500);
		h1["gen_deltaR_light"] = new TH1D("gen_deltaR_light","#Delta R",35,0.,7.);
		h1["gen_deltaR_b"] = new TH1D("gen_deltaR_b","#Delta R",35,0.,7.);
	}
	else if ( type == "jets") {
		h1["jets"+suffix1]     = new TH1D("jets"+suffix1,"Number of jets",11,4,15);
		h1["jet_et"+suffix1]  = new TH1D("jet_et"+suffix1,"Jet E_{T} [GeV/c]",80,0,300);
		h1["jet_eta"+suffix1] = new TH1D("jet_eta"+suffix1,"Jet #eta",50,-3.,3.);
		h1["jet_phi"+suffix1] = new TH1D("jet_phi"+suffix1,"Jet #phi",30,-3.15,3.15);
		h2["jet_res_et"+suffix1]  = new TH2D("jet_res_et"+suffix1,"Jet E_{T} [GeV/c]",80,0,300,60,0,30.);
	}
	else if ( type == "muons") {
		h1["muons"+suffix1]         = new TH1D("muons"+suffix1,"Number of muons",4,1,5);
		h1["muon_normchi2"+suffix1] = new TH1D("muon_normchi2"+suffix1,"#chi^{2}/ndof",40,0,30);
		h1["muon_pt"+suffix1]       = new TH1D("muon_pt"+suffix1,"Muon p_{T} [GeV/c]",80,0.0,200.0);
	}
	else if ( type == "MET") {
		h1["MET"+suffix1] = new TH1D("MET"+suffix1,"MET [GeV]",60,0.0,300.0);
		h1["METcomplex"+suffix1] = new TH1D("METcomplex"+suffix1,"MET [GeV]",80,0.0,300.0);
	}
	else if ( type == "Zprime") {

	}
	else {

		h1["phi_Wb"+suffix1] = new TH1D("phi_Wb"+suffix1,"Phi Difference of W(u+v) and J",50,0.0,3.2);    // phi balance t->W+b
	
		h1["kinfit_chi2"+suffix1] = new TH1D("kinfit_chi2"+suffix1,"#chi^{2} Distribution",100,0.,200.);
		h1["kinfit_probchi2"+suffix1] = new TH1D("kinfit_probchi2"+suffix1,"#chi^{2} Probability",80,0.,1.);
		
		h1["nu_pz"+suffix1] = new TH1D("nu_pz"+suffix1,"Neutrino p_{z} [GeV/c]",50,-500.0,500.0);
	
		h1["WToMuNu"+suffix1] = new TH1D("WToMuNu"+suffix1,"(#mu + #nu) mass [GeV/c^{2}]",80,0.0,300.0);
		h1["tToWlnuj"+suffix1] = new TH1D("tToWlnuj"+suffix1,"(W_{l#nu} + jet) mass [GeV/c^{2}]",50,0.0,500.0);
		h1["tToWlnub"+suffix1] = new TH1D("tToWlnub"+suffix1,"(W_{l#nu} + jet) mass [GeV/c^{2}]",50,0.0,500.0);
		
		h1["WTojj"+suffix1] = new TH1D("WTojj"+suffix1,"(jet+jet) mass [GeV/c^{2}]",80,0.0,300.0);
		h1["tTojjj"+suffix1] = new TH1D("tTojjj"+suffix1,"(three-jet) mass [GeV/c^{2}]",50,0.0,500.0);
		h1["tToWj"+suffix1] = new TH1D("tToWj"+suffix1,"(W_{jj} + jet) mass [GeV/c^{2}]",50,0.0,500.0);
		h1["topPair"+suffix1] = new TH1D("topPair"+suffix1,"top pair Mass [Gev/c^{2}]",100,100,4500);
		h1["topPairRes"+suffix1] = new TH1D("topPairRes"+suffix1,"top pair Mass Resolution",50,-1,1);

		h1["EtpRes"+suffix1] = new TH1D("EtpRes"+suffix1,"|E_{T} - E_{Tfit}|/#sigma_{Efit_{T}}",50,-10,10);
		h1["EtqRes"+suffix1] = new TH1D("EtqRes"+suffix1,"|E_{T} - E_{Tfit}|/#sigma_{Efit_{T}}",50,-10,10);
		h1["EthbRes"+suffix1] = new TH1D("EthbRes"+suffix1,"|E_{T} - E_{Tfit}|/#sigma_{Efit_{T}}",50,-10,10);
		h1["EtlbRes"+suffix1] = new TH1D("EtlbRes"+suffix1,"|E_{T} - E_{Tfit}|/#sigma_{Efit_{T}}",50,-10,10);
		h1["EtlRes"+suffix1] = new TH1D("EtlRes"+suffix1,"|E_{T} - E_{Tfit}|/#sigma_{Efit_{T}}",50,-10,10);
		h1["EtnRes"+suffix1] = new TH1D("EtnRes"+suffix1,"|E_{T} - E_{Tfit}|/#sigma_{Efit_{T}}",50,-10,10);
		h1["EtapRes"+suffix1] = new TH1D("EtapRes"+suffix1,"|#eta - #eta_{Tfit}|/#sigma_{#eta fit}",50,-10,10);
		h1["EtaqRes"+suffix1] = new TH1D("EtaqRes"+suffix1,"|#eta - #eta_{Tfit}|/#sigma_{#eta fit}",50,-10,10);
		h1["EtahbRes"+suffix1] = new TH1D("EtahbRes"+suffix1,"|#eta - #eta_{Tfit}|/#sigma_{#eta fit}",50,-10,10);
		h1["EtalbRes"+suffix1] = new TH1D("EtalbRes"+suffix1,"|#eta - #eta_{Tfit}|/#sigma_{#eta fit}",50,-10,10);
		h1["EtalRes"+suffix1] = new TH1D("EtalRes"+suffix1,"|#eta - #eta_{Tfit}|/#sigma_{#eta fit}",50,-10,10);
		h1["EtanRes"+suffix1] = new TH1D("EtanRes"+suffix1,"|#eta - #eta_{Tfit}|/#sigma_{#eta fit",50,-10,10);
		h1["PhipRes"+suffix1] = new TH1D("PhipRes"+suffix1,"|#phi - #phi_{Tfit}|/#sigma_{#phi fit}",50,-10,10);
		h1["PhiqRes"+suffix1] = new TH1D("PhiqRes"+suffix1,"|#phi - #phi_{Tfit}|/#sigma_{#phi fit}",50,-10,10);
		h1["PhihbRes"+suffix1] = new TH1D("PhihbRes"+suffix1,"|#phi - #phi_{Tfit}|/#sigma_{#phi fit}",50,-10,10);
		h1["PhilbRes"+suffix1] = new TH1D("PhilbRes"+suffix1,"|#phi - #phi_{Tfit}|/#sigma_{#phi fit}",50,-10,10);
		h1["PhilRes"+suffix1] = new TH1D("PhilRes"+suffix1,"|#phi - #phi_{Tfit}|/#sigma_{#phi fit}",50,-10,10);
		h1["PhinRes"+suffix1] = new TH1D("PhinRes"+suffix1,"|#phi - #phi_{Tfit}|/#sigma_{#phi fit}",50,-10,10);
		
		
		h1["E0Res"+suffix1] = new TH1D("E0Res"+suffix1,"|E - E_{fit}|/#sigma_{Efit}",50,-10,10);
		h1["E1Res"+suffix1] = new TH1D("E1Res"+suffix1,"|E - E_{fit}|/#sigma_{Efit}",50,-10,10);
		h1["E2Res"+suffix1] = new TH1D("E2Res"+suffix1,"|E - E_{fit}|/#sigma_{Efit}",50,-10,10);
		h1["E3Res"+suffix1] = new TH1D("E3Res"+suffix1,"|E - E_{fit}|/#sigma_{Efit}",50,-10,10);
		h1["PzNuRes"+suffix1] = new TH1D("PzNuRes"+suffix1,"|Pz - Pz_{fit}|/#sigma_{Pzfit}",50,-10,10);
		
		h1["WTojj_nob"+suffix1] = new TH1D("WTojj_nob"+suffix1,"(jet+jet) mass [GeV/c^{2}]",80,0.0,300.0);
		h1["tTojjb"+suffix1] = new TH1D("tTojjb"+suffix1,"(jet+jet+b-jet) mass [GeV/c^{2}]",50,0.0,500.0);
		h1["tToWb"+suffix1] = new TH1D("tToWb"+suffix1,"(W_{jj} + b-jet) mass [GeV/c^{2}]",50,0.0,500.0);
		h1["Ht"+suffix1] = new TH1D("Ht"+suffix1,"Ht [GeV/c]",100,0.0,1000.0);
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
