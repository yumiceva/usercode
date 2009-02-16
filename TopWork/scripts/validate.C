#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TString.h"

#include <vector>
#include <string>
#include <iostream>


#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"

//#include "DataFormats/Candidate/interface/Candidate.h"
//#include "DataFormats/Candidate/interface/Particle.h"
//#include "DataFormats/JetReco/interface/CaloJetCollection.h"
//#include "DataFormats/JetReco/interface/CaloJet.h"
//#include "DataFormats/JetReco/interface/GenJetCollection.h"
//#include "DataFormats/JetReco/interface/GenJet.h"
//#include "DataFormats/HepMCCandidate/interface/GenParticle.h" 
//#include "DataFormats/MuonReco/interface/Muon.h"
//#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#endif

//using namespace fwlite;
using namespace std;

void validate()
{

	vector<string> fileNamesttbar;
//	fileNamesttbar.push_back("dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_1_v3/TTJets-madgraph_Fall08/TTJets_madgraph_Fall08_1.root");
//	fileNamesttbar.push_back("dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_1_v3/TTJets-madgraph_Fall08/TTJets_madgraph_Fall08_2.root");
	fileNamesttbar.push_back("file:/uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/challange2009/PatChallengeTest.root");
	
	fwlite::ChainEvent ev(fileNamesttbar);
	
	cout << "Total entries = " << ev.size() << endl;

	unsigned int nevents = 0;

	int good4jets = 0;

	int withGlobalMuons = 0;
	int withGMpt = 0;
	int withGMeta = 0;
	int withGMcuts = 0;
	int withGMhits = 0;
	int withGMnormchi2 = 0;
	int withGoodMuons = 0;
	int with1GoodMuon = 0;
	int withGoodIsoMuons = 0;
	int withOneIsoMuon = 0;
	int withDeltaR = 0;

	for (ev.toBegin(); !ev.atEnd(); ++ev) {
		
		//cout << " entry = " << nevents << endl;

		++nevents;

		int goodjets = 0;
		
		fwlite::Handle< std::vector< pat::Jet> > jets;
		jets.getByLabel( ev, "selectedLayer1Jets");
		//cout << " got jets: "<< jets->size() << endl;
		
		fwlite::Handle< std::vector<pat::Muon> > muons;
		muons.getByLabel(ev, "selectedLayer1Muons");
		//cout << " got muons: "<< muons->size() << endl;
		
		for ( int ijet=0; ijet != jets->size(); ++ijet ) {

			pat::Jet jet = jets->at(ijet);

			if ( jet.pt() > 30. && fabs(jet.eta())<2.4 ) {

				goodjets++;
			}

		}

		if ( goodjets > 3) good4jets++;


		int NGM = 0;
		int NGMpt = 0;
		int NGMeta = 0;
		int NGMcuts = 0;
		int NGMhits = 0;
		int NGMnormchi2 = 0;
		int Nmuons= 0;
		int Nisomuons = 0;
		
		for ( int imuon=0; imuon != muons->size(); ++imuon ) {
			
			pat::Muon mu = muons->at(imuon);
			//cout << " pt = " << mu.pt() << endl;
			if ( mu.isGlobalMuon() ) {

				NGM++;

				if ( mu.pt() > 20 ) NGMpt++;
				if ( fabs(mu.eta()) < 2.1 ) NGMeta++;
				if ( mu.pt() > 20. && (fabs(mu.eta()) <2.1) ) NGMcuts++;
				
				int nhits = mu.innerTrack()->numberOfValidHits();
				//cout << " nhits" << nhits << endl;
				double chi2 = mu.globalTrack()->chi2();
				//cout << " chi2= " << chi2 << endl;
				double ndof = mu.globalTrack()->ndof();
				//cout << " ndof= " << ndof << endl;
				double normchi2 = chi2/ndof;

				if ( mu.pt() > 20. && (fabs(mu.eta()) <2.1) && nhits>11 ) NGMhits++;
				
				if ( mu.pt() > 20. && (fabs(mu.eta())<2.1) &&
					 nhits>11 && normchi2<10 )
				{

					Nmuons++;
					
					double relIso = mu.pt() / (mu.pt() + mu.trackIso() + mu.caloIso() );
					if ( relIso > 0.95 ) Nisomuons++;
				
				}
			}
		}

		if ( goodjets > 3 ) {
			if ( NGM > 0 ) withGlobalMuons++;
			if ( NGMpt > 0 ) withGMpt++;
			if ( NGMeta > 0 ) withGMeta++;
			if ( NGMhits > 0 ) withGMhits++;
			if ( NGMcuts > 0 ) withGMcuts++;
			if ( Nmuons >0 ) withGoodMuons++;
			//if ( Nmuons==1 ) with1GoodMuon++;
			if ( Nisomuons>0 ) withGoodIsoMuons++;
			if ( Nisomuons==1 ) withOneIsoMuon++;
		}
	}

	cout << " good 4 jets : " << good4jets << endl;
	cout << " + isGlobalMuon : " << withGlobalMuons << endl;
	cout << " + pt>20 : " << withGMpt << endl;
	//cout << " with GM and eta   : " << withGMeta << endl;
	cout << " + fabs(eta) < 2.1 : " << withGMcuts << endl;
	cout << " + nHits > 11 : " << withGMhits << endl;
	cout << " + normchi2 < 10 : " << withGoodMuons << endl;
	cout << " + RelIso>0.95 : " << withGoodIsoMuons << endl;
	cout << " + only one muon : " << withOneIsoMuon << endl;
	
}

