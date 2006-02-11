# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.0 (http://bakefile.sourceforge.net)
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
#   1 - DLL
WX_SHARED = 0

# Compile Unicode build of wxWidgets? [0,1]
#   1 - Unicode
WX_UNICODE = 0

# Use debug build of wxWidgets (define __WXDEBUG__)? [0,1]
#   0 - Release
#   1 - Debug
WX_DEBUG = 1

# Version of the wx library to build against. 
WX_VERSION = 26

# If 1 then wxHTTPEngine component will be used for proxy/authentication stuff [1,0]
USE_HTTPENGINE = 1

# The path to the wxHTTPBuilder (httpengine) component 
HTTPENGINE_DIR = ..\..\wxhttpengine



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
__webupdate_lib___depname =
!ifeq WX_SHARED 0
__webupdate_lib___depname = ..\lib\webupdate$(WXLIBPOSTFIX).lib
!endif
__webupdate_dll___depname =
!ifeq WX_SHARED 1
__webupdate_dll___depname = ..\lib\webupdate$(WXLIBPOSTFIX).dll
!endif
__MYOPTIMIZEFLAG =
!ifeq WX_DEBUG 0
__MYOPTIMIZEFLAG = -os
!endif
!ifeq WX_DEBUG 1
__MYOPTIMIZEFLAG = -od
!endif
__HTTPENGINEDEP_LIB_p =
!ifeq USE_HTTPENGINE 1
__HTTPENGINEDEP_LIB_p = httpengine$(WXLIBPOSTFIX).lib
!endif
__HTTPENGINEDEP_DEF_p =
!ifeq USE_HTTPENGINE 0
__HTTPENGINEDEP_DEF_p = -dwxUSE_HTTPENGINE=0
!endif
!ifeq USE_HTTPENGINE 1
__HTTPENGINEDEP_DEF_p = -dwxUSE_HTTPENGINE=1
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
__DEBUGINFO_4 =
!ifeq WX_DEBUG 0
__DEBUGINFO_4 = 
!endif
!ifeq WX_DEBUG 1
__DEBUGINFO_4 = debug all
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
WXLIBPATH =
!ifeq WX_SHARED 0
WXLIBPATH = \lib\wat_lib
!endif
!ifeq WX_SHARED 1
WXLIBPATH = \lib\wat_dll
!endif

### Variables: ###

WEBUPDATE_LIB_CXXFLAGS = $(__WARNINGS) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include -i=$(HTTPENGINE_DIR)\include &
	$(__HTTPENGINEDEP_DEF_p) $(__MYOPTIMIZEFLAG) $(CPPFLAGS) $(CXXFLAGS)
WEBUPDATE_LIB_OBJECTS =  &
	watcom\webupdate_lib_webupdate.obj &
	watcom\webupdate_lib_webupdatedlg.obj &
	watcom\webupdate_lib_checkedlistctrl.obj &
	watcom\webupdate_lib_md5.obj &
	watcom\webupdate_lib_installer.obj &
	watcom\webupdate_lib_download.obj &
	watcom\webupdate_lib_stdactions.obj &
	watcom\webupdate_lib_webupdatectrl.obj
WEBUPDATE_DLL_CXXFLAGS = -bd $(__WARNINGS) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include -dWXMAKINGDLL_WEBUPDATE &
	-i=$(HTTPENGINE_DIR)\include $(__HTTPENGINEDEP_DEF_p) $(__MYOPTIMIZEFLAG) &
	$(CPPFLAGS) $(CXXFLAGS)
WEBUPDATE_DLL_OBJECTS =  &
	watcom\webupdate_dll_webupdate.obj &
	watcom\webupdate_dll_webupdatedlg.obj &
	watcom\webupdate_dll_checkedlistctrl.obj &
	watcom\webupdate_dll_md5.obj &
	watcom\webupdate_dll_installer.obj &
	watcom\webupdate_dll_download.obj &
	watcom\webupdate_dll_stdactions.obj &
	watcom\webupdate_dll_webupdatectrl.obj
