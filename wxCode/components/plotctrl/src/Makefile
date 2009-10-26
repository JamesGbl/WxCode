# ----------------------------------------------------------------------------
# Purpose : Makefile for wxplotctrl lib - UNIX or cygwin using gmake (gcc)
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
WXCFLAGS   := $(shell $(WXCONFIG) --cflags)
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
# find the GTK headers
# in plotdraw.cpp wxPLOT_FAST_GRAPHICS we draw on dc using native code

ifndef GTK_CFLAGS
    ifeq (gtk2, $(findstring gtk2, $(WXBASENAME)))
        GTK_CFLAGS := $(shell pkg-config --cflags gtk+-2.0)
    else
        ifeq (gtk, $(findstring gtk, $(WXBASENAME)))
            GTK_CFLAGS := $(shell gtk-config --cflags)
        else
            GTK_CFLAGS =
        endif
    endif
endif

# ----------------------------------------------------------------------------
# the location of wxCode, where wxThings is located

WXCODE_DIR   = ../../..
WXTHINGS_DIR = $(WXCODE_DIR)/components/wxthings
WXTHINGS_LIBNAME = $(WXBASENAME)_wxthings-$(WXRELEASE)
WXTHINGS_LIBNAME := $(patsubst wx_%,wxcode_%,$(WXTHINGS_LIBNAME))

# ----------------------------------------------------------------------------
# CXX flags and libs

CXXFLAGS = $(WXCXXFLAGS) $(GTK_CFLAGS) -MMD -g  -Wall -Wunused -Wunused-parameter -fPIC -DPIC -Wextra
CFLAGS   = $(WXCFLAGS) -MMD -g  -Wall -Wunused -Wunused-parameter -fPIC -DPIC
LDLIBS   = $(WXLIBS)
CXX      = $(WXCXX)

# -pg in libs and defs for profiling
PROFILE = #-pg
APPEXTRADEFS = -I../include -I$(WXTHINGS_DIR)/include $(PROFILE)
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
    fourier.o \
    fparser.o \
    fpoptimizer.o \
    lm_lsqr.o \
    plotctrl.o \
    plotcurv.o \
    plotdata.o \
    plotdraw.o \
    plotfunc.o \
    plotmark.o \
    plotprnt.o

DEPFILES := $(OBJECTS:.o=.d)

.c.o:
	$(CC) -c $(CFLAGS) $(APPEXTRADEFS) -o $@ $<

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(APPEXTRADEFS) -o $@ $<

all: $(TARGET_LIB_STATIC) $(TARGET_LIB_SHARED)

$(TARGET_LIB_STATIC) : $(OBJECTS)
		@$(RM) $(TARGET_LIB_STATIC)
		$(AR) $(AROPTIONS) $(TARGET_LIB_STATIC) $(OBJECTS)
		$(RANLIB) $(TARGET_LIB_STATIC)

$(TARGET_LIB_SHARED) : $(OBJECTS)
		$(CXX) -shared -o $(TARGET_LIB_SHARED) $(OBJECTS) $(WXLIBS) -l$(WXTHINGS_LIBNAME)
		cd $(WXPLOTCTRL_LIBDIR) \
			&& $(RM) $(TARGET_LIB_LINK1) $(TARGET_LIB_LINK2) \
				&& $(LN_S) $(TARGET_LIB_SHARED) $(TARGET_LIB_LINK1) \
				&& $(LN_S) $(TARGET_LIB_SHARED) $(TARGET_LIB_LINK2)

clean:
	rm -f $(OBJECTS) $(DEPFILES) core \
		$(TARGET_LIB_STATIC) \
		$(TARGET_LIB_SHARED) \
		$(WXPLOTCTRL_LIBDIR)/$(TARGET_LIB_LINK1) \
		$(WXPLOTCTRL_LIBDIR)/$(TARGET_LIB_LINK2)


-include $(DEPFILES)
