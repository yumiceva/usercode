
#include "MakePlots.h"

#include "TString.h"
#include "TSystem.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TObject.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TKey.h"
#include "TImageDump.h"
#include "TLegend.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

std::vector<std::string> getAllKeys (const TDirectory* fDir, const std::string& fClassName) {
  //std::cout << "getAllKeys-> " << fDir->GetName() << ", " <<  fClassName << std::endl;
  //  fDir->ls();
  std::vector<std::string> result;
  TIter next (fDir->GetListOfKeys ());
  for (TKey* key = 0; (key = (TKey *) next());) {
    //std::cout << "key from list: " << key->GetName()  << '/' << key->GetClassName () << std::endl;
    if (fClassName == key->GetClassName ()) {
      result.push_back (std::string (key->GetName ()));
    } 
  }
  return result;
}

TObject* getObject (TDirectory* fDir, const std::vector <std::string>& fObjectName) {
  TObject* result = 0; // nothing so far
  TDirectory* dir = fDir;
  for (unsigned i = 0; i < fObjectName.size (); ++i) {
    dir->GetObject (fObjectName[i].c_str(), result);
    if (result) {
      if (i < fObjectName.size () - 1) {
	dir = (TDirectory*) result;
	result = 0;
      }
    }
    else {
      std::cerr << "[make_plots] getObject-> Can not find (sub)dir/object " << fObjectName[i] << " in directory " << dir->GetName () << std::endl;
      return 0;
    }
  }
  return result;
}

TH1* HistoCompare(TH1 *h, TH1 *refHisto) {

  std::cout << "Comparing " << h->GetName() << " and " << refHisto->GetName() << std::endl;
  std::cout << " entries " << h->GetEntries() << " and " << refHisto->GetEntries() << std::endl;

  if (h->GetEntries() == 0) {
    TH1 *resHisto = (TH1*) refHisto->Clone(TString("Reference"));
    resHisto->SetTitle("Reference");
    return resHisto;
  }
  //create a residual histogram
  //if (resHisto_) delete resHisto_;
  TH1 *resHisto = (TH1*) h->Clone(TString(h->GetName())+"_residuals");
  resHisto->Reset();

  // clone input histogram
  TH1 *htemp = (TH1*) h->Clone(TString(h->GetName()));

  
  // normalize histograms
  htemp->Sumw2();
  refHisto->Sumw2();

    
  if (htemp->Integral() != 0 ) htemp->Scale(1./htemp->Integral());
  if (refHisto->Integral() != 0) refHisto->Scale(1./refHisto->Integral());
        
  resHisto->Add( htemp, refHisto, 1., -1.);

  resHisto->SetTitle("Residuals");
  resHisto->SetYTitle("");
 
  return resHisto;
  
}

