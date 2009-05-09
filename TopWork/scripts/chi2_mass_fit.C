void chi2_mass_fit(int type=0,bool smear=false, double lumi=20.) {

	gSystem->Load("libRooFit");
	using namespace RooFit;

	TString decay = "Hadronic";
	if (type==1) decay = "Leptonic";

	if (lumi==20) lumi = 1.;//already scaled to 20/pb.
	else lumi = lumi/10.;

	// get histograms
	TFile *infile = TFile::Open("cuy_IPsig3_Iso95.root");

	TH1D *hadTop_tTj;
	TH1D *hadTop_tTjO;
	TH1D *hadTop_Wjets;
	TH1D *hadTop_Zjets;
	TH1D *hadTop_qcd;
	TH1D *hadTop_ST_s;
	TH1D *hadTop_ST_t;
	TH1D *hadTop_ST_tW;
	TH1D *hadTop_tTj_c;
	TH1D *hadTop_tTjO_c;
	TH1D *hadTop_Wjets_c;
	TH1D *hadTop_Zjets_c;
	TH1D *hadTop_qcd_c;
	TH1D *hadTop_ST_s_c;
	TH1D *hadTop_ST_t_c;
	TH1D *hadTop_ST_tW_c;
	
	//hadronic
	if (type==0) {
		hadTop_tTj = (TH1D*) gDirectory->Get("tT0j_HadronicTop_mass_cut2");
		// mc bkg
		//hadTop_tTjO = (TH1D*) gDirectory->Get("tTjetsO_HadronicTop_mass_cut4");
		hadTop_Wjets = (TH1D*) gDirectory->Get("W0j_HadronicTop_mass_cut2");
		hadTop_Zjets = (TH1D*) gDirectory->Get("Z0j_HadronicTop_mass_cut2");
		hadTop_qcd = (TH1D*) gDirectory->Get("qcd_HadronicTop_mass_cut2");
		hadTop_ST_s = (TH1D*) gDirectory->Get("ST_s_HadronicTop_mass_cut2");
		hadTop_ST_t = (TH1D*) gDirectory->Get("ST_t_HadronicTop_mass_cut2");
		hadTop_ST_tW = (TH1D*) gDirectory->Get("ST_tW_HadronicTop_mass_cut2");
		// background shape from bad combinations
		hadTop_tTj_c = (TH1D*) gDirectory->Get("tT0j_HadronicTop_mass_cut3");
		//hadTop_tTjO_c = (TH1D*) gDirectory->Get("tTjetsO_HadronicTop_mass_cut5");
		hadTop_Wjets_c = (TH1D*) gDirectory->Get("W0j_HadronicTop_mass_cut3");
		hadTop_Zjets_c = (TH1D*) gDirectory->Get("Z0j_HadronicTop_mass_cut3");
		hadTop_qcd_c = (TH1D*) gDirectory->Get("qcd_HadronicTop_mass_cut3");
		hadTop_ST_s_c = (TH1D*) gDirectory->Get("ST_s_HadronicTop_mass_cut3");
		hadTop_ST_t_c = (TH1D*) gDirectory->Get("ST_t_HadronicTop_mass_cut3");
		hadTop_ST_tW_c = (TH1D*) gDirectory->Get("ST_tW_HadronicTop_mass_cut3");
	} else {
		//leptonic
		hadTop_tTj = (TH1D*) gDirectory->Get("tTjets_LeptonicTop_mass_cut4");
		// mc bkg
		hadTop_tTjO = (TH1D*) gDirectory->Get("tTjetsO_LeptonicTop_mass_cut4");
		hadTop_Wjets = (TH1D*) gDirectory->Get("Wjets_LeptonicTop_mass_cut4");
		hadTop_ppMu = (TH1D*) gDirectory->Get("ppMu_LeptonicTop_mass_cut4");
		// combinatorics shape
		hadTop_tTj_c = (TH1D*) gDirectory->Get("tTjets_LeptonicTop_mass_cut5");
		hadTop_tTjO_c = (TH1D*) gDirectory->Get("tTjetsO_LeptonicTop_mass_cut5");
		hadTop_Wjets_c = (TH1D*) gDirectory->Get("Wjets_LeptonicTop_mass_cut5");
		hadTop_ppMu_c = (TH1D*) gDirectory->Get("ppMu_LeptonicTop_mass_cut5");
	}
	
	// get all
	TH1D *hadTop_all = hadTop_tTj->Clone("hadTop_all");
	//TH1D *hadTop_all = hadTop_Wjets->Clone("hadTop_all");// fit only Wjets
	hadTop_all->Sumw2();
	//hadTop_all->Add(hadTop_tTjO);
	hadTop_all->Add(hadTop_Wjets);
	hadTop_all->Add(hadTop_Zjets);
	hadTop_all->Add(hadTop_qcd);
	hadTop_all->Add(hadTop_ST_s);
	hadTop_all->Add(hadTop_ST_t);
	hadTop_all->Add(hadTop_ST_tW);

	// only background
	TH1D *hadTop_bkp = hadTop_Wjets->Clone("hadTop_bkp");
	hadTop_bkp->Add(hadTop_Zjets);
	hadTop_bkp->Add(hadTop_qcd);
	hadTop_bkp->Add(hadTop_ST_s);
	hadTop_bkp->Add(hadTop_ST_t);
	hadTop_bkp->Add(hadTop_ST_tW);
	
	// only mc bkg
	//TH1D *hadTop_bkg_mc = hadTop_tTjO->Clone("hadTop_bkg_mc");
	//TH1D *hadTop_bkg_mc = hadTop_Wjets->Clone("hadTop_bkg_mc");
	//hadTop_bkg_mc->Sumw2();
	//hadTop_bkg_mc->Add(hadTop_Wjets);

	// get shape from wrong combinations
	TH1D *hadTop_c = hadTop_tTj_c->Clone("hadTop_c");
	//TH1D *hadTop_c = hadTop_Wjets_c->Clone("hadTop_c");
	hadTop_c->Sumw2();
	//hadTop_c->Add(hadTop_tTjO_c);
	hadTop_c->Add(hadTop_Wjets_c);
	hadTop_c->Add(hadTop_Zjets_c);
	hadTop_c->Add(hadTop_qcd_c);
	hadTop_c->Add(hadTop_ST_s_c);
	hadTop_c->Add(hadTop_ST_t_c);
	hadTop_c->Add(hadTop_ST_tW_c);

	TH1D *hadTop_c_norm = hadTop_c->Clone("hadTop_c_norm");
	
	// scale by lumi
	hadTop_all->Scale(lumi);
	hadTop_c->Scale(lumi);
	
	//smear histos if requeste
	if (smear) {

		TH1D *hadTop_all_old = hadTop_all->Clone();
		TH1D *hadTop_c_old = hadTop_c->Clone();
		
		TRandom *rnds = new TRandom(65539);
		TRandom *rndc = new TRandom(93556);
		
		for (int i=0; i<= hadTop_all->GetNbinsX(); ++i) {
			hadTop_all->SetBinContent( i, rnds->Poisson(hadTop_all->GetBinContent(i)) );
			hadTop_all->SetBinError( i, TMath::Sqrt(hadTop_all->GetBinContent(i)) );

			hadTop_c->SetBinContent( i, rndc->Poisson(hadTop_c->GetBinContent(i)) );
			hadTop_c->SetBinError( i, TMath::Sqrt(hadTop_c->GetBinContent(i)) );

		}

		//hadTop_all->Print("all");
		TCanvas *cv_smear = new TCanvas("cv_smear","cv_smear",700,700);
		hadTop_all_old->Draw("HIST");
		hadTop_all->Draw("same");
		hadTop_c->SetLineColor(2);
		hadTop_c_old->SetLineColor(2);
		hadTop_c->SetMarkerStyle(4);
		hadTop_c->SetMarkerColor(2);
		hadTop_c_old->Draw("HIST same");
		hadTop_c->Draw("same");
	}
	else {

		TCanvas *cv_no = new TCanvas("cv_no","cv_no",700,700);
		hadTop_all->Draw("same");
		hadTop_c->SetLineColor(2);
		hadTop_c->SetMarkerColor(2);
		hadTop_c->Draw("same");
	}

	cout << " total events = " << hadTop_all->Integral() << endl;
	cout << " total events in combinatoric bkg. = " << hadTop_c->Integral() << endl;

	//plot shapes
	TCanvas *c00 = new TCanvas("c00","c00",700,700);
	hadTop_c_norm->Scale(1./hadTop_c_norm->Integral());
	hadTop_bkp->Scale(1./hadTop_bkp->Integral());
	hadTop_c_norm->SetXTitle("M3'(#chi^{2})");
	hadTop_c_norm->SetYTitle("a.u.");
	hadTop_c_norm->Draw();
	hadTop_bkp->Draw("same");
	TLegend *len00 = new TLegend(0.6,0.2,0.8,0.4);
	len00->SetMargin(0.12);
	len00->SetTextSize(0.035);
	len00->SetFillColor(10);
	len00->SetBorderSize(0);
	len00->AddEntry(hadTop_c_norm, "only background, lowest #chi^{2}");
	len00->AddEntry(hadTop_bkp, "t#bar{t} + background, 3rd #chi^{2}");
	len00->Draw();
	
	// --- Observable ---
	
	RooRealVar mass("mass",decay+" top mass",100,500,"GeV/c^{2}") ; 
 
    // --- Build Gaussian signal PDF ---
	RooRealVar meanS("mean","top mass",170,165,220,"GeV/c^{2}") ; 
	RooRealVar widthS("width","top width",15,5,40,"GeV/c^{2}") ;
	RooGaussian gauss("gauss","gaussian p.d.f.",mass,meanS,widthS) ; 

	// --- Build background PDF ---

	// opion non parametric function
	//RooDataHist h_bkg("h_bkg","background shape", mass, hadTop_c);	
	//RooHistPdf background("background","background", mass, h_bkg, 2 );

	/*
	RooRealVar a("a","exp constant",-0.004,-0.005,-0.003);
	////RooExponential expo("expo","exponential PDF",mass,a);
	RooRealVar p0("p0","parameter0",-30,-35,-25);
	RooRealVar p1("p1","parameter1",0.4,0.3,0.5);
	RooRealVar p2("p2","parameter1",-0.0006,-0.0007,-0.0005);
	////RooPolynomial poly("poly","polynominal p.d.f.",mass,RooArgList(p0,p1,p2) );
	RooGenericPdf background("background","(p0 + p1*mass + p2*mass*mass)*exp(-a*mass)",RooArgSet(mass,p0,p1,p2,a) );
	*/
	// Chebychev
	RooRealVar cheby0("cheby0","coefficient #0", 1.0,-1.,1.) ; 
	RooRealVar cheby1("cheby1","coefficient #1", 0.1,-1.,1.) ; 
	RooRealVar cheby2("cheby2","coefficient #2",-0.1,-1.,1.) ;
	RooRealVar cheby3("cheby3","coefficient #3",-0.1,-1.,1.) ;
	//RooChebychev landau("landau","landau", mass,RooArgList(cheby0,cheby1,cheby2,cheby3)) ; 

	// Landau
	RooRealVar meanLandau("meanLandau","mean landau",180,170,210); 
	RooRealVar sigmaLandau("sigmaLandau","sigma landau",40,20,50) ; 
	RooLandau landau0("landau0","landau",mass,meanLandau,sigmaLandau) ; 

	// bkg: gauss + exp
	RooRealVar mGS_B("mean","mass",170,150,220,"GeV/c^{2}");
	RooRealVar wGS_B("widht","width",15,5,40,"GeV/c^{2}");
	RooGaussian gaus_B("gaussB","guassian pdf",mass,mGS_B,wGS_B);
	RooChebychev cheby("cheby","cheby",mass,RooArgList(cheby0,cheby1,cheby2,cheby3)) ;
	
	RooRealVar b1frac("b1frac","fraction 1",0.5);
	RooRealVar b2frac("b2frac","fraction 2",0.4);
	////RooAddPdf landau("landau","landau", RooArgList(gaus_B,cheby),RooArgList(b1frac,b2frac));
	RooAddPdf landau("landau","landau", RooArgList(landau0,gaus_B),RooArgList(b1frac,b2frac));
	
	// --- first fit background ---

	//RooDataSet *databkg = background.generate(mass,1000);
	RooDataHist *databkg = new RooDataHist("databkg","bkg shape",mass,hadTop_c);
		
	//background.fitTo(*databkg);
	RooFitResult *resultsbkg = landau.fitTo(*databkg,Save(true));
	cout << " 2.*nll bkg = " << 2.*(resultsbkg->minNll()) << endl;
	
	RooPlot *bkgframe = mass.frame();
	bkgframe->SetMaximum(42.5);
	databkg->plotOn(bkgframe,DataError(RooAbsData::SumW2));//force RooFit to use sqrt(N) errors. RooFit by default shows the 68% confidence interval for Poisson statistics
	//background.plotOn(bkgframe);
	landau.plotOn(bkgframe,LineStyle(kDashed));
	
	TCanvas *c1 = new TCanvas("c1","c1",700,700);
	landau.paramOn(bkgframe,Label("Fit Results"),Format("NEU",AutoPrecision(2)));
	bkgframe->Draw();
	cout << "chi^2 = " << bkgframe->chiSquare() << endl;
	
	// fix backgroun values
	meanLandau.setConstant(kTRUE);
	sigmaLandau.setConstant(kTRUE);
	cheby0.setConstant(kTRUE);
	cheby1.setConstant(kTRUE);
	cheby2.setConstant(kTRUE);
	cheby3.setConstant(kTRUE);
	b1frac.setConstant(kTRUE);
	b2frac.setConstant(kTRUE);
	mGS_B.setConstant(kTRUE);
	wGS_B.setConstant(kTRUE);
	
    // --- Construct signal+background PDF ---
	Double_t Ntot = hadTop_all->Integral();
	//Double_t Ntot = 1000.;
	//RooRealVar Nsig("Nsig","signal fraction",55.,0.,Ntot) ;
	//RooRealVar Nbkg("Nbkg","background fraction",80.,0.,Ntot) ;
	RooRealVar Nsig("Nsig","signal fraction",0.5*Ntot,0,1000) ;
	RooRealVar Nbkg("Nbkg","background fraction",0.5*Ntot,0,1000) ;
	//RooAddPdf model("model","model",RooArgList(gauss,background),RooArgList(Nsig,Nbkg));
	RooAddPdf model("model","model",RooArgList(gauss,landau),RooArgList(Nsig,Nbkg));
	

	// --- Generate a toyMC sample from composite PDF --- 
	//RooDataSet *data = model.generate(mass,1000); 

	// get data from histogram
	RooDataHist *data = new RooDataHist("data","mass dataset",mass,hadTop_all) ;
	
    // --- Perform extended ML fit of composite PDF to toy data --- 
	//model.fitTo(*data,Extended()) ; 
	// fit by hand
	RooNLLVar nll("nll","nll",model,*data,Extended()) ;
	RooMinuit minuit(nll) ;
	minuit.migrad() ;
	minuit.hesse() ;

	RooFitResult *result = minuit.save();
	cout << "2*minNll = " << 2.*(result->minNll()) << endl;
		
	Double_t val = nll.getVal() ;
	RooArgSet* vars = nll.getVariables();
	cout << "val = " << val << endl;
	vars->Print();
	RooProfileLL pll_mean("pll_mean","pll",nll, RooArgList(meanS)) ;
	//cout << "pll val= " << pll_mean.Eval() << endl;
	TCanvas *cv_nll = new TCanvas("cv_nll","cv_nll",700,700);
	RooPlot* nllframe = meanS.frame() ;
	//pll_mean.plotOn(nllframe) ;
	//nll.plotOn(nllframe);
	nllframe->Draw();
	
    // --- Plot toy data and composite PDF overlaid --- 
	RooPlot* finalframe = mass.frame();
	finalframe->SetMaximum(42.5);
	data->plotOn(finalframe,DataError(RooAbsData::SumW2));
	model.plotOn(finalframe) ;
	cout << "chi^2 = " << finalframe->chiSquare() << endl;
	//model.plotOn(finalframe,Components(background),LineStyle(kDashed)) ;
	model.plotOn(finalframe,Components(landau),LineStyle(kDashed)) ;
	model.paramOn(finalframe,Label("Fit Results"),Format("NEU",AutoPrecision(2)));

	
	TCanvas *c2 = new TCanvas("c2","c2",700,700);
	finalframe->Draw();
	
	
	cout << mass.getVal() << endl;
	cout << "model.getVal()" << model.getVal(RooArgSet(Nsig,Nbkg,meanS,widthS)) << endl;
	//TCanvas *c3 = new TCanvas("c3","c3",700,700);
	//RooPlot* mpframe = meanS.frame();
	//model.plotParamOn(mpframe);
	//mpframe->Draw();

	// TOY MC Study
	RooDataHist hdata_ttbar("hdata_ttbar","ttbar", mass, hadTop_tTj);       
	RooDataHist hdata_bkg("hdata_bkg","wjets", mass, hadTop_bkp);
		
	RooHistPdf hpdf_ttbar("hpdf_ttbar","signal pdf", mass, hdata_ttbar, 0 );
	RooHistPdf hpdf_bkg("hpdf_bkg","W+jets pdf", mass, hdata_bkg, 0 );

	RooRealVar Ntt("Ntt","number of t#bar{t} events", hadTop_tTj->Integral(), -500 , 1000);
    RooRealVar NW("NW","number of W+jets events", hadTop_bkp->Integral(), -500 , 1000);

	RooAddPdf model_histpdf("model_histpdf", "TTjets+Wjets", RooArgList(hpdf_ttbar,hpdf_bkg),
							RooArgList(Nsig, Nbkg) ) ;

	RooMCStudy *mcstudy = new RooMCStudy(model, mass, FitModel(model),Binned(kTRUE),Silence(), Extended() , 
										 //FitOptions(Save(kTRUE),Minos(kTRUE),Extended()) );
										 FitOptions(Save(kTRUE),Minos(kTRUE),Extended()));
	

	return;
	
	TCanvas* cva = new TCanvas("cva","cva",1800,600) ;
	RooPlot *initialframe = mass.frame();
	model_histpdf.plotOn(initialframe,LineColor(kRed));
	initialframe->Draw();
	
	// generate PEs
	int Nsamples = 1000;

	// Fit
	mcstudy->generateAndFit(Nsamples,0,kTRUE);

//	gDirectory->Add(mcstudy) ;	
	// E x p l o r e   r e s u l t s   o f   s t u d y 
	// ------------------------------------------------

	// Make plots of the distributions of mean, the error on mean and the pull of mean
	RooPlot* frame1 = mcstudy->plotParam(Nsig,Bins(40));
	RooPlot* frame2 = mcstudy->plotError(Nsig,Bins(40)) ;
	RooPlot* frame3 = mcstudy->plotPull(Nsig,Bins(40),FitGauss(kTRUE)) ;
	RooPlot* frame1w = mcstudy->plotParam(Nbkg,Bins(40)) ;
	RooPlot* frame2w = mcstudy->plotError(Nbkg,Bins(40)) ;
	RooPlot* frame3w = mcstudy->plotPull(Nbkg,Bins(40),FitGauss(kTRUE)) ;

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

	
}