WEBUPDATER_CXXFLAGS = $(__WARNINGS) $(__DEBUGINFO) -bm $(__WX_SHAREDDEFINE_p) &
	$(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	-i=..\include -i=$(HTTPENGINE_DIR)\include $(__HTTPENGINEDEP_DEF_p) &
	$(__MYOPTIMIZEFLAG) $(CPPFLAGS) $(CXXFLAGS)
WEBUPDATER_OBJECTS =  &
	watcom\webupdater_webapp.obj
SIMPLE_1_0_0_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include $(CPPFLAGS) $(CXXFLAGS)
SIMPLE_1_0_0_OBJECTS =  &
	watcom\simple_1_0_0_minimal.obj
SIMPLE_2_0_3_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include $(CPPFLAGS) $(CXXFLAGS)
SIMPLE_2_0_3_OBJECTS =  &
	watcom\simple_2_0_3_minimal.obj
ADVANCED_0_0_1_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include $(CPPFLAGS) $(CXXFLAGS)
ADVANCED_0_0_1_OBJECTS =  &
	watcom\advanced_0_0_1_minimal.obj
ADVANCED_1_5_0_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include $(CPPFLAGS) $(CXXFLAGS)
ADVANCED_1_5_0_OBJECTS =  &
	watcom\advanced_1_5_0_minimal.obj


all : watcom
watcom :
	-if not exist watcom mkdir watcom

### Targets: ###

all : .SYMBOLIC $(__webupdate_lib___depname) $(__webupdate_dll___depname) ..\src\webupdater.exe rep4_simple100 rep4_simple203 rep4_adv001 rep4_adv150 ..\samples\simple\v1.0.0\simple.exe ..\samples\simple\v2.0.3\simple.exe ..\samples\advanced\v0.0.1\advanced.exe ..\samples\advanced\v1.5.0\advanced.exe

clean : .SYMBOLIC 
	-if exist watcom\*.obj del watcom\*.obj
	-if exist watcom\*.res del watcom\*.res
	-if exist watcom\*.lbc del watcom\*.lbc
	-if exist watcom\*.ilk del watcom\*.ilk
	-if exist watcom\*.pch del watcom\*.pch
	-if exist ..\lib\webupdate$(WXLIBPOSTFIX).lib del ..\lib\webupdate$(WXLIBPOSTFIX).lib
	-if exist ..\lib\webupdate$(WXLIBPOSTFIX).dll del ..\lib\webupdate$(WXLIBPOSTFIX).dll
	-if exist ..\lib\webupdate$(WXLIBPOSTFIX).lib del ..\lib\webupdate$(WXLIBPOSTFIX).lib
	-if exist ..\src\webupdater.exe del ..\src\webupdater.exe
	-if exist webupdater.exe del webupdater.exe
	-if exist webupdatedlg.xrc del webupdatedlg.xrc
	-if exist webupdater.exe del webupdater.exe
	-if exist webupdatedlg.xrc del webupdatedlg.xrc
	-if exist webupdater.exe del webupdater.exe
	-if exist webupdatedlg.xrc del webupdatedlg.xrc
	-if exist webupdater.exe del webupdater.exe
	-if exist webupdatedlg.xrc del webupdatedlg.xrc
	-if exist ..\samples\simple\v1.0.0\simple.exe del ..\samples\simple\v1.0.0\simple.exe
	-if exist ..\samples\simple\v2.0.3\simple.exe del ..\samples\simple\v2.0.3\simple.exe
	-if exist ..\samples\advanced\v0.0.1\advanced.exe del ..\samples\advanced\v0.0.1\advanced.exe
	-if exist ..\samples\advanced\v1.5.0\advanced.exe del ..\samples\advanced\v1.5.0\advanced.exe

tarball :  
	-cd ..
	-tar -cvzf ..\webupdate.tar.gz --exclude=*.pdb --exclude=*.log --exclude=*.o* *

zip :  
	-cd ..
	-zip -r9 ..\webupdate.zip *  -x *.pdb -x *.log -x *.o*

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

install :  all install-wxheaders
	copy /Y ..\lib\webupdate$(WXLIBPOSTFIX).lib $(WX_DIR)$(WXLIBPATH)
	copy /Y ..\lib\webupdate$(WXLIBPOSTFIX).dll $(WX_DIR)$(WXLIBPATH)

install-wxheaders :  
	mkdir $(WX_DIR)\include\wx
	cd .
	copy /Y  ..\include\wx\*.h $(WX_DIR)\include\wx

uninstall :  
	-if exist $(WX_DIR)\include\wx\webupdatedef.h \
	$(WX_DIR)\include\wx\webupdate.h \
	$(WX_DIR)\include\wx\webupdatedlg.h \
	$(WX_DIR)\include\wx\checkedlistctrl.h \
	$(WX_DIR)\include\wx\md5.h \
	$(WX_DIR)\include\wx\installer.h \
	$(WX_DIR)\include\wx\download.h \
	$(WX_DIR)\include\wx\stdactions.h \
	$(WX_DIR)\include\wx\webupdatectrl.h del /Q $(WX_DIR)\include\wx\webupdatedef.h \
	$(WX_DIR)\include\wx\webupdate.h \
	$(WX_DIR)\include\wx\webupdatedlg.h \
	$(WX_DIR)\include\wx\checkedlistctrl.h \
	$(WX_DIR)\include\wx\md5.h \
	$(WX_DIR)\include\wx\installer.h \
	$(WX_DIR)\include\wx\download.h \
	$(WX_DIR)\include\wx\stdactions.h \
	$(WX_DIR)\include\wx\webupdatectrl.h
	-if exist $(WX_DIR)$(WXLIBPATH)\*webupdate* del /Q $(WX_DIR)$(WXLIBPATH)\*webupdate*

!ifeq WX_SHARED 0
..\lib\webupdate$(WXLIBPOSTFIX).lib :  $(WEBUPDATE_LIB_OBJECTS)
	@%create watcom\webupdate_lib.lbc
	@for %i in ($(WEBUPDATE_LIB_OBJECTS)) do @%append watcom\webupdate_lib.lbc +%i
	wlib -q -p4096 -n -b $^@ @watcom\webupdate_lib.lbc
!endif

!ifeq WX_SHARED 1
..\lib\webupdate$(WXLIBPOSTFIX).dll :  $(WEBUPDATE_DLL_OBJECTS)
	@%create watcom\webupdate_dll.lbc
	@%append watcom\webupdate_dll.lbc option quiet
	@%append watcom\webupdate_dll.lbc name $^@
	@%append watcom\webupdate_dll.lbc option caseexact
	@%append watcom\webupdate_dll.lbc $(LDFLAGS) $(__DEBUGINFO_4)  libpath $(WX_DIR)$(WXLIBPATH) libpath ..\lib libpath $(HTTPENGINE_DIR)\lib
	@for %i in ($(WEBUPDATE_DLL_OBJECTS)) do @%append watcom\webupdate_dll.lbc file %i
	@for %i in ( $(__HTTPENGINEDEP_LIB_p) wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_adv.lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_html.lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_xrc.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX)_xml.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX)_net.lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\webupdate_dll.lbc library %i
	@%append watcom\webupdate_dll.lbc
	@%append watcom\webupdate_dll.lbc system nt_dll
	wlink @watcom\webupdate_dll.lbc
	wlib -q -n -b ..\lib\webupdate$(WXLIBPOSTFIX).lib +$^@
