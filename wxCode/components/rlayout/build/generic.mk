
# Generic things for GNU cross platform makefile

# Current OS 
# Windows or Linux supported now, but can be modified
PLATFORM= $(if $(findstring Windows,$(OS)),win32,linux)
CWD= $(shell pwd)

# Basic settings to all projects in shared environment 
ifeq ($(PLATFORM),win32)
  detest= if exist $(1)\nul echo 1
  fetest= if exist $(1) echo 1
  # Projects location
  PROJ:= G:/proj
  CPP:= mingw32-g++
  RC:= windres.exe
  EXESUFF:=.exe
  SOSUFF:=.dll
  # -Wno-attributes  gcc 4.1 - suppress warnings
  CPPFLAGS:= -mthreads
  DEFS:= -D__GNUWIN32__ 
  # Fix for g++ bug on win32, make g++ see system include dirs before app dirs
  #INCZ:= -IC:\mingw\include\c++\4.2.0
  INCZ+= -I$(WXWIN)/lib/gcc_dll/mswud 
  LIBP:= 
  LIBZ:= 
  LIBDL:= $(WXWIN)/lib/gcc_dll/wxbase28ud_gcc_arst.dll 
  # --enable-extra-pe-debug -mwindows
  LINKFLAGS:=  
else
  detest= if [ -d $(1) ]; then echo 1; else echo 0; fi
  fetest= if [ -e $(1) ]; then echo 1; else echo 0; fi
  # Projects location
  PROJ:= /proj
  SOSUFF:=.so
  SOPREF:=lib
  CPP:= g++
  CPPFLAGS:= 
  DEFS:= -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES 
  INCZ= 
  LIBZ:= 
  LIBDL:= -ldl 
  LINKFLAGS:= -pthread
endif

# make function to test for file/dir
DirExists= $(shell $(call detest,$(1)))
FileExists= $(shell $(call fetest,$(1)))

# Add shared settings
CPPFLAGS += -W -pipe
INCZ += -I$(CWD) -I$(PROJ)/home 

# The binary output directory
BINDIR= bin_$(PLATFORM)
ifneq ($(call DirExists,$(BINDIR)),1)
  $(shell mkdir $(BINDIR))
endif

# Initialize to nothing
OBJS := 
LOG := 

# This will add the MAIN target if not given already (and only one goal)
#ifeq ($(words $(MAKECMDGOALS)),1)
#  ifeq ($(findstring MAIN,$(MAKECMDGOALS)),)
#    MAKECMDGOALS += MAIN
#  endif
#endif
# $(warning goals: $(MAKECMDGOALS))

# Tests of above
#$(warning $(call DirExists,bu))
#$(warning $(call DirExists,bu12))
#$(warning DO: $(call FileExists,DynObj.cpp))
#$(warning DO1: $(call FileExists,DynObj1.cpp))
#$(warning $(if $(call DirExists,bu),it exists,nope no fir))

