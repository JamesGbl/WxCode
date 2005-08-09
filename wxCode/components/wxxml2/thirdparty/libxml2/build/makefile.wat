# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.9 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================



# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------

# C compiler 
CC = wcc386

# Standard flags for CC 
CFLAGS = 

# Standard preprocessor flags (common for CC and CXX) 
CPPFLAGS = 

# Standard linker flags 
LDFLAGS = 

# If set to zero a STATIC libxml library will be built [0,1]
SHARED = 0

# Compile Unicode build? [0,1]
UNICODE = 0

# Type of compiled binaries [debug,release]
BUILD = release

# The iconv library main folder 
ICONV_DIR = c:\iconv

# Enable thread safety [no,ctls,native,posix]
WITH_THREADS = native

# Enable iconv support [0,1]
WITH_ICONV = 1

# Enable ZLIB support [0,1]
WITH_ZLIB = 0



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

THREADS_DEF =
!ifeq WITH_THREADS native
THREADS_DEF = -d_REENTRANT -dHAVE_WIN32_THREADS
!endif
!ifeq WITH_THREADS ctls
THREADS_DEF = -d_REENTRANT -dHAVE_WIN32_THREADS -dHAVE_COMPILER_TLS
!endif
!ifeq WITH_THREADS posix
THREADS_DEF = -d_REENTRANT -dHAVE_PTHREAD_H
!endif
__WARNINGS =
!ifeq BUILD debug
__WARNINGS = -wx
!endif
!ifeq BUILD release
__WARNINGS = 
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
__DEBUGINFO_14 =
!ifeq BUILD debug
__DEBUGINFO_14 = debug all
!endif
!ifeq BUILD release
__DEBUGINFO_14 = 
!endif
__ZLIB_DEF_p =
!ifeq WITH_ZLIB 1
__ZLIB_DEF_p = -dHAVE_ZLIB_H
!endif
__DEBUG_DEF_p =
!ifeq BUILD debug
__DEBUG_DEF_p = -d_DEBUG
!endif
!ifeq BUILD release
__DEBUG_DEF_p = -dNDEBUG
!endif
__STATIC_DEF_p =
!ifeq SHARED 0
__STATIC_DEF_p = -dLIBXML_STATIC
!endif
__ICONV_LIB_p =
!ifeq WITH_ICONV 1
__ICONV_LIB_p = iconv.lib
!endif
__WSOCK32_LIB_p =
!ifeq WITH_THREADS native
__WSOCK32_LIB_p = wsock32.lib
!endif
__ZLIB_LIB_p =
!ifeq WITH_ZLIB 1
__ZLIB_LIB_p = zdll.lib
!endif
__POSIX_LIB_p =
!ifeq WITH_THREADS posix
__POSIX_LIB_p = pthreadVC.lib
!endif

### Variables: ###

LIBXML2_CFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(ICONV_DIR)\include $(__ZLIB_DEF_p) &
	$(__DEBUG_DEF_p) $(__STATIC_DEF_p) -dWIN32 -d_WINDOWS -d_MBCS $(THREADS_DEF) &
	$(CPPFLAGS) $(CFLAGS)
