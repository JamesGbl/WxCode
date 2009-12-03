# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.8 (http://www.bakefile.org)
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

# Build this wxCode component as DLL or as static library? [0,1]
#   0 - Static
#   1 - DLL
SHARED = 0



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
! if $(__VERSION__) >= 1280
!  loaddll wlink    wlinkd
! else
!  loaddll wlink    wlink
! endif
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
!ifeq SHARED 0
_BUILDDIR_SHARED_SUFFIX = 
!endif
!ifeq SHARED 1
_BUILDDIR_SHARED_SUFFIX = _dll
!endif
__wxadvtable_lib___depname =
!ifeq WX_SHARED 0
__wxadvtable_lib___depname = &
	..\lib\wat_$(____wxadvtable_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.lib
!endif
__wxadvtable_dll___depname =
!ifeq WX_SHARED 1
__wxadvtable_dll___depname = &
	..\lib\wat_$(____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.dll
!endif
__wxadvtable_sample___depname =
!ifeq WX_SHARED 0
__wxadvtable_sample___depname = ..\samples\wxadvtable_sample.exe
!endif
__wxadvtable_sample_dll___depname =
!ifeq WX_SHARED 1
__wxadvtable_sample_dll___depname = ..\samples\wxadvtable_sample_dll.exe
!endif
__builder___depname =
!ifeq WX_SHARED 0
__builder___depname = ..\builder\builder.exe
!endif
__builder_dll___depname =
!ifeq WX_SHARED 1
__builder_dll___depname = ..\builder\builder_dll.exe
!endif
____wxadvtable_lib__DIRNAME_SHARED_SUFFIX_FILENAMES =
!ifeq SHARED 0
____wxadvtable_lib__DIRNAME_SHARED_SUFFIX_FILENAMES = lib
!endif
!ifeq SHARED 1
____wxadvtable_lib__DIRNAME_SHARED_SUFFIX_FILENAMES = dll
!endif
____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES =
!ifeq WX_SHARED 0
____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES = lib
!endif
!ifeq WX_SHARED 1
____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES = dll
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
VAR =
!ifeq WX_DEBUG 0
VAR = -ot -ox
!endif
!ifeq WX_DEBUG 1
VAR = -od
!endif
VAR_2 =
!ifeq WX_DEBUG 0
VAR_2 = -d0
!endif
!ifeq WX_DEBUG 1
VAR_2 = -d2
!endif
VAR_3 =
!ifeq WX_DEBUG 0
VAR_3 = 
!endif
!ifeq WX_DEBUG 1
VAR_3 = debug all
!endif
__WXLIB_ADV_NAME_p =
!ifeq WX_MONOLITHIC 0
__WXLIB_ADV_NAME_p = wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_adv.lib
!endif
__WXLIB_AUI_NAME_p =
!ifeq WX_MONOLITHIC 0
__WXLIB_AUI_NAME_p = wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_aui.lib
!endif
__WXLIB_CORE_NAME_p =
!ifeq WX_MONOLITHIC 0
__WXLIB_CORE_NAME_p = wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib
!endif
__WXLIB_BASE_NAME_p =
!ifeq WX_MONOLITHIC 0
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib
!endif
!ifeq WX_MONOLITHIC 1
__WXLIB_BASE_NAME_p = wxmsw$(WX_VERSION)$(WXLIBPOSTFIX).lib
!endif
WXLIBPATH =
!ifeq WX_SHARED 0
WXLIBPATH = \lib\wat_lib
!endif
!ifeq WX_SHARED 1
WXLIBPATH = \lib\wat_dll
!endif

### Variables: ###

WXADVTABLE_LIB_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_2) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
WXADVTABLE_LIB_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_lib_wxadvtable.obj
WXADVTABLE_DLL_CXXFLAGS = -bd $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_2) -wx -i=..\include -dWXMAKINGDLL_ADVTABLE $(CPPFLAGS) $(CXXFLAGS)
WXADVTABLE_DLL_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_dll_wxadvtable.obj
WXADVTABLE_SAMPLE_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_2) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
WXADVTABLE_SAMPLE_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_demo.obj
WXADVTABLE_SAMPLE_DLL_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_2) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
WXADVTABLE_SAMPLE_DLL_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_dll_demo.obj
BUILDER_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_2) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
BUILDER_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_builder.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_properties.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_propertypage.obj
BUILDER_DLL_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_2) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
BUILDER_DLL_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll_builder.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll_properties.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll_propertypage.obj


all : watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)
watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX) :
	-if not exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX) mkdir watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)

### Targets: ###

all : .SYMBOLIC test_for_selected_wxbuild $(__wxadvtable_lib___depname) $(__wxadvtable_dll___depname) $(__wxadvtable_sample___depname) $(__wxadvtable_sample_dll___depname) $(__builder___depname) $(__builder_dll___depname)

