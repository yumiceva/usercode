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
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"

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
	fileNamesttbar.push_back("dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_5/TTJets-madgraph_Fall08/TTJets_madgraph_Fall08_1.root");
	fileNamesttbar.push_back("dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_5/TTJets-madgraph_Fall08/TTJets_madgraph_Fall08_2.root");
	//fileNamesttbar.push_back("file:/uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/challange2009/PatChallengeTest.root");
	
	fwlite::ChainEvent ev(fileNamesttbar);
	
	cout << "Total entries = " << ev.size() << endl;

	unsigned int nevents = 0;
	
	int good4jets = 0;

	int withGlobalMuons = 0;
	int withGMnormchi2 = 0;
	int withGoodMuons = 0;
	int with1GoodMuon = 0;
	int withGoodIsoMuons = 0;
	int withOneIsoMuon = 0;
	

	//histograms
	TH1 *h1_muon_pt = new TH1F("h1_muon_pt","muon p_{T} [GeV/c]",40,0,120);
	TH1 *h1_muon_pt_hlt = new TH1F("h1_muon_pt_htl","muon p_{T} [GeV/c]",40,0,120);
	
	for (ev.toBegin(); !ev.atEnd(); ++ev) {
		
		//cout << " entry = " << nevents << endl;

		++nevents;

		int goodjets = 0;
		
		//fwlite::Handle< std::vector< pat::Jet> > jets;
		//jets.getByLabel( ev, "selectedLayer1Jets");
		//cout << " got jets: "<< jets->size() << endl;
		
		fwlite::Handle< std::vector<pat::Muon> > muons;
		muons.getByLabel(ev, "selectedLayer1Muons");
		//cout << " got muons: "<< muons->size() << endl;
		
		fwlite::Handle<edm::TriggerResults> hltresults;
		hltresults.getByLabel(ev, "TriggerResults::HLT");
		int ntrigs=hltresults->size();
		//edm::TriggerNames triggerNames_;
		triggerNames_.init(hltresults);
		bool accept = false;
		for (int itrig = 0; itrig < ntrigs; ++itrig) {
			if (triggerNames_.triggerName(itrig) == "HLT_Mu15") {
				accept = hltresults.accept(itrig);
			}
		}

			
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

				h1_muon_pt->Fill( mu.pt() );

				
				
			}
		}
		

	
	}


	
}

