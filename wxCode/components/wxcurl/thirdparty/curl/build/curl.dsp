# Microsoft Developer Studio Project File - Name="curl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libcurl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "curl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "curl.mak" CFG="libcurl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libcurl - Win32 DLL Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libcurl - Win32 DLL Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libcurl - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libcurl - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libcurl - Win32 DLL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "Release\libcurl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "Release\libcurl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "..\include" /I "..\lib" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "..\include" /I "..\lib" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "..\include" /i ..\lib
# ADD RSC /l 0x409 /i "..\include" /i ..\lib
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:i386 /out:"..\lib\libcurl.dll" /implib:"..\lib\libcurl.lib"
# ADD LINK32 /nologo /dll /machine:i386 /out:"..\lib\libcurl.dll" /implib:"..\lib\libcurl.lib"

!ELSEIF  "$(CFG)" == "libcurl - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "Release\libcurl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "Release\libcurl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "..\include" /I "..\lib" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "NDEBUG" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "..\include" /I "..\lib" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "NDEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /i "..\include" /i ..\lib
# ADD RSC /l 0x409 /d "NDEBUG" /i "..\include" /i ..\lib
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:i386 /out:"..\lib\libcurl.dll" /implib:"..\lib\libcurl.lib"
# ADD LINK32 /nologo /dll /machine:i386 /out:"..\lib\libcurl.dll" /implib:"..\lib\libcurl.lib"

!ELSEIF  "$(CFG)" == "libcurl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "Release\libcurl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "Release\libcurl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "..\include" /I "..\lib" /D "WIN32" /D "_LIB" /D "CURL_STATICLIB" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "..\include" /I "..\lib" /D "WIN32" /D "_LIB" /D "CURL_STATICLIB" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\libcurl.lib"
# ADD LIB32 /nologo /out:"..\lib\libcurl.lib"

!ELSEIF  "$(CFG)" == "libcurl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "Release\libcurl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "Release\libcurl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "..\include" /I "..\lib" /D "WIN32" /D "_LIB" /D "CURL_STATICLIB" /D "NDEBUG" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "..\include" /I "..\lib" /D "WIN32" /D "_LIB" /D "CURL_STATICLIB" /D "NDEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\libcurl.lib"
# ADD LIB32 /nologo /out:"..\lib\libcurl.lib"

!ENDIF

# Begin Target

# Name "libcurl - Win32 DLL Release"
# Name "libcurl - Win32 DLL Debug"
# Name "libcurl - Win32 Release"
# Name "libcurl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\lib\base64.c
# End Source File
# Begin Source File

SOURCE=..\lib\connect.c
# End Source File
# Begin Source File

SOURCE=..\lib\content_encoding.c
# End Source File
# Begin Source File

SOURCE=..\lib\cookie.c
# End Source File
# Begin Source File

SOURCE=..\lib\dict.c
# End Source File
# Begin Source File

SOURCE=..\lib\easy.c
# End Source File
# Begin Source File

SOURCE=..\lib\escape.c
# End Source File
# Begin Source File

SOURCE=..\lib\file.c
# End Source File
# Begin Source File

SOURCE=..\lib\formdata.c
# End Source File
# Begin Source File

SOURCE=..\lib\ftp.c
# End Source File
# Begin Source File

SOURCE=..\lib\getenv.c
# End Source File
# Begin Source File

SOURCE=..\lib\getinfo.c
# End Source File
# Begin Source File

SOURCE=..\lib\gtls.c
# End Source File
# Begin Source File

SOURCE=..\lib\hash.c
# End Source File
# Begin Source File

SOURCE=..\lib\hostares.c
# End Source File
# Begin Source File

SOURCE=..\lib\hostasyn.c
# End Source File
# Begin Source File

SOURCE=..\lib\hostip.c
# End Source File
# Begin Source File

SOURCE=..\lib\hostip4.c
# End Source File
# Begin Source File

SOURCE=..\lib\hostip6.c
# End Source File
# Begin Source File

SOURCE=..\lib\hostsyn.c
# End Source File
# Begin Source File

SOURCE=..\lib\hostthre.c
# End Source File
# Begin Source File

SOURCE=..\lib\http.c
# End Source File
# Begin Source File

SOURCE=..\lib\http_chunks.c
# End Source File
# Begin Source File

SOURCE=..\lib\http_digest.c
# End Source File
# Begin Source File

