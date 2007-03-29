#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TFile.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TMarker.h"

void plot_contour() {

  TGraph *g_1;
  TGraph *g_8;
  TGraph *g_7;
  TGraph *g_4;

  TCanvas *cv1 = new TCanvas("cv1","cv1",700,700);
  TMultiGraph *multi = new TMultiGraph();

  TFile* input_ro_file1 = TFile::Open("PythiaGenerator/lhc_2008_1_1001_20000_results.root");
  
  Double_t xpt,ypt;

  g_1 = (TGraph*) gDirectory->Get("gra1_sig2");
  g_1->SetName("g_1");

  int ipt = g_1->GetN() -1;
  g_1->GetPoint(0,xpt,ypt);
  g_1->SetPoint(ipt,xpt,ypt);
  //g_1->SetLineColor();
  //g_1->Draw("alp");

  TFile* input_ro_file8 = TFile::Open("PythiaGenerator/lhc_2008_1_1008_20000_results.root");

  g_8 = (TGraph*) gDirectory->Get("gra1_sig2");
  g_8->SetName("g_8");
  g_8->GetPoint(0,xpt,ypt);
  g_8->SetPoint(ipt,xpt,ypt);
  //g_8->Draw("lp");

  TFile* input_ro_file7 = TFile::Open("PythiaGenerator/lhc_2008_1_1007_20000_results.root");

  g_7 = (TGraph*) gDirectory->Get("gra1_sig2");
  g_7->SetName("g_7");
  g_7->GetXaxis()->SetTitle("x0 [cm]");
  g_7->GetYaxis()->SetTitle("y0 [cm]");
  g_7->GetPoint(0,xpt,ypt);
  g_7->SetPoint(ipt,xpt,ypt);
  //g_7->Draw("lp");

  TFile* input_ro_file4 = TFile::Open("PythiaGenerator/lhc_2008_1_1004_20000_results.root");

  g_4 = (TGraph*) gDirectory->Get("gra1_sig2");
  g_4->SetName("g_4");
  g_4->GetPoint(0,xpt,ypt);
  g_4->SetPoint(ipt,xpt,ypt);
  //multi->Add(g_4,"lp");
  //g_4->Draw("lp");
  
  multi->Add(g_4,"lp");
  multi->Add(g_7,"lp");
  multi->Add(g_8,"lp");
  multi->Add(g_1,"lp");
  multi->Draw("a");
   
  TMarker *point = new TMarker(0.03,0.06,29);
  point->Draw();
  
}
