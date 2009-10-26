# ----------------------------------------------------------------------------
# Purpose : Makefile for wxplotctrl sample - UNIX or cygwin using gmake (gcc)
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

WXPLOTCTRL_LIBDIR  = $(WXLIBDIR)
WXPLOTCTRL_LIBNAME = $(WXBASENAME)_plotctrl-$(WXRELEASE)
WXPLOTCTRL_LIBNAME := $(patsubst wx_%,wxcode_%,$(WXPLOTCTRL_LIBNAME))

LIBVERSION_CURRENT  = 1
LIBVERSION_REVISION = 0
LIBVERSION_AGE      = 0

TARGET_LIBNAME = lib$(WXPLOTCTRL_LIBNAME)
TARGET_LIBNAME_STATIC = $(TARGET_LIBNAME).a
TARGET_LIBNAME_SHARED = $(TARGET_LIBNAME).so.$(LIBVERSION_CURRENT).$(LIBVERSION_REVISION).$(LIBVERSION_AGE)
TARGET_LIB_STATIC = $(WXPLOTCTRL_LIBDIR)/$(TARGET_LIBNAME_STATIC)
TARGET_LIB_SHARED = $(WXPLOTCTRL_LIBDIR)/$(TARGET_LIBNAME_SHARED)
TARGET_LIB_LINK1  = $(TARGET_LIBNAME).so.$(LIBVERSION_CURRENT)
TARGET_LIB_LINK2  = $(TARGET_LIBNAME).so

# ----------------------------------------------------------------------------
# the location of wxCode, where wxThings is located

WXCODE_DIR   = ../../../..
WXTHINGS_DIR = $(WXCODE_DIR)/components/wxthings
WXTHINGS_LIBNAME = $(WXBASENAME)_wxthings-$(WXRELEASE)
WXTHINGS_LIBNAME := $(patsubst wx_%,wxcode_%,$(WXTHINGS_LIBNAME))

# ----------------------------------------------------------------------------
# CXX flags and libs

CXXFLAGS = $(WXCXXFLAGS) -MMD -g  -Wall -Wunused -Wunused-parameter -fPIC -DPIC
LDLIBS   = $(WXLIBS)
CXX      = $(WXCXX)

# -pg in libs and defs for profiling
PROFILE = #-pg
APPEXTRADEFS = -I../../include -I$(WXTHINGS_DIR)/include $(PROFILE)
APPEXTRALIBS = $(PROFILE)

# ----------------------------------------------------------------------------
# Source files, objects to link

PROGRAM = wxplotctrl

OBJECTS = \
    $(WXTHINGS_DIR)/src/block.o \
    $(WXTHINGS_DIR)/src/genergdi.o \
    $(WXTHINGS_DIR)/src/geometry.o \
    $(WXTHINGS_DIR)/src/range.o

DEPFILES := $(OBJECTS:.o=.d)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(APPEXTRADEFS) -o $@ $<

all: $(WXPLOTCTRL_LIBNAME) $(WXTHINGS_LIBNAME) $(PROGRAM)

$(WXPLOTCTRL_LIBNAME) :
	@(cd ../../src && make)

$(WXTHINGS_LIBNAME) :
	@(cd $(WXTHINGS_DIR)/src && make)

# wxplotctrl program using shared plotctrl library
$(PROGRAM): $(PROGRAM).o $(TARGET_LIB_SHARED)
	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o -l$(WXPLOTCTRL_LIBNAME) -l$(WXTHINGS_LIBNAME) $(APPEXTRALIBS) $(LDLIBS)

# wxplotctrl program using static stedit library
#$(PROGRAM): $(PROGRAM).o $(TARGET_LIB_STATIC)
#	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(LDLIBS) $(TARGET_LIB_STATIC) $(APPEXTRALIBS)

# wxplotctrl program using just object files and not the stedit lib
#$(PROGRAM): $(PROGRAM).o $(OBJECTS)
#	$(CXX) $(LDFLAGS_EXE) -o $@ $(PROGRAM).o $(OBJECTS) $(LDLIBS) $(APPEXTRALIBS)

clean:
	rm -f $(OBJECTS) $(DEPFILES) $(PROGRAM) $(PROGRAM).o $(PROGRAM).d core

cleanall: clean
	@(cd ../../src && make clean)


-include $(DEPFILES)
