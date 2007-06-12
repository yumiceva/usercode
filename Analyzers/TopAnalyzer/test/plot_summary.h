#ifndef plot_summary_h
#define plot_summary_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLorentzVector.h"

#include <iostream>

#include "../../../Analyzers/TopEvent/interface/TopEvent.h"



class plot_summary {

  public:
	plot_summary(TString filename);
	virtual ~plot_summary();
	
	virtual Int_t  GetEntry(Long64_t entry);
	virtual void     Init(TChain *tree);
	virtual void     Loop();
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Show(Long64_t entry = -1);
	virtual Int_t    Cut(Long64_t entry);
	virtual Bool_t   Notify();
	//virtual std::string itoa(int i);
	//virtual std::string ftoa(float i);
	void ApplyJetCorrections(bool option = true ) {fcorrections = option; };
	void ApplyKinFit( bool option = true ) { fkinfit = option; };
	void SetMinKFChi2( double value) { fminKFchi2 = value; };
	void SetPhiCut( double value) { fphicut = value; };
	void Verbose(bool option) { fverbose = option; };
	void SampleName(TString sample) {
		fsamplename = sample;
	};
	void Print(std::string extension="png",std::string tag="") {
		if ( tag != "" ) tag = "_"+tag;
		
		for(std::map<std::string,TCanvas*>::const_iterator icv=cv_map.begin(); icv!=cv_map.end(); ++icv){

			std::string tmpname = icv->first;
			TCanvas *acv = icv->second;
			acv->Print(TString(tmpname+tag+"."+extension));
		}		
	};
	
	void SaveToFile(TString filename="plots_summary.root") {

		//if (!foutfile) {
			
		foutfile = new TFile(filename,"RECREATE");
		for(std::map<std::string,TH1* >::const_iterator ih=h1.begin(); ih!=h1.end(); ++ih){
			TH1 *htemp = ih->second;
			htemp->Write();
		}
		for(std::map<std::string,TH2* >::const_iterator ih=h2.begin(); ih!=h2.end(); ++ih){
			TH2 *htemp = ih->second;
			htemp->Write();
		}
		
		foutfile->Write();
		foutfile->Close();
	};

	double GetPzNu(TLorentzVector p4Muon, TLorentzVector p4MET  ) {
		// solve for a W using muon and MET
	  fcomplex = false;
		double M_W  = 80.4;
		double M_mu =  0.10566;
		double emu = p4Muon.E();
		double pxmu = p4Muon.Px();
		double pymu = p4Muon.Py();
		double pzmu = p4Muon.Pz();
		double pxnu = p4MET.Px();
		double pynu = p4MET.Py();
		double pznu = 0.;
		
		double a = M_W*M_W - M_mu*M_mu + 2.0*pxmu*pxnu + 2.0*pymu*pynu;
		double A = 4.0*(emu*emu - pzmu*pzmu);
		double B = -4.0*a*pzmu;
		double C = 4.0*emu*emu*(pxnu*pxnu + pynu*pynu) - a*a;

		double tmproot = B*B - 4.0*A*C;
		
		if (tmproot<0) {
			fcomplex= true;
			pznu = - B/(2*A); // take real part of complex roots
			//std::cout<< "complex sol. tmproot=" << tmproot << " pznu=" << pznu << std::endl;
		}
		else {
			double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2.0*A);
			double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2.0*A);
			// two real roots, pick one with pz closest to muon
			if (TMath::Abs(tmpsol2-pzmu) < TMath::Abs(tmpsol1-pzmu)) { pznu = tmpsol2;}
			else pznu = tmpsol1;
		}

		return pznu;
	};

/*
	double Dans(XYZTLorentzVectorD p4Muon, 	XYZTLorentzVectorD p4MET ) {

		double Pxu = p4Muon.Px();
		double Pyu = p4Muon.Py();
		double Pzu = p4Muon.Pz();
		double Pu = p4Muon.P();

		double Pxv = p4MET.Px();
		double Pyv = p4MET.Py();
		double Pzv = 0;
		
		double aux = Pxu/Pu;
		double auy = Pyu/Pu;
		double auz = Pzu/Pu;
		double MW = 80.4;

        double cc1 = (MW*MW)/(2.0*Pu) + aux*Pxv + auy*Pyv;

		double bb = -(2.0 * cc1 * auz)/(1.0 - auz*auz);

		double cc = (Pxv*Pxv + Pyv*Pyv - cc1*cc1)/(1.0-auz*auz);

		double temp = bb*bb - 4.0*cc;

		if(temp < 0) {

			Pzv = -bb/2.0;     // complex roots, take real part

		}

		if(temp > 0){

			Pzv = (-bb + sqrt(temp))/2.0;
			
			double tempW = (-bb - sqrt(temp))/2.0;  // 2 real roots, pick one with Pz closest to muon

			if (fabs(tempW-Pzu) < fabs(Pzv-Pzu)) Pzv = tempW;

		}

		return Pzv;

	};
	*/
	
	TChain            *fChain;
	Int_t             fCurrent;
	TFile            *ffile;
	TFile            *foutfile;
	TString           fsamplename;
	bool              fverbose;
	bool              fcorrections;
	bool              fcomplex;
	bool              fkinfit;
	Double_t          fminKFchi2;
	Double_t          fphicut;

	TopEvent *fevent;
	std::map<TString, TCanvas*> cv_map;
	std::map<TString, TH1*> h1;
	std::map<TString, TH2*> h2;
	
		
};

#endif

#ifdef plot_summary_cxx
plot_summary::plot_summary(TString filename)
{
	//ffile = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
	//ffile = new TFile(filename);
	//TTree *tree = (TTree*)gDirectory->Get("summary");

	fverbose = false;
	fcorrections = true;
	TChain *tree = new TChain("summary");
	tree->Add(filename);
	
	fevent = new TopEvent();
	
	Init(tree);

	fkinfit = false;
	fminKFchi2 = 0.;
	fphicut = 0.;

}

plot_summary::~plot_summary()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

std::string itoa(int i) {
  char temp[20];
  sprintf(temp,"%d",i);
  return((std::string)temp);
}

std::string ftoa(float i, const char *format="0") {
  char temp[20];
  if ( format == "0" ) sprintf(temp,"%.2f",i);
  else sprintf(temp,format,i);
  
  return((std::string)temp);
}

std::string GetStringFlavour(int i) {
	if ( i == 5 ) { return "_b"; }
	else if ( i == 4 ) { return "_c"; }
	else if ( i>0 && i<=3 ) { return "_udsg"; }
	else if ( i == 21 ) { return "_udsg"; }
	else if ( i == 0 ) { return "ambiguous";}
	else { 
		//cout << " not defined flavour " << i << endl;
		return "";
	}
}

void NormalizeHistograms(std::vector< TH1* > hist) {

	for ( std::vector< TH1* >::size_type ihist = 0; ihist != hist.size() ; ++ihist ) {
		hist[ihist]->Scale(1./hist[ihist]->Integral());
	}
	
}

Int_t plot_summary::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t plot_summary::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TChain::Class()) return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void plot_summary::Init(TChain *tree)
{

	if (!tree) return;

	fChain = tree;
	fCurrent = -1;
	
	fChain->SetBranchAddress("top.",&fevent);
	//fChain->SetBranchAddress("summaryKVF.",&fBTagSummary[1]);
	//fChain->SetBranchAddress("summaryTKF.",&fBTagSummary[2]);

	//cout << "Init done" << endl;
}

Bool_t plot_summary::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void plot_summary::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t plot_summary::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef plot_summary_cxx