!endif

xrcscan :  
	wxrc ../src/webupdatedlg.xrc -g -c -o ../src/webupdatedlg.xrc.extracted

..\src\webupdater.exe :  $(WEBUPDATER_OBJECTS) locale watcom\webupdater_app.res $(__webupdate_lib___depname)
	@%create watcom\webupdater.lbc
	@%append watcom\webupdater.lbc option quiet
	@%append watcom\webupdater.lbc name $^@
	@%append watcom\webupdater.lbc option caseexact
	@%append watcom\webupdater.lbc $(LDFLAGS) $(__DEBUGINFO_4)  libpath $(WX_DIR)$(WXLIBPATH) libpath ..\lib libpath $(HTTPENGINE_DIR)\lib system nt_win ref '_WinMain@16'
	@for %i in ($(WEBUPDATER_OBJECTS)) do @%append watcom\webupdater.lbc file %i
	@for %i in ( ..\lib\webupdate$(WXLIBPOSTFIX).lib $(__HTTPENGINEDEP_LIB_p) wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_adv.lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_html.lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_xrc.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX)_xml.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX)_net.lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\webupdater.lbc library %i
	@%append watcom\webupdater.lbc option resource=watcom\webupdater_app.res
	@for %i in () do @%append watcom\webupdater.lbc option stack=%i
	wlink @watcom\webupdater.lbc

rep4_simple100 : .SYMBOLIC 
	if not exist ..\samples\simple\v1.0.0 mkdir ..\samples\simple\v1.0.0
	for %f in (webupdater.exe webupdatedlg.xrc) do copy ..\src\%f ..\samples\simple\v1.0.0

rep4_simple203 : .SYMBOLIC 
	if not exist ..\samples\simple\v2.0.3 mkdir ..\samples\simple\v2.0.3
	for %f in (webupdater.exe webupdatedlg.xrc) do copy ..\src\%f ..\samples\simple\v2.0.3

rep4_adv001 : .SYMBOLIC 
	if not exist ..\samples\advanced\v0.0.1 mkdir ..\samples\advanced\v0.0.1
	for %f in (webupdater.exe webupdatedlg.xrc) do copy ..\src\%f ..\samples\advanced\v0.0.1

