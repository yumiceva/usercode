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

#include "KinFit.h"

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


	h1["njets"] = new TH1D("njets","number of jets",50,0,50);
	h1["nleptons"] = new TH1D("nleptons","number of muons",8,0,8);

	h1["allmuon_normchi2"] = new TH1D("allmuon_normchi2","#chi^{2}/ndof",40,0,30);
	h1["allmuon_normchi2_MC"] = new TH1D("allmuon_normchi2_MC","#chi^{2}/ndof",40,0,30);
	
	h1["allmuon_pt"] = new TH1D("allmuon_pt","Muon p_{T} [GeV/c]",50,0,100);
	h1["allmuon_pt_MC"] = new TH1D("allmuon_pt_MC","Muon p_{T} [GeV/c]",50,0,100);
	h1["allmuon_d0"] = new TH1D("allmuon_d0","Muon d_{0} [cm]",35,-1,1);
	h1["allmuon_d0_MC"] = new TH1D("allmuon_d0_MC","Muon d_{0} [cm]",35,-1,1);
	h1["allmuon_ips"] = new TH1D("allmuon_ips","Muon d_{0}/#sigma_{d0}",50,-60,60);
	h1["allmuon_ips_MC"] = new TH1D("allmuon_ips_MC","Muon d_{0}/#sigma_{d0}",50,-60,60);

	h1["alljet_et"] = new TH1D("alljet_et","Jet E_{T} [GeV/c]",50,0,100);
	h1["alljet_eta"] = new TH1D("alljet_eta","Jet #eta",50,-5,5);
	h1["alljet_phi"] = new TH1D("alljet_phi","Jet #phi",50,-3.15,3.15);
	
	
	h1["MET"] = new TH1D("MET","MET [GeV]",50,0.0,150.0);
	h1["METcomplex"] = new TH1D("METcomplex","MET [GeV]",50,0.0,150.0);
	h1["phi_Wb"] = new TH1D("phi_Wb","Phi Difference of W(u+v) and J",50,0.0,3.2);    // phi balance t->W+b
	
	h1["kinfit_chi2"] = new TH1D("kinfit_chi2","#chi^{2} Probability",50,0.,1.);
		
	h1["muon_pt"] = new TH1D("muon_pt","Muon p_{T} [GeV/c]",50,0.0,100.0);
	h1["nu_pz"] = new TH1D("nu_pz","Neutrino p_{z} [GeV/c]",50,-500.0,500.0);
	
	h1["mu_nu"] = new TH1D("mu_nu","(#mu + #nu) mass [GeV/c]",80,0.0,300.0);
	
	std::vector < std::string > str_twobody;
	str_twobody.push_back("M01");
	str_twobody.push_back("M02");
	str_twobody.push_back("M03");
	str_twobody.push_back("M12");
	str_twobody.push_back("M13");
	str_twobody.push_back("M23");
	int size_twobody = str_twobody.size();
	
	h1["twobody"] = new TH1D("twobody","jet pair mass [GeV/c]",80,0.0,300.0);
		
	std::vector < std::string > str_threebody;
	str_threebody.push_back("M012");
	str_threebody.push_back("M013");
	str_threebody.push_back("M021");
	str_threebody.push_back("M023");
	str_threebody.push_back("M031");
	str_threebody.push_back("M032");
	str_threebody.push_back("M120");
	str_threebody.push_back("M123");
	str_threebody.push_back("M130");
	str_threebody.push_back("M132");
	str_threebody.push_back("M230");
	str_threebody.push_back("M231");
	int size_threebody = str_threebody.size();

	h1["threebody"] = new TH1D("threebody","jet 3-body mass [GeV/c]",50,0.0,500.0);
	h1["threebody_1b"] = new TH1D("threebody_1b","jet 3-body mass [GeV/c]",50,0.0,500.0);
	h1["threebody_2b"] = new TH1D("threebody_2b","jet 3-body mass [GeV/c]",50,0.0,500.0);
	
	h2["scatter_plot_0"] = new TH2D("scatter_plot_0","two vs three body",80,0,300,50,0,500);
	h2["scatter_plot_1"] = new TH2D("scatter_plot_1","two vs three body",80,0,300,50,0,500);

	h1["mu_nu_jet"] = new TH1D("mu_nu_jet","(#mu+#nu+jet) mass [GeV/c]",50,0,500);
	
	h1["twobody_cut1"] = new TH1D("twobody_cut1","jet pair mass [GeV/c]",80,0.0,300.0);
	h1["twobody_cut2"] = new TH1D("twobody_cut2","jet pair mass [GeV/c]",80,0.0,300.0);
	h1["twobody_cut21"] = new TH1D("twobody_cut21","jet pair mass [GeV/c]",80,0.0,300.0);

	h1["threebody_cut1"] = new TH1D("threebody_cut1","jet 3-body mass [GeV/c]",50,0.0,500.0);

	h1["threebody_cut2"] = new TH1D("threebody_cut2","jet 3-body mass [GeV/c]",50,0.0,500.0);
	h1["threebody_kinfit"] = new TH1D("threebody_kinfit","jet 3-body mass [GeV/c]",50,0.0,500.0);

	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();
	std::cout << " Total entries = " << fChain->GetEntries() << std::endl;
	

	// KinFit
	KinFit afit;
		
	//______ main loop over entries______
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
	
		if ( fverbose ) { std::cout << " processing entry: " << jentry << std::endl; }
		else if ( jentry%100000 == 0 ) { std::cout << " processing entry: " << jentry << std::endl; }

				
		// total number of objects
		h1["njets"]->Fill(fevent->njets);
		h1["nleptons"]->Fill(fevent->nleptons);
		
		// all muons
		int muon_size = fevent->allmuon_pt.size();
		for (int imu=0; imu!= muon_size; ++imu) {
			h1["allmuon_normchi2"]->Fill(fevent->allmuon_normchi2[imu]);
			h1["allmuon_pt"]->Fill(fevent->allmuon_pt[imu]);
			h1["allmuon_d0"]->Fill(fevent->allmuon_d0[imu]);
			h1["allmuon_ips"]->Fill(fevent->allmuon_d0[imu]/fevent->allmuon_d0Error[imu]);
			
			int muid = fevent->allmuon_mc_pdgid[imu];
			if (TMath::Abs(muid) == 13 ) {
				h1["allmuon_normchi2_MC"]->Fill(fevent->allmuon_normchi2[imu]);
				h1["allmuon_pt_MC"]->Fill(fevent->allmuon_pt[imu]);
				h1["allmuon_d0_MC"]->Fill(fevent->allmuon_d0[imu]);
				h1["allmuon_ips_MC"]->Fill(fevent->allmuon_d0[imu]/fevent->allmuon_d0Error[imu]);
			}
			
		}
		// all jets
		TLorentzVector p4MET;

		int jet_size = fevent->alljet_et.size();
		for (int ijet=0; ijet!= jet_size; ++ijet) {

		  double correction = 1.;

		  if (fcorrections) correction = fevent->jet_correction[ijet];
		  p4MET += TLorentzVector(correction*fevent->alljet_px[ijet],correction*fevent->alljet_py[ijet],0.,0.);

		  if ( jentry < 3 ) std::cout << "met px= " << p4MET.Px() << " py=" <<  p4MET.Py() << " pz=" <<  p4MET.Pz() <<  " e\
=" <<  p4MET.E() << std::endl;

			h1["alljet_et"]->Fill(fevent->alljet_et[ijet]);
			h1["alljet_eta"]->Fill(fevent->alljet_eta[ijet]);
			h1["alljet_phi"]->Fill(fevent->alljet_phi[ijet]);
		}

		//int imuon = 0;
		if ( fverbose ) std::cout << " try to get a muon" << std::endl;
		TLorentzVector p4Muon;
		int cutmu_size = fevent->muon_px.size();
		bool hasgoodmuon = false;
		TLorentzVector tmpp4Muon;
		for (int imu=0; imu != cutmu_size; ++imu) {

			tmpp4Muon.SetXYZT(fevent->muon_px[imu],
						   fevent->muon_py[imu],
						   fevent->muon_pz[imu],
						   fevent->muon_e[imu]);

			if ( jentry < 3 ) std::cout << " tmp MUON pt = " << tmpp4Muon.Pt() << std::endl;
			if ( (tmpp4Muon.Pt() >10.) && (tmpp4Muon.Pt() > p4Muon.Pt() ) ) {
				p4Muon = tmpp4Muon;
				hasgoodmuon=true;
			}
			//else {
				//std::cout << "no good muons" << endl;
				//continue;
			//}
		}
		if ( jentry < 3 ) std::cout << " the MUON pt = " << p4Muon.Pt() << std::endl;
		if ( jentry < 3 ) std::cout << "muon px= " << p4Muon.Px() << " py=" <<  p4Muon.Py() << " pz=" <<  p4Muon.Pz() <<  " e=" <<  p4Muon.E() << std::endl; 
		if (!hasgoodmuon) {
			//std::cout << " NO good muons skipping " << std::endl;
			continue;
		}
		
		double muon_pt  = p4Muon.Pt();
		double muon_phi = p4Muon.Phi();
		double muon_p   = p4Muon.P();
		
		h1["muon_pt"]->Fill(muon_pt);

		if ( fverbose ) std::cout << " got muon" << std::endl;
		
		//TLorentzVector p4MET;
		TLorentzVector p4Jet[4];
		double phiJet[4];
		
		bool isbtagged[4] = {false};
		int nbtagged = 0;


		
		for (int ijet=0; ijet<4; ijet++ ) {

			double correction = 1.;

			if (fcorrections) correction = fevent->jet_correction[ijet];
			
			p4Jet[ijet] = correction * TLorentzVector(fevent->jet_px[ijet],
													  fevent->jet_py[ijet],
													  fevent->jet_pz[ijet],
													  fevent->jet_e[ijet]);
			
			if ( jentry < 3 ) std::cout << "jet px= " << p4Jet[ijet].Px() << " py=" <<  p4Jet[ijet].Py() << " pz=" <<  p4Jet[ijet].Pz() <<  " e=" <<  p4Jet[ijet].E() << std::endl; 
			//p4MET += TLorentzVector(p4Jet[ijet].Px(),p4Jet[ijet].Py(),0.,0.);

			//if ( jentry < 3 ) std::cout << "met px= " << p4MET.Px() << " py=" <<  p4MET.Py() << " pz=" <<  p4MET.Pz() <<  " e=" <<  p4MET.E() << std::endl; 
			
			phiJet[ijet] = p4Jet[ijet].Phi();
			phiJet[ijet] = p4Jet[ijet].Phi();
			phiJet[ijet] = p4Jet[ijet].Phi();
			phiJet[ijet] = p4Jet[ijet].Phi();
			
			// b taggingig
			if (fevent->jet_btag_discriminant[ijet]>4. ) {
				isbtagged[ijet] = true;
				nbtagged++;
			}

			/*
			if ( tag1jet && !tag2jet && (fevent->jet_btag_discriminant[ijet]>4.) ) {
				disc2 = fevent->jet_btag_discriminant[ijet];
				tag2jet = true;
			}
			if ( !tag1jet && (fevent->jet_btag_discriminant[ijet]>4.) ) {
				disc1 = fevent->jet_btag_discriminant[ijet];
				tag1jet = true;
			}
			*/
			
		}
		//std::cout << " nbtagged= " << nbtagged << std::endl;
		
		if ( fverbose ) std::cout << " got jet" << std::endl;
		
		p4MET += TLorentzVector(p4Muon.Px(),p4Muon.Py(),0.,0.);

		p4MET = (-1.)*p4MET;
		if ( jentry < 3 ) std::cout << "p4METx = " << p4MET.Px() << " p4METy = " << p4MET.Py() << " p4METz = " << p4MET.Pz() << std::endl;
		
		h1["MET"]->Fill(p4MET.Pt());

		//if ( p4MET.Pt() <= 25.0 ) continue;
		
		// re-do 2- and 3- body combinations	
		TLorentzVector twobody[6];
		TLorentzVector threebody[12];
		std::map< std::string, double > twophi;
		
		twobody[0] = p4Jet[0] + p4Jet[1];
		twobody[1] = p4Jet[0] + p4Jet[2];
		twobody[2] = p4Jet[0] + p4Jet[3];
		twobody[3] = p4Jet[1] + p4Jet[2];
		twobody[4] = p4Jet[1] + p4Jet[3];
		twobody[5] = p4Jet[2] + p4Jet[3];

		twophi["01"] = twobody[0].Phi();
		twophi["02"] = twobody[1].Phi();
		twophi["03"] = twobody[2].Phi();
		twophi["12"] = twobody[3].Phi();
		twophi["13"] = twobody[4].Phi();
		twophi["23"] = twobody[5].Phi();
		
		std::map< std::string, double > deltaphi;
		deltaphi["012"] = TMath::Abs(twophi["01"] - phiJet[2]);
		deltaphi["013"] = TMath::Abs(twophi["01"] - phiJet[3]);
		deltaphi["021"] = TMath::Abs(twophi["02"] - phiJet[1]);
		deltaphi["023"] = TMath::Abs(twophi["02"] - phiJet[3]);
		deltaphi["031"] = TMath::Abs(twophi["03"] - phiJet[1]);
		deltaphi["032"] = TMath::Abs(twophi["03"] - phiJet[2]);
		deltaphi["120"] = TMath::Abs(twophi["12"] - phiJet[0]);
		deltaphi["123"] = TMath::Abs(twophi["12"] - phiJet[3]);
		deltaphi["130"] = TMath::Abs(twophi["13"] - phiJet[0]);
		deltaphi["132"] = TMath::Abs(twophi["13"] - phiJet[2]);
		deltaphi["230"] = TMath::Abs(twophi["23"] - phiJet[0]);
		deltaphi["231"] = TMath::Abs(twophi["23"] - phiJet[1]);
				
		for (std::map<std::string, double>::const_iterator imap = deltaphi.begin();
			 imap != deltaphi.end(); ++imap ) {
			std::string comb = imap->first;
			double tmpphi = imap->second;
			if ( tmpphi> TMath::Pi() ) deltaphi[comb] = TMath::Abs(2.*TMath::Pi() - tmpphi);
		}

				
		threebody[0] = twobody[0] + p4Jet[2];
		threebody[1] = twobody[0] + p4Jet[3];
		threebody[2] = twobody[1] + p4Jet[1];
		threebody[3] = twobody[1] + p4Jet[3];
		threebody[4] = twobody[2] + p4Jet[1];
		threebody[5] = twobody[2] + p4Jet[2];
		threebody[6] = twobody[3] + p4Jet[0];
		threebody[7] = twobody[3] + p4Jet[3];
		threebody[8] = twobody[4] + p4Jet[0];
		threebody[9] = twobody[4] + p4Jet[2];
		threebody[10] = twobody[5] + p4Jet[0];
		threebody[11] = twobody[5] + p4Jet[1];
				
		for ( int ii=0; ii<6; ii++) {
			h1["twobody"]->Fill(twobody[ii].M());			
		}

		for ( int ii=0; ii<12; ii++) {
			
			h1["threebody"]->Fill(threebody[ii].M());			
		}

		
		h2["scatter_plot_0"]->Fill( twobody[0].M(), threebody[0].M() );
		h2["scatter_plot_0"]->Fill( twobody[0].M(), threebody[1].M() );
		h2["scatter_plot_0"]->Fill( twobody[1].M(), threebody[2].M() );
		h2["scatter_plot_0"]->Fill( twobody[1].M(), threebody[3].M() );
		h2["scatter_plot_0"]->Fill( twobody[2].M(), threebody[4].M() );
		h2["scatter_plot_0"]->Fill( twobody[2].M(), threebody[5].M() );
		h2["scatter_plot_0"]->Fill( twobody[3].M(), threebody[6].M() );
		h2["scatter_plot_0"]->Fill( twobody[3].M(), threebody[7].M() );
		h2["scatter_plot_0"]->Fill( twobody[4].M(), threebody[8].M() );
		h2["scatter_plot_0"]->Fill( twobody[4].M(), threebody[9].M() );
		h2["scatter_plot_0"]->Fill( twobody[5].M(), threebody[10].M() );
		h2["scatter_plot_0"]->Fill( twobody[5].M(), threebody[11].M() );
		
		
		// b tagging

		TLorentzVector threebodyb[12];
		if (nbtagged>0) {
		  threebodyb[0] = twobody[0] + p4Jet[2];
		  threebodyb[1] = twobody[0] + p4Jet[3];
		  threebodyb[2] = twobody[1] + p4Jet[1];
		  threebodyb[3] = twobody[1] + p4Jet[3];
		  threebodyb[4] = twobody[2] + p4Jet[1];
		  threebodyb[5] = twobody[2] + p4Jet[2];
		  threebodyb[6] = twobody[3] + p4Jet[0];
		  threebodyb[7] = twobody[3] + p4Jet[3];
		  threebodyb[8] = twobody[4] + p4Jet[0];
		  threebodyb[9] = twobody[4] + p4Jet[2];
		  threebodyb[10] = twobody[5] + p4Jet[0];
		  threebodyb[11] = twobody[5] + p4Jet[1];

		}

	       
		/*
		if ( isbtagged[0] ) {
		         if ( isbtagged[1] ) threebodycorrb[10] = twobodycorr[5] + p4Jetcorr[0];
			 if ( isbtagged[2] ) threebodycorrb[8] = twobodycorr[4] + p4Jetcorr[0];
			 if ( isbtaaged[3] ) threebodycorrb[6] = twobodycorr[3] + p4Jetcorr[0];
		}
		if ( isbtagged[1] ) {
		        if ( isbtagged[2] ) threebodycorrb[4] = twobodycorr[2] + p4Jetcorr[1];
			if ( isbtaaged[3] ) threebodycorrb[2] = twobodycorr[1] + p4Jetcorr[1];
		}
		if ( isbtagged[2] ) {
                       if ( isbtagged[3] ) threebodycorrb[0] = twobodycorr[0] + p4Jetcorr[2];
		}
		*/
		for ( int ii=0; ii<12; ii++) {
			h1["threebody_1b"]->Fill(threebodyb[ii].M());
		}
		

		TLorentzVector threebody2b[12];
		if ( nbtagged>1) {

			threebody2b[0] = twobody[0] + p4Jet[2];
			threebody2b[1] = twobody[0] + p4Jet[3];
			threebody2b[2] = twobody[1] + p4Jet[1];
			threebody2b[3] = twobody[1] + p4Jet[3];
			threebody2b[4] = twobody[2] + p4Jet[1];
			threebody2b[5] = twobody[2] + p4Jet[2];
			threebody2b[6] = twobody[3] + p4Jet[0];
			threebody2b[7] = twobody[3] + p4Jet[3];
			threebody2b[8] = twobody[4] + p4Jet[0];
			threebody2b[9] = twobody[4] + p4Jet[2];
			threebody2b[10] = twobody[5] + p4Jet[0];
			threebody2b[11] = twobody[5] + p4Jet[1];
		}

		/*
		if ( isbtagged[0] ) {
			if ( isbtagged[1] ) {
				
				threebodycorr2b[10] = twobodycorr[5] + p4Jetcorr[0];
			}
			if ( isbtagged[2] ) {
				threebodycorr2b[8] = twobodycorr[4] + p4Jetcorr[0];
			}
			if ( isbtagged[3] ) {
				threebodycorr2b[6] = twobodycorr[3] + p4Jetcorr[0];
			}
		}
		if ( isbtagged[1] ) {
			if ( isbtagged[0] ) {
				threebodycorr2b[11] = twobodycorr[5] + p4Jetcorr[1];
			}
			if ( isbtagged[2] ) {
				threebodycorr2b[4] = twobodycorr[2] + p4Jetcorr[1];
			}
			if ( isbtagged[3] ) {
				threebodycorr2b[2] = twobodycorr[1] + p4Jetcorr[1];
			}
		}
		if ( isbtagged[2] ) {
			if ( isbtagged[0] ) {
				threebodycorr2b[9] = twobodycorr[4] + p4Jetcorr[2];
			}
			if ( isbtagged[1] ) {
				threebodycorr2b[3] = twobodycorr[1] + p4Jetcorr[3];
			}
			if ( isbtagged[3] ) {
				threebodycorr2b[0] = twobodycorr[0] + p4Jetcorr[2];
			}
		}
		if ( isbtagged[3] ) {
			if ( isbtagged[0] ) {
				threebodycorr2b[7] = twobodycorr[3] + p4Jetcorr[3];
			}
			if ( isbtagged[1] ) {
				threebodycorr2b[3] = twobodycorr[1] + p4Jetcorr[3];
			}
			if ( isbtagged[2] ) {
				threebodycorr2b[1] = twobodycorr[0] + p4Jetcorr[3];
			}
		}
		*/

		for ( int ii=0; ii<12; ii++) {
			h1["threebody_2b"]->Fill(threebody2b[ii].M());
		}

		

		// solve for a W using muon and MET
		double pzNu = GetPzNu(p4Muon, p4MET);
		h1["nu_pz"]->Fill( pzNu );
		if ( jentry < 3 ) std::cout << "nupz=" << pzNu << std::endl;
		
		// now find mu+nu+jet 4 combos
		if (fcomplex) {
			h1["METcomplex"]->Fill( p4MET.Pt() ); 
		}
		if (true) {
		  h1["nu_pz"]->Fill( pzNu );

		  TLorentzVector p4Nu;
		  p4Nu.SetXYZT(p4MET.Px(), p4MET.Py(),pzNu,
					   TMath::Sqrt(p4MET.Px() * p4MET.Px() + p4MET.Py()*p4MET.Py() + pzNu*pzNu) );

		//std::cout << "got p4Nu" << std::endl;
		
		  TLorentzVector p4W = p4Muon + p4Nu;
		  h1["mu_nu"]->Fill(p4W.M() );

		  double deltaphiWb[4] = {0.};
		  for (int j=0; j<4; ++j) {

			  deltaphiWb[j] = TMath::Abs(phiJet[j] - p4W.Phi() );
			  if ( deltaphiWb[j] > TMath::Pi() ) deltaphiWb[j] = TMath::Abs(2.*TMath::Pi() - deltaphiWb[j] );
			  h1["phi_Wb"]->Fill(deltaphiWb[j]);
		  }
		  
		double mt[4] = {0};
		TLorentzVector tmp_mt;
		tmp_mt = p4Muon + p4Nu + p4Jet[0];
		mt[0] = tmp_mt.M();
		tmp_mt = p4Muon + p4Nu + p4Jet[1];
		mt[1] = tmp_mt.M();
		tmp_mt = p4Muon + p4Nu + p4Jet[2];
		mt[2] = tmp_mt.M();
		tmp_mt = p4Muon + p4Nu + p4Jet[3];
		mt[3] = tmp_mt.M();
		
/*
		double mtcorr[4] = {0};
		tmp_mt = p4Muon + p4Nu + p4Jetcorr[0];
		mtcorr[0] = tmp_mt.M();
		tmp_mt = p4Muon + p4Nu + p4Jetcorr[1];
		mtcorr[1] = tmp_mt.M();
		tmp_mt = p4Muon + p4Nu + p4Jetcorr[2];
		mtcorr[2] = tmp_mt.M();
		tmp_mt = p4Muon + p4Nu + p4Jetcorr[3];
		mtcorr[3] = tmp_mt.M();
*/
		
		//std::cout << "filled mtcorr" << std::endl;
		
		for ( int i=0; i<4; i++) { h1["mu_nu_jet"]->Fill(mt[i]); }
		
		double Mtmin = 0;
		double Mtmax = 150;

		if ( fcorrections ) {
			Mtmin = 140;
			Mtmax = 210;
		}
		
		if ((mt[0] > Mtmin) && (mt[0] < Mtmax) ) {

			h2["scatter_plot_1"]->Fill( twobody[3].M(), threebody[7].M() );
			h2["scatter_plot_1"]->Fill( twobody[4].M(), threebody[9].M() );
			h2["scatter_plot_1"]->Fill( twobody[5].M(), threebody[11].M() );
			h1["twobody_cut1"]->Fill( twobody[3].M() );
			h1["twobody_cut1"]->Fill( twobody[4].M() );
			h1["twobody_cut1"]->Fill( twobody[5].M() );
			h1["threebody_cut1"]->Fill( threebody[7].M() );
			h1["threebody_cut1"]->Fill( threebody[9].M() );
			h1["threebody_cut1"]->Fill( threebody[11].M() );

			if (deltaphiWb[0] > fphicut ) {
				h1["twobody_cut2"]->Fill( twobody[3].M() );
				h1["twobody_cut2"]->Fill( twobody[4].M() );
				h1["twobody_cut2"]->Fill( twobody[5].M() );
			
				if (deltaphi["012"] > fphicut ) {
					h1["threebody_cut2"]->Fill( threebody[7].M() );
					h1["twobody_cut21"]->Fill( twobody[3].M() );
				}
				if (deltaphi["132"] > fphicut ) {
					h1["threebody_cut2"]->Fill( threebody[9].M() );
					h1["twobody_cut21"]->Fill( twobody[4].M() );
				}
				if (deltaphi["231"] > fphicut ) {
					h1["threebody_cut2"]->Fill( threebody[11].M() );
					h1["twobody_cut21"]->Fill( twobody[5].M() );
				}
			}
		}
		if ((mt[1] > Mtmin) && (mt[1] < Mtmax) ) {
			
			h2["scatter_plot_1"]->Fill( twobody[1].M(), threebody[3].M() );
			h2["scatter_plot_1"]->Fill( twobody[2].M(), threebody[5].M());
			h2["scatter_plot_1"]->Fill( twobody[5].M(), threebody[10].M() );
			h1["twobody_cut1"]->Fill( twobody[1].M() );
			h1["twobody_cut1"]->Fill( twobody[2].M() );
			h1["twobody_cut1"]->Fill( twobody[5].M() );
			h1["threebody_cut1"]->Fill( threebody[3].M() );
			h1["threebody_cut1"]->Fill( threebody[5].M() );
			h1["threebody_cut1"]->Fill( threebody[10].M() );
			
			if (deltaphiWb[1] > fphicut ) {
				h1["twobody_cut2"]->Fill( twobody[1].M() );
				h1["twobody_cut2"]->Fill( twobody[2].M() );
				h1["twobody_cut2"]->Fill( twobody[5].M() );

				if (deltaphi["023"] > fphicut ) {
					h1["threebody_cut2"]->Fill( threebody[3].M() );
					h1["twobody_cut21"]->Fill( twobody[1].M() );
				}
				if (deltaphi["032"] > fphicut ) {
					h1["threebody_cut2"]->Fill( threebody[5].M() );
					h1["twobody_cut21"]->Fill( twobody[2].M() );
				}
				if (deltaphi["230"] > fphicut ) {
					h1["threebody_cut2"]->Fill( threebody[10].M() );
					h1["twobody_cut21"]->Fill( twobody[5].M() );
				}
				
			}
		}
		if ((mt[2] > Mtmin) && (mt[2] < Mtmax) ) {
			
			h2["scatter_plot_1"]->Fill( twobody[0].M(), threebody[1].M() );
			h2["scatter_plot_1"]->Fill( twobody[2].M(), threebody[4].M() );
			h2["scatter_plot_1"]->Fill( twobody[4].M(), threebody[8].M() );
			h1["twobody_cut1"]->Fill( twobody[0].M() );
			h1["twobody_cut1"]->Fill( twobody[2].M() );
			h1["twobody_cut1"]->Fill( twobody[4].M() );
			h1["threebody_cut1"]->Fill( threebody[1].M() );
			h1["threebody_cut1"]->Fill( threebody[4].M() );
			h1["threebody_cut1"]->Fill( threebody[8].M() );

			if (deltaphiWb[2] > fphicut ) {
				h1["twobody_cut2"]->Fill( twobody[0].M() );
				h1["twobody_cut2"]->Fill( twobody[2].M() );
				h1["twobody_cut2"]->Fill( twobody[4].M() );

				if (deltaphi["013"] > fphicut ) {
					h1["threebody_cut2"]->Fill( threebody[1].M() );
					h1["twobody_cut21"]->Fill( twobody[0].M() );
				}
				if (deltaphi["031"] > fphicut ) {
					h1["threebody_cut2"]->Fill( threebody[4].M() );
					h1["twobody_cut21"]->Fill( twobody[2].M() );
				}
				if (deltaphi["130"] > fphicut ) {
					h1["threebody_cut2"]->Fill( threebody[8].M() );
					h1["twobody_cut21"]->Fill( twobody[4].M() );
				}
			}
		}
		if ((mt[3] > Mtmin) && (mt[3] < Mtmax) ) {
			h2["scatter_plot_1"]->Fill( twobody[0].M(), threebody[0].M() );
			h2["scatter_plot_1"]->Fill( twobody[1].M(), threebody[2].M() );
			h2["scatter_plot_1"]->Fill( twobody[3].M(), threebody[6].M() );
			h1["twobody_cut1"]->Fill( twobody[0].M() );
			h1["twobody_cut1"]->Fill( twobody[1].M() );
			h1["twobody_cut1"]->Fill( twobody[3].M() );
			h1["threebody_cut1"]->Fill( threebody[0].M() );
			h1["threebody_cut1"]->Fill( threebody[2].M() );
			h1["threebody_cut1"]->Fill( threebody[6].M() );

			if (deltaphiWb[3] > fphicut ) {
				h1["twobody_cut2"]->Fill( twobody[0].M() );
				h1["twobody_cut2"]->Fill( twobody[1].M() );
				h1["twobody_cut2"]->Fill( twobody[3].M() );

				if (deltaphi["012"] > fphicut ) {
					h1["threebody_cut2"]->Fill( threebody[0].M() );
					h1["twobody_cut21"]->Fill( twobody[0].M() );
				}
				if (deltaphi["021"] > fphicut ) {
					h1["threebody_cut2"]->Fill( threebody[2].M() );
					h1["twobody_cut21"]->Fill( twobody[1].M() );
				}
				if (deltaphi["120"] > fphicut ) {
					h1["threebody_cut2"]->Fill( threebody[6].M() );
					h1["twobody_cut21"]->Fill( twobody[3].M() );
				}
			}
		}
		
		}

		// KinFit
		double kinfit_chi2[12] = {0.};
		double cfit[4] = {0.};
		TLorentzVector p4FitJet[4];
		TLorentzVector fitTop;
		TLorentzVector fitW;
		
		afit.SetPl(   p4Muon   );

		afit.SetPj1(  p4Jet[0] );
		afit.SetPj2(  p4Jet[1] );
		afit.SetPbj2( p4Jet[2] );
		afit.SetPbj1( p4Jet[3] );
		afit.SetPzNu( pzNu );
		afit.Fit();
		kinfit_chi2[0] = afit.GetChi2();
		if ( kinfit_chi2[0] > fminKFchi2 ) {
			cfit[0] = afit.GetFitEj1()/p4Jet[0].E();
			cfit[1] = afit.GetFitEj2()/p4Jet[1].E();
			cfit[2] = afit.GetFitEbj2()/p4Jet[2].E();
			cfit[3] = afit.GetFitEbj1()/p4Jet[3].E();

			p4FitJet[0] = cfit[0]*p4Jet[0];
			p4FitJet[1] = cfit[1]*p4Jet[1];
			p4FitJet[2] = cfit[2]*p4Jet[2];
			p4FitJet[3] = cfit[3]*p4Jet[3];
			
			fitTop = p4FitJet[0] + p4FitJet[1] + p4Jet[2];
			h1["threebody_kinfit"]->Fill(fitTop.M() );
		}
			
		afit.SetPj1(  p4Jet[0] );
		afit.SetPj2(  p4Jet[1] );
		afit.SetPbj2( p4Jet[3] );
		afit.SetPbj1( p4Jet[2] );
		afit.Fit();
		kinfit_chi2[1] = afit.GetChi2();
		if ( kinfit_chi2[1] > fminKFchi2 ) {
			cfit[0] = afit.GetFitEj1()/p4Jet[0].E();
			cfit[1] = afit.GetFitEj2()/p4Jet[1].E();
			cfit[2] = afit.GetFitEbj2()/p4Jet[3].E();
			cfit[3] = afit.GetFitEbj1()/p4Jet[2].E();

			p4FitJet[0] = cfit[0]*p4Jet[0];
			p4FitJet[1] = cfit[1]*p4Jet[1];
			p4FitJet[3] = cfit[2]*p4Jet[3];
			p4FitJet[2] = cfit[3]*p4Jet[2];
			
			fitTop = p4FitJet[0] + p4FitJet[1] + p4Jet[3];
			h1["threebody_kinfit"]->Fill(fitTop.M() );
		}
		
		afit.SetPj1(  p4Jet[0] );
		afit.SetPj2(  p4Jet[2] );
		afit.SetPbj2( p4Jet[1] );
		afit.SetPbj1( p4Jet[3] );
		afit.Fit();
		kinfit_chi2[2] = afit.GetChi2();
		if ( kinfit_chi2[2] > fminKFchi2 ) {
			cfit[0] = afit.GetFitEj1()/p4Jet[0].E();
			cfit[1] = afit.GetFitEj2()/p4Jet[2].E();
			cfit[2] = afit.GetFitEbj2()/p4Jet[1].E();
			cfit[3] = afit.GetFitEbj1()/p4Jet[3].E();

			p4FitJet[0] = cfit[0]*p4Jet[0];
			p4FitJet[2] = cfit[1]*p4Jet[2];
			p4FitJet[1] = cfit[2]*p4Jet[1];
			p4FitJet[3] = cfit[3]*p4Jet[3];
			
			fitTop = p4FitJet[0] + p4FitJet[2] + p4Jet[1];
			h1["threebody_kinfit"]->Fill(fitTop.M() );
		}
		
		afit.SetPj1(  p4Jet[0] );
		afit.SetPj2(  p4Jet[2] );
		afit.SetPbj2( p4Jet[3] );
		afit.SetPbj1( p4Jet[1] );
		afit.Fit();
		kinfit_chi2[3] = afit.GetChi2();
		if ( kinfit_chi2[3] > fminKFchi2 ) {
			cfit[0] = afit.GetFitEj1()/p4Jet[0].E();
			cfit[1] = afit.GetFitEj2()/p4Jet[2].E();
			cfit[2] = afit.GetFitEbj2()/p4Jet[3].E();
			cfit[3] = afit.GetFitEbj1()/p4Jet[1].E();

			p4FitJet[0] = cfit[0]*p4Jet[0];
			p4FitJet[2] = cfit[1]*p4Jet[2];
			p4FitJet[3] = cfit[2]*p4Jet[3];
			p4FitJet[1] = cfit[3]*p4Jet[1];
			
			fitTop = p4FitJet[0] + p4FitJet[2] + p4Jet[3];
			h1["threebody_kinfit"]->Fill(fitTop.M() );
		}
		
		afit.SetPj1(  p4Jet[0] );
		afit.SetPj2(  p4Jet[3] );
		afit.SetPbj2( p4Jet[1] );
		afit.SetPbj1( p4Jet[2] );
		afit.Fit();
		kinfit_chi2[4] = afit.GetChi2();
		if ( kinfit_chi2[4] > fminKFchi2 ) {
			cfit[0] = afit.GetFitEj1()/p4Jet[0].E();
			cfit[1] = afit.GetFitEj2()/p4Jet[3].E();
			cfit[2] = afit.GetFitEbj2()/p4Jet[1].E();
			cfit[3] = afit.GetFitEbj1()/p4Jet[2].E();

			p4FitJet[0] = cfit[0]*p4Jet[0];
			p4FitJet[3] = cfit[1]*p4Jet[3];
			p4FitJet[1] = cfit[2]*p4Jet[1];
			p4FitJet[2] = cfit[3]*p4Jet[2];
			
			fitTop = p4FitJet[0] + p4FitJet[3] + p4Jet[1];
			h1["threebody_kinfit"]->Fill(fitTop.M() );
		}
		
		afit.SetPj1(  p4Jet[0] );
		afit.SetPj2(  p4Jet[3] );
		afit.SetPbj2( p4Jet[2] );
		afit.SetPbj1( p4Jet[1] );
		afit.Fit();
		kinfit_chi2[5] = afit.GetChi2();
		if ( kinfit_chi2[5] > fminKFchi2 ) {
			cfit[0] = afit.GetFitEj1()/p4Jet[0].E();
			cfit[1] = afit.GetFitEj2()/p4Jet[3].E();
			cfit[2] = afit.GetFitEbj2()/p4Jet[2].E();
			cfit[3] = afit.GetFitEbj1()/p4Jet[1].E();

			p4FitJet[0] = cfit[0]*p4Jet[0];
			p4FitJet[3] = cfit[1]*p4Jet[3];
			p4FitJet[2] = cfit[2]*p4Jet[2];
			p4FitJet[1] = cfit[3]*p4Jet[1];
			
			fitTop = p4FitJet[0] + p4FitJet[3] + p4Jet[2];
			h1["threebody_kinfit"]->Fill(fitTop.M() );
		}
		
		afit.SetPj1(  p4Jet[1] );
		afit.SetPj2(  p4Jet[2] );
		afit.SetPbj2( p4Jet[0] );
		afit.SetPbj1( p4Jet[3] );
		afit.Fit();
		kinfit_chi2[6] = afit.GetChi2();
		if ( kinfit_chi2[6] > fminKFchi2 ) {
			cfit[0] = afit.GetFitEj1()/p4Jet[1].E();
			cfit[1] = afit.GetFitEj2()/p4Jet[2].E();
			cfit[2] = afit.GetFitEbj2()/p4Jet[0].E();
			cfit[3] = afit.GetFitEbj1()/p4Jet[3].E();

			p4FitJet[1] = cfit[0]*p4Jet[1];
			p4FitJet[2] = cfit[1]*p4Jet[2];
			p4FitJet[0] = cfit[2]*p4Jet[0];
			p4FitJet[3] = cfit[3]*p4Jet[3];
			
			fitTop = p4FitJet[1] + p4FitJet[2] + p4Jet[0];
			h1["threebody_kinfit"]->Fill(fitTop.M() );
		}
		
		afit.SetPj1(  p4Jet[1] );
		afit.SetPj2(  p4Jet[2] );
		afit.SetPbj2( p4Jet[3] );
		afit.SetPbj1( p4Jet[0] );
		afit.Fit();
		kinfit_chi2[7] = afit.GetChi2();
		if ( kinfit_chi2[7] > fminKFchi2 ) {
			cfit[0] = afit.GetFitEj1()/p4Jet[1].E();
			cfit[1] = afit.GetFitEj2()/p4Jet[2].E();
			cfit[2] = afit.GetFitEbj2()/p4Jet[3].E();
			cfit[3] = afit.GetFitEbj1()/p4Jet[0].E();

			p4FitJet[0] = cfit[0]*p4Jet[1];
			p4FitJet[1] = cfit[1]*p4Jet[2];
			p4FitJet[2] = cfit[2]*p4Jet[3];
			p4FitJet[3] = cfit[3]*p4Jet[0];
						
			fitTop = p4FitJet[1] + p4FitJet[2] + p4Jet[3];
			h1["threebody_kinfit"]->Fill(fitTop.M() );
		}
		
		afit.SetPj1(  p4Jet[1] );
		afit.SetPj2(  p4Jet[3] );
		afit.SetPbj2( p4Jet[0] );
		afit.SetPbj1( p4Jet[2] );
		afit.Fit();
		kinfit_chi2[8] = afit.GetChi2();
		if ( kinfit_chi2[8] > fminKFchi2 ) {
			cfit[0] = afit.GetFitEj1()/p4Jet[1].E();
			cfit[1] = afit.GetFitEj2()/p4Jet[3].E();
			cfit[2] = afit.GetFitEbj2()/p4Jet[0].E();
			cfit[3] = afit.GetFitEbj1()/p4Jet[2].E();

			p4FitJet[1] = cfit[0]*p4Jet[1];
			p4FitJet[3] = cfit[1]*p4Jet[3];
			p4FitJet[0] = cfit[2]*p4Jet[0];
			p4FitJet[2] = cfit[3]*p4Jet[2];
			
			fitTop = p4FitJet[1] + p4FitJet[3] + p4Jet[0];
			h1["threebody_kinfit"]->Fill(fitTop.M() );
		}
		
		
		afit.SetPj1(  p4Jet[1] );
		afit.SetPj2(  p4Jet[3] );
		afit.SetPbj2( p4Jet[2] );
		afit.SetPbj1( p4Jet[0] );
		afit.Fit();
		kinfit_chi2[9] = afit.GetChi2();
		if ( kinfit_chi2[9] > fminKFchi2 ) {
			cfit[0] = afit.GetFitEj1()/p4Jet[1].E();
			cfit[1] = afit.GetFitEj2()/p4Jet[3].E();
			cfit[2] = afit.GetFitEbj2()/p4Jet[2].E();
			cfit[3] = afit.GetFitEbj1()/p4Jet[0].E();

			p4FitJet[1] = cfit[0]*p4Jet[1];
			p4FitJet[3] = cfit[1]*p4Jet[3];
			p4FitJet[2] = cfit[2]*p4Jet[2];
			p4FitJet[0] = cfit[3]*p4Jet[0];
			
			fitTop = p4FitJet[1] + p4FitJet[3] + p4Jet[2];
			h1["threebody_kinfit"]->Fill(fitTop.M() );
		}

		
		afit.SetPj1(  p4Jet[2] );
		afit.SetPj2(  p4Jet[3] );
		afit.SetPbj2( p4Jet[0] );
		afit.SetPbj1( p4Jet[1] );
		afit.Fit();
		kinfit_chi2[10] = afit.GetChi2();
		if ( kinfit_chi2[10] > fminKFchi2 ) {
			cfit[0] = afit.GetFitEj1()/p4Jet[2].E();
			cfit[1] = afit.GetFitEj2()/p4Jet[3].E();
			cfit[2] = afit.GetFitEbj2()/p4Jet[0].E();
			cfit[3] = afit.GetFitEbj1()/p4Jet[1].E();

			p4FitJet[2] = cfit[0]*p4Jet[2];
			p4FitJet[3] = cfit[1]*p4Jet[3];
			p4FitJet[0] = cfit[2]*p4Jet[0];
			p4FitJet[1] = cfit[3]*p4Jet[1];
			
			fitTop = p4FitJet[2] + p4FitJet[3] + p4Jet[0];
			h1["threebody_kinfit"]->Fill(fitTop.M() );
		}
		
		
		afit.SetPj1(  p4Jet[2] );
		afit.SetPj2(  p4Jet[3] );
		afit.SetPbj2( p4Jet[1] );
		afit.SetPbj1( p4Jet[0] );
		afit.Fit();
		kinfit_chi2[11] = afit.GetChi2();
		if ( kinfit_chi2[11] > fminKFchi2 ) {
			cfit[0] = afit.GetFitEj1()/p4Jet[2].E();
			cfit[1] = afit.GetFitEj2()/p4Jet[3].E();
			cfit[2] = afit.GetFitEbj2()/p4Jet[1].E();
			cfit[3] = afit.GetFitEbj1()/p4Jet[0].E();

			p4FitJet[2] = cfit[0]*p4Jet[2];
			p4FitJet[3] = cfit[1]*p4Jet[3];
			p4FitJet[1] = cfit[2]*p4Jet[1];
			p4FitJet[0] = cfit[3]*p4Jet[0];
			
			fitTop = p4FitJet[2] + p4FitJet[3] + p4Jet[1];
			h1["threebody_kinfit"]->Fill(fitTop.M() );
		}


		
		double minchi2 = 99999.;
		int themin = 99999;
		for (int i=0; i<12; i++ ) {

			if (kinfit_chi2[i] < minchi2 ) {
				minchi2 = kinfit_chi2[i];
				themin = i;
			}
		}
		
		h1["kinfit_chi2"]->Fill( TMath::Prob(minchi2, 3) );
								 if ( jentry < 3 ) std::cout << " minchi2= " << minchi2 << " prob= " << TMath::Prob(minchi2,3) << std::endl;
		
	} // end main loop
	
	
	std::cout << " Total entries = " << fChain->GetEntries() << std::endl;

	//____________ PLOTS ___________________________________

	std::string hname = "allmuon_pt";
	std::string cvname = "cv_"+hname;
	cv_map[cvname] = new TCanvas(cvname.c_str(),cvname.c_str(),700,700);
	h1[hname]->SetXTitle(h1[hname]->GetTitle());
	h1[hname]->Draw();
	h1[hname+"_MC"]->SetLineStyle(2);
	h1[hname+"_MC"]->Draw("same");

	//______________________________________________________
	hname = "allmuon_normchi2";
	cvname = "cv_"+hname;
	cv_map[cvname] = new TCanvas(cvname.c_str(),cvname.c_str(),700,700);
	h1[hname]->SetXTitle(h1[hname]->GetTitle());
	h1[hname]->Draw();
	h1[hname+"_MC"]->SetLineStyle(2);
	h1[hname+"_MC"]->Draw("same");

    //______________________________________________________
	hname = "allmuon_d0";
	cvname = "cv_"+hname;
	cv_map[cvname] = new TCanvas(cvname.c_str(),cvname.c_str(),700,700);
	h1[hname]->SetXTitle(h1[hname]->GetTitle());
	h1[hname]->Draw();
	h1[hname+"_MC"]->SetLineStyle(2);
	h1[hname+"_MC"]->Draw("same");
	
    //______________________________________________________
	hname = "allmuon_ips";
	cvname = "cv_"+hname;
	cv_map[cvname] = new TCanvas(cvname.c_str(),cvname.c_str(),700,700);
	h1[hname]->SetXTitle(h1[hname]->GetTitle());
	h1[hname]->Draw();
	h1[hname+"_MC"]->SetLineStyle(2);
	h1[hname+"_MC"]->Draw("same");

	//______________________________________________________
	hname = "njets";
	cvname = "cv_"+hname;
	cv_map[cvname] = new TCanvas(cvname.c_str(),cvname.c_str(),700,700);
	h1[hname]->SetXTitle(h1[hname]->GetTitle());
	h1[hname]->Draw();

    //______________________________________________________
	hname = "nleptons";
	cvname = "cv_"+hname;
	cv_map[cvname] = new TCanvas(cvname.c_str(),cvname.c_str(),700,700);
	h1[hname]->SetXTitle(h1[hname]->GetTitle());
	h1[hname]->Draw();
	
	
	//______________________________________________________
	cv_map["cv_twobody"] = new TCanvas("cv_twobody","cv_twobody",700,700);
	h1["twobody"]->SetXTitle(h1["twobody"]->GetTitle());
	h1["twobody"]->Draw();
	//h1["twobody_corr"]->SetLineColor(2);
	//h1["twobody_corr"]->Draw("same");
	
	//______________________________________________________
	cv_map["cv_threebody"] = new TCanvas("cv_threebody","cv_threebody",700,700);
	h1["threebody"]->SetXTitle(h1["threebody"]->GetTitle());
	h1["threebody"]->Draw();
	//h1["threebody_corr"]->SetLineColor(2);
	//h1["threebody_corr"]->Draw("same");
	
	//______________________________________________________
	cv_map["cv_threebody_b"] = new TCanvas("cv_threebody_b","cv_threebody_b",700,700);
	h1["threebody"]->SetXTitle(h1["threebody"]->GetTitle());
	h1["threebody"]->Draw();
	h1["threebody_1b"]->SetLineColor(3);
	h1["threebody_1b"]->Draw("same");
	h1["threebody_2b"]->SetLineColor(4);
	h1["threebody_2b"]->Draw("same");
	

	//______________________________________________________
	cv_map["cv_MET"] = new TCanvas("cv_MET","cv_MET",700,700);
	h1["MET"]->SetXTitle(h1["MET"]->GetTitle());
	h1["MET"]->Draw();

	h1["METcomplex"]->SetLineColor(4);
	h1["METcomplex"]->Draw("same");
	std::cout << " total entries MET = " << h1["MET"]->GetEntries() << std::endl;
	std::cout << " total entries MET complex = " << h1["METcomplex"]->GetEntries() << std::endl;
	//h1["MET_corr"]->SetLineColor(2);
	//h1["MET_corr"]->Draw("same");

	
	//______________________________________________________
	//cv_map["cv_METcomplex"] = new TCanvas("cv_METcomplex","cv_METcomplex",700,700);
	//h1["METcomplex"]->SetXTitle(h1["METcomplex"]->GetTitle());
	//h1["METcomplex"]->Draw();
	
	//______________________________________________________
	cv_map["cv_nu_pz"] = new TCanvas("cv_nu_pz","cv_nu_pz",700,700);
	h1["nu_pz"]->SetXTitle(h1["nu_pz"]->GetTitle());
	h1["nu_pz"]->Draw();

	//______________________________________________________
	cv_map["cv_mu_pt"] = new TCanvas("cv_mu_pt","cv_mu_pt",700,700);
	h1["muon_pt"]->SetXTitle(h1["muon_pt"]->GetTitle());
	h1["muon_pt"]->Draw();

	//______________________________________________________
	cv_map["cv_mu_nu_jet"] = new TCanvas("cv_mu_nu_jet","cv_mu_nu_jet",700,700);
	h1["mu_nu_jet"]->SetXTitle(h1["mu_nu_jet"]->GetTitle());
	h1["mu_nu_jet"]->Draw();

	//______________________________________________________
	cv_map["cv_mu_nu"] = new TCanvas("cv_mu_nu","cv_mu_nu",700,700);
	h1["mu_nu"]->SetXTitle(h1["mu_nu"]->GetTitle());
	h1["mu_nu"]->Draw();

	//______________________________________________________
        cv_map["cv_phiWb"] = new TCanvas("cv_phiWb","cv_phiWb",700,700);
        h1["phi_Wb"]->SetXTitle(h1["phi_Wb"]->GetTitle());
        h1["phi_Wb"]->Draw();


	//______________________________________________________
	cv_map["cv_scatter_0"] = new TCanvas("cv_scatter_0","cv_scatter_0",700,700);
	//h2["scatter_plot_0"]->SetXTitle();
	h2["scatter_plot_0"]->Draw("colorz");
	double scat_max = h2["scatter_plot_0"]->GetMaximum();

	//______________________________________________________
	cv_map["cv_scatter_1"] = new TCanvas("cv_scatter_1","cv_scatter_1",700,700);
	//h1["scatter_plot_1"]->SetXTitle();
	h2["scatter_plot_1"]->SetMaximum(scat_max);
	h2["scatter_plot_1"]->Draw("colorz");

	//______________________________________________________
	cv_map["cv_twobody_cut1"] = new TCanvas("cv_twobody_cut1","cv_twobody_cut1",700,700);
	h1["twobody_cut1"]->SetXTitle(h1["twobody_cut1"]->GetTitle());
	h1["twobody_cut1"]->Draw();
	//h1["twobody_corr_cut1"]->SetLineColor(2);
	//h1["twobody_corr_cut1"]->Draw("same");
	
	//______________________________________________________
        cv_map["cv_twobody_cut2"] = new TCanvas("cv_twobody_cut2","cv_twobody_cut2",700,700);
        h1["twobody_cut2"]->SetXTitle(h1["twobody_cut2"]->GetTitle());
        h1["twobody_cut2"]->Draw();

	//______________________________________________________
        cv_map["cv_twobody_cut21"] = new TCanvas("cv_twobody_cut21","cv_twobody_cut21",700,700);
        h1["twobody_cut21"]->SetXTitle(h1["twobody_cut21"]->GetTitle());
        h1["twobody_cut21"]->Draw();

	//______________________________________________________
	cv_map["cv_threebody_cut1"] = new TCanvas("cv_threebody_cut1","cv_threebody_cut1",700,700);
	h1["threebody_cut1"]->SetXTitle(h1["threebody"]->GetTitle());
	h1["threebody_cut1"]->Draw();
	//h1["threebody_corr_cut1"]->SetLineColor(2);
	//h1["threebody_corr_cut1"]->Draw("same");

	//______________________________________________________
        cv_map["cv_threebody_cut2"] = new TCanvas("cv_threebody_cut2","cv_threebody_cut2",700,700);
        h1["threebody_cut2"]->SetXTitle(h1["threebody"]->GetTitle());
        h1["threebody_cut2"]->Draw();
	
	//______________________________________________________
	cv_map["cv_kinfit_chi2"] = new TCanvas("cv_kinfit_chi2","cv_kinfit_chi2",700,700);
	
	h1["kinfit_chi2"]->SetXTitle(h1["kinfit_chi2"]->GetTitle() );
	h1["kinfit_chi2"]->Draw();

	//______________________________________________________
	cv_map["cv_threebody_kinfit"] = new TCanvas("cv_threebody_kinfit","cv_threebody_kinfit",700,700);
	h1["threebody_kinfit"]->SetXTitle(h1["threebody_kinfit"]->GetTitle());
	h1["threebody_kinfit"]->Draw();
}
