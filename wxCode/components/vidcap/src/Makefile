# ----------------------------------------------------------------------------
# Purpose : Makefile for wxvidcap lib - UNIX or cygwin using gmake (gcc)
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
APPEXTRADEFS = -I../include $(PROFILE)
APPEXTRALIBS = $(PROFILE)

# ----------------------------------------------------------------------------
# Programs to use

RM        = rm -f
AR        = ar
AROPTIONS = rcu
RANLIB    = ranlib
LN_S      = ln -s

# ----------------------------------------------------------------------------
# Source files, objects to link

VIDCAP_SOURCES = \
    vcapwin \
    vcapdlgs \
    vcap_v4l \
    vcapconv

SOURCES=$(VIDCAP_SOURCES)
HEADERS=$(addsuffix .h, $(basename $(SOURCES)))
OBJECTS=$(addsuffix .o, $(basename $(SOURCES)))

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
	cd $(WXLIBDIR) \
		&& $(RM) $(TARGET_LIB_LINK1) $(TARGET_LIB_LINK2) \
		&& $(LN_S) $(TARGET_LIB_SHARED) $(TARGET_LIB_LINK1) \
		&& $(LN_S) $(TARGET_LIB_SHARED) $(TARGET_LIB_LINK2)

clean:
	rm -f $(OBJECTS) $(DEPFILES) $(PROGRAM) $(PROGRAM).o $(PROGRAM).d core \
		$(TARGET_LIB_STATIC) \
		$(TARGET_LIB_SHARED) \
		$(WXVIDCAP_LIBDIR)/$(TARGET_LIB_LINK1) \
		$(WXVIDCAP_LIBDIR)/$(TARGET_LIB_LINK2)


-include $(DEPFILES)
