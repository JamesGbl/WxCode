# =========================================================================
# =========================================================================



prefix := $(shell wx-config --prefix)
exec_prefix := $(shell wx-config --exec-prefix)
INSTALL = /usr/bin/install -c
EXEEXT = 
WINDRES = 
REZ = /Developer/Tools/Rez
SETFILE = /Developer/Tools/SetFile
NM = nm
srcdir = .
top_srcdir = $(prefix)/
top_builddir = $(exec_prefix)/
LDFLAGS_GUI = 
CXX = g++
CXXFLAGS = -Wall -Wundef -Wno-ctor-dtor-privacy -O0 -fno-common
CPPFLAGS := $(shell wx-config --cppflags) -fpascal-strings -I${top_srcdir}/src/mac/carbon/morefilex -I/Developer/Headers/FlatCarbon -gdwarf-2 -g3 -arch i386 -ftree-vectorize -march=prescott -msse2 -mfpmath=sse -isysroot /Developer/SDKs/MacOSX10.4u.sdk
LDFLAGS :=  $(shell wx-config --libs)
WX_LIB_FLAVOUR = 
TOOLKIT = MAC
TOOLKIT_LOWERCASE = mac
TOOLKIT_VERSION = 
TOOLCHAIN_FULLNAME = mac-unicode-debug-2.7
EXTRALIBS =  -arch i386 -mmacosx-version-min=10.4 -isysroot /Developer/SDKs/MacOSX10.4u.sdk -framework QuickTime -framework IOKit -framework Carbon -framework Cocoa -framework System  -lz -lpthread -liconv 
EXTRALIBS_GUI = -framework WebKit
HOST_SUFFIX = 
SAMPLES_RPATH_FLAG = 
SAMPLES_RPATH_POSTLINK = $(top_builddir)change-install-names $(LIBDIRNAME) $(prefix) $@

### Variables: ###

DESTDIR = 
WX_RELEASE = 2.7
WX_VERSION = $(WX_RELEASE).0
LIBDIRNAME = $(top_builddir)lib
CLIENT_CXXFLAGS = -D__WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p) \
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) \
	-I$(srcdir) $(__DLLFLAG_p) -I$(srcdir)/../../samples $(CPPFLAGS) $(CXXFLAGS)
CLIENT_OBJECTS =  \
	$(os2_lib_res) \
	client_client.o \
	$(__client___win32rc)
SERVER_CXXFLAGS = -D__WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p) \
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) \
	-I$(srcdir) $(__DLLFLAG_p) -I$(srcdir)/../../samples $(CPPFLAGS) $(CXXFLAGS)
SERVER_OBJECTS =  \
	$(os2_lib_res) \
	server_server.o \
	$(__server___win32rc)

### Conditionally set variables: ###

#CXXC = $(CXX)
CXXC = $(top_builddir)./bk-deps $(CXX)
#PORTNAME = base
PORTNAME = $(TOOLKIT_LOWERCASE)$(TOOLKIT_VERSION)
WXBASEPORT = _carbon
#WXDEBUGFLAG = d
WXDEBUGFLAG = d
WXUNICODEFLAG = u
#WXUNIVNAME = univ
#EXTRALIBS_FOR_BASE = $(EXTRALIBS)
EXTRALIBS_FOR_BASE = $(EXTRALIBS) $(EXTRALIBS_GUI)
#EXTRALIBS_FOR_GUI = $(EXTRALIBS_GUI)
EXTRALIBS_FOR_GUI = 
__client___mac_setfilecmd = \
	$(SETFILE) -a C client$(EXEEXT)
__client___mac_rezcmd = $(__MACOSX_RESOURCES_p_2)
COND_PLATFORM_OS2_1___client___os2_emxbindcmd = $(NM) client$(EXEEXT) | if grep \
	-q pmwin.763 ; then emxbind -ep client$(EXEEXT) ; fi
#__client___os2_emxbindcmd = $(COND_PLATFORM_OS2_1___client___os2_emxbindcmd)
__client_bundle___depname = client_bundle
#____client_BUNDLE_TGT_REF_DEP = \
#	client.app/Contents/PkgInfo
____client_BUNDLE_TGT_REF_DEP = \
	client.app/Contents/PkgInfo
#__client___win32rc = client_client_rc.o
__server___mac_setfilecmd = \
	$(SETFILE) -a C server$(EXEEXT)
__server___mac_rezcmd = $(__MACOSX_RESOURCES_p_3)
COND_PLATFORM_OS2_1___server___os2_emxbindcmd = $(NM) server$(EXEEXT) | if grep \
	-q pmwin.763 ; then emxbind -ep server$(EXEEXT) ; fi
#__server___os2_emxbindcmd = $(COND_PLATFORM_OS2_1___server___os2_emxbindcmd)
__server_bundle___depname = server_bundle
#____server_BUNDLE_TGT_REF_DEP = \
#	server.app/Contents/PkgInfo
____server_BUNDLE_TGT_REF_DEP = \
	server.app/Contents/PkgInfo
