# Microsoft Developer Studio Project File - Name="databaselayer_databaselayer_odbc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=databaselayer_odbc - Win32 Static ANSI Release Multilib Static
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "databaselayer_databaselayer_odbc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "databaselayer_databaselayer_odbc.mak" CFG="databaselayer_odbc - Win32 Static ANSI Release Multilib Static"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "databaselayer_odbc - Win32 DLL Unicode Debug Monolithic DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL Unicode Debug Monolithic Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL Unicode Debug Multilib DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL Unicode Debug Multilib Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL Unicode Release Monolithic DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL Unicode Release Monolithic Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL Unicode Release Multilib DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL Unicode Release Multilib Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL ANSI Debug Monolithic DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL ANSI Debug Monolithic Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL ANSI Debug Multilib DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL ANSI Debug Multilib Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL ANSI Release Monolithic DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL ANSI Release Monolithic Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL ANSI Release Multilib DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 DLL ANSI Release Multilib Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "databaselayer_odbc - Win32 Static Unicode Debug Monolithic DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static Unicode Debug Monolithic Static" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static Unicode Debug Multilib DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static Unicode Debug Multilib Static" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static Unicode Release Monolithic DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static Unicode Release Monolithic Static" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static Unicode Release Multilib DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static Unicode Release Multilib Static" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static ANSI Debug Monolithic DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static ANSI Debug Monolithic Static" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static ANSI Debug Multilib DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static ANSI Debug Multilib Static" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static ANSI Release Monolithic DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static ANSI Release Monolithic Static" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static ANSI Release Multilib DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "databaselayer_odbc - Win32 Static ANSI Release Multilib Static" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "databaselayer_odbc - Win32 DLL Unicode Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb"
# ADD LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL Unicode Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb"
# ADD LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL Unicode Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb"
# ADD LINK32 wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL Unicode Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb"
# ADD LINK32 wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL Unicode Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb"
# ADD LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL Unicode Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb"
# ADD LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL Unicode Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb"
# ADD LINK32 wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL Unicode Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb"
# ADD LINK32 wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL ANSI Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb"
# ADD LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL ANSI Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb"
# ADD LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL ANSI Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb"
# ADD LINK32 wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL ANSI Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d "WIN32COMMON" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb"
# ADD LINK32 wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL ANSI Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb"
# ADD LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL ANSI Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb"
# ADD LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL ANSI Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb"
# ADD LINK32 wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 DLL ANSI Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_DATABASELAYER" /D "WIN32COMMON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_DATABASELAYER" /d WIN32COMMON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb"
# ADD LINK32 wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static Unicode Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static Unicode Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswud\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswud\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28ud_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28ud_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static Unicode Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28ud_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static Unicode Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswud\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswud\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28ud_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28ud_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28ud_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static Unicode Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static Unicode Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswu\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswu\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28u_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28u_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static Unicode Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28u_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static Unicode Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswu\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswu\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28u_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28u_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28u_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static ANSI Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static ANSI Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswd\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswd\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28d_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28d_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static ANSI Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28d_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static ANSI Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswd\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswd\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28d_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32COMMON" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28d_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28d_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static ANSI Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static ANSI Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmsw\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmsw\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static ANSI Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw_dll\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw_dll\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28_databaselayer_odbc.lib"

!ELSEIF  "$(CFG)" == "databaselayer_odbc - Win32 Static ANSI Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmsw\databaselayer_odbc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmsw\databaselayer_odbc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28_databaselayer_odbc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "WIN32COMMON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28_databaselayer_odbc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28_databaselayer_odbc.lib"

!ENDIF

# Begin Target

