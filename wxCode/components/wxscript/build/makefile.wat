# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.4 (http://bakefile.sourceforge.net)
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

# The Python library main folder 
PYTHON_DIR = c:\Python

# The LUA library main folder 
LUA_DIR = c:\lua

# The TOLUA library main folder 
TOLUA_DIR = c:\tolua

# Compile Unicode build of wxWindows? [0,1]
UNICODE = 0

# Type of compiled binaries [debug,release]
BUILD = debug

# The wxWidgets library main folder 
WXWIN = c:\wxWidgets



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

WXSUBLIBPOSTFIX =
!ifeq BUILD debug
!ifeq UNICODE 0
WXSUBLIBPOSTFIX = d
!endif
!endif
__UNICODE_DEFINE_p =
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -d_UNICODE
!endif
__OPTIMIZEFLAG =
!ifeq BUILD debug
__OPTIMIZEFLAG = -od
!endif
!ifeq BUILD release
__OPTIMIZEFLAG = -ot -ox
!endif
__DEBUGINFO =
!ifeq BUILD debug
__DEBUGINFO = -d2
!endif
!ifeq BUILD release
__DEBUGINFO = -d0
!endif
__DEBUGINFO_0 =
!ifeq BUILD debug
__DEBUGINFO_0 = debug all
!endif
!ifeq BUILD release
__DEBUGINFO_0 = 
!endif
WXLIBPOSTFIX =
!ifeq BUILD debug
!ifeq UNICODE 0
WXLIBPOSTFIX = d
!endif
!endif
!ifeq BUILD debug
!ifeq UNICODE 1
WXLIBPOSTFIX = ud
!endif
!endif
!ifeq BUILD release
!ifeq UNICODE 1
WXLIBPOSTFIX = u
!endif
!endif
__DEBUG_DEFINE_p =
!ifeq BUILD debug
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif

### Variables: ###

WXSCRIPT_CXXFLAGS = $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) $(__DEBUGINFO) &
	-i=$(WXWIN)\include -i=$(WXWIN)\lib\wat_lib\msw$(WXLIBPOSTFIX) &
	-i=..\include -i=$(PYTHON_DIR)\include -i=$(LUA_DIR)\include &
	-i=$(TOLUA_DIR)\include -dwxSCRIPT_NO_CINT -dwxSCRIPT_NO_UNDERC &
	-dwxSCRIPT_NO_PYTHON $(CPPFLAGS) $(CXXFLAGS)
WXSCRIPT_OBJECTS =  &
	watcom\wxscript_script.obj &
	watcom\wxscript_scpython.obj &
	watcom\wxscript_sccint.obj &
	watcom\wxscript_scunderc.obj &
	watcom\wxscript_sclua.obj
MINIMAL_CXXFLAGS = $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) $(__DEBUGINFO) &
	-i=..\include -i=$(WXWIN)\include &
	-i=$(WXWIN)\lib\wat_lib\msw$(WXLIBPOSTFIX) -i=$(PYTHON_DIR)\include &
	-i=$(LUA_DIR)\include -i=$(TOLUA_DIR)\include -dwxSCRIPT_NO_CINT &
	-dwxSCRIPT_NO_UNDERC -dwxSCRIPT_NO_PYTHON $(__DEBUG_DEFINE_p) -d__WXMSW__ &
	$(CPPFLAGS) $(CXXFLAGS)
MINIMAL_OBJECTS =  &
	watcom\minimal_Test.obj
MINIMAL2_CXXFLAGS = $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) $(__DEBUGINFO) &
	-i=..\include -i=$(WXWIN)\include &
	-i=$(WXWIN)\lib\wat_lib\msw$(WXLIBPOSTFIX) -i=$(PYTHON_DIR)\include &
	-i=$(LUA_DIR)\include -i=$(TOLUA_DIR)\include -dwxSCRIPT_NO_CINT &
	-dwxSCRIPT_NO_UNDERC -dwxSCRIPT_NO_PYTHON $(__DEBUG_DEFINE_p) -d__WXMSW__ &
	$(CPPFLAGS) $(CXXFLAGS)
MINIMAL2_OBJECTS =  &
	watcom\minimal2_Test.obj


all : watcom
watcom :
	-if not exist watcom mkdir watcom

### Targets: ###

all : .SYMBOLIC ..\lib\wxscript$(WXLIBPOSTFIX).lib ..\tests\test1\minimal.exe ..\tests\test2\minimal2.exe

clean : .SYMBOLIC 
	-if exist watcom\*.obj del watcom\*.obj
	-if exist watcom\*.res del watcom\*.res
	-if exist watcom\*.lbc del watcom\*.lbc
	-if exist watcom\*.ilk del watcom\*.ilk
	-if exist ..\lib\wxscript$(WXLIBPOSTFIX).lib del ..\lib\wxscript$(WXLIBPOSTFIX).lib
	-if exist ..\tests\test1\minimal.exe del ..\tests\test1\minimal.exe
	-if exist ..\tests\test2\minimal2.exe del ..\tests\test2\minimal2.exe

tarball :  
	( cd .. && tar -cvzf wxscript.tar.gz --exclude=*.pdb --exclude=*.log --exclude=*.o* * )

zip :  
	( cd .. && zip -r9 wxscript.zip *  -x *.pdb -x *.log -x *.o* )

