/**_________________________________________________________________
   class:   TopZpAnalysis.cc
   package: Analyzer/TopAnalyzer


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopZpAnalysis.cc,v 1.1 2007/11/03 00:00:27 yumiceva Exp $

________________________________________________________________**/


#include "Analyzers/TopAnalyzer/test/TopZpAnalysis.h"
#include "Analyzers/TopTools/interface/TopBJet.h"
#include "Analyzers/TopTools/interface/TopCombos.h"
#include "Analyzers/TopTools/interface/TopComposite.h"
#include "Analyzers/TopTools/interface/TopPair.h"
#include "Math/GenVector/VectorUtil.h"

bool SortPt( TLorentzVector p4l, TLorentzVector p4r) {

	return p4l.Pt() > p4r.Pt();
}
bool SortBjets( TopBJet b1, TopBJet b2) {

	return b1.GetDiscriminator() > b2.GetDiscriminator(); 
}
bool SortbyChi2( TopComposite c1, TopComposite c2) {

	return c1.GetKFChi2() < c2.GetKFChi2();
}
bool SortbyChi2P( TopPair c1, TopPair c2) {

	return c1.GetKFChi2() < c2.GetKFChi2();
}

//_______________________________________________________________
TopZpAnalysis::TopZpAnalysis(TString filename) {

	fverbose     = false;
	fcorrections = true;
	fkinfit      = false;
	ffilter      = true;
	fPartonMatching= false;
	ftopo        = true;
	fminKFchi2   = 0.;
	fphicut      = 0.;
	fweight      = 1.;
	fsample      = -1;
	fscale       = 1.;
	
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
	h_->Init("generator");

	// resolutions
	ResJets_ = new GetResolutions("../../../TopQuarkAnalysis/TopObjectResolutions/data/Resolutions_lJets_MCJetCorJetIcone5.root");
	ResbJets_ = new GetResolutions("../../../TopQuarkAnalysis/TopObjectResolutions/data/Resolutions_bJets_MCJetCorJetIcone5.root");
	ResMuons_ = new GetResolutions("../../../TopQuarkAnalysis/TopObjectResolutions/data/Resolutions_muon.root");
	ResMET_ = new GetResolutions("../../../TopQuarkAnalysis/TopObjectResolutions/data/Resolutions_met.root");
}

//_______________________________________________________________
TopZpAnalysis::~TopZpAnalysis() {
  
  //  delete h_;
  delete ResJets_;
  delete ResbJets_;
  delete ResMuons_;
  delete ResMET_;
	if (!fChain) return;
	delete fChain->GetCurrentFile();
}