# Name "databaselayer_odbc - Win32 DLL Unicode Debug Monolithic DLL"
# Name "databaselayer_odbc - Win32 DLL Unicode Debug Monolithic Static"
# Name "databaselayer_odbc - Win32 DLL Unicode Debug Multilib DLL"
# Name "databaselayer_odbc - Win32 DLL Unicode Debug Multilib Static"
# Name "databaselayer_odbc - Win32 DLL Unicode Release Monolithic DLL"
# Name "databaselayer_odbc - Win32 DLL Unicode Release Monolithic Static"
# Name "databaselayer_odbc - Win32 DLL Unicode Release Multilib DLL"
# Name "databaselayer_odbc - Win32 DLL Unicode Release Multilib Static"
# Name "databaselayer_odbc - Win32 DLL ANSI Debug Monolithic DLL"
# Name "databaselayer_odbc - Win32 DLL ANSI Debug Monolithic Static"
# Name "databaselayer_odbc - Win32 DLL ANSI Debug Multilib DLL"
# Name "databaselayer_odbc - Win32 DLL ANSI Debug Multilib Static"
# Name "databaselayer_odbc - Win32 DLL ANSI Release Monolithic DLL"
# Name "databaselayer_odbc - Win32 DLL ANSI Release Monolithic Static"
# Name "databaselayer_odbc - Win32 DLL ANSI Release Multilib DLL"
# Name "databaselayer_odbc - Win32 DLL ANSI Release Multilib Static"
# Name "databaselayer_odbc - Win32 Static Unicode Debug Monolithic DLL"
# Name "databaselayer_odbc - Win32 Static Unicode Debug Monolithic Static"
# Name "databaselayer_odbc - Win32 Static Unicode Debug Multilib DLL"
# Name "databaselayer_odbc - Win32 Static Unicode Debug Multilib Static"
# Name "databaselayer_odbc - Win32 Static Unicode Release Monolithic DLL"
# Name "databaselayer_odbc - Win32 Static Unicode Release Monolithic Static"
# Name "databaselayer_odbc - Win32 Static Unicode Release Multilib DLL"
# Name "databaselayer_odbc - Win32 Static Unicode Release Multilib Static"
# Name "databaselayer_odbc - Win32 Static ANSI Debug Monolithic DLL"
# Name "databaselayer_odbc - Win32 Static ANSI Debug Monolithic Static"
# Name "databaselayer_odbc - Win32 Static ANSI Debug Multilib DLL"
# Name "databaselayer_odbc - Win32 Static ANSI Debug Multilib Static"
# Name "databaselayer_odbc - Win32 Static ANSI Release Monolithic DLL"
# Name "databaselayer_odbc - Win32 Static ANSI Release Monolithic Static"
# Name "databaselayer_odbc - Win32 Static ANSI Release Multilib DLL"
# Name "databaselayer_odbc - Win32 Static ANSI Release Multilib Static"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\DatabaseErrorReporter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\DatabaseLayer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\DatabaseQueryParser.cpp
# End Source File
# Begin Source File

SOURCE=..\src\DatabaseResultSet.cpp
# End Source File
# Begin Source File

SOURCE=..\src\DatabaseStringConverter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OdbcDatabaseLayer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OdbcInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OdbcParameter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OdbcPreparedStatement.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OdbcResultSet.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OdbcResultSetMetaData.cpp
# End Source File
# Begin Source File

SOURCE=..\src\PreparedStatement.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\DatabaseErrorCodes.h
# End Source File
# Begin Source File

SOURCE=..\include\DatabaseErrorReporter.h
# End Source File
# Begin Source File

SOURCE=..\include\DatabaseLayer.h
# End Source File
# Begin Source File

SOURCE=..\include\DatabaseLayerDef.h
# End Source File
# Begin Source File

SOURCE=..\include\DatabaseLayerException.h
# End Source File
# Begin Source File

SOURCE=..\include\DatabaseQueryParser.h
# End Source File
# Begin Source File

SOURCE=..\include\DatabaseResultSet.h
# End Source File
# Begin Source File

SOURCE=..\include\DatabaseStringConverter.h
# End Source File
# Begin Source File

SOURCE=..\include\OdbcDatabaseLayer.h
# End Source File
# Begin Source File

SOURCE=..\include\OdbcInterface.h
# End Source File
# Begin Source File

SOURCE=..\include\OdbcParameter.h
# End Source File
# Begin Source File

SOURCE=..\include\OdbcPreparedStatement.h
# End Source File
# Begin Source File

SOURCE=..\include\OdbcResultSet.h
# End Source File
# Begin Source File

SOURCE=..\include\OdbcResultSetMetaData.h
# End Source File
# Begin Source File

SOURCE=..\include\PreparedStatement.h
# End Source File
# Begin Source File

SOURCE=..\include\ResultSetMetaData.h
# End Source File
# End Group
# End Target
# End Project

