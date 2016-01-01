# ROOT part
ROOTCONFIG = root-config
CXX = c++
CXXso = c++
TUNEFLAG = $(shell $(CXX) --version | \
	   awk '// { for (i = 1; i <= NF; ++i) if ($$i ~ /[0-9]*\.[0-9]*\.[0-9]*$$/) {if ($$i < "4.2") print "-mtune=opteron"; else print "-mtune=native"; }; }')
CXXFLAGS = -Wall -Wextra -O2 $(TUNEFLAG) $(shell $(ROOTCONFIG) --cflags)
LDFLAGS = $(shell root-config --libs)
SOFLAGS = -fPIC -shared

# source and Dictionary files
allDicts = rtgpDict.cpp
allsrc = $(wildcard *.cpp) $(allDicts)

# general targets
all: dep librtgp.a librtgp.so quickplot

dep: Dicts $(patsubst %.cpp,%.d,$(allsrc) $(allDicts))

Dicts: $(allDicts)

clean: cleanDicts cleandeps
	rm -f *.o *.os *~ lib* quickplot

cleanDicts:
	rm -f *Dict*

cleandeps:
	rm -f *.d

distclean: cleandeps clean
	rm -f librtgp.so librtgp.a

# how to link libZooROOT.so
librtgp.so: $(patsubst %.cpp,%.os,$(allsrc))
#	$(CXXso) $(SOFLAGS) $^ $(shell ls ../Zoo/*.os) -o $@
	$(CXXso) $(SOFLAGS) $^ -o $@

librtgp.a: librtgp.a($(patsubst %.cpp,%.o,$(allsrc)))

rtgpDict.cpp: $(filter-out rtgpDict.h,$(wildcard *.h))

# General rules
%.d: %.cpp
	$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

%.os: %.cpp
	$(CXXso) -fPIC $(CXXFLAGS) -c $< -o $@

# ROOT rules (??? should I include them from somewhere in ROOT ???)
%Dict.cpp:
	$(shell $(ROOTCONFIG) --bindir)/rootcint -f $@ -c -p $(filter-out %LinkDef.h,$^) $(filter %LinkDef.h,$^) || rm -f $@

# include dependencies
include $(patsubst %.cpp,%.d,$(allsrc))

quickplot: quickplot.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ -L/usr/lib/x86_64-linux-gnu -lboost_iostreams -lboost_system -lboost_filesystem -L. -lrtgp
