# Microsoft Developer Studio Project File - Name="wxxml2_wxxml2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=wxxml2 - Win32 Static ANSI Release Multilib
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxxml2_wxxml2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxxml2_wxxml2.mak" CFG="wxxml2 - Win32 Static ANSI Release Multilib"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxxml2 - Win32 DLL Unicode Debug Monolithic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxxml2 - Win32 DLL Unicode Debug Multilib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxxml2 - Win32 DLL Unicode Release Monolithic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxxml2 - Win32 DLL Unicode Release Multilib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxxml2 - Win32 DLL ANSI Debug Monolithic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxxml2 - Win32 DLL ANSI Debug Multilib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxxml2 - Win32 DLL ANSI Release Monolithic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxxml2 - Win32 DLL ANSI Release Multilib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxxml2 - Win32 Static Unicode Debug Monolithic" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Static Unicode Debug Multilib" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Static Unicode Release Monolithic" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Static Unicode Release Multilib" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Static ANSI Debug Monolithic" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Static ANSI Debug Multilib" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Static ANSI Release Monolithic" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Static ANSI Release Multilib" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxxml2 - Win32 DLL Unicode Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud_dll\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud_dll\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw290ud_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw290ud_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libxml2.lib iconv.lib wxmsw290ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290ud_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290ud_wxxml2.lib" /debug /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"
# ADD LINK32 libxml2.lib iconv.lib wxmsw290ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290ud_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290ud_wxxml2.lib" /debug /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 DLL Unicode Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud_dll\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud_dll\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw290ud_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw290ud_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libxml2.lib iconv.lib wxmsw290ud_core.lib wxbase290ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290ud_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290ud_wxxml2.lib" /debug /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"
# ADD LINK32 libxml2.lib iconv.lib wxmsw290ud_core.lib wxbase290ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290ud_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290ud_wxxml2.lib" /debug /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 DLL Unicode Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu_dll\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu_dll\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw290u_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw290u_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libxml2.lib iconv.lib wxmsw290u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290u_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290u_wxxml2.lib" /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"
# ADD LINK32 libxml2.lib iconv.lib wxmsw290u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290u_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290u_wxxml2.lib" /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 DLL Unicode Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu_dll\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu_dll\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw290u_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw290u_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libxml2.lib iconv.lib wxmsw290u_core.lib wxbase290u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290u_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290u_wxxml2.lib" /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"
# ADD LINK32 libxml2.lib iconv.lib wxmsw290u_core.lib wxbase290u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290u_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290u_wxxml2.lib" /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 DLL ANSI Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd_dll\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd_dll\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw290d_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw290d_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libxml2.lib iconv.lib wxmsw290d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290d_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290d_wxxml2.lib" /debug /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"
# ADD LINK32 libxml2.lib iconv.lib wxmsw290d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290d_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290d_wxxml2.lib" /debug /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 DLL ANSI Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd_dll\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd_dll\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw290d_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw290d_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libxml2.lib iconv.lib wxmsw290d_core.lib wxbase290d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290d_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290d_wxxml2.lib" /debug /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"
# ADD LINK32 libxml2.lib iconv.lib wxmsw290d_core.lib wxbase290d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290d_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290d_wxxml2.lib" /debug /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 DLL ANSI Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw_dll\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw_dll\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw290_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw290_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libxml2.lib iconv.lib wxmsw290.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290_wxxml2.lib" /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"
# ADD LINK32 libxml2.lib iconv.lib wxmsw290.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290_wxxml2.lib" /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 DLL ANSI Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw_dll\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw_dll\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw290_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw290_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXXML2" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXXML2" /i "..\thirdparty\libxml2\include" /i ..\thirdparty\libiconv\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libxml2.lib iconv.lib wxmsw290_core.lib wxbase290.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290_wxxml2.lib" /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"
# ADD LINK32 libxml2.lib iconv.lib wxmsw290_core.lib wxbase290.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw290_wxxml2.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw290_wxxml2.lib" /libpath:"..\thirdparty\libxml2\lib" /libpath:"..\thirdparty\libiconv\lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Static Unicode Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswud\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswud\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw290ud_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw290ud_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290ud_wxxml2.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290ud_wxxml2.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Static Unicode Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswud\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswud\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw290ud_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw290ud_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290ud_wxxml2.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290ud_wxxml2.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Static Unicode Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswu\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswu\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw290u_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw290u_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290u_wxxml2.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290u_wxxml2.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Static Unicode Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswu\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswu\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw290u_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw290u_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290u_wxxml2.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290u_wxxml2.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Static ANSI Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswd\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswd\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw290d_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw290d_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290d_wxxml2.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290d_wxxml2.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Static ANSI Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswd\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswd\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw290d_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw290d_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290d_wxxml2.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290d_wxxml2.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Static ANSI Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmsw\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmsw\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw290_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw290_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290_wxxml2.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290_wxxml2.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Static ANSI Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmsw\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmsw\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw290_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw290_wxxml2.pdb /I "..\thirdparty\libxml2\include" /I "..\thirdparty\libiconv\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290_wxxml2.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw290_wxxml2.lib"

!ENDIF

# Begin Target

# Name "wxxml2 - Win32 DLL Unicode Debug Monolithic"
# Name "wxxml2 - Win32 DLL Unicode Debug Multilib"
# Name "wxxml2 - Win32 DLL Unicode Release Monolithic"
# Name "wxxml2 - Win32 DLL Unicode Release Multilib"
# Name "wxxml2 - Win32 DLL ANSI Debug Monolithic"
# Name "wxxml2 - Win32 DLL ANSI Debug Multilib"
# Name "wxxml2 - Win32 DLL ANSI Release Monolithic"
# Name "wxxml2 - Win32 DLL ANSI Release Multilib"
# Name "wxxml2 - Win32 Static Unicode Debug Monolithic"
# Name "wxxml2 - Win32 Static Unicode Debug Multilib"
# Name "wxxml2 - Win32 Static Unicode Release Monolithic"
# Name "wxxml2 - Win32 Static Unicode Release Multilib"
# Name "wxxml2 - Win32 Static ANSI Debug Monolithic"
# Name "wxxml2 - Win32 Static ANSI Debug Multilib"
# Name "wxxml2 - Win32 Static ANSI Release Monolithic"
# Name "wxxml2 - Win32 Static ANSI Release Multilib"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\dtd.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xml2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\wx\dtd.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\xml2.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\xml2def.h
# End Source File
# End Group
# End Target
# End Project

