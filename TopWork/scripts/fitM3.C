

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
	TH1F *hZjets;
	TH1F *hQCD;
	TH1F *hST_s;
	TH1F *hST_t;
	TH1F *hST_tW;
	
	TFile *infile0 = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_TTJets-madgraph_Fall08_all_all.root");
	hTTjets = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut1");
	
	TFile *infile1 = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_WJets_madgraph_Fall08_all.root");
	hWjets = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut1");

	TFile *infileZ = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_ZJets_madgraph_Fall08_all.root");
	hZjets = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut1");

	TFile *infileST_s = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_ST_s.root");
	hST_s = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut1");

	TFile *infileST_t = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_ST_t.root");
	hST_t = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut1");

	TFile *infileST_tW = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_ST_tW.root");
	hST_tW = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut1");

	TFile *infileQCD = TFile::Open("nominal_IPsig3_Iso95/TopAnalysis_InclusiveMuPt15_Summer08_all.root");
	hQCD = (TH1F*) gDirectory->Get("Mass/HadronicTop_mass_cut1");

	//TFile *outfile = TFile::Open("templates.root","RECREATE");
	//hTTjets->Write("ttbar");
	//hWjets->Write("Wjets");
	//outfile->Close();

	//overflow bin
	int maxbin=hTTjets->GetNbinsX();
	/*
	hTTjets->SetBinContent(maxbin, hTTjets->GetBinContent(maxbin+1)+hTTjets->GetBinContent(maxbin) );
	hWjets->SetBinContent(maxbin, hWjets->GetBinContent(maxbin+1)+hWjets->GetBinContent(maxbin) );
	hZjets->SetBinContent(maxbin, hZjets->GetBinContent(maxbin+1)+hZjets->GetBinContent(maxbin) );
	hQCD->SetBinContent(maxbin, hQCD->GetBinContent(maxbin+1)+hQCD->GetBinContent(maxbin) );
	hST_s->SetBinContent(maxbin, hST_s->GetBinContent(maxbin+1)+hST_s->GetBinContent(maxbin) );
	hST_t->SetBinContent(maxbin, hST_t->GetBinContent(maxbin+1)+hST_t->GetBinContent(maxbin) );
	hST_tW->SetBinContent(maxbin, hST_tW->GetBinContent(maxbin+1)+hST_tW->GetBinContent(maxbin) );
	*/
	
	// scale them to 20/pb
	hTTjets->Scale(0.0081);
	hWjets->Scale(0.0883);
	hZjets->Scale(0.0731);
	hQCD->Scale(0.4003);
	hST_s->Scale(0.003);
	hST_t->Scale(0.0027);
	hST_tW->Scale(0.0034);

	cout << " N expected ttbar+jets events = " << hTTjets->Integral() << endl;
	cout << " N expected W+jets     events = " << hWjets->Integral()  << endl;
	cout << " N expected Z+jets     events = " << hZjets->Integral()  << endl;
	cout << " N expected ST s       events = " << hST_s->Integral()  << endl;
	cout << " N expected ST t       events = " << hST_t->Integral()  << endl;
	cout << " N expected ST tW      events = " << hST_tW->Integral()  << endl;
	
	// add all three single top samples
	hST_t->Add(hST_s);
	hST_t->Add(hST_tW);

	// add Z+jets to W+jets sample
	//hWjets->Add(hZjets);
	//hWjets->Scale(1.+hZjets->Integral()/hWjets->Integral() + hQCD->Integral()/hWjets->Integral());
	
	double nexp_tt = hTTjets->Integral(); 
    double nexp_wj = hWjets->Integral();
	
	
	hM3 = (TH1F*) hTTjets->Clone("hM3");
	hM3->Add(hWjets);
	hM3->Add(hZjets);
	hM3->Add(hQCD);
	hM3->Add(hST_t);
	
	int Nbins = hM3->GetNbinsX();
	
	// --- Observable ---
        
	RooRealVar mass("mass","M3",100,500,"GeV/c^{2}") ; 
	RooRealVar Ntt("Ntt","number of t#bar{t} events",   nexp_tt, -500 , 1000);
    RooRealVar NW("NW","number of W+jets events", hWjets->Integral(), -500 , 1000);
	RooRealVar NST("NST","number of single top events", hST_t->Integral(), -50,50);
	RooRealVar NZjets("NZjets","number of Z+jets events", hZjets->Integral(), -50,50);
	RooRealVar Nqcd("Nqcd","number of QCD events", hQCD->Integral(), -50,50);
	
	RooDataHist hdata_ttbar("hdata_ttbar","ttbar", mass, hTTjets);       
	RooDataHist hdata_wjets("hdata_wjets","wjets", mass, hWjets);
	RooDataHist hdata_ST("hdata_ST","ST", mass, hST_t);
	RooDataHist hdata_zjets("hdata_zjets","zjets", mass, hZjets);
	RooDataHist hdata_qcd("hdata_qcd","qcd", mass, hQCD);
	
	RooHistPdf hpdf_ttbar("hpdf_ttbar","signal pdf", mass, hdata_ttbar, 0 );
	RooHistPdf hpdf_wjets("hpdf_wjets","W+jets pdf", mass, hdata_wjets, 0 );
	RooHistPdf hpdf_ST("hpdf_ST","ST pdf", mass, hdata_ST, 0 );
	RooHistPdf hpdf_zjets("hpdf_zjets","Z+jets pdf", mass, hdata_zjets, 0 );
	RooHistPdf hpdf_qcd("hpdf_qcd","qcd pdf", mass, hdata_qcd, 0 );
	
	RooAddPdf model_M3("modelM3","all", RooArgList(hpdf_ttbar,hpdf_wjets,hpdf_ST,hpdf_zjets,hpdf_qcd),
					   RooArgList(Ntt,NW,NST,NZjets,Nqcd) );
					   
	RooAddPdf model_histpdf("model", "TTjets+Wjets", RooArgList(hpdf_ttbar,hpdf_wjets,hpdf_ST),
							RooArgList(Ntt, NW, NST) ) ;

	// Construct another Gaussian constraint p.d.f on parameter f at n with resolution of sqrt(n)
	RooGaussian fconstext("fconstext","fconstext",NST,RooConst(hST_t->Integral()),RooConst(sqrt(hST_t->Integral())) );
	
	// --- Generate a toyMC sample 
	//RooMCStudy *mcstudyM3 = new RooMCStudy(model_M3, mass, Binned(kTRUE),Silence(),Extended(),
	//								   FitOptions(Save(kTRUE),Minos(kTRUE),Extended(), ExternalConstraints(fconstext)) );
	
	RooMCStudy *mcstudy = new RooMCStudy(model_M3, mass, FitModel(model_histpdf),Binned(kTRUE),Silence(), Extended() , 
										 //FitOptions(Save(kTRUE),Minos(kTRUE),Extended()) );
										 FitOptions(Save(kTRUE),Minos(kTRUE),Extended(),ExternalConstraints(fconstext)) ); //gaussian constraint
	
	// generate PEs
	int Nsamples = 1000;
	//mcstudyM3->generate(Nsamples,0,kFALSE,"toymc.dat");
	//mcstudyM3->generateAndFit(Nsamples,0,kFALSE,"toymc.dat");
	
	//TList dataList;
	//for (int isample=0; isample<Nsamples; ++isample) dataList.Add( mcstudyM3->genData(isample));

	
	// Fit
	mcstudy->generateAndFit(Nsamples,0,kTRUE);
	//mcstudy->fit(Nsamples, dataList);
	//mcstudy->fit(Nsamples, "toymc.dat");

		
	// E x p l o r e   r e s u l t s   o f   s t u d y 
	// ------------------------------------------------

	// Make plots of the distributions of mean, the error on mean and the pull of mean
	RooPlot* frame1 = mcstudy->plotParam(Ntt,Bins(40)) ;
	RooPlot* frame2 = mcstudy->plotError(Ntt,Bins(40)) ;
	RooPlot* frame3 = mcstudy->plotPull(Ntt,Bins(40),FitGauss(kTRUE)) ;
	RooPlot* frame1w = mcstudy->plotParam(NW,Bins(40)) ;
	RooPlot* frame2w = mcstudy->plotError(NW,Bins(40)) ;
	RooPlot* frame3w = mcstudy->plotPull(NW,Bins(40),FitGauss(kTRUE)) ;
	RooPlot* frame1st = mcstudy->plotParam(NST,Bins(40)) ;
	RooPlot* frame2st = mcstudy->plotError(NST,Bins(40)) ;
	//RooPlot* frame3st = mcstudy->plotPull(NST,Bins(40),FitGauss(kTRUE)) ;
	
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
	hpdf_ST.plotOn(initialframe,LineColor(kYellow));
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
	
	//cout << "name of argument:" << arglist[2].GetName() << endl;
	//cout << "name of argument:" << arglist[1].GetName() << endl;
	//cout << "name of argument:" << arglist[0].GetName() << endl;
		
	RooAddPdf model_histpdf_fitted("modelfitted", "TTjets+Wjets", RooArgList(hpdf_ttbar,hpdf_wjets,hpdf_ST),
								   RooArgList(arglist[2],arglist[1],arglist[0]) ) ;
	
	model_histpdf_fitted.plotOn(genframe2,LineColor(kRed));
	model_histpdf_fitted.plotOn(genframe2,Components(hpdf_wjets),LineColor(kGreen));
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
	TH2 *h2 = Ntt.createHistogram("Nttbar vs NWjets",NW);
	mcstudy->fitParDataSet().fillHistogram(h2,RooArgList(Ntt,NW));
	h2->Draw("box");

	
	TCanvas* cvc = new TCanvas("cvc","cvc",600,600) ;	
	// Plot distribution of minimized likelihood
	RooPlot* frame4 = mcstudy->plotNLL(Bins(40)) ;
	frame4->Draw();

	//return;//debuging

	
	TCanvas* cvd = new TCanvas("cvd","cvd",600,600) ;
	TCanvas* cve = new TCanvas("cve","cve",1200,600) ;
	TCanvas* cvf = new TCanvas("cvf","cvf",600,600) ;

	TH1F *hNgen = new TH1F("hNgen","Number of observed events",30,300,600);
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

