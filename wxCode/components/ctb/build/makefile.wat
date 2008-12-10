# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.9 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================



# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------

# C++ compiler 
CXX = wpp386

# Standard flags for C++ 
CXXFLAGS = 

# Standard preprocessor flags (common for CC and CXX) 
CPPFLAGS = 

# Standard linker flags 
LDFLAGS = 

# Set to 1 to build debug version [0,1]
DEBUG = 0

#  
GPIB = 0

#  
WXDIR = $(%WXWIN)

#  
INSTALLDIR = $(%WXWIN)



# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

# Speed up compilation a bit:
!ifdef __LOADDLL__
!  loaddll wcc      wccd
!  loaddll wccaxp   wccdaxp
!  loaddll wcc386   wccd386
!  loaddll wpp      wppdi86
!  loaddll wppaxp   wppdaxp
!  loaddll wpp386   wppd386
!  loaddll wlink    wlink
!  loaddll wlib     wlibd
!endif

# We need these variables in some bakefile-made rules:
WATCOM_CWD = $+ $(%cdrive):$(%cwd) $-

### Conditionally set variables: ###

OUTPUT =
!ifeq DEBUG 0
OUTPUT = release
!endif
!ifeq DEBUG 1
OUTPUT = debug
!endif
INSTALLDIR =
!ifeq WXDIR
INSTALLDIR = ..\lib
!endif
LIBFLAG =
!ifeq DEBUG 1
!ifeq GPIB 0
LIBFLAG = d
!endif
!endif
!ifeq DEBUG 0
!ifeq GPIB 1
LIBFLAG = -gpib
!endif
!endif
!ifeq DEBUG 1
!ifeq GPIB 1
LIBFLAG = d-gpib
!endif
!endif
GPIBFLAG =
!ifeq GPIB 1
GPIBFLAG = _gpib
!endif
____GPIBSRC_FILENAMES_OBJECTS =
!ifeq GPIB 1
____GPIBSRC_FILENAMES_OBJECTS =  &
	$(OUTPUT)\wxctb_lib_gpib.obj
!endif
____GPIBSRC_FILENAMES_1_OBJECTS =
!ifeq GPIB 1
____GPIBSRC_FILENAMES_1_OBJECTS =  &
	$(OUTPUT)\wxctb_dll_gpib.obj
!endif
____DEBUG_20_21 =
!ifeq DEBUG 0
____DEBUG_20_21 = -ot -ox
!endif
!ifeq DEBUG 1
____DEBUG_20_21 = -od
!endif
__OPTIMIZE_FLAG =
!ifeq DEBUG 0
__OPTIMIZE_FLAG = -od
!endif
!ifeq DEBUG 1
__OPTIMIZE_FLAG = -ot -ox
!endif
____DEBUG =
!ifeq DEBUG 0
____DEBUG = -dNDEBUG
!endif
!ifeq DEBUG 1
____DEBUG = 
!endif
____DEBUG_0 =
!ifeq DEBUG 0
____DEBUG_0 = -d0
!endif
!ifeq DEBUG 1
____DEBUG_0 = -d2
!endif
____DEBUG_2 =
!ifeq DEBUG 0
____DEBUG_2 = 
!endif
!ifeq DEBUG 1
____DEBUG_2 = debug all
!endif
__SYSLIB2_p =
!ifeq GPIB 1
__SYSLIB2_p = gpib32.lib
!endif

### Variables: ###

WXCTB_LIB_CXXFLAGS = $(____DEBUG) $(____DEBUG_0) -bm $(__OPTIMIZE_FLAG) &
	-i=..\include $(CPPFLAGS) $(CXXFLAGS)
WXCTB_LIB_OBJECTS =  &
	$(OUTPUT)\wxctb_lib_fifo.obj &
	$(OUTPUT)\wxctb_lib_getopt.obj &
	$(OUTPUT)\wxctb_lib_iobase.obj &
	$(OUTPUT)\wxctb_lib_serport.obj &
	$(OUTPUT)\wxctb_lib_timer.obj &
	$(____GPIBSRC_FILENAMES_OBJECTS)
WXCTB_DLL_CXXFLAGS = -bd $(____DEBUG) $(____DEBUG_0) -bm $(__OPTIMIZE_FLAG) &
	-i=..\include $(CPPFLAGS) $(CXXFLAGS)