SOURCE=..\lib\http_negotiate.c
# End Source File
# Begin Source File

SOURCE=..\lib\http_ntlm.c
# End Source File
# Begin Source File

SOURCE=..\lib\if2ip.c
# End Source File
# Begin Source File

SOURCE=..\lib\inet_ntop.c
# End Source File
# Begin Source File

SOURCE=..\lib\inet_pton.c
# End Source File
# Begin Source File

SOURCE=..\lib\krb4.c
# End Source File
# Begin Source File

SOURCE=..\lib\ldap.c
# End Source File
# Begin Source File

SOURCE=..\lib\llist.c
# End Source File
# Begin Source File

SOURCE=..\lib\md5.c
# End Source File
# Begin Source File

SOURCE=..\lib\memdebug.c
# End Source File
# Begin Source File

SOURCE=..\lib\mprintf.c
# End Source File
# Begin Source File

SOURCE=..\lib\multi.c
# End Source File
# Begin Source File

SOURCE=..\lib\netrc.c
# End Source File
# Begin Source File

SOURCE=..\lib\nwlib.c
# End Source File
# Begin Source File

SOURCE=..\lib\parsedate.c
# End Source File
# Begin Source File

SOURCE=..\lib\progress.c
# End Source File
# Begin Source File

SOURCE=..\lib\security.c
# End Source File
# Begin Source File

SOURCE=..\lib\select.c
# End Source File
# Begin Source File

SOURCE=..\lib\sendf.c
# End Source File
# Begin Source File

SOURCE=..\lib\share.c
# End Source File
# Begin Source File

SOURCE=..\lib\socks.c
# End Source File
# Begin Source File

SOURCE=..\lib\speedcheck.c
# End Source File
# Begin Source File

SOURCE=..\lib\splay.c
# End Source File
# Begin Source File

SOURCE=..\lib\ssh.c
# End Source File
# Begin Source File

SOURCE=..\lib\sslgen.c
# End Source File
# Begin Source File

SOURCE=..\lib\ssluse.c
# End Source File
# Begin Source File

SOURCE=..\lib\strdup.c
# End Source File
# Begin Source File

SOURCE=..\lib\strequal.c
# End Source File
# Begin Source File

SOURCE=..\lib\strerror.c
# End Source File
# Begin Source File

SOURCE=..\lib\strtok.c
# End Source File
# Begin Source File

SOURCE=..\lib\strtoofft.c
# End Source File
# Begin Source File

SOURCE=..\lib\telnet.c
# End Source File
# Begin Source File

SOURCE=..\lib\tftp.c
# End Source File
# Begin Source File

SOURCE=..\lib\timeval.c
# End Source File
# Begin Source File

SOURCE=..\lib\transfer.c
# End Source File
# Begin Source File

SOURCE=..\lib\url.c
# End Source File
# Begin Source File

SOURCE=..\lib\version.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\lib\amigaos.h
# End Source File
# Begin Source File

SOURCE=..\lib\arpa_telnet.h
# End Source File
# Begin Source File

SOURCE=..\lib\base64.h
# End Source File
# Begin Source File

SOURCE=..\lib\ca-bundle.h
# End Source File
# Begin Source File

SOURCE=..\lib\config-amigaos.h
# End Source File
# Begin Source File

SOURCE=..\lib\config-mac.h
# End Source File
# Begin Source File

SOURCE=..\lib\config-riscos.h
# End Source File
# Begin Source File

SOURCE=..\lib\config-tpf.h
# End Source File
# Begin Source File

SOURCE=..\lib\config-win32.h
# End Source File
# Begin Source File

SOURCE=..\lib\config-win32ce.h
# End Source File
# Begin Source File

SOURCE=..\lib\connect.h
# End Source File
# Begin Source File

SOURCE=..\lib\content_encoding.h
# End Source File
# Begin Source File

SOURCE=..\lib\cookie.h
# End Source File
# Begin Source File

SOURCE=..\include\curl\curl.h
# End Source File
# Begin Source File

SOURCE=..\include\curl\curlver.h
# End Source File
# Begin Source File

SOURCE=..\lib\curlx.h
# End Source File
# Begin Source File

SOURCE=..\lib\dict.h
# End Source File
# Begin Source File

SOURCE=..\include\curl\easy.h
# End Source File
# Begin Source File