LIBXML2_OBJECTS =  &
	watcom\libxml2_c14n.obj &
	watcom\libxml2_catalog.obj &
	watcom\libxml2_chvalid.obj &
	watcom\libxml2_debugXML.obj &
	watcom\libxml2_dict.obj &
	watcom\libxml2_DOCBparser.obj &
	watcom\libxml2_encoding.obj &
	watcom\libxml2_entities.obj &
	watcom\libxml2_error.obj &
	watcom\libxml2_globals.obj &
	watcom\libxml2_hash.obj &
	watcom\libxml2_HTMLparser.obj &
	watcom\libxml2_HTMLtree.obj &
	watcom\libxml2_legacy.obj &
	watcom\libxml2_list.obj &
	watcom\libxml2_nanoftp.obj &
	watcom\libxml2_nanohttp.obj &
	watcom\libxml2_parser.obj &
	watcom\libxml2_parserInternals.obj &
	watcom\libxml2_pattern.obj &
	watcom\libxml2_relaxng.obj &
	watcom\libxml2_SAX2.obj &
	watcom\libxml2_SAX.obj &
	watcom\libxml2_threads.obj &
	watcom\libxml2_tree.obj &
	watcom\libxml2_uri.obj &
	watcom\libxml2_valid.obj &
	watcom\libxml2_xinclude.obj &
	watcom\libxml2_xlink.obj &
	watcom\libxml2_xmlIO.obj &
	watcom\libxml2_xmlmemory.obj &
	watcom\libxml2_xmlreader.obj &
	watcom\libxml2_xmlregexp.obj &
	watcom\libxml2_xmlsave.obj &
	watcom\libxml2_xmlschemas.obj &
	watcom\libxml2_xmlschemastypes.obj &
	watcom\libxml2_xmlunicode.obj &
	watcom\libxml2_xmlwriter.obj &
	watcom\libxml2_xpath.obj &
	watcom\libxml2_xpointer.obj &
	watcom\libxml2_xmlstring.obj
TESTAUTOMATA_CFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(ICONV_DIR)\include $(__ZLIB_DEF_p) &
	$(__DEBUG_DEF_p) $(__STATIC_DEF_p) -dWIN32 -d_WINDOWS -d_MBCS $(THREADS_DEF) &
	$(CPPFLAGS) $(CFLAGS)
TESTAUTOMATA_OBJECTS =  &
	watcom\testAutomata_testAutomata.obj
TESTC14N_CFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(ICONV_DIR)\include $(__ZLIB_DEF_p) &
	$(__DEBUG_DEF_p) $(__STATIC_DEF_p) -dWIN32 -d_WINDOWS -d_MBCS $(THREADS_DEF) &
	$(CPPFLAGS) $(CFLAGS)
TESTC14N_OBJECTS =  &
	watcom\testC14N_testC14N.obj
TESTHTML_CFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(ICONV_DIR)\include $(__ZLIB_DEF_p) &
	$(__DEBUG_DEF_p) $(__STATIC_DEF_p) -dWIN32 -d_WINDOWS -d_MBCS $(THREADS_DEF) &
	$(CPPFLAGS) $(CFLAGS)
TESTHTML_OBJECTS =  &
	watcom\testHTML_testHTML.obj
TESTREADER_CFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(ICONV_DIR)\include $(__ZLIB_DEF_p) &
	$(__DEBUG_DEF_p) $(__STATIC_DEF_p) -dWIN32 -d_WINDOWS -d_MBCS $(THREADS_DEF) &
	$(CPPFLAGS) $(CFLAGS)
TESTREADER_OBJECTS =  &
	watcom\testReader_testReader.obj
TESTREGEXP_CFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(ICONV_DIR)\include $(__ZLIB_DEF_p) &
	$(__DEBUG_DEF_p) $(__STATIC_DEF_p) -dWIN32 -d_WINDOWS -d_MBCS $(THREADS_DEF) &
	$(CPPFLAGS) $(CFLAGS)
TESTREGEXP_OBJECTS =  &
	watcom\testRegexp_testRegexp.obj
TESTRELAX_CFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(ICONV_DIR)\include $(__ZLIB_DEF_p) &
	$(__DEBUG_DEF_p) $(__STATIC_DEF_p) -dWIN32 -d_WINDOWS -d_MBCS $(THREADS_DEF) &
	$(CPPFLAGS) $(CFLAGS)
TESTRELAX_OBJECTS =  &
	watcom\testRelax_testRelax.obj
TESTSAX_CFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(ICONV_DIR)\include $(__ZLIB_DEF_p) &
	$(__DEBUG_DEF_p) $(__STATIC_DEF_p) -dWIN32 -d_WINDOWS -d_MBCS $(THREADS_DEF) &
	$(CPPFLAGS) $(CFLAGS)
