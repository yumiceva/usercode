# make file to compile analysis macros
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

LIBS          = $(ROOTLIBS)
GLIBS         = $(ROOTGLIBS) -L/usr/X11R6/lib -lXext -lX11

CXX           = gcc

CXXFLAGS      = -fPIC -Wall -Wno-deprecated -m32 -DNOSCRAMV
LDFLAGS       = -fPIC -Wl -m32

LD_LIBRARY_PATH:= $(ROOTSYS)/lib/:$(LD_LIBRARY_PATH)

INCLUDEDIR    = -I$(ROOTSYS)/include -I../interface 

HEADERS       = TopCandidateEvent.h TopElectronEvent.h TopEventNtuple.h TopJetEvent.h TopMuonEvent.h TopVertexEvent.h
OBJS          = TopCandidateEvent.o TopElectronEvent.o TopEventNtuple.o TopJetEvent.o TopMuonEvent.o TopVertexEvent.o

all:	$(OBJS)
	$(CXX) -shared -o libYumicevaTop7TeV.so  $(LDFLAGS) $(OBJS) $(LIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIR) 
%.o: %.C
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIR)

%Dict.cc: %LinkDef.h
	@echo "Generating dictionary $@"
	@rootcint -f $@ -c -DNOSCRAMV -I../interface $(HEADERS) $^

clean:        
	@rm -f $(OBJS) core *~ *.o *.so 