rep4_adv150 : .SYMBOLIC 
	if not exist ..\samples\advanced\v1.5.0 mkdir ..\samples\advanced\v1.5.0
	for %f in (webupdater.exe webupdatedlg.xrc) do copy ..\src\%f ..\samples\advanced\v1.5.0

..\samples\simple\v1.0.0\simple.exe :  $(SIMPLE_1_0_0_OBJECTS) ..\src\webupdater.exe watcom\simple_1_0_0_minimal.res
	@%create watcom\simple_1_0_0.lbc
	@%append watcom\simple_1_0_0.lbc option quiet
	@%append watcom\simple_1_0_0.lbc name $^@
	@%append watcom\simple_1_0_0.lbc option caseexact
	@%append watcom\simple_1_0_0.lbc $(LDFLAGS) $(__DEBUGINFO_4)  libpath $(WX_DIR)$(WXLIBPATH) libpath ..\lib system nt_win ref '_WinMain@16'
	@for %i in ($(SIMPLE_1_0_0_OBJECTS)) do @%append watcom\simple_1_0_0.lbc file %i
	@for %i in ( wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\simple_1_0_0.lbc library %i
	@%append watcom\simple_1_0_0.lbc option resource=watcom\simple_1_0_0_minimal.res
	@for %i in () do @%append watcom\simple_1_0_0.lbc option stack=%i
	wlink @watcom\simple_1_0_0.lbc

..\samples\simple\v2.0.3\simple.exe :  $(SIMPLE_2_0_3_OBJECTS) ..\src\webupdater.exe watcom\simple_2_0_3_minimal.res
	@%create watcom\simple_2_0_3.lbc
	@%append watcom\simple_2_0_3.lbc option quiet
	@%append watcom\simple_2_0_3.lbc name $^@
	@%append watcom\simple_2_0_3.lbc option caseexact
	@%append watcom\simple_2_0_3.lbc $(LDFLAGS) $(__DEBUGINFO_4)  libpath $(WX_DIR)$(WXLIBPATH) libpath ..\lib system nt_win ref '_WinMain@16'
	@for %i in ($(SIMPLE_2_0_3_OBJECTS)) do @%append watcom\simple_2_0_3.lbc file %i
	@for %i in ( wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\simple_2_0_3.lbc library %i
	@%append watcom\simple_2_0_3.lbc option resource=watcom\simple_2_0_3_minimal.res
	@for %i in () do @%append watcom\simple_2_0_3.lbc option stack=%i
	wlink @watcom\simple_2_0_3.lbc

..\samples\advanced\v0.0.1\advanced.exe :  $(ADVANCED_0_0_1_OBJECTS) ..\src\webupdater.exe watcom\advanced_0_0_1_minimal.res
	@%create watcom\advanced_0_0_1.lbc
	@%append watcom\advanced_0_0_1.lbc option quiet
	@%append watcom\advanced_0_0_1.lbc name $^@
	@%append watcom\advanced_0_0_1.lbc option caseexact
	@%append watcom\advanced_0_0_1.lbc $(LDFLAGS) $(__DEBUGINFO_4)  libpath $(WX_DIR)$(WXLIBPATH) libpath ..\lib system nt_win ref '_WinMain@16'
	@for %i in ($(ADVANCED_0_0_1_OBJECTS)) do @%append watcom\advanced_0_0_1.lbc file %i
	@for %i in ( wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\advanced_0_0_1.lbc library %i
	@%append watcom\advanced_0_0_1.lbc option resource=watcom\advanced_0_0_1_minimal.res
	@for %i in () do @%append watcom\advanced_0_0_1.lbc option stack=%i
	wlink @watcom\advanced_0_0_1.lbc

..\samples\advanced\v1.5.0\advanced.exe :  $(ADVANCED_1_5_0_OBJECTS) ..\src\webupdater.exe watcom\advanced_1_5_0_minimal.res
	@%create watcom\advanced_1_5_0.lbc
	@%append watcom\advanced_1_5_0.lbc option quiet
	@%append watcom\advanced_1_5_0.lbc name $^@
	@%append watcom\advanced_1_5_0.lbc option caseexact
	@%append watcom\advanced_1_5_0.lbc $(LDFLAGS) $(__DEBUGINFO_4)  libpath $(WX_DIR)$(WXLIBPATH) libpath ..\lib system nt_win ref '_WinMain@16'
	@for %i in ($(ADVANCED_1_5_0_OBJECTS)) do @%append watcom\advanced_1_5_0.lbc file %i
	@for %i in ( wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\advanced_1_5_0.lbc library %i
	@%append watcom\advanced_1_5_0.lbc option resource=watcom\advanced_1_5_0_minimal.res
	@for %i in () do @%append watcom\advanced_1_5_0.lbc option stack=%i
	wlink @watcom\advanced_1_5_0.lbc

watcom\webupdate_lib_webupdate.obj :  .AUTODEPEND .\..\src\webupdate.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_LIB_CXXFLAGS) $<

watcom\webupdate_lib_webupdatedlg.obj :  .AUTODEPEND .\..\src\webupdatedlg.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_LIB_CXXFLAGS) $<

watcom\webupdate_lib_checkedlistctrl.obj :  .AUTODEPEND .\..\src\checkedlistctrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_LIB_CXXFLAGS) $<

watcom\webupdate_lib_md5.obj :  .AUTODEPEND .\..\src\md5.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_LIB_CXXFLAGS) $<

