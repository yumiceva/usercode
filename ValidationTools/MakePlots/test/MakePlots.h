#ifndef MakePlots_h
#define MakePlots_h
/** \class MakePltos
 *
 * Analyze ROOT files produced by analyzer and create plots
 *
 * \author Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)
 *
 * \version $Id: MakePlots.h,v 1.1 2007/09/24 18:26:48 yumiceva Exp $
 *
 */

#include "TString.h"

class MakePlots {

  public:
	
	MakePlots(TString root_filename, TString webpath, TString extension="png", bool compare = false, TString compare_filename="", bool logaxis = false);

	
};

#endif
