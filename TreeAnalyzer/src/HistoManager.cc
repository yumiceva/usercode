
#include "Yumiceva/TreeAnalyzer/interface/HistoManager.h"

#include "TH1F.h"

void HistoManager::CreateMuons()
{
  hmuons["pt"] = new TH1F("muon_pt"+fname,"p_{T}^{#mu} [GeV/c]", 25, 20, 100);
  hmuons["pt_1jet"] = new TH1F("muon_pt_1jet"+fname,"p_{T}^{#mu} [GeV/c]", 25, 20, 100);
  hmuons["pt_2jet"] = new TH1F("muon_pt_2jet"+fname,"p_{T}^{#mu} [GeV/c]", 25, 20, 100);
  hmuons["pt_3jet"] = new TH1F("muon_pt_3jet"+fname,"p_{T}^{#mu} [GeV/c]", 25, 20, 100);
  hmuons["pt_4jet"] = new TH1F("muon_pt_4jet"+fname,"p_{T}^{#mu} [GeV/c]", 25, 20, 100);
  hmuons["eta"] = new TH1F("muon_eta"+fname,"#eta^{#mu}", 20, -2.1, 2.1);
  hmuons["eta_1jet"] = new TH1F("muon_eta_1jet"+fname,"#eta^{#mu}", 20, -2.1, 2.1);
  hmuons["eta_2jet"] = new TH1F("muon_eta_2jet"+fname,"#eta^{#mu}", 20, -2.1, 2.1);
  hmuons["eta_3jet"] = new TH1F("muon_eta_3jet"+fname,"#eta^{#mu}", 20, -2.1, 2.1);
  hmuons["eta_4jet"] = new TH1F("muon_eta_4jet"+fname,"#eta^{#mu}", 20, -2.1, 2.1);
  hmuons["phi"] = new TH1F("muon_phi"+fname,"#phi^{#mu}", 20, 0, 3.15);
  hmuons["phi_1jet"] = new TH1F("muon_phi_1jet"+fname,"#phi^{#mu}", 20, 0, 3.15);
  hmuons["phi_2jet"] = new TH1F("muon_phi_2jet"+fname,"#phi^{#mu}", 20, 0, 3.15);
  hmuons["phi_3jet"] = new TH1F("muon_phi_3jet"+fname,"#phi^{#mu}", 20, 0, 3.15);
  hmuons["phi_4jet"] = new TH1F("muon_phi_4jet"+fname,"#phi^{#mu}", 20, 0, 3.15);
  hmuons["reliso"] = new TH1F("muon_reliso"+fname,"Relative Isolation", 30, 0, 1.5);
  hmuons["reliso_1jet"] = new TH1F("muon_reliso_1jet"+fname,"Relative Isolation", 30, 0, 1.5);
  hmuons["reliso_2jet"] = new TH1F("muon_reliso_2jet"+fname,"Relative Isolation", 30, 0, 1.5);
  hmuons["reliso_3jet"] = new TH1F("muon_reliso_3jet"+fname,"Relative Isolation", 30, 0, 1.5);
  hmuons["reliso_4jet"] = new TH1F("muon_reliso_4jet"+fname,"Relative Isolation", 30, 0, 1.5);
  hmuons["deltaR_cut0"] = new TH1F("deltaR_cut0"+fname,"#DeltaR(#mu,jet)",80, 0, 4);
  hmuons["deltaR"] = new TH1F("deltaR"+fname,"#DeltaR(#mu,jet)",80, 0, 4);
  hmuons["d0_cut1"] = new TH1F("d0_cut1"+fname,"#mu Impact Parameter [cm]",20,-0.1,0.1);
  hmuons["pt_cut1"] = new TH1F("muon_pt_cut1"+fname,"p_{T}^{#mu} [GeV/c]", 25, 0, 100);
  hmuons["pt_cut2"] = new TH1F("muon_pt_cut2"+fname,"p_{T}^{#mu} [GeV/c]", 25, 0, 100);
  hmuons["pt_cut3"] = new TH1F("muon_pt_cut3"+fname,"p_{T}^{#mu} [GeV/c]", 25, 0, 100);
  hmuons["dz"] = new TH1F("dz"+fname,"|z(#mu) - z_{PV}| [cm]", 25, 0, 1.);

}