//_______________________________________________________________
void TopZpAnalysis::Loop(int max_entry) {
	
	if (fChain == 0) return;


	// w+jet cross sections in pb
	double xsec[5] = {
		30000.0,
		8000.0,
		2500.0,
		722.0,
		174.0 };

	//if ( fweight != 1. && fsample != -1 ) fweight = xsec[fsample]/fweight;
	
	Long64_t nentries = fChain->GetEntriesFast();
	std::cout << " Total entries = " << fChain->GetEntries() << std::endl;
	if (max_entry!= 0) {
		nentries = (Long64_t) max_entry;
		std::cout << " run over a maximum of "<< nentries << std::endl;
	}

	int npassevents = 0;
	
	//______ main loop over entries______
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		
		if ( fverbose ) { std::cout << " processing entry: " << jentry << std::endl; }
		else if ( jentry%5000 == 0 ) { std::cout << " processing entry: " << jentry << std::endl; }

		// check filter
		if ( ffilter && ( !fevent->passfilter) ) continue; 

		npassevents++;
		
		// generator
		TLorentzVector gent;
		TLorentzVector genT;
		TLorentzVector genTopPair;
		TLorentzVector genjetl[2];
		TLorentzVector genjetb1;
		
		for (unsigned int itop =0; itop !=fevent->gentop_charge.size(); ++itop) {
			if (fevent->gentop_charge[itop] < 0 ) genT.SetXYZT(fevent->gentop_px[itop],fevent->gentop_py[itop],fevent->gentop_pz[itop],fevent->gentop_e[itop]);
			else  gent.SetXYZT(fevent->gentop_px[itop],fevent->gentop_py[itop],fevent->gentop_pz[itop],fevent->gentop_e[itop]);
		}
		genTopPair = gent + genT;
		h_->Fill2d(TString("gentop_rapidities"), gent.Rapidity(), genT.Rapidity() );
		h_->Fill1d(TString("gen_toppair_mass"), genTopPair.M() );
		int tmpigenl = 0;
		int tmpgenmother = 0;
		for ( size_t igen = 0; igen != fevent->genjet_pz.size(); ++igen ) {

			if ( std::abs(fevent->genjet_pdg[igen]) < 5 ) {
				genjetl[tmpigenl].SetPxPyPzE( fevent->genjet_px[igen],fevent->genjet_py[igen],fevent->genjet_pz[igen],fevent->genjet_e[igen] );
				tmpgenmother = fevent->genjet_mother[igen];
				tmpigenl++;
			}
		}
		for ( size_t igen = 0; igen != fevent->genjet_pz.size(); ++igen ) {

			if ( std::abs(fevent->genjet_pdg[igen]) == 5 || fevent->genjet_mother[igen] == tmpgenmother  ) {
				genjetb1.SetPxPyPzE( fevent->genjet_px[igen],fevent->genjet_py[igen],fevent->genjet_pz[igen],fevent->genjet_e[igen] );
			}
		}
		double gendeltar = ROOT::Math::VectorUtil::DeltaR( genjetl[0].Vect(), genjetl[1].Vect() );
		h_->Fill1d(TString("gen_deltaR_light"), gendeltar );
		gendeltar = ROOT::Math::VectorUtil::DeltaR( genjetl[0].Vect(), genjetb1.Vect() );
		h_->Fill1d(TString("gen_deltaR_b"), gendeltar );
		gendeltar = ROOT::Math::VectorUtil::DeltaR( genjetl[1].Vect(), genjetb1.Vect() );
		h_->Fill1d(TString("gen_deltaR_b"), gendeltar );
					
		
		// muons
		bool goodMuon = false;
		TLorentzVector p4Muon;

		std::vector< TLorentzVector > tmpmupt;
		int muon_size = fevent->nleptons;
		if (fverbose) std::cout << "number of muons = " << muon_size << std::endl;
		if (muon_size == 0 ) continue;
		
		h_->Fill1d(TString("muons")+"_"+"cut0",muon_size);

		for (int imu = 0; imu != muon_size; ++imu) {
			TLorentzVector tmpp4(fevent->muon_px[imu],fevent->muon_py[imu],fevent->muon_pz[imu],fevent->muon_e[imu]);
			tmpmupt.push_back(tmpp4);
			double pt_mu = tmpp4.Pt();
			double normchi2_mu = fevent->muon_normchi2[imu];
			
			h_->Fill1d(TString("muon_pt")+"_"+"cut0", pt_mu);
			h_->Fill1d(TString("muon_normchi2")+"_"+"cut0",normchi2_mu);
			if (TMath::Abs(tmpp4.Eta())< 2.4) h_->Fill1d(TString("muon_pt")+"_"+"cut1", pt_mu);
			if (fverbose) std::cout << "done part one " << std::endl;
			
			int muid = fevent->muon_mc_pdgid[imu];
			if ( TMath::Abs(muid) == 13 ) {
				h_->Fill1d(TString("muon_pt_cut0_MC"), pt_mu );
				h_->Fill1d(TString("muon_normchi2_cut0_MC"), normchi2_mu);
			}
			if (fverbose) std::cout << "done part two " << std::endl;
		}
		
		std::sort( tmpmupt.begin(), tmpmupt.end(), SortPt );//sort muons by pt

		// select muon with highest pt
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
		int nmatchedjets = 0;
		std::map< int, double > deltaRGenP4Map;//
		
		int jet_size = fevent->njets;
		h_->Fill1d(TString("jets")+"_"+"cut0",jet_size);
		std::vector< TLorentzVector > tmpjetpt;
		for (int ijet = 0; ijet != jet_size; ++ijet) {

			TLorentzVector tmpJet;
			TLorentzVector tmpcorrJet;
			double corr = 1.;
			if (fcorrections) {
				corr = fevent->jet_correction[ijet];
				corr = corr * fscale;
			}
			
			tmpJet.SetXYZT(fevent->jet_px[ijet],fevent->jet_py[ijet],fevent->jet_pz[ijet],fevent->jet_e[ijet]);
			tmpcorrJet = corr*tmpJet;
			
			p4MET += TLorentzVector(tmpcorrJet.Px(), tmpcorrJet.Py(),0.,0.);
			
			h_->Fill1d(TString("jet_et")+"_"+"cut0",tmpcorrJet.Et() );
			h_->Fill1d(TString("jet_eta")+"_"+"cut0",tmpcorrJet.Eta() );
			h_->Fill1d(TString("jet_phi")+"_"+"cut0",tmpcorrJet.Phi() );

			tmpjetpt.push_back( tmpcorrJet );

			// jet-paton matching
			float bestDR = 9999.;
			int bestParton = -1;
			bool isJetInMCmap = false;
			bool aBetterMatch = false;
			int oldmatch = -1;
			for (unsigned int itop =0; itop !=fevent->genjet_px.size(); ++itop) {
			  TLorentzVector tmpgenT;
				tmpgenT.SetXYZT(fevent->genjet_px[itop],fevent->genjet_py[itop],fevent->genjet_pz[itop],fevent->genjet_e[itop]);
				double delta  = ROOT::Math::VectorUtil::DeltaR(tmpgenT.Vect(), tmpcorrJet.Vect() );
				if ( delta < bestDR && (fevent->jet_flavour[ijet]==std::abs(fevent->genjet_pdg[itop])) && delta < 0.5 ) {
					bestDR =delta;
					bestParton = itop;
				}
			}
			if (bestParton != -1 ) {
			  nmatchedjets++;
			  double alreadybestDR = 9999.;
			  int tmpith = 0;
			  for( std::map<int, double>::const_iterator igenmap=deltaRGenP4Map.begin(); igenmap!=deltaRGenP4Map.end(); ++ igenmap ) {
			    if ( igenmap->first == bestParton ) {
			      isJetInMCmap = true;
			      alreadybestDR = igenmap->second;
			      oldmatch = tmpith;
			    }
			    tmpith++;
			  }
			  if (!isJetInMCmap) deltaRGenP4Map[bestParton] = bestDR;
			  else if ( bestDR < alreadybestDR ) {
			    // we have a better match
			    aBetterMatch = true;
			    deltaRGenP4Map[bestParton] = bestDR;
			  }
			  
			}
			if (fverbose) {
				TLorentzVector tmpgenT;
				if (bestParton != -1) tmpgenT.SetXYZT(fevent->genjet_px[bestParton],fevent->genjet_py[bestParton],fevent->genjet_pz[bestParton],fevent->genjet_e[bestParton]);
				std::cout << " matching parton: deltaR= " << bestDR << std::endl;
				if (bestParton != -1 ) {
					std::cout << "   jet: pt= " << tmpcorrJet.Pt() << " flavor= " << fevent->jet_flavour[ijet] << std::endl;
					std::cout << "parton: pt= " << tmpgenT.Pt() << " pdgid= " << fevent->genjet_pdg[bestParton] << std::endl;
					std::cout << " a better match? "<< aBetterMatch << std::endl;
				} else {
					std::cout << "   jet: pt= " << tmpcorrJet.Pt() << " flavor= " << fevent->jet_flavour[ijet] << std::endl;
					std::cout << " no matched parton " << std::endl;
				}
			}
			if ( fPartonMatching && (bestParton != -1 ) ) {
			  if ( aBetterMatch) {
			    // replace jet
			    if (fverbose) std::cout << " jet # " << oldmatch << " will be replaced, njets = " << njets<< std::endl;
			    p4Jet[oldmatch] = tmpcorrJet;
			    abjet[oldmatch].SetDiscriminator(fevent->jet_btag_discriminant[ijet]);
			    abjet[oldmatch].SetId(oldmatch);
			    tmpbvec.push_back(abjet[oldmatch]);
			  }
			  else if (!isJetInMCmap) {
			    if ( njets> 3 ) { std::cout << "more than 4 jets, continue" << std::endl; continue;}
				p4Jet[njets] = tmpcorrJet;
				//if (fevent->jet_flavour[ijet]==5) {
				abjet[njets].SetDiscriminator(fevent->jet_btag_discriminant[ijet]);
				abjet[njets].SetId(njets);
				tmpbvec.push_back(abjet[njets]);
				//}

			  }				//std::cout << " test matching!" << " njets= " << njets <<  std::endl;
			  
			  if (!aBetterMatch) {
			    if ( njets> 3 ) { std::cout << "more than 4 jets, continue" << std::endl; continue;}
				h_->Fill1d(TString("jet_et")+"_"+"cut1_MC", p4Jet[njets].Pt());
				h_->Fill1d(TString("jet_eta")+"_"+"cut1_MC", p4Jet[njets].Eta());
				h_->Fill1d(TString("jet_phi")+"_"+"cut1_MC", p4Jet[njets].Phi());
				h_->Fill2d(TString("jet_res_et")+"_"+"cut1_MC", p4Jet[njets].Pt(), ResJets_->getObsRes(6, ResJets_->getEtaBin( p4Jet[njets].Eta() ) , p4Jet[njets].Et() ) );
				njets++;
			  }
			}
			// select 4 leading jets and et > 30
			else if ( !fPartonMatching && (njets < 4) ) {

				//p4MET += TLorentzVector(tmpcorrJet.Px(), tmpcorrJet.Py(),0.,0.);
				
				p4Jet[njets] = tmpcorrJet;

				h_->Fill1d(TString("jet_et")+"_"+"cut1", p4Jet[njets].Pt());
				h_->Fill1d(TString("jet_eta")+"_"+"cut1", p4Jet[njets].Eta());
				h_->Fill2d(TString("jet_res_et")+"_"+"cut1", p4Jet[njets].Pt(), ResJets_->getObsRes(6, ResJets_->getEtaBin( p4Jet[njets].Eta() ) , p4Jet[njets].Et() ) );
				// b taggingig
				abjet[njets].SetDiscriminator(fevent->jet_btag_discriminant[ijet]);
				abjet[njets].SetId(njets);
				tmpbvec.push_back(abjet[njets]);
								
				njets++;
			}
		}// end jet loop
		
		//std::sort( tmpjetpt.begin(), tmpjetpt.end(), SortPt );//sort jets by pt
		tmpjetpt.clear();

		if ( fPartonMatching && ( nmatchedjets != 4 ) ) continue;
		
		// tag the best two b jets
		std::sort( tmpbvec.begin(), tmpbvec.end(), SortBjets);
		if (fverbose) {
			for (unsigned int i=0; i!=tmpbvec.size(); ++i) {
				std::cout << "Jet " << i << " ("<< p4Jet[i].X() <<","<<p4Jet[i].Y()<<","<<p4Jet[i].Z()<<","<<p4Jet[i].T()<<")"<<std::endl;
				
				std::cout << "b discriminant for jet" << i << " =" << tmpbvec[i].GetDiscriminator()
						  << " id= " << tmpbvec[i].GetId() << std::endl;
			}
		}
		// discriminator was 7 ?
		if ( tmpbvec[0].GetDiscriminator() > 4.0 ) {
			is_bjet[tmpbvec[0].GetId()] = true;
		}
		if ( tmpbvec[1].GetDiscriminator() > 4.0 ) is_bjet[tmpbvec[1].GetId()] = true;

		for (int i=0; i<4; ++i) {
			if ( is_bjet[i] ) {
				h_->Fill1d(TString("jet_et")+"_"+"cut2",p4Jet[i].Pt());
				h_->Fill1d(TString("jet_eta")+"_"+"cut2",p4Jet[i].Eta());
			}
		}
		
		// add muon to MET
		p4MET += TLorentzVector(p4Muon.Px(), p4Muon.Py(), 0., 0.);
		// fix MET sign
		p4MET = (-1.)*p4MET;

		// Cut on MET
		if ( p4MET.Pt() <= 20. ) continue;
		
		h_->Fill1d(TString("MET")+"_"+"cut0",p4MET.Pt());

		for (unsigned int inu =0; inu !=fevent->gennu_px.size(); ++inu) {
			h_->Fill1d(TString("MET_cut0_MC"),TMath::Sqrt((fevent->gennu_px[inu] * fevent->gennu_px[inu])+
														  (fevent->gennu_py[inu] * fevent->gennu_py[inu])) );
			h_->Fill1d(TString("nu_pz_cut0_MC"),fevent->gennu_pz[inu]);
		}

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

		// Ht variable
		double Ht = p4MET.Pt() + p4Muon.Pt() + p4Jet[0].Pt() + p4Jet[1].Pt() + p4Jet[2].Pt() + p4Jet[3].Pt();
		h_->Fill1d(TString("Ht_cut0"), Ht);
		
		// Combinations
		TopCombos tToWlnuj; // t -> W + j, W -> mu + nu
		TopCombos WTojj;//  = new TopTwoComb(); // W -> j + j
		TopCombos tTojjj; // t -> j + j +j
		TopCombos tToWj; //  = new TopTwoComb(); // t -> W(j+j) + j
		TopCombos tToWjNo; // with duplicates
		TopCombos ZpTott; // Zp -> t tbar
		
		// leptonic W
		std::vector< TLorentzVector > candw;
		candw.push_back( p4Wlnu );
		// all jets
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

		tToWlnuj.SetCandidate1( candw  );
		tToWlnuj.SetCandidate2( cand1 );
		std::vector< TopComposite > candtToWlnuj;
		candtToWlnuj = tToWlnuj.GetComposites();
		if (fverbose) std::cout << "tToWlnuj combinations = "<< candtToWlnuj.size() << std::endl;
		for (unsigned int i=0; i!= candtToWlnuj.size(); ++i) {
			h_->Fill1d(TString("tToWlnuj")+"_"+"cut0",candtToWlnuj[i].M() );
		}

		if (ftopo) tToWlnuj.SetMinDeltaPhi(1.5);

		candtToWlnuj.clear();
		candtToWlnuj = tToWlnuj.GetComposites();
		
		for (unsigned int i=0; i!= candtToWlnuj.size(); ++i) {
			h_->Fill1d(TString("tToWlnuj")+"_"+"cut1",candtToWlnuj[i].M() );// muonic top with topological cut on opp. top
		}
		
		WTojj.SetCandidate1( cand1 );
		WTojj.SetCandidate2( cand1 );

		std::vector< TopComposite > candWTojj;
		std::vector< TopComposite > candWTojjNo;//not remove duplicates
		candWTojj = WTojj.GetComposites();
		
		if (fverbose) std::cout << "WTojj combinations = "<< candWTojj.size() << std::endl;
		for (unsigned int i=0; i!= candWTojj.size(); ++i) {
			h_->Fill1d(TString("WTojj")+"_"+"cut0",candWTojj[i].M() );
		}

		WTojj.Clear();
		WTojj.SetCandidate1( cand1 );
		WTojj.SetCandidate2( cand1 );
		WTojj.SetMinInvMass(60.);
		WTojj.SetMaxInvMass(130.);
		candWTojj.clear();
		candWTojj = WTojj.GetComposites();
		WTojj.RemoveDuplicates(false);
		candWTojjNo = WTojj.GetComposites();
		if (fverbose) {
			std::cout << "WTojj combinations = "<< candWTojj.size() << std::endl;
			std::cout << "WTojj combinations with duplicates= "<< candWTojjNo.size() << std::endl;
		}
		if ( candWTojj.size() > 0 ) {
			for (unsigned int i=0; i!= candWTojj.size(); ++i) {
				h_->Fill1d(TString("WTojj")+"_"+"cut1",candWTojj[i].M() ); // had. W with cuts on the mass
			}
		}
		
		tTojjj.SetCandidate1( cand1 ); // j1 of t-> W + j3, W-> j1 + j2
		tTojjj.SetCandidate2( cand1 ); // j2
		tTojjj.SetCandidate3( cand1 ); // j3
		std::vector< TopComposite > candtTojjj;
		candtTojjj = tTojjj.GetComposites();
		if (fverbose) std::cout << "tTojjj combinations = " << candtTojjj.size() << std::endl;
		for (unsigned int i=0; i!= candtTojjj.size(); ++i) {
			h_->Fill1d(TString("tTojjj")+"_"+"cut0",candtTojjj[i].M() );// hadronic top no cuts

			if (Ht>300.) h_->Fill1d(TString("tTojjj")+"_"+"cut1",candtTojjj[i].M() ); // had. top + Htcut
		}

		
		//for (unsigned int i=0; i!= candtToWlnuj.size(); ++i) {
		//	if ( (candtToWlnuj[i].M() > 140.) && (candtToWlnuj[i].M() < 210.) ) {
		//		h_->Fill1d(TString("tToWj")+"_"+"cut1",candtToWj[i].M() );// had. top with cut on opposite top mass
		//	}
		//}

		if ( candWTojj.size() > 0 ) {
			tToWj.SetCandidate1( candWTojj ); // with W only in selected mass window
			tToWj.SetCandidate2( cand1 );  // jet

			tToWjNo.SetCandidate1( candWTojj );
			tToWjNo.SetCandidate2( cand1 );
			tToWjNo.RemoveDuplicates(false);
			//tToWj.SetFilterList( WTojj.GetList() ); // filter
			std::vector< TopComposite > candtToWj;
			candtToWj = tToWj.GetComposites();
			if (fverbose) std::cout << "tToWj combinations = " << candtToWj.size() << std::endl;
			for (unsigned int i=0; i!= candtToWj.size(); ++i) {
				h_->Fill1d(TString("tToWj")+"_"+"cut0",candtToWj[i].M() );// hadronic top with W in mass window
			}

			ZpTott.SetCandidate1( candtToWj );
			ZpTott.SetCandidate2( tToWlnuj.GetVectors() );
			std::vector< TopComposite > candZpTott;
			candZpTott = ZpTott.GetComposites();
			for (unsigned int i=0; i!= candZpTott.size(); ++i) {
				h_->Fill1d(TString("topPair")+"_"+"cut0",candZpTott[i].M() );//
				h_->Fill1d(TString("topPairRes_cut0"), (candZpTott[i].M() - genTopPair.M())/genTopPair.M() );
			}
			
			// request back-to-back events in phi
			if (ftopo) tToWj.SetMinDeltaPhi( 1.5 );
			candtToWj.clear();
			if (fkinfit) {
				if (fverbose) std::cout << " setup kinfit" << std::endl;
				//tToWj.RemoveDuplicates(false);
				tToWj.RemoveDuplicates(true);
				tToWj.DoKinFit(); // kinematic fit
				tToWj.SetResJets( ResJets_ );
				tToWj.SetResbJets( ResbJets_ );
				tToWj.SetResMuons( ResMuons_ );
				tToWj.SetResMET( ResMET_ );
				tToWj.SetMuon( p4Muon );
				tToWj.SetNu( p4Nu );
			}
									
			candtToWj = tToWj.GetComposites();
			if (fverbose) std::cout << "tToWj combinations = " << candtToWj.size() << std::endl;
			std::vector< TopPair > pairtToWj = tToWj.GetFittedTops();
			if (fverbose) std::cout << "tToWj kin fit done" << std::endl;
			// sort by kinfit chi2
			std::sort( candtToWj.begin(), candtToWj.end(), SortbyChi2 );
			std::sort( pairtToWj.begin(), pairtToWj.end(), SortbyChi2P );
			
			for (unsigned int i=0; i!= candtToWj.size(); ++i) {
				h_->Fill1d(TString("tToWj")+"_"+"cut1",candtToWj[i].M() );// had. top with W in mass window & topological cut
				// Ht cut
				if (Ht>300.)	h_->Fill1d(TString("tToWj")+"_"+"cut2",candtToWj[i].M() ); // had. top + Htcut

				if (fverbose) std::cout << "tToWj chi2= " << candtToWj[i].GetKFChi2() << std::endl;
				
				h_->Fill1d(TString("kinfit_chi2_cut1"), candtToWj[i].GetKFChi2() );
				h_->Fill1d(TString("kinfit_probchi2_cut1"), TMath::Prob(candtToWj[i].GetKFChi2(),candtToWj[i].GetKFndf()  ) );

				// select the cand with best chi2
				if (i==0 && fkinfit ) {
					double probchi2 = TMath::Prob(candtToWj[i].GetKFChi2(),candtToWj[i].GetKFndf() );
					h_->Fill1d(TString("tToWj")+"_"+"cut3",candtToWj[i].M() ); // lowest chi2 combination
					h_->Fill1d(TString("kinfit_chi2_cut3"), candtToWj[i].GetKFChi2() );
					h_->Fill1d(TString("kinfit_probchi2_cut3"),  probchi2 );
					if (fverbose) std::cout << "tToWj KF chi2 = " << candtToWj[i].GetKFChi2() << std::endl;
					if (fverbose) std::cout << "tToWj get a top pair, collection size = " << pairtToWj.size() << std::endl;
					TopPair apair = pairtToWj[i];
					if (fverbose) {
						std::cout << "tToWj got it!" << std::endl;
						std::cout << "tToWj top pair p-jet E= " << apair.GetCand("p").E() << std::endl;
						std::cout << "tToWj[0] top pair chi2= " << apair.GetKFChi2() << std::endl;
						std::cout << "tToWj top pair fitted p-jet E= " << apair.GetFitted("p").E() << std::endl;
						std::cout << "tToWj top pair res= " << apair.GetRes("p") << std::endl;
						std::cout << "tToWj KF lepton E= " << apair.GetCand("lepton").E() << std::endl;
						std::cout << "tToWj[0] top pair M= " << apair.TopPairM() << std::endl;
						std::cout << "tToWj[0] fitted top pair M= " << apair.FittedTopPairM() << std::endl;
					}
					double res = (apair.GetFitted("p").E() - apair.GetCand("p").E() )/ (apair.GetRes("p"));
					if (fverbose) std::cout << "tToWj kinfit resolution done" << std::endl;
					h_->Fill1d(TString("E0Res_cut3"), res );
					h_->Fill1d(TString("topPair_cut3"), apair.TopPairM() );
					h_->Fill1d(TString("topPairRes_cut3"), (apair.TopPairM() - genTopPair.M())/genTopPair.M() );

					
					if ( probchi2>0.01 ) {
						h_->Fill1d(TString("tToWj")+"_"+"cut4",candtToWj[i].M() );
						
						TMatrixD cov = apair.GetCovHadWJet(0);
						h_->Fill1d(TString("EtpRes_cut4"), (apair.GetFitted("p").Et() - apair.GetCand("p").Et())/sqrt(cov(0,0)));
						h_->Fill1d(TString("EtapRes_cut4"), (apair.GetFitted("p").Eta() - apair.GetCand("p").Eta())/sqrt(cov(1,1)));
						h_->Fill1d(TString("PhipRes_cut4"), (apair.GetFitted("p").Phi() - apair.GetCand("p").Phi())/sqrt(cov(2,2)));
						
						cov = apair.GetCovHadWJet(1);
						h_->Fill1d(TString("EtqRes_cut4"), (apair.GetFitted("q").Et() - apair.GetCand("q").Et())/sqrt(cov(0,0)));
						h_->Fill1d(TString("EtaqRes_cut4"), (apair.GetFitted("q").Eta() - apair.GetCand("q").Eta())/sqrt(cov(1,1)));
						h_->Fill1d(TString("PhiqRes_cut4"), (apair.GetFitted("q").Phi() - apair.GetCand("q").Phi())/sqrt(cov(2,2)));
						
						cov = apair.GetCovHadbJet();
						h_->Fill1d(TString("EthbRes_cut4"), (apair.GetFitted("hadbjet").Et() - apair.GetCand("hadbjet").Et())/sqrt(cov(0,0)));
						h_->Fill1d(TString("EtahbRes_cut4"), (apair.GetFitted("hadbjet").Eta() - apair.GetCand("hadbjet").Eta())/sqrt(cov(1,1)));
						h_->Fill1d(TString("PhihbRes_cut4"), (apair.GetFitted("hadbjet").Phi() - apair.GetCand("hadbjet").Phi())/sqrt(cov(2,2)));
					
						cov = apair.GetCovLepbJet();
						h_->Fill1d(TString("EtlbRes_cut4"), (apair.GetFitted("lepbjet").Et() - apair.GetCand("lepbjet").Et())/sqrt(cov(0,0)));
						h_->Fill1d(TString("EtalbRes_cut4"), (apair.GetFitted("lepbjet").Eta() - apair.GetCand("lepbjet").Eta())/sqrt(cov(1,1)));
						h_->Fill1d(TString("PhilbRes_cut4"), (apair.GetFitted("lepbjet").Phi() - apair.GetCand("lepbjet").Phi())/sqrt(cov(2,2)));
						
						TMatrixD cov2 = apair.GetCovLepW(0);
						h_->Fill1d(TString("EtlRes_cut4"), (apair.GetFitted("lepton").Et() - apair.GetCand("lepton").Et())/sqrt(cov2(0,0)));
						h_->Fill1d(TString("EtalRes_cut4"), (apair.GetFitted("lepton").Eta() - apair.GetCand("lepton").Eta())/sqrt(cov2(1,1)));
						h_->Fill1d(TString("PhilRes_cut4"), (apair.GetFitted("lepton").Phi() - apair.GetCand("lepton").Phi())/sqrt(cov2(2,2)));
						
						cov2 = apair.GetCovLepW(0);
						h_->Fill1d(TString("EtnRes_cut4"), (apair.GetFitted("neutrino").Et() - apair.GetCand("neutrino").Et())/sqrt(cov2(0,0)));
						h_->Fill1d(TString("EtanRes_cut4"), (apair.GetFitted("neutrino").Eta() - apair.GetCand("neutrino").Eta())/sqrt(cov2(1,1)));
						h_->Fill1d(TString("PhinRes_cut4"), (apair.GetFitted("neutrino").Phi() - apair.GetCand("neutrino").Phi())/sqrt(cov2(2,2)));
						
						
							   
					//	h_->Fill1d(TString("tToWj")+"_"+"cut4",candtToWj[i].M() );
					//	h_->Fill1d(TString("E0Res_cut4"), res );
						h_->Fill1d(TString("topPair_cut4"), apair.FittedTopPairM() );
						h_->Fill1d(TString("topPairRes_cut4"), (apair.FittedTopPairM() - genTopPair.M())/genTopPair.M() );
					}
				}

				
			}

			  
		}
		
		// considering b-tagging
		if (candb.size() > 0) {
			TopCombos tToWlnub;
			TopCombos WTojj_nob;
			TopCombos tTojjb;
			TopCombos tToWb;

			tToWlnub.SetCandidate1( candw );
			tToWlnub.SetCandidate2( candb );
			std::vector< TopComposite > candtToWlnub;
			candtToWlnub = tToWlnub.GetComposites();
			if (fverbose) std::cout << "tToWlnub combinations = "<< candtToWlnub.size() << std::endl;
			for (unsigned int i=0; i!= candtToWlnub.size(); ++i) {
				h_->Fill1d(TString("tToWlnub")+"_"+"cut0",candtToWlnub[i].M() );
			}
			// request back-to-back events in phi
			if (ftopo) tToWlnub.SetMinDeltaPhi(1.5);
			candtToWlnub.clear();
			candtToWlnub = tToWlnub.GetComposites();
			for (unsigned int i=0; i!= candtToWlnub.size(); ++i) {
				h_->Fill1d(TString("tToWlnub")+"_"+"cut1",candtToWlnub[i].M() ); // with topological cut
			}
		
			WTojj_nob.SetCandidate1( candq );
			WTojj_nob.SetCandidate2( candq );
			std::vector< TopComposite > candWTojj_nob;
			candWTojj_nob = WTojj_nob.GetComposites();
			if (fverbose) std::cout << "WTojj_nob combinations = "<< candWTojj_nob.size() << std::endl;
			for (unsigned int i=0; i!= candWTojj_nob.size(); ++i) {
				h_->Fill1d(TString("WTojj_nob")+"_"+"cut0",candWTojj_nob[i].M() );
			}

			WTojj_nob.SetMinInvMass(60.);
			WTojj_nob.SetMaxInvMass(130.);
			candWTojj_nob.clear();
			candWTojj_nob = WTojj_nob.GetComposites();
			for (unsigned int i=0; i!= candWTojj_nob.size(); ++i) {
				h_->Fill1d(TString("WTojj_nob")+"_"+"cut1",candWTojj_nob[i].M() ); // had. W with cuts on the mass
			}
			
			tTojjb.SetCandidate1( candq ); // j1 of t-> W + b, W -> j1 + j2
			tTojjb.SetCandidate2( candq ); // j2
			tTojjb.SetCandidate3( candb ); // b
			std::vector< TopComposite > candtTojjb;
			candtTojjb = tTojjb.GetComposites();
			if (fverbose) std::cout << "tTojjb combinations = " << candtTojjb.size() << std::endl;
			for (unsigned int i=0; i!= candtTojjb.size(); ++i) {
				h_->Fill1d(TString("tTojjb")+"_"+"cut0",candtTojjb[i].M() );
				if (Ht>300.) h_->Fill1d(TString("tTojjb")+"_"+"cut1",candtTojjb[i].M() ); // had. top + Htcut
			}

			if ( candWTojj_nob.size() > 0 ) {
				tToWb.SetCandidate1( WTojj_nob.GetVectors() );// candWTojj_nob ); // W-> jj no b-jets
				tToWb.SetCandidate2( candb ); // b-jet
				tToWb.SetFilterList( WTojj_nob.GetList() ); // filter
				std::vector< TopComposite > candtToWb;
				candtToWb = tToWb.GetComposites();
				if (fverbose) std::cout << "tToWb combinations = " << candtToWb.size() << std::endl;
				for (unsigned int i=0; i!= candtToWb.size(); ++i) {
					h_->Fill1d(TString("tToWb")+"_"+"cut0",candtToWb[i].M() );
				}
			
				// request back-to-back events in phi
				if (ftopo) tToWb.SetMinDeltaPhi(1.5);
				candtToWb.clear();
				candtToWb = tToWb.GetComposites();
				for (unsigned int i=0; i!= candtToWb.size(); ++i) {
					h_->Fill1d(TString("tToWb")+"_"+"cut1",candtToWb[i].M() );
					if (Ht>300.) h_->Fill1d(TString("tToWb")+"_"+"cut2",candtToWlnub[i].M() );
				}
			}
		}
		
		
		
	} // end main loop

	std::cout << " Total number of entries = " << nentries << std::endl;
	std::cout << " Entries analyzed        = " << npassevents << std::endl;
}


//_______________________________________________________________
Int_t TopZpAnalysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}

//_______________________________________________________________
Long64_t TopZpAnalysis::LoadTree(Long64_t entry)
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
void TopZpAnalysis::Init(TChain *tree)
{

	if (!tree) return;

	fChain = tree;
	fCurrent = -1;
	
	fChain->SetBranchAddress("top.",&fevent);
	
}

//_______________________________________________________________
Bool_t TopZpAnalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

//_______________________________________________________________
void TopZpAnalysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

//_______________________________________________________________
Int_t TopZpAnalysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

//_______________________________________________________________
void TopZpAnalysis::Print(std::string extension,std::string tag) {

	h_->Print(extension, tag );
	
}


//_______________________________________________________________
void TopZpAnalysis::SaveToFile(TString filename) {

	h_->SaveToFile(filename);
	
}

