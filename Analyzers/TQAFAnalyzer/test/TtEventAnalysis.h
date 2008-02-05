#ifndef TtEventAnalysis_H
#define TtEventAnalysis_H

/** \class TtEventAnalysis
 *
 *  Author: S.C. Kao  - UC Riverside
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "AnalysisDataFormats/TopObjects/interface/TopLepton.h" 
#include "AnalysisDataFormats/TopObjects/interface/TopObject.h" 
#include "AnalysisDataFormats/TopObjects/interface/TopParticle.h" 
#include "AnalysisDataFormats/TopObjects/interface/TopMET.h" 
#include "AnalysisDataFormats/TopObjects/interface/TopJet.h" 
#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h" 
#include "AnalysisDataFormats/TopObjects/interface/TtSemiEvtSolution.h" 

//#include "Analyzers/TQAFAnalyzer/test/TtEventDummyAnalysisHisto.h"
#include "Analyzers/TQAFAnalyzer/test/TQAFHistograms.h"

#include "TFile.h"
#include <vector>
#include <map>
#include <string>
#include <utility> 


class TtEventAnalysis : public edm::EDAnalyzer {

  public:

    /// Constructor
    TtEventAnalysis(const edm::ParameterSet& pset);

    /// Destructor
    virtual ~TtEventAnalysis();

    /// Perform the real analysis
    void analyze(const edm::Event & iEvent, const edm::EventSetup& iSetup);

	typedef math::XYZTLorentzVector LorentzVector;
	
  private: 

    // Histograms
    //HTOP1 *h_Jet;
    //HTOP2 *h_Mass;
    //HTOP3 *h_Other;

	TQAFHistograms *hmuons_;
	TQAFHistograms *hmet_;
	TQAFHistograms *hjets_;
	TQAFHistograms *hgen_;
	TQAFHistograms *hmass_;
	TQAFHistograms *hdisp_;
	
	std::map<TString, TString> cut_map;

    // TNtuple1 *tr_muon;

    // The file which will store the histos
    TFile *theFile;

    // Switch for debug output
    bool debug;
	bool ffilter;
	bool fdisplayJets;
	int feventToProcess;
	
    std::string rootFileName;
    std::string leptonFlavour;
	edm::InputTag genEvnSrc;
    edm::InputTag muonSrc;
    edm::InputTag electronSrc;
    edm::InputTag metSrc;
    edm::InputTag bjetSrc;
	edm::InputTag ljetSrc;
    edm::InputTag bjetObj;

    edm::InputTag evtsols;

	int nevents;
	

};


#endif