clean : .SYMBOLIC 
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.obj del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.obj
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.res del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.res
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.lbc del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.lbc
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.ilk del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.ilk
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.pch del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.pch
	-if exist ..\lib\wat_$(____wxadvtable_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.lib del ..\lib\wat_$(____wxadvtable_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.lib
	-if exist ..\lib\wat_$(____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.dll del ..\lib\wat_$(____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.dll
	-if exist ..\lib\wat_$(____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.lib del ..\lib\wat_$(____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.lib
	-if exist ..\samples\wxadvtable_sample.exe del ..\samples\wxadvtable_sample.exe
	-if exist ..\samples\wxadvtable_sample_dll.exe del ..\samples\wxadvtable_sample_dll.exe
	-if exist ..\builder\builder.exe del ..\builder\builder.exe
	-if exist ..\builder\builder_dll.exe del ..\builder\builder_dll.exe

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
..\lib\wat_$(____wxadvtable_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.lib :  make_dir_wxadvtable_lib  $(WXADVTABLE_LIB_OBJECTS)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_lib.lbc
	@for %i in ($(WXADVTABLE_LIB_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_lib.lbc +%i
	wlib -q -p4096 -n -b $^@ @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_lib.lbc
!endif

make_dir_wxadvtable_lib :  
	if not exist ..\lib\wat_$(____wxadvtable_lib__DIRNAME_SHARED_SUFFIX_FILENAMES) mkdir ..\lib\wat_$(____wxadvtable_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)

!ifeq WX_SHARED 1
..\lib\wat_$(____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.dll :  make_dir_wxadvtable_dll  $(WXADVTABLE_DLL_OBJECTS)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_dll.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_dll.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_dll.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_dll.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(VAR_3) libpath ..$(WXLIBPATH) $(LDFLAGS)
	@for %i in ($(WXADVTABLE_DLL_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_dll.lbc file %i
	@for %i in ( $(__WXLIB_ADV_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_dll.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_dll.lbc system nt_dll
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_dll.lbc
	wlib -q -n -b ..\lib\wat_$(____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.lib +$^@
!endif

make_dir_wxadvtable_dll :  
	if not exist ..\lib\wat_$(____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES) mkdir ..\lib\wat_$(____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)

!ifeq WX_SHARED 0
..\samples\wxadvtable_sample.exe :  $(WXADVTABLE_SAMPLE_OBJECTS) make_sample_dir_wxadvtable_sample  $(__wxadvtable_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(VAR_3) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'  $(LDFLAGS)
	@for %i in ($(WXADVTABLE_SAMPLE_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample.lbc file %i
	@for %i in ( ..\lib\wat_$(____wxadvtable_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.lib $(__WXLIB_ADV_NAME_p) $(__WXLIB_AUI_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample.lbc option resource=
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample.lbc
!endif

make_sample_dir_wxadvtable_sample :  
	if not exist ..\samples mkdir ..\samples

!ifeq WX_SHARED 1
..\samples\wxadvtable_sample_dll.exe :  $(WXADVTABLE_SAMPLE_DLL_OBJECTS) make_sample_dir_wxadvtable_sample_dll  $(__wxadvtable_dll___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_dll.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_dll.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_dll.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_dll.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(VAR_3) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'  $(LDFLAGS)
	@for %i in ($(WXADVTABLE_SAMPLE_DLL_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_dll.lbc file %i
	@for %i in ( ..\lib\wat_$(____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.lib $(__WXLIB_ADV_NAME_p) $(__WXLIB_AUI_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_dll.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_dll.lbc option resource=
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_dll.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_dll.lbc
!endif

make_sample_dir_wxadvtable_sample_dll :  
	if not exist ..\samples mkdir ..\samples

!ifeq WX_SHARED 0
..\builder\builder.exe :  $(BUILDER_OBJECTS) make_sample_dir_builder  $(__wxadvtable_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(VAR_3) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'  $(LDFLAGS)
	@for %i in ($(BUILDER_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder.lbc file %i
	@for %i in ( ..\lib\wat_$(____wxadvtable_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.lib $(__WXLIB_ADV_NAME_p) $(__WXLIB_AUI_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder.lbc option resource=
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder.lbc
!endif

make_sample_dir_builder :  
	if not exist ..\builder mkdir ..\builder

!ifeq WX_SHARED 1
..\builder\builder_dll.exe :  $(BUILDER_DLL_OBJECTS) make_sample_dir_builder_dll  $(__wxadvtable_dll___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(VAR_3) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'  $(LDFLAGS)
	@for %i in ($(BUILDER_DLL_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll.lbc file %i
	@for %i in ( ..\lib\wat_$(____wxadvtable_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_advtable.lib $(__WXLIB_ADV_NAME_p) $(__WXLIB_AUI_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll.lbc option resource=
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll.lbc
!endif

make_sample_dir_builder_dll :  
	if not exist ..\builder mkdir ..\builder

tarball :  
	make distclean
	-cd ..\..
	-tar -cvzf advtable.tar.gz --exclude="*~" --exclude="*.log" --exclude="*.o*" --exclude="*.a" --exclude=".svn" --exclude="autom4te.cache" advtable\*

zip :  clean
	del ..\..\advtable.zip
	-cd ..\..
	-zip -r9 advtable.zip advtable -x "*.pdb" -x "*.log" -x "*.o*"

docs :  
	-cd ..\docs
	-doxygen

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_lib_wxadvtable.obj :  .AUTODEPEND ..\src\wxadvtable.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXADVTABLE_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_dll_wxadvtable.obj :  .AUTODEPEND ..\src\wxadvtable.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXADVTABLE_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_demo.obj :  .AUTODEPEND ..\samples\demo.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXADVTABLE_SAMPLE_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxadvtable_sample_dll_demo.obj :  .AUTODEPEND ..\samples\demo.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXADVTABLE_SAMPLE_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_builder.obj :  .AUTODEPEND ..\builder\builder.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(BUILDER_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_properties.obj :  .AUTODEPEND ..\builder\properties.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(BUILDER_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_propertypage.obj :  .AUTODEPEND ..\builder\propertypage.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(BUILDER_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll_builder.obj :  .AUTODEPEND ..\builder\builder.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(BUILDER_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll_properties.obj :  .AUTODEPEND ..\builder\properties.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(BUILDER_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\builder_dll_propertypage.obj :  .AUTODEPEND ..\builder\propertypage.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(BUILDER_DLL_CXXFLAGS) $<

