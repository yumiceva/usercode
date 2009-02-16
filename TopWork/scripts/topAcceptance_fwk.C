{ 

gSystem->Load("libFWCoreFWLite.so"); 
AutoLibraryLoader::enable();

#include "DataFormats/FWLite/interface/Handle.h"
using namespace fwlite;

//TDCacheFile *file = new TDCacheFile("/pnfs/cms/WAX/11/store/mc/Summer08/TauolaTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/16AAC418-218A-DD11-AC33-001F2908F0E4.root");

TDCacheFile *file = new TDCacheFile("/pnfs/cms/WAX/11/store/mc/Summer08/TauolaTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/16AAC418-218A-DD11-AC33-001F2908F0E4.root");

Event ev(file);
unsigned int nevents = 0;
int accepted = 0;

TH1F *h1 = new TH1F("h1","h1",100,50,250);

for (ev.toBegin(); !ev.atEnd(); ++ev) {
	++nevents;

	Handle<reco::GenParticleCollection> src;
	src.getByLabel(ev, "genParticles");

	for ( int ip=0; ip< src->size(); ++ip ) {

		reco::GenParticle p = src->at(ip);
		if ( abs( p.pdgId() ) == 6 )
			h1->Fill( p.mass() );
	}

	if (nevents > 1000) break;
}

TCanvas *cv = new TCanvas("cv","cv",700,700);
h1->Draw();


}

