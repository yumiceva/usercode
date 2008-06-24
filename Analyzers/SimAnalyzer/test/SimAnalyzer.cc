// -*- C++ -*-
//
// Package:    SimAnalyzer
// Class:      SimAnalyzer
// 
/*
 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
//


// system include files
//#include <memory>

#include "Analyzers/SimAnalyzer/test/SimAnalyzer.h"

// user include files
//#include "FWCore/Framework/interface/Frameworkfwd.h"
//#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
//#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
 
#include "TFile.h"
#include "TH1.h"


SimAnalyzer::SimAnalyzer( const edm::ParameterSet& iConfig ):
  fOutputFileName(iConfig.getUntrackedParameter("HistOutFile",std::string("TestHiggsMass.root"))),
  SimTkLabel(iConfig.getUntrackedParameter("moduleLabelTk",std::string("g4SimHits"))),
  SimVtxLabel(iConfig.getUntrackedParameter("moduleLabelVtx",std::string("g4SimHits"))),
  //associate(iConfig.getUntrackedParameter("TkVtxAssociation",bool(true))),
  
  numbTk(0),momentumX(0),momentumY(0),momentumZ(0),momentumPERP(0),trackID(0),type(0),
  numbVtx(0),positionZ(0),parentInd(0),histoPlot(0)
{

}

void
SimAnalyzer::beginJob(const edm::EventSetup& iSetup){

  histoPlot = new TFile(fOutputFileName.c_str(),"RECREATE");

  numbTk  = new TH1D(  "NumberOfTrack"  , "Number of Tracks" , 100,  0., 100. ) ;
  momentumX  = new TH1D(  "MomentumX"  , "Momentum X" , 100,  -2., 2. ) ;
  momentumY  = new TH1D(  "MomentumY"  , "Momentum Y" , 100,  -2., 2. ) ;
  momentumZ  = new TH1D(  "MomentumZ"  , "Momentum Z" , 100,  -40., 40. ) ;
  momentumPERP  = new TH1D(  "MomentumPerp"  , "Momentum Perp" , 100,  0., 5. ) ;
  trackID  = new TH1D(  "TrackID"  , "Track ID" , 300,  0., 600. ) ;
  type  = new TH1D(  "Type"  , "Hep PDT code" , 200,  -400., 400. ) ;

  numbVtx  = new TH1D(  "NumberOfVertex"  , "Number of Vertex" , 100,  0., 100. ) ;
  positionZ  = new TH1D(  "PositionZ"  , "Position Z" , 200,  -300., 300. ) ;
  parentInd  = new TH1D(  "ParentIndex"  , "Parent Index" , 100,  -10., 500. ) ;

  return;

}

//
// member functions
//

// ------------ method called to produce the data  ------------
void
SimAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   using namespace edm;
   
   Handle <HepMCProduct> genProduct;
   iEvent.getByLabel("source", genProduct);
   HepMC::GenEvent myGenEvent = (*(genProduct->GetEvent()));
   
   Handle<SimTrackContainer> SimTk;
   iEvent.getByLabel(SimTkLabel, SimTk);
   const SimTrackContainer *SimTkColl = SimTk.product();
	   
   Handle<SimVertexContainer> SimVtx;
   iEvent.getByLabel(SimVtxLabel,SimVtx);
   const SimVertexContainer *SimVtxColl= SimVtx.product();


   const double ptcut = 2.5; // minimum PT of decaying mesons: DO NOT CHANGE
   const double radcut = 4000.; // in mm for GEN
   const double zcut = 4000.; // in mm for GEN
   const bool FIND_KAON = true;
   const bool FIND_PION = true;
   const bool ONLY_KAON = false;
   const bool ONLY_PION = false;
   const bool BOTH_KAON_AND_PION = false;
   
   const double ptcutSIM = 2.5; // in GeV for simulated muons

   bool muonfound = false;
   double ptmuref = -1.;
   
   SimTrackContainer::const_iterator atrack;
   
   for ( atrack = SimTkColl->begin(); atrack !=  SimTkColl->end(); ++atrack) {

	   int pdgid = atrack->type();
	   if (abs(pdgid)!=13) continue;
	   if (atrack->noVertex()) continue;
	   double pt = atrack->momentum().Pt();
	   if (pt>ptcutSIM) {
		   muonfound = true;
		   ptmuref = pt;
		   break;
	   }
   }

   if (!muonfound) return;

   int igen = -1;
   unsigned int found = 0;
            unsigned int pionfound = 0;
            unsigned int kaonfound = 0;
            double declen_meson = 1.e9;
            double pt_meson = 0.;

   HepMC::GenEvent::particle_iterator pi;
   for ( pi = myGenEvent.particles_begin(); pi != myGenEvent.particles_end(); ++pi ) {
	   HepMC::GenParticle* p = (*pi);
	   int pdgid = p->pdg_id();
	   //if (p->status() != 1) continue;
	   if (!(p->end_vertex())) continue;
	   if (abs(pdgid)!=211 && abs(pdgid)!=321) continue;
	   if (abs(pdgid)==211 && FIND_PION==false) continue;
	   if (abs(pdgid)==321 && FIND_KAON==false) continue;
	   double pt = p->momentum().perp();
	   double eta = p->momentum().eta();
	   double x = p->end_vertex()->position().x();
	   double y = p->end_vertex()->position().y();
	   double z = p->end_vertex()->position().y();
	   double r = sqrt(x*x+y*y);
	   if (pt<ptcut) continue;
	   if (r>radcut || abs(z)>zcut) continue;
	   if (abs(pdgid)==321 && ONLY_PION) {
		   found = 0; 
		   pionfound = 0; 
		   kaonfound = 0; 
		   break;
	   }
	   if (abs(pdgid)==211 && ONLY_KAON) {
		   found = 0; 
		   pionfound = 0; 
		   kaonfound = 0; 
		   break;
	   }
	   
	   printf("Id %d Status %d PDG Id %d Barcode %d\n", igen, p->status(), p->barcode());
	   printf("pt %f eta %f\n", pt, eta);
	   printf("r %f z %f\n", r, z);
	   found++;
	   if (abs(pdgid)==211) pionfound++;
	   if (abs(pdgid)==321) kaonfound++;
	   declen_meson = sqrt(r*r+z*z);
	   pt_meson = pt;

   }
   
   return;

}


void
SimAnalyzer::endJob(){

  histoPlot->Write() ;
  histoPlot->Close() ;
  
  return ;
}
