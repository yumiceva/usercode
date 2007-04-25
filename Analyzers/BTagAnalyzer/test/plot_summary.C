#define plot_summary_cxx
#include "plot_summary.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLatex.h>
#include "TLegend.h"

#include <iostream>
#include <iomanip>

bool SortByMaximum( TH1* lh, TH1* rh ) {

	return lh->GetMaximum() > rh->GetMaximum();

}

void plot_summary::Loop()
{

/*
 *
  gROOT->SetStyle("CMS");
  gSystem->Load("MySummary_cc.so");
  .L plot_summary.C++

 *
 */
	
	
//   In a ROOT session, you can do:
//      Root > .L mytree.C
//      Root > mytree t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
  
  double w_nevents[5] = { 4.8411e-2,
		      6.8658e-2,
		      54.0482e-2,
		      21.7107e-2,
		      12.5342e-2};

  double xsec[5] = {
	  6.32E-01,
	  1.63E-01,
	  2.16E-02,
	  3.08E-03,
      4.94E-04 };

	std::vector < std::string > pthat_label;
	pthat_label.push_back("_20_30");
	pthat_label.push_back("_30_50");
	pthat_label.push_back("_50_80");
	pthat_label.push_back("_80_120");
	pthat_label.push_back("_120_170");
	pthat_label.push_back("_20_170");
	
	std::map<std::string, TH1*> h1;
	
	std::string hname;
	std::string htitle;

	hname = "njets"; 
	htitle = "number of jets";
	h1[hname] = new TH1D(hname.c_str(),htitle.c_str(),40,0.,40.);
	
	hname = "nmuons"; 
	htitle = "number of muons";
	h1[hname] = new TH1D(hname.c_str(),htitle.c_str(),12,0.,12.);

	hname = "muon_pt"; 
	htitle = "muon p_{T} [GeV/c]";
	h1[hname] = new TH1D(hname.c_str(),htitle.c_str(),60,0.,80.);		

	hname = "jet_pt"; 
	htitle = "jet p_{T} [GeV/c]";
	h1[hname] = new TH1D(hname.c_str(),htitle.c_str(),60,0.,170.);

	//hname = "jet_et"; 
	//htitle = "jet E_{T} [GeV]";
	//h1[hname] = new TH1D(hname.c_str(),htitle.c_str(),60,0.,170.);

	hname = "jet_deltar"; 
	htitle = "#Delta R";
	h1[hname] = new TH1D(hname.c_str(),htitle.c_str(),60,0.,0.55);

	hname = "jet_deltar_b"; 
	htitle = "#Delta R";
	h1[hname] = new TH1D(hname.c_str(),htitle.c_str(),60,0.,0.55);
	h1[hname]->SetLineColor(2);
	hname = "jet_deltar_c"; 
	htitle = "#Delta R";
	h1[hname] = new TH1D(hname.c_str(),htitle.c_str(),60,0.,0.55);
	h1[hname]->SetLineColor(3);
	hname = "jet_deltar_uds"; 
	htitle = "#Delta R";
	h1[hname] = new TH1D(hname.c_str(),htitle.c_str(),60,0.,0.55);
	h1[hname]->SetLineColor(4);
	hname = "jet_deltar_g"; 
	htitle = "#Delta R";
	h1[hname] = new TH1D(hname.c_str(),htitle.c_str(),60,0.,0.55);
	h1[hname]->SetLineColor(6);
	
	
	std::vector < std::string > quark_label;
	quark_label.push_back("");
	quark_label.push_back("_b");
	quark_label.push_back("_c");
	quark_label.push_back("_uds");
	quark_label.push_back("_g");

	std::vector < int > quark_color;
	quark_color.push_back(1);
	quark_color.push_back(2);
	quark_color.push_back(3);
	quark_color.push_back(4);
	quark_color.push_back(6);
	
	std::vector < std::string > cut_label;
	cut_label.push_back("");
	cut_label.push_back("_cut1");
	cut_label.push_back("_cut2");
	cut_label.push_back("_cut3");
	
	hname = "jet_ptrel"; 
	htitle = "p_{Trel} [GeV/c]";

	int quark_size = quark_label.size();
	int cut_size = cut_label.size();
	int pthat_size = pthat_label.size();
	
	for ( int iq=0; iq != quark_size; ++iq ) {
				
		for (int ic=0; ic != cut_size; ++ic) {
		
			for (int ipt=0; ipt != pthat_size; ++ipt) {
			
				std::string thename = hname;
				thename += quark_label[iq];
				thename += cut_label[ic];
				thename += pthat_label[ipt];
				
				h1[thename] = new TH1D(thename.c_str(),htitle.c_str(),60,0.,4.);
				h1[thename]->SetLineColor(quark_color[iq]);
				h1[thename]->SetMarkerColor(quark_color[iq]);
				h1[thename]->SetXTitle(h1[thename]->GetTitle());
				cout << " Histogram: " << thename << " created." << endl;
			
				thename = hname;
				thename += "_other"+quark_label[iq];
				thename += cut_label[ic];
				thename += pthat_label[ipt];
				
				h1[thename] = new TH1D(thename.c_str(),htitle.c_str(),60,0.,4.);
				h1[thename]->SetLineColor(quark_color[iq]);
				h1[thename]->SetMarkerColor(quark_color[iq]);
				h1[thename]->SetXTitle(h1[thename]->GetTitle());
				cout << " Histogram: " << thename << " created." << endl;
				
			}
		}
	}

	hname = "jet_et";
	htitle = "E_{T} [GeV]";

	for ( int iq=0; iq != quark_size; ++iq ) {

		for (int ic=0; ic != cut_size; ++ic) {

			for (int ipt=0; ipt != pthat_size; ++ipt) {
				
				std::string thename = hname;
				thename += quark_label[iq];
				thename += cut_label[ic];
				thename += pthat_label[ipt];
				
				h1[thename] = new TH1D(thename.c_str(),htitle.c_str(),20,20.,170.);
				h1[thename]->Sumw2();
				h1[thename]->SetLineColor(quark_color[iq]);
				h1[thename]->SetMarkerColor(quark_color[iq]);
				h1[thename]->SetXTitle(h1[thename]->GetTitle());
				cout << " Histogram: " << thename << " created." << endl;

				thename = hname;
				thename += "_other"+quark_label[iq];
				thename += cut_label[ic];
				thename += pthat_label[ipt];
				
				h1[thename] = new TH1D(thename.c_str(),htitle.c_str(),20,20.,170.);
				h1[thename]->Sumw2();
				h1[thename]->SetLineColor(quark_color[iq]);
				h1[thename]->SetMarkerColor(quark_color[iq]);
				h1[thename]->SetXTitle(h1[thename]->GetTitle());
				cout << " Histogram: " << thename << " created." << endl;
			}
		}

	}

	hname = "discriminator1";
	htitle = "2nd track IP significance";
	for ( int iq=0; iq != quark_size; iq++ ) {
	  
	  std::string thename = hname;
	  thename += quark_label[iq];
	  h1[thename] = new TH1D(thename.c_str(),htitle.c_str(),80,-10.,40);
	  h1[thename]->SetLineColor(quark_color[iq]);
	  h1[thename]->SetMarkerColor(quark_color[iq]);
	  h1[thename]->SetXTitle(h1[thename]->GetTitle());
	  cout << " Histogram: " << thename << " created." << endl;

	}

	
	
	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();
	cout << " Total entries = " << fChain->GetEntries() << endl;
	int count_multiple_mu = 0;

	TFile *tmpfile;
	
	double TCdiscriminator = 4.0;
	double TCdiscriminatorLoose = 2.0;
	
	std::vector< bool > muonjet_tag;
	std::vector< bool > muonjet_ptrel_tag;
	std::vector< bool > muonjet_btag_tag;
	std::vector< bool > muonjet_both_tag;
	std::vector< bool > otherjet_tag;
	std::vector< bool > otherjet_ptrel_tag;
	std::vector< bool > otherjet_btag_tag;
	std::vector< bool > otherjet_both_tag;

	
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;

		if ( jentry%100000 == 0 ) cout << " processing entry: " << jentry << endl;
		//cout << " processing entry: " << jentry << endl;
		
		// calculate weights for samples
		double weight = 1.;
		tmpfile = fChain->GetFile();
		TString tmpfilename = TString(tmpfile->GetName());
		std::string this_pthat_label = "";
		if ( tmpfilename.Contains("qcd20_30_") ) { 
			weight = w_nevents[0] / xsec[0];
			this_pthat_label = "_20_30";
		}
		else if ( tmpfilename.Contains("qcd30_50_") ) { 
			weight = w_nevents[1] / xsec[1];
			this_pthat_label = "_30_50";
		}
		else if ( tmpfilename.Contains("qcd50_80_") ) { 
			weight = w_nevents[2] / xsec[2];
			this_pthat_label = "_50_80";
		}
		else if ( tmpfilename.Contains("qcd80_120_") ) { 
			weight = w_nevents[3] / xsec[3];
			this_pthat_label = "_80_120";
		}
		else if ( tmpfilename.Contains("qcd120_170_") ) { 
			weight = w_nevents[4] / xsec[4];
			this_pthat_label = "_120_170";
		}
		else { cout << " NO matched name found for dataset " << tmpfilename << endl; }
				
		h1["njets"]->Fill(fBTagSummary[0]->njets);
		h1["nmuons"]->Fill(fBTagSummary[0]->nmuons);

		muonjet_tag.clear();
		muonjet_ptrel_tag.clear();
		muonjet_btag_tag.clear();
		muonjet_both_tag.clear();
		otherjet_tag.clear();
		otherjet_ptrel_tag.clear();
		otherjet_btag_tag.clear();
		otherjet_both_tag.clear();
		
		
		std::vector<double> jet_pt_vec = fBTagSummary[0]->jet_pt;
		int vec_size = jet_pt_vec.size();
		if ( vec_size>1 ) count_multiple_mu++;
		
		int othervec_size = (fBTagSummary[0]->otherjet_pt).size();
		
		double large_d0sig = 0;
		int index_selected_mu = -1;
		
		// select only the muon with the best SIP
		for ( int ijet =0; ijet != vec_size; ++ijet) {
			double tmp_large_d0sig = TMath::Abs(fBTagSummary[0]->muon_d0[ijet] / fBTagSummary[0]->muon_d0sigma[ijet]);
			if ( tmp_large_d0sig >= large_d0sig ) {
				large_d0sig = tmp_large_d0sig;
				index_selected_mu = ijet;
			}
		}
				
		int ijet = index_selected_mu;
		if (ijet==-1) continue;
		double muon_pt = fBTagSummary[0]->muon_pt[ijet];
			//double muon_pt = fBTagSummary[0]->muon_pt_chi2_mc[ijet];
		int muon_pdgid_chi2_mc = fBTagSummary[0]->muon_pdgid_chi2_mc[ijet];
			
		double muon_phi = fBTagSummary[0]->muon_phi[ijet];
		double muon_p = fBTagSummary[0]->muon_p[ijet];
		double muon_chi2 = fBTagSummary[0]->muon_chi2[ijet];
		double muon_ndof = fBTagSummary[0]->muon_ndof[ijet];
		double jet_pt = fBTagSummary[0]->jet_pt[ijet];
		double jet_phi = fBTagSummary[0]->jet_phi[ijet];
		double jet_eta = fBTagSummary[0]->jet_eta[ijet];
		double jet_et = fBTagSummary[0]->jet_et[ijet];
		double genjet_eta = fBTagSummary[0]->genjet_eta[ijet];
		double genjet_et  = fBTagSummary[0]->genjet_et[ijet];
		//jet_et = genjet_et;
		//jet_eta = genjet_eta;
		double jet_p = fBTagSummary[0]->jet_p[ijet];
		double deltar = fBTagSummary[0]->jet_deltar[ijet];
		double ptrel = fBTagSummary[0]->jet_ptrel[ijet];
		int flavour = fBTagSummary[0]->jet_flavour[ijet];

			// change axis for ptrel
			/*
			TVector3 tmpmuon(muon_pt*TMath::Cos(muon_phi),
							 muon_pt*TMath::Sin(muon_phi),
							 TMath::Sqrt(muon_p*muon_p - muon_pt*muon_pt) );
			TVector3 tmpjet(jet_pt*TMath::Cos(jet_phi),
			                jet_pt*TMath::Sin(jet_phi),
							 TMath::Sqrt(jet_p*jet_p - jet_pt*jet_pt) );
			*/
			
		h1["jet_pt"]->Fill(jet_pt, weight);
		h1["muon_pt"]->Fill(muon_pt);
		h1["jet_deltar"]->Fill(deltar,weight);
			
			bool pass = false;
			//cout << " muon_chi2=" << muon_chi2 << " muon_ndof=" << muon_ndof << endl;
			
		if ( jet_et>20 && TMath::Abs(jet_eta)<2.0 &&
			 muon_pt> 4 &&
			 deltar < 0.4 && (muon_chi2/muon_ndof)<10
			) pass = true;
			
		//muonjet_tag.push_back(pass); // n sample
			
		bool othertagged = false;
		for ( int jjet =0; jjet != othervec_size; ++jjet) {

			double ojet_pt = fBTagSummary[0]->otherjet_pt[jjet];
			double ojet_eta = fBTagSummary[0]->otherjet_eta[jjet];
				//cout << "ojet_pt:" << ojet_pt << endl;
				
			// skip main jet
			for ( int jjet =0; jjet != vec_size; ++jjet) {
				if ( jet_pt  == ojet_pt ) continue;
				//cout << " skipping jet" << endl;
			}

			//bool othertagged = false;
			double otherdisc = fBTagSummary[0]->otherbtag_discriminator1[ijet];
			//cout << "otherdisc: " << otherdisc << endl;
			if ( (ojet_pt>20.) && TMath::Abs(jet_eta)<2.0 && (otherdisc > TCdiscriminator) ) othertagged = true;
		}

		//otherjet_tag.push_back(othertagged);
			
		bool ptrelcut = false;

		if ( pass && ptrel>0.8 ) ptrelcut = true;

		//muonjet_ptrel_tag.push_back(ptrelcut);
		//otherjet_ptrel_tag.push_back(othertagged&&ptrelcut);
				
		bool tagged = false;
		double disc = fBTagSummary[0]->btag_discriminator1[ijet];
		if ( pass && disc>4.0 ) tagged = true;

		//	muonjet_btag_tag.push_back(tagged);
		//	otherjet_btag_tag.push_back(othertagged&&tagged);
			
		bool bothtaggers = false;
		if ( pass && ptrelcut && disc > TCdiscriminator) bothtaggers = true;

		//muonjet_both_tag.push_back(bothtaggers);
		//otherjet_both_tag.push_back(othertagged&&bothtaggers);

			//cout << "fill histos" << endl;

		// fill histograms
		// only for jet without ambiguous parton flavour
		if ( flavour == 0 ) continue;
		
		std::string thename1 = "jet_ptrel";
		std::string thename2 = "jet_et";
		std::string suffixname = GetStringFlavour(flavour);
		suffixname += ""; // cut
		suffixname += this_pthat_label;
		if (pass) {
			//cout << " begin filling histo: pass " << (thename1+suffixname) << " " << (thename2+suffixname) << endl;
			h1[thename1+suffixname]->Fill(ptrel);
			h1[thename2+suffixname]->Fill(jet_et);
			suffixname = GetStringFlavour(flavour);
			suffixname += ""; // cut
			suffixname += "_20_170";
			h1[thename1+suffixname]->Fill(ptrel,weight);
			h1[thename2+suffixname]->Fill(jet_et,weight);
		}
		suffixname = GetStringFlavour(flavour);
		suffixname += "_cut1"; // cut
		suffixname += this_pthat_label;
		if (ptrelcut) {
			//cout << " begin filling histo: cut1" << endl;
			h1[thename1+suffixname]->Fill(ptrel);
			h1[thename2+suffixname]->Fill(jet_et);
			suffixname = GetStringFlavour(flavour);
			suffixname += "_cut1"; // cut
			suffixname += "_20_170";
			h1[thename1+suffixname]->Fill(ptrel,weight);
			h1[thename2+suffixname]->Fill(jet_et,weight);
			
		}
		suffixname = GetStringFlavour(flavour);
		suffixname += "_cut2"; // cut
		suffixname += this_pthat_label;
		if (tagged) {
			h1[thename1+suffixname]->Fill(ptrel);
			h1[thename2+suffixname]->Fill(jet_et);
			suffixname = GetStringFlavour(flavour);
			suffixname += "_cut2"; // cut
			suffixname += "_20_170";
			h1[thename1+suffixname]->Fill(ptrel,weight);
			h1[thename2+suffixname]->Fill(jet_et,weight);
			
		}
		suffixname = GetStringFlavour(flavour);
		suffixname += "_cut3"; // cut
		suffixname += this_pthat_label;
		if (bothtaggers) {
			h1[thename1+suffixname]->Fill(ptrel);
			h1[thename2+suffixname]->Fill(jet_et);
			suffixname = GetStringFlavour(flavour);
			suffixname += "_cut3"; // cut
			suffixname += "_20_170";
			h1[thename1+suffixname]->Fill(ptrel,weight);
			h1[thename2+suffixname]->Fill(jet_et,weight);
			
		}
		thename1 = "jet_ptrel_other";
		thename2 = "jet_et_other";
		suffixname = GetStringFlavour(flavour);
		suffixname += ""; // cut
		suffixname += this_pthat_label;
		if (pass && othertagged) {
			h1[thename1+suffixname]->Fill(ptrel);
			h1[thename2+suffixname]->Fill(jet_et);
			suffixname = GetStringFlavour(flavour);
			suffixname += ""; // cut
			suffixname += "_20_170";
			h1[thename1+suffixname]->Fill(ptrel,weight);
			h1[thename2+suffixname]->Fill(jet_et,weight);
		}
		suffixname = GetStringFlavour(flavour);
		suffixname += "_cut1"; // cut
		suffixname += this_pthat_label;
		if (ptrelcut && othertagged) {
			h1[thename1+suffixname]->Fill(ptrel);
			h1[thename2+suffixname]->Fill(jet_et);
			suffixname = GetStringFlavour(flavour);
			suffixname += "_cut1"; // cut
			suffixname += "_20_170";
			h1[thename1+suffixname]->Fill(ptrel,weight);
			h1[thename2+suffixname]->Fill(jet_et,weight);
			
		}
		suffixname = GetStringFlavour(flavour);
		suffixname += "_cut2"; // cut
		suffixname += this_pthat_label;
		if (tagged && othertagged) {
			h1[thename1+suffixname]->Fill(ptrel);
			h1[thename2+suffixname]->Fill(jet_et);
			suffixname = GetStringFlavour(flavour);
			suffixname += "_cut2"; // cut
			suffixname += "_20_170";
			h1[thename1+suffixname]->Fill(ptrel,weight);
			h1[thename2+suffixname]->Fill(jet_et,weight);
		}
		suffixname = GetStringFlavour(flavour);
		suffixname += "_cut3"; // cut
		suffixname += this_pthat_label;
		if (bothtaggers && othertagged) {
			h1[thename1+suffixname]->Fill(ptrel);
			h1[thename2+suffixname]->Fill(jet_et);
			suffixname = GetStringFlavour(flavour);
			suffixname += "_cut3"; // cut
			suffixname += "_20_170";
			h1[thename1+suffixname]->Fill(ptrel,weight);
			h1[thename2+suffixname]->Fill(jet_et,weight);
		}
		
//
	/*	
		if (pass) {
		  h1["jet_ptrel"]->Fill(ptrel,weight);
		  h1["discriminator1"]->Fill(disc);
		  h1["jet_et"]->Fill(jet_et);
		}
		if (ptrelcut) {
		  h1["jet_ptrel_cut1"]->Fill(ptrel);
		  h1["jet_et_cut1"]->Fill(jet_et);
		}
		if (tagged) {
		  h1["jet_ptrel_cut2"]->Fill(ptrel);
		  h1["jet_et_cut2"]->Fill(jet_et);
		}
		if (bothtaggers) {
		  h1["jet_ptrel_cut3"]->Fill(ptrel);
		  h1["jet_et_cut3"]->Fill(jet_et);
		}

		if (othertagged) {
			if (pass) h1["jet_ptrel_other"]->Fill(ptrel);
			if (ptrelcut) h1["jet_ptrel_other_cut1"]->Fill(ptrel);
			if (tagged) h1["jet_ptrel_other_cut2"]->Fill(ptrel);
			if (bothtaggers) h1["jet_ptrel_other_cut3"]->Fill(ptrel);
		}
			
			if (flavour==5) {
				h1["jet_deltar_b"]->Fill(deltar,weight);

				if (pass) {
				  h1["jet_ptrel_b"]->Fill(ptrel,weight);
				  h1["discriminator1_b"]->Fill(disc);
				  h1["jet_et_b"]->Fill(jet_et);
				}
				if (ptrelcut) {
				  h1["jet_ptrel_b_cut1"]->Fill(ptrel);
				  h1["jet_et_b_cut1"]->Fill(jet_et);
				}
				if (tagged) {
				  h1["jet_ptrel_b_cut2"]->Fill(ptrel);
				  h1["jet_et_b_cut2"]->Fill(jet_et);
				}
				if (bothtaggers) {
				  h1["jet_ptrel_b_cut3"]->Fill(ptrel);
				  h1["jet_et_b_cut3"]->Fill(jet_et);
				}
				if (othertagged) {
					if (pass) h1["jet_ptrel_other_b"]->Fill(ptrel);
					if (ptrelcut) h1["jet_ptrel_other_b_cut1"]->Fill(ptrel);
					if (tagged) h1["jet_ptrel_other_b_cut2"]->Fill(ptrel);
					if (bothtaggers) h1["jet_ptrel_other_b_cut3"]->Fill(ptrel);
				}
			}
			if (flavour==4) {
				h1["jet_deltar_c"]->Fill(deltar,weight);
				if (pass) {
				  h1["jet_ptrel_c"]->Fill(ptrel,weight);
				  h1["discriminator1_c"]->Fill(disc);
				  h1["jet_et_c"]->Fill(jet_et);
                                }
				if (ptrelcut) {
				  h1["jet_ptrel_c_cut1"]->Fill(ptrel);
				  h1["jet_et_c_cut1"]->Fill(jet_et);
				}
				if (tagged) {
				  h1["jet_ptrel_c_cut2"]->Fill(ptrel);
				  h1["jet_et_c_cut2"]->Fill(jet_et);
				}
				if (bothtaggers) {
				  h1["jet_ptrel_c_cut3"]->Fill(ptrel);
				  h1["jet_et_c_cut3"]->Fill(jet_et);
				}
				if (othertagged) {
					if (pass) h1["jet_ptrel_other_c"]->Fill(ptrel);
					if (ptrelcut) h1["jet_ptrel_other_c_cut1"]->Fill(ptrel);
					if (tagged) h1["jet_ptrel_other_c_cut2"]->Fill(ptrel);
					if (bothtaggers) h1["jet_ptrel_other_c_cut3"]->Fill(ptrel);
				}
			}
			if (flavour<=3) {
				h1["jet_deltar_uds"]->Fill(deltar,weight);
				if (pass) {
				  h1["jet_ptrel_uds"]->Fill(ptrel,weight);
				  h1["discriminator1_uds"]->Fill(disc);
				  h1["jet_et_uds"]->Fill(jet_et);
                                }
				if (ptrelcut) {
				  h1["jet_ptrel_uds_cut1"]->Fill(ptrel);
				  h1["jet_et_uds_cut1"]->Fill(jet_et);
				}
				if (tagged) {
				  h1["jet_ptrel_uds_cut2"]->Fill(ptrel);
				  h1["jet_et_uds_cut2"]->Fill(jet_et);
				}
				if (bothtaggers) {
				  h1["jet_ptrel_uds_cut3"]->Fill(ptrel);
				  h1["jet_et_uds_cut3"]->Fill(jet_et);
				}
				if (othertagged) {
					if (pass) h1["jet_ptrel_other_uds"]->Fill(ptrel);
					if (ptrelcut) h1["jet_ptrel_other_uds_cut1"]->Fill(ptrel);
					if (tagged) h1["jet_ptrel_other_uds_cut2"]->Fill(ptrel);
					if (bothtaggers) h1["jet_ptrel_other_uds_cut3"]->Fill(ptrel);
				}
			}
			if (flavour==21) {
				h1["jet_deltar_g"]->Fill(deltar,weight);
				if (pass) {
				  h1["jet_ptrel_g"]->Fill(ptrel,weight);
				  h1["discriminator1_g"]->Fill(disc);
				  h1["jet_et_g"]->Fill(jet_et);
                                }
				if (ptrelcut) {
				  h1["jet_ptrel_g_cut1"]->Fill(ptrel);
				  h1["jet_et_g_cut1"]->Fill(jet_et);
				}
				if (tagged) {
				  h1["jet_ptrel_g_cut2"]->Fill(ptrel);
				  h1["jet_et_g_cut2"]->Fill(jet_et);
				}
				if (bothtaggers) {
				  h1["jet_ptrel_g_cut3"]->Fill(ptrel);
				  h1["jet_et_g_cut3"]->Fill(jet_et);
				}
				if (othertagged) {
					if (pass) h1["jet_ptrel_other_g"]->Fill(ptrel);
					if (ptrelcut) h1["jet_ptrel_other_g_cut1"]->Fill(ptrel);
					if (tagged) h1["jet_ptrel_other_g_cut2"]->Fill(ptrel);
					if (bothtaggers) h1["jet_ptrel_other_g_cut3"]->Fill(ptrel);
				}
			}		
*/
			//cout << " filling done."<<endl;
	

		
				
	} // end loop
	
	cout << " Events with multiples muons: " << count_multiple_mu << endl;
	
	std::map<std::string, TCanvas*> cv_map;
	std::vector< TH1* > tmp_hist_vec;
	
	for ( int iq=1; iq != quark_size; ++iq ) { // ignore first entry with label ""
	
		string thenamecv = "ptrel_in_pthatbins";
		thenamecv += quark_label[iq];
		cv_map[thenamecv] = new TCanvas(thenamecv.c_str(),thenamecv.c_str(),700,700);

		std::string tmpname = "";
		tmp_hist_vec.clear();
		//tmp_hist_vec.push_back(h1["jet_ptrel"+quark_label[iq]+"_20_30"]);
		//tmp_hist_vec.push_back(h1["jet_ptrel"+quark_label[iq]+"_30_50"]);
		tmpname = "jet_ptrel"+quark_label[iq]+"_50_80";
		h1[tmpname]->SetLineColor(1);
		tmp_hist_vec.push_back(h1[tmpname]);
		tmpname = "jet_ptrel"+quark_label[iq]+"_80_120";
		h1[tmpname]->SetLineColor(2);
		tmp_hist_vec.push_back(h1[tmpname]);
		std::sort(tmp_hist_vec.begin(), tmp_hist_vec.end(), SortByMaximum);
		NormalizeHistograms(tmp_hist_vec);
		
		for ( std::vector< TH1* >::size_type ihist = 0; ihist != tmp_hist_vec.size() ; ++ihist ) {
			if ( ihist==0 ) tmp_hist_vec[ihist]->Draw();
			else tmp_hist_vec[ihist]->Draw("same");
		}
		
	}

	string thenamecv = "ptrel_allpthat";
	cv_map[thenamecv] = new TCanvas(thenamecv.c_str(),thenamecv.c_str(), 700, 700);

	tmp_hist_vec.clear();
	tmp_hist_vec.push_back(h1["jet_ptrel_b_20_170"]);
	tmp_hist_vec.push_back(h1["jet_ptrel_c_20_170"]);
	tmp_hist_vec.push_back(h1["jet_ptrel_uds_20_170"]);
	tmp_hist_vec.push_back(h1["jet_ptrel_g_20_170"]);


	



	
	
	
	/*
	
	// draw objects
	std::map<std::string, TCanvas*> cv;
	
	for(std::map<std::string,TH1*>::const_iterator imap=h1.begin(); imap!=h1.end(); ++imap){

		hname = imap->first;
		if (TString(hname).Contains("jet_ptrel") ) continue;

		cv[hname] = new TCanvas(hname.c_str(),hname.c_str(),700,700);
		
		TH1 *hist = imap->second;
		hist->SetXTitle(hist->GetTitle());
		hist->Draw();
		TString thename(hname);

		cv[hname]->Update();
		cv[hname]->Print("figures/"+TString(hname)+".png");

				
		//TString printname(ffile->GetName());
		//printname = "figures/" + printname.Remove(printname.Length()-5)+"/"+TString(hname)+".png";
		//if (fprintcanvas) cv[hname]->Print(printname);
	}

			
	TCanvas *cvdeltar = new TCanvas("alldeltar","alldeltar",700,700);
	h1["jet_deltar"]->Draw();
	h1["jet_deltar_b"]->Draw("same");
	h1["jet_deltar_c"]->Draw("same");
	h1["jet_deltar_uds"]->Draw("same");
	h1["jet_deltar_g"]->Draw("same");
	
	TLegend *leg = new TLegend(0.5,0.5,0.8,0.7);
	leg->SetBorderSize(0);
	leg->SetFillColor(10);
	//leg->SetTextSize(0.03);
	leg->AddEntry(h1["jet_deltar"],"total","l");
	leg->AddEntry(h1["jet_deltar_b"],"b-jet","l");
	leg->AddEntry(h1["jet_deltar_c"],"c-jet","l");
	leg->AddEntry(h1["jet_deltar_uds"],"uds-jet","l");
	leg->AddEntry(h1["jet_deltar_g"],"g-jet","l");
	leg->Draw();

	cvdeltar->Update();
	cvdeltar->RedrawAxis();
	cvdeltar->Print("figures/"+TString(cvdeltar->GetName())+".png");
	
	std::vector< TH1* > hist_sample;

	TCanvas *cvdisc1 = new TCanvas("alldisc1","alldisc1",700,700);
        h1["discriminator1_b"]->Scale(1./(h1["discriminator1_b"]->Integral()) );
	hist_sample.push_back(h1["discriminator1_b"]);
	h1["discriminator1_c"]->Scale(1./(h1["discriminator1_c"]->Integral()) );
        hist_sample.push_back(h1["discriminator1_c"]);
	h1["discriminator1_uds"]->Scale(1./(h1["discriminator1_uds"]->Integral()) );
        hist_sample.push_back(h1["discriminator1_uds"]);
	h1["discriminator1_g"]->Scale(1./(h1["discriminator1_g"]->Integral()) );
        hist_sample.push_back(h1["discriminator1_g"]);
	std::sort(hist_sample.begin(),hist_sample.end(),SortByMaximum);
	int tmpsize = hist_sample.size();
	for (int is = 0; is != tmpsize; ++is) {
	  if (is==0) hist_sample[is]->Draw();
	  else hist_sample[is]->Draw("same");
	}
	cvdisc1->SetLogy();
	TLegend *leg1 = new TLegend(0.65,0.65,0.9,0.75);
        leg1->SetBorderSize(0);
        leg1->SetFillColor(10);
        //leg->SetTextSize(0.03);
	leg1->AddEntry(h1["jet_deltar_b"],"b-jet","l");
        leg1->AddEntry(h1["jet_deltar_c"],"c-jet","l");
        leg1->AddEntry(h1["jet_deltar_uds"],"uds-jet","l");
        leg1->AddEntry(h1["jet_deltar_g"],"g-jet","l");
        leg1->Draw();

	cvdisc1->Update();
	cvdisc1->RedrawAxis();
	cvdisc1->Print("figures/"+TString(cvdisc1->GetName())+".png");

	////
				
	std::vector< double > events_sample;
	std::map<std::string, std::vector< double > > eventsmap;
	std::map<std::string, std::vector< TH1* > > histmap;

	std::vector< std::string > list_of_samples;
	list_of_samples.push_back("jet_ptrel");
	list_of_samples.push_back("jet_ptrel_other");
	
	int lsize = list_of_samples.size();
	for (int il=0; il != lsize; ++il ) {
		
		for ( int ic=0; ic != cut_size; ++ic ) {
			events_sample.clear();
			hist_sample.clear();

			hname = list_of_samples[il];
			string mapname = hname + cut_label[ic];
		
			for ( int iq=0; iq != quark_size; ++iq ) {
			
				std::string thename = hname;

				thename += quark_label[iq] + cut_label[ic];

				TH1 *hist = h1[thename];
				double events = hist->Integral();
				events_sample.push_back(events);
				hist_sample.push_back(hist);
				
			}
			eventsmap[mapname] = events_sample;
			std::sort(hist_sample.begin(),hist_sample.end(), SortByMaximum);
			histmap[mapname] = hist_sample;
		}
	}

	TLegend *leg2 = new TLegend(0.5,0.5,0.8,0.7);
	leg2->SetBorderSize(0);
	leg2->SetFillColor(10);
	leg2->AddEntry(h1["jet_deltar_b"],"b-jet","l");
	leg2->AddEntry(h1["jet_deltar_c"],"c-jet","l");
	leg2->AddEntry(h1["jet_deltar_uds"],"uds-jet","l");
	leg2->AddEntry(h1["jet_deltar_g"],"g-jet","l");
	
	for(std::map<std::string,std::vector< TH1* > >::const_iterator imap=histmap.begin(); imap!=histmap.end(); ++imap){

		hname = imap->first;

		if ( hname == "jet_ptrel" ) {

			hist_sample.clear();
			hist_sample = imap->second;

			TCanvas *cvptrel = new TCanvas("allptrel","allptrel",700,700);
			int tmpsize = hist_sample.size();
			for (int isample = 0; isample != tmpsize; ++isample ) {
				if (isample == 0 ) hist_sample[isample]->Draw();
				else hist_sample[isample]->Draw("same");
			}
			leg->Draw();
			cvptrel->Update();
			cvptrel->RedrawAxis();
			cvptrel->Print("figures/"+TString(cvptrel->GetName())+".png");
			// normalized
			TCanvas *cvptrelnorm = new TCanvas("allptrelnorm","allptrelnorm",700,700);
			for(int isample = 0; isample != tmpsize; ++isample ) {
				TH1 *tmphist = hist_sample[isample];
				tmphist->Scale(1./(tmphist->Integral()));
				hist_sample[isample] = tmphist;
			}
			std::sort(hist_sample.begin(),hist_sample.end(), SortByMaximum);
			for(int isample = 0; isample != tmpsize; ++isample ) {
				if ( TString(hist_sample[isample]->GetName()) == TString("jet_ptrel") ) continue;
				if (isample == 0 ) hist_sample[isample]->Draw();
				else hist_sample[isample]->Draw("same");
			}
			leg2->Draw();
			cvptrelnorm->Update();
			cvptrelnorm->RedrawAxis();
			cvptrelnorm->Print("figures/"+TString(cvptrelnorm->GetName())+".png");
		}
		
		if ( TString(hname).Contains("cut1") && (!TString(hname).Contains("other")) ) {

		  hist_sample.clear();
		  hist_sample = imap->second;

		  TCanvas *cvptrel_cut1 = new TCanvas("allptrel_cut1","allptrel_cut1",700,700);
		  int tmpsize = hist_sample.size();
		  for (int isample = 0; isample != tmpsize; ++isample ) {
		    if (isample == 0 ) hist_sample[isample]->Draw();
		    else hist_sample[isample]->Draw("same");
		  }
		  leg->Draw();
		  cvptrel_cut1->Update();
		  cvptrel_cut1->RedrawAxis();
		  cvptrel_cut1->Print("figures/"+TString(cvptrel_cut1->GetName())+".png");
		}

		if ( TString(hname).Contains("cut2") && (!TString(hname).Contains("other")) ) {

                  hist_sample.clear();
                  hist_sample = imap->second;

                  TCanvas *cvptrel_cut2 = new TCanvas("allptrel_cut2","allptrel_cut2",700,700);
                  int tmpsize = hist_sample.size();
                  for (int isample = 0; isample != tmpsize; ++isample ) {
                    if (isample == 0 ) hist_sample[isample]->Draw();
                    else hist_sample[isample]->Draw("same");
                  }
                  leg->Draw();
                  cvptrel_cut2->Update();
                  cvptrel_cut2->RedrawAxis();
                  cvptrel_cut2->Print("figures/"+TString(cvptrel_cut2->GetName())+".png");
                }

		if ( TString(hname).Contains("cut3") && (!TString(hname).Contains("other")) ) {

                  hist_sample.clear();
                  hist_sample = imap->second;

                  TCanvas *cvptrel_cut3 = new TCanvas("allptrel_cut3","allptrel_cut3",700,700);
                  int tmpsize = hist_sample.size();
                  for (int isample = 0; isample != tmpsize; ++isample ) {
                    if (isample == 0 ) hist_sample[isample]->Draw();
                    else hist_sample[isample]->Draw("same");
                  }
                  leg->Draw();
                  cvptrel_cut3->Update();
                  cvptrel_cut3->RedrawAxis();
                  cvptrel_cut3->Print("figures/"+TString(cvptrel_cut3->GetName())+".png");
                }


		if ( hname == "jet_ptrel_other") {
		  hist_sample.clear();
		  hist_sample = imap->second;

		  TCanvas *cvptrelother = new TCanvas("allptrelother","allptrelother",700,700);
		  int tmpsize = hist_sample.size();
		  for (int isample = 0; isample != tmpsize; ++isample ) {
		    if (isample == 0 ) hist_sample[isample]->Draw();
		    else hist_sample[isample]->Draw("same");
		  }
		  leg->Draw();
		  cvptrelother->Update();
		  cvptrelother->Print("figures/"+TString(cvptrelother->GetName())+".png");
		  // normalized
		  TCanvas *cvptrelothernorm = new TCanvas("allptrelothernorm","allptrelothernorm",700,700);
		  for(int isample = 0; isample != tmpsize; ++isample ) {
		    TH1 *tmphist = hist_sample[isample];
		    tmphist->Scale(1./(tmphist->Integral()));
		    hist_sample[isample] = tmphist;
		  }
		  std::sort(hist_sample.begin(),hist_sample.end(), SortByMaximum);
		  for(int isample = 0; isample != tmpsize; ++isample ) {
		    if ( TString(hist_sample[isample]->GetName()) == TString("jet_ptrel_other") ) continue;
		    if (isample == 0 ) hist_sample[isample]->Draw();
		    else hist_sample[isample]->Draw("same");
		  }
		  leg2->Draw();
		  cvptrelothernorm->Update();
		  cvptrelothernorm->Print("figures/"+TString(cvptrelothernorm->GetName())+".png");
		}

		if ( TString(hname).Contains("cut1") && (TString(hname).Contains("other")) ) {

                  hist_sample.clear();
                  hist_sample = imap->second;

                  TCanvas *cvptrelother_cut1 = new TCanvas("allptrelother_cut1","allptrelother_cut1",700,700);
                  int tmpsize = hist_sample.size();
                  for (int isample = 0; isample != tmpsize; ++isample ) {
                    if (isample == 0 ) hist_sample[isample]->Draw();
                    else hist_sample[isample]->Draw("same");
                  }
                  leg->Draw();
                  cvptrelother_cut1->Update();
                  cvptrelother_cut1->RedrawAxis();
                  cvptrelother_cut1->Print("figures/"+TString(cvptrelother_cut1->GetName())+".png");
                }

		if ( TString(hname).Contains("cut2") && (TString(hname).Contains("other")) ) {

                  hist_sample.clear();
                  hist_sample = imap->second;

                  TCanvas *cvptrelother_cut2 = new TCanvas("allptrelother_cut2","allptrelother_cut2",700,700);
                  int tmpsize = hist_sample.size();
                  for (int isample = 0; isample != tmpsize; ++isample ) {
                    if (isample == 0 ) hist_sample[isample]->Draw();
                    else hist_sample[isample]->Draw("same");
                  }
                  leg->Draw();
                  cvptrelother_cut2->Update();
                  cvptrelother_cut2->RedrawAxis();
                  cvptrelother_cut2->Print("figures/"+TString(cvptrelother_cut2->GetName())+".png");
                }

		if ( TString(hname).Contains("cut3") && (TString(hname).Contains("other")) ) {

                  hist_sample.clear();
                  hist_sample = imap->second;

                  TCanvas *cvptrelother_cut3 = new TCanvas("allptrelother_cut3","allptrelother_cut3",700,700);
                  int tmpsize = hist_sample.size();
                  for (int isample = 0; isample != tmpsize; ++isample ) {
                    if (isample == 0 ) hist_sample[isample]->Draw();
                    else hist_sample[isample]->Draw("same");
                  }
                  leg->Draw();
                  cvptrelother_cut3->Update();
                  cvptrelother_cut3->RedrawAxis();
                  cvptrelother_cut3->Print("figures/"+TString(cvptrelother_cut3->GetName())+".png");
                }


	}
	
	TCanvas *cvjetet_b = new TCanvas("jetet_b","jetet_b",700,700);
	cvjetet_b->Divide(2,2);
	cvjetet_b->cd(1);
	TH1D *h1d_b_cut1 = (TH1D*) h1["jet_et"]->Clone("h1d_b_cut1");
	h1d_b_cut1->Divide(h1["jet_et_b_cut1"],h1["jet_et_b"],1.,1.,"B");
	h1d_b_cut1->SetYTitle("#epsilon^{pTrel}");
	h1d_b_cut1->SetMarkerColor(1);
	h1d_b_cut1->SetMarkerStyle(8);
	h1d_b_cut1->SetMarkerSize(1.5);
	h1d_b_cut1->Draw("PE1");
	gPad->Update();

	cvjetet_b->cd(2);
        TH1D *h1d_b_cut2 = (TH1D*) h1["jet_et"]->Clone("h1d_b_cut2");
        h1d_b_cut2->Divide(h1["jet_et_b_cut2"],h1["jet_et_b"],1.,1.,"B");
	h1d_b_cut2->SetYTitle("#epsilon^{Tagger}");
	h1d_b_cut2->SetMarkerColor(1);
        h1d_b_cut2->SetMarkerStyle(8);
	h1d_b_cut2->SetMarkerSize(1.5);
        h1d_b_cut2->Draw("PE1");

	cvjetet_b->cd(3);
        TH1D *h1d_b_cut3 = (TH1D*) h1["jet_et"]->Clone("h1d_b_cut3");
        h1d_b_cut3->Divide(h1["jet_et_b_cut3"],h1["jet_et_b"],1.,1.,"B");
	h1d_b_cut3->SetYTitle("#epsilon^{pTrel&Tagger}");
	h1d_b_cut3->SetMarkerColor(1);
        h1d_b_cut3->SetMarkerStyle(8);
	h1d_b_cut3->SetMarkerSize(1.5);
        h1d_b_cut3->Draw("PE1");

        cvjetet_b->cd(4);
        TH1D *h1d_b_ratio = (TH1D*) h1["jet_et"]->Clone("h1d_b_ratio");
        h1d_b_ratio->Divide(h1d_b_cut3,h1d_b_cut1);
	h1d_b_ratio->Divide(h1d_b_cut2);
	h1d_b_ratio->SetYTitle("#epsilon^{pTrel&Tagger}/(#epsilon^{pTrel}#epsilon^{Tagger})");
	h1d_b_ratio->SetMarkerColor(1);
        h1d_b_ratio->SetMarkerStyle(8);
	h1d_b_ratio->SetMarkerSize(1.5);
        h1d_b_ratio->Draw("PE1");
	h1d_b_ratio->Fit("pol1","0");
	TF1 *f1_b = h1d_b_ratio->GetFunction("pol1");
	f1_b->SetLineColor(3);
	f1_b->Draw("same");

	cvjetet_b->Update();
	cvjetet_b->Print("figures/"+TString(cvjetet_b->GetName())+".pdf");
	

	TCanvas *cvjetet_c = new TCanvas("jetet_c","jetet_c",700,700);
        cvjetet_c->Divide(2,2);
        cvjetet_c->cd(1);
        TH1D *h1d_c_cut1 = (TH1D*) h1["jet_et"]->Clone("h1d_c_cut1");
        h1d_c_cut1->Divide(h1["jet_et_c_cut1"],h1["jet_et_c"],1.,1.,"B");
        h1d_c_cut1->SetYTitle("#epsilon^{pTrel}");
        h1d_c_cut1->SetMarkerColor(1);
        h1d_c_cut1->SetMarkerStyle(8);
        h1d_c_cut1->SetMarkerSize(1.5);
        h1d_c_cut1->Draw("PE1");
        gPad->Update();

        cvjetet_c->cd(2);
        TH1D *h1d_c_cut2 = (TH1D*) h1["jet_et"]->Clone("h1d_c_cut2");
        h1d_c_cut2->Divide(h1["jet_et_c_cut2"],h1["jet_et_c"],1.,1.,"B");
        h1d_c_cut2->SetYTitle("#epsilon^{Tagger}");
        h1d_c_cut2->SetMarkerColor(1);
        h1d_c_cut2->SetMarkerStyle(8);
        h1d_c_cut2->SetMarkerSize(1.5);
        h1d_c_cut2->Draw("PE1");

        cvjetet_c->cd(3);
        TH1D *h1d_c_cut3 = (TH1D*) h1["jet_et"]->Clone("h1d_c_cut3");
        h1d_c_cut3->Divide(h1["jet_et_c_cut3"],h1["jet_et_c"],1.,1.,"B");
        h1d_c_cut3->SetXTitle("#epsilon^{pTrel&Tagger}");
        h1d_c_cut3->SetMarkerColor(1);
        h1d_c_cut3->SetMarkerStyle(8);
        h1d_c_cut3->SetMarkerSize(1.5);
        h1d_c_cut3->Draw("PE1");

        cvjetet_c->cd(4);
        TH1D *h1d_c_ratio = (TH1D*) h1["jet_et"]->Clone("h1d_c_ratio");
        h1d_c_ratio->Divide(h1d_c_cut3,h1d_c_cut1);
        h1d_c_ratio->Divide(h1d_c_cut2);
        h1d_c_ratio->SetYTitle("#epsilon^{pTrel&Tagger}/(#epsilon^{pTrel}#epsilon^{Tagger})");
        h1d_c_ratio->SetMarkerColor(1);
        h1d_c_ratio->SetMarkerStyle(8);
        h1d_c_ratio->SetMarkerSize(1.5);
        h1d_c_ratio->Draw("PE1");
        h1d_c_ratio->Fit("pol1","0");
        TF1 *f1_c = h1d_c_ratio->GetFunction("pol1");
        f1_c->SetLineColor(3);
        f1_c->Draw("same");

	cvjetet_c->Update();
        cvjetet_c->Print("figures/"+TString(cvjetet_c->GetName())+".pdf");

	TCanvas *cvjetet_uds = new TCanvas("jetet_uds","jetet_uds",700,700);
        cvjetet_uds->Divide(2,2);
        cvjetet_uds->cd(1);
        TH1D *h1d_uds_cut1 = (TH1D*) h1["jet_et"]->Clone("h1d_uds_cut1");
        h1d_uds_cut1->Divide(h1["jet_et_uds_cut1"],h1["jet_et_uds"],1.,1.,"B");
        h1d_uds_cut1->SetYTitle("#epsilon^{pTrel}");
        h1d_uds_cut1->SetMarkerColor(1);
        h1d_uds_cut1->SetMarkerStyle(8);
        h1d_uds_cut1->SetMarkerSize(1.5);
        h1d_uds_cut1->Draw("PE1");
        gPad->Update();

        cvjetet_uds->cd(2);
        TH1D *h1d_uds_cut2 = (TH1D*) h1["jet_et"]->Clone("h1d_uds_cut2");
        h1d_uds_cut2->Divide(h1["jet_et_uds_cut2"],h1["jet_et_uds"],1.,1.,"B");
        h1d_uds_cut2->SetYTitle("#epsilon^{Tagger}");
        h1d_uds_cut2->SetMarkerColor(1);
        h1d_uds_cut2->SetMarkerStyle(8);
        h1d_uds_cut2->SetMarkerSize(1.5);
        h1d_uds_cut2->Draw("PE1");

        cvjetet_uds->cd(3);
        TH1D *h1d_uds_cut3 = (TH1D*) h1["jet_et"]->Clone("h1d_uds_cut3");
        h1d_uds_cut3->Divide(h1["jet_et_uds_cut3"],h1["jet_et_uds"],1.,1.,"B");
        h1d_uds_cut3->SetXTitle("#epsilon^{pTrel&Tagger}");
        h1d_uds_cut3->SetMarkerColor(1);
        h1d_uds_cut3->SetMarkerStyle(8);
        h1d_uds_cut3->SetMarkerSize(1.5);
        h1d_uds_cut3->Draw("PE1");

        cvjetet_uds->cd(4);
        TH1D *h1d_uds_ratio = (TH1D*) h1["jet_et"]->Clone("h1d_uds_ratio");
        h1d_uds_ratio->Divide(h1d_uds_cut3,h1d_uds_cut1);
        h1d_uds_ratio->Divide(h1d_uds_cut2);
        h1d_uds_ratio->SetYTitle("#epsilon^{pTrel&Tagger}/(#epsilon^{pTrel}#epsilon^{Tagger})");
        h1d_uds_ratio->SetMarkerColor(1);
        h1d_uds_ratio->SetMarkerStyle(8);
        h1d_uds_ratio->SetMarkerSize(1.5);
        h1d_uds_ratio->Draw("PE1");
        h1d_uds_ratio->Fit("pol1","0");
        TF1 *f1_uds = h1d_uds_ratio->GetFunction("pol1");
        f1_uds->SetLineColor(3);
        f1_uds->Draw("same");

	cvjetet_uds->Update();
        cvjetet_uds->Print("figures/"+TString(cvjetet_uds->GetName())+".pdf");

	TCanvas *cvjetet_g = new TCanvas("jetet_g","jetet_g",700,700);
        cvjetet_g->Divide(2,2);
        cvjetet_g->cd(1);
        TH1D *h1d_g_cut1 = (TH1D*) h1["jet_et"]->Clone("h1d_g_cut1");
        h1d_g_cut1->Divide(h1["jet_et_g_cut1"],h1["jet_et_g"],1.,1.,"B");
        h1d_g_cut1->SetYTitle("#epsilon^{pTrel}");
        h1d_g_cut1->SetMarkerColor(1);
        h1d_g_cut1->SetMarkerStyle(8);
        h1d_g_cut1->SetMarkerSize(1.5);
        h1d_g_cut1->Draw("PE1");
        gPad->Update();

        cvjetet_g->cd(2);
        TH1D *h1d_g_cut2 = (TH1D*) h1["jet_et"]->Clone("h1d_g_cut2");
        h1d_g_cut2->Divide(h1["jet_et_g_cut2"],h1["jet_et_g"],1.,1.,"B");
        h1d_g_cut2->SetYTitle("#epsilon^{Tagger}");
        h1d_g_cut2->SetMarkerColor(1);
        h1d_g_cut2->SetMarkerStyle(8);
        h1d_g_cut2->SetMarkerSize(1.5);
        h1d_g_cut2->Draw("PE1");

        cvjetet_g->cd(3);
        TH1D *h1d_g_cut3 = (TH1D*) h1["jet_et"]->Clone("h1d_g_cut3");
        h1d_g_cut3->Divide(h1["jet_et_g_cut3"],h1["jet_et_g"],1.,1.,"B");
        h1d_g_cut3->SetXTitle("#epsilon^{pTrel&Tagger}");
        h1d_g_cut3->SetMarkerColor(1);
        h1d_g_cut3->SetMarkerStyle(8);
        h1d_g_cut3->SetMarkerSize(1.5);
        h1d_g_cut3->Draw("PE1");

        cvjetet_g->cd(4);
        TH1D *h1d_g_ratio = (TH1D*) h1["jet_et"]->Clone("h1d_g_ratio");
        h1d_g_ratio->Divide(h1d_g_cut3,h1d_g_cut1);
        h1d_g_ratio->Divide(h1d_g_cut2);
        h1d_g_ratio->SetYTitle("#epsilon^{pTrel&Tagger}/(#epsilon^{pTrel}#epsilon^{Tagger})");
        h1d_g_ratio->SetMarkerColor(1);
        h1d_g_ratio->SetMarkerStyle(8);
        h1d_g_ratio->SetMarkerSize(1.5);
        h1d_g_ratio->Draw("PE1");
        h1d_g_ratio->Fit("pol1","0");
        TF1 *f1_g = h1d_g_ratio->GetFunction("pol1");
        f1_g->SetLineColor(3);
        f1_g->Draw("same");

	cvjetet_g->Update();
        cvjetet_g->Print("figures/"+TString(cvjetet_g->GetName())+".pdf");



	// print results
	int cw = 10;
	
	cout << " Results: \n" << endl;
	
	cout << "| sample " << setw(cw) << setw(cw) << setw(cw) << "|  total  " << setw(cw) << "|  b " << setw(cw) << "|  c " << setw(cw) << "|  uds " << setw(cw) << "|  g      |" << endl;

	//cout << " mu+jet:"<< setw(cw);
	for(std::map<std::string,std::vector< double > >::const_iterator imap=eventsmap.begin(); imap!=eventsmap.end(); ++imap){

		hname = imap->first;

		cout << hname << setw(cw) << setw(cw);

		events_sample.clear();
		events_sample = imap->second;
		int tmpsize = events_sample.size();
		for (int isample = 0; isample != tmpsize ; ++isample ) {
			cout << events_sample[isample] << setw(cw);
		}
		
		cout << endl;
	}
	
	 */
}




		


