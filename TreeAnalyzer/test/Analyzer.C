
// The class definition in Analyzer.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("Analyzer.C")
// Root > T->Process("Analyzer.C","some options")
// Root > T->Process("Analyzer.C+")
//

#include "Analyzer.h"

#include <TStyle.h>
#include <iostream>
#include <vector>


void Analyzer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   Info("Begin", "starting with process option: %s", option.Data());

}

void Analyzer::SlaveBegin(TTree * tree)
{
  
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   Info("SlaveBegin",
        "starting with process option: %s (tree: %p)", option.Data(), tree);

   //initialize the Tree branch addresses
   Init(tree);

   //create histograms
   h1test = new TH1F("h1test","muon p_{T}",100,10.,400);
   
   // cut flow
   cutmap["Processed"]    = 0;
   cutmap["OneIsoMu"]     = 0;
   cutmap["LooseMuVeto"]  = 0;
   cutmap["ElectronVeto"] = 0;
   cutmap["MET"]          = 0;
   cutmap["1Jet"]         = 0;
   cutmap["2Jet"]         = 0;
   cutmap["3Jet"]         = 0;

}

Bool_t Analyzer::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either Analyzer::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   //TString option = GetOption();

  //if ( entry % 100 == 0 )
  //cout<< "process entry " << entry << endl;

  //TString sEntry = Form("%f", float(entry) );
  //   Info("Process",
  //"entry # %s", Form("%f", float(entry) ) );

  //fChain->GetTree()->GetEntry(entry);
  fChain->GetEntry(entry);

  //cout << "met= " << ntuple->PFMET << endl;

   // event info
  //cout << "run: " << ntuple->run << " lumi: " << ntuple->lumi << endl;

  // get collections
  vector< TopVertexEvent > primaryVertices = ntuple->vertices;
  vector< TopMuonEvent > muons = ntuple->muons;
  vector< TopElectronEvent > electrons = ntuple->electrons;
  vector< TopJetEvent > jets = ntuple->PFjets;

  size_t total_pvs   = primaryVertices.size();
  size_t total_muons = muons.size();
  size_t total_electrons = electrons.size();
  size_t total_jets  = jets.size();

  float PVz = 0.;
  TLorentzVector p4muon, p4ele, p4lepton, p4MET;
  vector< TLorentzVector > p4jets;

  ////////////////////////////////////
  // PRIMARY VERTICES
  ///////////////////////////////////
  for ( size_t ipv=0; ipv < total_pvs; ++ipv) {

    if (ipv==0) PVz = primaryVertices[ipv].vz;

  }

  //////////////////////////////////
  // MUONS
  //////////////////////////////////
  int nloosemuons = 0;
  int ntightmuons = 0;

  for ( size_t imu=0; imu < total_muons; ++imu) {
     
    TopMuonEvent muon = muons[imu];
    
    h1test->Fill( muon.pt );
    
    // select only good muons

    if ( fMuSelector.MuonLoose( muon ) ) 
      {

	nloosemuons++;

	if ( fMuSelector.MuonTight( muon, PVz) ) ntightmuons++;

	p4muon.SetPtEtaPhiE( muon.pt, muon.eta, muon.phi, muon.e );

      }
  }

  //////////////////////////////////
  // ELECTRONS
  //////////////////////////////////                                                                                                                                                                              
  int nlooseelectrons = 0;
  int ntightelectrons = 0;
  bool IsConversion = false;

  for ( size_t iele=0; iele < total_electrons; ++iele) {

    TopElectronEvent electron = electrons[iele];
    
    if ( fEleSelector.ElectronLoose(electron) ) nlooseelectrons++;

    if ( fEleSelector.ElectronTight(electron, PVz ) )
      {

	if (ntightelectrons == 0)
	  {
	    IsConversion = electron.IsConversion;
	    p4ele.SetPtEtaPhiE( electron.pt, electron.eta, electron.phi, electron.e );
	  }
	ntightelectrons++;
      }
  }

  if ( ntightmuons != 1 ) return kTRUE;
  cutmap["OneIsoMu"]++;

  if ( nloosemuons > 1 ) return kTRUE;
  cutmap["LooseMuVeto"]++;

  if ( total_electrons > 0 ) return kTRUE;
  cutmap["ElectronVeto"]++;

  p4lepton = p4muon;

  /////////////////////////////////
  // MET
  /////////////////////////////////
  
  p4MET.SetPtEtaPhiE( ntuple->PFMET,
		      0.,
		      ntuple->PFMETphi,
		      ntuple->PFMET );

  if ( p4MET.Et() <= 20. ) return kTRUE;
  cutmap["MET"]++;
  
  /////////////////////////////////
  // JETS
  ////////////////////////////////
  
  int njets = 0;
  map< string, vector<float> > bdisc;
  map< string, vector<bool> >  isTagb;

  for ( size_t ijet=0; ijet < total_jets; ++ijet) 
    {

    TopJetEvent jet = jets[ijet];

    if ( jet.pt > 30. ) 
      {
	TLorentzVector tmpjet;
	tmpjet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e);
	p4jets[njets] = tmpjet;
	
	bdisc["TCHP"].push_back( jet.btag_TCHP );
	bdisc["SSVHE"].push_back( jet.btag_SSVHE );
	if ( jet.btag_TCHP > 1.19 ) isTagb["TCHPL"].push_back(true);
	else isTagb["TCHPL"].push_back(false);
	if ( jet.btag_SSVHE > 1.74) isTagb["SSVHEM"].push_back(true);
	else isTagb["SSVHEM"].push_back(false);

	njets++;
      }
    }
  
  if (njets==1)
    {
      cutmap["1Jet"]++;

    }

  if (njets==2)
    {
      cutmap["2Jet"]++;
    }

  if (njets==3)
    {
      cutmap["3Jet"]++;
    }

  if (njets>=4)
    {
      cutmap["4Jet"]++;
    }
  return kTRUE;
}

void Analyzer::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void Analyzer::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}
