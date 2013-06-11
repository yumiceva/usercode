// Dear emacs, this is -*- c++ -*-
#ifndef FourtopsPreSelectionCycle_H
#define FourtopsPreSelectionCycle_H

// SFrame include(s):
#include "SError.h"
#include "SFrameAnalysis/include/AnalysisCycle.h"
#include "SFrameTools/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "SFrameAnalysis/include/Cleaner.h"

/**
 *  @short Selection cycle to perform 
 *         pre-selection for  analysis
 *  @author Thomas Peiffer
 */


class FourtopsPreSelectionCycle : public AnalysisCycle {

public:
  /// Default constructor
  FourtopsPreSelectionCycle();
  /// Default destructor
  ~FourtopsPreSelectionCycle();

  /// Function called at the beginning of the cycle
  void BeginCycle() throw( SError );
  /// Function called at the end of the cycle
  void EndCycle() throw( SError );

  /// Function called at the beginning of a new input data
  void BeginInputData( const SInputData& ) throw( SError );
  /// Function called after finishing to process an input data
  void EndInputData  ( const SInputData& ) throw( SError );

  /// Function called after opening each new input file
  void BeginInputFile( const SInputData& ) throw( SError );

  /// Function called for every event
  void ExecuteEvent( const SInputData&, Double_t ) throw( SError );

private:
  //
  // Put all your private variables here
  //
  
  std::string m_Electron_Or_Muon_Selection;

  FactorizedJetCorrector* m_corrector;

  // Macro adding the functions for dictionary generation
  ClassDef( FourtopsPreSelectionCycle, 0 );

}; // class FourtopsPreSelectionCycle

#endif // FourtopsPreSelectionCycle_H