MakePlots::MakePlots(TString root_filename, TString webpath, TString extension, bool compare, TString compare_filename, bool logaxis) {

  // get a file
  TFile *afile = new TFile(root_filename);

  TFile *arefFile = 0;
  if ( compare ) {
    Long_t *id =0; Long_t *size = 0; Long_t *flags=0; Long_t *mt=0;
    if ( gSystem->GetPathInfo(compare_filename,id,size,flags,mt) == 0 ) {
			
      arefFile = new TFile(compare_filename);
      if ( arefFile->IsZombie() ) {
	std::cout << "[make_plots] is zombie? Error opening file " << compare_filename << " will not compare plots." << std::endl;
	compare = false;
      }
    } else {
      compare = false;
      std::cout << "[make_plots] file exists? Error opening file " << compare_filename << " will not compare plots." << std::endl;
    }
  }
  // get filename without extension
  TString thename = root_filename.Remove(0,webpath.Length());
  thename = thename.Remove(thename.Length()-5,thename.Length());
	
  gSystem->mkdir(webpath);
	
  TLatex *label = new TLatex(0.01,0.01,thename);
  label->SetNDC();
  label->SetTextColor(15);
  label->SetTextSize(0.02);
	
	
  std::map<std::string, TCanvas*> cv_map;
  TString name1 = "_C_";
  TString name2 = "_DUSG_";
	
	
  std::vector<std::string> dirName1 = getAllKeys ( afile , "TDirectory");
  std::cout << "dirName1.size=" << dirName1.size() << std::endl;

  if (dirName1.size()==0) {
    std::vector<std::string> histKeys = getAllKeys (gDirectory, "TH1F");

    for (unsigned ihist = 0; ihist < histKeys.size (); ++ihist) {
      TH1* hist = 0;
      gDirectory->GetObject (histKeys[ihist].c_str(), hist);

      if (hist) {

	std::string cvname = hist->GetName();
			
	cv_map["cv_"+cvname] = new TCanvas("cv_"+TString(cvname),"cv_"+TString(hist->GetName()),800,800);
			
	if (compare) {
	  // increase size of window
	  cv_map["cv_"+cvname]->SetWindowSize(800,1600);
				
	  cv_map["cv_"+cvname]->Divide(1,2);
				
	  cv_map["cv_"+cvname]->cd(1);
	  //hist->SetXTitle(hist->GetTitle());
	  hist->Draw();
	  label->Draw();
	  if (logaxis) {
	    std::cout << "  make log Y-axis scale" << std::endl;
	    gPad->SetLogy();
	    gPad->SetGrid();
	  }

	  cv_map["cv_"+cvname]->cd(2);
	  std::vector<std::string> histPathName;
	  //histPathName.push_back (dirName1[idir]);
	  //histPathName.push_back (dirName2[idir2]);
	  histPathName.push_back (histKeys[ihist]);
	  TH1* refhist = (TH1*) getObject (arefFile, histPathName);
	  if (refhist) {
	    double Chi2Test = 0.;
	    double KGTest = 0.;
	    Chi2Test = refhist->Chi2Test(hist,"UFOF");
	    KGTest   = refhist->KolmogorovTest(hist,"UO");
	    std::cout << "  chi2= " << Chi2Test << " KG= " << KGTest << std::endl;
					
	    TH1* reshist = HistoCompare(hist,refhist);
					
								
	    //refhist->Draw();
	    reshist->Draw();
					
	    //std::cout << " reshist= " << reshist->GetEntries() << std::endl;
	    char buf[1024];
	    sprintf (buf, "#chi^{2}= %1.2f", Chi2Test);
	    TLatex *labelchi2 = new TLatex(0.4,0.91,buf);
	    labelchi2->SetNDC();
	    //if (Chi2Test<0.1) gPad->SetFillColor(2);
	    labelchi2->SetTextSize(0.035);
	    labelchi2->Draw();
	    cv_map["cv_"+cvname]->Update();
	    //std::cout << "print chi2 label"<<std::endl;
	    sprintf (buf, "KG= %2.2f", KGTest);
	    TLatex *labelkg = new TLatex(0.6,0.91,buf);
	    labelkg->SetNDC();
	    if (KGTest<0.8) gPad->SetFillColor(2);
	    labelkg->SetTextSize(0.035);
	    labelkg->Draw();
	    //std::cout << " draw kg label"<<std::endl;
					
	  } else {
					
	    std::cout << " no reference plot " << histKeys[ihist] << std::endl;
	    TLatex *label = new TLatex(0.2,0.5,"no reference plot available");
	    label->SetNDC();
	    label->SetTextSize(0.07);
	    label->Draw();
	  }
	} else {
	  //hist->SetXTitle(hist->GetTitle());
	  hist->Draw();
	  label->Draw();
	  if (logaxis) {
	    std::cout << "  make log Y-axis scale" << std::endl;
	    gPad->SetLogy();
	    gPad->SetGrid();
	  }
	}


	cv_map["cv_"+cvname]->cd();
	std::cout << " print canvas" << std::endl;
	cv_map["cv_"+cvname]->Print(webpath+"/"+TString(cvname)+"."+extension);
			
	if ( extension=="eps" ) {
	  gSystem->Exec("cp "+webpath+"/"+TString(cvname)+"."+extension+" temp.eps");
	  gSystem->Exec("pstopnm -ppm -xborder 0 -yborder 0 -portrait temp.eps");
	  gSystem->Exec("ppmtogif temp.eps001.ppm > "+webpath+"/"+TString(cvname)+".gif");
	  gSystem->Exec("rm -rf temp.eps temp.eps001.ppm");
	  gSystem->Exec("rm "+webpath+"/"+TString(cvname)+"."+extension);
	}
	std::cout << " done"<<std::endl;
      }
      else {
	std::cerr << "Can not get histogram " << histKeys[ihist] << std::endl;
      }
    }

    std::vector<TH1*> histos_C;
    std::vector<TH1*> histos_DUSG;

    for (unsigned ihist = 0; ihist < histKeys.size (); ++ihist) {
      TH1* histo = 0;
      gDirectory->GetObject (histKeys[ihist].c_str(), histo);
      TString  HistoName = histo->GetName() ;
      // cout << " Histogram Name: " << HistoName << endl;	
	    
      if ( HistoName.Contains(name1)) {histos_C.push_back(histo);}
      if ( HistoName.Contains(name2)) {histos_DUSG.push_back(histo);}
      //cout << " Histos_C.size: " << histos_C.size() << endl;	
      //cout << " Histos_DUSG.size: " << histos_DUSG.size() << endl;
    }
    for (int i=0; i < histos_C.size(); ++i) {
      std::string cvname = histos_C[i]->GetName();
      //cout << cvname << endl;
      cv_map["merged_"+cvname] = new TCanvas("merged_"+TString(cvname), "merged_"+TString(histos_C[i]->GetName()),800,800);
      if (logaxis) {
	std::cout << "  make log Y-axis scale" << std::endl;
	gPad->SetLogy();
	gPad->SetGrid();
      }
      TH1F *mytmp = (TH1F*) histos_C[i];
      mytmp->SetMaximum(1); 
      mytmp->SetMinimum(10e-4); 
      mytmp->Draw();
      TString title= mytmp->GetTitle();
      title = title.Remove(0,24);
      title = title.Remove(title.Length()-8, title.Length());
      //cout << "title:" << title <<endl;
      mytmp->SetTitle(title);
      TH1F *mytmp2 = (TH1F*) histos_DUSG[i];
      mytmp2->SetMarkerColor(kRed);
      mytmp2->Draw("same");
      TLegend *leg1 = new TLegend(0.1,0.75,0.25,0.85,"","NDC");
      leg1->SetTextSize(0.04);
      leg1->SetFillColor(kWhite);
      leg1->AddEntry(mytmp,"C","P");
      leg1->AddEntry(mytmp2,"DUSG","P");
      leg1->Draw();
      std::cout << "Drew Merged Histos " << std::endl;
      cv_map["merged_"+cvname]->Print(webpath+"/"+"merged_"+TString(cvname)+"."+extension);

      if ( extension=="eps" ) {
	gSystem->Exec("cp "+webpath+"/"+TString(cvname)+"."+extension+" temp.eps");
	gSystem->Exec("pstopnm -ppm -xborder 0 -yborder 0 -portrait temp.eps");
	gSystem->Exec("ppmtogif temp.eps001.ppm > "+webpath+"/"+TString(cvname)+".gif");
	gSystem->Exec("rm -rf temp.eps temp.eps001.ppm");
	gSystem->Exec("rm "+webpath+"/"+TString(cvname)+"."+extension);
      }
      std::cout << " done merging "<<std::endl;
    }
  } else {
    for (unsigned idir = 0; idir < dirName1.size(); ++idir) {
      std::cout << " directory: " << dirName1[idir] << std::endl;
      TDirectory* dir1 = 0;
      afile->GetObject (dirName1[idir].c_str(), dir1);
      if (dir1) {

	std::vector<std::string> histKeys = getAllKeys (dir1, "TH1F");
	if ( histKeys.size() != 0 ) {

	  for (unsigned ihist = 0; ihist < histKeys.size (); ++ihist) {
	    TH1* hist = 0;
	    dir1->GetObject (histKeys[ihist].c_str(), hist);
				
	    if (hist) {
	      std::string cvname = hist->GetName();
						
	      cv_map["cv_"+cvname] = new TCanvas("cv_"+TString(cvname),
						 "cv_"+TString(hist->GetName()),800,800);
						
	      if (compare) {
		cv_map["cv_"+cvname]->Divide(1,2);
							
		cv_map["cv_"+cvname]->cd(1);
		//hist->SetXTitle(hist->GetTitle());
		hist->Draw();
		label->Draw();
		if (logaxis) {
		  std::cout << "  make log Y-axis scale" << std::endl;
		  gPad->SetLogy();
		  gPad->SetGrid();
		}

		cv_map["cv_"+cvname]->cd(2);
		std::vector<std::string> histPathName;
		histPathName.push_back (dirName1[idir]);
		//histPathName.push_back (dirName2[idir2]);
		histPathName.push_back (histKeys[ihist]);
		TH1* refhist = (TH1*) getObject (arefFile, histPathName);
		if (refhist) {
		  double Chi2Test = 0.;
		  double KGTest = 0.;
		  Chi2Test = refhist->Chi2Test(hist,"UFOF");
		  KGTest   = refhist->KolmogorovTest(hist,"UO");
		  std::cout << "  chi2= " << Chi2Test << " KG= " << KGTest << std::endl;
								
		  TH1* reshist = HistoCompare(hist,refhist);
														
								
		  //refhist->Draw();
		  reshist->Draw();
								
		  //std::cout << " reshist= " << reshist->GetEntries() << std::endl;
		  char buf[1024];
		  sprintf (buf, "#chi^{2}= %1.2f", Chi2Test);
		  TLatex *labelchi2 = new TLatex(0.4,0.91,buf);
		  labelchi2->SetNDC();
		  //if (Chi2Test<0.1) gPad->SetFillColor(2);
		  labelchi2->SetTextSize(0.035);
		  labelchi2->Draw();
		  cv_map["cv_"+cvname]->Update();
		  //std::cout << "print chi2 label"<<std::endl;
		  sprintf (buf, "KG= %2.2f", KGTest);
		  TLatex *labelkg = new TLatex(0.6,0.91,buf);
		  labelkg->SetNDC();
		  if (KGTest<0.8) gPad->SetFillColor(2);
		  labelkg->SetTextSize(0.035);
		  labelkg->Draw();
		  //std::cout << " draw kg label"<<std::endl;
								
		} else {

		  std::cout << " no reference plot " << histKeys[ihist] << std::endl;
		  TLatex *label = new TLatex(0.2,0.5,"no reference plot available");
		  label->SetNDC();
		  label->SetTextSize(0.07);
		  label->Draw();
		}
	      } else {
		//hist->SetXTitle(hist->GetTitle());
		hist->Draw();
		label->Draw();
		if (logaxis) {
		  std::cout << "  make log Y-axis scale" << std::endl;
		  gPad->SetLogy();
		  gPad->SetGrid();
		}
	      }
								

	      cv_map["cv_"+cvname]->cd();
	      std::cout << " print canvas" << std::endl;
	      cv_map["cv_"+cvname]->Print(webpath+"/"+TString(cvname)+"."+extension);
	      if ( extension=="eps" ) {
		gSystem->Exec("cp "+webpath+"/"+TString(cvname)+"."+extension+" temp.eps");
		gSystem->Exec("pstopnm -ppm -xborder 0 -yborder 0 -portrait temp.eps");
		gSystem->Exec("ppmtogif temp.eps001.ppm > "+webpath+"/"+TString(cvname)+".gif");
		gSystem->Exec("rm -rf temp.eps temp.eps001.ppm");
		gSystem->Exec("rm "+webpath+"/"+TString(cvname)+"."+extension);
	      }
	      std::cout << " done"<<std::endl;
	    }
	    else {
	      std::cerr << "Can not get histogram " << histKeys[ihist] << std::endl;
	    }
	  }
	  std::vector<TH1*> histos_C;
	  std::vector<TH1*> histos_DUSG;

	  for (unsigned ihist = 0; ihist < histKeys.size (); ++ihist) {
	    TH1* histo = 0;
	    gDirectory->GetObject (histKeys[ihist].c_str(), histo);
	    TString  HistoName = histo->GetName() ;
	    //cout << " Histogram Name: " << HistoName << endl;	
	    
	    if ( HistoName.Contains(name1)) {histos_C.push_back(histo);}
	    if ( HistoName.Contains(name2)) {histos_DUSG.push_back(histo);}
	    //cout << " Histos_C.size: " << histos_C.size() << endl;	
	    //cout << " Histos_DUSG.size: " << histos_DUSG.size() << endl;
	  }
	  for (int i=0; i < histos_C.size(); ++i) {
	    std::string cvname = histos_C[i]->GetName();
	    //cout << cvname << endl;
	    cv_map["merged_"+cvname] = new TCanvas("merged_"+TString(cvname), "merged_"+TString(histos_C[i]->GetName()),800,800);
	    if (logaxis) {
	      std::cout << "  make log Y-axis scale" << std::endl;
	      gPad->SetLogy();
	      gPad->SetGrid();
	    }
	    TH1F *mytmp = (TH1F*) histos_C[i];
	    mytmp->Draw();
	    mytmp->SetMaximum(1); 
	    mytmp->SetMinimum(10e-4); 
	    TString title= mytmp->GetTitle();
	    title = title.Remove(0,24);
	    title = title.Remove(title.Length()-8, title.Length());
	    //cout << "title:" << title <<endl;
	    mytmp->SetTitle(title);
	    TH1F *mytmp2 = (TH1F*) histos_DUSG[i];
	    mytmp2->SetMarkerColor(kRed);
	    mytmp2->Draw("same");
	    TLegend *leg1 = new TLegend(0.1,0.75,0.25,0.85,"","NDC");
	    leg1->SetFillColor(kWhite);
	    leg1->SetTextSize(0.04);
	    leg1->AddEntry(mytmp,"C","P");
	    leg1->AddEntry(mytmp2,"DUSG","P");
	    leg1->Draw();

	    std::cout << "Drew Merged Histos " << std::endl;
	    cv_map["merged_"+cvname]->Print(webpath+"/"+"merged_"+TString(cvname)+"."+extension);

	    if ( extension=="eps" ) {
	      gSystem->Exec("cp "+webpath+"/"+TString(cvname)+"."+extension+" temp.eps");
	      gSystem->Exec("pstopnm -ppm -xborder 0 -yborder 0 -portrait temp.eps");
	      gSystem->Exec("ppmtogif temp.eps001.ppm > "+webpath+"/"+TString(cvname)+".gif");
	      gSystem->Exec("rm -rf temp.eps temp.eps001.ppm");
	      gSystem->Exec("rm "+webpath+"/"+TString(cvname)+"."+extension);
	    }
	    std::cout << " done merging "<<std::endl;
	  }	
	} else {
				
	  std::vector<std::string> dirName2 = getAllKeys (dir1, "TDirectory");
	  std::cout << "dirName2.size=" << dirName2.size() << std::endl;
	  for (unsigned idir2 = 0; idir2 < dirName2.size(); ++idir2) {
	    std::cout << " directory: " << dirName2[idir2] << std::endl;

	    //if ( dirName2[idir2] == "SoftLepton" ) continue;
					
	    TDirectory* dir2 = 0;
	    dir1->GetObject (dirName2[idir2].c_str(), dir2);
	    if (dir2) {

	      std::vector<std::string> histKeys = getAllKeys (dir2, "TH1F");
	      for (unsigned ihist = 0; ihist < histKeys.size (); ++ihist) {
		TH1* hist = 0;
		dir2->GetObject (histKeys[ihist].c_str(), hist);
				
		if (hist) {
		  std::cout << "  histogram: " << hist->GetName() << " entries: " << hist->GetEntries() << std::endl;
								
		  std::string cvname = hist->GetName();
								
		  cv_map["cv_"+cvname] = new TCanvas("cv_"+TString(cvname),
						     "cv_"+TString(hist->GetName()),800,800);
						
		  if (compare) {
		    cv_map["cv_"+cvname]->Divide(1,2);
									
		    cv_map["cv_"+cvname]->cd(1);
		    //hist->SetXTitle(hist->GetTitle());
		    hist->Draw();
		    label->Draw();
		    if (logaxis) {
		      std::cout << "  make log Y-axis scale" << std::endl;
		      gPad->SetLogy();
		      gPad->SetGrid(); 
		    }

		    cv_map["cv_"+cvname]->cd(2);
		    std::vector<std::string> histPathName;
		    histPathName.push_back (dirName1[idir]);
		    histPathName.push_back (dirName2[idir2]);
		    histPathName.push_back (histKeys[ihist]);
		    TH1* refhist = (TH1*) getObject (arefFile, histPathName);
		    if (refhist) {
		      double Chi2Test = 0.;
		      double KGTest = 0.;
		      Chi2Test = refhist->Chi2Test(hist,"UFOF");
		      KGTest   = refhist->KolmogorovTest(hist,"UO");
		      std::cout << "  chi2= " << Chi2Test << " KG= " << KGTest << std::endl;
										
		      TH1* reshist = HistoCompare(hist,refhist);
										
		      //refhist->Draw();
		      reshist->Draw();
		      //std::cout << " reshist= " << reshist->GetEntries() << std::endl;
		      char buf[1024];
		      sprintf (buf, "#chi^{2}= %1.2f", Chi2Test);
		      TLatex *labelchi2 = new TLatex(0.4,0.91,buf);
		      labelchi2->SetNDC();
		      //if (Chi2Test<0.1) gPad->SetFillColor(2);
		      labelchi2->SetTextSize(0.035);
		      labelchi2->Draw();
		      cv_map["cv_"+cvname]->Update();
		      //std::cout << "print chi2 label"<<std::endl;
		      sprintf (buf, "KG= %2.2f", KGTest);
		      TLatex *labelkg = new TLatex(0.6,0.91,buf);
		      labelkg->SetNDC();
		      if (KGTest<0.8) gPad->SetFillColor(2);
		      labelkg->SetTextSize(0.035);
		      labelkg->Draw();
		      //std::cout << " draw kg label"<<std::endl;
								
		    }else {

		      std::cout << " no reference plot " << histKeys[ihist] << std::endl;
		      TLatex *label = new TLatex(0.2,0.5,"no reference plot available");
		      label->SetNDC();
		      label->SetTextSize(0.07);
		      label->Draw();
		    }
									
		  } else {
		    //hist->SetXTitle(hist->GetTitle());
		    hist->Draw();
		    label->Draw();
		    if (logaxis) {
		      std::cout << "  make log Y-axis scale" << std::endl;
		      gPad->SetLogy();
		      gPad->SetGrid();
		    }
		  }
									

		  cv_map["cv_"+cvname]->cd();
		  std::cout << " print canvas" << std::endl;
		  // temporal fix to print gif
		  //TImageDump *di = new TImageDump(webpath+"/"+TString(cvname)+"."+extension);
		  //cv_map["cv_"+cvname]->Paint();
		  //di->Close();
		  //delete di;
								
		  cv_map["cv_"+cvname]->Print(webpath+"/"+TString(cvname)+"."+extension);
		  if ( extension=="eps" ) {

		    gSystem->Exec("cp "+webpath+"/"+TString(cvname)+"."+extension+" temp.eps");
		    gSystem->Exec("pstopnm -ppm -xborder 0 -yborder 0 -portrait temp.eps");
		    gSystem->Exec("ppmtogif temp.eps001.ppm > "+webpath+"/"+TString(cvname)+".gif");
		    gSystem->Exec("rm -rf temp.eps temp.eps001.ppm");
		    gSystem->Exec("rm "+webpath+"/"+TString(cvname)+"."+extension);
		  }	
		  std::cout << " done"<<std::endl;
		}
		else {
		  std::cerr << "Can not get histogram " << histKeys[ihist] << std::endl;
		}
	      }
	      std::vector<TH1*> histos_C;
	      std::vector<TH1*> histos_DUSG;

	      for (unsigned ihist = 0; ihist < histKeys.size (); ++ihist) {
		TH1* histo = 0;
		dir2->GetObject (histKeys[ihist].c_str(), histo);
		TString  HistoName = histo->GetName() ;
		//cout << " Histogram Name: " << HistoName << endl;	
	    
		if ( HistoName.Contains(name1)) {histos_C.push_back(histo);}
		if ( HistoName.Contains(name2)) {histos_DUSG.push_back(histo);}
		//cout << " Histos_C.size: " << histos_C.size() << endl;	
		//cout << " Histos_DUSG.size: " << histos_DUSG.size() << endl;
	      }
	      for (int i=0; i < histos_C.size(); ++i) {
		std::string cvname = histos_C[i]->GetName();
		//cout << cvname << endl;	 
		cv_map["merged_"+cvname] = new TCanvas("merged_"+TString(cvname), "merged_"+TString(histos_C[i]->GetName()),800,800);
		if (logaxis) {
		  std::cout << "  make log Y-axis scale" << std::endl;
		  gPad->SetLogy();
		  gPad->SetGrid();
		}
		TH1F *mytmp = (TH1F*) histos_C[i];
		mytmp->SetMaximum(1); 
		mytmp->SetMinimum(10e-4); 
		TString title= mytmp->GetTitle();
		title = title.Remove(0,24);
		title = title.Remove(title.Length()-8, title.Length());
		//cout << "title:" << title <<endl;
		mytmp->Draw();
		mytmp->SetTitle(title);
		TH1F *mytmp2 = (TH1F*) histos_DUSG[i];
		mytmp2->SetMarkerColor(kRed);
		mytmp2->Draw("same");
		TLegend *leg1 = new TLegend(0.1,0.75,0.25,0.85,"","NDC");
		leg1->SetFillColor(kWhite);
		leg1->SetTextSize(0.04);
		leg1->AddEntry(mytmp,"C","P");
		leg1->AddEntry(mytmp2,"DUSG","P");
		leg1->Draw();

		std::cout << " Drew Histograms merged" << std::endl;
		cv_map["merged_"+cvname]->Print(webpath+"/"+"merged_"+TString(cvname)+"."+extension);

		if ( extension=="eps" ) {
		  gSystem->Exec("cp "+webpath+"/"+TString(cvname)+"."+extension+" temp.eps");
		  gSystem->Exec("pstopnm -ppm -xborder 0 -yborder 0 -portrait temp.eps");
		  gSystem->Exec("ppmtogif temp.eps001.ppm > "+webpath+"/"+TString(cvname)+".gif");
		  gSystem->Exec("rm -rf temp.eps temp.eps001.ppm");
		  gSystem->Exec("rm "+webpath+"/"+TString(cvname)+"."+extension);
		}
		std::cout << " done merging "<<std::endl;
	      }						

	    }
	  }
	}
      }
      else {
	std::cerr << "Can not find dir1: " << dirName1[idir] << std::endl;
      }
    }
  }


  // change directory
  //gSystem->cd(webpath);
  // make a directory
  //gSystem->mkdir(thename);
  //gSystem->cd(thename);
	
  /*
    for(std::map<std::string,TCanvas*>::const_iterator icv=cv_map.begin(); icv!=cv_map.end(); ++icv){

    std::string tmpname = icv->first;
    TCanvas *acv = icv->second;
    acv->Print(TString(tmpname+"."+extension));
    }
  */
	
}
