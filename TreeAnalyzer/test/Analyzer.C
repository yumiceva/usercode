
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
   TDirectory *savedir = gDirectory;
   if (!(fFile = fProofFile->OpenFile("RECREATE"))) {
     Warning("SlaveBegin", "problems opening file: %s/%s",
	     fProofFile->GetDir(), fProofFile->GetFileName());
   }

   //create histograms
   h1test = new TH1F("h1test","muon p_{T}",100,10.,400);
   //fHist = new HistoManager(string(fSample));
   TString hname = "_"+fSample;
   hmuons["pt"] = new TH1F("muon_pt"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 300);
   hmuons["pt_1jet"] = new TH1F("muon_pt_1jet"+hname,"p_{T}^{#mu} [GeV/c]", 60, 20, 500);
   hmuons["pt_2jet"] = new TH1F("muon_pt_2jet"+hname,"p_{T}^{#mu} [GeV/c]", 60, 20, 500);
   hmuons["pt_3jet"] = new TH1F("muon_pt_3jet"+hname,"p_{T}^{#mu} [GeV/c]", 60, 20, 500);
   hmuons["pt_4jet"] = new TH1F("muon_pt_4jet"+hname,"p_{T}^{#mu} [GeV/c]", 60, 20, 500);
   hmuons["eta"] = new TH1F("muon_eta"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_1jet"] = new TH1F("muon_eta_1jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_2jet"] = new TH1F("muon_eta_2jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_3jet"] = new TH1F("muon_eta_3jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_4jet"] = new TH1F("muon_eta_4jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["phi"] = new TH1F("muon_phi"+hname,"#phi^{#mu}", 20, 0, 3.15);
   hmuons["phi_1jet"] = new TH1F("muon_phi_1jet"+hname,"#phi^{#mu}", 20, 0, 3.15);
   hmuons["phi_2jet"] = new TH1F("muon_phi_2jet"+hname,"#phi^{#mu}", 20, 0, 3.15);
   hmuons["phi_3jet"] = new TH1F("muon_phi_3jet"+hname,"#phi^{#mu}", 20, 0, 3.15);
   hmuons["phi_4jet"] = new TH1F("muon_phi_4jet"+hname,"#phi^{#mu}", 20, 0, 3.15);
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
   hmuons["dz"] = new TH1F("dz"+hname,"|z(#mu) - z_{PV}| [cm]", 25, 0, 1.);

   hPVs["N"] = new TH1F("NPV"+hname,"Number of PVs",25,-0.5,24.5);
   hPVs["Nreweight"] = new TH1F("NPVreweight"+hname,"Number of PVs",25,-0.5,24.5);

   // cut flow
   fCutLabels.push_back("Processed");
   fCutLabels.push_back("OneIsoMu");
   fCutLabels.push_back("LooseMuVeto");
   fCutLabels.push_back("ElectronVeto");
   fCutLabels.push_back("MET");
   fCutLabels.push_back("1Jet");
   fCutLabels.push_back("2Jet");
   fCutLabels.push_back("3Jet");
   fCutLabels.push_back("4Jet");

   hcutflow = new TH1F("cutflow","cut flow", fCutLabels.size(), 0.5, fCutLabels.size() +0.5 );

   for ( vector<string>::const_iterator ivec= fCutLabels.begin(); ivec!=fCutLabels.end(); ++ivec)
     {
       cutmap[ *ivec ] = 0;
     }

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

  cutmap["Processed"]++;

  //if (entry>10) return kTRUE;

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

  hPVs["N"]->Fill( total_pvs );
  hPVs["Nreweight"]->Fill( total_pvs );

  if (fVerbose) cout << "done pv" << endl;
  //////////////////////////////////
  // MUONS
  //////////////////////////////////
  int nloosemuons = 0;
  int ntightmuons = 0;

  for ( size_t imu=0; imu < total_muons; ++imu) {
     
    TopMuonEvent muon = muons[imu];
    
    h1test->Fill( muon.pt );
    hmuons["pt"]->Fill( muon.pt );

    // select only good muons

    if ( fMuSelector.MuonLoose( muon ) ) 
      {

	nloosemuons++;

	if ( fMuSelector.MuonTight( muon, PVz) ) ntightmuons++;

	p4muon.SetPtEtaPhiE( muon.pt, muon.eta, muon.phi, muon.e );

      }
  }

  hmuons["pt_cut3"]->Fill( p4muon.Pt() );

  if (fVerbose) cout << "done mu" << endl;
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

  if (fVerbose) cout << "done electron" << endl;

  if ( ntightmuons != 1 ) return kTRUE;
  cutmap["OneIsoMu"]++;

  if ( nloosemuons > 1 ) return kTRUE;
  cutmap["LooseMuVeto"]++;

  if ( total_electrons > 0 ) return kTRUE;
  cutmap["ElectronVeto"]++;

  p4lepton = p4muon;
  
  if (fVerbose) cout << "done lepton selection " << endl;

  /////////////////////////////////
  // MET
  /////////////////////////////////
  
  p4MET.SetPtEtaPhiE( ntuple->PFMET,
		      0.,
		      ntuple->PFMETphi,
		      ntuple->PFMET );

  if ( p4MET.Et() <= 20. ) return kTRUE;
  cutmap["MET"]++;

  if (fVerbose) cout << "done MET " << endl;
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
	if (fVerbose) cout << " jet pt " << jet.pt << endl;

	TLorentzVector tmpjet;
	tmpjet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e);
	p4jets.push_back( tmpjet);
	if (fVerbose) cout << "done storing njets " << njets << endl;

	if (fVerbose) cout << " bdisc " << jet.btag_TCHP << endl;
	if (fVerbose) cout << " bdisc " << jet.btag_SSVHE << endl;

	bdisc["TCHP"].push_back( jet.btag_TCHP );
	bdisc["SSVHE"].push_back( jet.btag_SSVHE );
	if (fVerbose) cout << "store bdisc" << endl;
	if ( jet.btag_TCHP > 1.19 ) isTagb["TCHPL"].push_back(true);
	else isTagb["TCHPL"].push_back(false);
	if (fVerbose) cout << "done tchpl" << endl;
	if ( jet.btag_SSVHE > 1.74) isTagb["SSVHEM"].push_back(true);
	else isTagb["SSVHEM"].push_back(false);
	if (fVerbose) cout << "done ssvem" << endl;

	njets++;
      }
    }

  if (fVerbose) cout << "done jets" << endl;
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
