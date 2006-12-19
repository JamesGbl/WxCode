prefix = $(shell wx-config --prefix)
exec_prefix = $(shell wx-config --exec-prefix)
srcdir = .
top_srcdir = $(prefix)/
top_builddir = $(exec_prefix)/

CXX = g++
CXXFLAGS = -Wall -Wundef -Wno-ctor-dtor-privacy -O0 -fno-common -I./include
CPPFLAGS = $(shell wx-config --cppflags) -fpascal-strings -I${top_srcdir}/src/mac/carbon/morefilex -I/Developer/Headers/FlatCarbon -gdwarf-2 -g3 -arch i386 -ftree-vectorize -march=prescott -msse2 -mfpmath=sse -isysroot /Developer/SDKs/MacOSX10.4u.sdk
LDFLAGS =  $(shell wx-config --libs) -dynamiclib

SRCS = src/servicediscoverybrowser.cpp src/servicediscoveryresolver.cpp src/servicediscoveryresult.cpp src/servicediscoveryservice.cpp src/servicediscoverytaskbase.cpp
OBJS = src/servicediscoverybrowser.o src/servicediscoveryresolver.o src/servicediscoveryresult.o src/servicediscoveryservice.o src/servicediscoverytaskbase.o

all:  wxDNSSD-2.7.0.dylib

.cpp:
	$(CXX) ${CPPFLAGS} ${CXXFLAGS} -c $@.cpp -o $@
	

wxDNSSD-2.7.0.dylib: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o wxDNSSD-2.7.0.dylib

clean:
	rm -f $(OBJS)

src/servicediscoverybrowser.o: src/servicediscoverybrowser.cpp
src/servicediscoveryresolver.o: src/servicediscoveryresolver.cpp
src/servicediscoveryresult.o: src/servicediscoveryresult.cpp
src/servicediscoveryservice.o: src/servicediscoveryservice.cpp
src/servicediscoverytaskbase.o: src/servicediscoverytaskbase.cpp