TESTSAX_OBJECTS =  &
	watcom\testSax_testSax.obj
TESTSCHEMAS_CFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(ICONV_DIR)\include $(__ZLIB_DEF_p) &
	$(__DEBUG_DEF_p) $(__STATIC_DEF_p) -dWIN32 -d_WINDOWS -d_MBCS $(THREADS_DEF) &
	$(CPPFLAGS) $(CFLAGS)
TESTSCHEMAS_OBJECTS =  &
	watcom\testSchemas_testSchemas.obj
TESTURI_CFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(ICONV_DIR)\include $(__ZLIB_DEF_p) &
	$(__DEBUG_DEF_p) $(__STATIC_DEF_p) -dWIN32 -d_WINDOWS -d_MBCS $(THREADS_DEF) &
	$(CPPFLAGS) $(CFLAGS)
TESTURI_OBJECTS =  &
	watcom\testURI_testURI.obj
TESTXPATH_CFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(ICONV_DIR)\include $(__ZLIB_DEF_p) &
	$(__DEBUG_DEF_p) $(__STATIC_DEF_p) -dWIN32 -d_WINDOWS -d_MBCS $(THREADS_DEF) &
	$(CPPFLAGS) $(CFLAGS)
TESTXPATH_OBJECTS =  &
	watcom\testXPath_testXPath.obj
XMLLINT_CFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(ICONV_DIR)\include $(__ZLIB_DEF_p) &
	$(__DEBUG_DEF_p) $(__STATIC_DEF_p) -dWIN32 -d_WINDOWS -d_MBCS $(THREADS_DEF) &
	$(CPPFLAGS) $(CFLAGS)
XMLLINT_OBJECTS =  &
	watcom\xmllint_xmllint.obj


all : watcom
watcom :
	-if not exist watcom mkdir watcom

### Targets: ###

all : .SYMBOLIC ..\config.h setupdirs ..\lib\libxml2.lib ..\bin\testAutomata.exe ..\bin\testC14N.exe ..\bin\testHTML.exe ..\bin\testReader.exe ..\bin\testRegexp.exe ..\bin\testRelax.exe ..\bin\testSax.exe ..\bin\testSchemas.exe ..\bin\testURI.exe ..\bin\testXPath.exe ..\bin\xmllint.exe

clean : .SYMBOLIC 
	-if exist watcom\*.obj del watcom\*.obj
	-if exist watcom\*.res del watcom\*.res
	-if exist watcom\*.lbc del watcom\*.lbc
	-if exist watcom\*.ilk del watcom\*.ilk
	-if exist watcom\*.pch del watcom\*.pch
	-if exist ..\lib\libxml2.lib del ..\lib\libxml2.lib
	-if exist ..\bin\testAutomata.exe del ..\bin\testAutomata.exe
	-if exist ..\bin\testC14N.exe del ..\bin\testC14N.exe
	-if exist ..\bin\testHTML.exe del ..\bin\testHTML.exe
	-if exist ..\bin\testReader.exe del ..\bin\testReader.exe
	-if exist ..\bin\testRegexp.exe del ..\bin\testRegexp.exe
	-if exist ..\bin\testRelax.exe del ..\bin\testRelax.exe
	-if exist ..\bin\testSax.exe del ..\bin\testSax.exe
	-if exist ..\bin\testSchemas.exe del ..\bin\testSchemas.exe
	-if exist ..\bin\testURI.exe del ..\bin\testURI.exe
	-if exist ..\bin\testXPath.exe del ..\bin\testXPath.exe
	-if exist ..\bin\xmllint.exe del ..\bin\xmllint.exe

..\config.h :  ..\include\win32config.h
	copy ..\include\win32config.h ..\config.h

..\lib :  
	if not exist ..\lib mkdir ..\lib

..\bin :  
	if not exist ..\bin mkdir ..\bin

setupdirs : .SYMBOLIC ..\lib ..\bin