#__server___win32rc = server_server_rc.o
COND_TOOLKIT_MAC___MACOSX_RESOURCES_p_2 = $(REZ) -d __DARWIN__ -t APPL -d \
	__WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p_0) $(__EXCEPTIONS_DEFINE_p_0) \
	$(__RTTI_DEFINE_p_0) $(__THREAD_DEFINE_p_0) -i $(srcdir) $(__DLLFLAG_p_0) -i \
	$(srcdir)/../../samples $(__RCDEFDIR_p) -i $(top_srcdir)/include -o \
	client$(EXEEXT) Carbon.r sample.r
__MACOSX_RESOURCES_p_2 = $(COND_TOOLKIT_MAC___MACOSX_RESOURCES_p_2)
COND_TOOLKIT_MAC___MACOSX_RESOURCES_p_3 = $(REZ) -d __DARWIN__ -t APPL -d \
	__WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p_0) $(__EXCEPTIONS_DEFINE_p_0) \
	$(__RTTI_DEFINE_p_0) $(__THREAD_DEFINE_p_0) -i $(srcdir) $(__DLLFLAG_p_0) -i \
	$(srcdir)/../../samples $(__RCDEFDIR_p) -i $(top_srcdir)/include -o \
	server$(EXEEXT) Carbon.r sample.r
__MACOSX_RESOURCES_p_3 = $(COND_TOOLKIT_MAC___MACOSX_RESOURCES_p_3)
#__WXUNIV_DEFINE_p = -D__WXUNIVERSAL__
#__WXUNIV_DEFINE_p_0 = -d __WXUNIVERSAL__
#__WXUNIV_DEFINE_p_6 = --define __WXUNIVERSAL__
#__EXCEPTIONS_DEFINE_p = -DwxNO_EXCEPTIONS
#__EXCEPTIONS_DEFINE_p_0 = -d wxNO_EXCEPTIONS
#__EXCEPTIONS_DEFINE_p_6 = --define wxNO_EXCEPTIONS
#__RTTI_DEFINE_p = -DwxNO_RTTI
#__RTTI_DEFINE_p_0 = -d wxNO_RTTI
#__RTTI_DEFINE_p_6 = --define wxNO_RTTI
#__THREAD_DEFINE_p = -DwxNO_THREADS
#__THREAD_DEFINE_p_0 = -d wxNO_THREADS
#__THREAD_DEFINE_p_6 = --define wxNO_THREADS
__DLLFLAG_p = -DWXUSINGDLL
__DLLFLAG_p_0 = -d WXUSINGDLL
__DLLFLAG_p_6 = --define WXUSINGDLL
#__RCDEFDIR_p = -i \
#	$(LIBDIRNAME)/wx/include/$(TOOLCHAIN_FULLNAME)
#__RCDEFDIR_p_0 = --include-dir \
#	$(LIBDIRNAME)/wx/include/$(TOOLCHAIN_FULLNAME)
#os2_lib_res = $(top_srcdir)/include/wx/os2/wx.res
COND_MONOLITHIC_0___WXLIB_CORE_p = \
	-lwx_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_core-$(WX_RELEASE)$(HOST_SUFFIX)
