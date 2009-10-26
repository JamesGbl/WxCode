# ----------------------------------------------------------------------------
# Purpose : Makefile for wxsheet lib - UNIX or cygwin using gmake (gcc)
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
APPEXTRADEFS = -I../include $(PROFILE)
APPEXTRALIBS =  $(PROFILE)

# ----------------------------------------------------------------------------
# Programs to use

RM        = rm -f
AR        = ar
AROPTIONS = rcu
RANLIB    = ranlib
LN_S      = ln -s

# ----------------------------------------------------------------------------
# Source files, objects to link

OBJECTS = \
	sheet.o \
	sheetatr.o \
	sheetedg.o \
	sheetedt.o \
	sheetren.o \
	sheetsel.o \
	sheetspt.o \
	sheettbl.o \
	sheetval.o

DEPFILES := $(OBJECTS:.o=.d)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(APPEXTRADEFS) -o $@ $<

all: $(TARGET_LIB_STATIC) $(TARGET_LIB_SHARED)

$(TARGET_LIB_STATIC) : $(OBJECTS)
		@$(RM) $(TARGET_LIB_STATIC)
		$(AR) $(AROPTIONS) $(TARGET_LIB_STATIC) $(OBJECTS)
		$(RANLIB) $(TARGET_LIB_STATIC)

$(TARGET_LIB_SHARED) : $(OBJECTS)
		$(CXX) -shared -o $(TARGET_LIB_SHARED) $(OBJECTS) $(WXLIBS)
		cd $(WXSHEET_LIBDIR) \
			&& $(RM) $(TARGET_LIB_LINK1) $(TARGET_LIB_LINK2) \
				&& $(LN_S) $(TARGET_LIB_SHARED) $(TARGET_LIB_LINK1) \
				&& $(LN_S) $(TARGET_LIB_SHARED) $(TARGET_LIB_LINK2)

clean:
	rm -f $(OBJECTS) $(DEPFILES) core \
		$(TARGET_LIB_STATIC) \
		$(TARGET_LIB_SHARED) \
		$(WXSHEET_LIBDIR)/$(TARGET_LIB_LINK1) \
		$(WXSHEET_LIBDIR)/$(TARGET_LIB_LINK2)


-include $(DEPFILES)