..\lib\libxml2.lib :  $(LIBXML2_OBJECTS) ..\config.h ..\lib
	@%create watcom\libxml2.lbc
	@for %i in ($(LIBXML2_OBJECTS)) do @%append watcom\libxml2.lbc +%i
	wlib -q -p4096 -n -b $^@ @watcom\libxml2.lbc

..\bin\testAutomata.exe :  $(TESTAUTOMATA_OBJECTS) ..\lib\libxml2.lib
	@%create watcom\testAutomata.lbc
	@%append watcom\testAutomata.lbc option quiet
	@%append watcom\testAutomata.lbc name $^@
	@%append watcom\testAutomata.lbc option caseexact
	@%append watcom\testAutomata.lbc $(LDFLAGS) $(__DEBUGINFO_14) libpath $(ICONV_DIR)\lib system nt ref 'main_'
	@for %i in ($(TESTAUTOMATA_OBJECTS)) do @%append watcom\testAutomata.lbc file %i
	@for %i in ( ..\lib\libxml2.lib $(__ICONV_LIB_p) $(__WSOCK32_LIB_p) $(__ZLIB_LIB_p) $(__POSIX_LIB_p)) do @%append watcom\testAutomata.lbc library %i
	@%append watcom\testAutomata.lbc
	wlink @watcom\testAutomata.lbc

..\bin\testC14N.exe :  $(TESTC14N_OBJECTS) ..\lib\libxml2.lib
	@%create watcom\testC14N.lbc
	@%append watcom\testC14N.lbc option quiet
	@%append watcom\testC14N.lbc name $^@
	@%append watcom\testC14N.lbc option caseexact
	@%append watcom\testC14N.lbc $(LDFLAGS) $(__DEBUGINFO_14) libpath $(ICONV_DIR)\lib system nt ref 'main_'
	@for %i in ($(TESTC14N_OBJECTS)) do @%append watcom\testC14N.lbc file %i
	@for %i in ( ..\lib\libxml2.lib $(__ICONV_LIB_p) $(__WSOCK32_LIB_p) $(__ZLIB_LIB_p) $(__POSIX_LIB_p)) do @%append watcom\testC14N.lbc library %i
	@%append watcom\testC14N.lbc
	wlink @watcom\testC14N.lbc

..\bin\testHTML.exe :  $(TESTHTML_OBJECTS) ..\lib\libxml2.lib
	@%create watcom\testHTML.lbc
	@%append watcom\testHTML.lbc option quiet
	@%append watcom\testHTML.lbc name $^@
	@%append watcom\testHTML.lbc option caseexact
	@%append watcom\testHTML.lbc $(LDFLAGS) $(__DEBUGINFO_14) libpath $(ICONV_DIR)\lib system nt ref 'main_'
	@for %i in ($(TESTHTML_OBJECTS)) do @%append watcom\testHTML.lbc file %i
	@for %i in ( ..\lib\libxml2.lib $(__ICONV_LIB_p) $(__WSOCK32_LIB_p) $(__ZLIB_LIB_p) $(__POSIX_LIB_p)) do @%append watcom\testHTML.lbc library %i
	@%append watcom\testHTML.lbc
	wlink @watcom\testHTML.lbc

..\bin\testReader.exe :  $(TESTREADER_OBJECTS) ..\lib\libxml2.lib
	@%create watcom\testReader.lbc
	@%append watcom\testReader.lbc option quiet
	@%append watcom\testReader.lbc name $^@
	@%append watcom\testReader.lbc option caseexact
	@%append watcom\testReader.lbc $(LDFLAGS) $(__DEBUGINFO_14) libpath $(ICONV_DIR)\lib system nt ref 'main_'
	@for %i in ($(TESTREADER_OBJECTS)) do @%append watcom\testReader.lbc file %i
	@for %i in ( ..\lib\libxml2.lib $(__ICONV_LIB_p) $(__WSOCK32_LIB_p) $(__ZLIB_LIB_p) $(__POSIX_LIB_p)) do @%append watcom\testReader.lbc library %i
	@%append watcom\testReader.lbc
	wlink @watcom\testReader.lbc

