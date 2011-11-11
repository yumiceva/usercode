
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
#include "BTagWeight.h"
//#include "Yumiceva/TreeAnalyzer/interface/JetCombinatorics.h"

#include <TStyle.h>
#include <TSystem.h>

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

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
  if (fMyOpt.Contains("JECUP")) { fdoJECunc = true; fdoJECup = true; }
  if (fMyOpt.Contains("JECDOWN")) { fdoJECunc = true; fdoJECup = false; }
  if (fMyOpt.Contains("PUUP")) { fpuhistogram = "WHistUp";}
  if (fMyOpt.Contains("PUDOWN")) { fpuhistogram = "WHistDown";}
  if (fMyOpt.Contains("QCD1")) fdoQCD1SideBand = true;
  if (fMyOpt.Contains("QCD2")) fdoQCD2SideBand = true;
  if (fMyOpt.Contains("mtop")) fdoMtopCut = true;
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
      if (fdoJECunc && fdoJECup==true) fSample += "_JECUP";
      if (fdoJECunc && fdoJECup==false) fSample += "_JECDOWN";
      if (fMyOpt.Contains("PUUP")) fSample += "_PUUP";
      if (fMyOpt.Contains("PUDOWN")) fSample += "_PUDOWN";

      Info("Begin","Histogram names will have suffix: %s", fSample.Data());

      if ( fSample.Contains("data") )
	{
	  fIsMC = false;
	  Info("Begin","This sample is treated as DATA");
	  if (fdoQCD1SideBand) fSample = "dataQCD1";
	  if (fdoQCD2SideBand) fSample = "dataQCD2";
	}
      else
	{
	  Info("Begin","This sample is treated as MC");
	  Info("Begin","we will use pileup set: %s", fpuhistogram.Data());
	}
      
      
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
     if (out) fProofFile->SetOutputFileName(fOutdir + out->GetTitle());
   }

   // Open the file
   //TDirectory *savedir = gDirectory;
   if (!(fFile = fProofFile->OpenFile("RECREATE"))) {
     Warning("SlaveBegin", "problems opening file: %s/%s",
	     fProofFile->GetDir(), fProofFile->GetFileName());
   }

   // Get PU weights
   TString weightfilename = "/uscms/home/yumiceva/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/Weight3D.root";
   fweightfile =  new TFile(weightfilename,"read");
   f3Dweight = (TH1D*) fweightfile->Get(fpuhistogram);

   //create histograms
   h1test = new TH1F("h1test","muon p_{T}",100,10.,400);
   //fHist = new HistoManager(string(fSample));
   TString hname = "_"+fSample;
   
   hmuons["pt_1jet"] = new TH1F("muon_pt_1jet"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt_2jet"] = new TH1F("muon_pt_2jet"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt_fin"] = new TH1F("muon_pt_fin"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["eta"] = new TH1F("muon_eta"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_1jet"] = new TH1F("muon_eta_1jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_2jet"] = new TH1F("muon_eta_2jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_fin"] = new TH1F("muon_eta_fin"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["phi"] = new TH1F("muon_phi"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["phi_1jet"] = new TH1F("muon_phi_1jet"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["phi_2jet"] = new TH1F("muon_phi_2jet"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["phi_fin"] = new TH1F("muon_phi_fin"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["reliso"] = new TH1F("muon_reliso"+hname,"Relative Isolation", 40, 0, 0.2);
   hmuons["reliso_1jet"] = new TH1F("muon_reliso_1jet"+hname,"Relative Isolation", 40, 0, 0.2);
   hmuons["reliso_2jet"] = new TH1F("muon_reliso_2jet"+hname,"Relative Isolation", 40, 0, 0.2);
   hmuons["reliso_fin"] = new TH1F("muon_reliso_fin"+hname,"Relative Isolation", 40, 0, 0.2);
   hmuons["deltaR_cut0"] = new TH1F("deltaR_cut0"+hname,"#DeltaR(#mu,jet)",80, 0, 4);
   hmuons["deltaR"] = new TH1F("deltaR"+hname,"#DeltaR(#mu,jet)", 80, 0, 4);
   hmuons["d0_cut1"] = new TH1F("d0_cut1"+hname,"#mu Impact Parameter [cm]",20,-0.1,0.1);
   hmuons["pt_cut1"] = new TH1F("muon_pt_cut1"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt_cut2"] = new TH1F("muon_pt_cut2"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt_cut3"] = new TH1F("muon_pt_cut3"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt"] = new TH1F("muon_pt"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["dz"] = new TH1F("dz"+hname,"|z(#mu) - z_{PV}| [cm]", 25, 0, 1.);
   hmuons["Niso"] = new TH1F("Niso"+hname,"Number of Primary Vertices", 25,-0.5,24.5);
   hmuons["Ngood"] = new TH1F("Ngood"+hname,"Number of Primary Vertices",25,-0.5,24.5);

   hPVs["N"] = new TH1F("NPV"+hname,"Number of PVs",25,-0.5,24.5);
   hPVs["Nreweight"] = new TH1F("NPVreweight"+hname,"Number of PVs",25,-0.5,24.5);
   hPVs["Nreweight_2jet"] = new TH1F("NPVreweight_2jet"+hname,"Number of PVs",25,-0.5,24.5);

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
   helectrons["reliso"] = new TH1F("electron_reliso"+hname,"Relative Isolation", 40, 0, 0.2);
   helectrons["reliso_1jet"] = new TH1F("electron_reliso_1jet"+hname,"Relative Isolation", 40, 0, 0.2);
   helectrons["reliso_2jet"] = new TH1F("electron_reliso_2jet"+hname,"Relative Isolation", 40, 0, 0.2);
   helectrons["reliso_3jet"] = new TH1F("electron_reliso_3jet"+hname,"Relative Isolation", 40, 0, 0.2);
   helectrons["reliso_4jet"] = new TH1F("electron_reliso_4jet"+hname,"Relative Isolation", 40, 0, 0.2);
   helectrons["deltaR_cut0"] = new TH1F("electron_deltaR_cut0"+hname,"#DeltaR(#mu,jet)",80, 0, 4);
   helectrons["deltaR"] = new TH1F("electron_deltaR"+hname,"#DeltaR(#mu,jet)",80, 0, 4);
   helectrons["d0_cut1"] = new TH1F("electron_d0_cut1"+hname,"#mu Impact Parameter [cm]",20,-0.1,0.1);
   helectrons["dz"] = new TH1F("electron_dz"+hname,"|z(#mu) - z_{PV}| [cm]", 25, 0, 1.);
      
   hMET["MET"] = new TH1F("MET"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["MET_2jet"] = new TH1F("MET_2jet"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["MET_fin"] = new TH1F("MET_fin"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["genMET_2jet"] = new TH1F("genMET_2jet"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["deltaMET_2jet"] = new TH1F("deltaMET_2jet"+hname,"Missing Transverse Energy [GeV]", 50, -200, 200);
   hMET["phi"] = new TH1F("MET_phi"+hname,"#phi Missing Transverse Energy [GeV]", 20, 0, 3.15);
   hMET["Ht"] = new TH1F("Ht"+hname,"H_{T} [GeV]", 50, 0, 1000);
   hMET["Htlep"] = new TH1F("Htlep"+hname,"H_{T,lep} [GeV]", 50, 0, 1000);
   hMET["PzNu"] = new TH1F("PzNu"+hname,"p_{z} #nu [GeV]", 40, -300,300);
   hMET["EtaNu"] = new TH1F("EtaNu"+hname,"#eta",50,-2.2,2.2);
   hMET["LepWmass"] = new TH1F("LepWmass"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150);
   hMET["LepWmass_topcut"] = new TH1F("LepWmass_topcut"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150);
   hMET["LepWmassNoPt"]=new TH1F("LepWmassNoPt"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150);
   hMET["deltaPhi"] = new TH1F("deltaPhi"+hname,"#Delta #phi(#mu,MET)",50, -3.15, 3.15);

   hM["WMt"] = new TH1F("Mt"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 300);
   hM["WMt_2jet"] = new TH1F("Mt_2jet"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 300);
   hM["WMt_fin"] = new TH1F("Mt_fin"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 300);
   hM["dijet"] = new TH1F("dijet"+hname,"(jj) mass [GeV/c^{2}]", 50, 0, 1000);
   hM["trijet"] = new TH1F("trijet"+hname,"(jjj) mass [GeV/c^{2}]", 50, 0, 1000);
   hM["top_1btag"] = new TH1F("top_1btag"+hname,"top mass [GeV/c^{2}]",50,0,500);
   hM["top_2btag"] = new TH1F("top_2btag"+hname,"top mass [GeV/c^{2}]",50,0,500);
   hM["Wprime"] = new TH1F("Wprime"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
   hM["Wprime_0btag"] = new TH1F("Wprime_0btag"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
   hM["Wprime_0btag_light"] = new TH1F("Wprime_0btag_light"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
   hM["Wprime_0btag_bb"] = new TH1F("Wprime_0btag_bb"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
   hM["Wprime_0btag_cc"] = new TH1F("Wprime_0btag_cc"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
   hM["Wprime_1btag"] = new TH1F("Wprime_1btag"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
   hM["Wprime_1btag_systUp"] = new TH1F("Wprime_1btag_systUp"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
   hM["Wprime_1btag_systDown"] = new TH1F("Wprime_1btag_systDown"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
   if (fSample == "WJets")
     {
       hM["Wprime_0btag_light"] = new TH1F("Wprime_0btag_light"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
       hM["Wprime_0btag_bb"] = new TH1F("Wprime_0btag_bb"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
       hM["Wprime_0btag_cc"] = new TH1F("Wprime_0btag_cc"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);       
       hM["Wprime_1btag_light"] = new TH1F("Wprime_1btag_light"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
       hM["Wprime_1btag_bb"] = new TH1F("Wprime_1btag_bb"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
       hM["Wprime_1btag_cc"] = new TH1F("Wprime_1btag_cc"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
     }
   hM["Wprime_1onlybtag"] = new TH1F("Wprime_1onlybtag"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
   hM["Wprime_2btag"] = new TH1F("Wprime_2btag"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
   hM["Wprime_2btag_systUp"] = new TH1F("Wprime_2btag_systUp"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
   hM["Wprime_2btag_systDown"] = new TH1F("Wprime_2btag_systDown"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
   if ( fSample.Contains("Wprime") )
     {
       hM["Wprime_0btag_MChad"] = new TH1F("Wprime_0btag_MChad"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
       hM["Wprime_0btag_MCsemimu"] = new TH1F("Wprime_0btag_MCsemimu"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
       hM["Wprime_0btag_MCsemie"] = new TH1F("Wprime_0btag_MCsemie"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
       hM["Wprime_1btag_MChad"] = new TH1F("Wprime_1btag_MChad"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
       hM["Wprime_1btag_MCsemimu"] = new TH1F("Wprime_1btag_MCsemimu"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
       hM["Wprime_1btag_MCsemie"] = new TH1F("Wprime_1btag_MCsemie"+hname,"W' invariant mass [GeV/c^{2}]", 70, 100, 3000);
     }

   hjets["pt"] = new TH1F("jet_pt"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_b"] = new TH1F("jet_pt_b"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_c"] = new TH1F("jet_pt_c"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_l"] = new TH1F("jet_pt_l"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_btag"] = new TH1F("jet_pt_btag"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_btag_b"] = new TH1F("jet_pt_btag_b"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_btag_c"] = new TH1F("jet_pt_btag_c"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_btag_l"] = new TH1F("jet_pt_btag_l"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["1st_pt"] = new TH1F("jet1_pt"+hname,"Leading jet p_{T} [GeV/c]",60, 30, 800);
   hjets["2nd_pt"] = new TH1F("jet2_pt"+hname,"2nd jet p_{T} [GeV/c]",60, 30, 800);
   hjets["3rd_pt"] = new TH1F("jet3_pt"+hname,"3rd jet p_{T} [GeV/c]",60, 30, 800);
   hjets["4th_pt"] = new TH1F("jet4_pt"+hname,"4th jet p_{T} [GeV/c]",60, 30, 800);
   hjets["1st_pt_fin"] = new TH1F("jet1_pt_fin"+hname,"Leading jet p_{T} [GeV/c]",60, 30, 800);
   hjets["2nd_pt_fin"] = new TH1F("jet2_pt_fin"+hname,"2nd jet p_{T} [GeV/c]",60, 30, 800);
   hjets["eta"] = new TH1F("jet_eta"+hname,"jet #eta",50, -2.4, 2.4);
   hjets["1st_eta"] = new TH1F("jet1_eta"+hname,"Leading jet #eta",50, -2.4, 2.4);
   hjets["2nd_eta"] = new TH1F("jet2_eta"+hname,"2nd jet #eta",50, -2.4, 2.4);
   hjets["1st_eta_fin"] = new TH1F("jet1_eta_fin"+hname,"Leading jet #eta",50, -2.4, 2.4);
   hjets["2nd_eta_fin"] = new TH1F("jet2_eta_fin"+hname,"2nd jet #eta",50, -2.4, 2.4);
   hjets["phi"] = new TH1F("jet_phi"+hname,"jet #phi",50, 0, 3.15);
   hjets["Njets"] = new TH1F("Njets"+hname,"jet multiplicity",6,0.5,6.5);
   hjets["Njets_1tag"] = new TH1F("Njets_1tag"+hname,"jet multiplicity",6,0.5,6.5);
   hjets["Njets_2tag"] = new TH1F("Njets_2tag"+hname,"jet multiplicity",6,0.5,6.5);
   hjets["Nbtags_TCHPM"] = new TH1F("Nbjets_TCHPM_N2j"+hname,"Tagged b-jets",3,-0.5,2.5);
   hjets["Nbtags_SSVHEM"] = new TH1F("Nbjets_SSVHEM_N2j"+hname,"Tagged b-jets",3,-0.5,2.5);
   hjets["Dijet_deltaPhi"] = new TH1F("Dijet_deltaPhi"+hname,"#Delta #phi(j,j)",30,-3.15,3.15);
   hjets["tb_deltaPhi"] = new TH1F("tb_deltaPhi"+hname,"#Delta #phi(t,b)",30,0.,3.15);
   hjets["tb_deltaEta"] = new TH1F("tb_deltaEta"+hname,"#Delta #eta(t,b)",30,-5,5);
   h2_pt_Wprime = new TH2F("toppt_vs_Wprime"+hname,"top p_{T} vs W' mass",60,0,1500,70, 100, 3000);
   hjets["pt_Wprime"] = new TH1F("pt_Wprime"+hname,"W' p_{T} [GeV]",50,0,200);
   hjets["pt_top"]  = new TH1F("pt_top"+hname,"top p_{T} [GeV]",60,0,1500);
   hjets["pt_b"]  = new TH1F("pt_b"+hname,"b-jet p_{T} [GeV]",60,0,1500);
   hjets["gen_deltaR_mub"] = new TH1F("gen_deltaR_mub"+hname,"#Delta R(#mu,b)",40,0,4);

   map<string,TH1* > allhistos = hmuons;
   allhistos.insert( helectrons.begin(), helectrons.end() );
   allhistos.insert( hMET.begin(), hMET.end() );
   allhistos.insert( hM.begin(), hM.end() );
   allhistos.insert( hjets.begin(), hjets.end() );

   for ( map<string,TH1* >::const_iterator imap=allhistos.begin(); imap!=allhistos.end(); ++imap )
     {
       TH1 *temp = imap->second;
       temp->Sumw2();
       temp->SetXTitle( temp->GetTitle() );
     }

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
       fCutLabels.push_back("2Jet2b");
       fCutLabels.push_back("MaxJets");
       fCutLabels.push_back("phi");
       fCutLabels.push_back("topmass");
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
       fCutLabels.push_back("2Jet2b");
     }
   hcutflow = new TH1D("cutflow","cut flow", fCutLabels.size(), 0.5, fCutLabels.size() +0.5 );

   for ( vector<string>::const_iterator ivec= fCutLabels.begin(); ivec!=fCutLabels.end(); ++ivec)
     {
       cutmap[ *ivec ] = 0;
     }

   double pu_weights[35] = {
     0.0255506,
     0.251923,
     0.549605,
     0.924918,
     1.25977,
     1.48142,
     1.57923,
     1.57799,
     1.52152,
     1.4414,
     1.35889,
     1.2841,
     1.21927,
     1.16125,
     1.11244,
     1.06446,
     1.01666,
     0.966989,
     0.913378,
     0.85774,
     0.799258,
     0.734225,
     0.670242,
     0.607641,
     0.54542,
     0.484084,
     0.427491,
     0.369787,
     0.321454,
     0.280706,
     0.238499,
     0.198961,
     0.166742,
     0.146428,
     0.224425
   };
   
   fpu_weights_vec.assign( pu_weights, pu_weights + 35 );

   // For JEC uncertainties
   if (fdoJECunc) fJECunc = new JetCorrectionUncertainty("/uscms/home/yumiceva/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/GR_R_42_V19_AK5PF_Uncertainty.txt");

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
  vector< TopElectronEvent > electrons = ntuple->PFelectrons; // use PF electrons (gsf collection is called "electrons")
  vector< TopJetEvent > jets = ntuple->PFjets;

  // USE PF Isolation
  fMuSelector.UsePFIsolation(true);
  fEleSelector.UsePFIsolation(true);

  size_t total_pvs   = primaryVertices.size();
  size_t total_muons = muons.size();
  size_t total_electrons = electrons.size();
  size_t total_jets  = jets.size();

  float PVz = 0.;
  TLorentzVector p4muon, p4ele, p4lepton, p4MET;
  TLorentzVector p4Nu, p4OtherNu;
  TLorentzVector p4QCDmuon;

  vector< TLorentzVector > p4jets;

  ////////////////////
  // GENERATOR
  ///////////////////
  if (fIsMC && fSample.Contains("Wp") )
    {
      TLorentzVector p4genLepton;
      TLorentzVector p4genNu;
      TLorentzVector p4genb;
      if (ntuple->gen.bLep_pt>0)
	{
	  p4genLepton.SetPtEtaPhiE(ntuple->gen.mu_pt,ntuple->gen.mu_eta,ntuple->gen.mu_phi,ntuple->gen.mu_e);
	  p4genNu.SetPtEtaPhiE(ntuple->gen.nu_pt,ntuple->gen.nu_eta,ntuple->gen.nu_phi,ntuple->gen.nu_e);
	  p4genb.SetPtEtaPhiE(ntuple->gen.bLep_pt,ntuple->gen.bLep_eta,ntuple->gen.bLep_phi,ntuple->gen.bLep_e);
	  
	  hjets["gen_deltaR_mub"]->Fill( p4genLepton.DeltaR( p4genb ) ); 
	}
    }
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

    Int_t      mc_npvminus1 = TMath::Min(ntuple->gen.Bx_minus1,34);
    Int_t      mc_npv0   = TMath::Min(ntuple->gen.Bx_0,34);
    Int_t      mc_npvplus1  = TMath::Min(ntuple->gen.Bx_plus1,34);

    PUweight = f3Dweight->GetBinContent(mc_npvminus1,mc_npv0,mc_npvplus1);

    //int iibin = 0;
    //for ( vector<double>::iterator ivec = fpu_weights_vec.begin(); ivec != fpu_weights_vec.end(); ++ivec )
    // {
    //	int mc_npvs = ntuple->gen.Bx_0; // in-time pile up
    ////int mc_npvs = (int)total_pvs;
    //if ( mc_npvs >= iibin+1 ) PUweight = *ivec; // use the last weight for last bin
    //if ( ( iibin <= mc_npvs ) && ( mc_npvs < iibin + 1 ) ) PUweight = *ivec; 
    //iibin++;
    //}
  }

  hPVs["Nreweight"]->Fill( total_pvs, PUweight );

  cutmap["Processed"] += PUweight;

  if (fVerbose) cout << "done pv" << endl;
  //////////////////////////////////
  // MUONS
  //////////////////////////////////
  int ngoodIDmuons = 0;
  int nloosemuons = 0;
  int ntightmuons = 0;
  int nqcdmuons = 0;

  double RelIso = -1.;
  double deltaR = -1.;
  double QCDRelIso = -1.;
  double QCDdeltaR = -1;

  for ( size_t imu=0; imu < total_muons; ++imu) {
     
    TopMuonEvent muon = muons[imu];
    
    h1test->Fill( muon.pt );
    hmuons["pt_cut1"]->Fill( muon.pt, PUweight );

    if ( fMuSelector.MuonID( muon, PVz ) ) ngoodIDmuons++;

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
    // check muon in QCD control region
    if ( fMuSelector.MuonRelax02IsoQCD( muon, PVz, jets ) ) 
      {
	nqcdmuons++;
	// keep the leading muon for selection
	if (nqcdmuons==1) {
	  p4QCDmuon.SetPtEtaPhiE( muon.pt, muon.eta, muon.phi, muon.e );
	  QCDRelIso = muon.reliso03;
	  QCDdeltaR = fMuSelector.GetDeltaR();
	}
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

      if (fdoQCD2SideBand) {

	if (nqcdmuons  == 0) return kTRUE;
	cutmap["OneIsoMu"] += PUweight;

	p4lepton = p4QCDmuon;
	RelIso = QCDRelIso;
	deltaR = QCDdeltaR;
      } else {

	if ( ngoodIDmuons > 0 ) hmuons["Ngood"]->Fill( total_pvs, PUweight);
	if ( ntightmuons > 0 ) hmuons["Niso"]->Fill( total_pvs, PUweight);

	if ( ntightmuons != 1 ) return kTRUE;
	cutmap["OneIsoMu"] += PUweight;

	if ( nloosemuons > 1 ) return kTRUE;
	cutmap["LooseMuVeto"] += PUweight;
	
	if ( nlooseelectrons > 0 ) return kTRUE;
	cutmap["ElectronVeto"] += PUweight;
	
	p4lepton = p4muon;
	if (fVerbose) cout << "got a good lepton" << endl;
      }

      hmuons["pt"]->Fill( p4lepton.Pt(), PUweight );
      hmuons["eta"]->Fill( p4lepton.Eta(), PUweight );
      hmuons["phi"]->Fill( p4lepton.Phi(), PUweight );
      hmuons["reliso"]->Fill( RelIso, PUweight );
      hmuons["deltaR"]->Fill( deltaR, PUweight );

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

  //temporal check using genMET
  //p4MET.SetPtEtaPhiE( ntuple->gen.MET,
  //                    0.,
  //                    ntuple->gen.METPhi,
  //                    ntuple->gen.MET );

  if (fdoQCD1SideBand && p4MET.Et() > 20.) return kTRUE;
  else if ( p4MET.Et() <= 20. && fdoQCD2SideBand==false ) return kTRUE;

  if (fVerbose) cout << "pass MET cut" << endl;

  //cutmap["MET"] += PUweight;
  hMET["MET"]->Fill( p4MET.Pt(), PUweight );
  hMET["phi"]->Fill( p4MET.Phi(), PUweight );
  hMET["Ht"]->Fill( ntuple->PFHt, PUweight );
  hMET["Htlep"]->Fill( ntuple->PFHt + p4lepton.Pt(), PUweight );

  double Wpt = p4lepton.Pt() + p4MET.Pt();
  double Wpx = p4lepton.Px() + p4MET.Px();
  double Wpy = p4lepton.Py() + p4MET.Py();
  double WMt = sqrt(Wpt*Wpt-Wpx*Wpx-Wpy*Wpy);

  if ( WMt <= 40. ) return kTRUE;
  cutmap["MET"] += PUweight;

  if (fVerbose) cout << "pass W MT cut " << endl;

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

  double WmassNoPt = (p4Nu+p4lepton).M();

  if ( fzCalculator.IsComplex() )
    {
      double ptNu1 = fzCalculator.getPtneutrino(1);
      double ptNu2 = fzCalculator.getPtneutrino(2);
      TLorentzVector p4Nu1tmp;
      TLorentzVector p4Nu2tmp;

      p4Nu1tmp.SetPxPyPzE( ptNu1*p4MET.Px()/p4MET.Pt(), ptNu1*p4MET.Py()/p4MET.Pt(), pzNu, sqrt(ptNu1*ptNu1+pzNu*pzNu));
      p4Nu2tmp.SetPxPyPzE( ptNu2*p4MET.Px()/p4MET.Pt(), ptNu2*p4MET.Py()/p4MET.Pt(), pzNu, sqrt(ptNu2*ptNu2+pzNu*pzNu));
      
      TLorentzVector Wtmp;
      Wtmp = p4lepton + p4Nu1tmp;
      double Wm1 = 0;
      double Wm2 = 0;
      Wm1 = Wtmp.M();
      Wtmp = p4lepton + p4Nu2tmp;
      Wm2 = Wtmp.M();
      if ( fabs( Wm1 - 80.) < fabs( Wm2 - 80.) ) p4Nu = p4Nu1tmp;
      else p4Nu = p4Nu2tmp;

      p4OtherNu = p4Nu; // since we chose the real part, the two solutions are the same.
    }


  hMET["PzNu"]->Fill(pzNu, PUweight ); //change this to 2d with two sol and as a function of jets
                                                                                                                       
  TLorentzVector p4LepW = p4lepton + p4Nu;
  TLorentzVector p4OtherLepW = p4lepton + p4OtherNu;

  //hMET["LepWmass"]->Fill(p4LepW.M(), PUweight );
  //if ( fzCalculator.IsComplex() ) hMET["LepWmassComplex"]->Fill( p4LepW.M(), PUweight );


  /////////////////////////////////
  // JETS
  ////////////////////////////////
  
  //JetCombinatorics myCombi = JetCombinatorics();

  int njets = 0;
  map< string, vector<float> > bdisc;
  map< string, vector<bool> >  isTagb;
  vector<int> listflavor;

  for ( size_t ijet=0; ijet < total_jets; ++ijet) 
    {

    TopJetEvent jet = jets[ijet];
    double SF_JEC = 1.;
    if (fdoJECunc)
      {
	fJECunc->setJetEta( jet.eta);
	fJECunc->setJetPt( jet.pt);
	double jec_unc = fJECunc->getUncertainty(true);
	if (fVerbose) cout << "JEC uncertainty is " << jec_unc << endl;
	if (fdoJECup) SF_JEC = 1.+jec_unc;
	else SF_JEC = 1.-jec_unc;
      }

    if ( SF_JEC*jet.pt > 35. && fabs(jet.eta) < 2.4 && SF_JEC*jets[0].pt > 120. ) 
      {
	if (fVerbose) cout << " jet pt " << SF_JEC*jet.pt << endl;
	
	//hjets["pt"]->Fill( jet.pt, PUweight );
	//hjets["eta"]->Fill( jet.eta, PUweight );
	//hjets["phi"]->Fill( jet.phi, PUweight );

	TLorentzVector tmpjet;
	tmpjet.SetPtEtaPhiE(SF_JEC*jet.pt, jet.eta, jet.phi, SF_JEC*jet.e);
	p4jets.push_back( tmpjet);
	listflavor.push_back( jet.mc.flavor );

	if (fVerbose) {
	  cout << "done storing njets " << njets << endl;
	  cout << " bdisc " << jet.btag_TCHP << endl;
	  cout << " bdisc " << jet.btag_SSVHE << endl;
	}
	// store discriminators
	bdisc["TCHP"].push_back( jet.btag_TCHP );
	bdisc["SSVHE"].push_back( jet.btag_SSVHE );
	if (fVerbose) cout << "store bdisc" << endl;
	// TCHPL cut at 1.19
	// check TCHPM cut at 1.93
	if ( jet.btag_TCHP > 1.93 ) isTagb["TCHPM"].push_back(true);
	else isTagb["TCHPM"].push_back(false);
	if (fVerbose) cout << "done tchpl" << endl;
	// check SSVHEM cut at 1.74
	if ( jet.btag_SSVHE > 1.74) isTagb["SSVHEM"].push_back(true);
	else isTagb["SSVHEM"].push_back(false);
	if (fVerbose) cout << "done ssvem" << endl;
	// CSVM cut at 0.679

	njets++;
      }
    }

  if (njets>0) hM["WMt"]->Fill( WMt, PUweight );

  hjets["Njets"]->Fill( njets, PUweight );

  if (fVerbose) cout << "done jets" << endl;

  if (njets > 0 ) cutmap["1Jet"] += PUweight;
  if (njets > 1 ) cutmap["2Jet"] += PUweight;
  if (njets > 2 ) cutmap["3Jet"] += PUweight;
  if (njets > 3 ) cutmap["4Jet"] += PUweight;

  //if (njets == 1)
  //  {
  //    hjets["1st_pt"]->Fill( p4jets[0].Pt(), PUweight );
  //   }

  if (njets > 1 )
    {
      // count partons
      int number_of_b = 0;
      int number_of_c = 0;
      int number_of_l = 0;
      int number_of_b_highpt = 0;
      int number_of_c_highpt = 0;
      float SFb_0tag = 1.;
      float SFb_only1tag = 1.;
      float SFb_1tag = 1.;
      float SFb_2tag = 1.;
      float SFb_0tag_syst[2] = {1.}; // for systematics
      float SFb_1tag_syst[2] = {1.};
      float SFb_2tag_syst[2] = {1.};
      //float SFb_1tag_systhighpt[2] = {1.};

      for ( size_t kk=0; kk < p4jets.size(); ++kk)
	{
	  hjets["pt"]->Fill( p4jets[kk].Pt(), PUweight );
	  if ( abs(listflavor[kk])==5 && p4jets[kk].Pt()<=240 ) { number_of_b++; hjets["pt_b"]->Fill( p4jets[kk].Pt(), PUweight );}
	  if ( abs(listflavor[kk])==4 && p4jets[kk].Pt()<=240 ) { number_of_c++; hjets["pt_c"]->Fill( p4jets[kk].Pt(), PUweight );}
	  if ( abs(listflavor[kk])==1 || abs(listflavor[kk])==2 || abs(listflavor[kk])==3 || abs(listflavor[kk])==21 ) 
	    { number_of_l++; hjets["pt_l"]->Fill( p4jets[kk].Pt(), PUweight );}
	  if ( abs(listflavor[kk])==5 && p4jets[kk].Pt()>240 ) { number_of_b_highpt++; hjets["pt_b"]->Fill( p4jets[kk].Pt(), PUweight );}
	  if ( abs(listflavor[kk])==4 && p4jets[kk].Pt()>240 ) { number_of_c_highpt++; hjets["pt_c"]->Fill( p4jets[kk].Pt(), PUweight );}

	  if ( isTagb["TCHPM"][kk] ) 
	    {
	      hjets["pt_btag"]->Fill( p4jets[kk].Pt(), PUweight );
	      if ( abs(listflavor[kk])==5 ) hjets["pt_btag_b"]->Fill( p4jets[kk].Pt(), PUweight );
	      if ( abs(listflavor[kk])==4 ) hjets["pt_btag_c"]->Fill( p4jets[kk].Pt(), PUweight );
	      if ( abs(listflavor[kk])==1 || abs(listflavor[kk])==2 || abs(listflavor[kk])==3 || abs(listflavor[kk])==21 ) hjets["pt_btag_l"]->Fill( p4jets[kk].Pt(), PUweight );
	    }
	}

      hPVs["Nreweight_2jet"]->Fill( total_pvs, PUweight );
      hjets["1st_pt"]->Fill( p4jets[0].Pt(), PUweight );
      hjets["2nd_pt"]->Fill( p4jets[1].Pt(), PUweight );
      hjets["1st_eta"]->Fill( p4jets[0].Eta(), PUweight );
      hjets["2nd_eta"]->Fill( p4jets[1].Eta(), PUweight );

      hmuons["pt_2jet"]->Fill( p4lepton.Pt(), PUweight );
      hmuons["reliso_2jet"]->Fill( RelIso, PUweight );
      hMET["MET_2jet"]->Fill( p4MET.Pt(), PUweight );

      if (fIsMC) {
	hMET["genMET_2jet"]->Fill( ntuple->gen.MET, PUweight );
	hMET["deltaMET_2jet"]->Fill( p4MET.Pt() - ntuple->gen.MET, PUweight );
      }

      hM["WMt_2jet"]->Fill( WMt, PUweight );

      TLorentzVector p4Dijet = p4jets[0] + p4jets[1];
      double Dijet_deltaPhi = p4jets[0].DeltaPhi(p4jets[1]);
      TLorentzVector p4Top = p4jets[2] + p4LepW;

      TLorentzVector p4Wprime = p4LepW + p4Dijet;
      hM["Wprime"]->Fill( p4Wprime.M(), PUweight );

      if (fVerbose) cout << "Wprime mass calculated" << endl;

      // count number of b-tags
      int Nbtags_TCHPM = 0;
      int Nbtags_SSVHEM = 0;
      for ( size_t itag=0; itag< isTagb["TCHPM"].size(); ++itag )
	{
	  if ( isTagb["TCHPM"][itag] ) Nbtags_TCHPM++;
	  if ( isTagb["SSVHEM"][itag] ) Nbtags_SSVHEM++;
	}

      // compute b-tag event weight
      if ( fIsMC )
        {

	  // zeto tag
          BTagWeight b0(0,0); // number of tags 
	  BTagWeight::JetInfo bj(0.63,0.91); // mean MC eff and mean SF. For TCHPM=0.91\pm0.09, CSVM=0.96\pm0.096
	  BTagWeight::JetInfo cj(0.15,0.91);
          double light_mceff = 0.017;
          if ( 100 < p4jets[0].Pt() && p4jets[0].Pt() <= 200 ) light_mceff = 0.04;
          if ( 200 < p4jets[0].Pt() && p4jets[0].Pt() <= 300 ) light_mceff = 0.08;
          if ( 300 < p4jets[0].Pt() && p4jets[0].Pt() <= 400 ) light_mceff = 0.12;
          if ( 400 < p4jets[0].Pt() ) light_mceff = 0.14;
	  BTagWeight::JetInfo lj(light_mceff,1.22); //for TCHPM=1.22, CSVM=1.08 \pm 0.13
	  // b-tag systematic UP 9% for b, 18% for c
	  BTagWeight::JetInfo bjUP(0.63,0.99);                                                                                                                                                                
	  BTagWeight::JetInfo cjUP(0.15,1.07);
	  // b-tag systemacit DOWN 9% for b, 18% for c
	  BTagWeight::JetInfo bjDOWN(0.63,0.83);
	  BTagWeight::JetInfo cjDOWN(0.15,0.75);
	  // for high pt jets > 240 UP 50% for b and c
	  BTagWeight::JetInfo bjUPhighpt(0.63,1.36);
	  BTagWeight::JetInfo cjUPhighpt(0.15,1.36);
          // for high pt jets > 240 UP 50% for b and c
	  BTagWeight::JetInfo bjDOWNhighpt(0.63,0.46);
	  BTagWeight::JetInfo cjDOWNhighpt(0.15,0.46);

          vector<BTagWeight::JetInfo> j;
          for(int i=0;i<number_of_b;i++)j.push_back(bj);
	  for(int i=0;i<number_of_b_highpt;i++)j.push_back(bj);
          for(int i=0;i<number_of_c;i++)j.push_back(cj);
	  for(int i=0;i<number_of_c_highpt;i++)j.push_back(cj);
          for(int i=0;i<number_of_l;i++)j.push_back(lj);
          
	  if (Nbtags_TCHPM==0) {
	    SFb_0tag = b0.weight(j,0);
	    hjets["Nbtags_TCHPM"]->Fill( Nbtags_TCHPM, PUweight*SFb_0tag ); // fill bin 0
	  }
	  // only one tag
	  BTagWeight b11(1,1); // number of tags
          if (Nbtags_TCHPM==1) {
	    SFb_only1tag = b11.weight(j,1);
	    hjets["Nbtags_TCHPM"]->Fill( Nbtags_TCHPM, PUweight*SFb_only1tag ); // fill bin 1
	  }
	  // at least one tag
	  BTagWeight b1(1,Nbtags_TCHPM); // number of tags
	  if (Nbtags_TCHPM>=1) {
	    SFb_1tag = b1.weight(j,1);

	    vector<BTagWeight::JetInfo> jj;
	    for(int i=0;i<number_of_b;i++)jj.push_back(bjUP);
	    for(int i=0;i<number_of_b_highpt;i++)jj.push_back(bjUPhighpt);
	    for(int i=0;i<number_of_c;i++)jj.push_back(cjUP);
	    for(int i=0;i<number_of_c_highpt;i++)jj.push_back(cjUPhighpt);
	    for(int i=0;i<number_of_l;i++)jj.push_back(lj);
	    SFb_1tag_syst[0] = b1.weight(jj,1); //UP
	    vector<BTagWeight::JetInfo> jk;
            for(int i=0;i<number_of_b;i++)jk.push_back(bjDOWN);
	    for(int i=0;i<number_of_b_highpt;i++)jk.push_back(bjDOWNhighpt);
            for(int i=0;i<number_of_c;i++)jk.push_back(cjDOWN);
	    for(int i=0;i<number_of_c_highpt;i++)jk.push_back(cjDOWNhighpt);
            for(int i=0;i<number_of_l;i++)jk.push_back(lj);
            SFb_1tag_syst[1] = b1.weight(jk,1);//DOWN
	    
	  }
	  // at least two tags
	  BTagWeight b2(2,Nbtags_TCHPM); // number of tags
	  if (Nbtags_TCHPM>=2) {
	    SFb_2tag = b2.weight(j,2);
	    hjets["Nbtags_TCHPM"]->Fill( Nbtags_TCHPM, PUweight*SFb_2tag ); // fill bin >=2
	    
	    vector<BTagWeight::JetInfo> jj;
            for(int i=0;i<number_of_b;i++)jj.push_back(bjUP);
	    for(int i=0;i<number_of_b_highpt;i++)jj.push_back(bjUPhighpt);
            for(int i=0;i<number_of_c;i++)jj.push_back(cjUP);
	    for(int i=0;i<number_of_c_highpt;i++)jj.push_back(cjUPhighpt);
            for(int i=0;i<number_of_l;i++)jj.push_back(lj);
            SFb_2tag_syst[0] = b2.weight(jj,2); //UP
                                                                                                                                                                                   
            vector<BTagWeight::JetInfo> jk;
            for(int i=0;i<number_of_b;i++)jk.push_back(bjDOWN);
	    for(int i=0;i<number_of_b_highpt;i++)jk.push_back(bjDOWNhighpt);
            for(int i=0;i<number_of_c;i++)jk.push_back(cjDOWN);
	    for(int i=0;i<number_of_c_highpt;i++)jk.push_back(cjDOWNhighpt);
            for(int i=0;i<number_of_l;i++)jk.push_back(lj);
            SFb_2tag_syst[1] = b2.weight(jk,2);//DOWN
	  }
        }
      else hjets["Nbtags_TCHPM"]->Fill( Nbtags_TCHPM );

      //hjets["Nbtags_TCHPM"]->Fill( Nbtags_TCHPM, PUweight*SFb );
      //hjets["Nbtags_SSVHEM"]->Fill( Nbtags_SSVHEM, PUweight*SFb );

      if ( Nbtags_TCHPM == 0 && njets < 5)
        {
	  // compute top mass
	  int top_bjet_index = 0;
	  double deltaM = 99999.;
	  //double top_mass = 0.;
	  //TLorentzVector p4Top;
	  TLorentzVector bestp4Top;
	  for ( size_t itejet = 0; itejet < p4jets.size(); ++itejet )
            {
	      
	      for ( int isolw =0; isolw<2; ++ isolw)
		{
		  
		  p4Top = p4jets[itejet] + p4LepW;
		  if (isolw==1) p4Top = p4jets[itejet] + p4OtherLepW;
		  
		  if ( fabs( p4Top.M() - 172.) < deltaM ) {
		    top_bjet_index = itejet;
		    //top_mass = p4Top.M();
		    deltaM = fabs( p4Top.M() - 172.);
		    bestp4Top = p4Top;
		  }
	      
		}
	    }
	  p4Top = bestp4Top;
	  bool passcutWlep = false;
	  if ( p4LepW.M() < 90 ) passcutWlep = true;

	  if (passcutWlep) {

	    if ( top_bjet_index == 0 ) {
	      p4Wprime = p4Top + p4jets[1];
	    }
	    else {
	      p4Wprime = p4Top + p4jets[0];
	    }
	
	    bool passcut = true;
            if (fdoMtopCut && ( p4Top.M() <= 130 || p4Top.M() >= 210 ) ) passcut = false;

            if (passcut) {

	      hM["Wprime_0btag"]->Fill( p4Wprime.M(), PUweight*SFb_0tag );
	      if (fSample=="WJets")
		{
		  int FH = ntuple->flavorHistory;
		  if ( FH == 1 || FH == 2 || FH == 5 || FH == 7 || FH == 9 ) hM["Wprime_0btag_bb"]->Fill( p4Wprime.M(), PUweight*SFb_0tag );
		  else if ( FH == 3 || FH == 4 || FH == 6 || FH == 8 || FH == 10) hM["Wprime_0btag_cc"]->Fill( p4Wprime.M(), PUweight*SFb_0tag );
		  else if ( FH == 11 ) hM["Wprime_0btag_light"]->Fill( p4Wprime.M(), PUweight*SFb_0tag );
		}
	      if (fSample.Contains("Wprime"))
		{
		  if ( ntuple->gen.LeptonicChannel == 11 ) hM["Wprime_0btag_MCsemie"]->Fill( p4Wprime.M(), PUweight*SFb_0tag );
		  else if ( ntuple->gen.LeptonicChannel == 13 ) hM["Wprime_0btag_MCsemimu"]->Fill( p4Wprime.M(), PUweight*SFb_0tag );
		  else hM["Wprime_0btag_MChad"]->Fill( p4Wprime.M(), PUweight*SFb_0tag );
		}
	    } // top mass cut
	  }// Wlep cut
	}// 0 tag

      // check if the two leading jets have a b jet 
      if ( Nbtags_TCHPM >= 1 && (isTagb["TCHPM"][0] || isTagb["TCHPM"][1]) )
	{

	  //cutmap["2Jet1b"] += PUweight*SFb_1tag;
	  hjets["Njets_1tag"]->Fill( njets, PUweight*SFb_1tag );
	  if (Nbtags_TCHPM > 1 ) hjets["Njets_2tag"]->Fill( njets, PUweight*SFb_2tag );

	  if ( njets < 5 ) {

	    //cutmap["2Jet1b"] += PUweight*SFb_1tag;
	  // calculate dijet mass closest to W mass
	  double the_dijet_mass = 0;
	  double sigma2 = 13.*13.;
	  double tmpchi2 = 999999.;
	  for ( size_t itejet = 0; itejet < p4jets.size(); ++itejet )
	    {
	      for ( size_t jtejet = itejet+1; jtejet < p4jets.size(); ++jtejet )
		{
		  TLorentzVector tmpvv = p4jets[itejet] + p4jets[jtejet];
		  double tmpmass = tmpvv.M();
		  if ( (tmpmass - 83.8 )*(tmpmass - 83.8)/sigma2 < tmpchi2 ) { tmpchi2 = (tmpmass - 83.8 )*(tmpmass - 83.8)/sigma2; the_dijet_mass = tmpmass; }
		} 
	    }
	  hM["dijet"]->Fill( the_dijet_mass, PUweight*SFb_1tag );
	  hjets["Dijet_deltaPhi"]->Fill( Dijet_deltaPhi, PUweight*SFb_1tag );
	  
	  // compute jjj mass for events with >=3 jets
	  if ( njets > 2 ) {
	    TLorentzVector p4Trijet = p4jets[0] + p4jets[1] + p4jets[2];
	    hM["trijet"]->Fill( p4Trijet.M(), PUweight*SFb_1tag );
	  }
	  // compute top mass
	  int top_bjet_index = 0;
	  double deltaM = 99999.;
	  //double top_mass = 0.;
	  //TLorentzVector p4Top;
	  TLorentzVector bestp4Top;
	  for ( size_t itejet = 0; itejet < p4jets.size(); ++itejet )
            {
	      
	      for ( int isolw =0; isolw<2; ++ isolw)
		{
		  
		  p4Top = p4jets[itejet] + p4LepW;
		  if (isolw==1) p4Top = p4jets[itejet] + p4OtherLepW;
		  
		  if ( fabs( p4Top.M() - 172.) < deltaM ) {
		    top_bjet_index = itejet;
		    //top_mass = p4Top.M();
		    deltaM = fabs( p4Top.M() - 172.);
		    bestp4Top = p4Top;
		  }
	      
		}
	    }
	  p4Top = bestp4Top;
	  hM["top_1btag"]->Fill( p4Top.M(), PUweight*SFb_1tag );
	  hMET["LepWmass"]->Fill(p4LepW.M(), PUweight*SFb_1tag );
	  hMET["LepWmassNoPt"]->Fill(WmassNoPt, PUweight*SFb_1tag );

	  bool passcutWlep = false;
	  if ( p4LepW.M() < 90 ) passcutWlep = true;

	  if (passcutWlep) {

	    if ( p4Top.M() > 130 && p4Top.M() < 210 ) hMET["LepWmass_topcut"]->Fill(p4LepW.M(), PUweight*SFb_1tag );

	    double tb_deltaPhi = 0.;
	    double tb_deltaR = 0.;
	    double tb_deltaEta = -999;
	    double pt_wprime = 0;
	    double pt_top = 0;
	    double pt_b = 0;
	    if ( top_bjet_index == 0 ) {
	      p4Wprime = p4Top + p4jets[1];
	      tb_deltaPhi = p4Top.DeltaPhi( p4jets[1] );
	      tb_deltaR = p4Top.DeltaR( p4jets[1] );
	      tb_deltaEta = p4Top.Eta() - p4jets[1].Eta();
	      pt_wprime = p4Wprime.Pt();
	      pt_top = p4Top.Pt();
	      pt_b = p4jets[1].Pt();
	    }
	    else {
	      p4Wprime = p4Top + p4jets[0];
	      tb_deltaPhi = fabs( p4Top.DeltaPhi( p4jets[0] ) );
	      tb_deltaR = p4Top.DeltaR( p4jets[0] );
	      tb_deltaEta = p4Top.Eta() - p4jets[0].Eta();
	      pt_wprime= p4Wprime.Pt();
	      pt_top = p4Top.Pt();
	      pt_b = p4jets[0].Pt();
	    }

	    bool passcut = true;
	    if (fdoMtopCut && ( p4Top.M() <= 130 || p4Top.M() >= 210 ) ) passcut = false;

	    if (passcut) {
	      cutmap["2Jet1b"] += PUweight*SFb_1tag;
	      h2_pt_Wprime->Fill( p4Top.Pt(), p4Wprime.M(), PUweight*SFb_1tag );
	      hM["Wprime_1btag"]->Fill( p4Wprime.M(), PUweight*SFb_1tag );
	      // FINAL Plots
	      hMET["deltaPhi"]->Fill( p4lepton.DeltaPhi( p4MET ), PUweight*SFb_1tag );
	      hjets["tb_deltaPhi"]->Fill( tb_deltaPhi, PUweight*SFb_1tag );
	      hjets["tb_deltaEta"]->Fill( tb_deltaEta, PUweight*SFb_1tag );
	      //hjets["tb_deltaR"]->Fill( tb_deltaR, PUweight*SFb_1tag );
	      hjets["pt_Wprime"]->Fill( pt_wprime, PUweight*SFb_1tag );
	      hjets["pt_top"]->Fill( pt_top, PUweight*SFb_1tag );
	      hjets["pt_b"]->Fill( pt_b, PUweight*SFb_1tag );
	      hjets["1st_pt_fin"]->Fill( p4jets[0].Pt(), PUweight*SFb_1tag );
	      hjets["2nd_pt_fin"]->Fill( p4jets[1].Pt(), PUweight*SFb_1tag );
	      hjets["1st_eta_fin"]->Fill( p4jets[0].Eta(), PUweight*SFb_1tag );
	      hjets["2nd_eta_fin"]->Fill( p4jets[1].Eta(), PUweight*SFb_1tag );
	      hmuons["pt_fin"]->Fill( p4lepton.Pt(), PUweight*SFb_1tag );
	      hmuons["reliso_fin"]->Fill( RelIso, PUweight*SFb_1tag );
	      hMET["MET_fin"]->Fill( p4MET.Pt(), PUweight*SFb_1tag );
	      hM["WMt_fin"]->Fill( WMt, PUweight*SFb_1tag );

	      if ( p4Wprime.M() > 1500.)
		{
		  TString outstring = "run: ";
		  outstring += TString(Form("%i",ntuple->run)) +" lumi: "+ TString(Form("%i",ntuple->lumi)) + " event: " + TString(Form("%i",ntuple->event)); 
		  Info("Process",outstring);
		}

	      if (fIsMC)
		{
		  hM["Wprime_1btag_systUp"]->Fill( p4Wprime.M(), PUweight*SFb_1tag_syst[0] );
		  hM["Wprime_1btag_systDown"]->Fill( p4Wprime.M(), PUweight*SFb_1tag_syst[1] );
		}

	      if (fSample.Contains("Wprime"))
		{
		  if ( ntuple->gen.LeptonicChannel == 11 ) hM["Wprime_1btag_MCsemie"]->Fill( p4Wprime.M(), PUweight*SFb_1tag );
		  else if ( ntuple->gen.LeptonicChannel == 13 ) hM["Wprime_1btag_MCsemimu"]->Fill( p4Wprime.M(), PUweight*SFb_1tag );
		  else hM["Wprime_1btag_MChad"]->Fill( p4Wprime.M(), PUweight*SFb_1tag );
		}

	      if (fSample=="WJets")
		{
		  int FH = ntuple->flavorHistory;
		  if ( FH == 1 || FH == 2 || FH == 5 || FH == 7 || FH == 9 ) hM["Wprime_1btag_bb"]->Fill( p4Wprime.M(), PUweight*SFb_1tag );
		  else if ( FH == 3 || FH == 4 || FH == 6 || FH == 8 || FH == 10) hM["Wprime_1btag_cc"]->Fill( p4Wprime.M(), PUweight*SFb_1tag );
		  else if ( FH == 11 ) hM["Wprime_1btag_light"]->Fill( p4Wprime.M(), PUweight*SFb_1tag );
		}

	      // check two b-tags
	      if ( Nbtags_TCHPM > 1 )
		{
		  cutmap["2Jet2b"] += PUweight*SFb_2tag;
		  hM["Wprime_2btag"]->Fill( p4Wprime.M(), PUweight*SFb_2tag );

		  if (fIsMC)
		    {
		      hM["Wprime_2btag_systUp"]->Fill( p4Wprime.M(), PUweight*SFb_2tag_syst[0] );
		      hM["Wprime_2btag_systDown"]->Fill( p4Wprime.M(), PUweight*SFb_2tag_syst[1] );
		    }
		}

	    }//passcut mtop cut if requested
	  }//passcutWlep cut
	  }// njets < 5

	}

      if ( Nbtags_TCHPM == 1 ) hM["Wprime_1onlybtag"]->Fill( p4Wprime.M(), PUweight*SFb_only1tag );

    }

  /*
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
  */

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
      h2_pt_Wprime->Write();
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

    h2_pt_Wprime->SetDirectory(0);

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