SOURCE=..\lib\easyif.h
# End Source File
# Begin Source File

SOURCE=..\lib\escape.h
# End Source File
# Begin Source File

SOURCE=..\lib\file.h
# End Source File
# Begin Source File

SOURCE=..\lib\formdata.h
# End Source File
# Begin Source File

SOURCE=..\lib\ftp.h
# End Source File
# Begin Source File

SOURCE=..\lib\getinfo.h
# End Source File
# Begin Source File

SOURCE=..\lib\gtls.h
# End Source File
# Begin Source File

SOURCE=..\lib\hash.h
# End Source File
# Begin Source File

SOURCE=..\lib\hostip.h
# End Source File
# Begin Source File

SOURCE=..\lib\http.h
# End Source File
# Begin Source File

SOURCE=..\lib\http_chunks.h
# End Source File
# Begin Source File

SOURCE=..\lib\http_digest.h
# End Source File
# Begin Source File

SOURCE=..\lib\http_negotiate.h
# End Source File
# Begin Source File

SOURCE=..\lib\http_ntlm.h
# End Source File
# Begin Source File

SOURCE=..\lib\if2ip.h
# End Source File
# Begin Source File

SOURCE=..\lib\inet_ntoa_r.h
# End Source File
# Begin Source File

SOURCE=..\lib\inet_ntop.h
# End Source File
# Begin Source File

SOURCE=..\lib\inet_pton.h
# End Source File
# Begin Source File

SOURCE=..\lib\krb4.h
# End Source File
# Begin Source File

SOURCE=..\lib\ldap.h
# End Source File
# Begin Source File

SOURCE=..\lib\llist.h
# End Source File
# Begin Source File

SOURCE=..\lib\md5.h
# End Source File
# Begin Source File

SOURCE=..\lib\memdebug.h
# End Source File
# Begin Source File

SOURCE=..\lib\memory.h
# End Source File
# Begin Source File

SOURCE=..\include\curl\mprintf.h
# End Source File
# Begin Source File

SOURCE=..\include\curl\multi.h
# End Source File
# Begin Source File

SOURCE=..\lib\multiif.h
# End Source File
# Begin Source File

SOURCE=..\lib\netrc.h
# End Source File
# Begin Source File

SOURCE=..\lib\parsedate.h
# End Source File
# Begin Source File

SOURCE=..\lib\progress.h
# End Source File
# Begin Source File

SOURCE=..\lib\select.h
# End Source File
# Begin Source File

SOURCE=..\lib\sendf.h
# End Source File
# Begin Source File

SOURCE=..\lib\setup.h
# End Source File
# Begin Source File

SOURCE=..\lib\setup_once.h
# End Source File
# Begin Source File

SOURCE=..\lib\share.h
# End Source File
# Begin Source File

SOURCE=..\lib\sockaddr.h
# End Source File
# Begin Source File

SOURCE=..\lib\socks.h
# End Source File
# Begin Source File

SOURCE=..\lib\speedcheck.h
# End Source File
# Begin Source File

SOURCE=..\lib\splay.h
# End Source File
# Begin Source File

SOURCE=..\lib\ssh.h
# End Source File
# Begin Source File

SOURCE=..\lib\sslgen.h
# End Source File
# Begin Source File

SOURCE=..\lib\ssluse.h
# End Source File
# Begin Source File

SOURCE=..\include\curl\stdcheaders.h
# End Source File
# Begin Source File

SOURCE=..\lib\strdup.h
# End Source File
# Begin Source File

SOURCE=..\lib\strequal.h
# End Source File
# Begin Source File

SOURCE=..\lib\strerror.h
# End Source File
# Begin Source File

SOURCE=..\lib\strtok.h
# End Source File
# Begin Source File

SOURCE=..\lib\strtoofft.h
# End Source File
# Begin Source File

SOURCE=..\lib\telnet.h
# End Source File
# Begin Source File

SOURCE=..\lib\tftp.h
# End Source File
# Begin Source File

SOURCE=..\lib\timeval.h
# End Source File
# Begin Source File

SOURCE=..\lib\transfer.h
# End Source File
# Begin Source File

SOURCE=..\include\curl\types.h
# End Source File
# Begin Source File

SOURCE=..\lib\url.h
# End Source File
# Begin Source File

SOURCE=..\lib\urldata.h
# End Source File
# End Group
# End Target
# End Project

