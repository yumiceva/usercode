# make file to compile analysis macros
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

LIBS          = $(ROOTLIBS)
GLIBS         = $(ROOTGLIBS) -L/usr/X11R6/lib -lXext -lX11

CXX           = g++

ROOTCONFIG   := root-config

ARCH         := $(shell $(ROOTCONFIG) --arch)

CXXFLAGS      = -Wall -Wno-deprecated -m32 -DNOSCRAMV
LDFLAGS       = -fPIC `root-config --libs`
CXXSHARE      = g++ -shared

ifeq ($(ARCH),macosx64)
CXXFLAGS      = -O2 -pipe -Wall -W -Woverloaded-virtual -D_REENTRANT -pthread -m64 -DNOSCRAMV
LDFLAGS       = -fPIC -m64 `root-config --libs`
CXXSHARE      =	g++ -dynamiclib -single_module -undefined dynamic_lookup
endif

ifeq ($(ARCH),macosx)
CXXFLAGS      = -O2 -pipe -Wall -W -Woverloaded-virtual -D_REENTRANT -pthread -m32 -DNOSCRAMV
LDFLAGS       = -fPIC `root-config --libs`
CXXSHARE      = g++ -dynamiclib -single_module -undefined dynamic_lookup
endif


LD_LIBRARY_PATH:= $(ROOTSYS)/lib/:$(LD_LIBRARY_PATH)

INCLUDEDIR    = -I$(ROOTSYS)/include -I../interface 

HEADERS       = TopCandidateEvent.h TopGenCandidateEvent.h TopGenMuonEvent.h TopMyGenEvent.h TopElectronEvent.h TopGenElectronEvent.h TopJetEvent.h TopVertexEvent.h TopEventNtuple.h TopGenJetEvent.h TopMuonEvent.h

OBJS          = TopCandidateEvent.o TopGenCandidateEvent.o TopGenMuonEvent.o TopMyGenEvent.o TopElectronEvent.o TopGenElectronEvent.o TopJetEvent.o TopVertexEvent.o TopEventNtuple.o TopGenJetEvent.o TopMuonEvent.o TopEventNtupleDict.o


all:	$(OBJS) 
	$(CXXSHARE) -o libYumicevaTop7TeV.so  $(LDFLAGS) $(OBJS) $(LIBS)
	@mv libYumicevaTop7TeV.so ../test/tools/libYumicevaTop7TeV.so

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIR) 
%.o: %.C
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIR)

TopEventNtupleDict.cc: TopEventNtupleLinkDef.h
	@echo "Generating dictionary $@"
	rootcint -f $@ -c -DNOSCRAMV -I../interface $(HEADERS) $^

clean:        
	@rm -f $(OBJS) core *~ *.o *.so *Dict.cc *Dict.h
