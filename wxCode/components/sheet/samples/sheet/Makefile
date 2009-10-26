# ----------------------------------------------------------------------------
# Purpose : Makefile for wxsheet sample - UNIX or cygwin using gmake (gcc)
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

WXSHEET_LIBDIR  = $(WXLIBDIR)
WXSHEET_LIBNAME = $(WXBASENAME)_sheet-$(WXRELEASE)
WXSHEET_LIBNAME := $(patsubst wx_%,wxcode_%,$(WXSHEET_LIBNAME))

LIBVERSION_CURRENT  = 1
LIBVERSION_REVISION = 0
LIBVERSION_AGE      = 0

TARGET_LIBNAME = lib$(WXSHEET_LIBNAME)
TARGET_LIBNAME_STATIC = $(TARGET_LIBNAME).a
TARGET_LIBNAME_SHARED = $(TARGET_LIBNAME).so.$(LIBVERSION_CURRENT).$(LIBVERSION_REVISION).$(LIBVERSION_AGE)
TARGET_LIB_STATIC = $(WXSHEET_LIBDIR)/$(TARGET_LIBNAME_STATIC)
TARGET_LIB_SHARED = $(WXSHEET_LIBDIR)/$(TARGET_LIBNAME_SHARED)
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
APPEXTRALIBS =  $(PROFILE)

# ----------------------------------------------------------------------------
# Source files, objects to link

PROGRAM = sheetdemo

OBJECTS = \
	../../src/sheet.o \
	../../src/sheetatr.o \
	../../src/sheetedg.o \
	../../src/sheetedt.o \
	../../src/sheetren.o \
	../../src/sheetsel.o \
	../../src/sheetspt.o \
	../../src/sheettbl.o \
	../../src/sheetval.o

DEPFILES := $(OBJECTS:.o=.d)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(APPEXTRADEFS) -o $@ $<

all: $(WXSHEET_LIBNAME) $(PROGRAM)


$(WXSHEET_LIBNAME) :
	@(cd ../../src && make)

# sheetdemo program using shared sheet library
$(PROGRAM): $(PROGRAM).o $(TARGET_LIB_SHARED)
	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(LDLIBS) $(APPEXTRALIBS) -l$(WXSHEET_LIBNAME)

# sheetdemo program using static sheet library
#$(PROGRAM): $(PROGRAM).o $(TARGET_LIB_STATIC)
#	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(LDLIBS) $(APPEXTRALIBS) $(TARGET_LIB_STATIC)

# sheetdemo program using just object files and not the sheet lib
#$(PROGRAM): $(PROGRAM).o $(OBJECTS)
#	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(OBJECTS) $(LDLIBS) $(APPEXTRALIBS)

clean:
	rm -f $(OBJECTS) $(DEPFILES) $(PROGRAM) $(PROGRAM).o $(PROGRAM).d core

cleanall: clean
	@(cd ../../src && make clean)


-include $(DEPFILES)
