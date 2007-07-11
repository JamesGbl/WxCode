# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.2 (http://www.bakefile.org)
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

# Use DLL build of wx library? [0,1]
#   0 - Static
#   1 - DLL
WX_SHARED = 0

# Use Unicode build of wxWidgets? [0,1]
#   0 - ANSI
#   1 - Unicode
WX_UNICODE = 0

# Use debug build of wxWidgets (define __WXDEBUG__)? [0,1]
#   0 - Release
#   1 - Debug
WX_DEBUG = 1

# Version of the wx library to build against. 
WX_VERSION = 28

# Use monolithic build of wxWidgets? [0,1]
#   0 - Multilib
#   1 - Monolithic
WX_MONOLITHIC = 0

# The directory where wxWidgets library is installed 
WX_DIR = $(%WXWIN)



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
_BUILDDIR_SHARED_SUFFIX =
!ifeq WX_SHARED 0
_BUILDDIR_SHARED_SUFFIX = 
!endif
!ifeq WX_SHARED 1
_BUILDDIR_SHARED_SUFFIX = _dll
!endif
__keybinder_lib___depname =
!ifeq WX_SHARED 0
__keybinder_lib___depname = &
	..\lib\wat_$(____keybinder_3)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_keybinder.lib
!endif
__keybinder_dll___depname =
!ifeq WX_SHARED 1
__keybinder_dll___depname = &
	..\lib\wat_$(____keybinder_3)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_keybinder.dll
!endif
VAR =
!ifeq WX_DEBUG 0
VAR = -ot -ox
!endif
!ifeq WX_DEBUG 1
VAR = -od
!endif
VAR_0 =
!ifeq WX_DEBUG 0
VAR_0 = -d0
!endif
!ifeq WX_DEBUG 1
VAR_0 = -d2
!endif
VAR_1 =
!ifeq WX_DEBUG 0
VAR_1 = 
!endif
!ifeq WX_DEBUG 1
VAR_1 = debug all
!endif
____keybinder_3 =
!ifeq WX_SHARED 0
____keybinder_3 = lib
!endif
!ifeq WX_SHARED 1
____keybinder_3 = dll
!endif
__WXLIB_CORE_NAME_p =
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 0
__WXLIB_CORE_NAME_p = wxmsw$(WX_VERSION)_core.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 1
__WXLIB_CORE_NAME_p = wxmsw$(WX_VERSION)u_core.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 0
__WXLIB_CORE_NAME_p = wxmsw$(WX_VERSION)d_core.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 1
__WXLIB_CORE_NAME_p = wxmsw$(WX_VERSION)ud_core.lib
!endif
!endif
!endif
__WXLIB_BASE_NAME_p =
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 0
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION).lib
!endif
!endif
!endif
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 1
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION)u.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 1
!ifeq WX_UNICODE 0
__WXLIB_BASE_NAME_p = wxmsw$(WX_VERSION).lib
!endif
!endif
!endif
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 1
!ifeq WX_UNICODE 1
__WXLIB_BASE_NAME_p = wxmsw$(WX_VERSION)u.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 0
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION)d.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 1
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION)ud.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 1
!ifeq WX_UNICODE 0
__WXLIB_BASE_NAME_p = wxmsw$(WX_VERSION)d.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 1
!ifeq WX_UNICODE 1
__WXLIB_BASE_NAME_p = wxmsw$(WX_VERSION)ud.lib
!endif
!endif
!endif
____WX_SHARED =
!ifeq WX_SHARED 0
____WX_SHARED = 
!endif
!ifeq WX_SHARED 1
____WX_SHARED = -dWXUSINGDLL
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
WXLIBPATH =
!ifeq WX_SHARED 0
WXLIBPATH = \lib\wat_lib
!endif
!ifeq WX_SHARED 1
WXLIBPATH = \lib\wat_dll
!endif

### Variables: ###

KEYBINDER_LIB_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_0) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
KEYBINDER_LIB_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_lib_menuutils.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_lib_keybinder.obj
KEYBINDER_DLL_CXXFLAGS = -bd $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_0) -wx -i=..\include -dWXMAKINGDLL_KEYBINDER $(CPPFLAGS) $(CXXFLAGS)
KEYBINDER_DLL_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll_menuutils.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll_keybinder.obj
MINIMAL_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_0) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
MINIMAL_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal_minimal.obj


all : watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)
watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX) :
	-if not exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX) mkdir watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)

### Targets: ###

