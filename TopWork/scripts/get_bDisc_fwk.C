// need first to load:
// gSystem->Load("libCondFormatsJetMETObjects.so")

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

//#include "DataFormats/JetReco/interface/CaloJetCollection.h"
//#include "DataFormats/JetReco/interface/CaloJet.h"
//#include "DataFormats/JetReco/interface/GenJetCollection.h"
//#include "DataFormats/JetReco/interface/GenJet.h"
//#include "DataFormats/HepMCCandidate/interface/GenParticle.h" 
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
//#include "DataFormats/PatCandidates/interface/Muon.h"

#endif

//using namespace fwlite;
using namespace std;

void get_bDisc(TString outfilename)
{

	
	vector<string> fileNamesttbar;
	fileNamesttbar.push_back("dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_1_v3/TTJets-madgraph_Fall08/TTJets_madgraph_Fall08_1.root");
	fileNamesttbar.push_back("dcache:/pnfs/cms/WAX/resilient/yumiceva/Top/PatTuple_2_2_1_v3/TTJets-madgraph_Fall08/TTJets_madgraph_Fall08_2.root");

	fwlite::ChainEvent ev(fileNamesttbar);

	cout << "Total entries = " << ev.size() << endl;
	
	unsigned int nevents = 0;

	TFile *outfile = new TFile(outfilename,"RECREATE");
	
	TH1F *hdiscNorm_b = new TH1F("hdiscNorm_b","hdiscNorm_b",200,-20,80);
	TH1F *hdiscNorm_cl = new TH1F("hdiscNorm_cl","hdiscNorm_cl",200,-20,80);

	for (ev.toBegin(); !ev.atEnd(); ++ev) {
		
		//cout << " entry = " << nevents << endl;

		++nevents;

		int goodjets = 0;

		fwlite::Handle< std::vector< pat::Jet> > jets;
		jets.getByLabel( ev, "selectedLayer1Jets");
		//cout << " got jets: "<< jets->size() << endl;
		
		for ( int ijet=0; ijet != jets->size(); ++ijet ) {

			pat::Jet jet = jets->at(ijet);

			if ( jet.pt() > 30. && fabs(jet.eta())<2.4 ) {

				goodjets++;

				if ( abs(jet.partonFlavour()) == 5 )  
				  hdiscNorm_b->Fill( jet.bDiscriminator("trackCountingHighEffBJetTags") );
				if ( abs(jet.partonFlavour()) < 5 || abs(jet.partonFlavour()) == 21 )
				  hdiscNorm_cl->Fill(jet.bDiscriminator("trackCountingHighEffBJetTags") );
			}

		}

	}

	hdiscNorm_b->Scale(1./hdiscNorm_b->Integral());
	hdiscNorm_cl->Scale(1./hdiscNorm_cl->Integral());
	
	outfile->Write();
	
}
