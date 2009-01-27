
#include "TString.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TCanvas.h"

#include <vector>
#include <iostream>
#include <iomanip>

void points(TString filename) {

	
	TString cmssw;
	// 167
	
	cmssw = "$1.6.7$";
	
	TFile *f = TFile::Open(filename);

	std::vector< TString > taggers;
	taggers.push_back( "gTC2_udsg" );
	taggers.push_back( "gTC3_udsg" );
	taggers.push_back( "gTP_udsg" );
	taggers.push_back( "gJBP_udsg" );
 	taggers.push_back( "gSSV_udsg" );
 	taggers.push_back( "gCSV_udsg" );

	std::vector< TString > discriminators;
	discriminators.push_back( "discTC2_udsg" );
	discriminators.push_back( "discTC3_udsg" );
	discriminators.push_back( "discTP_udsg" );
	discriminators.push_back( "discJBP_udsg" );
 	discriminators.push_back( "discSSV_udsg" );
 	discriminators.push_back( "discCSV_udsg" );

	//TCanvas *cv_TC = new TCanvas("cv_TC","cv_TC",700,700);
	//TCanvas *cv_TP = new TCanvas("cv_TP","cv_TP",700,700);

	std::cout << "Tagger & Point & Discriminator & light mistag & b-efficiency \\\\ \\hline" << std::endl;
	for ( size_t itagger = 0; itagger < taggers.size(); ++itagger ) {

		TString tag = taggers[itagger];
		TGraphErrors *agraph = (TGraphErrors*) gDirectory->Get("Histograms/MCtruth/"+tag);

		//if (taggers == "gTC2_udsg" || taggers =
		
		TGraph *dgraph = (TGraph*) gDirectory->Get("Histograms/MCtruth/"+discriminators[itagger]);
		dgraph->Sort();

		TGraphErrors *g = new TGraphErrors(agraph->GetN(),agraph->GetY(),agraph->GetX(),agraph->GetEY(),agraph->GetEX());
		g->Sort();
		
		//cv[itagger] = new TCanvas("cv","cv",600,600);
		//g->Draw("ACP");
		TString se = " & ";
		
		
		std::cout << tag << se << "Loose" << se << std::setprecision(3) << dgraph->Eval(0.1) << se << "0.1" << se << std::setprecision(2) << g->Eval(0.1) << "\\\\" << std::endl;
		std::cout << tag << se << "Medium" << se << std::setprecision(3) << dgraph->Eval(0.01) << se << "0.01" << se << std::setprecision(2) << g->Eval(0.01) << "\\\\" << std::endl;
		std::cout << tag << se << "Tight" << se << std::setprecision(3) << dgraph->Eval(0.001) << se << "0.001" << se << std::setprecision(2) << g->Eval(0.001) << "\\\\ \\hline" << std::endl;
		
	}

	
}
