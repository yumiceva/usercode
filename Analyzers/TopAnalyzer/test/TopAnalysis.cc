/**_________________________________________________________________
   class:   TopAnalysis.cc
   package: Analyzer/TopAnalyzer


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopAnalysis.cc,v 1.4 2007/05/31 21:25:08 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/TopAnalyzer/test/TopAnalysis.h"
#include "Analyzers/TopTools/interface/TopBJet.h"
#include "Analyzers/TopTools/interface/TopCombos.h"


bool SortPt( TLorentzVector p4l, TLorentzVector p4r) {

	return p4l.Pt() > p4r.Pt();
}
bool SortBjets( TopBJet b1, TopBJet b2) {

	return b1.GetDiscriminator() > b2.GetDiscriminator(); 
}


//_______________________________________________________________
TopAnalysis::TopAnalysis(TString filename) {

	fverbose     = false;
	fcorrections = true;
	fkinfit      = false;
	fminKFchi2   = 0.;
	fphicut      = 0.;
	
	TChain *tree = new TChain("summary");
	tree->Add(filename);
	
	fevent = new TopEvent();
	
	Init(tree);

	// initialize cuts
	cut_map["cut0"] = "Initial selection";
	cut_map["cut1"] = "Min Muon pt";
	cut_map["cut2"] = "Min Jet Et";
	cut_map["cut3"] = "Min MET";
	cut_map["cut4"] = "";
	
	// initialize histograms
	h_ = new TopHistograms();

	for (std::map<TString, TString>::const_iterator imap=cut_map.begin();
		 imap!=cut_map.end(); ++imap) {

		TString acut = imap->first;
		
		h_->Init("jets",acut);
		h_->Init("jets",acut,"MC");
		h_->Init("muons",acut);
		h_->Init("muons",acut,"MC");
		h_->Init("MET",acut);
		h_->Init("MET",acut,"MC");
		h_->Init("others",acut);
		h_->Init("others",acut,"MC");
	}
	
}

//_______________________________________________________________
TopAnalysis::~TopAnalysis() {
	if (!fChain) return;
	delete fChain->GetCurrentFile();
}

//_______________________________________________________________
void TopAnalysis::Loop(int max_entry) {
	
	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();
	if (fverbose) std::cout << " Total entries = " << fChain->GetEntries() << std::endl;
	if (max_entry!= 0) {
		nentries = (Long64_t) max_entry;
		std::cout << " run over a maximum of "<< nentries << std::endl;
	}
	
	//______ main loop over entries______
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		
		if ( fverbose ) { std::cout << " processing entry: " << jentry << std::endl; }
		else if ( jentry%100000 == 0 ) { std::cout << " processing entry: " << jentry << std::endl; }

		// muons
		bool goodMuon = false;
		TLorentzVector p4Muon;

		std::vector< TLorentzVector > tmpmupt;
		int muon_size = fevent->nleptons;
		if (fverbose) std::cout << "number of muons = " << muon_size << std::endl;
		h_->Fill1d(TString("muons")+"_"+"cut0",muon_size);

		for (int imu = 0; imu != muon_size; ++imu) {

			double pt_mu = fevent->allmuon_pt[imu];
			double normchi2_mu = fevent->allmuon_normchi2[imu];
			
			h_->Fill1d(TString("muon_pt")+"_"+"cut0", pt_mu);
			h_->Fill1d(TString("muon_normchi2")+"_"+"cut0",normchi2_mu);

			
		}
		// pt>10
		muon_size = fevent->muon_px.size();
		if (fverbose) std::cout << "number of muons pt>10 = " << muon_size << std::endl;
		if (muon_size == 0 ) continue;
		
		for (int imu = 0; imu != muon_size; ++imu) {
			TLorentzVector tmpp4(fevent->muon_px[imu],fevent->muon_py[imu],fevent->muon_pz[imu],fevent->muon_e[imu]);
			h_->Fill1d(TString("muon_pt")+"_"+"cut1", tmpp4.Pt());
			//h_->Fill1d("muons_normchi2"+"_"+"cut1",normchi2_mu);
			tmpmupt.push_back(tmpp4);
			
		} // end muon loop

		std::sort( tmpmupt.begin(), tmpmupt.end(), SortPt );//sort muons by pt

		p4Muon = tmpmupt[0];

		if (fverbose) {
			std::cout << "Muon 0th (" << p4Muon.Px() <<","<< p4Muon.Py() <<","<< p4Muon.Pz() <<","<<p4Muon.E()<<")"<<std::endl;
			if (muon_size>1) std::cout << "Muon 1th (" << tmpmupt[1].Px() <<","<< tmpmupt[1].Py() <<","<< tmpmupt[1].Pz() <<","<< tmpmupt[1].E()<<")"<<std::endl;
		}
		tmpmupt.clear();

		// MET
		TLorentzVector p4MET;
		
		// jets
		int njets =0;
		TLorentzVector p4Jet[4];
		TopBJet abjet[4];
		std::vector< TopBJet > tmpbvec;
		bool is_bjet[4] = {false};
		int nbjets = 0;
		
		int jet_size = fevent->njets;
		h_->Fill1d(TString("jets")+"_"+"cut0",jet_size);

		for (int ijet = 0; ijet != jet_size; ++ijet) {

			double corr = 1.;
			if (fcorrections) corr = fevent->jet_correction[ijet];

			p4MET += TLorentzVector(corr*fevent->alljet_px[ijet],
									corr*fevent->alljet_py[ijet],
									0.,
									0.);
			
			h_->Fill1d(TString("jet_et")+"_"+"cut0",fevent->alljet_et[ijet]);
			h_->Fill1d(TString("jet_eta")+"_"+"cut0",fevent->alljet_eta[ijet]);
			h_->Fill1d(TString("jet_phi")+"_"+"cut0",fevent->alljet_phi[ijet]);

			// select leading 4 jets
			if ( njets < 4 ) {
				p4Jet[njets] = TLorentzVector(corr*fevent->alljet_px[ijet],
											  corr*fevent->alljet_py[ijet],
											  corr*fevent->alljet_pz[ijet],
											  corr*fevent->alljet_e[ijet]);

				// b taggingig
				abjet[njets].SetDiscriminator(fevent->jet_btag_discriminant[ijet]);
				abjet[njets].SetId(njets);
				tmpbvec.push_back(abjet[njets]);
								
				njets++;
			}
		}// end jet loop
		
		// tag the best two b jets
		std::sort( tmpbvec.begin(), tmpbvec.end(), SortBjets);
		if (fverbose) {
			for (unsigned int i=0; i!=tmpbvec.size(); ++i) {
				std::cout << "b discriminant for jet" << i << " =" << tmpbvec[i].GetDiscriminator()
						  << " id= " << tmpbvec[i].GetId() << std::endl;
			}
		}
		if ( tmpbvec[0].GetDiscriminator() > 4.0 ) {
			is_bjet[tmpbvec[0].GetId()] = true;
		}
		if ( tmpbvec[1].GetDiscriminator() > 4.0 ) is_bjet[tmpbvec[1].GetId()] = true;
		
		// add muon to MET
		p4MET += TLorentzVector(p4Muon.Px(), p4Muon.Py(), 0., 0.);
		// fix MET sign
		p4MET = (-1.)*p4MET;

		h_->Fill1d(TString("MET")+"_"+"cut0",p4MET.Pt());

		// solve for W -> l + nu
		TopWSolver wsolver;
		wsolver.SetP4Muon( p4Muon );
		wsolver.SetP4MET( p4MET );
		
		double pzNu = wsolver.GetPzNu();
		if ( wsolver.IsComplex() ) {
			h_->Fill1d(TString("METcomplex")+"_"+"cut0",p4MET.Pt());
		}
		h_->Fill1d(TString("nu_pz")+"_"+"cut0", pzNu );
		TLorentzVector p4Nu(p4MET.Px(), p4MET.Py(), pzNu,
							TMath::Sqrt(p4MET.Px()*p4MET.Px()+p4MET.Py()*p4MET.Py()+pzNu*pzNu) );

		TLorentzVector p4Wlnu = p4Muon + p4Nu;
		h_->Fill1d(TString("WToMuNu")+"_"+"cut0",p4Wlnu.M() );
		
		// Combinations
		TopCombos WTojj;//  = new TopTwoComb(); // W -> j + j
		TopCombos tToWj; //  = new TopTwoComb(); // t -> W(j+j) + j

		// jets
		std::vector< TLorentzVector > cand1;
		// light jets
		std::vector< TLorentzVector > candq;
		// b-jets
		std::vector< TLorentzVector > candb;
		for (int i=0; i<4; ++i ) {
			cand1.push_back( p4Jet[i] );
			//cand2.push_back( p4Jet[i] );
			if ( is_bjet[i] ) candb.push_back( p4Jet[i] );
			else candq.push_back( p4Jet[i] );
		}
		if (fverbose) {
			std::cout << "njets in cand1="<< cand1.size() << std::endl;
			std::cout << "nbjets= " << candb.size() << std::endl;
			std::cout << "nlight= " << candq.size() << std::endl;
		}
		
		WTojj.SetCandidate1( cand1 );
		WTojj.SetCandidate2( cand1 );

		std::vector< TLorentzVector > candWTojj;
		candWTojj = WTojj.GetComposites();
		if (fverbose) std::cout << "WTojj combinations = "<< candWTojj.size() << std::endl;
		for (unsigned int i=0; i!= candWTojj.size(); ++i) {
			h_->Fill1d(TString("WTojj")+"_"+"cut0",candWTojj[i].M() );
		}

		tToWj.SetCandidate1( cand1 ); // j1 of t-> W + j3, W-> j1 + j2
		tToWj.SetCandidate2( cand1 ); // j2
		tToWj.SetCandidate3( cand1 ); // j3
		std::vector< TLorentzVector > candtToWj;
		candtToWj = tToWj.GetComposites();
		if (fverbose) std::cout << "tToWj combinations = " << candtToWj.size() << std::endl;
		for (unsigned int i=0; i!= candtToWj.size(); ++i) {
			h_->Fill1d(TString("tToWj")+"_"+"cut0",candtToWj[i].M() );
		}

		// b-tagging
		TopCombos WTojj_nob;
		TopCombos tToWb;

		WTojj_nob.SetCandidate1( candq );
		WTojj_nob.SetCandidate2( candq );
		std::vector< TLorentzVector > candWTojj_nob;
		candWTojj_nob = WTojj_nob.GetComposites();
		if (fverbose) std::cout << "WTojj_nob combinations = "<< candWTojj_nob.size() << std::endl;
		for (unsigned int i=0; i!= candWTojj_nob.size(); ++i) {
			h_->Fill1d(TString("WTojj_nob")+"_"+"cut0",candWTojj_nob[i].M() );
		}

		tToWb.SetCandidate1( candq ); // j1 of t-> W + b, W -> j1 + j2
		tToWb.SetCandidate2( candq ); // j2
		tToWb.SetCandidate3( candb ); // b
		std::vector< TLorentzVector > candtToWb;
		candtToWb = tToWb.GetComposites();
		if (fverbose) std::cout << "tToWb combinations = " << candtToWj.size() << std::endl;
		for (unsigned int i=0; i!= candtToWb.size(); ++i) {
			h_->Fill1d(TString("tToWb")+"_"+"cut0",candtToWj[i].M() );
		}
		
		
		
	} // end main loop

	std::cout << " Total number of entries = " << fChain->GetEntries() << std::endl;
	
}


//_______________________________________________________________
Int_t TopAnalysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}

//_______________________________________________________________
Long64_t TopAnalysis::LoadTree(Long64_t entry)
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

//_______________________________________________________________
void TopAnalysis::Init(TChain *tree)
{

	if (!tree) return;

	fChain = tree;
	fCurrent = -1;
	
	fChain->SetBranchAddress("top.",&fevent);
	
}

//_______________________________________________________________
Bool_t TopAnalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

//_______________________________________________________________
void TopAnalysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

//_______________________________________________________________
Int_t TopAnalysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

//_______________________________________________________________
void TopAnalysis::Print(std::string extension,std::string tag) {

	h_->Print(extension, tag );
	
}


//_______________________________________________________________
void TopAnalysis::SaveToFile(TString filename) {

	h_->SaveToFile(filename);
	
}

