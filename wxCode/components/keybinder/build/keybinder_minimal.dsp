# Microsoft Developer Studio Project File - Name="keybinder_minimal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=minimal - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "keybinder_minimal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "keybinder_minimal.mak" CFG="minimal - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "minimal - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "minimal - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "minimal - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "minimal - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "minimal - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\sample"
# PROP BASE Intermediate_Dir "msvc6prj\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\sample"
# PROP Intermediate_Dir "msvc6prj\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /Fd..\sample\minimal.pdb /D "WIN32" /D "_UNICODE" /D "_WINDOWS" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /Fd..\sample\minimal.pdb /D "WIN32" /D "_UNICODE" /D "_WINDOWS" /D "__WXMSW__" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "_WINDOWS" /D "__WXMSW__" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "_WINDOWS" /D "__WXMSW__" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /d "_WINDOWS" /i "..\sample" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /d "_WINDOWS" /i "..\sample" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\keybinderu.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\sample\minimal.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /subsystem:windows
# ADD LINK32 ..\lib\keybinderu.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\sample\minimal.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "minimal - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\sample"
# PROP BASE Intermediate_Dir "msvc6prj\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\sample"
# PROP Intermediate_Dir "msvc6prj\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\sample\minimal.pdb /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "_WINDOWS" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\sample\minimal.pdb /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "_WINDOWS" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "_WINDOWS" /D "__WXDEBUG__" /D "__WXMSW__" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "_WINDOWS" /D "__WXDEBUG__" /D "__WXMSW__" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /d "_WINDOWS" /i "..\sample" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /d "_WINDOWS" /i "..\sample" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\keybinderud.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexud.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\sample\minimal.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /subsystem:windows /debug
# ADD LINK32 ..\lib\keybinderud.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexud.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\sample\minimal.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /subsystem:windows /debug

!ELSEIF  "$(CFG)" == "minimal - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\sample"
# PROP BASE Intermediate_Dir "msvc6prj\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\sample"
# PROP Intermediate_Dir "msvc6prj\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /Fd..\sample\minimal.pdb /D "WIN32" /D "_WINDOWS" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /Fd..\sample\minimal.pdb /D "WIN32" /D "_WINDOWS" /D "__WXMSW__" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /D "__WXMSW__" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /D "__WXMSW__" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /d "_WINDOWS" /i "..\sample" /d __WXMSW__
# ADD RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /d "_WINDOWS" /i "..\sample" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\keybinder.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\sample\minimal.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /subsystem:windows
# ADD LINK32 ..\lib\keybinder.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\sample\minimal.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "minimal - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\sample"
# PROP BASE Intermediate_Dir "msvc6prj\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\sample"
# PROP Intermediate_Dir "msvc6prj\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\sample\minimal.pdb /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\sample\minimal.pdb /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__WXDEBUG__" /D "__WXMSW__" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__WXDEBUG__" /D "__WXMSW__" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /d "_WINDOWS" /i "..\sample" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /d "_WINDOWS" /i "..\sample" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\keybinderd.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\sample\minimal.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /subsystem:windows /debug
# ADD LINK32 ..\lib\keybinderd.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\sample\minimal.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /subsystem:windows /debug

!ENDIF

# Begin Target

# Name "minimal - Win32 Unicode Release"
# Name "minimal - Win32 Unicode Debug"
# Name "minimal - Win32 Release"
# Name "minimal - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\sample\minimal.cpp
# End Source File
# Begin Source File

SOURCE=.\..\sample\minimal.rc
# End Source File
# End Group
# End Target
# End Project

