

//

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooMCStudy.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TH2.h"
#include "RooFitResult.h"
#include "TStyle.h"
#include "TDirectory.h"

using namespace RooFit ;


void fitM3()
{

	// LOAD HISTOGRAMS FROM FILES
	/////////////////////////////////
	TH1F *hTTjets;
	TH1F *hWjets;
	TH1F *hM3;
	TH1F *hZjets;
	TH1F *hQCD;
	TH1F *hST_s;
	TH1F *hST_t;
	TH1F *hST_tW;

	// histograms from nonimal sample
	///////////
	
	TFile *infile0 = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_TTJets-madgraph_Fall08_all_all.root");
	//TFile *infile0 = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_TauolaTTbar.root");
	hTTjets = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");
	
	TFile *infile1 = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_WJets_madgraph_Fall08_all.root");
	hWjets = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");
	
	TFile *infile1Fast = TFile::Open("nominal_IPsig3_Iso95_Fast/TopAnalysis_Wjets_madgraph_Winter09_v2_all.root");
	hWjetsFast = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	TFile *infileZ = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_ZJets_madgraph_Fall08_all.root");
	hZjets = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");
	TFile *infileZFast = TFile::Open("nominal_IPsig3_Iso95_Fast/TopAnalysis_Zjets_madgraph_Winter09_v2_all.root");
	hZjetsFast = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	TFile *infileST_s = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_ST_s.root");
	hST_s = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	TFile *infileST_t = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_ST_t.root");
	hST_t = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	TFile *infileST_tW = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_ST_tW.root");
	hST_tW = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	TFile *infileQCD = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_InclusiveMuPt15_Summer08_all.root");
	hQCD = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	// histograms from systematic samples
	//////////
	TFile *infile0S = TFile::Open("nominal_JESUp/TopAnalysis_TTJets-madgraph_Fall08_all_all.root");
	hTTjetsS = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");
		
	TFile *infile1S = TFile::Open("nominal_JESUp/TopAnalysis_WJets_madgraph_Fall08_all.root");// from FullSim
	hWjetsS = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	//TFile *infile1SF = TFile::Open("nominal_JESUp_Fast/TopAnalysis_WJets_madgraph_Fall08_all.root");// from FastSim
	//TFile *infile1SF = TFile::Open("nominal_IPsig3_Iso95_Fast/TopAnalysis_Wjets_ScaleUp_madgraph_Winter09_all.root");
	TFile *infile1SF = TFile::Open("nominal_IPsig3_Iso95_Fast/TopAnalysis_WJets_Threshold20GeV_madgraph_Winter09_all.root");
	hWjetsSFast = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	TFile *infileZS = TFile::Open("nominal_JESUp/TopAnalysis_ZJets_madgraph_Fall08_all.root");// from FullSim
	hZjetsS = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	TFile *infileZSF = TFile::Open("nominal_JESUp_Fast/TopAnalysis_ZJets_madgraph_Fall08_all.root");// from FullSim
	hZjetsSFast = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	TFile *infileST_sS = TFile::Open("nominal_JESUp/TopAnalysis_ST_s.root");
	hST_sS = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	TFile *infileST_tS = TFile::Open("nominal_JESUp/TopAnalysis_ST_t.root");
	hST_tS = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	TFile *infileST_tWS = TFile::Open("nominal_JESUp/TopAnalysis_ST_tW.root");
	hST_tWS = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	TFile *infileQCDS = TFile::Open("nominal_JESUp/TopAnalysis_InclusiveMuPt15_Summer08_all.root");//
	hQCDS = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");
	
	//TFile *infileQCD_CS = TFile::Open("nominal_antiMuon/TopAnalysis_InclusiveMuPt15_Summer08_all.root");
	//hQCD_CS = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut2");

	// write templates to file
	//TFile *outfile = TFile::Open("templates.root","RECREATE");
	//hTTjets->Write("ttbar");
	//hWjets->Write("Wjets");
	//outfile->Close();

	// Add over/underflow bins if requested
	bool UseOverflow = true;
	bool UseUnderflow = true;
	
	if (UseOverflow) {
		int maxbin=hTTjets->GetNbinsX();
	
		hTTjets->SetBinContent(maxbin,    hTTjets->GetBinContent(maxbin+1)+hTTjets->GetBinContent(maxbin) );
		hWjets->SetBinContent(maxbin,     hWjets->GetBinContent(maxbin+1)+hWjets->GetBinContent(maxbin) );
		hWjetsFast->SetBinContent(maxbin, hWjetsFast->GetBinContent(maxbin+1)+hWjetsFast->GetBinContent(maxbin) );
		hZjets->SetBinContent(maxbin,     hZjets->GetBinContent(maxbin+1)+hZjets->GetBinContent(maxbin) );
		hZjetsFast->SetBinContent(maxbin, hZjetsFast->GetBinContent(maxbin+1)+hZjetsFast->GetBinContent(maxbin) );
		hQCD->SetBinContent(maxbin,       hQCD->GetBinContent(maxbin+1)+hQCD->GetBinContent(maxbin) );
		//hQCD_CS->SetBinContent(maxbin, hQCD_CS->GetBinContent(maxbin+1)+hQCD_CS->GetBinContent(maxbin) );
		hST_s->SetBinContent(maxbin,      hST_s->GetBinContent(maxbin+1)+hST_s->GetBinContent(maxbin) );
		hST_t->SetBinContent(maxbin,      hST_t->GetBinContent(maxbin+1)+hST_t->GetBinContent(maxbin) );
		hST_tW->SetBinContent(maxbin,     hST_tW->GetBinContent(maxbin+1)+hST_tW->GetBinContent(maxbin) );
	}
	//underflow bin
	if (UseUnderflow) {
		int maxbin=1;
		hTTjets->SetBinContent(maxbin,    hTTjets->GetBinContent(maxbin-1)+hTTjets->GetBinContent(maxbin) );
		hWjets->SetBinContent(maxbin,     hWjets->GetBinContent(maxbin-1)+hWjets->GetBinContent(maxbin) );
		hWjetsFast->SetBinContent(maxbin, hWjetsFast->GetBinContent(maxbin-1)+hWjetsFast->GetBinContent(maxbin) );
		hZjets->SetBinContent(maxbin,     hZjets->GetBinContent(maxbin-1)+hZjets->GetBinContent(maxbin) );
		hZjetsFast->SetBinContent(maxbin, hZjetsFast->GetBinContent(maxbin-1)+hZjetsFast->GetBinContent(maxbin) );
		hQCD->SetBinContent(maxbin,       hQCD->GetBinContent(maxbin-1)+hQCD->GetBinContent(maxbin) );
		//hQCD_CS->SetBinContent(maxbin, hQCD_CS->GetBinContent(maxbin-1)+hQCD_CS->GetBinContent(maxbin) );
		hST_s->SetBinContent(maxbin,      hST_s->GetBinContent(maxbin-1)+hST_s->GetBinContent(maxbin) );
		hST_t->SetBinContent(maxbin,      hST_t->GetBinContent(maxbin-1)+hST_t->GetBinContent(maxbin) );
		hST_tW->SetBinContent(maxbin,     hST_tW->GetBinContent(maxbin-1)+hST_tW->GetBinContent(maxbin) );
	}
	//syst.
	if (UseOverflow) {
		int maxbin=hTTjetsS->GetNbinsX();
		hTTjetsS->SetBinContent(maxbin,    hTTjetsS->GetBinContent(maxbin+1)+hTTjetsS->GetBinContent(maxbin) );
		hWjetsS->SetBinContent(maxbin,     hWjetsS->GetBinContent(maxbin+1)+hWjetsS->GetBinContent(maxbin) );
		hWjetsSFast->SetBinContent(maxbin, hWjetsSFast->GetBinContent(maxbin+1)+hWjetsSFast->GetBinContent(maxbin) );
		hZjetsS->SetBinContent(maxbin,     hZjetsS->GetBinContent(maxbin+1)+hZjetsS->GetBinContent(maxbin) );
		hZjetsSFast->SetBinContent(maxbin, hZjetsSFast->GetBinContent(maxbin+1)+hZjetsSFast->GetBinContent(maxbin) );
		hQCDS->SetBinContent(maxbin,       hQCDS->GetBinContent(maxbin+1)+hQCDS->GetBinContent(maxbin) );
		hST_sS->SetBinContent(maxbin,      hST_sS->GetBinContent(maxbin+1)+hST_sS->GetBinContent(maxbin) );
		hST_tS->SetBinContent(maxbin,      hST_tS->GetBinContent(maxbin+1)+hST_tS->GetBinContent(maxbin) );
		hST_tWS->SetBinContent(maxbin,     hST_tWS->GetBinContent(maxbin+1)+hST_tWS->GetBinContent(maxbin) );
	}
	if (UseUnderflow) {
		//underflow bin
		int maxbin=1;
		hTTjetsS->SetBinContent(maxbin,    hTTjetsS->GetBinContent(maxbin-1)+hTTjetsS->GetBinContent(maxbin) );
		hWjetsS->SetBinContent(maxbin,     hWjetsS->GetBinContent(maxbin-1)+hWjetsS->GetBinContent(maxbin) );
		hWjetsSFast->SetBinContent(maxbin, hWjetsSFast->GetBinContent(maxbin-1)+hWjetsSFast->GetBinContent(maxbin) );
		hZjetsS->SetBinContent(maxbin,     hZjetsS->GetBinContent(maxbin-1)+hZjetsS->GetBinContent(maxbin) );
		hZjetsSFast->SetBinContent(maxbin,     hZjetsSFast->GetBinContent(maxbin-1)+hZjetsSFast->GetBinContent(maxbin) );
		hQCDS->SetBinContent(maxbin,       hQCDS->GetBinContent(maxbin-1)+hQCDS->GetBinContent(maxbin) );
		hST_sS->SetBinContent(maxbin,      hST_sS->GetBinContent(maxbin-1)+hST_sS->GetBinContent(maxbin) );
		hST_tS->SetBinContent(maxbin,      hST_tS->GetBinContent(maxbin-1)+hST_tS->GetBinContent(maxbin) );
		hST_tWS->SetBinContent(maxbin,     hST_tWS->GetBinContent(maxbin-1)+hST_tWS->GetBinContent(maxbin) );
	}
	
	// scale histograms to 20/pb

	hTTjets->Scale(0.0081); // madgraph
	//hTTjets->Scale(0.0777);//Tauola
	hWjets->Scale(0.0883);
	//hWjetsFast->Scale(0.0091); //fastsim
	hWjetsFast->Scale(hWjets->Integral() / hWjetsFast->Integral()); // scale to FullSim
		
	hZjets->Scale(0.0731);
	hZjetsFast->Scale(hZjets->Integral()/hZjetsFast->Integral()); //scale to FullSim
	hQCD->Scale(0.4003);
	hQCD_WFast = (TH1F*) hWjetsFast->Clone("hQCD_WFast"); //take shape from Wjets
	hQCD_WFast->Scale(hQCD->Integral()/hQCD_WFast->Integral()); //scale to FullSim
	hST_t->Scale(0.003);
	hST_s->Scale(0.0027);
	hST_tW->Scale(0.0034);

	hTTjetsS->Scale(0.0081); // 
	//hTTjetsS->Scale(0.0008); // for fastsim
	hWjetsS->Scale(0.0883);
	//hWjetsS->Scale(0.0091);// from fastsim
	//hWjetsSFast->Scale(hWjetsS->Integral() / hWjetsSFast->Integral()); // scale to FullSim
	//hWjetsSFast->Scale(0.6642); // scaleUP
	//hWjetsSFast->Scale(0.8041); // scaleDown
	//hWjetsSFast->Scale(0.0605); // threshold 5gev
	hWjetsSFast->Scale(0.042); // threshold 20gev
	
	hZjetsS->Scale(0.0731);
	//hZjetsS->Scale(0.0085);// from fastsim
	hZjetsSFast->Scale(hZjetsS->Integral() / hZjetsSFast->Integral()); // scale to FullSim
	hQCDS->Scale(0.4003);
	//hQCDS_WFast = (TH1F*) hWjetsS->Clone("hQCDS_WFast");
	//hQCDS_WFast->Scale(hQCDS->Integral()/hQCDS_WFast->Integral());
	hST_tS->Scale(0.003);
	hST_sS->Scale(0.0027);
	hST_tWS->Scale(0.0034);
	
	cout << " N expected ttbar+jets events = " << hTTjets->Integral() << endl;
	cout << " N expected W+jets     events = " << hWjets->Integral()  << endl;
	cout << " N expected Z+jets     events = " << hZjets->Integral()  << endl;
	cout << " N expected ST s       events = " << hST_s->Integral()  << endl;
	cout << " N expected ST t       events = " << hST_t->Integral()  << endl;
	cout << " N expected ST tW      events = " << hST_tW->Integral()  << endl;
	cout << " N expected qcd        events = " << hQCD->Integral()  << endl;

	cout << endl;
	cout << " N expected W+jets fast = " << hWjetsFast->Integral() << endl;
	cout << " N expected z+jets fast = " << hZjetsFast->Integral() << endl;
	cout << " N expected qcd Wfast = " << hQCD_WFast->Integral() << endl;

	cout << "\n systematics: " << endl;
	cout << " N expected W+jets fast = " << hWjetsSFast->Integral() << endl;
	cout << " N expected z+jets fast = " << hZjetsS->Integral() << endl;
	cout << " N expected qcd Wfast = " << hQCDS->Integral() << endl;

	// add all three single top samples

	// for systematics
	hST_t->Scale(2.);
	
	hST_t->Add(hST_s);
	hST_t->Add(hST_tW);
	cout << " number of ST = " << hST_t->Integral() << endl;
	// syst. uncertainty in single top
	//double tmpST = 0.6* hST_t->Integral();
	//hST_t->Scale(0.6);
	//cout << tmpST << endl;
	cout << " New number of ST = " << hST_t->Integral() << endl;
	
	hST_tS->Add(hST_sS);
	hST_tS->Add(hST_tWS);

	// dump scaled histograms in root file
	//TFile *output = TFile::Open("fitM3.root","RECREATE");
	//hTTjets->SetName("ttbar");hTTjets->Write();
	//hWjetsFast->SetName("WjetsFast");hWjetsFast->Write();
	//hST_t->SetName("ST");hST_t->Write();
	//output->Close();
	
		
	hM3 = (TH1F*) hTTjets->Clone("hM3");
	hM3->Add(hWjets);
	hM3->Add(hZjets);
	hM3->Add(hQCD);
	hM3->Add(hST_t);
	
	int Nbins = hM3->GetNbinsX();
	
	// --- Observable ---
        
	RooRealVar mass("mass","M3'(#chi^{2})",100,500,"GeV/c^{2}") ; 
	RooRealVar Ntt("Ntt","number of t#bar{t} events", hTTjets->Integral(), -100 , 1000);
    RooRealVar NW("NW","number of W+jets events", hWjetsFast->Integral(), -500 , 1000);
	RooRealVar NST("NST","number of single top events", hST_t->Integral(), -500,100);
	RooRealVar NZjets("NZjets","number of Z+jets events", hZjetsS->Integral(), -500,500);
	RooRealVar Nqcd("Nqcd","number of QCD events", hQCD_WFast->Integral(), -500,100);
	//RooRealVar Nbkg("Nbkg","number of bkg events", hWjetsFast->Integral()+hST_t->Integral()+hZjetsFast->Integral()+hQCD_WFast->Integral(), -500 , 1000);
	//RooRealVar Nbkg("Nbkg","number of W+jets events", hWjets->Integral(), -500 , 1000); // 2 templates

	RooRealVar Nbkg("Nbkg","number of bkg events", hWjetsFast->Integral()+hZjets->Integral()+hQCD_WFast->Integral(), -500 , 1000);
	//RooRealVar Nbkg("Nbkg","number of bkg events", hWjetsFast->Integral(), -500 , 1000);
	
	// for systematics
	//RooRealVar Nbkg("Nbkg","number of bkg events", hWjetsSFast->Integral()+hZjetsS->Integral()+hQCDS->Integral(), -500 , 1000);
	//RooRealVar Nbkg("Nbkg","number of bkg events", hWjetsSFast->Integral(), -500 , 1000);
	
	mass.setBins(Nbins);

	// RooFit datasets
	RooDataHist hdata_ttbar("hdata_ttbar","ttbar", mass, hTTjets);       
	//RooDataHist hdata_wjets("hdata_wjets","wjets", mass, hWjets);
	RooDataHist hdata_wjetsFast("hdata_wjetsFast","wjets_Fast", mass, hWjetsFast);
	RooDataHist hdata_ST("hdata_ST","ST", mass, hST_t);
	RooDataHist hdata_zjets("hdata_zjets","zjets", mass, hZjets);
	//RooDataHist hdata_qcd("hdata_qcd","qcd", mass, hQCD);
	RooDataHist hdata_zjetsFast("hdata_zjetsFast","zjets_Fast", mass, hZjetsFast);
	RooDataHist hdata_qcdWFast("hdata_qcdWFast","qcd WFast", mass, hQCD_WFast);
	
	RooHistPdf hpdf_ttbar("hpdf_ttbar","signal pdf", mass, hdata_ttbar, 0 );
	//RooHistPdf hpdf_wjets("hpdf_wjets","W+jets pdf", mass, hdata_wjets, 0 );
	RooHistPdf hpdf_wjetsFast("hpdf_wjetsFast","W+jets pdf", mass, hdata_wjetsFast, 0 );
	RooHistPdf hpdf_ST("hpdf_ST","ST pdf", mass, hdata_ST, 0 );
	//RooHistPdf hpdf_zjets("hpdf_zjets","Z+jets pdf", mass, hdata_zjets, 0 );
	//RooHistPdf hpdf_qcd("hpdf_qcd","qcd pdf", mass, hdata_qcd, 0 );
	RooHistPdf hpdf_zjetsFast("hpdf_zjetsFast","Z+jets pdf", mass, hdata_zjetsFast, 0 );
	RooHistPdf hpdf_qcdWFast("hpdf_qcdWFast","qcd WFast pdf", mass, hdata_qcdWFast, 0 );
	
	// for systematics
	RooDataHist hdata_ttbarS("hdata_ttbarS","ttbar", mass, hTTjetsS);       
	RooDataHist hdata_wjetsS("hdata_wjetsS","wjets", mass, hWjetsSFast);
	RooDataHist hdata_STS("hdata_STS","ST", mass, hST_tS);
	RooDataHist hdata_zjetsS("hdata_zjetsS","zjets", mass, hZjetsSFast);
	RooDataHist hdata_qcdS("hdata_qcdS","qcd", mass, hQCDS);
	//RooDataHist hdata_qcdSWFast("hdata_qcdSWFast","qcd WFast", mass, hQCDS_WFast);
		
	RooHistPdf hpdf_ttbarS("hpdf_ttbarS","signal pdf", mass, hdata_ttbarS, 0 );
	RooHistPdf hpdf_wjetsS("hpdf_wjetsS","W+jets pdf", mass, hdata_wjetsS, 0 );
	RooHistPdf hpdf_STS("hpdf_STS","ST pdf", mass, hdata_STS, 0 );
	RooHistPdf hpdf_zjetsS("hpdf_zjetsS","Z+jets pdf", mass, hdata_zjetsS, 0 );
	RooHistPdf hpdf_qcdS("hpdf_qcdS","qcd pdf", mass, hdata_qcdS, 0 );
	//RooHistPdf hpdf_qcdSWFast("hpdf_qcdSWFast","qcd WFast pdf", mass, hdata_qcdSWFast, 0 );

	//RooAddPdf hpdf_bkg("hpdf_bkg","bkg", RooArgList(hpdf_wjetsFast,hpdf_ST,hpdf_qcdWFast),
	//				   RooArgList(NW,NST,Nqcd) );
					   
	//RooAddPdf hpdf_bkg("hpdf_bkg","bkg", RooArgList(hpdf_wjetsFast,hpdf_ST,hpdf_zjetsFast,hpdf_qcdWFast),
					   //RooAddPdf hpdf_bkg("hpdf_bkg","bkg", RooArgList(hpdf_wjetsS,hpdf_STS,hpdf_zjetsS,hpdf_qcdSWFast),
					   //RooArgList(NW,NST,NZjets,Nqcd) );
// only two pdfs: ttbar + Wjets
//RooHistPdf hpdf_bkg = hpdf_wjetsFast;
	
	//RooAddPdf model_M3("modelM3","all", RooArgList(hpdf_ttbar,hpdf_wjetsFast,hpdf_ST,hpdf_zjetsFast,hpdf_qcdWFast),
	//			   RooArgList(Ntt,Nbkg,NST,NZjets,Nqcd));
	// for systematics
	RooAddPdf model_M3("modelM3","all", RooArgList(hpdf_ttbar,hpdf_wjetsFast,hpdf_ST),//RooArgList(hpdf_ttbar,hpdf_wjetsS,hpdf_ST),
					   RooArgList(Ntt,Nbkg,NST) );
	
	//RooAddPdf model_M3("modelM3","all",RooArgList(hpdf_ttbar,hpdf_bkg),
	//		   RooArgList(Ntt,Nbkg) );
	//RooArgList(Ntt,Nbkg,NST,Nqcd) );
	
	RooAddPdf model_histpdf("model", "TTjets+Wjets", RooArgList(hpdf_ttbar,hpdf_wjetsFast,hpdf_ST),
							RooArgList(Ntt, Nbkg, NST) ) ;

	// Construct another Gaussian constraint p.d.f on parameter f at n with resolution of sqrt(n)
	RooGaussian STgaussConstraint("STgaussConstraint","STgaussConstraint",NST,RooConst(hST_t->Integral()),RooConst(sqrt(hST_t->Integral() + (0.3*hST_t->Integral())*(0.3*hST_t->Integral()))) );
	//RooGaussian fconstext2("fconstext2","fconstext2",NZjets,RooConst(hZjets->Integral()),RooConst(sqrt(hZjets->Integral())) );
	
	// --- Generate a toyMC sample 
	//RooMCStudy *mcstudyM3 = new RooMCStudy(model_M3, mass, Binned(kTRUE),Silence(),Extended(),
	//								   FitOptions(Save(kTRUE),Minos(kTRUE),Extended(), ExternalConstraints(fconstext)) );

	// generate PEs
	int Nsamples = 1000;
	// PEs for ttbar
/*
	RooExtendPdf ext_hpdf_ttbar("ext_hpdf_ttbar","ext_hpdf_ttbar",hpdf_ttbar,Ntt);
	RooExtendPdf ext_hpdf_wjets("ext_hpdf_wjets","ext_hpdf_wjets",hpdf_wjetsFast,NW);
	RooExtendPdf ext_hpdf_zjets("ext_hpdf_zjets","ext_hpdf_zjets",hpdf_zjetsFast,NZjets);
	RooExtendPdf ext_hpdf_qcd("ext_hpdf_qcd","ext_hpdf_qcd",hpdf_qcdWFast,Nqcd);
	RooExtendPdf ext_hpdf_ST("ext_hpdf_ST","ext_hpdf_ST",hpdf_ST,NST);
	
	RooMCStudy *mc_ttbar = new RooMCStudy(ext_hpdf_ttbar,mass,Binned(kTRUE),Silence(kTRUE));
	mc_ttbar->generate(Nsamples,0,kFALSE,"data/toymc_ttbar_%04d.dat");
	RooMCStudy *mc_wjets = new RooMCStudy(ext_hpdf_wjets,mass,Binned(kTRUE),Silence(kTRUE));
	mc_wjets->generate(Nsamples,0,kFALSE,"data/toymc_wjets_%04d.dat");
	RooMCStudy *mc_zjets = new RooMCStudy(ext_hpdf_zjets,mass,Binned(kTRUE),Silence(kTRUE));
	mc_zjets->generate(Nsamples,0,kFALSE,"data/toymc_zjets_%04d.dat");
	RooMCStudy *mc_qcd = new RooMCStudy(ext_hpdf_qcd,mass,Binned(kTRUE),Silence(kTRUE));
	mc_qcd->generate(Nsamples,0,kFALSE,"data/toymc_qcd_%04d.dat");
	RooMCStudy *mc_ST = new RooMCStudy(ext_hpdf_ST,mass,Binned(kTRUE),Silence(kTRUE),FitOptions(ExternalConstraints(STgaussConstraint)));
	mc_ST->generate(Nsamples,0,kFALSE,"data/toymc_ST_%04d.dat");

	return;
*/	
	RooMCStudy *mcstudy = new RooMCStudy(model_M3, mass, FitModel(model_histpdf),Binned(kTRUE),Silence(kTRUE), Extended() , 
										 //FitOptions(Save(kTRUE),Minos(kTRUE),Extended()) );
										 FitOptions(Save(kTRUE),Minos(kTRUE),Extended(),ExternalConstraints(STgaussConstraint)));//RooArgList(fconstext,fconstext2)) )); //gaussian constraint
	
		
	//mcstudyM3->generate(Nsamples,0,kFALSE,"toymc.dat");
	//mcstudyM3->generateAndFit(Nsamples,0,kFALSE,"toymc.dat");
	
	//TList dataList;
	//for (int isample=0; isample<Nsamples; ++isample) dataList.Add( mcstudyM3->genData(isample));

	
	// Fit
	mcstudy->generateAndFit(Nsamples,0,kTRUE);
	//mcstudy->fit(Nsamples, "data/toymc_%04d.dat");

		
	gDirectory->Add(mcstudy) ;	
	// E x p l o r e   r e s u l t s   o f   s t u d y 
	// ------------------------------------------------

	// Make plots of the distributions of mean, the error on mean and the pull of mean
	RooPlot* frame1 = mcstudy->plotParam(Ntt,Bins(40));
	RooPlot* frame2 = mcstudy->plotError(Ntt,Bins(40)) ;
	RooPlot* frame3 = mcstudy->plotPull(Ntt,Bins(40),FitGauss(kTRUE)) ;
	RooPlot* frame1w = mcstudy->plotParam(Nbkg,Bins(40)) ;
	RooPlot* frame2w = mcstudy->plotError(Nbkg,Bins(40)) ;
	RooPlot* frame3w = mcstudy->plotPull(Nbkg,Bins(40),FitGauss(kTRUE)) ;
	RooPlot* frame1st = mcstudy->plotParam(NST,Bins(40)) ;
	RooPlot* frame2st = mcstudy->plotError(NST,Bins(40)) ;
	//RooPlot* frame3st = mcstudy->plotPull(NST,Bins(40),FitGauss(kTRUE)) ;
	
	// Plot distribution of minimized likelihood
	RooPlot* frame4 = mcstudy->plotNLL(Bins(40)) ;

	// Make some histograms from the parameter dataset
	TH1* hh_cor_ttbar_w = mcstudy->fitParDataSet().createHistogram("hh",Ntt,YVar(Nbkg)) ;

	// Access some of the saved fit results from individual toys
	//TH2* corrHist000 = mcstudy->fitResult(0)->correlationHist("c000") ;
	//TH2* corrHist127 = mcstudy->fitResult(127)->correlationHist("c127") ;
	//TH2* corrHist953 = mcstudy->fitResult(953)->correlationHist("c953") ;

	
	// Draw all plots on a canvas
	gStyle->SetPalette(1) ;
	gStyle->SetOptStat(0) ;

	TCanvas* cv = new TCanvas("cv","cv",600,600) ;
	hM3->SetFillColor(kRed);
	hWjets->SetFillColor(kGreen);
	hM3->Draw();
	hWjets->Draw("same");
	gPad->RedrawAxis();
	
	TCanvas* cva = new TCanvas("cva","cva",1800,600) ;
	cva->Divide(3);
	cva->cd(1) ;
	RooPlot *initialframe = mass.frame();
	//initial->SetMaximum(10);
	hpdf_ttbar.plotOn(initialframe,LineColor(kRed));
	hpdf_wjetsFast.plotOn(initialframe,LineColor(kGreen));
	hpdf_ST.plotOn(initialframe,LineColor(kYellow));
	initialframe->Draw();
	//initialframe->SetTitle();
	cva->cd(2);
	//retrieve data for only one PE
	
	int Npe = 10;
	RooPlot *genframe = mass.frame(Nbins);
	RooDataSet *gendata = mcstudy->genData(Npe);
	cout << " N events = " << gendata->numEntries() << endl;
	gendata->plotOn(genframe);
	//mcstudy->fitResult(Npe)->plotOn(genframe, model_histpdf);
	genframe->Draw();
	cva->cd(3);
	RooPlot *genframe2 = mass.frame(Nbins);
	mcstudy->fitResult(Npe)->Print("v");
	gendata->plotOn(genframe2);
	RooArgList arglist = mcstudy->fitResult(Npe)->floatParsFinal();

	
	//cout << "name of argument:" << arglist[2].GetName() << endl;
	//cout << "name of argument:" << arglist[1].GetName() << endl;
	//cout << "name of argument:" << arglist[0].GetName() << endl;
	
	RooAddPdf model_histpdf_fitted("modelfitted", "TTjets+Wjets", RooArgList(hpdf_ttbar,hpdf_wjetsFast,hpdf_ST),
							   RooArgList(arglist[2],arglist[1],arglist[0]) ) ;
	
	model_histpdf_fitted.plotOn(genframe2,LineColor(kRed));
	model_histpdf_fitted.plotOn(genframe2,Components(hpdf_wjetsFast),LineColor(kGreen));
	model_histpdf_fitted.plotOn(genframe2,Components(hpdf_ST),LineColor(kYellow));
	genframe2->Draw();

	TCanvas* cvb = new TCanvas("cvb","cvb",1800,600) ;
	cvb->Divide(3);
	cvb->cd(1) ; frame1->Draw();
	cvb->cd(2) ; frame2->Draw();
	cvb->cd(3) ; frame3->Draw();
	TCanvas* cvbb = new TCanvas("cvbb","cvbb",1800,600) ;
	cvbb->Divide(3);
	cvbb->cd(1) ; frame1w->Draw();
	cvbb->cd(2) ; frame2w->Draw();
	cvbb->cd(3) ; frame3w->Draw();

	TCanvas* cvbbb = new TCanvas("cvbbb","cvbbb",1200,600) ;
	cvbbb->Divide(2);
	cvbbb->cd(1) ; frame1st->Draw();
	cvbbb->cd(2) ; frame2st->Draw();
	//cvbbb->cd(3) ; frame3st->Draw();
	
	TCanvas* cvbc = new TCanvas("cvbc","cvbc",600,600) ;
	TH2 *h2 = Ntt.createHistogram("Nttbar vs NWjets",Nbkg);
	mcstudy->fitParDataSet().fillHistogram(h2,RooArgList(Ntt,Nbkg));
	h2->Draw("box");

	
	TCanvas* cvc = new TCanvas("cvc","cvc",600,600) ;	
	// Plot distribution of minimized likelihood
	RooPlot* frame4 = mcstudy->plotNLL(Bins(40)) ;
	frame4->Draw();

	//return;//debuging

	
	TCanvas* cvd = new TCanvas("cvd","cvd",600,600) ;
	TCanvas* cve = new TCanvas("cve","cve",1200,600) ;
	TCanvas* cvf = new TCanvas("cvf","cvf",600,600) ;

	TH1F *hNgen = new TH1F("hNgen","Number of observed events",30,350,650);
	hNgen->SetXTitle("Number of observed events");

	TH1F *hNttresults = new TH1F("hNttresults","number of ttbar events",50,20,600);
	TH1F *hNWresults = new TH1F("hNWresults","number of W events",50,-150,400);
	TH1F *hNSTresults = new TH1F("hNSTresults","number of ttbar events",50,5,25);
									 
	bool gotone = false;
	int Nfailed = 0;
	for ( int i=0; i< Nsamples; i++)
	{
		
		RooFitResult *r = mcstudy->fitResult(i);
		RooArgList list = r->floatParsFinal();
		RooRealVar *rrv_nt = (RooRealVar*)list.at(2);
		double nt = rrv_nt->getVal();
		//double nte= rrv_nt->getError();
		RooRealVar *rrv_nw = (RooRealVar*)list.at(1);
		double nw = rrv_nw->getVal();
		//double nwe= rrv_nw->getError();
		RooRealVar *rrv_nst = (RooRealVar*)list.at(0);
		double nst = rrv_nst->getVal();

				
		hNttresults->Fill(nt);
		hNWresults->Fill(nw);
		hNSTresults->Fill(nst);
		
		
		RooDataSet *adata = mcstudy->genData(i);
		hNgen->Fill(adata->numEntries());
		
		if ( r->numInvalidNLL() > 0 ) Nfailed++;
		
		
		/*
		if ( false ) {
			cout << " sample # " << i << endl;
			gotone = true;
			r->Print("v");
			cout << " invalidNLL = "<< r->numInvalidNLL() << endl;
			cout << " N events = " << adata->numEntries() << endl;

			
			RooAddPdf amodel("amodel", "TTjets+Wjets", RooArgList(hpdf_ttbar,hpdf_wjets,hpdf_ST),
							 RooArgList(list[2],list[1],list[0])) ;
			RooPlot *d2 = new RooPlot(Ntt,NW,0,500,-200,200);
			r->plotOn(d2,Ntt,NW,"ME12ABHV");
			cvd->cd();
			d2->Draw();
			
			RooNLLVar nll("nll","nll", amodel, *adata, Extended() );//, Extended(), PrintEvalErrors(-1) );
			RooMinuit myminuit(nll)
			myminuit.migrad();
			myminuit.hesse();
			myminuit.minos();
			//myminuit.Save()->Print("v");

			cve->Divide(2);
			RooPlot *nllframett = Ntt.frame(Bins(50),Range(100,600));//,Range(10,2000));
			nll.plotOn(nllframett);//,ShiftToZero());
						
			RooProfileLL pll_ntt("pll_ntt","pll_ntt",nll,Ntt);
			pll_ntt.plotOn(nllframett,LineColor(kRed));

			RooPlot *nllframeW = NW.frame(Bins(50),Range(0,250));//,Range(10,2000));
			nll.plotOn(nllframeW);//,ShiftToZero());
						
			RooProfileLL pll_nW("pll_nW","pll_nW",nll,NW);
			pll_nW.plotOn(nllframeW,LineColor(kRed));

			cve->cd(1);
			nllframett->SetMaximum(2);
			nllframett->Draw();
			cve->cd(2);
			nllframeW->SetMaximum(2);
			nllframeW->Draw();
			
		}
		*/
	}

	TCanvas *tmpcv = new TCanvas("tmpcv","tmpcv",700,700);
	cout << "\n ==================================" << endl;
	cout << "gaussian fit of Nttbar fitted values: " << endl;
	//hNttresults->Print("all");
	hNttresults->Fit("gaus");

	cout << "\n ==================================" << endl;
	cout << "gaussian fit of NW fitted values: " << endl;
	//hNWresults->Print("all");
	hNWresults->Fit("gaus");

	cout << "\n ==================================" << endl;
	cout << "gaussian fit of NST fitted values: " << endl;
	//hNSTresults->Print("all");
	hNSTresults->Fit("gaus");

	
	cout << "N failed fits = " << Nfailed << endl;
	
	cvf->cd();
	hNgen->Draw();
	
	// Make RooMCStudy object available on command line after
	// macro finishes
	//gDirectory->Add(mcstudy) ;
}

