# Microsoft Developer Studio Project File - Name="wxplotctrl_plotctrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=plotctrl - Win32 Static ANSI Release Multilib
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxplotctrl_plotctrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxplotctrl_plotctrl.mak" CFG="plotctrl - Win32 Static ANSI Release Multilib"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "plotctrl - Win32 DLL Unicode Debug Monolithic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plotctrl - Win32 DLL Unicode Debug Multilib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plotctrl - Win32 DLL Unicode Release Monolithic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plotctrl - Win32 DLL Unicode Release Multilib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plotctrl - Win32 DLL ANSI Debug Monolithic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plotctrl - Win32 DLL ANSI Debug Multilib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plotctrl - Win32 DLL ANSI Release Monolithic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plotctrl - Win32 DLL ANSI Release Multilib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plotctrl - Win32 Static Unicode Debug Monolithic" (based on "Win32 (x86) Static Library")
!MESSAGE "plotctrl - Win32 Static Unicode Debug Multilib" (based on "Win32 (x86) Static Library")
!MESSAGE "plotctrl - Win32 Static Unicode Release Monolithic" (based on "Win32 (x86) Static Library")
!MESSAGE "plotctrl - Win32 Static Unicode Release Multilib" (based on "Win32 (x86) Static Library")
!MESSAGE "plotctrl - Win32 Static ANSI Debug Monolithic" (based on "Win32 (x86) Static Library")
!MESSAGE "plotctrl - Win32 Static ANSI Debug Multilib" (based on "Win32 (x86) Static Library")
!MESSAGE "plotctrl - Win32 Static ANSI Release Monolithic" (based on "Win32 (x86) Static Library")
!MESSAGE "plotctrl - Win32 Static ANSI Release Multilib" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "plotctrl - Win32 DLL Unicode Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud_dll\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud_dll\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw28ud_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw28ud_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PLOTCTRL
# ADD RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PLOTCTRL
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_plotctrl.lib" /debug
# ADD LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_plotctrl.lib" /debug

!ELSEIF  "$(CFG)" == "plotctrl - Win32 DLL Unicode Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud_dll\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud_dll\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw28ud_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw28ud_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PLOTCTRL
# ADD RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PLOTCTRL
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28ud_adv.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_plotctrl.lib" /debug
# ADD LINK32 wxmsw28ud_adv.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_plotctrl.lib" /debug

!ELSEIF  "$(CFG)" == "plotctrl - Win32 DLL Unicode Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu_dll\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu_dll\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PLOTCTRL
# ADD RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PLOTCTRL
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_plotctrl.lib"
# ADD LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_plotctrl.lib"

!ELSEIF  "$(CFG)" == "plotctrl - Win32 DLL Unicode Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu_dll\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu_dll\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PLOTCTRL
# ADD RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PLOTCTRL
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28u_adv.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_plotctrl.lib"
# ADD LINK32 wxmsw28u_adv.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_plotctrl.lib"

!ELSEIF  "$(CFG)" == "plotctrl - Win32 DLL ANSI Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd_dll\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd_dll\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw28d_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw28d_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PLOTCTRL
# ADD RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PLOTCTRL
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_plotctrl.lib" /debug
# ADD LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_plotctrl.lib" /debug

!ELSEIF  "$(CFG)" == "plotctrl - Win32 DLL ANSI Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd_dll\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd_dll\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw28d_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_dll\wxcode_msw28d_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PLOTCTRL
# ADD RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PLOTCTRL
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d_adv.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_plotctrl.lib" /debug
# ADD LINK32 wxmsw28d_adv.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_plotctrl.lib" /debug

!ELSEIF  "$(CFG)" == "plotctrl - Win32 DLL ANSI Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw_dll\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw_dll\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PLOTCTRL
# ADD RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PLOTCTRL
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_plotctrl.lib"
# ADD LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_plotctrl.lib"

!ELSEIF  "$(CFG)" == "plotctrl - Win32 DLL ANSI Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw_dll\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw_dll\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_plotctrl.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PLOTCTRL" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PLOTCTRL
# ADD RSC /l 0x409 /i "$(WXCODE)/components/wxthings\include" /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PLOTCTRL
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28_adv.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_plotctrl.lib"
# ADD LINK32 wxmsw28_adv.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_plotctrl.dll" /libpath:"$(WXCODE)/components/wxthings\lib\vc_dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_plotctrl.lib"

