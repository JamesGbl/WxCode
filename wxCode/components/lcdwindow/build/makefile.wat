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

# The directory where wxWidgets library is installed 
WX_DIR = $(%WXWIN)

# Use DLL build of wx library to use? [0,1]
WX_SHARED = 0

# Compile Unicode build of wxWidgets? [0,1]
WX_UNICODE = 0

# Use debug build of wxWidgets (define __WXDEBUG__)? [0,1]
WX_DEBUG = 1

# Version of the wx library to build against. 
WX_VERSION = 26



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

WX3RDPARTYLIBPOSTFIX =
!ifeq WX_DEBUG 1
WX3RDPARTYLIBPOSTFIX = d
!endif
__lcdwindow_lib___depname =
!ifeq WX_SHARED 0
__lcdwindow_lib___depname = ..\lib\lcdwindow$(WXLIBPOSTFIX).lib
!endif
__lcdwindow_dll___depname =
!ifeq WX_SHARED 1
__lcdwindow_dll___depname = ..\lib\lcdwindow$(WXLIBPOSTFIX).dll
!endif
__WARNINGS =
!ifeq WX_DEBUG 0
__WARNINGS = 
!endif
!ifeq WX_DEBUG 1
__WARNINGS = -wx
!endif
__OPTIMIZEFLAG =
!ifeq WX_DEBUG 0
__OPTIMIZEFLAG = -ot -ox
!endif
!ifeq WX_DEBUG 1
__OPTIMIZEFLAG = -od
!endif
__DEBUGINFO =
!ifeq WX_DEBUG 0
__DEBUGINFO = -d0
!endif
!ifeq WX_DEBUG 1
__DEBUGINFO = -d2
!endif
__DEBUGINFO_1 =
!ifeq WX_DEBUG 0
__DEBUGINFO_1 = 
!endif
!ifeq WX_DEBUG 1
__DEBUGINFO_1 = debug all
!endif
__WX_SHAREDDEFINE_p =
!ifeq WX_SHARED 1
__WX_SHAREDDEFINE_p = -dWXUSINGDLL
!endif
__WXUNICODE_DEFINE_p =
!ifeq WX_UNICODE 1
__WXUNICODE_DEFINE_p = -d_UNICODE
!endif
__WXDEBUG_DEFINE_p =
!ifeq WX_DEBUG 1
__WXDEBUG_DEFINE_p = -d__WXDEBUG__
!endif
WXLIBPOSTFIX =
!ifeq WX_DEBUG 0
!ifeq WX_UNICODE 1
WXLIBPOSTFIX = u
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_UNICODE 0
WXLIBPOSTFIX = d
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_UNICODE 1
WXLIBPOSTFIX = ud
!endif
!endif
__WXLIBPATH_FILENAMES =
!ifeq WX_SHARED 0
__WXLIBPATH_FILENAMES = \lib\wat_lib
!endif
!ifeq WX_SHARED 1
__WXLIBPATH_FILENAMES = \lib\wat_dll
!endif

### Variables: ###

LCDWINDOW_LIB_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include $(CPPFLAGS) $(CXXFLAGS)
LCDWINDOW_LIB_OBJECTS =  &
	watcom\lcdwindow_lib_lcdwindow.obj
LCDWINDOW_DLL_CXXFLAGS = -bd $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include -dWXMAKINGDLL_LCDWINDOW $(CPPFLAGS) &
	$(CXXFLAGS)
LCDWINDOW_DLL_OBJECTS =  &
	watcom\lcdwindow_dll_lcdwindow.obj
MINIMAL_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include $(CPPFLAGS) $(CXXFLAGS)
MINIMAL_OBJECTS =  &
	watcom\minimal_lcdsampleapp.obj &
	watcom\minimal_lcdsampleframe.obj &
	watcom\minimal_clockthread.obj


all : watcom
watcom :
	-if not exist watcom mkdir watcom

### Targets: ###

all : .SYMBOLIC $(__lcdwindow_lib___depname) $(__lcdwindow_dll___depname) ..\samples\minimal.exe

clean : .SYMBOLIC 
	-if exist watcom\*.obj del watcom\*.obj
	-if exist watcom\*.res del watcom\*.res
	-if exist watcom\*.lbc del watcom\*.lbc
	-if exist watcom\*.ilk del watcom\*.ilk
	-if exist watcom\*.pch del watcom\*.pch
	-if exist ..\lib\lcdwindow$(WXLIBPOSTFIX).lib del ..\lib\lcdwindow$(WXLIBPOSTFIX).lib
	-if exist ..\lib\lcdwindow$(WXLIBPOSTFIX).dll del ..\lib\lcdwindow$(WXLIBPOSTFIX).dll
	-if exist ..\lib\lcdwindow$(WXLIBPOSTFIX).lib del ..\lib\lcdwindow$(WXLIBPOSTFIX).lib
	-if exist ..\samples\minimal.exe del ..\samples\minimal.exe

tarball :  
	-cd ..
	-tar -cvzf ..\lcdwindow.tar.gz --exclude=*.pdb --exclude=*.log --exclude=*.o* *

zip :  
	-cd ..
	-zip -r9 ..\lcdwindow.zip *  -x *.pdb -x *.log -x *.o*

