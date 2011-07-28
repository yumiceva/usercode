#ifndef HistoManager_h
#define HistoManager_h
/**_________________________________________________________________
   class:   HistoManager.cc

 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: HistoManager.h,v 1.1 2011/05/29 21:02:06 yumiceva Exp $

 ________________________________________________________________**/

#include "TH1.h"
#include "TFile.h"

#include<map>
#include<string>

using namespace std;

class HistoManager {

 public:
  HistoManager(TString name) {
    fname = name;
    CreateMuons();
  }

  void FillMuons( string name, Double_t val)
  {
    hmuons[name]->Fill(val);
  }
  void FillElectrons( string name, Double_t val)
  {
    helectrons[name]->Fill(val);
  }
  void FillJets( string name, Double_t val)
  {
    hjets[name]->Fill(val);
  }
  void FillM3( string name, Double_t val)
  {
    hM3[name]->Fill(val);
  }
  void FillMET( string name, Double_t val)
  {
    hMET[name]->Fill(val);
  }
  void FillMt( string name, Double_t val)
  {
    hMt[name]->Fill(val);
  }

  void SetupXTitle( map<string, TH1*> *mymap )
  {
    for ( map<string,TH1* >::const_iterator imap=mymap->begin(); imap!=mymap->end(); ++imap )
      {
	TH1 *temp = imap->second;
	temp->SetXTitle( temp->GetTitle() );
	temp->Sumw2();
      }
  }
  
  void Write(TFile *tfile) 
  {
    if (tfile->IsOpen() ) {
      tfile->cd();
      tfile->mkdir("muons");
      tfile->cd("muons");
      for ( map<string,TH1* >::const_iterator imap=hmuons.begin(); imap!=hmuons.end(); ++imap )
	{
	  TH1 *temp = imap->second;
	  if ( temp->GetEntries() > 0 )
	    temp->Write();
	  //else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
	}
      tfile->cd();
      tfile->mkdir("electrons");
      tfile->cd("electrons");
      for ( map<string,TH1* >::const_iterator imap=helectrons.begin(); imap!=helectrons.end(); ++imap )
        {
	  TH1 *temp = imap->second;
          if ( temp->GetEntries() > 0 )
            temp->Write();
          //else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;                     
        }
      tfile->cd();
      tfile->mkdir("jets");
      tfile->cd("jets");
      for ( map<string,TH1* >::const_iterator imap=hjets.begin(); imap!=hjets.end(); ++imap )
        {
	  TH1 *temp = imap->second;
          if ( temp->GetEntries() > 0 )
            temp->Write();
          //else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;                     
        }
      tfile->cd();
      tfile->mkdir("M3");
      tfile->cd("M3");
      for ( map<string,TH1* >::const_iterator imap=hM3.begin(); imap!=hM3.end(); ++imap )
        {
	  TH1 *temp = imap->second;
          if ( temp->GetEntries() > 0 )
            temp->Write();
          //else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;                     
        }
      tfile->cd();
      tfile->mkdir("MET");
      tfile->cd("MET");
      for ( map<string,TH1* >::const_iterator imap=hMET.begin(); imap!=hMET.end(); ++imap )
        {
	  TH1 *temp = imap->second;
          if ( temp->GetEntries() > 0 )
            temp->Write();
          //else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;                     
        }
      tfile->cd();
      tfile->mkdir("Mt");
      tfile->cd("Mt");
      for ( map<string,TH1* >::const_iterator imap=hMt.begin(); imap!=hMt.end(); ++imap )
        {
	  TH1 *temp = imap->second;
          if ( temp->GetEntries() > 0 )
            temp->Write();
          //else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;                     
        }

    }
  }

 private:
  void CreateMuons();
  TString fname;
  map<string, TH1*> hmuons;
  map<string, TH1*> helectrons;
  map<string, TH1*> hjets;
  map<string, TH1*> hM3;
  map<string, TH1*> hMET;
  map<string, TH1*> hMt;

};

#endif
