#ifndef PlotTree_h
#define PlotTree_h

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"


class PlotTree {

  public:

	Int_t fCurrent[4];
		//TTree *fChain;
	TTree *vtxtree[4];
	TTree *ptree[4];

	PlotTree();
	virtual ~PlotTree();
	
	virtual void Loop();
	virtual void Initvtx(TTree *tree, int i);
	virtual void Initp(TTree *tree, int i);
	virtual Long64_t LoadTree(TTree *fChain, int i, Long64_t entry);
	virtual Bool_t   Notify();
	double fvx,fvy,fvz;
	double fpx,fpy,fpz,fpt,fp,fe,feta,fphi;

};
#endif

#ifdef PlotTree_cxx

PlotTree::PlotTree() {

	TFile *files[4];

	files[0]   = new TFile("VtxTest_qcd3000_gauss.root");
	vtxtree[0] = (TTree*) gDirectory->Get("vtxtree");
	ptree[0] = (TTree*) gDirectory->Get("ptree");
	
	files[1]   = new TFile("VtxTest_qcd3000_beta_nox.root");
	vtxtree[1] = (TTree*) gDirectory->Get("vtxtree");
	ptree[1] = (TTree*) gDirectory->Get("ptree");
	
	files[2]   = new TFile("VtxTest_qcd3000_beta_x.root");
	vtxtree[2] = (TTree*) gDirectory->Get("vtxtree");
	ptree[2] = (TTree*) gDirectory->Get("ptree");
	
	files[3]   = new TFile("VtxTest_qcd3000_beta_x_cms.root");
	vtxtree[3] = (TTree*) gDirectory->Get("vtxtree");
	ptree[3] = (TTree*) gDirectory->Get("ptree");

	for (int i=0; i<3; i++) {
		Initvtx(vtxtree[i],i);
		Initp(ptree[i],i);
	}
}

PlotTree::~PlotTree() {}

std::string itoa(int i) {
  char temp[20];
  sprintf(temp,"%d",i);
  return((std::string)temp);
}

std::string ftoa(float i, const char *format="0") {
  char temp[20];
  if ( format == "0" ) sprintf(temp,"%.2f",i);
  else sprintf(temp,format,i);

  return((std::string)temp);
}

void PlotTree::Initvtx(TTree *tree, int i) {

	if (!tree) return;

	fCurrent[i] = -1;

	tree->SetBranchAddress("vx",&fvx);
    tree->SetBranchAddress("vy",&fvy);
    tree->SetBranchAddress("vz",&fvz);
    
}

void PlotTree::Initp(TTree *tree, int i) {

	if (!tree) return;

	fCurrent[i] = -1;

	tree->SetBranchAddress("px",&fpx);
    tree->SetBranchAddress("py",&fpy);
    tree->SetBranchAddress("pz",&fpz);
    tree->SetBranchAddress("pt",&fpt);
    tree->SetBranchAddress("p",&fp);
    tree->SetBranchAddress("e",&fe);
    
}


Long64_t PlotTree::LoadTree(TTree *fChain, int i, Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TTree::Class()) return centry;
   TTree *chain = (TTree*)fChain;
   if (chain->GetTreeNumber() != fCurrent[i]) {
      fCurrent[i] = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

Bool_t PlotTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}



#endif