WXCTB_DLL_OBJECTS =  &
	$(OUTPUT)\wxctb_dll_fifo.obj &
	$(OUTPUT)\wxctb_dll_getopt.obj &
	$(OUTPUT)\wxctb_dll_iobase.obj &
	$(OUTPUT)\wxctb_dll_serport.obj &
	$(OUTPUT)\wxctb_dll_timer.obj &
	$(____GPIBSRC_FILENAMES_1_OBJECTS)
CTBTEST_CXXFLAGS = $(____DEBUG) $(____DEBUG_20_21) $(____DEBUG_0) &
	-dGPIB=$(GPIB) -i=..\include $(CPPFLAGS) $(CXXFLAGS)
CTBTEST_OBJECTS =  &
	$(OUTPUT)\ctbtest_ctbtest.obj


all : $(OUTPUT)
$(OUTPUT) :
	-if not exist $(OUTPUT) mkdir $(OUTPUT)

### Targets: ###

all : .SYMBOLIC ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.lib ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.dll $(OUTPUT)\ctbtest.exe tip-win32

clean : .SYMBOLIC 
	-if exist $(OUTPUT)\*.obj del $(OUTPUT)\*.obj
	-if exist $(OUTPUT)\*.res del $(OUTPUT)\*.res
	-if exist $(OUTPUT)\*.lbc del $(OUTPUT)\*.lbc
	-if exist $(OUTPUT)\*.ilk del $(OUTPUT)\*.ilk
	-if exist $(OUTPUT)\*.pch del $(OUTPUT)\*.pch
	-if exist ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.lib del ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.lib
	-if exist ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.dll del ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.dll
	-if exist ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.lib del ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.lib
	-if exist $(OUTPUT)\ctbtest.exe del $(OUTPUT)\ctbtest.exe

..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.lib :  $(WXCTB_LIB_OBJECTS)
	@%create $(OUTPUT)\wxctb_lib.lbc
	@for %i in ($(WXCTB_LIB_OBJECTS)) do @%append $(OUTPUT)\wxctb_lib.lbc +%i
	wlib -q -p4096 -n -b $^@ @$(OUTPUT)\wxctb_lib.lbc

..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.dll :  $(WXCTB_DLL_OBJECTS)
	@%create $(OUTPUT)\wxctb_dll.lbc
	@%append $(OUTPUT)\wxctb_dll.lbc option quiet
	@%append $(OUTPUT)\wxctb_dll.lbc name $^@
	@%append $(OUTPUT)\wxctb_dll.lbc option caseexact
	@%append $(OUTPUT)\wxctb_dll.lbc $(LDFLAGS) $(____DEBUG_2)  libpath ..\lib
	@for %i in ($(WXCTB_DLL_OBJECTS)) do @%append $(OUTPUT)\wxctb_dll.lbc file %i
	@for %i in ( winmm.lib $(__SYSLIB2_p)) do @%append $(OUTPUT)\wxctb_dll.lbc library %i
	@%append $(OUTPUT)\wxctb_dll.lbc
	@%append $(OUTPUT)\wxctb_dll.lbc system nt_dll
	wlink @$(OUTPUT)\wxctb_dll.lbc
	wlib -q -n -b ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.lib +$^@

$(OUTPUT)\ctbtest.exe :  $(CTBTEST_OBJECTS) ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.lib
	@%create $(OUTPUT)\ctbtest.lbc
	@%append $(OUTPUT)\ctbtest.lbc option quiet
	@%append $(OUTPUT)\ctbtest.lbc name $^@
	@%append $(OUTPUT)\ctbtest.lbc option caseexact
	@%append $(OUTPUT)\ctbtest.lbc $(LDFLAGS) $(____DEBUG_2) libpath ..\lib
	@for %i in ($(CTBTEST_OBJECTS)) do @%append $(OUTPUT)\ctbtest.lbc file %i
	@for %i in ( ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.lib winmm.lib $(__SYSLIB2_p)) do @%append $(OUTPUT)\ctbtest.lbc library %i
	@%append $(OUTPUT)\ctbtest.lbc
	wlink @$(OUTPUT)\ctbtest.lbc