!ELSEIF  "$(CFG)" == "plotctrl - Win32 Static Unicode Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswud\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswud\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw28ud_plotctrl.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw28ud_plotctrl.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28ud_plotctrl.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28ud_plotctrl.lib"

!ELSEIF  "$(CFG)" == "plotctrl - Win32 Static Unicode Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswud\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswud\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw28ud_plotctrl.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw28ud_plotctrl.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28ud_plotctrl.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28ud_plotctrl.lib"

!ELSEIF  "$(CFG)" == "plotctrl - Win32 Static Unicode Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswu\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswu\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28u_plotctrl.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28u_plotctrl.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28u_plotctrl.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28u_plotctrl.lib"

!ELSEIF  "$(CFG)" == "plotctrl - Win32 Static Unicode Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswu\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswu\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28u_plotctrl.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28u_plotctrl.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28u_plotctrl.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28u_plotctrl.lib"

!ELSEIF  "$(CFG)" == "plotctrl - Win32 Static ANSI Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswd\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswd\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw28d_plotctrl.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw28d_plotctrl.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28d_plotctrl.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28d_plotctrl.lib"

!ELSEIF  "$(CFG)" == "plotctrl - Win32 Static ANSI Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswd\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswd\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw28d_plotctrl.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /Zi /Gm /GZ /Fd..\lib\vc_lib\wxcode_msw28d_plotctrl.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28d_plotctrl.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28d_plotctrl.lib"

!ELSEIF  "$(CFG)" == "plotctrl - Win32 Static ANSI Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmsw\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmsw\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28_plotctrl.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28_plotctrl.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28_plotctrl.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28_plotctrl.lib"

!ELSEIF  "$(CFG)" == "plotctrl - Win32 Static ANSI Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmsw\plotctrl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmsw\plotctrl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28_plotctrl.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXCODE)/components/wxthings\include" /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28_plotctrl.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28_plotctrl.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28_plotctrl.lib"

!ENDIF

# Begin Target

# Name "plotctrl - Win32 DLL Unicode Debug Monolithic"
# Name "plotctrl - Win32 DLL Unicode Debug Multilib"
# Name "plotctrl - Win32 DLL Unicode Release Monolithic"
# Name "plotctrl - Win32 DLL Unicode Release Multilib"
# Name "plotctrl - Win32 DLL ANSI Debug Monolithic"
# Name "plotctrl - Win32 DLL ANSI Debug Multilib"
# Name "plotctrl - Win32 DLL ANSI Release Monolithic"
# Name "plotctrl - Win32 DLL ANSI Release Multilib"
# Name "plotctrl - Win32 Static Unicode Debug Monolithic"
# Name "plotctrl - Win32 Static Unicode Debug Multilib"
# Name "plotctrl - Win32 Static Unicode Release Monolithic"
# Name "plotctrl - Win32 Static Unicode Release Multilib"
# Name "plotctrl - Win32 Static ANSI Debug Monolithic"
# Name "plotctrl - Win32 Static ANSI Debug Multilib"
# Name "plotctrl - Win32 Static ANSI Release Monolithic"
# Name "plotctrl - Win32 Static ANSI Release Multilib"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\fourier.c
# End Source File
# Begin Source File

SOURCE=..\src\fparser.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fpoptimizer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\lm_lsqr.cpp
# End Source File
# Begin Source File

SOURCE=..\src\plotctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\src\plotcurv.cpp
# End Source File
# Begin Source File

SOURCE=..\src\plotdata.cpp
# End Source File
# Begin Source File

SOURCE=..\src\plotdraw.cpp
# End Source File
# Begin Source File

SOURCE=..\src\plotfunc.cpp
# End Source File
# Begin Source File

SOURCE=..\src\plotmark.cpp
# End Source File
# Begin Source File

SOURCE=..\src\plotprnt.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\wx\plotctrl\fourier.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\plotctrl\fparser.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\plotctrl\lm_lsqr.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\plotctrl\plotctrl.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\plotctrl\plotcurv.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\plotctrl\plotdata.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\plotctrl\plotdefs.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\plotctrl\plotdraw.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\plotctrl\plotfunc.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\plotctrl\plotmark.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\plotctrl\plotprnt.h
# End Source File
# End Group
# End Target
# End Project