cleanbuilddirs :  
	-if exist msvc rmdir /S /Q msvc
	-if exist watcom rmdir /S /Q watcom
	-if exist borland rmdir /S /Q borland
	-if exist mingw rmdir /S /Q mingw

cleanall :  clean cleanbuilddirs
	del /S /Q ..\*.log >NUL
	del /S /Q ..\*.lib >NUL
	del /S /Q ..\*.a >NUL
	del /S /Q ..\*.exe >NUL

docs :  
	( cd ..\docs && doxygen )

cleandocs :  
	-if exist ..\docs\html rmdir /S /Q ..\docs\html

..\lib\wxscript$(WXLIBPOSTFIX).lib :  $(WXSCRIPT_OBJECTS)
	@%create watcom\wxscript.lbc
	@for %i in ($(WXSCRIPT_OBJECTS)) do @%append watcom\wxscript.lbc +%i
	wlib -q -p4096 -n -b $^@ @watcom\wxscript.lbc

..\tests\test1\minimal.exe :  $(MINIMAL_OBJECTS) ..\lib\wxscript$(WXLIBPOSTFIX).lib
	@%create watcom\minimal.lbc
	@%append watcom\minimal.lbc option quiet
	@%append watcom\minimal.lbc name $^@
	@%append watcom\minimal.lbc option caseexact
	@%append watcom\minimal.lbc $(LDFLAGS) $(__DEBUGINFO_0) libpath ..\lib libpath $(WXWIN)\lib\wat_lib libpath $(PYTHON_DIR)\lib libpath $(LUA_DIR)\lib libpath $(TOLUA_DIR)\lib system nt ref 'main_'
	@for %i in ($(MINIMAL_OBJECTS)) do @%append watcom\minimal.lbc file %i
	@for %i in ( ..\lib\wxscript$(WXLIBPOSTFIX).lib lua.lib lualib.lib tolua.lib wxmsw25$(WXLIBPOSTFIX)_core.lib wxmsw25$(WXLIBPOSTFIX)_html.lib wxbase25$(WXLIBPOSTFIX).lib wxtiff$(WXSUBLIBPOSTFIX).lib wxjpeg$(WXSUBLIBPOSTFIX).lib wxpng$(WXSUBLIBPOSTFIX).lib wxzlib$(WXSUBLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WXSUBLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\minimal.lbc library %i
	@%append watcom\minimal.lbc
	wlink @watcom\minimal.lbc

..\tests\test2\minimal2.exe :  $(MINIMAL2_OBJECTS) ..\lib\wxscript$(WXLIBPOSTFIX).lib
	@%create watcom\minimal2.lbc
	@%append watcom\minimal2.lbc option quiet
	@%append watcom\minimal2.lbc name $^@
	@%append watcom\minimal2.lbc option caseexact
	@%append watcom\minimal2.lbc $(LDFLAGS) $(__DEBUGINFO_0) libpath ..\lib libpath $(WXWIN)\lib\wat_lib libpath $(PYTHON_DIR)\lib libpath $(LUA_DIR)\lib libpath $(TOLUA_DIR)\lib system nt ref 'main_'
	@for %i in ($(MINIMAL2_OBJECTS)) do @%append watcom\minimal2.lbc file %i
	@for %i in ( ..\lib\wxscript$(WXLIBPOSTFIX).lib lua.lib lualib.lib tolua.lib wxmsw25$(WXLIBPOSTFIX)_core.lib wxmsw25$(WXLIBPOSTFIX)_html.lib wxbase25$(WXLIBPOSTFIX).lib wxtiff$(WXSUBLIBPOSTFIX).lib wxjpeg$(WXSUBLIBPOSTFIX).lib wxpng$(WXSUBLIBPOSTFIX).lib wxzlib$(WXSUBLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WXSUBLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\minimal2.lbc library %i
	@%append watcom\minimal2.lbc
	wlink @watcom\minimal2.lbc

watcom\wxscript_script.obj :  .AUTODEPEND .\..\src\script.cpp
	$(CXX) -zq -fo=$^@ $(WXSCRIPT_CXXFLAGS) $<

watcom\wxscript_scpython.obj :  .AUTODEPEND .\..\src\scpython.cpp
	$(CXX) -zq -fo=$^@ $(WXSCRIPT_CXXFLAGS) $<

watcom\wxscript_sccint.obj :  .AUTODEPEND .\..\src\sccint.cpp
	$(CXX) -zq -fo=$^@ $(WXSCRIPT_CXXFLAGS) $<

watcom\wxscript_scunderc.obj :  .AUTODEPEND .\..\src\scunderc.cpp
	$(CXX) -zq -fo=$^@ $(WXSCRIPT_CXXFLAGS) $<

watcom\wxscript_sclua.obj :  .AUTODEPEND .\..\src\sclua.cpp
	$(CXX) -zq -fo=$^@ $(WXSCRIPT_CXXFLAGS) $<

watcom\minimal_Test.obj :  .AUTODEPEND .\..\tests\test1\Test.cpp
	$(CXX) -zq -fo=$^@ $(MINIMAL_CXXFLAGS) $<

watcom\minimal2_Test.obj :  .AUTODEPEND .\..\tests\test2\Test.cpp
	$(CXX) -zq -fo=$^@ $(MINIMAL2_CXXFLAGS) $<

