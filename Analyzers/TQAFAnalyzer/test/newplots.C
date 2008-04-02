#include "TFile.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TGaxis.h"

#include <map>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

void plots(bool print=false) {

        gROOT->SetStyle("CMS");

		gROOT->ForceStyle();
		
		TFile *Zp1 = TFile::Open("RESULTS_L2/Zp1Tev_outputLayer2_fromLayer1_condor_all.root");
		TFile *Zp2 = TFile::Open("RESULTS_L2/Zp2Tev_outputLayer2_fromLayer1_condor_all.root");
		TFile *Zp3 = TFile::Open("RESULTS_L2/Zp3Tev_outputLayer2_fromLayer1_condor_all.root");
		TFile *Zp4 = TFile::Open("RESULTS_L2/Zp4Tev_outputLayer2_fromLayer1_condor_all.root");
		

		map<TString, TCanvas*> cv_map;
		map<TString, TH1* > h1z1,h1z2,h1z3,h1z4;
		map<TString, TH2* > h2z1,h2z2,h2z3,h2z4;

		map<TString, int> color;
		color["black"] = 1;
		color["zp1"] = 2;
		color["zp2"] = 3;
		color["zp3"] = 4;
		color["zp4"] = 6;


		// generator
		Zp1->cd();
		h2z1["gen_toprapidity_vs_deltaR_pq"] = (TH2D*) gDirectory->Get("Generator/gen_toprapidity_vs_deltaR_pq");
		h2z1["gen_toprapidity_vs_psi_pq"] = (TH2D*) gDirectory->Get("Generator/gen_toprapidity_vs_psi_pq");
		Zp2->cd();
		h2z2["gen_toprapidity_vs_deltaR_pq"] = (TH2D*) gDirectory->Get("Generator/gen_toprapidity_vs_deltaR_pq");
		h2z2["gen_toprapidity_vs_psi_pq"] = (TH2D*) gDirectory->Get("Generator/gen_toprapidity_vs_psi_pq");
		Zp3->cd();
		h2z3["gen_toprapidity_vs_deltaR_pq"] = (TH2D*) gDirectory->Get("Generator/gen_toprapidity_vs_deltaR_pq");
		h2z3["gen_toprapidity_vs_psi_pq"] = (TH2D*) gDirectory->Get("Generator/gen_toprapidity_vs_psi_pq");
		Zp4->cd();
		h2z4["gen_toprapidity_vs_deltaR_pq"] = (TH2D*) gDirectory->Get("Generator/gen_toprapidity_vs_deltaR_pq");
		h2z4["gen_toprapidity_vs_psi_pq"] = (TH2D*) gDirectory->Get("Generator/gen_toprapidity_vs_psi_pq");


		TProfile *pdeltar_zp1 = h2z1["gen_toprapidity_vs_deltaR_pq"]->ProfileX("pdeltar_zp1");
		TProfile *ppsi_zp1 = h2z1["gen_toprapidity_vs_psi_pq"]->ProfileX("ppsi_zp1");

		TProfile *pdeltar_zp2 = h2z2["gen_toprapidity_vs_deltaR_pq"]->ProfileX("pdeltar_zp2");
		TProfile *ppsi_zp2 = h2z2["gen_toprapidity_vs_psi_pq"]->ProfileX("ppsi_zp2");

		TProfile *pdeltar_zp3 = h2z3["gen_toprapidity_vs_deltaR_pq"]->ProfileX("pdeltar_zp3");
		TProfile *ppsi_zp3 = h2z3["gen_toprapidity_vs_psi_pq"]->ProfileX("ppsi_zp3");

		TProfile *pdeltar_zp4 = h2z4["gen_toprapidity_vs_deltaR_pq"]->ProfileX("pdeltar_zp4");
		TProfile *ppsi_zp4 = h2z4["gen_toprapidity_vs_psi_pq"]->ProfileX("ppsi_zp4");

		pdeltar_zp1->SetLineColor(color["zp1"]);
		ppsi_zp1->SetLineColor(color["zp1"]);
		pdeltar_zp2->SetLineColor(color["zp2"]);
		ppsi_zp2->SetLineColor(color["zp2"]);
		pdeltar_zp3->SetLineColor(color["zp3"]);
		ppsi_zp3->SetLineColor(color["zp3"]);
		pdeltar_zp4->SetLineColor(color["zp4"]);
		ppsi_zp4->SetLineColor(color["zp4"]);

		pdeltar_zp1->SetMarkerColor(color["zp1"]);
		ppsi_zp1->SetMarkerColor(color["zp1"]);
		pdeltar_zp2->SetMarkerColor(color["zp2"]);
		ppsi_zp2->SetMarkerColor(color["zp2"]);
		pdeltar_zp3->SetMarkerColor(color["zp3"]);
		ppsi_zp3->SetMarkerColor(color["zp3"]);
		pdeltar_zp4->SetMarkerColor(color["zp4"]);
		ppsi_zp4->SetMarkerColor(color["zp4"]);

		cv_map["gen_deltar"] = new TCanvas("gen_deltar","gen_deltar",700,700);

		pdeltar_zp1->SetMinimum(0.1);
		pdeltar_zp1->SetMaximum(1.8);
		pdeltar_zp1->SetXTitle("Top rapidity");
		pdeltar_zp1->SetYTitle("#DeltaR(p,q)");
		
		pdeltar_zp1->Draw();
		pdeltar_zp2->Draw("same");
		pdeltar_zp3->Draw("same");
		pdeltar_zp4->Draw("same");

		cv_map["gen_psi"] = new TCanvas("gen_psi","gen_psi",700,700);

		//ppsi_zp1->SetMinimum(0.1);
		//ppsi_zp1->SetMaximum(1.8);
		ppsi_zp1->SetXTitle("Top rapidity");
		ppsi_zp1->SetYTitle("#psi(W(p,q))");
		
		ppsi_zp1->Draw();
		ppsi_zp2->Draw("same");
		ppsi_zp3->Draw("same");
		ppsi_zp4->Draw("same");

		// muons
		Zp1->cd();
		h2z1["muons_vsJets_cut0"]  = (TH2D*) gDirectory->Get("Muons/muons_vsJets_cut0");
		h1z1["muon_pt_cut0"]  = (TH1D*) gDirectory->Get("Muons/muon_pt_cut0");
		Zp2->cd();
		h2z2["muons_vsJets_cut0"]  = (TH2D*) gDirectory->Get("Muons/muons_vsJets_cut0");
		h1z2["muon_pt_cut0"]  = (TH1D*) gDirectory->Get("Muons/muon_pt_cut0");
		Zp3->cd();
		h2z3["muons_vsJets_cut0"]  = (TH2D*) gDirectory->Get("Muons/muons_vsJets_cut0");
		h1z3["muon_pt_cut0"]  = (TH1D*) gDirectory->Get("Muons/muon_pt_cut0");
		Zp4->cd();
		h2z4["muons_vsJets_cut0"]  = (TH2D*) gDirectory->Get("Muons/muons_vsJets_cut0");
		h1z4["muon_pt_cut0"]  = (TH1D*) gDirectory->Get("Muons/muon_pt_cut0");	

		cv_map["muon_pt"] = new TCanvas("muon_pt","muon_pt",700,700);

		h1z1["muon_pt_cut0"]->SetLineColor(color["zp1"]);
		h1z2["muon_pt_cut0"]->SetLineColor(color["zp2"]);
		h1z3["muon_pt_cut0"]->SetLineColor(color["zp3"]);
		h1z4["muon_pt_cut0"]->SetLineColor(color["zp4"]);
		
		h1z1["muon_pt_cut0"]->Draw();
		h1z2["muon_pt_cut0"]->Draw("same");
		h1z3["muon_pt_cut0"]->Draw("same");
		h1z4["muon_pt_cut0"]->Draw("same");
		
		cv_map["muons"] = new TCanvas("muons","muons",700,700);

		TH1D *muonsz1 = h2z1["muons_vsJets_cut0"]->ProjectionY("muonsz1");
		TH1D *muonsz2 = h2z2["muons_vsJets_cut0"]->ProjectionY("muonsz2");
		TH1D *muonsz3 = h2z3["muons_vsJets_cut0"]->ProjectionY("muonsz3");
		TH1D *muonsz4 = h2z4["muons_vsJets_cut0"]->ProjectionY("muonsz4");

		muonsz1->SetLineColor(color["zp1"]);
		muonsz2->SetLineColor(color["zp2"]);
		muonsz3->SetLineColor(color["zp3"]);
		muonsz4->SetLineColor(color["zp4"]);

		muonsz1->GetXaxis()->SetBinLabel(1,"1 Jet");
		muonsz1->GetXaxis()->SetBinLabel(2,"2 Jet");
		muonsz1->GetXaxis()->SetBinLabel(3,"3 Jet");
		muonsz1->GetXaxis()->SetBinLabel(4,"#geq4 Jet");

		muonsz1->Draw();
		muonsz2->Draw("same");
		muonsz3->Draw("same");
		muonsz4->Draw("same");
		
		
		// jet et, eta, n
		Zp1->cd();
		h1z1["jet_et_cut0"]  = (TH1D*) gDirectory->Get("Jets/jet_et_cut0");
		h1z1["jet_eta_cut0"] = (TH1D*) gDirectory->Get("Jets/jet_eta_cut0");
		h1z1["jets_cut0"] = (TH1D*) gDirectory->Get("Jets/jets_cut0");
		h1z1["LeptonicTop_psi_cut0"] = (TH1D*) gDirectory->Get("Jets/LeptonicTop_psi_cut0");
		h1z1["HadronicTop_psi_cut0"] = (TH1D*) gDirectory->Get("Jets/HadronicTop_psi_cut0");
		h1z1["LeptonicTop_pt_cut0"] = (TH1D*) gDirectory->Get("Jets/LeptonicTop_pt_cut0");
		h1z1["HadronicTop_pt_cut0"] = (TH1D*) gDirectory->Get("Jets/HadronicTop_pt_cut0");
		h1z1["jet_deltaR_LeptonicW_cut0"] = (TH1D*) gDirectory->Get("Jets/jet_deltaR_LeptonicW_cut0");
		Zp2->cd();
		h1z2["jet_et_cut0"]  = (TH1D*) gDirectory->Get("Jets/jet_et_cut0");
		h1z2["jet_eta_cut0"] = (TH1D*) gDirectory->Get("Jets/jet_eta_cut0");
		h1z2["jets_cut0"] = (TH1D*) gDirectory->Get("Jets/jets_cut0");
		h1z2["LeptonicTop_psi_cut0"] = (TH1D*) gDirectory->Get("Jets/LeptonicTop_psi_cut0");
		h1z2["HadronicTop_psi_cut0"] = (TH1D*) gDirectory->Get("Jets/HadronicTop_psi_cut0");
		h1z2["LeptonicTop_pt_cut0"] = (TH1D*) gDirectory->Get("Jets/LeptonicTop_pt_cut0");
		h1z2["HadronicTop_pt_cut0"] = (TH1D*) gDirectory->Get("Jets/HadronicTop_pt_cut0");
		h1z2["jet_deltaR_LeptonicW_cut0"] = (TH1D*) gDirectory->Get("Jets/jet_deltaR_LeptonicW_cut0");
		Zp3->cd();
		h1z3["jet_et_cut0"]  = (TH1D*) gDirectory->Get("Jets/jet_et_cut0");
		h1z3["jet_eta_cut0"] = (TH1D*) gDirectory->Get("Jets/jet_eta_cut0");
		h1z3["jets_cut0"] = (TH1D*) gDirectory->Get("Jets/jets_cut0");
		h1z3["LeptonicTop_psi_cut0"] = (TH1D*) gDirectory->Get("Jets/LeptonicTop_psi_cut0");
		h1z3["HadronicTop_psi_cut0"] = (TH1D*) gDirectory->Get("Jets/HadronicTop_psi_cut0");
		h1z3["LeptonicTop_pt_cut0"] = (TH1D*) gDirectory->Get("Jets/LeptonicTop_pt_cut0");
		h1z3["HadronicTop_pt_cut0"] = (TH1D*) gDirectory->Get("Jets/HadronicTop_pt_cut0");
		h1z3["jet_deltaR_LeptonicW_cut0"] = (TH1D*) gDirectory->Get("Jets/jet_deltaR_LeptonicW_cut0");
		Zp4->cd();
		h1z4["jet_et_cut0"]  = (TH1D*) gDirectory->Get("Jets/jet_et_cut0");
		h1z4["jet_eta_cut0"] = (TH1D*) gDirectory->Get("Jets/jet_eta_cut0");
		h1z4["jets_cut0"] = (TH1D*) gDirectory->Get("Jets/jets_cut0");
		h1z4["LeptonicTop_psi_cut0"] = (TH1D*) gDirectory->Get("Jets/LeptonicTop_psi_cut0");
		h1z4["HadronicTop_psi_cut0"] = (TH1D*) gDirectory->Get("Jets/HadronicTop_psi_cut0");
		h1z4["LeptonicTop_pt_cut0"] = (TH1D*) gDirectory->Get("Jets/LeptonicTop_pt_cut0");
		h1z4["HadronicTop_pt_cut0"] = (TH1D*) gDirectory->Get("Jets/HadronicTop_pt_cut0");
		h1z4["jet_deltaR_LeptonicW_cut0"] = (TH1D*) gDirectory->Get("Jets/jet_deltaR_LeptonicW_cut0");

		cv_map["jet_et_cut0"] = new TCanvas("jet_et_cut0","jet_et_cut0",700,700);

		h1z1["jet_et_cut0"]->SetLineColor(color["zp1"]);
		h1z2["jet_et_cut0"]->SetLineColor(color["zp2"]);
		h1z3["jet_et_cut0"]->SetLineColor(color["zp3"]);
		h1z4["jet_et_cut0"]->SetLineColor(color["zp4"]);
		
		h1z1["jet_et_cut0"]->Draw();
		h1z2["jet_et_cut0"]->Draw("same");
		h1z3["jet_et_cut0"]->Draw("same");
		h1z4["jet_et_cut0"]->Draw("same");

		cv_map["jet_eta_cut0"] = new TCanvas("jet_eta_cut0","jet_eta_cut0",700,700);

		h1z1["jet_eta_cut0"]->SetLineColor(color["zp1"]);
		h1z2["jet_eta_cut0"]->SetLineColor(color["zp2"]);
		h1z3["jet_eta_cut0"]->SetLineColor(color["zp3"]);
		h1z4["jet_eta_cut0"]->SetLineColor(color["zp4"]);
		
		h1z1["jet_eta_cut0"]->Draw();
		h1z2["jet_eta_cut0"]->Draw("same");
		h1z3["jet_eta_cut0"]->Draw("same");
		h1z4["jet_eta_cut0"]->Draw("same");

		cv_map["jets_cut0"] = new TCanvas("jets_cut0","jets_cut0",700,700);

		h1z1["jets_cut0"]->SetLineColor(color["zp1"]);
		h1z2["jets_cut0"]->SetLineColor(color["zp2"]);
		h1z3["jets_cut0"]->SetLineColor(color["zp3"]);
		h1z4["jets_cut0"]->SetLineColor(color["zp4"]);
		
		h1z1["jets_cut0"]->Draw();
		h1z2["jets_cut0"]->Draw("same");
		h1z3["jets_cut0"]->Draw("same");
		h1z4["jets_cut0"]->Draw("same");

		cv_map["LeptonicTop_psi_cut0"] = new TCanvas("LeptonicTop_psi_cut0","LeptonicTop_psi_cut0",700,700);

		h1z1["LeptonicTop_psi_cut0"]->SetLineColor(color["zp1"]);
		h1z2["LeptonicTop_psi_cut0"]->SetLineColor(color["zp2"]);
		h1z3["LeptonicTop_psi_cut0"]->SetLineColor(color["zp3"]);
		h1z4["LeptonicTop_psi_cut0"]->SetLineColor(color["zp4"]);
		
		h1z1["LeptonicTop_psi_cut0"]->Draw();
		h1z2["LeptonicTop_psi_cut0"]->Draw("same");
		h1z3["LeptonicTop_psi_cut0"]->Draw("same");
		h1z4["LeptonicTop_psi_cut0"]->Draw("same");

		cv_map["LeptonicTop_pt_cut0"] = new TCanvas("LeptonicTop_pt_cut0","LeptonicTop_pt_cut0",700,700);

		h1z1["LeptonicTop_pt_cut0"]->SetLineColor(color["zp1"]);
		h1z2["LeptonicTop_pt_cut0"]->SetLineColor(color["zp2"]);
		h1z3["LeptonicTop_pt_cut0"]->SetLineColor(color["zp3"]);
		h1z4["LeptonicTop_pt_cut0"]->SetLineColor(color["zp4"]);
		
		h1z1["LeptonicTop_pt_cut0"]->Draw();
		h1z2["LeptonicTop_pt_cut0"]->Draw("same");
		h1z3["LeptonicTop_pt_cut0"]->Draw("same");
		h1z4["LeptonicTop_pt_cut0"]->Draw("same");

		cv_map["HadronicTop_psi_cut0"] = new TCanvas("HadronicTop_psi_cut0","HadronicTop_psi_cut0",700,700);

		h1z1["HadronicTop_psi_cut0"]->SetLineColor(color["zp1"]);
		h1z2["HadronicTop_psi_cut0"]->SetLineColor(color["zp2"]);
		h1z3["HadronicTop_psi_cut0"]->SetLineColor(color["zp3"]);
		h1z4["HadronicTop_psi_cut0"]->SetLineColor(color["zp4"]);
		
		h1z1["HadronicTop_psi_cut0"]->Draw();
		h1z2["HadronicTop_psi_cut0"]->Draw("same");
		h1z3["HadronicTop_psi_cut0"]->Draw("same");
		h1z4["HadronicTop_psi_cut0"]->Draw("same");

		cv_map["HadronicTop_pt_cut0"] = new TCanvas("HadronicTop_pt_cut0","HadronicTop_pt_cut0",700,700);

		h1z1["HadronicTop_pt_cut0"]->SetLineColor(color["zp1"]);
		h1z2["HadronicTop_pt_cut0"]->SetLineColor(color["zp2"]);
		h1z3["HadronicTop_pt_cut0"]->SetLineColor(color["zp3"]);
		h1z4["HadronicTop_pt_cut0"]->SetLineColor(color["zp4"]);
		
		h1z1["HadronicTop_pt_cut0"]->Draw();
		h1z2["HadronicTop_pt_cut0"]->Draw("same");
		h1z3["HadronicTop_pt_cut0"]->Draw("same");
		h1z4["HadronicTop_pt_cut0"]->Draw("same");

		cv_map["jet_deltaR_LeptonicW_cut0"] = new TCanvas("jet_deltaR_LeptonicW_cut0","jet_deltaR_LeptonicW_cut0",700,700);

		h1z1["jet_deltaR_LeptonicW_cut0"]->SetLineColor(color["zp1"]);
		h1z2["jet_deltaR_LeptonicW_cut0"]->SetLineColor(color["zp2"]);
		h1z3["jet_deltaR_LeptonicW_cut0"]->SetLineColor(color["zp3"]);
		h1z4["jet_deltaR_LeptonicW_cut0"]->SetLineColor(color["zp4"]);
		
		h1z1["jet_deltaR_LeptonicW_cut0"]->Draw();
		h1z2["jet_deltaR_LeptonicW_cut0"]->Draw("same");
		h1z3["jet_deltaR_LeptonicW_cut0"]->Draw("same");
		h1z4["jet_deltaR_LeptonicW_cut0"]->Draw("same");
		
		// MET
		Zp1->cd();
		h1z1["MET_cut0"]  = (TH1D*) gDirectory->Get("MET/MET_cut0");
		h1z1["delta_nu_pz_cut0"]  = (TH1D*) gDirectory->Get("MET/delta_nu_pz_cut0");
		h1z1["LeptonicW_psi_cut0"]  = (TH1D*) gDirectory->Get("MET/LeptonicW_psi_cut0");
		h2z1["MET_vsJets_cut0"]  = (TH2D*) gDirectory->Get("MET/MET_vsJets_cut0");
		Zp2->cd();
		h1z2["MET_cut0"]  = (TH1D*) gDirectory->Get("MET/MET_cut0");
		h1z2["delta_nu_pz_cut0"]  = (TH1D*) gDirectory->Get("MET/delta_nu_pz_cut0");
		h1z2["LeptonicW_psi_cut0"]  = (TH1D*) gDirectory->Get("MET/LeptonicW_psi_cut0");
		h2z2["MET_vsJets_cut0"]  = (TH2D*) gDirectory->Get("MET/MET_vsJets_cut0");
		Zp3->cd();
		h1z3["MET_cut0"]  = (TH1D*) gDirectory->Get("MET/MET_cut0");
		h1z3["delta_nu_pz_cut0"]  = (TH1D*) gDirectory->Get("MET/delta_nu_pz_cut0");
		h1z3["LeptonicW_psi_cut0"]  = (TH1D*) gDirectory->Get("MET/LeptonicW_psi_cut0");
		h2z3["MET_vsJets_cut0"]  = (TH2D*) gDirectory->Get("MET/MET_vsJets_cut0");
		Zp4->cd();
		h1z4["MET_cut0"]  = (TH1D*) gDirectory->Get("MET/MET_cut0");
		h1z4["delta_nu_pz_cut0"]  = (TH1D*) gDirectory->Get("MET/delta_nu_pz_cut0");
		h1z4["LeptonicW_psi_cut0"]  = (TH1D*) gDirectory->Get("MET/LeptonicW_psi_cut0");
		h2z4["MET_vsJets_cut0"]  = (TH2D*) gDirectory->Get("MET/MET_vsJets_cut0");
												  
		
		h1z1["MET_cut0"]->SetLineColor(color["zp1"]);
		h1z2["MET_cut0"]->SetLineColor(color["zp2"]);
		h1z3["MET_cut0"]->SetLineColor(color["zp3"]);
		h1z4["MET_cut0"]->SetLineColor(color["zp4"]);

		h1z1["delta_nu_pz_cut0"]->SetLineColor(color["zp1"]);
		h1z2["delta_nu_pz_cut0"]->SetLineColor(color["zp2"]);
		h1z3["delta_nu_pz_cut0"]->SetLineColor(color["zp3"]);
		h1z4["delta_nu_pz_cut0"]->SetLineColor(color["zp4"]);

		h1z1["LeptonicW_psi_cut0"]->SetLineColor(color["zp1"]);
		h1z2["LeptonicW_psi_cut0"]->SetLineColor(color["zp2"]);
		h1z3["LeptonicW_psi_cut0"]->SetLineColor(color["zp3"]);
		h1z4["LeptonicW_psi_cut0"]->SetLineColor(color["zp4"]);

										  
		cv_map["MET"] = new TCanvas("MET","MET",700,700);

		h1z1["MET_cut0"]->Draw();
		h1z2["MET_cut0"]->Draw("same");
		h1z3["MET_cut0"]->Draw("same");
		h1z4["MET_cut0"]->Draw("same");

		cv_map["delta_nu_pz"] = new TCanvas("delta_nu_pz","delta_nu_pz",700,700);

		h1z1["delta_nu_pz_cut0"]->Draw();
		h1z2["delta_nu_pz_cut0"]->Draw("same");
		h1z3["delta_nu_pz_cut0"]->Draw("same");
		h1z4["delta_nu_pz_cut0"]->Draw("same");

		cv_map["LeptonicW_psi"] = new TCanvas("LeptonicW_psi","LeptonicW_psi",700,700);

		h1z1["LeptonicW_psi_cut0"]->Draw();
		h1z2["LeptonicW_psi_cut0"]->Draw("same");
		h1z3["LeptonicW_psi_cut0"]->Draw("same");
		h1z4["LeptonicW_psi_cut0"]->Draw("same");


		cv_map["METvsJets"] = new TCanvas("METvsJets","METvsJets",700,700);

		TH1D *METvsJetsz1 = h2z1["MET_vsJets_cut0"]->ProjectionY("METvsJetsz1");
		TH1D *METvsJetsz2 = h2z2["MET_vsJets_cut0"]->ProjectionY("METvsJetsz2");
		TH1D *METvsJetsz3 = h2z3["MET_vsJets_cut0"]->ProjectionY("METvsJetsz3");
		TH1D *METvsJetsz4 = h2z4["MET_vsJets_cut0"]->ProjectionY("METvsJetsz4");

		METvsJetsz1->SetLineColor(color["zp1"]);
		METvsJetsz2->SetLineColor(color["zp2"]);
		METvsJetsz3->SetLineColor(color["zp3"]);
		METvsJetsz4->SetLineColor(color["zp4"]);

		METvsJetsz1->GetXaxis()->SetBinLabel(1,"1 Jet");
		METvsJetsz1->GetXaxis()->SetBinLabel(2,"2 Jet");
		METvsJetsz1->GetXaxis()->SetBinLabel(3,"3 Jet");
		METvsJetsz1->GetXaxis()->SetBinLabel(4,"#geq4 Jet");

		METvsJetsz1->Draw();
		METvsJetsz2->Draw("same");
		METvsJetsz3->Draw("same");
		METvsJetsz4->Draw("same");
		

		// MASS
		Zp1->cd();
		h1z1["HadronicTop_mass_cut1"]  = (TH1D*) gDirectory->Get("Mass/HadronicTop_mass_cut1");
		h1z1["LeptonicTop_mass_cut1"]  = (TH1D*) gDirectory->Get("Mass/LeptonicTop_mass_cut1");
		h1z1["LeptonicW_mass_cut1"]  = (TH1D*) gDirectory->Get("Mass/LeptonicW_mass_cut1");
		h1z1["topPair_cut1"]  = (TH1D*) gDirectory->Get("Mass/topPair_cut1");
		Zp2->cd();
		h1z2["HadronicTop_mass_cut1"]  = (TH1D*) gDirectory->Get("Mass/HadronicTop_mass_cut1");
		h1z2["LeptonicTop_mass_cut1"]  = (TH1D*) gDirectory->Get("Mass/LeptonicTop_mass_cut1");
		h1z2["LeptonicW_mass_cut1"]  = (TH1D*) gDirectory->Get("Mass/LeptonicW_mass_cut1");
		h1z2["topPair_cut1"]  = (TH1D*) gDirectory->Get("Mass/topPair_cut1");
		Zp3->cd();
		h1z3["HadronicTop_mass_cut1"]  = (TH1D*) gDirectory->Get("Mass/HadronicTop_mass_cut1");
		h1z3["LeptonicTop_mass_cut1"]  = (TH1D*) gDirectory->Get("Mass/LeptonicTop_mass_cut1");
		h1z3["LeptonicW_mass_cut1"]  = (TH1D*) gDirectory->Get("Mass/LeptonicW_mass_cut1");
		h1z3["topPair_cut1"]  = (TH1D*) gDirectory->Get("Mass/topPair_cut1");
		Zp4->cd();
		h1z4["HadronicTop_mass_cut1"]  = (TH1D*) gDirectory->Get("Mass/HadronicTop_mass_cut1");
		h1z4["LeptonicTop_mass_cut1"]  = (TH1D*) gDirectory->Get("Mass/LeptonicTop_mass_cut1");
		h1z4["LeptonicW_mass_cut1"]  = (TH1D*) gDirectory->Get("Mass/LeptonicW_mass_cut1");
		h1z4["topPair_cut1"]  = (TH1D*) gDirectory->Get("Mass/topPair_cut1");

		
		cv_map["HadronicTop_mass_cut1"] = new TCanvas("HadronicTop_mass_cut1","HadronicTop_mass_cut1",700,700);

		h1z1["HadronicTop_mass_cut1"]->SetLineColor(color["zp1"]);
		h1z2["HadronicTop_mass_cut1"]->SetLineColor(color["zp2"]);
		h1z3["HadronicTop_mass_cut1"]->SetLineColor(color["zp3"]);
		h1z4["HadronicTop_mass_cut1"]->SetLineColor(color["zp4"]);
		
		h1z1["HadronicTop_mass_cut1"]->Draw();
		h1z2["HadronicTop_mass_cut1"]->Draw("same");
		h1z3["HadronicTop_mass_cut1"]->Draw("same");
		h1z4["HadronicTop_mass_cut1"]->Draw("same");

		cv_map["LeptonicTop_mass_cut1"] = new TCanvas("LeptonicTop_mass_cut1","LeptonicTop_mass_cut1",700,700);

		h1z1["LeptonicTop_mass_cut1"]->SetLineColor(color["zp1"]);
		h1z2["LeptonicTop_mass_cut1"]->SetLineColor(color["zp2"]);
		h1z3["LeptonicTop_mass_cut1"]->SetLineColor(color["zp3"]);
		h1z4["LeptonicTop_mass_cut1"]->SetLineColor(color["zp4"]);
		
		h1z1["LeptonicTop_mass_cut1"]->Draw();
		h1z2["LeptonicTop_mass_cut1"]->Draw("same");
		h1z3["LeptonicTop_mass_cut1"]->Draw("same");
		h1z4["LeptonicTop_mass_cut1"]->Draw("same");

		cv_map["LeptonicW_mass_cut1"] = new TCanvas("LeptonicW_mass_cut1","LeptonicW_mass_cut1",700,700);

		h1z1["LeptonicW_mass_cut1"]->SetLineColor(color["zp1"]);
		h1z2["LeptonicW_mass_cut1"]->SetLineColor(color["zp2"]);
		h1z3["LeptonicW_mass_cut1"]->SetLineColor(color["zp3"]);
		h1z4["LeptonicW_mass_cut1"]->SetLineColor(color["zp4"]);
		
		h1z1["LeptonicW_mass_cut1"]->Draw();
		h1z2["LeptonicW_mass_cut1"]->Draw("same");
		h1z3["LeptonicW_mass_cut1"]->Draw("same");
		h1z4["LeptonicW_mass_cut1"]->Draw("same");

		

		h1z1["topPair_cut1"]->SetLineColor(color["zp1"]);
		h1z2["topPair_cut1"]->SetLineColor(color["zp2"]);
		h1z3["topPair_cut1"]->SetLineColor(color["zp3"]);
		h1z4["topPair_cut1"]->SetLineColor(color["zp4"]);

		//TGaxis::SetMaxDigits(1);
		
		cv_map["topPair_zp1"] = new TCanvas("topPair_zp1","topPair_zp1",700,700);
		h1z1["topPair_cut1"]->GetXaxis()->SetNdivisions(505);
		h1z1["topPair_cut1"]->Draw();
		cv_map["topPair_zp2"] = new TCanvas("topPair_zp2","topPair_zp2",700,700);
		h1z2["topPair_cut1"]->GetXaxis()->SetNdivisions(505);
		h1z2["topPair_cut1"]->Draw();
		cv_map["topPair_zp3"] = new TCanvas("topPair_zp3","topPair_zp3",700,700);
		h1z3["topPair_cut1"]->GetXaxis()->SetNdivisions(505);
		h1z3["topPair_cut1"]->Draw();
		cv_map["topPair_zp4"] = new TCanvas("topPair_zp4","topPair_zp4",700,700);
		h1z4["topPair_cut1"]->GetXaxis()->SetNdivisions(505);
		h1z4["topPair_cut1"]->Draw();


		if (print) {

			for(std::map<TString,TCanvas*>::const_iterator icv=cv_map.begin(); icv!=cv_map.end(); ++icv){

				TString tmpname = icv->first;
				TCanvas *acv = icv->second;
				acv->Print(tmpname+TString(".png"));
			}		
		}
		

		
}

