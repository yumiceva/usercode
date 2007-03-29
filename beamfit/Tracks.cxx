#define Tracks_cxx
#include "Tracks.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TNtuple.h"
using namespace std;

void Tracks::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L Tracks.C
//      Root > Tracks t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TFile* file = TFile::Open(outputfname, "RECREATE"); 
   TNtuple* tnt    = new TNtuple("tnt","simulated data","run:event:pt:d0:phi:sigmaD:z0:sigmaz0:x:y");

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
	  if (trk_nSvxRphi>3&&trk_pT>2.0&&trk_nCotAx>15&&trk_nCotSt>10)
		  //if (trk_nSvxRphi>5&&trk_pT>2&&trk_nCotAx>20&&trk_nCotSt>16)
	  {
		  tnt->Fill(evt_runnum,evt_evetnum,trk_pT,trk_d0,trk_phi0,trk_sigmaD0,trk_z0,trk_sigmaZ0,0.,0.);
	  }
      //      cout << evt_evetnum <<endl;
      // if (Cut(ientry) < 0) continue;
   }
  tnt->Write();
  file->Write();
  file->Close();
}
int main(int argc, char **argv)
{
  if ( argc < 3 || "-h" == string(argv[1]) || "--help" == string(argv[1]) )
    {
      std::cout << "Usage: " << argv[0] << "inputfile outputfile " << 
std::endl;
      return 1;
    }
  //  std::cout<<argv[1]<<" main  "<<argv[2]<<std::endl;  
  Tracks* track = new Tracks(argv[1],argv[2]);
  track->Loop();
  return 0;
}