..\bin\testRegexp.exe :  $(TESTREGEXP_OBJECTS) ..\lib\libxml2.lib
	@%create watcom\testRegexp.lbc
	@%append watcom\testRegexp.lbc option quiet
	@%append watcom\testRegexp.lbc name $^@
	@%append watcom\testRegexp.lbc option caseexact
	@%append watcom\testRegexp.lbc $(LDFLAGS) $(__DEBUGINFO_14) libpath $(ICONV_DIR)\lib system nt ref 'main_'
	@for %i in ($(TESTREGEXP_OBJECTS)) do @%append watcom\testRegexp.lbc file %i
	@for %i in ( ..\lib\libxml2.lib $(__ICONV_LIB_p) $(__WSOCK32_LIB_p) $(__ZLIB_LIB_p) $(__POSIX_LIB_p)) do @%append watcom\testRegexp.lbc library %i
	@%append watcom\testRegexp.lbc
	wlink @watcom\testRegexp.lbc

..\bin\testRelax.exe :  $(TESTRELAX_OBJECTS) ..\lib\libxml2.lib
	@%create watcom\testRelax.lbc
	@%append watcom\testRelax.lbc option quiet
	@%append watcom\testRelax.lbc name $^@
	@%append watcom\testRelax.lbc option caseexact
	@%append watcom\testRelax.lbc $(LDFLAGS) $(__DEBUGINFO_14) libpath $(ICONV_DIR)\lib system nt ref 'main_'
	@for %i in ($(TESTRELAX_OBJECTS)) do @%append watcom\testRelax.lbc file %i
	@for %i in ( ..\lib\libxml2.lib $(__ICONV_LIB_p) $(__WSOCK32_LIB_p) $(__ZLIB_LIB_p) $(__POSIX_LIB_p)) do @%append watcom\testRelax.lbc library %i
	@%append watcom\testRelax.lbc
	wlink @watcom\testRelax.lbc

..\bin\testSax.exe :  $(TESTSAX_OBJECTS) ..\lib\libxml2.lib
	@%create watcom\testSax.lbc
	@%append watcom\testSax.lbc option quiet
	@%append watcom\testSax.lbc name $^@
	@%append watcom\testSax.lbc option caseexact
	@%append watcom\testSax.lbc $(LDFLAGS) $(__DEBUGINFO_14) libpath $(ICONV_DIR)\lib system nt ref 'main_'
	@for %i in ($(TESTSAX_OBJECTS)) do @%append watcom\testSax.lbc file %i
	@for %i in ( ..\lib\libxml2.lib $(__ICONV_LIB_p) $(__WSOCK32_LIB_p) $(__ZLIB_LIB_p) $(__POSIX_LIB_p)) do @%append watcom\testSax.lbc library %i
	@%append watcom\testSax.lbc
	wlink @watcom\testSax.lbc

..\bin\testSchemas.exe :  $(TESTSCHEMAS_OBJECTS) ..\lib\libxml2.lib
	@%create watcom\testSchemas.lbc
	@%append watcom\testSchemas.lbc option quiet
	@%append watcom\testSchemas.lbc name $^@
	@%append watcom\testSchemas.lbc option caseexact
	@%append watcom\testSchemas.lbc $(LDFLAGS) $(__DEBUGINFO_14) libpath $(ICONV_DIR)\lib system nt ref 'main_'
	@for %i in ($(TESTSCHEMAS_OBJECTS)) do @%append watcom\testSchemas.lbc file %i
	@for %i in ( ..\lib\libxml2.lib $(__ICONV_LIB_p) $(__WSOCK32_LIB_p) $(__ZLIB_LIB_p) $(__POSIX_LIB_p)) do @%append watcom\testSchemas.lbc library %i
	@%append watcom\testSchemas.lbc
	wlink @watcom\testSchemas.lbc