deepclean :  
	-del /S ..\*.a
	-del /S ..\*.lib
	-del /S ..\*.pdb
	-del /S ..\*.dll
	-del /S ..\*.exp
	-del /S ..\*.so*
	-del /S ..\*.exe
	-del /S ..\*.obj
	-del /S ..\*.o
	-del /S ..\*.log
	-del /S ..\*.manifest*
	-del /S ..\*.log
	-del /S ..\.bakefile_gen.state
	-del /S ..\*.pch
	-del /S ..\*.ncb
	-del /S ..\*.plg
	-del /S ..\*.ncb
	-del /S ..\*.aps
	-del /S ..\*.suo
	-del /S ..\*.user
	-del /S ..\*.res
	-del /S ..\*.il?
	-del /S ..\*.tds
	-del /S ..\*.idb
	-del /S ..\*.map
	-if exist ..\autom4te.cache rmdir /S /Q ..\autom4te.cache
	-if exist ..\.deps rmdir /S /Q ..\.deps
	-del /S ..\config.status
	-del /S ..\config.log
	-del /S ..\Makefile
	-del /S ..\bk-deps
	-del /S .\*.a
	-del /S .\*.lib
	-del /S .\*.pdb
	-del /S .\*.dll
	-del /S .\*.exp
	-del /S .\*.so*
	-del /S .\*.exe
	-del /S .\*.obj
	-del /S .\*.o
	-del /S .\*.log
	-del /S .\*.manifest*
	-del /S .\*.log
	-del /S .\.bakefile_gen.state
	-del /S .\*.pch
	-del /S .\*.ncb
	-del /S .\*.plg
	-del /S .\*.ncb
	-del /S .\*.aps
	-del /S .\*.suo
	-del /S .\*.user
	-del /S .\*.res
	-del /S .\*.il?
	-del /S .\*.tds
	-del /S .\*.idb
	-del /S .\*.map
	-if exist .\autom4te.cache rmdir /S /Q .\autom4te.cache
	-if exist .\.deps rmdir /S /Q .\.deps
	-del /S .\config.status
	-del /S .\config.log
	-del /S .\Makefile
	-del /S .\bk-deps

docs :  
	-cd ..\docs
	-doxygen

cleandocs :  
	-if exist ..\docs\html rmdir /S /Q ..\docs\html

!ifeq WX_SHARED 0
..\lib\lcdwindow$(WXLIBPOSTFIX).lib :  $(LCDWINDOW_LIB_OBJECTS)
	@%create watcom\lcdwindow_lib.lbc
	@for %i in ($(LCDWINDOW_LIB_OBJECTS)) do @%append watcom\lcdwindow_lib.lbc +%i
	wlib -q -p4096 -n -b $^@ @watcom\lcdwindow_lib.lbc
!endif

!ifeq WX_SHARED 1
..\lib\lcdwindow$(WXLIBPOSTFIX).dll :  $(LCDWINDOW_DLL_OBJECTS)
	@%create watcom\lcdwindow_dll.lbc
	@%append watcom\lcdwindow_dll.lbc option quiet
	@%append watcom\lcdwindow_dll.lbc name $^@
	@%append watcom\lcdwindow_dll.lbc option caseexact
	@%append watcom\lcdwindow_dll.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(WX_DIR)$(__WXLIBPATH_FILENAMES) libpath ..\lib
	@for %i in ($(LCDWINDOW_DLL_OBJECTS)) do @%append watcom\lcdwindow_dll.lbc file %i
	@for %i in ( wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\lcdwindow_dll.lbc library %i
	@%append watcom\lcdwindow_dll.lbc
	@%append watcom\lcdwindow_dll.lbc system nr_dll
	wlink @watcom\lcdwindow_dll.lbc
	wlib -q -n -b ..\lib\lcdwindow$(WXLIBPOSTFIX).lib +$^@
!endif

..\samples\minimal.exe :  $(MINIMAL_OBJECTS) $(__lcdwindow_lib___depname)
	@%create watcom\minimal.lbc
	@%append watcom\minimal.lbc option quiet
	@%append watcom\minimal.lbc name $^@
	@%append watcom\minimal.lbc option caseexact
	@%append watcom\minimal.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(WX_DIR)$(__WXLIBPATH_FILENAMES) libpath ..\lib system nt_win ref '_WinMain@16'
	@for %i in ($(MINIMAL_OBJECTS)) do @%append watcom\minimal.lbc file %i
	@for %i in ( ..\lib\lcdwindow$(WXLIBPOSTFIX).lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\minimal.lbc library %i
	@%append watcom\minimal.lbc
	wlink @watcom\minimal.lbc

watcom\lcdwindow_lib_lcdwindow.obj :  .AUTODEPEND .\..\src\lcdwindow.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(LCDWINDOW_LIB_CXXFLAGS) $<

watcom\lcdwindow_dll_lcdwindow.obj :  .AUTODEPEND .\..\src\lcdwindow.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(LCDWINDOW_DLL_CXXFLAGS) $<

watcom\minimal_lcdsampleapp.obj :  .AUTODEPEND .\..\samples\lcdsampleapp.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MINIMAL_CXXFLAGS) $<

watcom\minimal_lcdsampleframe.obj :  .AUTODEPEND .\..\samples\lcdsampleframe.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MINIMAL_CXXFLAGS) $<

watcom\minimal_clockthread.obj :  .AUTODEPEND .\..\samples\clockthread.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MINIMAL_CXXFLAGS) $<

