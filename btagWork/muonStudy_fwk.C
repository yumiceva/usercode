
// gSystem->Load("libCondFormatsJetMETObjects.so")

#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TString.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include <vector>
#include <string>
#include <iostream>


#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "CondFormats/JetMETObjects/interface/CombinedJetCorrector.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"

#include "DataFormats/Candidate/interface/Candidate.h"
//#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
//#include "DataFormats/JetReco/interface/GenJetCollection.h"
//#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#endif

//using namespace fwlite;
using namespace std;


/*
TLorentzVector getMCMuon( TLorentzVector recomuon,
						  std::vector< reco::GenParticle > gens, bool prompt = false) {

	double mindeltaR = 0.1;
	double tmpmin = 99999.;
		
	TLorentzVector genP4;
	
	for ( int ip=0; ip < gens->size(); ++ip ) {

		reco::GenParticle p = gens->at(ip);
		// get a muon
		if ( abs( p.pdgId() ) == 13 && p.status() == 1) {
			// find the mother ID
			int mompdg = p.mother()->pdgId();

			TLorentzVector tmpP4( p.px(), p.py(), p.pz(), p.energy() );

			double deltaR = tmpP4.Vect().DeltaR( recomuon.Vect() );
			
			if ( deltaR < tmpmin && deltaR < mindeltaR ) {

				if ( prompt ) {

					if ( mompdg > 321 ) {
						tmpmin = deltaR;
						genP4 = tmpP4;
					}
				} else {
					tmpmin = deltaR;
					genP4 = tmpP4;
				}
			}
			
		}
	}

	return genP4;
}

*/