..\bin\testURI.exe :  $(TESTURI_OBJECTS) ..\lib\libxml2.lib
	@%create watcom\testURI.lbc
	@%append watcom\testURI.lbc option quiet
	@%append watcom\testURI.lbc name $^@
	@%append watcom\testURI.lbc option caseexact
	@%append watcom\testURI.lbc $(LDFLAGS) $(__DEBUGINFO_14) libpath $(ICONV_DIR)\lib system nt ref 'main_'
	@for %i in ($(TESTURI_OBJECTS)) do @%append watcom\testURI.lbc file %i
	@for %i in ( ..\lib\libxml2.lib $(__ICONV_LIB_p) $(__WSOCK32_LIB_p) $(__ZLIB_LIB_p) $(__POSIX_LIB_p)) do @%append watcom\testURI.lbc library %i
	@%append watcom\testURI.lbc
	wlink @watcom\testURI.lbc

..\bin\testXPath.exe :  $(TESTXPATH_OBJECTS) ..\lib\libxml2.lib
	@%create watcom\testXPath.lbc
	@%append watcom\testXPath.lbc option quiet
	@%append watcom\testXPath.lbc name $^@
	@%append watcom\testXPath.lbc option caseexact
	@%append watcom\testXPath.lbc $(LDFLAGS) $(__DEBUGINFO_14) libpath $(ICONV_DIR)\lib system nt ref 'main_'
	@for %i in ($(TESTXPATH_OBJECTS)) do @%append watcom\testXPath.lbc file %i
	@for %i in ( ..\lib\libxml2.lib $(__ICONV_LIB_p) $(__WSOCK32_LIB_p) $(__ZLIB_LIB_p) $(__POSIX_LIB_p)) do @%append watcom\testXPath.lbc library %i
	@%append watcom\testXPath.lbc
	wlink @watcom\testXPath.lbc

..\bin\xmllint.exe :  $(XMLLINT_OBJECTS) ..\lib\libxml2.lib
	@%create watcom\xmllint.lbc
	@%append watcom\xmllint.lbc option quiet
	@%append watcom\xmllint.lbc name $^@
	@%append watcom\xmllint.lbc option caseexact
	@%append watcom\xmllint.lbc $(LDFLAGS) $(__DEBUGINFO_14) libpath $(ICONV_DIR)\lib system nt ref 'main_'
	@for %i in ($(XMLLINT_OBJECTS)) do @%append watcom\xmllint.lbc file %i
	@for %i in ( ..\lib\libxml2.lib $(__ICONV_LIB_p) $(__WSOCK32_LIB_p) $(__ZLIB_LIB_p) $(__POSIX_LIB_p)) do @%append watcom\xmllint.lbc library %i
	@%append watcom\xmllint.lbc
	wlink @watcom\xmllint.lbc

