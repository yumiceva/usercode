// $Id: GenTTTTCycle.cxx,v 1.2 2012/05/23 13:20:10 peiffer Exp $

// Local include(s):
#include "include/GenTTTTCycle.h"

ClassImp( GenTTTTCycle );


GenTTTTCycle::GenTTTTCycle()
   : SCycleBase() {

   SetLogName( GetName() );
}

GenTTTTCycle::~GenTTTTCycle() {

}

void GenTTTTCycle::BeginCycle() throw( SError ) {
  
  return;

}

void GenTTTTCycle::EndCycle() throw( SError ) {

  return;

}

void GenTTTTCycle::BeginInputData( const SInputData& ) throw( SError ) {
  

  //
  // Declare the output histograms:
  //
  Book( TH1F( "top_pt_gen", "p_{T}^{gen}", 100,0,350 ) );
  Book( TH1F( "top_mindeltaR_gen", "minimum #Delta R(t,t)",100,0,7) );

  return;

}

void GenTTTTCycle::EndInputData( const SInputData& ) throw( SError ) {

  //TGraphAsymmErrors *eff = new TGraphAsymmErrors(Hist("pt_rectau_hist"), Hist("pt_gentau_hist"), "cp");
  //eff->SetName("pt_tau_effi");
  //eff->SetTitle("pt_tau_effi");

  //Book( *eff );
   return;

}

void GenTTTTCycle::BeginInputFile( const SInputData& ) throw( SError ) {

  ConnectVariable( "AnalysisTree", "GenParticles" , bcc.genparticles);
  //  ConnectVariable( "AnalysisTree", "selectedPatTausPFlow" , bcc.taus); 

  return;

}

void GenTTTTCycle::ExecuteEvent( const SInputData&, Double_t weight) throw( SError ) {

  
  GenParticle gentop[4];
  int itop = 0;

  for(unsigned int i=0; i<bcc.genparticles->size(); ++i){
    GenParticle genp = bcc.genparticles->at(i);
    
    //if(abs(genp.pdgId())!=15) continue;
    if(fabs(genp.eta())>2.5) continue;

    if(fabs(genp.pdgId()) != 6 ) continue;
    
    gentop[itop] = genp;
    itop++;

    // pt of top quarks
    Hist("top_pt_gen")->Fill(genp.pt());

  }

  float deltaR = 9999.0;

  for (int j=0; j<4; j++) {

    for (int k=j+1; k<4; k++) {

      float tmpdeltaR = gentop[j].deltaR(gentop[k]);

      if (tmpdeltaR < deltaR) deltaR = tmpdeltaR;

    }
  }

  Hist("top_mindeltaR_gen")->Fill( deltaR );

  
  return;

}

