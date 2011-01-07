#define top_readskim_cxx
#include "top_readskim.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

#include "MuonSelector.cc"

void top::Loop()
{
//   In a ROOT session, you can do:
//      Root > gSystem->Load("libYumicevaTop7TeV.so")
//      Root > .include ../../interface
//      Root > .include ../../../../
//      Root > .L MuonSelector.cc++
//      Root > .L top_readskim.C++
//      Root > top t
//      Root > t.Loop();       // Loop on all entries
//
// The input file is defined in top.h constructor
//

  TFile fresults = TFile("results.root","RECREATE");

  TH1F *h_muon_pt = new TH1F("h_muon_pt","p_{T}^{#mu} [GeV/c]",25,20,100);

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   // setup muon selector
   MuonSelector muon_selector;
   // Ref selection V3
   muon_selector.Version(4);

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      // event info
      cout << "run: " << ntuple->run << " lumi: " << ntuple->lumi << endl;
      
      cout << "weight = " << ntuple_weight.weight << endl;

      // get collections
      vector< TopVertexEvent > primaryVertices = ntuple->vertices;
      vector< TopMuonEvent > muons = ntuple->muons;
      vector< TopJetEvent > Calojets = ntuple->Calojets;

      size_t total_muons = muons.size();
      size_t total_calojets = Calojets.size();

      // get Z position of good primary vertex
      float PVz = primaryVertices[0].vz;
      
      // loop over muons in the event
      for ( size_t imu=0; imu < total_muons; ++imu) {
	
	// select only good muons
	if ( ! muon_selector.Pass( muons[imu], Calojets, PVz) ) continue;
	cout << " muon pt= " << muons[imu].pt << endl;
	if (muons[imu].pt > 20.) h_muon_pt->Fill( muons[imu].pt );
 
      }
      // loop over jets in the event
      for ( size_t ijet=0; ijet < total_calojets; ++ijet) {

	cout << " calo jet pt= " << Calojets[ijet].pt << endl;

      }
   }

   // save results
   fresults.cd();
   h_muon_pt->Write();

}
