#ifdef NOSCRAMV
#include "TopEventNtuple.h"
#include "TopBaseEvent.h"
#include "TopMuonEvent.h"
#include "TopJetEvent.h"
#include "TopElectronEvent.h"
//#include "TopVertexEvent.h"
#else
#include "Yumiceva/Top7TeV/interface/TopEventNtuple.h"
#include "Yumiceva/Top7TeV/interface/TopBaseEvent.h"
#include "Yumiceva/Top7TeV/interface/TopMuonEvent.h"
#include "Yumiceva/Top7TeV/interface/TopElectronEvent.h"
#include "Yumiceva/Top7TeV/interface/TopJetEvent.h"
//#include "Yumiceva/Top7TeV/interface/TopVertexEvent.h"
#endif
 
#ifdef __CINT__
 
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ class TopEventNtuple+;
#pragma link C++ class TopBaseEvent+;
#pragma link C++ class TopMuonEvent+;
#pragma link C++ class TopJetEvent+;
#pragma link C++ class TopElectronEvent+;
//#pragma link C++ class TopVertexEvent+;

#endif
