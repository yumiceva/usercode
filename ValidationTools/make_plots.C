
#include "TString.h"
#include "TSystem.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TObject.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TKey.h"

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

  cout << "Comparing " << h->GetName() << " and " << refHisto->GetName() << endl;
  cout << " entries " << h->GetEntries() << " and " << refHisto->GetEntries() << endl;

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
  
  return resHisto;
  
}

void make_plots(TString root_filename, TString webpath, TString extension="png", bool compare = false, TString compare_filename="", bool logaxis = false) {

	// get a file
	TFile *afile = new TFile(root_filename);

	TFile *arefFile;
	if ( compare ) {
	  arefFile = new TFile(compare_filename);
	  if ( arefFile->IsZombie()) {
		  cout << "[make_plots] Error opening file " << compare_filename << " will not compare plots." << endl;
		  compare = false;
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
	
	std::vector<std::string> dirName1 = getAllKeys ( afile , "TDirectory");
	cout << "dirName1.size=" << dirName1.size() << endl;

	if (dirName1.size()==0) {
	  std::vector<std::string> histKeys = getAllKeys (gDirectory, "TH1F");

	  for (unsigned ihist = 0; ihist < histKeys.size (); ++ihist) {
	    TH1* hist = 0;
	    gDirectory->GetObject (histKeys[ihist].c_str(), hist);

	    if (hist) {

			std::string cvname = hist->GetName();
			
			cv_map["cv_"+cvname] = new TCanvas("cv_"+TString(cvname),
														   "cv_"+TString(hist->GetName()),800,800);
			
			if (compare) {
				cv_map["cv_"+cvname]->Divide(1,2);
				
				cv_map["cv_"+cvname]->cd(1);
				hist->SetXTitle(hist->GetTitle());
				hist->Draw();
				label->Draw();
				if (logaxis) {
				  cout << "  make log Y-axis scale" << endl;
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
					cout << "  chi2= " << Chi2Test << " KG= " << KGTest << endl;
					
					TH1* reshist = HistoCompare(hist,refhist);
					
								
					//refhist->Draw();
					reshist->Draw();
					
					//cout << " reshist= " << reshist->GetEntries() << endl;
					char buf[1024];
					sprintf (buf, "#chi^{2}= %1.2f", Chi2Test);
					TLatex *labelchi2 = new TLatex(0.4,0.91,buf);
					labelchi2->SetNDC();
					//if (Chi2Test<0.1) gPad->SetFillColor(2);
					labelchi2->SetTextSize(0.035);
					labelchi2->Draw();
					cv_map["cv_"+cvname]->Update();
					//cout << "print chi2 label"<<endl;
					sprintf (buf, "KG= %2.2f", KGTest);
					TLatex *labelkg = new TLatex(0.6,0.91,buf);
					labelkg->SetNDC();
					if (KGTest<0.8) gPad->SetFillColor(2);
					labelkg->SetTextSize(0.035);
					labelkg->Draw();
					//cout << " draw kg label"<<endl;
					
				} else {
					
					cout << " no reference plot " << histKeys[ihist] << endl;
					TLatex *label = new TLatex(0.2,0.5,"no reference plot available");
					label->SetNDC();
					label->SetTextSize(0.07);
					label->Draw();
				}
			} else {
				hist->SetXTitle(hist->GetTitle());
				hist->Draw();
				label->Draw();
			}


			cv_map["cv_"+cvname]->cd();
			cout << " print canvas" << endl;
			cv_map["cv_"+cvname]->Print(webpath+"/"+TString(cvname)+"."+extension);
			cout << " done"<<endl;
	    }
	    else {
	      std::cerr << "Can not get histogram " << histKeys[ihist] << std::endl;
	    }
	  }

	} else {
    for (unsigned idir = 0; idir < dirName1.size(); ++idir) {
      cout << " directory: " << dirName1[idir] << endl;
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
							hist->SetXTitle(hist->GetTitle());
							hist->Draw();
							label->Draw();
							if (logaxis) {
							  cout << "  make log Y-axis scale" << endl;
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
								cout << "  chi2= " << Chi2Test << " KG= " << KGTest << endl;
								
								TH1* reshist = HistoCompare(hist,refhist);
														
								
								//refhist->Draw();
								reshist->Draw();
								
								//cout << " reshist= " << reshist->GetEntries() << endl;
								char buf[1024];
								sprintf (buf, "#chi^{2}= %1.2f", Chi2Test);
								TLatex *labelchi2 = new TLatex(0.4,0.91,buf);
								labelchi2->SetNDC();
								//if (Chi2Test<0.1) gPad->SetFillColor(2);
								labelchi2->SetTextSize(0.035);
								labelchi2->Draw();
								cv_map["cv_"+cvname]->Update();
								//cout << "print chi2 label"<<endl;
								sprintf (buf, "KG= %2.2f", KGTest);
								TLatex *labelkg = new TLatex(0.6,0.91,buf);
								labelkg->SetNDC();
								if (KGTest<0.8) gPad->SetFillColor(2);
								labelkg->SetTextSize(0.035);
								labelkg->Draw();
								//cout << " draw kg label"<<endl;
								
							} else {

								cout << " no reference plot " << histKeys[ihist] << endl;
								TLatex *label = new TLatex(0.2,0.5,"no reference plot available");
								label->SetNDC();
								label->SetTextSize(0.07);
								label->Draw();
							}
						} else {
							hist->SetXTitle(hist->GetTitle());
							hist->Draw();
							label->Draw();
						}
								

						cv_map["cv_"+cvname]->cd();
						cout << " print canvas" << endl;
						cv_map["cv_"+cvname]->Print(webpath+"/"+TString(cvname)+"."+extension);
						cout << " done"<<endl;
					}
					else {
						std::cerr << "Can not get histogram " << histKeys[ihist] << std::endl;
					}
				}
			} else {
				
				std::vector<std::string> dirName2 = getAllKeys (dir1, "TDirectory");
				cout << "dirName2.size=" << dirName2.size() << endl;
				for (unsigned idir2 = 0; idir2 < dirName2.size(); ++idir2) {
					cout << " directory: " << dirName2[idir2] << endl;

					TDirectory* dir2 = 0;
					dir1->GetObject (dirName2[idir2].c_str(), dir2);
					if (dir2) {

						std::vector<std::string> histKeys = getAllKeys (dir2, "TH1F");
						
						for (unsigned ihist = 0; ihist < histKeys.size (); ++ihist) {
							TH1* hist = 0;
							dir2->GetObject (histKeys[ihist].c_str(), hist);
				
							if (hist) {
								std::string cvname = hist->GetName();
								
								cv_map["cv_"+cvname] = new TCanvas("cv_"+TString(cvname),
														   "cv_"+TString(hist->GetName()),800,800);
						
								if (compare) {
									cv_map["cv_"+cvname]->Divide(1,2);
									
									cv_map["cv_"+cvname]->cd(1);
									hist->SetXTitle(hist->GetTitle());
									hist->Draw();
									label->Draw();
									if (logaxis) {
									  cout << "  make log Y-axis scale" << endl;
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
										cout << "  chi2= " << Chi2Test << " KG= " << KGTest << endl;
										
										TH1* reshist = HistoCompare(hist,refhist);
										
										//refhist->Draw();
										reshist->Draw();
										//cout << " reshist= " << reshist->GetEntries() << endl;
										char buf[1024];
										sprintf (buf, "#chi^{2}= %1.2f", Chi2Test);
										TLatex *labelchi2 = new TLatex(0.4,0.91,buf);
										labelchi2->SetNDC();
										//if (Chi2Test<0.1) gPad->SetFillColor(2);
										labelchi2->SetTextSize(0.035);
										labelchi2->Draw();
										cv_map["cv_"+cvname]->Update();
										//cout << "print chi2 label"<<endl;
										sprintf (buf, "KG= %2.2f", KGTest);
										TLatex *labelkg = new TLatex(0.6,0.91,buf);
										labelkg->SetNDC();
										if (KGTest<0.8) gPad->SetFillColor(2);
										labelkg->SetTextSize(0.035);
										labelkg->Draw();
										//cout << " draw kg label"<<endl;
								
									}else {

										cout << " no reference plot " << histKeys[ihist] << endl;
										TLatex *label = new TLatex(0.2,0.5,"no reference plot available");
										label->SetNDC();
										label->SetTextSize(0.07);
										label->Draw();
									}
									
								} else {
									hist->SetXTitle(hist->GetTitle());
									hist->Draw();
									label->Draw();
								}
									

								cv_map["cv_"+cvname]->cd();
								cout << " print canvas" << endl;
								cv_map["cv_"+cvname]->Print(webpath+"/"+TString(cvname)+"."+extension);
								cout << " done"<<endl;
							}
							else {
								std::cerr << "Can not get histogram " << histKeys[ihist] << std::endl;
							}
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