watcom\webupdate_lib_installer.obj :  .AUTODEPEND .\..\src\installer.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_LIB_CXXFLAGS) $<

watcom\webupdate_lib_download.obj :  .AUTODEPEND .\..\src\download.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_LIB_CXXFLAGS) $<

watcom\webupdate_lib_stdactions.obj :  .AUTODEPEND .\..\src\stdactions.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_LIB_CXXFLAGS) $<

watcom\webupdate_lib_webupdatectrl.obj :  .AUTODEPEND .\..\src\webupdatectrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_LIB_CXXFLAGS) $<

watcom\webupdate_dll_webupdate.obj :  .AUTODEPEND .\..\src\webupdate.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_DLL_CXXFLAGS) $<

watcom\webupdate_dll_webupdatedlg.obj :  .AUTODEPEND .\..\src\webupdatedlg.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_DLL_CXXFLAGS) $<

watcom\webupdate_dll_checkedlistctrl.obj :  .AUTODEPEND .\..\src\checkedlistctrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_DLL_CXXFLAGS) $<

watcom\webupdate_dll_md5.obj :  .AUTODEPEND .\..\src\md5.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_DLL_CXXFLAGS) $<

watcom\webupdate_dll_installer.obj :  .AUTODEPEND .\..\src\installer.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_DLL_CXXFLAGS) $<

watcom\webupdate_dll_download.obj :  .AUTODEPEND .\..\src\download.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_DLL_CXXFLAGS) $<

watcom\webupdate_dll_stdactions.obj :  .AUTODEPEND .\..\src\stdactions.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_DLL_CXXFLAGS) $<

watcom\webupdate_dll_webupdatectrl.obj :  .AUTODEPEND .\..\src\webupdatectrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATE_DLL_CXXFLAGS) $<

watcom\webupdater_app.res :  .AUTODEPEND .\..\src\app.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=$(HTTPENGINE_DIR)\include $(__HTTPENGINEDEP_DEF_p)  -i=..\src $<

watcom\webupdater_webapp.obj :  .AUTODEPEND .\..\src\webapp.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATER_CXXFLAGS) $<

watcom\simple_1_0_0_minimal.obj :  .AUTODEPEND .\..\samples\simple\v1.0.0\minimal.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(SIMPLE_1_0_0_CXXFLAGS) $<

watcom\simple_1_0_0_minimal.res :  .AUTODEPEND .\..\samples\simple\v1.0.0\minimal.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=..\samples\simple\v1.0.0 $<

watcom\simple_2_0_3_minimal.obj :  .AUTODEPEND .\..\samples\simple\v2.0.3\minimal.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(SIMPLE_2_0_3_CXXFLAGS) $<

watcom\simple_2_0_3_minimal.res :  .AUTODEPEND .\..\samples\simple\v2.0.3\minimal.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=..\samples\simple\v2.0.3 $<

watcom\advanced_0_0_1_minimal.obj :  .AUTODEPEND .\..\samples\advanced\v0.0.1\minimal.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(ADVANCED_0_0_1_CXXFLAGS) $<

watcom\advanced_0_0_1_minimal.res :  .AUTODEPEND .\..\samples\advanced\v0.0.1\minimal.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=..\samples\advanced\v0.0.1 $<

watcom\advanced_1_5_0_minimal.obj :  .AUTODEPEND .\..\samples\advanced\v1.5.0\minimal.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(ADVANCED_1_5_0_CXXFLAGS) $<

watcom\advanced_1_5_0_minimal.res :  .AUTODEPEND .\..\samples\advanced\v1.5.0\minimal.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=..\samples\advanced\v1.5.0 $<