void muonStudy_fwk(bool TM = false) {

	vector<string> fileNames;
	fileNames.push_back("dcache:/pnfs/cms/WAX/11/store/mc/Summer08/InclusiveMu5Pt50/GEN-SIM-RECO/IDEAL_V9_v1/0000/0011B828-3F90-DD11-AF35-003048770DCE.root");
	fileNames.push_back("dcache:/pnfs/cms/WAX/11/store/mc/Summer08/InclusiveMu5Pt50/GEN-SIM-RECO/IDEAL_V9_v1/0000/002898D5-2290-DD11-9EC0-00304865C2CA.root");
	
	fwlite::ChainEvent ev(fileNames);
	
	cout << "Total entries = " << ev.size() << endl;

	unsigned int nevents = 0;
	
	// Jet corrections
	string Levels = "L2:L3";
	string Tags = "Summer08_L2Relative_IC5Calo:Summer08_L3Absolute_IC5Calo";
	CombinedJetCorrector *L2L3JetCorrector = new CombinedJetCorrector(Levels,Tags);
	
	TFile *outfile = new TFile("muonStudy.root","RECREATE");
	
	TH1F *hjet1pt = new TH1F("hjet1pt","jet p_{T} [GeV/c]",30,0,150);
	TH1F *hjet2pt = new TH1F("hjet2pt","jet p_{T} [GeV/c]",30,0,150);
	TH1F *hjet3pt = new TH1F("hjet3pt","jet p_{T} [GeV/c]",30,0,150);
	TH1F *hjet1eta = new TH1F("hjet1eta","jet #eta",30,-3,3);
	TH1F *hjet2eta = new TH1F("hjet2eta","jet #eta",30,-3,3);
	TH1F *hjet3eta = new TH1F("hjet3eta","jet #eta",30,-3,3);
	TH1F *hjetmupt = new TH1F("hjetmupt","jet p_{T} [GeV/c]",30,0,150);
	TH1F *hjetmuptcor = new TH1F("hjetmuptcor","jet p_{T} [GeV/c]",30,0,150);
	TH1F *hNgoodjets = new TH1F("hNgoodjets","Number of jets",20,0,20);
	TH1F *hNjetswithMuonInJet = new TH1F("hNjetswithMuonInJet","Number of jets with a muon",10,0,10);
	TH1F *hjetpt_withmuon = new TH1F("hjetpt_withmuon","jet p_{T} [GeV/c]",30,0,150);
	TH1F *hjeteta_withmuon = new TH1F("hjeteta_withmuon","jet #eta",30,-3,3);
	
	TH1F *hmuon_pt1 = new TH1F("hmuon_pt1","muon p_{T} [GeV/c]",30,0,80);
	TH1F *hmuon_pt2 = new TH1F("hmuon_pt2","muon p_{T} [GeV/c]",30,0,80);
	TH1F *hmuon_pt3 = new TH1F("hmuon_pt3","muon p_{T} [GeV/c]",30,0,80);
	TH1F *hmuon_eta1 = new TH1F("hmuon_eta1","muon #eta",30,-3,3);
	TH1F *hmuon_eta2 = new TH1F("hmuon_eta2","muon #eta",30,-3,3);
	TH1F *hmuon_eta3 = new TH1F("hmuon_eta3","muon #eta",30,-3,3);
	TH1F *hmuon_d01 = new TH1F("hmuon_d01", "muon d_{0} [cm] ", 30, -0.5,0.5);
	TH1F *hmuon_Ngood = new TH1F("hmuon_Ngood","muons", 5, 0, 5);
	TH1F *hNmuinjet = new TH1F("hNmuinjet","muons",5,0,5);
	TH1F *hmuon_deltaR = new TH1F("hmuon_deltaR","#Delta R(#mu,jet)",30,0,0.5);
	TH1F *hmuon_deltaR3 = new TH1F("hmuon_deltaR3","#Delta R(#mu,jet)",30,0,0.5);
	TH1F *hmuon_nthjet = new TH1F("hmuon_nthjet","nth jet with a muon",5,0,5);
	TH1F *hmuon_ptrel3 = new TH1F("hmuon_ptrel3","p_{T}^{Rel}",30,0,6);

	TH1F *hmuon_pt1matched = new TH1F("hmuon_pt1matched","muon p_{T} [GeV/c]",30,0,80);
	TH1F *hmuon_ptgen = new TH1F("hmuon_ptgen","muon p_{T} [GeV/c]",30,0,80);
	TH1F *hmuon_ptgenPrompt = new TH1F("hmuon_ptgenPrompt","muon p_{T} [GeV/c]",30,0,80);
	
	for (ev.toBegin(); !ev.atEnd(); ++ev) {
		
		//cout << " entry = " << nevents << endl;

		++nevents;

		int goodjets = 0;
		int NjetswithMuonInJet =0;
		
		fwlite::Handle< std::vector< reco::CaloJet > > jets;
		jets.getByLabel( ev, "iterativeCone5CaloJets" );
		//cout << " got jets: "<< jets->size() << endl;

		fwlite::Handle< std::vector< reco::Muon > > muons;
		muons.getByLabel( ev, "muons" );
		//cout << " got muons: "<< muons->size() << endl;

		fwlite::Handle< std::vector<reco::GenParticle> > gens;
		gens.getByLabel(ev, "genParticles");
		
		TLorentzVector jetP4[3];
		TLorentzVector muonP4;

		//generator
		for ( int ip=0; ip < gens->size(); ++ip ) {

			reco::GenParticle p = gens->at(ip);
			// get a muon
			if ( abs( p.pdgId() ) == 13 && p.status() == 1) {
				// find the mother
				int mompdg = p.mother()->pdgId();
				if ( mompdg <= 321) {
					// muons from pions and kaons decays
					hmuon_ptgenPrompt->Fill( p.pt() );
				}
				
				hmuon_ptgen->Fill( p.pt() );
			
			}
		}
			
		for ( int ijet=0; ijet != jets->size(); ++ijet ) {

			reco::CaloJet jet = jets->at(ijet);

			//cout << " jet pt: " << jet.pt() << " eta: " << jet.eta() << endl;
			double correction = L2L3JetCorrector->getCorrection(jet.pt(), jet.eta() );
			
			//cout << " correction " << correction << endl;

			TLorentzVector tmpP4(jet.px(), jet.py(), jet.pz(), jet.energy());
			tmpP4 = correction * tmpP4;

			//cout << " TL pt: " << tmpP4.Pt() << " eta: " << tmpP4.Eta() << endl;

			// jet selection
			if ( tmpP4.Pt() <= 30. || fabs(tmpP4.Eta())>= 2.4 ) continue;
			goodjets++;
			
			// leading jets
			if ( tmpP4.Pt() > jetP4[0].Pt() ) jetP4[0] = tmpP4;
			if ( tmpP4.Pt() > jetP4[1].Pt() && tmpP4.Pt() < jetP4[0].Pt() ) jetP4[1] = tmpP4;
			if ( tmpP4.Pt() > jetP4[2].Pt() && tmpP4.Pt() < jetP4[1].Pt() ) jetP4[2] = tmpP4;

			int NgoodMuon = 0;
			int Nmuinjet = 0 ;
			int iofjet = 0;
			bool gotMuonInJet = false;
			TLorentzVector MuP4;
			
			for ( int imuon=0; imuon != muons->size(); ++imuon ) {

				reco::Muon mu = muons->at(imuon);
		  
				if ( mu.isGlobalMuon() ) {

					double mupt = mu.pt();
					if ( TM ) mupt = mu.innerTrack()->pt();
					hmuon_pt1->Fill( mupt );

					//TLorentzVector matchMuP4 = getMCMuon( TLorentzVector(mu.px(),mu.py(),mu.pz(),mu.energy()), &gens );
					//if (matchMuP4.Pt() > 0 ) hmuon_pt1matched->Fill( mu.pt() );
														  
					
					int nhit = mu.innerTrack()->numberOfValidHits();
					double normChi2 = mu.globalTrack()->chi2() / mu.globalTrack()->ndof();
					double d0 = mu.innerTrack()->d0();
		  
					if ( nhit > 10 && normChi2 < 10 && mupt >3 ) {

						TLorentzVector tmpMuP4(mu.px(),mu.py(),mu.pz(),mu.energy());
						if ( TM ) tmpMuP4.SetPxPyPzE(mu.innerTrack()->px(),
													 mu.innerTrack()->py(),
													 mu.innerTrack()->pz(),
													 sqrt(mu.innerTrack()->p()*mu.innerTrack()->p() +(0.1057*0.1057)));
						
						TLorentzVector tmpJetMuP4 = tmpMuP4+tmpP4;
						double deltaR = tmpP4.Vect().DeltaR( tmpMuP4.Vect() );
						//double ptRel = 0.;
						
						/// associate muon to jet
						if ( deltaR < 0.4 ) {
							gotMuonInJet = true;
							if (Nmuinjet==0) MuP4 = tmpMuP4;// get leading muon
							Nmuinjet++;
							hmuon_deltaR->Fill( deltaR );
							hmuon_nthjet->Fill( ijet );// when jet corrections are applied this could be misleading because jets are not sorted in pt anymore.

							hmuon_pt2->Fill( tmpMuP4.Pt() );
							hmuon_eta2->Fill( tmpMuP4.Eta() );
							hjetpt_withmuon->Fill( tmpP4.Pt() );
							hjeteta_withmuon->Fill( tmpP4.Eta() );

							
						}
					}
				}
			}//close muons
			
			if (gotMuonInJet) {
				hNmuinjet->Fill( Nmuinjet );
				NjetswithMuonInJet++;
				//take leading muon
				double deltaR = tmpP4.Vect().DeltaR( MuP4.Vect() );
				TVector3 leptonvec = MuP4.Vect();
				TVector3 jetvec = tmpP4.Vect();
				jetvec += leptonvec;
				double ptRel = leptonvec.Perp(jetvec);
				hmuon_pt3->Fill(MuP4.Pt());
				hmuon_eta3->Fill(MuP4.Eta());
				hmuon_deltaR3->Fill( deltaR );
				hmuon_ptrel3->Fill( ptRel );			
			}
		}

		hNgoodjets->Fill(goodjets);
		hNjetswithMuonInJet->Fill(NjetswithMuonInJet);
		
		/// fill leading jets
		if ( jetP4[0].Pt() > 0 ) {
			hjet1pt->Fill(jetP4[0].Pt());
			hjet1eta->Fill(jetP4[0].Eta());
		}
		if ( jetP4[1].Pt() > 0 ) {
			hjet2pt->Fill(jetP4[1].Pt());
			hjet2eta->Fill(jetP4[1].Eta());
		}
		if ( jetP4[2].Pt() > 0 ) {
			hjet3pt->Fill(jetP4[2].Pt());
			hjet3eta->Fill(jetP4[2].Eta());
		}
		
		//cout << " jet 1 " << jetP4[0].Pt() << endl;
		//cout << " jet 2 " << jetP4[1].Pt() << endl;
		//cout << " jet 3 " << jetP4[2].Pt() << endl;
		
				
		//if (nevents > 1 ) break;
	}

	outfile->Write();
	
}

