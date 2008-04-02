

void massfit() {

	gSystem->Load("libRooFit");
	using namespace RooFit;


	// get histograms
	TFile *ttbarf = TFile::Open("TtEventAnalysis_inclttbar.root");
	TFile *zp1f = TFile::Open("TtEventAnalysis_Zp1TeV.root");
	TFile *zp2f = TFile::Open("TtEventAnalysis_Zp2TeV.root");
	TFile *zp3f = TFile::Open("TtEventAnalysis_Zp3TeV.root");
	TFile *zp4f = TFile::Open("TtEventAnalysis_Zp4TeV.root");

	ttbarf->cd();
	TH1D *mttbar = (TH1D*) gDirectory->Get("Jets/topPair_mass_cut1");
	zp1f->cd();
	TH1D *mzp1 = (TH1D*) gDirectory->Get("Jets/topPair_mass_cut1");
	zp2f->cd();
	TH1D *mzp2 = (TH1D*) gDirectory->Get("Jets/topPair_mass_cut1");
	zp3f->cd();
	TH1D *mzp3 = (TH1D*) gDirectory->Get("Jets/topPair_mass_cut1");
	zp4f->cd();
	TH1D *mzp4 = (TH1D*) gDirectory->Get("Jets/topPair_mass_cut1");
	
	mttbar->Scale((1000.*833*0.15)/19022.);
	mzp1->Scale((1000.*20.*0.15)/7762.);
	mzp2->Scale((1000.*1.*0.15)/7929.);
	mzp3->Scale((1000.*0.7*0.15)/8760.);
	mzp4->Scale((1000.*0.6*0.15)/7812.);

	TH1D *mzp1tot = (TH1D*) mzp1->Clone("mzp1tot");
	TH1D *mzp2tot = (TH1D*) mzp2->Clone("mzp2tot");
	TH1D *mzp3tot = (TH1D*) mzp3->Clone("mzp3tot");
	TH1D *mzp4tot = (TH1D*) mzp4->Clone("mzp4tot");

	mzp1tot->Add(mttbar);
	mzp2tot->Add(mttbar);
	mzp3tot->Add(mttbar);
	mzp4tot->Add(mttbar);
	
	Double_t Ntot = mzp1tot->Integral();
	
	// --- Observable --- 
	RooRealVar mass("mass","top pair mass",300,2000,"GeV/c^{2}") ; 
 
    // --- Build Gaussian signal PDF ---
	RooRealVar sigmean("mean","X mass",900,750,1150,"GeV/c^{2}") ; 
	RooRealVar sigwidth("width","X width",150,50,400,"GeV/c^{2}") ;
	//RooRealVar sigmean("mean","X mass",1900,1750,2150,"GeV/c^{2}") ; 
	//RooRealVar sigwidth("width","X width",250,100,400,"GeV/c^{2}") ;
	//RooRealVar sigmean("mean","X mass",2900,2700,3150,"GeV/c^{2}") ; 
	//RooRealVar sigwidth("width","X width",300,150,1200,"GeV/c^{2}") ;
	//RooRealVar sigmean("mean","X mass",3700,3500,4200,"GeV/c^{2}") ; 
	//RooRealVar sigwidth("width","X width",400,150,1200,"GeV/c^{2}") ;
	
	RooGaussian gauss("gauss","gaussian PDF",mass,sigmean,sigwidth) ; 

	// --- Build Exponential background PDF ---
	RooRealVar a("a","exp constant",-0.004,-0.01,-0.001);
	RooExponential expo("expo","exponential PDF",mass,a);
	
    // --- Construct signal+background PDF --- 
	RooRealVar Nsig("Nsig","signal fraction",50,0.,Ntot) ; 
	RooRealVar Nbkg("Nbkg","background fraction",0.9*Ntot,0.,Ntot) ; 
 	
	RooAddPdf model("model","model",RooArgList(gauss,expo),RooArgList(Nsig,Nbkg));
 
    // --- Generate a toyMC sample from composite PDF --- 
	//RooDataSet *data = model.generate(mass,2000); 

	// get data from histogram
	RooDataHist *data = new RooDataHist("data","mass dataset",mass,mzp1tot) ;
	
    // --- Perform extended ML fit of composite PDF to toy data --- 
	model.fitTo(*data,Extended()) ; 
 
    // --- Plot toy data and composite PDF overlaid --- 
	RooPlot* mesframe = mass.frame();
	data->plotOn(mesframe) ; 
	model.plotOn(mesframe) ; 
	model.plotOn(mesframe,Components(expo),LineStyle(kDashed)) ; 
	model.paramOn(mesframe,Label("Fit Results"),Format("NEU",AutoPrecision(2)));
	
	mesframe->Draw();
}
