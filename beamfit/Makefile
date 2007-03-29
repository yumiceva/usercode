# Makefile for the ROOT test programs.
# This Makefile shows nicely how to compile and link applications
# using the ROOT libraries on all supported platforms.
#
# Copyright (c) 2000 Rene Brun and Fons Rademakers
#
# Author: Fons Rademakers, 29/2/2000

include Makefile.arch

#------------------------------------------------------------------------------

TNTO       = tnt.$(ObjSuf) tntDict.$(ObjSuf) 
TNTS       = tnt.$(SrcSuf) tntDict.$(SrcSuf) 
TNTSO      = tnt.$(DllSuf) 

GENO       = gen.$(ObjSuf)  
GENS       = gen.$(SrcSuf)  
GEN        = gen$(ExeSuf)

DPHI_FITO       = dphi_fit.$(ObjSuf)  
DPHI_FITS       = dphi_fit.$(SrcSuf)  
DPHI_FIT        = dphi_fit$(ExeSuf)

BEAMFITO       = beamfit.$(ObjSuf) 
BEAMFITS       = beamfit.$(SrcSuf)  
BEAMFIT        = beamfit$(ExeSuf)

BEAMFITALLO    = beamfitall.$(ObjSuf) 
BEAMFITALLS    = beamfitall.$(SrcSuf)  
BEAMFITALL     = beamfitall$(ExeSuf)

TRACKSO       = Tracks.$(ObjSuf)
TRACKSS       = Tracks.$(SrcSuf)
TRACKS        = Tracks$(ExeSuf)


OBJS    =   $(TNTO)  $(BEAMFITO)  $(DPHI_FITO) $(TRACKSO)

INCLUDES = tnt.h global.h Tracks.h

PROGRAMS= $(TNTSO) $(BEAMFIT) $(BEAMFITALL) $(GEN)  $(DPHI_FIT) $(TRACKS)

all : $(PROGRAMS) 

$(TNTSO):    $(TNTO) $(TNTS)  $(INCLUDES)
	$(LD) $(SOFLAGS) $(LDFLAGS) $(TNTO)  $(OutPutOpt) $(TNTSO)
	@echo "$(TNTSO) done"

$(GEN):    $(GENO) $(GENS)  $(INCLUDES)
	$(LD) $(LDFLAGS)  $(GENO)  $(LIBS) $(OutPutOpt) $(GEN) 
	@echo "$(GEN) done"

$(DPHI_FIT):    $(DPHI_FITO) $(DPHI_FITS) $(TNTSO)  $(TNTO) $(INCLUDES)
	$(LD) $(LDFLAGS)  $(DPHI_FITO) $(TNTSO)  $(TNTO)  $(LIBS) $(OutPutOpt) $(DPHI_FIT) 
	@echo "$(DPHI_FIT) done"

$(BEAMFIT): $(TNTSO)  $(TNTO) $(BEAMFITO)   $(INCLUDES)
	$(LD) $(LDFLAGS) $(TNTSO)  $(TNTO) $(BEAMFITO)  $(LIBS) $(OutPutOpt) $(BEAMFIT)
	@echo "$(BEAMFIT) done"

$(BEAMFITALL): $(TNTSO)  $(TNTO) $(BEAMFITALLO)   $(INCLUDES)
	$(LD) $(LDFLAGS) $(TNTSO)  $(TNTO) $(BEAMFITALLO)  $(LIBS) $(OutPutOpt) $(BEAMFITALL)
	@echo "$(BEAMFITALL) done"

$(TRACKS):  $(TRACKSS) $(TRACKSO)  $(INCLUDES)
	$(LD) $(LDFLAGS)  $(TRACKSO)  $(LIBS) $(OutPutOpt) $(TRACKS)
	@echo "$(TRACKS) done"

clean:
	@rm -f $(OBJS) $(PROGRAMS) *Dict.* core *.o *.so

tidy:
	@rm -f $(OBJS)  *Dict.* core *.o *.so *~ $(PROGRAMS)
.SUFFIXES: .$(SrcSuf) .$(ObjSuf)


tnt.$(ObjSuf): tnt.h 
tntDict.$(SrcSuf): tnt.h
	@echo "Generating dictionary tntDict..."
	@$(ROOTSYS)/bin/rootcint -f  tntDict.$(SrcSuf) -c  tnt.h

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<
