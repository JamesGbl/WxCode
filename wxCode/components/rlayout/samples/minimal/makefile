
# Shared settings
include ../../build/generic.mk

# Root for this project
PROJROOT:= $(CWD)/../..

# Generic build dir
GEN_BUILD_DIR:= $(PROJROOT)/build

ifeq ($(PLATFORM),win32)
  ifndef dlags
    flags:=du
  endif
  WXW:= C:/wxWidgets
  DEFS+= -D__WXMSW__ 
  INCZ+= -I$(WXW)/lib/gcc_dll/mswud
  LIBP+= $(WXW)/lib/gcc_dll
  LIBZ+= -L$(LIBP)  -L$(CWD)\lib_win32 
  LIBDL+= -lcomctl32 -lcomdlg32 \
    -lgdi32 -lole32 -loleaut32 -luuid -lkernel32 \
    $(LIBP)/wxbase28ud_gcc_arst.dll \
    $(LIBP)/wxmsw28ud_core_gcc_arst.dll \
    $(LIBP)/wxmsw28ud_xrc_gcc_arst.dll 
  LINKFLAGS+= --enable-extra-pe-debug -mconsole -mwindows 
else
  WXW:= /usr/src/wxCVS
  CPP:= g++
  CPPFLAGS:= 
  DEFS:= -D__WXGTK__ -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES 
  INCZ= -I$(WXW)/build-debug/lib/wx/include/gtk2-ansi-debug-2.9  
  LIBZ:= -L/usr/local/lib -L$(CWD)/lib-linux
  LIBDL:= -lwx_gtk2d_core-2.9 -lwx_gtk2d_xrc-2.9 -lwx_based-2.9 
  LINKFLAGS:= -pthread
endif

# Settings common to both Win32 & GTK
DEFS += -DWXUSINGDLL -DWXWIDGETS 
CPPFLAGS += -O0 
wxCPPFLAGS:= 
INCZ += -I$(WXW)/include -I$(PROJROOT)/include
INCZPCH:= $(INCZ)

# set default value for flags if not given on cmdline
ifndef flags
  flags := d
endif

# Modify above based on cmd line input flags
ifeq ($(findstring d,$(flags)),d)
  CPPFLAGS += -g -O0 
  DEFS += -D__WXDEBUG__ 
endif

ifeq ($(findstring u,$(flags)),u)
  DEFS += -DwxUSE_UNICODE  
endif
 

ifeq ($(findstring min, $(MAKECMDGOALS)),min)
  # Target executable
  .PHONY: OUT
  OUT= ../rl-minimal

  # No support for loading layouts - some source files can be removed
  DEFS += -DRL_NO_STATE 
  
  # Include other directories files 
  
  SUBDIR:= .
  SRCS := minimal.cpp
  include $(GEN_BUILD_DIR)/subdir.mk

  SUBDIR:= $(PROJROOT)/src/rl
  include $(SUBDIR)/dir.mk

  SUBDIR:= $(PROJROOT)/src/utils-wx
  SRCS := wxUtils.cpp SizerUtils.cpp 
  include $(GEN_BUILD_DIR)/subdir.mk

  # Targets
  
  # Resources
  ifeq ($(PLATFORM),win32)
    # Build resource file
    RESOURCE_OBJ:= $(BINDIR)/$(OUT)_resource.o
    $(RESOURCE_OBJ): $(OUT).rc
		$(RC) --use-temp-file -i$< -o$@ $(DEFS) $(INCZ) 
  endif
  
  # Executable
  $(OUT)$(EXE_SUFFIX): $(OBJS) makefile 
	$(CPP) -o $(OUT)$(EXE_SUFFIX) $(OBJS) $(RESORCE_OBJ) $(LINKFLAGS) $(LIBZ) $(LIBDL)

  min: $(OUT)$(EXE_SUFFIX)
endif



# How to build precompiled header
-include $(PCH).h.d
$(PCH).h.gch: wx_pch.h
	$(CPP) $(CPPFLAGS) $(DEFS) $(INCZ) $(wxCPPFLAGS) -MT$@ -MF$(PCH).h.d -MD -c $< -o $@ 

default: min
	    

clean:
	-rm $(BINDIR)/$(OUT)$(EXE_SUFFIX) $(PCH).h.gch $(OBJS) 
	-rm -f $(BINDIR)/*
	
print-%: ;	@$(error $* is $($*) ($(value $*)) (from $(origin $*)))

