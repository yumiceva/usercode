

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

	// load templates
	TH1F *hTTjets;
	TH1F *hWjets;
	TH1F *hM3;
	
	TFile *infile0 = TFile::Open("nominalIP3/TopAnalysis_TTJets-madgraph_Fall08_all_all.root");
	hTTjets = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut1");
	
	TFile *infile1 = TFile::Open("nominalIP3/TopAnalysis_WJets_madgraph_Fall08_all.root");
	hWjets = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut1");

	//TFile *outfile = TFile::Open("templates.root","RECREATE");
	//hTTjets->Write("ttbar");
	//hWjets->Write("Wjets");
	//outfile->Close();
	
	// scale them to 10/pb
	hTTjets->Scale(0.0094);
	hWjets->Scale(0.0177);
	double nexp_tt = hTTjets->Integral(); 
    double nexp_wj = hWjets->Integral();

	cout << " N expected ttbar+jets events = " << nexp_tt << endl;
	cout << " N expected W+jets     events = " << nexp_wj << endl;
	
	hM3 = (TH1F*) hTTjets->Clone("hM3");
	hM3->Add(hWjets);
	int Nbins = hM3->GetNbinsX();
	
	// --- Observable ---
        
	RooRealVar mass("mass","M3",100,500,"GeV/c^{2}") ; 
	RooRealVar Ntt("Ntt","number of t#bar{t} events",   nexp_tt, -500 , 1000);
    RooRealVar NW("NW","number of W+jets events", nexp_wj, -500 , 1000);

	RooDataHist hdata_ttbar("hdata_ttbar","ttbar", mass, hTTjets);       
	RooDataHist hdata_wjets("hdata_wjets","wjets", mass, hWjets);
	RooHistPdf hpdf_ttbar("hpdf_ttbar","signal pdf", mass, hdata_ttbar, 0 );
	RooHistPdf hpdf_wjets("hpdf_wjets","background pdf", mass, hdata_wjets, 0 );
		
	RooAddPdf model_histpdf("model", "TTjets+Wjets", RooArgList(hpdf_ttbar,hpdf_wjets),
							RooArgList(Ntt, NW) ) ;

	
	// --- Generate a toyMC sample 
        
	RooMCStudy *mcstudy = new RooMCStudy(model_histpdf, mass, Binned(kTRUE),Silence(), Extended() , 
										 FitOptions(Save(kTRUE),Minos(kTRUE),Extended()) );
	
	// generate and fit
	int Nsamples = 1000;
	mcstudy->generateAndFit(Nsamples,0,kTRUE);

	// E x p l o r e   r e s u l t s   o f   s t u d y 
	// ------------------------------------------------

	// Make plots of the distributions of mean, the error on mean and the pull of mean
	RooPlot* frame1 = mcstudy->plotParam(Ntt,Bins(40)) ;
	RooPlot* frame2 = mcstudy->plotError(Ntt,Bins(40)) ;
	RooPlot* frame3 = mcstudy->plotPull(Ntt,Bins(40),FitGauss(kTRUE)) ;
	RooPlot* frame1w = mcstudy->plotParam(NW,Bins(40)) ;
	RooPlot* frame2w = mcstudy->plotError(NW,Bins(40)) ;
	RooPlot* frame3w = mcstudy->plotPull(NW,Bins(40),FitGauss(kTRUE)) ;
	
	// Plot distribution of minimized likelihood
	RooPlot* frame4 = mcstudy->plotNLL(Bins(40)) ;

	// Make some histograms from the parameter dataset
	TH1* hh_cor_ttbar_w = mcstudy->fitParDataSet().createHistogram("hh",Ntt,YVar(NW)) ;

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
	hpdf_wjets.plotOn(initialframe,LineColor(kGreen));
	initialframe->Draw();
	//initialframe->SetTitle();
	cva->cd(2);
	//retrieve data:
	RooPlot *genframe = mass.frame(Nbins);
	RooDataSet *gendata = mcstudy->genData(10);
	cout << " N events = " << gendata->numEntries() << endl;
	gendata->plotOn(genframe);
	//mcstudy->fitResult(10)->plotOn(genframe, model_histpdf);
	genframe->Draw();
	cva->cd(3);
	RooPlot *genframe2 = mass.frame(Nbins);
	mcstudy->fitResult(10)->Print("v");
	gendata->plotOn(genframe2);
	RooArgList arglist = mcstudy->fitResult(10)->floatParsFinal();
	RooAddPdf model_histpdf_fitted("modelfitted", "TTjets+Wjets", RooArgList(hpdf_ttbar,hpdf_wjets),
								   RooArgList(arglist[1],arglist[0])) ;
	model_histpdf_fitted.plotOn(genframe2,LineColor(kRed));
	model_histpdf_fitted.plotOn(genframe2,Components(hpdf_wjets),LineColor(kGreen));
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

	TCanvas* cvbc = new TCanvas("cvbc","cvbc",600,600) ;
	TH2 *h2 = Ntt.createHistogram("Nttbar vs NWjets",NW);
	mcstudy->fitParDataSet().fillHistogram(h2,RooArgList(Ntt,NW));
	h2->Draw("box");

	
	TCanvas* cvc = new TCanvas("cvc","cvc",600,600) ;	
	// Plot distribution of minimized likelihood
	RooPlot* frame4 = mcstudy->plotNLL(Bins(40)) ;
	frame4->Draw();

	
	TCanvas* cvd = new TCanvas("cvd","cvd",600,600) ;
	TCanvas* cve = new TCanvas("cve","cve",1200,600) ;
	TCanvas* cvf = new TCanvas("cvf","cvf",600,600) ;

	TH1F *hNgen = new TH1F("hNgen","Number of observed events",30,100,500);
	hNgen->SetXTitle("Number of observed events");
	
	bool gotone = false;
	int Nfailed = 0;
	for ( int i=0; i< Nsamples; i++)
	{
		RooFitResult *r = mcstudy->fitResult(i);
		RooArgList list = r->floatParsFinal();
		RooRealVar *rrv_nt = (RooRealVar*)list.at(1);
		double nt = rrv_nt->getVal();
		double nte= rrv_nt->getError();
		RooRealVar *rrv_nw = (RooRealVar*)list.at(0);
		double nw = rrv_nw->getVal();
		double nwe= rrv_nw->getError();

		RooDataSet *adata = mcstudy->genData(i);
		hNgen->Fill(adata->numEntries());

		if ( r->numInvalidNLL() > 0 ) Nfailed++;
		
		if ( !gotone && nt > 250 && nt< 270 )
		{
			cout << " sample # " << i << endl;
			gotone = true;
			r->Print("v");
			cout << " invalidNLL = "<< r->numInvalidNLL() << endl;
			cout << " N events = " << adata->numEntries() << endl;

			
			RooAddPdf amodel("amodel", "TTjets+Wjets", RooArgList(hpdf_ttbar,hpdf_wjets),
							 RooArgList(list[1],list[0])) ;
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
			RooPlot *nllframett = Ntt.frame(Bins(50),Range(0,500));//,Range(10,2000));
			nll.plotOn(nllframett);//,ShiftToZero());
						
			RooProfileLL pll_ntt("pll_ntt","pll_ntt",nll,Ntt);
			pll_ntt.plotOn(nllframett,LineColor(kRed));

			RooPlot *nllframeW = NW.frame(Bins(50),Range(-50,200));//,Range(10,2000));
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
	}

	cout << "N failed fits = " << Nfailed << endl;
	
	cvf->cd();
	hNgen->Draw();
	
	// Make RooMCStudy object available on command line after
	// macro finishes
	gDirectory->Add(mcstudy) ;
}

