
#include "TFile.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"

#include <map>
#include <vector>
#include <iostream>

void plots(TString filename,TString suffix="") {

	gROOT->SetStyle("CMS");

	TFile *infile = new TFile(filename);

	std::map<TString, TCanvas* > cv_map;
	std::map<TString, TH1* > h1;
	std::vector< TString > names;
	names.push_back("kinfit_probchi2_cut0");
	names.push_back("kinfit_probchi2_cut1");
	names.push_back("kinfit_probchi2_cut2");
	names.push_back("topPairRes_cut0");
	names.push_back("topPairRes_cut1");
	names.push_back("topPairRes_cut2");
	names.push_back("fittopPairRes_cut0");
	names.push_back("fittopPairRes_cut1");
	names.push_back("fittopPairRes_cut2");
	names.push_back("EtpRes_cut2");
	names.push_back("fitEtpRes_cut2");

	std::cout << " initialized" << std::endl;
	
	for( size_t iname =0; iname != names.size(); ++iname) {
		
		h1[names[iname]] = (TH1D*) infile->Get("Mass/"+names[iname]);
		h1[names[iname]]->SetLineWidth(2);
		std::cout << " got histogram " << names[iname] << std::endl;
	}
	
	// mass plots
	cv_map["probchi2"] = new TCanvas("probchi2","probchi2",700,700);
	h1["kinfit_probchi2_cut0"]->SetFillColor(7);
	h1["kinfit_probchi2_cut0"]->Draw();
	h1["kinfit_probchi2_cut2"]->SetFillColor(3);
	h1["kinfit_probchi2_cut2"]->Draw("same");
	h1["kinfit_probchi2_cut1"]->SetFillColor(6);
	h1["kinfit_probchi2_cut1"]->Draw("same");
	TLegend *leg = new TLegend(0.34,0.68,0.74,0.84,"","NDC");
	leg->SetFillColor(10);
	leg->AddEntry(h1["kinfit_probchi2_cut0"],"12 combiniations","f");
	leg->AddEntry(h1["kinfit_probchi2_cut2"],"highest prob","f");
	leg->AddEntry(h1["kinfit_probchi2_cut1"],"best MC match","f");
	leg->Draw();
	gPad->SetLogy();
	cv_map["probchi2"]->RedrawAxis();
	cv_map["probchi2"]->Update();

	
	cv_map["topPairRes_allsolutions"] = new TCanvas("topPairRes_allsolutions","topPairRes_allsolutions",700,700);
	h1["topPairRes_cut0"]->Draw();
	h1["fittopPairRes_cut0"]->SetLineColor(2);
	h1["fittopPairRes_cut0"]->Draw("same");

	cv_map["topPairRes_highestProb"] = new TCanvas("topPairRes_highestProb","topPairRes_highestProb",700,700);
	h1["fittopPairRes_cut2"]->SetLineColor(2);
	h1["fittopPairRes_cut2"]->Draw();
	h1["topPairRes_cut2"]->Draw("same");
	
	cv_map["topPairRes_bestMC"] = new TCanvas("topPairRes_bestMC","topPairRes_bestMC",700,700);
	h1["fittopPairRes_cut1"]->SetLineColor(2);
	h1["fittopPairRes_cut1"]->Draw();
	h1["topPairRes_cut1"]->Draw("same");
	
	cv_map["EtpRes_highestProb"] = new TCanvas("EtpRes_highestProb","EtpRes_highestProb",700,700);
	h1["fitEtpRes_cut2"]->SetLineColor(2);
	h1["EtpRes_cut2"]->Draw();
	h1["fitEtpRes_cut2"]->Draw("same");
	

	for( std::map<TString,TCanvas* >::const_iterator imap = cv_map.begin(); imap!= cv_map.end(); ++imap) {

		TCanvas* cvtemp = imap->second;
		cvtemp->Print(imap->first + "_"+suffix+ ".png");

	}
}
