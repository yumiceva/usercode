#define PlotTree_cxx
#include "PlotTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TString.h"
#include "TCanvas.h"
#include "TTree.h"

#include <iostream>
#include <vector>


void PlotTree::Loop() {


	TH1D *hvx[4];
	TH1D *hvy[4];
	TH1D *hvz[4];
	TH1D *hvxvy[4];
	TH1D *hpx[4];
	TH1D *hpy[4];
	TH1D *hpz[4];
	TH1D *hpt[4];
	TH1D *hp[4];
	TH1D *he[4];
	TH1D *hdeltapx = new TH1D("hdeltapx","(p_{X boost} - p_{X}) [GeV/c]",100,-.5,2.);
	TH1D *hdeltapy = new TH1D("hdeltapy","(p_{Y boost} - p_{Y}) [GeV/c]",100,-.5,2.);
	TH1D *hdeltapz = new TH1D("hdeltapz","(p_{Z boost} - p_{Z}) [GeV/c]",100,-.5,2.);
	TH1D *hdeltae = new TH1D("hdeltape","(E_{boost} - E) [GeV/c]",100,-.5,2.);
	TH1D *hdeltap = new TH1D("hdeltap","(p_{boost} - p) [GeV/c]",100,-.5,2.);
	hdeltapx->SetXTitle(hdeltapx->GetTitle());
	hdeltapy->SetXTitle(hdeltapy->GetTitle());
	hdeltapz->SetXTitle(hdeltapz->GetTitle());
	hdeltae->SetXTitle(hdeltae->GetTitle());
	hdeltap->SetXTitle(hdeltap->GetTitle());
	
	
	int color_line[4] = {1,2,3,4};

	for (int ih=0; ih<4; ih++ ) {
		
		hvx[ih] = new TH1D(TString("hvx")+itoa(ih),"v_{X} [mm]",100,-0.1,0.1);
		hvy[ih] = new TH1D(TString("hvy")+itoa(ih),"v_{Y} [mm]",100,-0.1,0.1);
		hvz[ih] = new TH1D(TString("hvz")+itoa(ih),"v_{z} [mm]",100,-500.,500);
		hvx[ih]->SetLineColor(color_line[ih]);
		hvy[ih]->SetLineColor(color_line[ih]);
		hvz[ih]->SetLineColor(color_line[ih]);
			
		hpx[ih] = new TH1D(TString("hpx")+itoa(ih),"p_{X} [GeV/c]",200,-3000,3000);
		hpy[ih] = new TH1D(TString("hpy")+itoa(ih),"p_{Y} [GeV/c]",200,-3000,3000);
		hpz[ih] = new TH1D(TString("hpz")+itoa(ih),"p_{Z} [GeV/c]",200,-3000,3000);
		hpt[ih] = new TH1D(TString("hpt")+itoa(ih),"p_{T} [GeV/c]",200,-3000,3000);
		hp[ih] = new TH1D(TString("hp")+itoa(ih),"p [GeV/c]",200,0,4000);
		he[ih] = new TH1D(TString("he")+itoa(ih),"E [GeV]",200,0,4000);
		hpx[ih]->SetLineColor(color_line[ih]);
		hpy[ih]->SetLineColor(color_line[ih]);
		hpz[ih]->SetLineColor(color_line[ih]);
		hpt[ih]->SetLineColor(color_line[ih]);
		hp[ih]->SetLineColor(color_line[ih]);
		he[ih]->SetLineColor(color_line[ih]);

				
	}

	std::vector< double > pxboost;
	std::vector< double > pxcms;
	std::vector< double > pyboost;
	std::vector< double > pycms;
	std::vector< double > pzboost;
	std::vector< double > pzcms;
	std::vector< double > eboost;
	std::vector< double > ecms;
	std::vector< double > pboost;
	std::vector< double > pcms;
	

	for (int itree=0; itree<4; ++itree) {
		TTree *thetree = vtxtree[itree];
	
		Long64_t nentries = thetree->GetEntriesFast();
		cout << "itree= " << itree << " Total entries = " << thetree->GetEntries() << endl;

		Long64_t nbytes = 0, nb = 0;
		for (Long64_t jentry=0; jentry<nentries;jentry++) {
			Long64_t ientry = LoadTree(thetree,itree,jentry);
			if (ientry < 0) break;
			nb = thetree->GetEntry(jentry);   nbytes += nb;
			
			if ( jentry%100000 == 0 ) cout << " processing entry: " << jentry << endl;
			hvx[itree]->Fill(fvx);
			hvy[itree]->Fill(fvy);
			hvz[itree]->Fill(fvz);
		}
		// p tree
		thetree = ptree[itree];
	
		nentries = thetree->GetEntriesFast();
		cout << "itree= " << itree << " Total entries = " << thetree->GetEntries() << endl;

		nbytes = 0; nb = 0;
		for (Long64_t jentry=0; jentry<nentries;jentry++) {
			Long64_t ientry = LoadTree(thetree,itree,jentry);
			if (ientry < 0) break;
			nb = thetree->GetEntry(jentry);   nbytes += nb;
			
			if ( jentry%100000 == 0 ) cout << " processing entry: " << jentry << endl;
			hpx[itree]->Fill(fpx);
			hpy[itree]->Fill(fpy);
			hpz[itree]->Fill(fpz);
			if (itree == 0 ) {
				pxcms.push_back(fpx);
				pycms.push_back(fpy);
				pzcms.push_back(fpz);
				ecms.push_back(fe);
				pcms.push_back(fp);
			}
			if (itree == 2 ) {
				pxboost.push_back(fpx);
				pyboost.push_back(fpy);
				pzboost.push_back(fpz);
				eboost.push_back(fe);
				pboost.push_back(fp);
				
			}
			
		}

	}

	// fill delta p histo
	int pcmssize = pcms.size();
	int pboostsize = pboost.size();
	cout << " pcms vector size   = " << pcmssize << endl;;
	cout << " pboost vector size = " << pboostsize << endl;
	
	for ( int ip=0; ip != pcmssize; ++ip) {
		hdeltapx->Fill(pxboost[ip]-pxcms[ip]);
		hdeltapy->Fill(pyboost[ip]-pycms[ip]);
		hdeltapz->Fill(pzboost[ip]-pzcms[ip]);
		hdeltae->Fill(eboost[ip]-ecms[ip]);
		hdeltap->Fill(pboost[ip]-pcms[ip]);
		
	}
	

	
	TCanvas *cv[9];
	for (int ic=0; ic<9; ic++ ) {
		cv[ic] = new TCanvas(TString("cv")+itoa(ic),TString("cv")+itoa(ic),700,700);
	}

	cv[0]->cd();
	for (int ih=0; ih<4; ih++) {
		hvx[0]->Draw();
		cout << hvx[ih]->GetEntries() << endl;
		//if (ih==0) hvx[0]->Draw();
		//else hvx[ih]->Draw("same");
	}

	TCanvas *cvdeltapx = new TCanvas("cvdeltapx","cvdeltapx",700,700);
	hdeltapx->Draw();
	TCanvas *cvdeltapy = new TCanvas("cvdeltapy","cvdeltapy",700,700);
	hdeltapy->Draw();
	TCanvas *cvdeltapz = new TCanvas("cvdeltapz","cvdeltapz",700,700);
	hdeltapz->Draw();
	TCanvas *cvdeltae = new TCanvas("cvdeltae","cvdeltae",700,700);
	hdeltae->Draw();
	TCanvas *cvdeltap = new TCanvas("cvdeltap","cvdeltap",700,700);
	hdeltap->Draw();
	
/*
	cv[1]->cd();
	for (int ih=0; ih<4; ih++) {
		if (ih==0) hvy[0]->Draw();
		else hvy[ih]->Draw("same");
	}

	cv[2]->cd();
	for (int ih=0; ih<4; ih++) {
		if (ih==0) hvz[0]->Draw();
		else hvz[ih]->Draw("same");
	}

	cv[3]->cd();
	for (int ih=0; ih<4; ih++) {
		if (ih==0) hpx[0]->Draw();
		else hpx[ih]->Draw("same");
	}

	cv[4]->cd();
	for (int ih=0; ih<4; ih++) {
		if (ih==0) hpy[0]->Draw();
		else hpy[ih]->Draw("same");
	}

	cv[5]->cd();
	for (int ih=0; ih<4; ih++) {
		if (ih==0) hpz[0]->Draw();
		else hpz[ih]->Draw("same");
	}

	cv[6]->cd();
	for (int ih=0; ih<4; ih++) {
		if (ih==0) hpt[0]->Draw();
		else hpt[ih]->Draw("same");
	}

	cv[7]->cd();
	for (int ih=0; ih<4; ih++) {
		if (ih==0) hp[0]->Draw();
		else hp[ih]->Draw("same");
	}

	cv[8]->cd();
	for (int ih=0; ih<4; ih++) {
		if (ih==0) he[0]->Draw();
		else he[ih]->Draw("same");
	}
*/
	
}
