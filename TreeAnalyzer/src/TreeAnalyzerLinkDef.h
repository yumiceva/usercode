#include "Yumiceva/TreeAnalyzer/interface/MuonSelector.h"
#include "Yumiceva/TreeAnalyzer/interface/ElectronSelector.h"
#include "Yumiceva/TreeAnalyzer/interface/HistoManager.h"
#include "Yumiceva/TreeAnalyzer/interface/METzCalculator.h"
#include "Yumiceva/TreeAnalyzer/interface/combination.h"
#include "Yumiceva/TreeAnalyzer/interface/JetCombinatorics.h"
#include "TH1.h"

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class MuonSelector+;
#pragma link C++ class ElectronSelector+;
#pragma link C++ class HistoManager+;
#pragma link C++ class METzCalculator+;
#pragma link C++ class JetCombinatorics+;

#pragma link C++ class std::map< string, TH1* >;
#pragma link C++ class std::map< string, TH1* >::const_iterator;

#endif
