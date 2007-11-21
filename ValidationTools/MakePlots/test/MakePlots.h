#ifndef MakePlots_h
#define MakePlots_h
/** \class MakePltos
 *
 * Analyze ROOT files produced by analyzer and create plots
 *
 * \author Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)
 *
 * \version $Id: MakePlots.h,v 1.2 2007/11/09 16:57:32 yumiceva Exp $
 *
 */

#include "TString.h"

class MakePlots {

  public:
	MakePlots();
	//MakePlots(TString root_filename, TString webpath, TString extension="png", bool compare = false, TString compare_filename="", bool logaxis = false);
	void Draw();
	void SetFilename(TString name) { root_filename = name; }
	void SetWebPath(TString name) { webpath = name; }
	void SetExtension(TString name) { extension = name; }
	void SetCompare( bool option ) { compare = option; }
	void SetCompareFilename(TString name) { compare_filename = name; }
	void SetLogAxis( bool option) { logaxis = option; }
	void SetReleaseVer(TString name) {release_version = name;}
	void SetCompareVer(TString name) {compare_version = name;}
  private:
	TString root_filename;
	TString webpath;
	TString extension;
	bool compare;
	TString compare_filename;
	bool logaxis;
	TString release_version;
	TString compare_version;

	
};

#endif
