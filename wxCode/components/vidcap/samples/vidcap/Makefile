# ----------------------------------------------------------------------------
# Purpose : Makefile for wxvidcap sample - UNIX or cygwin using gmake (gcc)
# Author : John Labenski
# Copyright : John Labenski 2006
# License : wxWidgets license
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# wxWidgets build settings

WXCONFIG   := wx-config

WXDIR      := $(shell $(WXCONFIG) --prefix)
WXEXECDIR  := $(shell $(WXCONFIG) --exec-prefix)
WXLIBDIR   := $(WXEXECDIR)/lib

WXBASENAME := $(shell $(WXCONFIG) --basename)
WXVERSION  := $(shell $(WXCONFIG) --version)
WXRELEASE  := $(shell $(WXCONFIG) --release)

WXCXX      := $(shell $(WXCONFIG) --cxx)
WXCXXFLAGS := $(shell $(WXCONFIG) --cxxflags)
WXLIBS     := $(shell $(WXCONFIG) --libs)

WXSTC_LIB  := $(WXBASENAME)_stc-$(WXRELEASE)

# ----------------------------------------------------------------------------
# Settings for the library

WXVIDCAP_LIBDIR  = $(WXLIBDIR)
WXVIDCAP_LIBNAME = $(WXBASENAME)_vidcap-$(WXRELEASE)
WXVIDCAP_LIBNAME := $(patsubst wx_%,wxcode_%,$(WXVIDCAP_LIBNAME))

LIBVERSION_CURRENT  = 1
LIBVERSION_REVISION = 0
LIBVERSION_AGE      = 0

TARGET_LIBNAME = lib$(WXVIDCAP_LIBNAME)
TARGET_LIBNAME_STATIC = $(TARGET_LIBNAME).a
TARGET_LIBNAME_SHARED = $(TARGET_LIBNAME).so.$(LIBVERSION_CURRENT).$(LIBVERSION_REVISION).$(LIBVERSION_AGE)
TARGET_LIB_STATIC = $(WXVIDCAP_LIBDIR)/$(TARGET_LIBNAME_STATIC)
TARGET_LIB_SHARED = $(WXVIDCAP_LIBDIR)/$(TARGET_LIBNAME_SHARED)
TARGET_LIB_LINK1  = $(TARGET_LIBNAME).so.$(LIBVERSION_CURRENT)
TARGET_LIB_LINK2  = $(TARGET_LIBNAME).so

# ----------------------------------------------------------------------------
# CXX flags and libs

CXXFLAGS = $(WXCXXFLAGS) -MMD -g  -Wall -Wunused -Wunused-parameter -fPIC -DPIC
LDLIBS   = $(WXLIBS)
CXX      = $(WXCXX)

# -pg in libs and defs for profiling
PROFILE = #-pg
APPEXTRADEFS = -I../../include $(PROFILE)
APPEXTRALIBS = $(PROFILE)

# ----------------------------------------------------------------------------
# Source files, objects to link

PROGRAM = wxvidcap

OBJECTS = \
	wxvidcap.o \
	wxvidcap_wdr.o

DEPFILES := $(OBJECTS:.o=.d)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(APPEXTRADEFS) -o $@ $<

all: $(WXVIDCAP_LIBNAME) $(PROGRAM)

$(WXVIDCAP_LIBNAME) :
	@(cd ../../src && make)

# wxvidcap program using shared vidcap library
$(PROGRAM): $(OBJECTS) $(TARGET_LIB_SHARED)
	$(CXX) $(LDFLAGS_EXE) -o $@ $(OBJECTS) $(LDLIBS) $(APPEXTRALIBS) -l$(WXVIDCAP_LIBNAME)

# wxvidcap program using static vidcap library
#$(PROGRAM): $(PROGRAM).o $(TARGET_LIB_STATIC)
#	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(LDLIBS) $(APPEXTRALIBS) $(TARGET_LIB_STATIC)

# wxvidcap program using just object files and not the vidcap lib
#$(PROGRAM): $(PROGRAM).o $(OBJECTS)
#	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(OBJECTS) $(LDLIBS) $(APPEXTRALIBS)

clean:
	rm -f $(OBJECTS) $(DEPFILES) $(PROGRAM) $(PROGRAM).o $(PROGRAM).d core

cleanall: clean
	@(cd ../../src && make clean)


-include $(DEPFILES)
