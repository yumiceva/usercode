#define top_cxx
#include "top.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void top::Loop()
{
//   In a ROOT session, you can do:
//      Root > gSystem->Load("libYumicevaTop7TeV.so")
//      Root > .include ../../interface
//      Root > .include ../../../../
//      Root > .L top.C
//      Root > top t
//      Root > t.Loop();       // Loop on all entries
//
// The input file is defined in top.h constructor
//

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      // event info
      cout << "run: " << ntuple->run << " lumi: " << ntuple->lumi << endl;

      // get collections
      vector< TopMuonEvent > muons = ntuple->muons;
      vector< TopJetEvent > Calojets = ntuple->Calojets;

      size_t total_muons = muons.size();
      size_t total_calojets = Calojets.size();
      // loop over muons in the event
      for ( size_t imu=0; imu < total_muons; ++imu) {
	
	cout << " muon pt= " << muons[imu].pt << endl;
      }
      // loop over jets in the event
      for ( size_t ijet=0; ijet < total_calojets; ++ijet) {

	cout << " calo jet pt= " << Calojets[ijet].pt << endl;

      }
   }
}