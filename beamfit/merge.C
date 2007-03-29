#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"

#include <iostream>

void merge() {

	TChain *chain = new TChain("tnt");

	chain->Add("Tevdphi1.root");
	chain->Add("Tevdphi2.root");
	chain->Add("Tevdphi3.root");
	chain->Add("Tevdphi4.root");
	chain->Add("Tevdphi8.root");
	chain->Add("Tevdphi9.root");

	
	//TFile* newfile = TFile::Open("allTev.root", "RECREATE");
	chain->Merge("allTev.root");

	TH1F *z0   = new TH1F("z0" ,"z0",100,-50,50);
	chain->Draw("z0>>z0");
	z0->SetDirectory(0);
	
	TFile* newfile = TFile::Open("allTev.root", "UPDATE");
	cout << "entries in z0: "<< z0->GetEntries() << endl;
	z0->Write();
	newfile->Write();
	

}
