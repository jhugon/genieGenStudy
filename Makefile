
include $(GENIE)/src/make/Make.include

GENIE_LIBS  = $(shell $(GENIE)/src/scripts/setup/genie-config --libs)
#ROOT_LIBS   = $(shell root-config --libs)
LIBRARIES  := $(GENIE_LIBS) $(LIBRARIES) $(CERN_LIBRARIES)

rewght: rewght.cc
	$(CXX) $(CXXFLAGS) -c rewght.cc $(INCLUDES)
	$(LD) $(LDFLAGS) rewght.o $(LIBRARIES) -o rewght

