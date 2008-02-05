/**_________________________________________________________________
   class:   TQAFHistograms.cc
   package: Analyzer/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TQAFHistograms.cc,v 1.11 2007/11/03 00:39:24 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/TQAFAnalyzer/test/TQAFHistograms.h"
#include "TF1.h"

#include<iostream>

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
TQAFHistograms::TQAFHistograms() {

}

//_______________________________________________________________
TQAFHistograms::~TQAFHistograms() {

	this->DeleteHisto();
	
}

//_______________________________________________________________
void TQAFHistograms::Init(TString type, TString suffix1, TString suffix2) {

	if (suffix1 != "") suffix1 = "_" + suffix1;
	if (suffix2 != "") suffix1 += "_" + suffix2;

	if ( type == "generator") {
		h2["gentop_rapidities"] = new TH2D("gentop_rapidities","y_{t} vs y_{T}",50,-3,3,50,-3,3);
		h1["gen_toppair_mass"] = new TH1D("gen_toppair_mass","top pair Mass [Gev/c^{2}]",100,100,4500);
		h1["gen_deltaR_qb"] = new TH1D("gen_deltaR_qb","#Delta R(q,Had-b)",35,0.,7.);
		h1["gen_deltaR_pb"] = new TH1D("gen_deltaR_pb","#Delta R(p,Had-b)",35,0.,7.);
		h1["gen_deltaR_pq"] = new TH1D("gen_deltaR_pq","#Delta R(p,q)",35,0.,7.);
		h1["gen_deltaR_lb"] = new TH1D("gen_deltaR_lb","#Delta R(#mu,Lep-b)",35,0.,7.);
		h1["gen_deltaR_qLepb"] = new TH1D("gen_deltaR_qLepb","#Delta R(q,Lep-b)",35,0.,7.);
		h1["gen_deltaR_qmu"] = new TH1D("gen_deltaR_qmu","#Delta R(q,#mu)",35,0.,7.);
		h1["gen_nu_pz"] = new TH1D("gen_nu_pz","Neutrino p_{z} [GeV/c]",50,-500.0,500.0);
	}
	else if ( type == "Jets") {
		h1["jets"+suffix1]     = new TH1D("jets"+suffix1,"Number of jets",11,4,15);
		h1["jet0_et"+suffix1]  = new TH1D("jet0_et"+suffix1,"Jet #1 E_{T} [GeV]",100,0,2500);
		h1["jet1_et"+suffix1]  = new TH1D("jet1_et"+suffix1,"Jet #2 E_{T} [GeV]",100,0,2500);
		h1["jet2_et"+suffix1]  = new TH1D("jet2_et"+suffix1,"Jet #3 E_{T} [GeV]",100,0,2500);
		h1["jet3_et"+suffix1]  = new TH1D("jet3_et"+suffix1,"Jet #4 E_{T} [GeV]",100,0,2500);
		h1["jet0_eta"+suffix1]  = new TH1D("jet0_eta"+suffix1,"Jet #1 #eta [GeV]",50,-3.,3.);
		h1["jet_et"+suffix1]  = new TH1D("jet_et"+suffix1,"Jet E_{T} [GeV]",100,0,2000);
		h1["jet_eta"+suffix1] = new TH1D("jet_eta"+suffix1,"Jet #eta",50,-3.,3.);
		h1["jet_phi"+suffix1] = new TH1D("jet_phi"+suffix1,"Jet #phi",30,-3.15,3.15);
		h2["jet_res_et"+suffix1]  = new TH2D("jet_res_et"+suffix1,"Jet E_{T} [GeV]",80,0,300,60,0,30.);
		h1["Mjet_mass"+suffix1] = new TH1D("Mjet_mass"+suffix1, "MJet mass [GeV/c^{2}]",80,0,500);
		h1["Mjet_et"+suffix1] = new TH1D("Mjet_et"+suffix1, "MJet E_{T} [GeV]",100,0,2500);
		h1["Mjet_et_subjets"+suffix1] = new TH1D("Mjet_et_subjets"+suffix1, "Jets in MJet E_{T} [GeV]",100,0,2500);
		h1["Mjet_deltaR_mu"+suffix1] = new TH1D("Mjet_deltaR_mu"+suffix1, "#Delta R(MJet,#mu)",35,0,5);
		h1["Mjet_njets"+suffix1]     = new TH1D("Mjet_njets"+suffix1,"Number of jets in Mjet",10,0,10);
		h1["Mjet_deltaR_b"+suffix1] = new TH1D("Mjet_deltaR_b"+suffix1, "#Delta R(MJet,b)",35,0,5);
		h1["bjet_mass"+suffix1] = new TH1D("bjet_mass"+suffix1, "b (leptonic) Jet mass [GeV/c^{2}]",80,0,500);
		h1["bjet_et"+suffix1] = new TH1D("bjet_et"+suffix1, "b (leptonic) Jet E_{T} [GeV/c]",100,0,2500);
		h1["topPair_mass"+suffix1] = new TH1D("topPair_mass"+suffix1, "top pair mass [GeV/c^{2}]",100,100,4500);
		//h1["topPair_et"+suffix1] = new TH1D("Mjet_et"+suffix1, "MJet E_{T} [GeV/c^{2}]",100,0,2500);
		h1["jet_deltaR_muon"+suffix1] = new TH1D("jet_deltaR_muon"+suffix1, "#Delta R(Jet,#mu)",35,0,5);
	}
	else if ( type == "DisplayJets") {
		//h2["jet0_calotowerI"+suffix1]  = new TH2D("jet0_calotowerI"+suffix1,"Jet #1 CaloTowers [GeV/c]",40,-1.740,1.740,72,0.,3.141);
		h2["jet0_calotowerI"+suffix1]  = new TH2D("jet0_calotowerI"+suffix1,"Jet #1 CaloTowers [GeV/c]",58,-2.523,2.523,144,-3.14,3.141);
		h2["jet1_calotowerI"+suffix1]  = new TH2D("jet1_calotowerI"+suffix1,"Jet #2 CaloTowers [GeV/c]",58,-2.523,2.523,144,-3.14,3.141);
		h2["jet2_calotowerI"+suffix1]  = new TH2D("jet2_calotowerI"+suffix1,"Jet #3 CaloTowers [GeV/c]",58,-2.523,2.523,144,-3.14,3.141);
		h2["jet3_calotowerI"+suffix1]  = new TH2D("jet3_calotowerI"+suffix1,"Jet #4 CaloTowers [GeV/c]",58,-2.523,2.523,144,-3.14,3.141);

		//Double_t caloEta[8] = {1.740,1.830,1.930,2.043,2.172,2.322,2.500,2.650}; //8 bins
		//h2["jet0_calotowerII"+suffix1]  = new TH2D("jet0_calotowerII"+suffix1,"Jet #1 CaloTowers [GeV/c]",7,caloEta,36,0.,3.141);
		//h2["jet1_calotowerII"+suffix1]  = new TH2D("jet1_calotowerII"+suffix1,"Jet #2 CaloTowers [GeV/c]",7,caloEta,36,0.,3.141);
		//h2["jet2_calotowerII"+suffix1]  = new TH2D("jet2_calotowerII"+suffix1,"Jet #3 CaloTowers [GeV/c]",7,caloEta,36,0.,3.141);
		//h2["jet3_calotowerII"+suffix1]  = new TH2D("jet3_calotowerII"+suffix1,"Jet #4 CaloTowers [GeV/c]",7,caloEta,36,0.,3.141);

	}
	else if ( type == "Muons") {
		h1["muons"+suffix1]         = new TH1D("muons"+suffix1,"Number of muons",4,1,5);
		h1["muon_normchi2"+suffix1] = new TH1D("muon_normchi2"+suffix1,"#chi^{2}/ndof",40,0,30);
		h1["muon_pt"+suffix1]       = new TH1D("muon_pt"+suffix1,"Muon p_{T} [GeV/c]",80,0.0,200.0);
		h1["muon_caloIso"+suffix1]       = new TH1D("muon_caloIso"+suffix1,"caloIsolation",80,0.0,300.0);
		h1["muon_trackIso"+suffix1]       = new TH1D("muon_trackIso"+suffix1,"trackIsolation",80,0.0,100.0);
		h1["muon_deltaR_nu"+suffix1]  = new TH1D("muon_deltaR_nu"+suffix1, "#Delta R(#mu,#nu)",35,0,5);
		h1["muon_deltaPhi_nu"+suffix1]  = new TH1D("muon_deltaPhi_nu"+suffix1, "#Delta #phi(#mu,#nu)",35,0,3.142);
		h1["muon_deltaR_b"+suffix1]  = new TH1D("muon_deltaR_b"+suffix1, "#Delta R(#mu,b)",35,0,5);
	}
	else if ( type == "MET") {
		h1["MET"+suffix1] = new TH1D("MET"+suffix1,"MET [GeV]",100,0.0,1500.0);
		h1["MET_deltaR_muon"+suffix1] = new TH1D("MET_deltaR_muon"+suffix1,"#DeltaR(MET,#mu)",35,0.,7.);
		//h1["METcomplex"+suffix1] = new TH1D("METcomplex"+suffix1,"MET [GeV]",80,0.0,300.0);
		h1["nu_pz"+suffix1] = new TH1D("nu_pz"+suffix1,"Neutrino p_{z} [GeV/c]",50,-500.0,500.0);	

	}
	else if ( type == "Mass") {

		h1["WTolnu"+suffix1] = new TH1D("WTolnu"+suffix1,"(#mu + #nu) mass [GeV/c^{2}]",80,0.0,300.0);
		h1["tToWlnuj"+suffix1] = new TH1D("tToWlnuj"+suffix1,"(W_{l#nu} + jet) mass [GeV/c^{2}]",50,0.0,500.0);
		h1["tToWlnub"+suffix1] = new TH1D("tToWlnub"+suffix1,"(W_{l#nu} + jet) mass [GeV/c^{2}]",50,0.0,500.0);
		
		h1["WTojj"+suffix1] = new TH1D("WTojj"+suffix1,"(jet+jet) mass [GeV/c^{2}]",80,0.0,300.0);
		h1["tTojjj"+suffix1] = new TH1D("tTojjj"+suffix1,"(three-jet) mass [GeV/c^{2}]",50,0.0,500.0);
		//h1["tToWj"+suffix1] = new TH1D("tToWj"+suffix1,"(W_{jj} + jet) mass [GeV/c^{2}]",50,0.0,500.0);
		h1["topPair"+suffix1] = new TH1D("topPair"+suffix1,"top pair Mass [Gev/c^{2}]",100,100,4500);
		h1["fittopPair"+suffix1] = new TH1D("fittopPair"+suffix1,"top pair Mass [Gev/c^{2}]",100,100,4500);
		h1["topPairRes"+suffix1] = new TH1D("topPairRes"+suffix1,"top pair Mass Resolution",50,-1,1);
		h1["fittopPairRes"+suffix1] = new TH1D("fittopPairRes"+suffix1,"top pair Mass Resolution",50,-1,1);
		//h1["kinfit_chi2"+suffix1] = new TH1D("kinfit_chi2"+suffix1,"#chi^{2} Distribution",100,0.,200.);
		h1["kinfit_probchi2"+suffix1] = new TH1D("kinfit_probchi2"+suffix1,"#chi^{2} Probability",80,0.,1.);

		
		h1["EtpPull"+suffix1] = new TH1D("EtpPull"+suffix1,"(E^{fit}_{T} - E_{T})/#sigma_{E_{T}}",50,-10,10);
		h1["EtqPull"+suffix1] = new TH1D("EtqPull"+suffix1,"(E^{fit}_{T} - E_{T})/#sigma_{E_{T}}",50,-10,10);
		h1["EthbPull"+suffix1] = new TH1D("EthbPull"+suffix1,"(E^{fit}_{T} - E_{T})/#sigma_{E_{T}}",50,-10,10);
		h1["EtlbPull"+suffix1] = new TH1D("EtlbPull"+suffix1,"(E^{fit}_{T} - E_{T})/#sigma_{E_{T}}",50,-10,10);
		h1["EtlPull"+suffix1] = new TH1D("EtlPull"+suffix1,"(E^{fit}_{T} - E_{T})/#sigma_{E_{T}}",50,-10,10);
		h1["fitEtpPull"+suffix1] = new TH1D("fitEtpPull"+suffix1,"|E^{fit}_{T} - E^{gen}_{T}|/#sigma_{E_{T}}",50,-10,10);
		h1["fitEtqPull"+suffix1] = new TH1D("fitEtqPull"+suffix1,"|E^{fit}_{T} - E^{gen}_{T}|/#sigma_{E_{T}}",50,-10,10);
		h1["fitEthbPull"+suffix1] = new TH1D("fitEthbPull"+suffix1,"|E^{fit}_{T} - E^{gen}_{T}|/#sigma_{E_{T}}",50,-10,10);
		h1["fitEtlbPull"+suffix1] = new TH1D("fitEtlbPull"+suffix1,"|E^{fit}_{T} - E^{gen}_{T}|/#sigma_{E_{T}}",50,-10,10);
		h1["fitEtlPull"+suffix1] = new TH1D("fitEtlPull"+suffix1,"|E^{fit}_{T} - E^{gen}_{T}|/#sigma_{E_{T}}",50,-10,10);
		
	}
	else {

		h1["phi_Wb"+suffix1] = new TH1D("phi_Wb"+suffix1,"Phi Difference of W(u+v) and J",50,0.0,3.2);    // phi balance t->W+b
		
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
void TQAFHistograms::Fill1d(TString name, Double_t x) {

	h1[name]->Fill(x);
}

//_______________________________________________________________
void TQAFHistograms::Fill2d(TString name, Double_t x, Double_t y, Double_t weight) {

	h2[name]->Fill(x,y,weight);
	
}

//_______________________________________________________________
void TQAFHistograms::Fit(TString name, Double_t mean) {

	Int_t npar = 7;
	Double_t params[7] = {700,mean,50.,350,6,0.5};
	TF1 *simple = new TF1("simple",Total,0,10,npar);
	simple->SetParameters(params);

	h1[name]->Fit("simple");
	
}


//_______________________________________________________________
void TQAFHistograms::Print(TString extension, TString tag) {

	if ( tag != "" ) tag = "_"+tag;
                
	for(std::map<TString,TCanvas*>::const_iterator icv=cv_map.begin(); icv!=cv_map.end(); ++icv){

		TString tmpname = icv->first;
		TCanvas *acv = icv->second;
		acv->Print(TString(tmpname+tag+"."+extension));
	}               

}
//_______________________________________________________________
void TQAFHistograms::Save() {
	
	for(std::map<TString,TH1* >::const_iterator ih=h1.begin(); ih!=h1.end(); ++ih){
		//std::cout << "get histo: " << std::endl;
		TH1D *htemp = (TH1D*) ih->second;
		//std::cout << htemp->Print() << std::endl;
		if (htemp->GetEntries() > 0 ) htemp->Write();
	}
	for(std::map<TString,TH2* >::const_iterator ih=h2.begin(); ih!=h2.end(); ++ih){
		//std::cout << "get histo: " << std::endl;
		TH2D *htemp = (TH2D*) ih->second;
		//std::cout << htemp->Print() << std::endl;
		if (htemp->GetEntries() > 0 ) htemp->Write();
	}	
}

//_______________________________________________________________
void TQAFHistograms::SaveToFile(TString filename) {

	foutfile = new TFile(filename,"RECREATE");
	for(std::map<TString,TH1* >::const_iterator ih=h1.begin(); ih!=h1.end(); ++ih){
		//std::cout << "get histo: " << std::endl;
		TH1D *htemp = (TH1D*) ih->second;
		//std::cout << htemp->Print() << std::endl;
		htemp->Write();
	}
	for(std::map<TString,TH2* >::const_iterator ih=h2.begin(); ih!=h2.end(); ++ih){
		//std::cout << "get histo: " << std::endl;
		TH2D *htemp = (TH2D*) ih->second;
		//std::cout << htemp->Print() << std::endl;
		htemp->Write();
	}
                
	foutfile->Write();
	foutfile->Close();
	
}
