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

# The libxml2 library main folder 
LIBXML2_DIR = c:\libxml2

# The iconv library main folder 
ICONV_DIR = c:\iconv

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
__DEBUG_DEFINE_p =
!ifeq BUILD debug
__DEBUG_DEFINE_p = -d__WXDEBUG__
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

### Variables: ###

WXXML2_CXXFLAGS = -i=$(WXWIN)\include &
	-i=$(WXWIN)\lib\wat_lib\msw$(WXLIBPOSTFIX) -i=..\include &
	-i=$(LIBXML2_DIR)\include -i=$(ICONV_DIR)\include $(CPPFLAGS) $(CXXFLAGS)
WXXML2_OBJECTS =  &
	watcom\wxxml2_xml2.obj
MINIMAL_CXXFLAGS = -i=$(WXWIN)\include &
	-i=$(WXWIN)\lib\wat_lib\msw$(WXLIBPOSTFIX) -i=..\include &
	-i=$(LIBXML2_DIR)\include -i=$(ICONV_DIR)\include $(__DEBUG_DEFINE_p) &
	-d__WXMSW__ $(CPPFLAGS) $(CXXFLAGS)
MINIMAL_OBJECTS =  &
	watcom\minimal_xml2.obj &
	watcom\minimal_minimal.obj



all: watcom
watcom:
	-if not exist watcom mkdir watcom

### Targets: ###

all : .SYMBOLIC ..\lib\wxxml2.lib ..\sample\minimal.exe

clean : .SYMBOLIC 
	-if exist watcom\*.obj del watcom\*.obj
	-if exist watcom\*.res del watcom\*.res
	-if exist watcom\*.lbc del watcom\*.lbc
	-if exist watcom\*.ilk del watcom\*.ilk
	-if exist ..\lib\wxxml2.lib del ..\lib\wxxml2.lib
	-if exist ..\sample\minimal.exe del ..\sample\minimal.exe

tarball :  
	( cd .. && tar -cvzf wxxml2.tar.gz --exclude=*.pdb --exclude=*.log --exclude=*.o* * )

zip :  
	( cd .. && zip -r9 wxxml2.zip *  -x *.pdb -x *.log -x *.o* )

cleandocs :  
	-if exist ..\docs\html rmdir /S /Q ..\docs\html

..\lib\wxxml2.lib :  $(WXXML2_OBJECTS)
	@%create watcom\wxxml2.lbc
	@for %i in ($(WXXML2_OBJECTS)) do @%append watcom\wxxml2.lbc +%i
	wlib -q -p4096 -n -b $^@ @watcom\wxxml2.lbc

..\sample\minimal.exe :  $(MINIMAL_OBJECTS)
	@%create watcom\minimal.lbc
	@%append watcom\minimal.lbc option quiet
	@%append watcom\minimal.lbc name $^@
	@%append watcom\minimal.lbc option caseexact
	@%append watcom\minimal.lbc $(LDFLAGS) libpath $(WXWIN)\lib\wat_lib system nt_win ref '_WinMain@16' libpath ..\lib libpath $(LIBXML2_DIR)\lib libpath $(ICONV_DIR)\lib
	@for %i in ($(MINIMAL_OBJECTS)) do @%append watcom\minimal.lbc file %i
	@for %i in ( wxxml2.lib libxml2.lib iconv.lib wxmsw25$(WXLIBPOSTFIX)_core.lib wxbase25$(WXLIBPOSTFIX).lib wxtiff$(WXSUBLIBPOSTFIX).lib wxjpeg$(WXSUBLIBPOSTFIX).lib wxpng$(WXSUBLIBPOSTFIX).lib wxzlib$(WXSUBLIBPOSTFIX).lib wxregex$(WXSUBLIBPOSTFIX).lib wxexpat$(WXSUBLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\minimal.lbc library %i
	@%append watcom\minimal.lbc
	wlink @watcom\minimal.lbc

docs :  
	( cd ..\docs && doxygen )

watcom\wxxml2_xml2.obj :  .AUTODEPEND .\..\src\xml2.cpp
	$(CXX) -zq -fo=$^@ $(WXXML2_CXXFLAGS) $<

watcom\minimal_xml2.obj :  .AUTODEPEND .\..\src\xml2.cpp
	$(CXX) -zq -fo=$^@ $(MINIMAL_CXXFLAGS) $<

watcom\minimal_minimal.obj :  .AUTODEPEND .\..\sample\minimal.cpp
	$(CXX) -zq -fo=$^@ $(MINIMAL_CXXFLAGS) $<

