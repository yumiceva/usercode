
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


MakePlots::MakePlots() {

  extension = "png";
  compare = false;
  compare_filename = "";
  logaxis = false;
  release_version = "";
  compare_version = "";
	
}

void MakePlots::Draw() {

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
  TString name3 = "discr_";
  TString name4 = "GLOBALC";
  TString name5 = "GLOBALDUSG";
  TString name6 = "effVsDiscrCut";	
	
  std::vector<std::string> dirName1 = getAllKeys ( afile , "TDirectory");
  std::cout << "dirName1.size=" << dirName1.size() << std::endl;

  if (dirName1.size()==0) {
    std::vector<std::string> histKeys = getAllKeys (gDirectory, "TH1F");

    for (unsigned ihist = 0; ihist < histKeys.size (); ++ihist) {
      TH1* hist = 0;
      gDirectory->GetObject (histKeys[ihist].c_str(), hist);

      if (hist) {

	std::string cvname = hist->GetName();
	TString HistName   = hist->GetName();	
	cv_map["cv_"+cvname] = new TCanvas("cv_"+TString(cvname),"cv_"+TString(hist->GetName()),800,800);	
	  //std::cout << "  no comparison done" << std::endl;
	if (HistName.Contains(name1) || HistName.Contains(name2)|| HistName.Contains(name3) & !HistName.Contains(name6) ) {
	    //hist->SetXTitle(hist->GetTitle());
	    std::cout << "  HistName" << HistName << std::endl;
	  } else {
	  std::string a = hist->GetName();
	  Int_t na = a.find("_GLOBAL");
	  Int_t nd = a.length()-na;
	  TString title(a);
	  title = title.Remove(title.Length()-nd,title.Length());
	  hist->Draw();
	  hist->SetTitle(title);
	    //label->Draw();
	  TLatex *labelkg = new TLatex(0.3,0.91,release_version);  
	  labelkg->SetNDC();
	  labelkg->SetTextSize(0.035);
	  labelkg->Draw();

	    if (logaxis) {
	      std::cout << "  make log Y-axis scale" << std::endl;
	      gPad->SetLogy();
	      gPad->SetGrid();
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
	  }
	  std::cout << " done"<<std::endl;
      }
      else {
	std::cerr << "Can not get histogram " << histKeys[ihist] << std::endl;
      }
    }

    std::vector<TH1*> histos_C;
    std::vector<TH1*> histos_DUSG;
    std::vector<TH1*> discr_C;
    std::vector<TH1*> discr_DUSG;

    for (unsigned ihist = 0; ihist < histKeys.size (); ++ihist) {
      TH1* histo = 0;
      gDirectory->GetObject (histKeys[ihist].c_str(), histo);
      TString  HistoName = histo->GetName() ;
      // cout << " Histogram Name: " << HistoName << endl;	
	    
      if ( HistoName.Contains(name1)) {histos_C.push_back(histo);}
      if ( HistoName.Contains(name2)) {histos_DUSG.push_back(histo);}
      if ( HistoName.Contains(name3) & !HistoName.Contains(name6)){
	if ( HistoName.Contains(name4)) {discr_C.push_back(histo);}
	if ( HistoName.Contains(name5)) {discr_DUSG.push_back(histo);}
      }
      //std::cout << " discr_C.size: " << discr_C.size() << std::endl;	
      //std::cout << " discr_DUSG.size: " << discr_DUSG.size() << std::endl;
    }
    for (size_t i=0; i < histos_C.size(); ++i) {
      std::string cvname = histos_C[i]->GetName();
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
      std::string a= mytmp->GetName();
      Int_t na = a.find("discr_");
      Int_t nb = a.find("_GLOBAL");
      Int_t nd = a.length()-nb;
      TString title(a);
      title = title.Remove(0,na+6);
      title = title.Remove(title.Length()-nd, title.Length());
      mytmp->SetTitle(title);
      TH1F *mytmp2 = (TH1F*) histos_DUSG[i];
      mytmp2->SetMarkerColor(kRed);
      mytmp2->SetLineColor(kRed);
      mytmp2->Draw("same");   
      if (compare) {
	  TString dirname = "trackCounting";
	  TString name_c= mytmp->GetName();
	  TString name_dusg= mytmp2->GetName();
	  name_c.Remove(name_c.Length()-15,name_c.Length());
	  name_dusg.Remove(name_dusg.Length()-15,name_dusg.Length());
	  TString HName_C = name_c+"JetTags_GLOBAL";
	  TString HName_DUSG = name_dusg+"JetTags_GLOBAL";
	  std::string HName_c(HName_C);
	  std::string HName_dusg(HName_DUSG);
	  std::vector<std::string> histPath_C;
	  std::string dname = "TrackCounting";
	  //histPath_C.push_back (dirName1[idir]);
	  // if(name_c.Contains(dirname)){
	  //    std::cout << " dir name contains Counting" << std::endl;
	  //    histPath_C.push_back (dname);
	  //      }else{
	  //    histPath_C.push_back (dirName2[idir2]);
	  //    std::cout << "directoryname" << dirName2[idir2] << std::endl;}
	  histPath_C.push_back (HName_c);
	  TH1* refhist_c = (TH1*) getObject (arefFile, histPath_C);
	  if (refhist_c) {
	    double KGTest = 0.;
	    KGTest   = refhist_c->KolmogorovTest(mytmp,"UO");
	    std::cout <<  " KG For C= " << KGTest << std::endl;
	    refhist_c->Draw("same");
	    refhist_c->SetMarkerStyle(25);
	    refhist_c->SetMarkerColor(17);
	    refhist_c->SetLineColor(17);
	    //char buf[1024];
	    //sprintf (buf, "KG[c]= %2.2f", KGTest);
	    //TLatex *labelkg = new TLatex(0.3,0.91,buf);
	    //labelkg->SetNDC();
	    //if (KGTest<0.8) gPad->SetFillColor(2);
	    //labelkg->SetTextSize(0.035);
	    //labelkg->Draw();
	    //std::cout << " draw kg label"<<std::endl;
	  }else {
	    std::cout << " no reference plot " << std::endl;
	    TLatex *label = new TLatex(0.2,0.5,"no reference plot available");
	    label->SetNDC();
	    label->SetTextSize(0.07);
	    //label->Draw();
	  }

	  std::vector<std::string> histPath_DUSG;
	  //histPath_DUSG.push_back (dirName1[idir]);
	  //if(name_dusg.Contains(dirname)){
	  //std::string dname = "TrackCounting";
	  //histPath_DUSG.push_back (dname);
	  //   }else{
	  //     histPath_DUSG.push_back (dirName2[idir2]);}
	  histPath_DUSG.push_back (HName_dusg);
	  TH1* refhist_dusg = (TH1*) getObject (arefFile, histPath_DUSG);
	  if (refhist_dusg) {
	    double KGTest = 0.;
	    KGTest   = refhist_dusg->KolmogorovTest(mytmp,"UO");
	    //std::cout <<  " KG For DUSG= " << KGTest << std::endl;
	    refhist_dusg->Draw("same");
	    refhist_dusg->SetMarkerStyle(25);
	    refhist_dusg->SetMarkerColor(46);
	    refhist_dusg->SetLineColor(46);
	    //char buf[1024];
	    //sprintf (buf, "KG[dusg]= %2.2f", KGTest);
	    //TLatex *labelkg = new TLatex(0.4,0.91,release_version+" Vs "+compare_version);
	    //labelkg->SetNDC();
	    //if (KGTest<0.8) gPad->SetFillColor(2);
	    //labelkg->SetTextSize(0.035);
	    //labelkg->Draw();
	    //std::cout << " draw kg label"<<std::endl;
	  }else {
	    std::cout << " no reference plot " << std::endl;
	    TLatex *label = new TLatex(0.2,0.5,"no reference plot available");
	    label->SetNDC();
	    label->SetTextSize(0.07);
	    //label->Draw();
	  }
      } 
      TLegend *leg1 = new TLegend(0.1,0.75,0.25,0.85,"","NDC");
      leg1->SetTextSize(0.04);
      leg1->SetFillColor(kWhite);
      leg1->AddEntry(mytmp,"c","P");
      leg1->AddEntry(mytmp2,"dusg","P");
      leg1->Draw();
      TString labelv;
      if (compare) {
	labelv = release_version+" Vs "+compare_version;  
      }else{
	labelv = release_version;  
      }
      TLatex *labelkg = new TLatex(0.2,0.91,labelv);
      labelkg->SetNDC();
      labelkg->SetTextSize(0.035);
      labelkg->Draw();
      cv_map["merged_"+cvname]->Print(webpath+"/"+"Amerged_"+TString(cvname)+"."+extension);

      if ( extension=="eps" ) {
	gSystem->Exec("cp "+webpath+"/"+"Amerged_"+TString(cvname)+"."+extension+" temp.eps");
	gSystem->Exec("pstopnm -ppm -xborder 0 -yborder 0 -portrait temp.eps");
	gSystem->Exec("ppmtogif temp.eps001.ppm > "+webpath+"/"+"Amerged_"+TString(cvname)+".gif");
	gSystem->Exec("rm -rf temp.eps temp.eps001.ppm");
	gSystem->Exec("rm "+webpath+"/"+"Amerged_"+TString(cvname)+"."+extension);
      }
      std::cout << " done merging "<<std::endl;
    }

    for (size_t i=0; i < discr_C.size(); ++i) {
      std::string cvname = discr_C[i]->GetName();
      cv_map["merged_"+cvname] = new TCanvas("merged_"+TString(cvname), "merged_"+TString(discr_C[i]->GetName()),800,800);
      
      TH1F *mytmp = (TH1F*) discr_C[i];
      //mytmp->SetMaximum(1); 
      //mytmp->SetMinimum(10e-4); 
      mytmp->Draw();
      mytmp->SetMarkerColor(kBlack);
      std::string a= mytmp->GetName();
      Int_t na = a.find("discr_");
      Int_t nb = a.find("_GLOBAL");
      Int_t nd = a.length()-nb;
      TString title(a);
      title = title.Remove(0,na+6);
      title = title.Remove(title.Length()-nd, title.Length());
      //std::cout << "title:" << title <<std::endl;
      mytmp->SetTitle(title);
      TH1F *mytmp2 = (TH1F*) discr_DUSG[i];
      mytmp2->SetMarkerColor(kRed);
      mytmp2->SetLineColor(kRed);
      mytmp2->Draw("same");
      TLegend *leg1 = new TLegend(0.1,0.75,0.25,0.85,"","NDC");
      leg1->SetTextSize(0.04);
      leg1->SetFillColor(kWhite);
      leg1->AddEntry(mytmp,"c","P");
      leg1->AddEntry(mytmp2,"dusg","P");
      leg1->Draw();
      TLatex *labelkg = new TLatex(0.5,0.91,release_version);  
      labelkg->SetNDC();
      labelkg->SetTextSize(0.035);
      labelkg->Draw();
      cv_map["merged_"+cvname]->Print(webpath+"/"+"Dmerged_"+TString(cvname)+"."+extension);

      if ( extension=="eps" ) {
	gSystem->Exec("cp "+webpath+"/"+"Dmerged_"+TString(cvname)+"."+extension+" temp.eps");
	gSystem->Exec("pstopnm -ppm -xborder 0 -yborder 0 -portrait temp.eps");
	gSystem->Exec("ppmtogif temp.eps001.ppm > "+webpath+"/"+"Dmerged_"+TString(cvname)+".gif");
	gSystem->Exec("rm -rf temp.eps temp.eps001.ppm");
	gSystem->Exec("rm "+webpath+"/"+"Dmerged_"+TString(cvname)+"."+extension);
      }
      std::cout << " done merging of discriminant "<<std::endl;
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
	      TString HistName =  hist->GetName();
	      cv_map["cv_"+cvname] = new TCanvas("cv_"+TString(cvname),
						 "cv_"+TString(hist->GetName()),800,800);
						

	      if ( HistName.Contains(name1) ||  HistName.Contains(name2) || HistName.Contains(name3) & !HistName.Contains(name6) ) {
		  std::cout << "  HistName:" << HistName << std::endl;
		} else {
		std::string a = hist->GetName();
		Int_t na = a.find("_GLOBAL");
		Int_t nd = a.length()-na;
		TString title(a);
		title = title.Remove(title.Length()-nd,title.Length());
		hist->Draw();
		hist->SetTitle(title);
		TLatex *labelkg = new TLatex(0.3,0.91,release_version);  
		labelkg->SetNDC();
		labelkg->SetTextSize(0.035);
		labelkg->Draw();
		  //label->Draw();

		  if (logaxis) {
		    std::cout << "  make log Y-axis scale" << std::endl;
		    gPad->SetLogy();
		    gPad->SetGrid();
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
		}
	      								
	      std::cout << " done" << ihist <<std::endl;
	    }
	    else {
	      std::cerr << "Can not get histogram " << histKeys[ihist] << std::endl;
	    }
	  }
	  std::vector<TH1*> histos_C;
	  std::vector<TH1*> histos_DUSG;
	  std::vector<TH1*> discr_C;
	  std::vector<TH1*> discr_DUSG;
	  
	  for (unsigned ihist = 0; ihist < histKeys.size (); ++ihist) {
	    TH1* histo = 0;
	    dir1->GetObject (histKeys[ihist].c_str(), histo);
	    TString  HistoName = histo->GetName() ;
	    if ( HistoName.Contains(name1)) {histos_C.push_back(histo);}
	    if ( HistoName.Contains(name2)) {histos_DUSG.push_back(histo);}
	    if ( HistoName.Contains(name3) & !HistoName.Contains(name6) ){
	      if ( HistoName.Contains(name4)) {discr_C.push_back(histo);}
	      if ( HistoName.Contains(name5)) {discr_DUSG.push_back(histo);}
	    }
	    // std::cout << " discr_C.size: " << discr_C.size() << std::endl;	
	    //std::cout << " discr_DUSG.size: " << discr_DUSG.size() << std::endl;
	  }
	  for (size_t i=0; i < histos_C.size(); ++i) {
	    std::string cvname = histos_C[i]->GetName();
	    //std::cout << cvname << std::endl;
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
	    std::string a= mytmp->GetName();
	    Int_t na = a.find("discr_");
	    Int_t nb = a.find("_GLOBAL");
	    Int_t nd = a.length()-nb;
	    TString title(a);
	    title = title.Remove(0,na+6);
	    title = title.Remove(title.Length()-nd, title.Length());
	    //cout << "title:" << title <<endl;
	    mytmp->SetTitle(title);
	    TH1F *mytmp2 = (TH1F*) histos_DUSG[i];
	    mytmp2->SetMarkerColor(kRed);
	    mytmp2->SetLineColor(kRed);
	    mytmp2->Draw("same");
		if (compare) {                  
		  TString dirname = "trackCounting";
		  TString name_c= mytmp->GetName();
		  TString name_dusg= mytmp2->GetName();
		  name_c.Remove(name_c.Length()-15,name_c.Length());
		  name_dusg.Remove(name_dusg.Length()-15,name_dusg.Length());
		  TString HName_C = name_c+"JetTags_GLOBAL";
		  TString HName_DUSG = name_dusg+"JetTags_GLOBAL";
                  std::string HName_c(HName_C);
		  std::string HName_dusg(HName_DUSG);
		  std::vector<std::string> histPath_C;
		  std::string dname = "TrackCounting";
		  histPath_C.push_back (dirName1[idir]);
		  //if(name_c.Contains(dirname)){
		  //  std::cout << " dir name contains Counting" << std::endl;
		  //  histPath_C.push_back (dname);
		  //    }else{
		  //  histPath_C.push_back (dirName2[idir2]);
		  //   std::cout << "directoryname" << dirName2[idir2] << std::endl;}
		  histPath_C.push_back (HName_c);
		  TH1* refhist_c = (TH1*) getObject (arefFile, histPath_C);
		  if (refhist_c) {
		    double KGTest = 0.;
		    KGTest   = refhist_c->KolmogorovTest(mytmp,"UO");
		    std::cout <<  " KG For C= " << KGTest << std::endl;
		    refhist_c->Draw("same");
		    refhist_c->SetMarkerStyle(25);
		    refhist_c->SetMarkerColor(17);
		    refhist_c->SetLineColor(17);
		    //char buf[1024];
		    //sprintf (buf, "KG[c]= %2.2f", KGTest);
		    //TLatex *labelkg = new TLatex(0.3,0.91,buf);
		    //labelkg->SetNDC();
		    //if (KGTest<0.8) gPad->SetFillColor(2);
		    //labelkg->SetTextSize(0.035);
		    //labelkg->Draw();
		    //std::cout << " draw kg label"<<std::endl;
		  }else {
		    std::cout << " no reference plot " << std::endl;
		    TLatex *label = new TLatex(0.2,0.5,"no reference plot available");
		    label->SetNDC();
		    label->SetTextSize(0.07);
		    //label->Draw();
		  }

		  std::vector<std::string> histPath_DUSG;
		  histPath_DUSG.push_back (dirName1[idir]);
		  // if(name_dusg.Contains(dirname)){
		  //   std::string dname = "TrackCounting";
		  //   histPath_DUSG.push_back (dname);
		  // }else{
		  //    histPath_DUSG.push_back (dirName2[idir2]);}
		  histPath_DUSG.push_back (HName_dusg);
		  TH1* refhist_dusg = (TH1*) getObject (arefFile, histPath_DUSG);
		  if (refhist_dusg) {
		    double KGTest = 0.;
		    KGTest   = refhist_dusg->KolmogorovTest(mytmp,"UO");
		    std::cout <<  " KG For DUSG= " << KGTest << std::endl;
		    refhist_dusg->Draw("same");
		    refhist_dusg->SetMarkerStyle(25);
		    refhist_dusg->SetMarkerColor(46);
		    refhist_dusg->SetLineColor(46);
		    //char buf[1024];
		    //sprintf (buf, "KG[dusg]= %2.2f", KGTest);
		    //TLatex *labelkg = new TLatex(0.4,0.91,release_version+" Vs "+compare_version);
		    //labelkg->SetNDC();
		    //if (KGTest<0.8) gPad->SetFillColor(2);
		    //labelkg->SetTextSize(0.035);
		    //labelkg->Draw();
		    //std::cout << " draw kg label"<<std::endl;
		  }else {
		    std::cout << " no reference plot " << std::endl;
		    TLatex *label = new TLatex(0.2,0.5,"no reference plot available");
		    label->SetNDC();
		    label->SetTextSize(0.07);
		    //label->Draw();
		  }
		} 
	    TLegend *leg1 = new TLegend(0.1,0.75,0.25,0.85,"","NDC");
	    leg1->SetFillColor(kWhite);
	    leg1->SetTextSize(0.04);
	    leg1->AddEntry(mytmp,"c","P");
	    leg1->AddEntry(mytmp2,"dusg","P");
	    leg1->Draw();
	    TString labelv;
	    if (compare) {
	      labelv = release_version+" Vs "+compare_version;  
	    }else{
	      labelv = release_version;  
	    }
	    TLatex *labelkg = new TLatex(0.2,0.91,labelv);
	    labelkg->SetNDC();
	    labelkg->SetTextSize(0.035);
	    labelkg->Draw();
	    //std::cout << "Drew Merged Histos " << std::endl;
	    cv_map["merged_"+cvname]->Print(webpath+"/"+"Amerged_"+TString(cvname)+"."+extension);

	    if ( extension=="eps" ) {
	      gSystem->Exec("cp "+webpath+"/"+"Amerged_"+TString(cvname)+"."+extension+" temp.eps");
	      gSystem->Exec("pstopnm -ppm -xborder 0 -yborder 0 -portrait temp.eps");
	      gSystem->Exec("ppmtogif temp.eps001.ppm > "+webpath+"/"+"Amerged_"+TString(cvname)+".gif");
	      gSystem->Exec("rm -rf temp.eps temp.eps001.ppm");
	      gSystem->Exec("rm "+webpath+"/"+"Amerged_"+TString(cvname)+"."+extension);
	    }
	    //std::cout << " done merging "<<std::endl;
	  }	
 
	  for (size_t i=0; i < discr_C.size(); ++i) {
	    std::string cvname = discr_C[i]->GetName();
	    //cout << cvname << endl;
	    cv_map["merged_"+cvname] = new TCanvas("merged_"+TString(cvname), "merged_"+TString(discr_C[i]->GetName()),800,800);
	    
	    TH1F *mytmp = (TH1F*) discr_C[i];
	    //mytmp->SetMaximum(1); 
	    //mytmp->SetMinimum(10e-4); 
	    mytmp->Draw();
	    mytmp->SetMarkerColor(kBlack); 
	    std::string a= mytmp->GetName();
	    Int_t na = a.find("discr_");
	    Int_t nb = a.find("_GLOBAL");
	    Int_t nd = a.length()-nb;
	    TString title(a);
	    title = title.Remove(0,na+6);
	    title = title.Remove(title.Length()-nd, title.Length());
	    mytmp->SetTitle(title);
	    TH1F *mytmp2 = (TH1F*) discr_DUSG[i];
	    mytmp2->SetMarkerColor(kRed);
	    mytmp2->SetLineColor(kRed);
	    mytmp2->Draw("same");
	    TLegend *leg1 = new TLegend(0.1,0.75,0.25,0.85,"","NDC");
	    leg1->SetTextSize(0.04);
	    leg1->SetFillColor(kWhite);
	    leg1->AddEntry(mytmp,"c","P");
	    leg1->AddEntry(mytmp2,"dusg","P");
	    leg1->Draw();
	    TLatex *labelkg = new TLatex(0.5,0.91,release_version);
	    labelkg->SetNDC();
	    labelkg->SetTextSize(0.035);
	    labelkg->Draw();

	    cv_map["merged_"+cvname]->Print(webpath+"/"+"Dmerged_"+TString(cvname)+"."+extension);
	    
	    if ( extension=="eps" ) {
	      gSystem->Exec("cp "+webpath+"/"+"Dmerged_"+TString(cvname)+"."+extension+" temp.eps");
	      gSystem->Exec("pstopnm -ppm -xborder 0 -yborder 0 -portrait temp.eps");
	      gSystem->Exec("ppmtogif temp.eps001.ppm > "+webpath+"/"+"Dmerged_"+TString(cvname)+".gif");
	      gSystem->Exec("rm -rf temp.eps temp.eps001.ppm");
	      gSystem->Exec("rm "+webpath+"/"+"Dmerged_"+TString(cvname)+"."+extension);
	    }
	    //std::cout << " done merging of discriminator "<<std::endl;
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
		  TString HistName = hist->GetName();
								
		  cv_map["cv_"+cvname] = new TCanvas("cv_"+TString(cvname),
						     "cv_"+TString(hist->GetName()),800,800);

		    //std::cout << "  no comparison done" << std::endl;
		  if (HistName.Contains(name1) || HistName.Contains(name2) ||HistName.Contains(name3) & !HistName.Contains(name6)) {
		      std::cout << "  HistName:" << HistName << std::endl;
		    } else {
		    std::string a = hist->GetName();
		    Int_t na = a.find("_GLOBAL");
		    Int_t nd = a.length()-na;
		    TString title(a);
		    title = title.Remove(title.Length()-nd,title.Length());
		    hist->Draw();
		    hist->SetTitle(title);
		      //label->Draw();
		    TLatex *labelkg = new TLatex(0.3,0.91,release_version);  
		    labelkg->SetNDC();
		    labelkg->SetTextSize(0.035);
		    labelkg->Draw();

		    if (logaxis) {
		      std::cout << "  make log Y-axis scale" << std::endl;
		      gPad->SetLogy();
		      gPad->SetGrid();
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
		    }
		  						
		  std::cout << " done"<<std::endl;
		}
		else {
		  std::cerr << "Can not get histogram " << histKeys[ihist] << std::endl;
		}
	      }
	      std::vector<TH1*> histos_C;
	      std::vector<TH1*> histos_DUSG;
	      std::vector<TH1*> discr_C;
	      std::vector<TH1*> discr_DUSG;

	      for (unsigned ihist = 0; ihist < histKeys.size (); ++ihist) {
		TH1* histo = 0;
		dir2->GetObject (histKeys[ihist].c_str(), histo);
		TString  HistoName = histo->GetName() ;
		//cout << " Histogram Name: " << HistoName << endl;	
	    
		if ( HistoName.Contains(name1)) {histos_C.push_back(histo);}
		if ( HistoName.Contains(name2)) {histos_DUSG.push_back(histo);}
		if ( HistoName.Contains(name4)) {discr_C.push_back(histo);}
		if ( HistoName.Contains(name5)) {discr_DUSG.push_back(histo);}
	      }
	      //std::cout << " discr_C.size: " << discr_C.size() << std::endl;	
	      //std::cout << " discr_DUSG.size: " << discr_DUSG.size() << std::endl;
 	    
	      for (size_t i=0; i < histos_C.size(); ++i) {
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
		std::string a= mytmp->GetName();
		Int_t na = a.find("discr_");
		Int_t nb = a.find("_GLOBAL");
		Int_t nd = a.length()-nb;
		TString title(a);
		title = title.Remove(0,na+6);
		title = title.Remove(title.Length()-nd, title.Length());
		mytmp->Draw();
		mytmp->SetTitle(title);
		TH1F *mytmp2 = (TH1F*) histos_DUSG[i];
		mytmp2->SetMarkerColor(kRed);
		mytmp2->SetLineColor(kRed);
		mytmp2->Draw("same");
						
		if (compare) {
                  
		  TString dirname = "trackCounting";
		  TString name_c= mytmp->GetName();
		  TString name_dusg= mytmp2->GetName();
		  name_c.Remove(name_c.Length()-15,name_c.Length());
		  name_dusg.Remove(name_dusg.Length()-15,name_dusg.Length());
		  TString HName_C = name_c+"JetTags_GLOBAL";
		  TString HName_DUSG = name_dusg+"JetTags_GLOBAL";
                  std::string HName_c(HName_C);
		  std::string HName_dusg(HName_DUSG);
		  std::vector<std::string> histPath_C;
		  std::string dname = "TrackCounting";
		  histPath_C.push_back (dirName1[idir]);
		  if(name_c.Contains(dirname)){
		    std::cout << " dir name contains Counting" << std::endl;
		    histPath_C.push_back (dname);
		      }else{
		    histPath_C.push_back (dirName2[idir2]);
		    std::cout << "directoryname" << dirName2[idir2] << std::endl;}
		  histPath_C.push_back (HName_c);
		  TH1* refhist_c = (TH1*) getObject (arefFile, histPath_C);
		  if (refhist_c) {
		    double KGTest = 0.;
		    KGTest   = refhist_c->KolmogorovTest(mytmp,"UO");
		    std::cout <<  " KG For C= " << KGTest << std::endl;
		    refhist_c->Draw("same");
		    refhist_c->SetMarkerStyle(25);
		    refhist_c->SetMarkerColor(17);
		    refhist_c->SetLineColor(17);
		    //char buf[1024];
		    //sprintf (buf, "KG[c]= %2.2f", KGTest);
		    //TLatex *labelkg = new TLatex(0.3,0.91,buf);
		    //labelkg->SetNDC();
		    //if (KGTest<0.8) gPad->SetFillColor(2);
		    //labelkg->SetTextSize(0.035);
		    //labelkg->Draw();
		    //std::cout << " draw kg label"<<std::endl;
		  }else {
		    std::cout << " no reference plot " << std::endl;
		    TLatex *label = new TLatex(0.2,0.5,"no reference plot available");
		    label->SetNDC();
		    label->SetTextSize(0.07);
		    //label->Draw();
		  }

		  std::vector<std::string> histPath_DUSG;
		  histPath_DUSG.push_back (dirName1[idir]);
		   if(name_dusg.Contains(dirname)){
		     std::string dname = "TrackCounting";
		     histPath_DUSG.push_back (dname);
		   }else{
		     histPath_DUSG.push_back (dirName2[idir2]);}
		  histPath_DUSG.push_back (HName_dusg);
		  TH1* refhist_dusg = (TH1*) getObject (arefFile, histPath_DUSG);
		  if (refhist_dusg) {
		    double KGTest = 0.;
		    KGTest   = refhist_dusg->KolmogorovTest(mytmp,"UO");
		    std::cout <<  " KG For DUSG= " << KGTest << std::endl;
		    refhist_dusg->Draw("same");
		    refhist_dusg->SetMarkerStyle(25);
		    refhist_dusg->SetMarkerColor(46);
		    refhist_dusg->SetLineColor(46);

		    //char buf[1024];
		    //sprintf (buf, "KG[dusg]= %2.2f", KGTest);
		    //TLatex *labelkg = new TLatex(0.4,0.91,release_version+" Vs "+compare_version);
		    //labelkg->SetNDC();
		    //if (KGTest<0.8) gPad->SetFillColor(2);
		    //labelkg->SetTextSize(0.035);
		    //labelkg->Draw();
		    //std::cout << " draw kg label"<<std::endl;
		  }else {
		    std::cout << " no reference plot " << std::endl;
		    TLatex *label = new TLatex(0.2,0.5,"no reference plot available");
		    label->SetNDC();
		    label->SetTextSize(0.07);
		    //label->Draw();
		  }
		} 
		TLegend *leg1 = new TLegend(0.1,0.75,0.25,0.85,"","NDC");
		leg1->SetFillColor(kWhite);
		leg1->SetTextSize(0.04);
		leg1->AddEntry(mytmp,"c","P");
		leg1->AddEntry(mytmp2,"dusg","P");
		leg1->Draw();
		TString labelv;
		if (compare) {
		labelv = release_version+" Vs "+compare_version;  
		}else{
		labelv = release_version;  
		}
		TLatex *labelkg = new TLatex(0.2,0.91,labelv);
		labelkg->SetNDC();
		labelkg->SetTextSize(0.035);
		labelkg->Draw();
		//std::cout << " Drew Histograms merged" << std::endl;
		cv_map["merged_"+cvname]->Print(webpath+"/"+"Amerged_"+TString(cvname)+"."+extension);

		if ( extension=="eps" ) {
		  gSystem->Exec("cp "+webpath+"/"+"Amerged_"+TString(cvname)+"."+extension+" temp.eps");
		  gSystem->Exec("pstopnm -ppm -xborder 0 -yborder 0 -portrait temp.eps");
		  gSystem->Exec("ppmtogif temp.eps001.ppm > "+webpath+"/"+"Amerged_"+TString(cvname)+".gif");
		  gSystem->Exec("rm -rf temp.eps temp.eps001.ppm");
		  gSystem->Exec("rm "+webpath+"/"+"Amerged_"+TString(cvname)+"."+extension);
		}
		//std::cout << " done merging "<<std::endl;
	      }						
	      for (size_t i=0; i < discr_C.size(); ++i) {
		std::string cvname = discr_C[i]->GetName();
		//cout << cvname << endl;
		cv_map["merged_"+cvname] = new TCanvas("merged_"+TString(cvname), "merged_"+TString(discr_C[i]->GetName()),800,800);
		
		TH1F *mytmp = (TH1F*) discr_C[i];
		//mytmp->SetMaximum(1); 
		//mytmp->SetMinimum(10e-4); 
		mytmp->Draw();
		mytmp->SetMarkerColor(kBlack);
		std::string a= mytmp->GetName();
		Int_t na = a.find("discr_");
		Int_t nb = a.find("_GLOBAL");
		Int_t nd = a.length()-nb;
		TString title(a);
		title = title.Remove(0,na+6);
		title = title.Remove(title.Length()-nd, title.Length());
		mytmp->SetTitle(title);
		TH1F *mytmp2 = (TH1F*) discr_DUSG[i];
		mytmp2->SetMarkerColor(kRed);
		mytmp2->SetLineColor(kRed);
		mytmp2->Draw("same");
		TLegend *leg1 = new TLegend(0.1,0.75,0.25,0.85,"","NDC");
		leg1->SetTextSize(0.04);
		leg1->SetFillColor(kWhite);
		leg1->AddEntry(mytmp,"c","P");
		leg1->AddEntry(mytmp2,"dusg","P");
		leg1->Draw();
		TLatex *labelkg = new TLatex(0.3,0.91,release_version);  
		labelkg->SetNDC();
		labelkg->SetTextSize(0.035);
		labelkg->Draw();
  		//std::cout << "Drew Merged Discriminant " << std::endl;
		cv_map["merged_"+cvname]->Print(webpath+"/"+"Dmerged_"+TString(cvname)+"."+extension);

		if ( extension=="eps" ) {
		  gSystem->Exec("cp "+webpath+"/"+"Dmerged_"+TString(cvname)+"."+extension+" temp.eps");
		  gSystem->Exec("pstopnm -ppm -xborder 0 -yborder 0 -portrait temp.eps");
		  gSystem->Exec("ppmtogif temp.eps001.ppm > "+webpath+"/"+"Dmerged_"+TString(cvname)+".gif");
		  gSystem->Exec("rm -rf temp.eps temp.eps001.ppm");
		  gSystem->Exec("rm "+webpath+"/"+"Dmerged_"+TString(cvname)+"."+extension);
		}
		//std::cout << " done merging "<<std::endl;
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