wxinstall :  
	@copy ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.lib $(INSTALLDIR)\lib\watcom_lib
	@copy ..\lib\wxctb$(LIBFLAG)$(GPIBFLAG)-0.13.dll $(INSTALLDIR)\lib\watcom_lib
	
	@if not exist $(INSTALLDIR)\include\wx\ctb-0.13\win32 mkdir $(INSTALLDIR)\include\wx\ctb-0.13\win32
	@copy ..\include\wx\ctb-0.13\*.h $(INSTALLDIR)\include\wx\ctb-0.13
	@copy ..\include\wx\ctb-0.13\win32\*.h $(INSTALLDIR)\include\wx\ctb-0.13\win32

tip-win32 :  
	@echo "                                                                "
	@echo "================================================================"
	@echo " The building of wxctb is finished. If you have an installed    "
	@echo " wxWidget package (WXWIN must be defined in the enviroment),    "
	@echo " you'll now have to run:                                        "
	@echo "                                                                "
	@echo " make DEBUG=$(DEBUG) GPIB=$(GPIB) wxinstall                     "
	@echo "                                                                "
	@echo " to install the libraries in:                                   "
	@echo " $(INSTALLDIR)\lib\watcom_lib               "
	@echo " and the header files in"
	@echo " $(INSTALLDIR)\wx\ctb-0.13"
	@echo "                                                                "
	@echo " If you are using another compiler (Borland, Watcom, mingw,...) "
	@echo " take a look in the README in this directory!                   "
	@echo "                                                                "
	@echo " wxctb comes with no guarantees and doesn't claim               "
	@echo " to be suitable for any purpose.                                "
	@echo "================================================================"
	@echo "                                                                "

$(OUTPUT)\wxctb_lib_fifo.obj :  .AUTODEPEND .\..\src\fifo.cpp
	$(CXX) -zq -fo=$^@ $(WXCTB_LIB_CXXFLAGS) $<

$(OUTPUT)\wxctb_lib_getopt.obj :  .AUTODEPEND .\..\src\getopt.cpp
	$(CXX) -zq -fo=$^@ $(WXCTB_LIB_CXXFLAGS) $<

$(OUTPUT)\wxctb_lib_iobase.obj :  .AUTODEPEND .\..\src\iobase.cpp
	$(CXX) -zq -fo=$^@ $(WXCTB_LIB_CXXFLAGS) $<

$(OUTPUT)\wxctb_lib_serport.obj :  .AUTODEPEND .\..\src\win32\serport.cpp
	$(CXX) -zq -fo=$^@ $(WXCTB_LIB_CXXFLAGS) $<

$(OUTPUT)\wxctb_lib_timer.obj :  .AUTODEPEND .\..\src\win32\timer.cpp
	$(CXX) -zq -fo=$^@ $(WXCTB_LIB_CXXFLAGS) $<

$(OUTPUT)\wxctb_lib_gpib.obj :  .AUTODEPEND .\..\src\gpib.cpp
	$(CXX) -zq -fo=$^@ $(WXCTB_LIB_CXXFLAGS) $<

$(OUTPUT)\wxctb_dll_fifo.obj :  .AUTODEPEND .\..\src\fifo.cpp
	$(CXX) -zq -fo=$^@ $(WXCTB_DLL_CXXFLAGS) $<

$(OUTPUT)\wxctb_dll_getopt.obj :  .AUTODEPEND .\..\src\getopt.cpp
	$(CXX) -zq -fo=$^@ $(WXCTB_DLL_CXXFLAGS) $<

$(OUTPUT)\wxctb_dll_iobase.obj :  .AUTODEPEND .\..\src\iobase.cpp
	$(CXX) -zq -fo=$^@ $(WXCTB_DLL_CXXFLAGS) $<

$(OUTPUT)\wxctb_dll_serport.obj :  .AUTODEPEND .\..\src\win32\serport.cpp
	$(CXX) -zq -fo=$^@ $(WXCTB_DLL_CXXFLAGS) $<

$(OUTPUT)\wxctb_dll_timer.obj :  .AUTODEPEND .\..\src\win32\timer.cpp
	$(CXX) -zq -fo=$^@ $(WXCTB_DLL_CXXFLAGS) $<

$(OUTPUT)\wxctb_dll_gpib.obj :  .AUTODEPEND .\..\src\gpib.cpp
	$(CXX) -zq -fo=$^@ $(WXCTB_DLL_CXXFLAGS) $<

$(OUTPUT)\ctbtest_ctbtest.obj :  .AUTODEPEND .\..\samples\ctbtest.cpp
	$(CXX) -zq -fo=$^@ $(CTBTEST_CXXFLAGS) $<

