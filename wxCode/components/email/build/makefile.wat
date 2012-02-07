# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.9 (http://www.bakefile.org)
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
__wxEMail_lib___depname =
!ifeq SHARED 0
__wxEMail_lib___depname = &
	..\lib\wat_$(____wxEMail_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.lib
!endif
__wxEMail_dll___depname =
!ifeq SHARED 1
__wxEMail_dll___depname = &
	..\lib\wat_$(____wxEMail_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.dll
!endif
__WXLIB_HTML_NAME_p =
!ifeq WX_MONOLITHIC 0
__WXLIB_HTML_NAME_p = wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_html.lib
!endif
____wxEMail_lib__DIRNAME_SHARED_SUFFIX_FILENAMES =
!ifeq SHARED 0
____wxEMail_lib__DIRNAME_SHARED_SUFFIX_FILENAMES = lib
!endif
!ifeq SHARED 1
____wxEMail_lib__DIRNAME_SHARED_SUFFIX_FILENAMES = dll
!endif
____wxEMail_dll__DIRNAME_SHARED_SUFFIX_FILENAMES =
!ifeq WX_SHARED 0
____wxEMail_dll__DIRNAME_SHARED_SUFFIX_FILENAMES = lib
!endif
!ifeq WX_SHARED 1
____wxEMail_dll__DIRNAME_SHARED_SUFFIX_FILENAMES = dll
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
____wxEMail =
!ifeq WX_DEBUG 0
____wxEMail = -ot -ox
!endif
!ifeq WX_DEBUG 1
____wxEMail = -od
!endif
____wxEMail_3 =
!ifeq WX_DEBUG 0
____wxEMail_3 = -d0
!endif
!ifeq WX_DEBUG 1
____wxEMail_3 = -d2
!endif
____wxEMail_4 =
!ifeq WX_DEBUG 0
____wxEMail_4 = 
!endif
!ifeq WX_DEBUG 1
____wxEMail_4 = debug all
!endif
__WXLIB_NET_NAME_p =
!ifeq WX_MONOLITHIC 0
__WXLIB_NET_NAME_p = wxbase$(WX_VERSION)$(WXLIBPOSTFIX)_net.lib
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

WXEMAIL_LIB_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	$(____wxEMail) $(____wxEMail_3) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
WXEMAIL_LIB_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxemail.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_charsetconv.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_rfc2047.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_rfc2231.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_body.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_contentdescription.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_contentdisposition.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_contentid.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_contenttransferencoding.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_contenttype.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_fieldparam.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_header.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_message.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_mimeentity.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_mimeversion.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_strutils.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_utils.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_version.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_base64.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_qp.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_address.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_addresslist.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_datetime.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_field.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_fieldvalue.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_group.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_mailbox.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_mailboxlist.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_messageid.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_rheader.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_rmessage.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxpop3.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxpop3states.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxsmtp.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxsmtpemail.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxsmtpstates.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxSSLSocketClient.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxcmdprot.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxmd5.obj
WXEMAIL_DLL_CXXFLAGS = -bd $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	$(____wxEMail) $(____wxEMail_3) -wx -i=..\include -dWXMAKINGDLL_EMAIL &
	$(CPPFLAGS) $(CXXFLAGS)
WXEMAIL_DLL_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxemail.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_charsetconv.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_rfc2047.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_rfc2231.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_body.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_contentdescription.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_contentdisposition.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_contentid.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_contenttransferencoding.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_contenttype.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_fieldparam.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_header.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_message.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_mimeentity.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_mimeversion.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_strutils.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_utils.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_version.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_base64.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_qp.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_address.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_addresslist.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_datetime.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_field.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_fieldvalue.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_group.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_mailbox.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_mailboxlist.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_messageid.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_rheader.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_rmessage.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxpop3.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxpop3states.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxsmtp.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxsmtpemail.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxsmtpstates.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxSSLSocketClient.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxcmdprot.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxmd5.obj
WXEMAILSMTP_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	$(____wxEMail) $(____wxEMail_3) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
WXEMAILSMTP_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailSmtp_SendMail.obj
WXEMAILPOP3_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	$(____wxEMail) $(____wxEMail_3) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
WXEMAILPOP3_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailPop3_Pop3Client.obj


all : watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)
watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX) :
	-if not exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX) mkdir watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)