#__WXLIB_CORE_p = $(COND_MONOLITHIC_0___WXLIB_CORE_p)
COND_MONOLITHIC_0___WXLIB_NET_p = \
	-lwx_base$(WXBASEPORT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_net-$(WX_RELEASE)$(HOST_SUFFIX)
#__WXLIB_NET_p = $(COND_MONOLITHIC_0___WXLIB_NET_p)
COND_MONOLITHIC_0___WXLIB_BASE_p = \
	-lwx_base$(WXBASEPORT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
#__WXLIB_BASE_p = $(COND_MONOLITHIC_0___WXLIB_BASE_p)
COND_MONOLITHIC_1___WXLIB_MONO_p = \
	-lwx_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__WXLIB_MONO_p = $(COND_MONOLITHIC_1___WXLIB_MONO_p)
__LIB_TIFF_p \
	= \
	-lwxtiff$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__LIB_JPEG_p \
	= \
	-lwxjpeg$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__LIB_PNG_p \
	= \
	-lwxpng$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
#__LIB_ZLIB_p = \
#	-lwxzlib$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
#__LIB_ODBC_p = \
#	-lwxodbc$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
COND_WXUSE_REGEX_BUILTIN___LIB_REGEX_p = \
	-lwxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__LIB_REGEX_p = $(COND_WXUSE_REGEX_BUILTIN___LIB_REGEX_p)
__LIB_EXPAT_p = \
	-lwxexpat$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)

### Targets: ###

all: client$(EXEEXT) $(__client_bundle___depname) server$(EXEEXT) $(__server_bundle___depname)

install: all

uninstall: 

install-strip: install

clean: 
	rm -rf ./.deps ./.pch
	rm -f ./*.o
	rm -f client$(EXEEXT)
	rm -rf client.app
	rm -f server$(EXEEXT)
	rm -rf server.app

distclean: clean
	rm -f config.cache config.log config.status bk-deps bk-make-pch shared-ld-sh Makefile

client$(EXEEXT): $(CLIENT_OBJECTS) $(__client___win32rc)
	$(CXX) -o $@ $(CLIENT_OBJECTS) $(LDFLAGS)   -L$(LIBDIRNAME) $(LDFLAGS_GUI) $(SAMPLES_RPATH_FLAG)  $(__WXLIB_CORE_p)  $(__WXLIB_NET_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) $(EXTRALIBS_FOR_GUI) $(__LIB_ZLIB_p) $(__LIB_ODBC_p) $(__LIB_REGEX_p) $(__LIB_EXPAT_p) $(EXTRALIBS_FOR_BASE) ../wxDNSSD-2.7.0.dylib
	$(__client___mac_rezcmd)
	$(__client___mac_setfilecmd)
	$(__client___os2_emxbindcmd)
	$(SAMPLES_RPATH_POSTLINK)

client.app/Contents/PkgInfo: client$(EXEEXT) $(top_srcdir)/src/mac/carbon/Info.plist.in $(top_srcdir)/src/mac/carbon/wxmac.icns
	mkdir -p client.app/Contents
	mkdir -p client.app/Contents/MacOS
	mkdir -p client.app/Contents/Resources
	
	
	sed -e "s/IDENTIFIER/`echo $(srcdir) | sed -e 's,\.\./,,g' | sed -e 's,/,.,g'`/" \
	-e "s/EXECUTABLE/client/" \
	-e "s/VERSION/$(WX_VERSION)/" \
	$(top_srcdir)/src/mac/carbon/Info.plist.in >client.app/Contents/Info.plist
	
	
	echo -n "APPL????" >client.app/Contents/PkgInfo
	
	
	ln -f client$(EXEEXT) client.app/Contents/MacOS/client
	
	
	cp -f $(top_srcdir)/src/mac/carbon/wxmac.icns client.app/Contents/Resources/wxmac.icns

client_bundle: $(____client_BUNDLE_TGT_REF_DEP)

server$(EXEEXT): $(SERVER_OBJECTS) $(__server___win32rc)
	$(CXX) -o $@ $(SERVER_OBJECTS) $(LDFLAGS)   -L$(LIBDIRNAME) $(LDFLAGS_GUI) $(SAMPLES_RPATH_FLAG)  $(__WXLIB_CORE_p)  $(__WXLIB_NET_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) $(EXTRALIBS_FOR_GUI) $(__LIB_ZLIB_p) $(__LIB_ODBC_p) $(__LIB_REGEX_p) $(__LIB_EXPAT_p) $(EXTRALIBS_FOR_BASE) ../wxDNSSD-2.7.0.dylib
	$(__server___mac_rezcmd)
	$(__server___mac_setfilecmd)
	$(__server___os2_emxbindcmd)
	$(SAMPLES_RPATH_POSTLINK)

server.app/Contents/PkgInfo: server$(EXEEXT) $(top_srcdir)/src/mac/carbon/Info.plist.in $(top_srcdir)/src/mac/carbon/wxmac.icns
	mkdir -p server.app/Contents
	mkdir -p server.app/Contents/MacOS
	mkdir -p server.app/Contents/Resources
	
	
	sed -e "s/IDENTIFIER/`echo $(srcdir) | sed -e 's,\.\./,,g' | sed -e 's,/,.,g'`/" \
	-e "s/EXECUTABLE/server/" \
	-e "s/VERSION/$(WX_VERSION)/" \
	$(top_srcdir)/src/mac/carbon/Info.plist.in >server.app/Contents/Info.plist
	
	
	echo -n "APPL????" >server.app/Contents/PkgInfo
	
	
	ln -f server$(EXEEXT) server.app/Contents/MacOS/server
	
	
	cp -f $(top_srcdir)/src/mac/carbon/wxmac.icns server.app/Contents/Resources/wxmac.icns

server_bundle: $(____server_BUNDLE_TGT_REF_DEP)

client_client.o: $(srcdir)/client.cpp
	$(CXXC) -c -o $@ $(CLIENT_CXXFLAGS) $(srcdir)/client.cpp

client_client_rc.o: $(srcdir)/client.rc
	$(WINDRES) -i$< -o$@   --define __WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p_6)  $(__EXCEPTIONS_DEFINE_p_6) $(__RTTI_DEFINE_p_6) $(__THREAD_DEFINE_p_6)   --include-dir $(srcdir) $(__DLLFLAG_p_6) --include-dir $(srcdir)/../../samples $(__RCDEFDIR_p_0) --include-dir $(top_srcdir)/include

server_server.o: $(srcdir)/server.cpp
	$(CXXC) -c -o $@ $(SERVER_CXXFLAGS) $(srcdir)/server.cpp

server_server_rc.o: $(srcdir)/server.rc
	$(WINDRES) -i$< -o$@   --define __WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p_6)  $(__EXCEPTIONS_DEFINE_p_6) $(__RTTI_DEFINE_p_6) $(__THREAD_DEFINE_p_6)   --include-dir $(srcdir) $(__DLLFLAG_p_6) --include-dir $(srcdir)/../../samples $(__RCDEFDIR_p_0) --include-dir $(top_srcdir)/include


# Include dependency info, if present:
-include .deps/*.d

.PHONY: all install uninstall clean distclean client_bundle server_bundle