all : .SYMBOLIC test_for_selected_wxbuild $(__keybinder_lib___depname) $(__keybinder_dll___depname) ..\sample\minimal.exe

clean : .SYMBOLIC 
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.obj del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.obj
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.res del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.res
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.lbc del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.lbc
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.ilk del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.ilk
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.pch del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.pch
	-if exist ..\lib\wat_$(____keybinder_3)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_keybinder.lib del ..\lib\wat_$(____keybinder_3)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_keybinder.lib
	-if exist ..\lib\wat_$(____keybinder_3)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_keybinder.dll del ..\lib\wat_$(____keybinder_3)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_keybinder.dll
	-if exist ..\lib\wat_$(____keybinder_3)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_keybinder.lib del ..\lib\wat_$(____keybinder_3)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_keybinder.lib
	-if exist ..\sample\minimal.exe del ..\sample\minimal.exe

test_for_selected_wxbuild :  
	@if not exist $(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX)\wx\setup.h \
	echo ----------------------------------------------------------------------------
	@if not exist $(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX)\wx\setup.h \
	echo The selected wxWidgets build is not available!
	@if not exist $(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX)\wx\setup.h \
	echo Please use the options prefixed with WX_ to select another wxWidgets build.
	@if not exist $(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX)\wx\setup.h \
	echo ----------------------------------------------------------------------------
	@if not exist $(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX)\wx\setup.h \
	exit 1

!ifeq WX_SHARED 0
..\lib\wat_$(____keybinder_3)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_keybinder.lib :  make_dir_keybinder_lib  $(KEYBINDER_LIB_OBJECTS)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_lib.lbc
	@for %i in ($(KEYBINDER_LIB_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_lib.lbc +%i
	wlib -q -p4096 -n -b $^@ @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_lib.lbc
!endif

make_dir_keybinder_lib :  
	if not exist ..\lib\wat_$(____keybinder_3) mkdir ..\lib\wat_$(____keybinder_3)

!ifeq WX_SHARED 1
..\lib\wat_$(____keybinder_3)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_keybinder.dll :  make_dir_keybinder_dll  $(KEYBINDER_DLL_OBJECTS)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll.lbc $(LDFLAGS) libpath $(WX_DIR)$(WXLIBPATH) $(VAR_1) libpath ..$(WXLIBPATH)
	@for %i in ($(KEYBINDER_DLL_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll.lbc file %i
	@for %i in ( $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll.lbc system nt_dll
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll.lbc
	wlib -q -n -b ..\lib\wat_$(____keybinder_3)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_keybinder.lib +$^@
!endif

make_dir_keybinder_dll :  
	if not exist ..\lib\wat_$(____keybinder_3) mkdir ..\lib\wat_$(____keybinder_3)

..\sample\minimal.exe :  $(MINIMAL_OBJECTS) make_sample_dir_minimal watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal_minimal.res $(__keybinder_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal.lbc $(LDFLAGS) libpath $(WX_DIR)$(WXLIBPATH) $(VAR_1) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'
	@for %i in ($(MINIMAL_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal.lbc file %i
	@for %i in ( ..\lib\wat_$(____keybinder_3)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_keybinder.lib $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal.lbc option resource=watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal_minimal.res
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal.lbc

make_sample_dir_minimal :  
	if not exist ..\sample mkdir ..\sample

tarball :  
	make distclean
	-cd ..\..
	-tar -cvzf keybinder.tar.gz --exclude="*~" --exclude="*.log" --exclude="*.o*" --exclude="*.a" --exclude=".svn" --exclude="autom4te.cache" keybinder\*

zip :  clean
	del ..\..\keybinder.zip
	-cd ..\..
	-zip -r9 keybinder.zip keybinder -x "*.pdb" -x "*.log" -x "*.o*"

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_lib_menuutils.obj :  .AUTODEPEND ..\src\menuutils.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(KEYBINDER_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_lib_keybinder.obj :  .AUTODEPEND ..\src\keybinder.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(KEYBINDER_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll_menuutils.obj :  .AUTODEPEND ..\src\menuutils.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(KEYBINDER_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\keybinder_dll_keybinder.obj :  .AUTODEPEND ..\src\keybinder.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(KEYBINDER_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal_minimal.obj :  .AUTODEPEND ..\sample\minimal.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MINIMAL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\minimal_minimal.res :  .AUTODEPEND ..\sample\minimal.rc
	wrc -q -ad -bt=nt -r -fo=$^@  $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=..\sample $<