### Targets: ###

all : .SYMBOLIC test_for_selected_wxbuild $(__wxEMail_lib___depname) $(__wxEMail_dll___depname) ..\samples\SendMail\wxEMailSmtp.exe ..\samples\Pop3Client\wxEMailPop3.exe

clean : .SYMBOLIC 
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.obj del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.obj
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.res del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.res
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.lbc del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.lbc
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.ilk del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.ilk
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.pch del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.pch
	-if exist ..\lib\wat_$(____wxEMail_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.lib del ..\lib\wat_$(____wxEMail_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.lib
	-if exist ..\lib\wat_$(____wxEMail_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.dll del ..\lib\wat_$(____wxEMail_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.dll
	-if exist ..\lib\wat_$(____wxEMail_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.lib del ..\lib\wat_$(____wxEMail_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.lib
	-if exist ..\samples\SendMail\wxEMailSmtp.exe del ..\samples\SendMail\wxEMailSmtp.exe
	-if exist ..\samples\Pop3Client\wxEMailPop3.exe del ..\samples\Pop3Client\wxEMailPop3.exe

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

!ifeq SHARED 0
..\lib\wat_$(____wxEMail_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.lib :  make_dir_wxEMail_lib  $(WXEMAIL_LIB_OBJECTS)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib.lbc
	@for %i in ($(WXEMAIL_LIB_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib.lbc +%i
	wlib -q -p4096 -n -b $^@ @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib.lbc
!endif

make_dir_wxEMail_lib :  
	if not exist ..\lib\wat_$(____wxEMail_lib__DIRNAME_SHARED_SUFFIX_FILENAMES) mkdir ..\lib\wat_$(____wxEMail_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)

!ifeq SHARED 1
..\lib\wat_$(____wxEMail_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.dll :  make_dir_wxEMail_dll  $(WXEMAIL_DLL_OBJECTS)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(____wxEMail_4) libpath ..$(WXLIBPATH) $(LDFLAGS)
	@for %i in ($(WXEMAIL_DLL_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll.lbc file %i
	@for %i in ( $(__WXLIB_NET_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll.lbc system nt_dll
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll.lbc
	wlib -q -n -b ..\lib\wat_$(____wxEMail_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.lib +$^@
!endif

make_dir_wxEMail_dll :  
	if not exist ..\lib\wat_$(____wxEMail_dll__DIRNAME_SHARED_SUFFIX_FILENAMES) mkdir ..\lib\wat_$(____wxEMail_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)

..\samples\SendMail\wxEMailSmtp.exe :  $(WXEMAILSMTP_OBJECTS) make_sample_dir_wxEMailSmtp  $(__wxEMail_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailSmtp.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailSmtp.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailSmtp.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailSmtp.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailSmtp.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(____wxEMail_4) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'  $(LDFLAGS)
	@for %i in ($(WXEMAILSMTP_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailSmtp.lbc file %i
	@for %i in ( ..\lib\wat_$(____wxEMail_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.lib $(__WXLIB_NET_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailSmtp.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailSmtp.lbc option resource=
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailSmtp.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailSmtp.lbc

make_sample_dir_wxEMailSmtp :  
	if not exist ..\samples\SendMail mkdir ..\samples\SendMail

..\samples\Pop3Client\wxEMailPop3.exe :  $(WXEMAILPOP3_OBJECTS) make_sample_dir_wxEMailPop3  $(__wxEMail_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailPop3.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailPop3.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailPop3.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailPop3.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailPop3.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(____wxEMail_4) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'  $(LDFLAGS)
	@for %i in ($(WXEMAILPOP3_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailPop3.lbc file %i
	@for %i in ( ..\lib\wat_$(____wxEMail_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_email.lib $(__WXLIB_HTML_NAME_p) $(__WXLIB_NET_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailPop3.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailPop3.lbc option resource=
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailPop3.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailPop3.lbc

make_sample_dir_wxEMailPop3 :  
	if not exist ..\samples\Pop3Client mkdir ..\samples\Pop3Client

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxemail.obj :  .AUTODEPEND ..\src\wxemail.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_charsetconv.obj :  .AUTODEPEND ..\src\codec\charsetconv.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_rfc2047.obj :  .AUTODEPEND ..\src\codec\rfc2047.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_rfc2231.obj :  .AUTODEPEND ..\src\codec\rfc2231.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_body.obj :  .AUTODEPEND ..\src\mimetic\body.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_contentdescription.obj :  .AUTODEPEND ..\src\mimetic\contentdescription.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_contentdisposition.obj :  .AUTODEPEND ..\src\mimetic\contentdisposition.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_contentid.obj :  .AUTODEPEND ..\src\mimetic\contentid.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_contenttransferencoding.obj :  .AUTODEPEND ..\src\mimetic\contenttransferencoding.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_contenttype.obj :  .AUTODEPEND ..\src\mimetic\contenttype.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_fieldparam.obj :  .AUTODEPEND ..\src\mimetic\fieldparam.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_header.obj :  .AUTODEPEND ..\src\mimetic\header.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_message.obj :  .AUTODEPEND ..\src\mimetic\message.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_mimeentity.obj :  .AUTODEPEND ..\src\mimetic\mimeentity.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_mimeversion.obj :  .AUTODEPEND ..\src\mimetic\mimeversion.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_strutils.obj :  .AUTODEPEND ..\src\mimetic\strutils.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_utils.obj :  .AUTODEPEND ..\src\mimetic\utils.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_version.obj :  .AUTODEPEND ..\src\mimetic\version.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_base64.obj :  .AUTODEPEND ..\src\mimetic\codec\base64.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_qp.obj :  .AUTODEPEND ..\src\mimetic\codec\qp.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_address.obj :  .AUTODEPEND ..\src\mimetic\rfc822\address.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_addresslist.obj :  .AUTODEPEND ..\src\mimetic\rfc822\addresslist.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_datetime.obj :  .AUTODEPEND ..\src\mimetic\rfc822\datetime.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_field.obj :  .AUTODEPEND ..\src\mimetic\rfc822\field.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_fieldvalue.obj :  .AUTODEPEND ..\src\mimetic\rfc822\fieldvalue.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_group.obj :  .AUTODEPEND ..\src\mimetic\rfc822\group.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_mailbox.obj :  .AUTODEPEND ..\src\mimetic\rfc822\mailbox.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_mailboxlist.obj :  .AUTODEPEND ..\src\mimetic\rfc822\mailboxlist.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_messageid.obj :  .AUTODEPEND ..\src\mimetic\rfc822\messageid.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_rheader.obj :  .AUTODEPEND ..\src\mimetic\rfc822\rheader.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_rmessage.obj :  .AUTODEPEND ..\src\mimetic\rfc822\rmessage.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxpop3.obj :  .AUTODEPEND ..\src\pop3\wxpop3.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxpop3states.obj :  .AUTODEPEND ..\src\pop3\wxpop3states.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxsmtp.obj :  .AUTODEPEND ..\src\smtp\wxsmtp.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxsmtpemail.obj :  .AUTODEPEND ..\src\smtp\wxsmtpemail.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxsmtpstates.obj :  .AUTODEPEND ..\src\smtp\wxsmtpstates.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxSSLSocketClient.obj :  .AUTODEPEND ..\src\ssl\wxSSLSocketClient.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxcmdprot.obj :  .AUTODEPEND ..\src\utils\wxcmdprot.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_lib_wxmd5.obj :  .AUTODEPEND ..\src\utils\wxmd5.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxemail.obj :  .AUTODEPEND ..\src\wxemail.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_charsetconv.obj :  .AUTODEPEND ..\src\codec\charsetconv.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_rfc2047.obj :  .AUTODEPEND ..\src\codec\rfc2047.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_rfc2231.obj :  .AUTODEPEND ..\src\codec\rfc2231.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_body.obj :  .AUTODEPEND ..\src\mimetic\body.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_contentdescription.obj :  .AUTODEPEND ..\src\mimetic\contentdescription.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_contentdisposition.obj :  .AUTODEPEND ..\src\mimetic\contentdisposition.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_contentid.obj :  .AUTODEPEND ..\src\mimetic\contentid.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_contenttransferencoding.obj :  .AUTODEPEND ..\src\mimetic\contenttransferencoding.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_contenttype.obj :  .AUTODEPEND ..\src\mimetic\contenttype.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_fieldparam.obj :  .AUTODEPEND ..\src\mimetic\fieldparam.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_header.obj :  .AUTODEPEND ..\src\mimetic\header.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_message.obj :  .AUTODEPEND ..\src\mimetic\message.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_mimeentity.obj :  .AUTODEPEND ..\src\mimetic\mimeentity.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_mimeversion.obj :  .AUTODEPEND ..\src\mimetic\mimeversion.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_strutils.obj :  .AUTODEPEND ..\src\mimetic\strutils.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_utils.obj :  .AUTODEPEND ..\src\mimetic\utils.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_version.obj :  .AUTODEPEND ..\src\mimetic\version.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_base64.obj :  .AUTODEPEND ..\src\mimetic\codec\base64.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_qp.obj :  .AUTODEPEND ..\src\mimetic\codec\qp.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_address.obj :  .AUTODEPEND ..\src\mimetic\rfc822\address.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_addresslist.obj :  .AUTODEPEND ..\src\mimetic\rfc822\addresslist.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_datetime.obj :  .AUTODEPEND ..\src\mimetic\rfc822\datetime.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_field.obj :  .AUTODEPEND ..\src\mimetic\rfc822\field.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_fieldvalue.obj :  .AUTODEPEND ..\src\mimetic\rfc822\fieldvalue.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_group.obj :  .AUTODEPEND ..\src\mimetic\rfc822\group.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_mailbox.obj :  .AUTODEPEND ..\src\mimetic\rfc822\mailbox.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_mailboxlist.obj :  .AUTODEPEND ..\src\mimetic\rfc822\mailboxlist.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_messageid.obj :  .AUTODEPEND ..\src\mimetic\rfc822\messageid.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_rheader.obj :  .AUTODEPEND ..\src\mimetic\rfc822\rheader.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_rmessage.obj :  .AUTODEPEND ..\src\mimetic\rfc822\rmessage.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxpop3.obj :  .AUTODEPEND ..\src\pop3\wxpop3.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxpop3states.obj :  .AUTODEPEND ..\src\pop3\wxpop3states.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxsmtp.obj :  .AUTODEPEND ..\src\smtp\wxsmtp.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxsmtpemail.obj :  .AUTODEPEND ..\src\smtp\wxsmtpemail.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxsmtpstates.obj :  .AUTODEPEND ..\src\smtp\wxsmtpstates.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxSSLSocketClient.obj :  .AUTODEPEND ..\src\ssl\wxSSLSocketClient.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxcmdprot.obj :  .AUTODEPEND ..\src\utils\wxcmdprot.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMail_dll_wxmd5.obj :  .AUTODEPEND ..\src\utils\wxmd5.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAIL_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailSmtp_SendMail.obj :  .AUTODEPEND ..\samples\SendMail\SendMail.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAILSMTP_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxEMailPop3_Pop3Client.obj :  .AUTODEPEND ..\samples\Pop3Client\Pop3Client.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXEMAILPOP3_CXXFLAGS) $<