watcom\libxml2_c14n.obj :  .AUTODEPEND .\..\c14n.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_catalog.obj :  .AUTODEPEND .\..\catalog.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_chvalid.obj :  .AUTODEPEND .\..\chvalid.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_debugXML.obj :  .AUTODEPEND .\..\debugXML.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_dict.obj :  .AUTODEPEND .\..\dict.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_DOCBparser.obj :  .AUTODEPEND .\..\DOCBparser.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_encoding.obj :  .AUTODEPEND .\..\encoding.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_entities.obj :  .AUTODEPEND .\..\entities.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_error.obj :  .AUTODEPEND .\..\error.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_globals.obj :  .AUTODEPEND .\..\globals.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_hash.obj :  .AUTODEPEND .\..\hash.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_HTMLparser.obj :  .AUTODEPEND .\..\HTMLparser.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_HTMLtree.obj :  .AUTODEPEND .\..\HTMLtree.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_legacy.obj :  .AUTODEPEND .\..\legacy.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_list.obj :  .AUTODEPEND .\..\list.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_nanoftp.obj :  .AUTODEPEND .\..\nanoftp.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_nanohttp.obj :  .AUTODEPEND .\..\nanohttp.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_parser.obj :  .AUTODEPEND .\..\parser.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_parserInternals.obj :  .AUTODEPEND .\..\parserInternals.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_pattern.obj :  .AUTODEPEND .\..\pattern.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_relaxng.obj :  .AUTODEPEND .\..\relaxng.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_SAX2.obj :  .AUTODEPEND .\..\SAX2.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_SAX.obj :  .AUTODEPEND .\..\SAX.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_threads.obj :  .AUTODEPEND .\..\threads.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_tree.obj :  .AUTODEPEND .\..\tree.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_uri.obj :  .AUTODEPEND .\..\uri.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_valid.obj :  .AUTODEPEND .\..\valid.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xinclude.obj :  .AUTODEPEND .\..\xinclude.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xlink.obj :  .AUTODEPEND .\..\xlink.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xmlIO.obj :  .AUTODEPEND .\..\xmlIO.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xmlmemory.obj :  .AUTODEPEND .\..\xmlmemory.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xmlreader.obj :  .AUTODEPEND .\..\xmlreader.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xmlregexp.obj :  .AUTODEPEND .\..\xmlregexp.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xmlsave.obj :  .AUTODEPEND .\..\xmlsave.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xmlschemas.obj :  .AUTODEPEND .\..\xmlschemas.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xmlschemastypes.obj :  .AUTODEPEND .\..\xmlschemastypes.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xmlunicode.obj :  .AUTODEPEND .\..\xmlunicode.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xmlwriter.obj :  .AUTODEPEND .\..\xmlwriter.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xpath.obj :  .AUTODEPEND .\..\xpath.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xpointer.obj :  .AUTODEPEND .\..\xpointer.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\libxml2_xmlstring.obj :  .AUTODEPEND .\..\xmlstring.c
	$(CC) -zq -fo=$^@ $(LIBXML2_CFLAGS) $<

watcom\testAutomata_testAutomata.obj :  .AUTODEPEND .\..\testAutomata.c
	$(CC) -zq -fo=$^@ $(TESTAUTOMATA_CFLAGS) $<

watcom\testC14N_testC14N.obj :  .AUTODEPEND .\..\testC14N.c
	$(CC) -zq -fo=$^@ $(TESTC14N_CFLAGS) $<

watcom\testHTML_testHTML.obj :  .AUTODEPEND .\..\testHTML.c
	$(CC) -zq -fo=$^@ $(TESTHTML_CFLAGS) $<

watcom\testReader_testReader.obj :  .AUTODEPEND .\..\testReader.c
	$(CC) -zq -fo=$^@ $(TESTREADER_CFLAGS) $<

watcom\testRegexp_testRegexp.obj :  .AUTODEPEND .\..\testRegexp.c
	$(CC) -zq -fo=$^@ $(TESTREGEXP_CFLAGS) $<

watcom\testRelax_testRelax.obj :  .AUTODEPEND .\..\testRelax.c
	$(CC) -zq -fo=$^@ $(TESTRELAX_CFLAGS) $<

watcom\testSax_testSax.obj :  .AUTODEPEND .\..\testSax.c
	$(CC) -zq -fo=$^@ $(TESTSAX_CFLAGS) $<

watcom\testSchemas_testSchemas.obj :  .AUTODEPEND .\..\testSchemas.c
	$(CC) -zq -fo=$^@ $(TESTSCHEMAS_CFLAGS) $<

watcom\testURI_testURI.obj :  .AUTODEPEND .\..\testURI.c
	$(CC) -zq -fo=$^@ $(TESTURI_CFLAGS) $<

watcom\testXPath_testXPath.obj :  .AUTODEPEND .\..\testXPath.c
	$(CC) -zq -fo=$^@ $(TESTXPATH_CFLAGS) $<

watcom\xmllint_xmllint.obj :  .AUTODEPEND .\..\xmllint.c
	$(CC) -zq -fo=$^@ $(XMLLINT_CFLAGS) $<

