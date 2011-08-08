
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
#include <TSystem.h>

#include <iostream>
#include <vector>

void Analyzer::ParseInput()
{

  if (fMyOpt.Contains("muon")) 
    {
      fChannel = 1;      
    }
  if (fMyOpt.Contains("electron"))
    {
      fChannel = 2;
    }
  if (fMyOpt.Contains("verbose"))
    {
      fVerbose = true;
    }
  if (fMyOpt.Contains("sample"))
    {
      TString tmp = fMyOpt;
      tmp = tmp.Remove(0,fMyOpt.Index("sample")+7);
      fSample = tmp;
      Info("Begin","Histogram names will have suffix: %s", fSample.Data());

      if ( fSample.Contains("data") )
	{
	  fIsMC = false;
	  Info("Begin","This sample is treated as DATA");
	}
      else Info("Begin","This sample is treated as MC");


      
    }
}

void Analyzer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   fMyOpt = option;
   ParseInput();

   Info("Begin", "starting with process option: %s", option.Data());

}

void Analyzer::SlaveBegin(TTree * tree)
{
  
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   fMyOpt = option;
   ParseInput();

   Info("SlaveBegin",
        "starting with process option: %s (tree: %p)", option.Data(), tree);

   //initialize the Tree branch addresses
   Init(tree);

   // We may be creating a dataset or a merge file: check it
   TNamed *nm = dynamic_cast<TNamed *>(fInput->FindObject("SimpleNtuple.root"));
   if (nm) {
     // Just create the object
     UInt_t opt = TProofOutputFile::kRegister | TProofOutputFile::kOverwrite | TProofOutputFile::kVerify;
     fProofFile = new TProofOutputFile("SimpleNtuple.root",
				       TProofOutputFile::kDataset, opt, nm->GetTitle());
   } else {
     // For the ntuple, we use the automatic file merging facility
     // Check if an output URL has been given
     TNamed *out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE_LOCATION");
     Info("SlaveBegin", "PROOF_OUTPUTFILE_LOCATION: %s", (out ? out->GetTitle() : "undef"));
     TString tmpfilename = "results";
     if ( fSample != "" ) tmpfilename += "_"+fSample+".root";
     else tmpfilename = "results.root";
     fProofFile = new TProofOutputFile(tmpfilename, (out ? out->GetTitle() : "M"));
     out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE");
     if (out) fProofFile->SetOutputFileName(out->GetTitle());
   }

   // Open the file
   //TDirectory *savedir = gDirectory;
   if (!(fFile = fProofFile->OpenFile("RECREATE"))) {
     Warning("SlaveBegin", "problems opening file: %s/%s",
	     fProofFile->GetDir(), fProofFile->GetFileName());
   }

   //create histograms
   h1test = new TH1F("h1test","muon p_{T}",100,10.,400);
   //fHist = new HistoManager(string(fSample));
   TString hname = "_"+fSample;
   
   hmuons["pt_1jet"] = new TH1F("muon_pt_1jet"+hname,"p_{T}^{#mu} [GeV/c]", 60, 20, 500);
   hmuons["pt_2jet"] = new TH1F("muon_pt_2jet"+hname,"p_{T}^{#mu} [GeV/c]", 60, 20, 500);
   hmuons["pt_3jet"] = new TH1F("muon_pt_3jet"+hname,"p_{T}^{#mu} [GeV/c]", 60, 20, 500);
   hmuons["pt_4jet"] = new TH1F("muon_pt_4jet"+hname,"p_{T}^{#mu} [GeV/c]", 60, 20, 500);
   hmuons["eta"] = new TH1F("muon_eta"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_1jet"] = new TH1F("muon_eta_1jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_2jet"] = new TH1F("muon_eta_2jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_3jet"] = new TH1F("muon_eta_3jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_4jet"] = new TH1F("muon_eta_4jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["phi"] = new TH1F("muon_phi"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["phi_1jet"] = new TH1F("muon_phi_1jet"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["phi_2jet"] = new TH1F("muon_phi_2jet"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["phi_3jet"] = new TH1F("muon_phi_3jet"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["phi_4jet"] = new TH1F("muon_phi_4jet"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["reliso"] = new TH1F("muon_reliso"+hname,"Relative Isolation", 30, 0, 1.5);
   hmuons["reliso_1jet"] = new TH1F("muon_reliso_1jet"+hname,"Relative Isolation", 30, 0, 1.5);
   hmuons["reliso_2jet"] = new TH1F("muon_reliso_2jet"+hname,"Relative Isolation", 30, 0, 1.5);
   hmuons["reliso_3jet"] = new TH1F("muon_reliso_3jet"+hname,"Relative Isolation", 30, 0, 1.5);
   hmuons["reliso_4jet"] = new TH1F("muon_reliso_4jet"+hname,"Relative Isolation", 30, 0, 1.5);
   hmuons["deltaR_cut0"] = new TH1F("deltaR_cut0"+hname,"#DeltaR(#mu,jet)",80, 0, 4);
   hmuons["deltaR"] = new TH1F("deltaR"+hname,"#DeltaR(#mu,jet)",80, 0, 4);
   hmuons["d0_cut1"] = new TH1F("d0_cut1"+hname,"#mu Impact Parameter [cm]",20,-0.1,0.1);
   hmuons["pt_cut1"] = new TH1F("muon_pt_cut1"+hname,"p_{T}^{#mu} [GeV/c]", 60, 20, 500);
   hmuons["pt_cut2"] = new TH1F("muon_pt_cut2"+hname,"p_{T}^{#mu} [GeV/c]", 60, 20, 500);
   hmuons["pt_cut3"] = new TH1F("muon_pt_cut3"+hname,"p_{T}^{#mu} [GeV/c]", 60, 20, 500);
   hmuons["pt"] = new TH1F("muon_pt"+hname,"p_{T}^{#mu} [GeV/c]", 60, 20, 500);
   hmuons["dz"] = new TH1F("dz"+hname,"|z(#mu) - z_{PV}| [cm]", 25, 0, 1.);

   hPVs["N"] = new TH1F("NPV"+hname,"Number of PVs",25,-0.5,24.5);
   hPVs["Nreweight"] = new TH1F("NPVreweight"+hname,"Number of PVs",25,-0.5,24.5);

   helectrons["pt"] = new TH1F("electron_pt"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["pt_cut2"] = new TH1F("electron_pt_cut2"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["pt_1jet"] = new TH1F("electron_pt_1jet"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["pt_2jet"] = new TH1F("electron_pt_2jet"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["pt_3jet"] = new TH1F("electron_pt_3jet"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["pt_4jet"] = new TH1F("electron_pt_4jet"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["eta"] = new TH1F("electron_eta"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   helectrons["eta_cut2"] = new TH1F("electron_eta_cut2"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   helectrons["eta_1jet"] = new TH1F("electron_eta_1jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   helectrons["eta_2jet"] = new TH1F("electron_eta_2jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   helectrons["eta_3jet"] = new TH1F("electron_eta_3jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   helectrons["eta_4jet"] = new TH1F("electron_eta_4jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   helectrons["phi"] = new TH1F("electron_phi"+hname,"#phi^{#mu}", 20, 0, 3.15);
   helectrons["phi_cut2"] = new TH1F("electron_phi_cut2"+hname,"#phi^{#mu}", 20, 0, 3.15);
   helectrons["phi_1jet"] = new TH1F("electron_phi_1jet"+hname,"#phi^{#mu}", 20, 0, 3.15);
   helectrons["phi_2jet"] = new TH1F("electron_phi_2jet"+hname,"#phi^{#mu}", 20, 0, 3.15);
   helectrons["phi_3jet"] = new TH1F("electron_phi_3jet"+hname,"#phi^{#mu}", 20, 0, 3.15);
   helectrons["phi_4jet"] = new TH1F("electron_phi_4jet"+hname,"#phi^{#mu}", 20, 0, 3.15);
   helectrons["reliso"] = new TH1F("electron_reliso"+hname,"Relative Isolation", 30, 0, 1.5);
   helectrons["reliso_1jet"] = new TH1F("electron_reliso_1jet"+hname,"Relative Isolation", 30, 0, 1.5);
   helectrons["reliso_2jet"] = new TH1F("electron_reliso_2jet"+hname,"Relative Isolation", 30, 0, 1.5);
   helectrons["reliso_3jet"] = new TH1F("electron_reliso_3jet"+hname,"Relative Isolation", 30, 0, 1.5);
   helectrons["reliso_4jet"] = new TH1F("electron_reliso_4jet"+hname,"Relative Isolation", 30, 0, 1.5);
   helectrons["deltaR_cut0"] = new TH1F("electron_deltaR_cut0"+hname,"#DeltaR(#mu,jet)",80, 0, 4);
   helectrons["deltaR"] = new TH1F("electron_deltaR"+hname,"#DeltaR(#mu,jet)",80, 0, 4);
   helectrons["d0_cut1"] = new TH1F("electron_d0_cut1"+hname,"#mu Impact Parameter [cm]",20,-0.1,0.1);
   helectrons["pt_cut1"] = new TH1F("electron_pt_cut1"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["pt_cut2"] = new TH1F("electron_pt_cut2"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["dz"] = new TH1F("electron_dz"+hname,"|z(#mu) - z_{PV}| [cm]", 25, 0, 1.);
      
   hMET["MET"] = new TH1F("MET"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["MET_2jet"] = new TH1F("MET_2jet"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["phi"] = new TH1F("MET_phi"+hname,"#phi Missing Transverse Energy [GeV]", 20, 0, 3.15);
   hMET["Ht"] = new TH1F("Ht"+hname,"H_{T} [GeV]", 50, 0, 1000);
   hMET["Htlep"] = new TH1F("Htlep"+hname,"H_{T,lep} [GeV]", 50, 0, 1000);
   hMET["PzNu"] = new TH1F("PzNu"+hname,"p_{z} #nu [GeV]", 40, -300,300);
   hMET["EtaNu"] = new TH1F("EtaNu"+hname,"#eta",50,-2.2,2.2);
   hMET["LepWmass"] = new TH1F("LepWmass"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150);
   hMET["LepWmassComplex"]=new TH1F("LepWmassComplex"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150);
   
   hM["WMt"] = new TH1F("Mt"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 300);
   hM["Wprime"] = new TH1F("Wprime"+hname,"W' invariant mass [GeV/c^{2}]", 60, 20, 2600);
   hM["Wprime_0btag"] = new TH1F("Wprime_0btag"+hname,"W' invariant mass [GeV/c^{2}]", 60, 20, 2600);
   hM["Wprime_1btag"] = new TH1F("Wprime_1btag"+hname,"W' invariant mass [GeV/c^{2}]", 60, 20, 2600);
   //hM["Wprime_2btag"] = new TH1F("Wprime_2btag"+hname,"W' invariant mass [GeV/c^{2}]", 60, 20, 2600);
   
   hjets["pt"] = new TH1F("jet_pt"+hname,"jet p_{T} [GeV/c]", 50, 30, 500);
   hjets["1st_pt"] = new TH1F("jet1_pt"+hname,"Leading jet p_{T} [GeV/c]",50, 30, 500);
   hjets["2nd_pt"] = new TH1F("jet2_pt"+hname,"2nd jet p_{T} [GeV/c]",50, 30, 500);
   hjets["3rd_pt"] = new TH1F("jet3_pt"+hname,"3rd jet p_{T} [GeV/c]",50, 30, 500);
   hjets["4th_pt"] = new TH1F("jet4_pt"+hname,"4th jet p_{T} [GeV/c]",50, 30, 500);
   hjets["eta"] = new TH1F("jet_eta"+hname,"jet #eta",50, -2.4, 2.4);
   hjets["phi"] = new TH1F("jet_phi"+hname,"jet #phi",50, 0, 3.15);
   hjets["Njets"] = new TH1F("Njets"+hname,"jet multiplicity",5,0,5);
   hjets["Nbtags_TCHPL"] = new TH1F("Nbjets_TCHPL_N3j"+hname,"Tagged b-jets",3,0,3);
   hjets["Nbtags_SSVHEM"] = new TH1F("Nbjets_TCHPL_N4j"+hname,"Tagged b-jets",3,0,3);
   
   
   // cut flow
   if (fChannel==1) 
     { //muon +jets
       fCutLabels.push_back("Processed");
       fCutLabels.push_back("OneIsoMu");
       fCutLabels.push_back("LooseMuVeto");
       fCutLabels.push_back("ElectronVeto");
       fCutLabels.push_back("MET");
       fCutLabels.push_back("1Jet");
       fCutLabels.push_back("2Jet");
       fCutLabels.push_back("3Jet");
       fCutLabels.push_back("4Jet");
       fCutLabels.push_back("2Jet1b");
     }
   else
     { //electron+jets
       fCutLabels.push_back("Processed");
       fCutLabels.push_back("OneIsoElectron");
       fCutLabels.push_back("LooseMuVeto");
       fCutLabels.push_back("ZVeto");
       fCutLabels.push_back("ConversionVeto");
       fCutLabels.push_back("MET");
       fCutLabels.push_back("1Jet");
       fCutLabels.push_back("2Jet");
       fCutLabels.push_back("3Jet");
       fCutLabels.push_back("4Jet");
       fCutLabels.push_back("2Jet1b");
     }
   hcutflow = new TH1D("cutflow","cut flow", fCutLabels.size(), 0.5, fCutLabels.size() +0.5 );

   for ( vector<string>::const_iterator ivec= fCutLabels.begin(); ivec!=fCutLabels.end(); ++ivec)
     {
       cutmap[ *ivec ] = 0;
     }

   double pu_weights[25] = { 
     0,
     0.216263,
     0.731377,
     1.1416,
     1.4592,
     1.58665,
     1.53217,
     1.37603,
     1.15313,
     0.950982,
     0.753041,
     0.591861,
     0.453312,
     0.344267,
     0.256831,
     0.187768,
     0.147568,
     0.110464,
     0.0824476,
     0.0645739,
     0.0480766,
     0.0308829,
     0.0248396,
     0.0160992,
     0.0171581,
   };

   fpu_weights_vec.assign( pu_weights, pu_weights + 25 );

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

  //if (entry>10) return kTRUE;

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
  TLorentzVector p4Nu, p4OtherNu;

  vector< TLorentzVector > p4jets;

  ////////////////////////////////////
  // PRIMARY VERTICES
  ///////////////////////////////////

  for ( size_t ipv=0; ipv < total_pvs; ++ipv) {

    if (ipv==0) PVz = primaryVertices[ipv].vz;

  }

  hPVs["N"]->Fill( total_pvs );
  // calculate PU weight
  double PUweight = 1.;

  if (fIsMC) {
    int iibin = 0;
    for ( vector<double>::iterator ivec = fpu_weights_vec.begin(); ivec != fpu_weights_vec.end(); ++ivec )
      {
	if ( ( iibin <= (int)total_pvs ) && ( (int)total_pvs < iibin + 1 ) ) PUweight = *ivec;
	iibin++;
      }
  }

  hPVs["Nreweight"]->Fill( total_pvs, PUweight );

  cutmap["Processed"] += PUweight;

  if (fVerbose) cout << "done pv" << endl;
  //////////////////////////////////
  // MUONS
  //////////////////////////////////
  int nloosemuons = 0;
  int ntightmuons = 0;
  double RelIso = -1.;
  double deltaR = -1.;

  for ( size_t imu=0; imu < total_muons; ++imu) {
     
    TopMuonEvent muon = muons[imu];
    
    h1test->Fill( muon.pt );
    hmuons["pt_cut1"]->Fill( muon.pt, PUweight );

    // select only good muons

    if ( fMuSelector.MuonLoose( muon ) ) 
      {

	nloosemuons++;

	if ( fMuSelector.MuonTight( muon, PVz) )  hmuons["pt_cut2"]->Fill( muon.pt, PUweight );
	if ( fMuSelector.MuonTightDeltaR( muon, PVz, jets) ) {
	  ntightmuons++;
	  deltaR = fMuSelector.GetDeltaR();
	}
	p4muon.SetPtEtaPhiE( muon.pt, muon.eta, muon.phi, muon.e );
	RelIso = muon.reliso03;
	
      }
  }

  if (fVerbose) cout << "done muons" << endl;
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
	    helectrons["pt_cut2"]->Fill( p4ele.Pt(), PUweight );
	    helectrons["eta_cut2"]->Fill( p4ele.Eta(), PUweight );
	    helectrons["phi_cut2"]->Fill( p4ele.Phi(), PUweight );

	  }
	ntightelectrons++;
      }
  }

  if (fVerbose) cout << "done electron" << endl;

  if ( fChannel == 1 ) // muon+jets
    {
      if ( ntightmuons != 1 ) return kTRUE;
      cutmap["OneIsoMu"] += PUweight;
      hmuons["pt"]->Fill( p4muon.Pt(), PUweight );
      hmuons["eta"]->Fill( p4muon.Eta(), PUweight );
      hmuons["phi"]->Fill( p4muon.Phi(), PUweight );
      hmuons["reliso"]->Fill( RelIso, PUweight );
      hmuons["deltaR"]->Fill( deltaR, PUweight );

      if ( nloosemuons > 1 ) return kTRUE;
      cutmap["LooseMuVeto"] += PUweight;

      if ( nlooseelectrons > 0 ) return kTRUE;
      cutmap["ElectronVeto"] += PUweight;

      p4lepton = p4muon;
      if (fVerbose) cout << "got a good lepton" << endl;
    }
  else // electron+jets
    {
      // pending ...
    }
  
  if (fVerbose) cout << "done lepton selection " << endl;

  /////////////////////////////////
  // MET
  /////////////////////////////////
  
  p4MET.SetPtEtaPhiE( ntuple->PFMET,
		      0.,
		      ntuple->PFMETphi,
		      ntuple->PFMET );

  if ( p4MET.Et() <= 20. ) return kTRUE;
  if (fVerbose) cout << "pass MET cut" << endl;

  cutmap["MET"] += PUweight;
  hMET["MET"]->Fill( p4MET.Pt(), PUweight );
  hMET["phi"]->Fill( p4MET.Phi(), PUweight );
  hMET["Ht"]->Fill( ntuple->PFHt, PUweight );
  hMET["Htlep"]->Fill( ntuple->PFHt + p4lepton.Pt(), PUweight );

  double Wpt = p4lepton.Pt() + p4MET.Pt();
  double Wpx = p4lepton.Px() + p4MET.Px();
  double Wpy = p4lepton.Py() + p4MET.Py();
  double WMt = sqrt(Wpt*Wpt-Wpx*Wpx-Wpy*Wpy);

  hM["WMt"]->Fill( WMt, PUweight );

  if (fVerbose) cout << "done MET " << endl;

  /////////////////////////////////
  // estimate Pz of neutrino
  ////////////////////////////////

  fzCalculator.SetMET(p4MET);
  fzCalculator.SetLepton(p4lepton);
  if (fChannel==2) 
    {
      fzCalculator.SetLeptonType("electron");
    }

  double pzNu = fzCalculator.Calculate();
  p4Nu = TLorentzVector();
  p4OtherNu = TLorentzVector();

  p4Nu.SetPxPyPzE(p4MET.Px(), p4MET.Py(), pzNu, sqrt(p4MET.Px()*p4MET.Px()+p4MET.Py()*p4MET.Py()+pzNu*pzNu));
  //print "pzNu = " +str(pzNu)
  //print "p4Nu =("+str(p4Nu.Px())+","+str(p4Nu.Py())+","+str(p4Nu.Pz())+","+str(p4Nu.E())
  double pzOtherNu = fzCalculator.getOther();
  p4OtherNu.SetPxPyPzE( p4MET.Px(), p4MET.Py(),pzOtherNu,sqrt(p4MET.Px()*p4MET.Px()+p4MET.Py()*p4MET.Py()+pzOtherNu*pzOtherNu));
  
  hMET["PzNu"]->Fill(pzNu, PUweight ); //change this to 2d with two sol and as a function of jets
                                                                                                                       
  TLorentzVector p4LepW = p4lepton + p4Nu;
  TLorentzVector p4OtherLepW = p4lepton + p4OtherNu;

  hMET["LepWmass"]->Fill(p4LepW.M(), PUweight );
  if ( fzCalculator.IsComplex() ) hMET["LepWmassComplex"]->Fill( p4LepW.M(), PUweight );


  /////////////////////////////////
  // JETS
  ////////////////////////////////
  
  int njets = 0;
  map< string, vector<float> > bdisc;
  map< string, vector<bool> >  isTagb;

  for ( size_t ijet=0; ijet < total_jets; ++ijet) 
    {

    TopJetEvent jet = jets[ijet];

    if ( jet.pt > 35. && fabs(jet.eta) < 2.4 ) 
      {
	if (fVerbose) cout << " jet pt " << jet.pt << endl;

	TLorentzVector tmpjet;
	tmpjet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e);
	p4jets.push_back( tmpjet);
	if (fVerbose) {
	  cout << "done storing njets " << njets << endl;
	  cout << " bdisc " << jet.btag_TCHP << endl;
	  cout << " bdisc " << jet.btag_SSVHE << endl;
	}
	// store discriminators
	bdisc["TCHP"].push_back( jet.btag_TCHP );
	bdisc["SSVHE"].push_back( jet.btag_SSVHE );
	if (fVerbose) cout << "store bdisc" << endl;
	// check TCHPL
	if ( jet.btag_TCHP > 1.19 ) isTagb["TCHPL"].push_back(true);
	else isTagb["TCHPL"].push_back(false);
	if (fVerbose) cout << "done tchpl" << endl;
	// check SSVHEM
	if ( jet.btag_SSVHE > 1.74) isTagb["SSVHEM"].push_back(true);
	else isTagb["SSVHEM"].push_back(false);
	if (fVerbose) cout << "done ssvem" << endl;

	njets++;
      }
    }

  hjets["Njets"]->Fill( njets, PUweight );

  if (fVerbose) cout << "done jets" << endl;

  if (njets > 0 ) cutmap["1Jet"] += PUweight;
  if (njets > 1 ) cutmap["2Jet"] += PUweight;
  if (njets > 2 ) cutmap["3Jet"] += PUweight;
  if (njets > 3 ) cutmap["4Jet"] += PUweight;

  if (njets == 1)
    {
      hjets["1st_pt"]->Fill( p4jets[0].Pt(), PUweight );
     }

  if (njets > 1 )
    {
      hmuons["pt_2jet"]->Fill( p4lepton.Pt(), PUweight );
      hMET["MET_2jet"]->Fill( p4MET.Pt(), PUweight );
      hjets["Nbtags_TCHPL"]->Fill( int(isTagb["TCHPL"].size()), PUweight );
      hjets["Nbtags_SSVHEM"]->Fill( int(isTagb["SSVHEM"].size()), PUweight );

      TLorentzVector p4Dijet = p4jets[0] + p4jets[1];
      TLorentzVector p4Wprime = p4LepW + p4Dijet;
      hM["Wprime"]->Fill( p4Wprime.M(), PUweight );

      if (fVerbose) cout << "Wprime mass calculated" << endl;

      if ( int(isTagb["TCHPL"].size()) == 0 )
        {
          hM["Wprime_0btag"]->Fill( p4Wprime.M(), PUweight );
        }

      if ( int(isTagb["TCHPL"].size()) >= 1 )
	{
	  cutmap["2Jet1b"] += PUweight;
	  hM["Wprime_1btag"]->Fill( p4Wprime.M(), PUweight );
	}
    }

  if (njets==2)
    {
      hjets["2nd_pt"]->Fill( p4jets[1].Pt(), PUweight );
    }

  if (njets==3)
    {
      hjets["3rd_pt"]->Fill( p4jets[2].Pt(), PUweight );
    }

  if (njets >= 4)
    {
      hjets["4th_pt"]->Fill( p4jets[3].Pt(), PUweight );
    }

  if (fVerbose) cout << "done analysis" << endl;

  return kTRUE;
}

void Analyzer::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

  // fill cutflow histogram

  int ibin = 1;
  for ( vector<string>::const_iterator ivec= fCutLabels.begin(); ivec != fCutLabels.end(); ++ivec )
       //  for ( map<string, int >::const_iterator imap=cutmap.begin(); imap!=cutmap.end(); ++imap )
    {
      hcutflow->SetBinContent( ibin, cutmap[ *ivec ] );
      ibin++;
    }
  // Write the ntuple to the file
  if (fFile) {
    Bool_t cleanup = kFALSE;
    TDirectory *savedir = gDirectory;
    if ( h1test->GetEntries() > 0) {
      fFile->cd();
      h1test->Write();
      hcutflow->Write();
      fFile->mkdir("muons");
      fFile->cd("muons");
      for ( map<string,TH1* >::const_iterator imap=hmuons.begin(); imap!=hmuons.end(); ++imap )
        {
          TH1 *temp = imap->second;
          if ( temp->GetEntries() > 0 )
            temp->Write();
          //else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
        }
      fFile->cd();
      fFile->mkdir("PVs");
      fFile->cd("PVs");
      for ( map<string,TH1* >::const_iterator imap=hPVs.begin(); imap!=hPVs.end(); ++imap )
        {
          TH1 *temp = imap->second;
          if ( temp->GetEntries() > 0 )
            temp->Write();
          //else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;                                                                                                      
        }
      fFile->cd();
      fFile->mkdir("jets");
      fFile->cd("jets");
      for ( map<string,TH1* >::const_iterator imap=hjets.begin(); imap!=hjets.end(); ++imap )
        {
          TH1 *temp = imap->second;
          if ( temp->GetEntries() > 0 )
            temp->Write();
        }
      fFile->cd();
      fFile->mkdir("mass");
      fFile->cd("mass");
      for ( map<string,TH1* >::const_iterator imap=hM.begin(); imap!=hM.end(); ++imap )
        {
          TH1 *temp = imap->second;
          if ( temp->GetEntries() > 0 )
            temp->Write();
        }
      fFile->cd();
      fFile->mkdir("MET");
      fFile->cd("MET");
      for ( map<string,TH1* >::const_iterator imap=hMET.begin(); imap!=hMET.end(); ++imap )
        {
          TH1 *temp = imap->second;
          if ( temp->GetEntries() > 0 )
            temp->Write();
        }

      fFile->cd();

      fProofFile->Print();
      fOutput->Add(fProofFile);
    } else {
      cleanup = kTRUE;
    }
    h1test->SetDirectory(0);
    hcutflow->SetDirectory(0);
    gDirectory = savedir;
    fFile->Close();
    // Cleanup, if needed
    if (cleanup) {
      TUrl uf(*(fFile->GetEndpointUrl()));
      SafeDelete(fFile);
      gSystem->Unlink(uf.GetFile());
      SafeDelete(fProofFile);
    }
  }

}

void Analyzer::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  Info("Terminate","Analyzer done.");
}
