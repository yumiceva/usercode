//#define SimpleAnalyzer_cxx
// The class definition in top.h has been generated automatically
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
// Root > T->Process("top.C")
// Root > T->Process("top.C","some options")
// Root > T->Process("top.C+")
//

#include "SimpleAnalyzer.h"

#include <TStyle.h>
#include <TSystem.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

void SimpleAnalyzer::ParseInput()
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
  if (fMyOpt.Contains("outdir")) {
    TString tmp = fMyOpt;
    tmp = tmp.Remove(0,fMyOpt.Index("outdir")+7);
    fOutdir = tmp+"/";
    Info("Begin","output files will be written to directory: %s", fOutdir.Data());
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
      else
        {
          Info("Begin","This sample is treated as MC");
        }


    }
}

void SimpleAnalyzer::CreateHistograms()
{
  h1test = new TH1F("h1test","muon p_{T}",100,10.,400);
  h1test->SetDirectory(fFile);

  TString hname = "_"+fSample;

  hmuons["eta"] = new TH1F("muon_eta"+hname,"#eta^{#mu}", 25, -2.5, 2.5);
  hmuons["phi"] = new TH1F("muon_phi"+hname,"#phi^{#mu}", 35, -3.5, 3.5);
  hmuons["pt"] = new TH1F("muon_pt"+hname,"p_{T}^{#mu} [GeV/c]", 25, 0, 500);

  //map<string,TH1* > allhistos = hmuons;                
}

void SimpleAnalyzer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   fMyOpt = option;
   ParseInput();

   Info("Begin", 
        "starting with process option: %s ", option.Data());

}

void SimpleAnalyzer::SlaveBegin(TTree * tree)
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
     fProofFile = new TProofOutputFile( "SimpleNtuple.root",TProofOutputFile::kDataset, opt, nm->GetTitle());
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
     if (out) fProofFile->SetOutputFileName(fOutdir + out->GetTitle());
   }
   // Open the file
   //TDirectory *savedir = gDirectory;
   if (!(fFile = fProofFile->OpenFile("RECREATE"))) {
     Warning("SlaveBegin", "problems opening file: %s/%s",
             fProofFile->GetDir(), fProofFile->GetFileName());
   }

   //create histograms
   CreateHistograms();

}

Bool_t SimpleAnalyzer::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either SimpleAnalyzer::GetEntry() or TBranch::GetEntry()
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

  fChain->GetEntry(entry);
  // get collections                                                                                                                          
  vector< TopVertexEvent > primaryVertices = ntuple->vertices;
  vector< TopMuonEvent > muons = ntuple->muons;
  vector< TopElectronEvent > electrons = ntuple->PFelectrons; 
  vector< TopJetEvent > jets = ntuple->PFjets;

  size_t total_muons = muons.size();

  TLorentzVector p4muon;
  int N_muons = 0;

  //////////////////////////////////
  // MUONS
  //////////////////////////////////       
  if (fVerbose) cout << "total number of muons = " << total_muons << endl;

  for ( size_t imu=0; imu < total_muons; ++imu) {

    TopMuonEvent muon = muons[imu];

    h1test->Fill( muon.pt );

    if (muon.pt > 32. && fabs(muon.eta)<2.1 && muon.IsTrackerMuon==1 ) {
      if ( N_muons == 0 ) p4muon.SetPtEtaPhiE( muon.pt, muon.eta, muon.phi, muon.e );
      N_muons++;
    }
  }

  if (fVerbose) cout << "muons done." << endl;

  if (fVerbose) cout << "analysis done." << endl;
  return kTRUE;
}

void SimpleAnalyzer::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

  // Write the ntuple to the file                                                                                                             
  if (fFile) {
    Bool_t cleanup = kFALSE;
    TDirectory *savedir = gDirectory;
    if (fVerbose) cout << "number of entries in h1test histogram = " << h1test->GetEntries() << endl;

    if(h1test->GetEntries() > 0){
      fFile->cd();
      h1test->Write();
      h1test->SetDirectory(0);
      fFile->mkdir("muons");
      fFile->cd("muons");
      for ( map<string,TH1* >::const_iterator imap=hmuons.begin(); imap!=hmuons.end(); ++imap ) {
        TH1 *temp = imap->second;
        if ( temp->GetEntries() > 0 )
          temp->Write();
        //else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;             
      }
    } else {
      cleanup = kTRUE;
    }

    gDirectory = savedir;
    fFile->Close();
    // Cleanup, if needed
                                                                                                                            
    if (cleanup) {
      Info("SlaveTerminate", "nothing to save: just cleanup everything ...");
      TUrl uf(*(fFile->GetEndpointUrl()));
      SafeDelete(fFile);
      gSystem->Unlink(uf.GetFile());
      SafeDelete(fProofFile);
    } else {

      Info("SlaveTerminate", "objects saved into '%s%s': sending related TProofOutputFile ...",
           fProofFile->GetFileName(), fProofFile->GetOptionsAnchor());
      fProofFile->Print();
      fOutput->Add(fProofFile);
    }
  }

}

void SimpleAnalyzer::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  
  Info("Terminate","Analyzer done.");
}
