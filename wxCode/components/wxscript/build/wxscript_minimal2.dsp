# Microsoft Developer Studio Project File - Name="wxscript_minimal2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=minimal2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxscript_minimal2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxscript_minimal2.mak" CFG="minimal2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "minimal2 - Win32 Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal2 - Win32 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal2 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal2 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "minimal2 - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test2"
# PROP BASE Intermediate_Dir "msvc6prj\minimal2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test2"
# PROP Intermediate_Dir "msvc6prj\minimal2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O1 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Fd..\tests\test2\minimal2.pdb /D "WIN32" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O1 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Fd..\tests\test2\minimal2.pdb /D "WIN32" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint\include" /i "c:\ucc\include" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint\include" /i "c:\ucc\include" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptu.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\minimal2.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint\lib" /libpath:"c:\ucc\lib" /subsystem:console
# ADD LINK32 ..\lib\wxscriptu.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\minimal2.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint\lib" /libpath:"c:\ucc\lib" /subsystem:console

!ELSEIF  "$(CFG)" == "minimal2 - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test2"
# PROP BASE Intermediate_Dir "msvc6prj\minimal2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test2"
# PROP Intermediate_Dir "msvc6prj\minimal2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Zi /Gm /GZ /Fd..\tests\test2\minimal2.pdb /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Zi /Gm /GZ /Fd..\tests\test2\minimal2.pdb /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint\include" /i "c:\ucc\include" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint\include" /i "c:\ucc\include" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptud.lib python24_d.lib lua.lib lualib.lib tolua.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexud.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\minimal2.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint\lib" /libpath:"c:\ucc\lib" /debug /subsystem:console
# ADD LINK32 ..\lib\wxscriptud.lib python24_d.lib lua.lib lualib.lib tolua.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexud.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\minimal2.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint\lib" /libpath:"c:\ucc\lib" /debug /subsystem:console

!ELSEIF  "$(CFG)" == "minimal2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test2"
# PROP BASE Intermediate_Dir "msvc6prj\minimal2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test2"
# PROP Intermediate_Dir "msvc6prj\minimal2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O1 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Fd..\tests\test2\minimal2.pdb /D "WIN32" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O1 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Fd..\tests\test2\minimal2.pdb /D "WIN32" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint\include" /i "c:\ucc\include" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
# ADD RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint\include" /i "c:\ucc\include" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscript.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\minimal2.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint\lib" /libpath:"c:\ucc\lib" /subsystem:console
# ADD LINK32 ..\lib\wxscript.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\minimal2.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint\lib" /libpath:"c:\ucc\lib" /subsystem:console

!ELSEIF  "$(CFG)" == "minimal2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test2"
# PROP BASE Intermediate_Dir "msvc6prj\minimal2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test2"
# PROP Intermediate_Dir "msvc6prj\minimal2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Zi /Gm /GZ /Fd..\tests\test2\minimal2.pdb /D "WIN32" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Zi /Gm /GZ /Fd..\tests\test2\minimal2.pdb /D "WIN32" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint\include" /i "c:\ucc\include" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint\include" /i "c:\ucc\include" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptd.lib python24_d.lib lua.lib lualib.lib tolua.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\minimal2.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint\lib" /libpath:"c:\ucc\lib" /debug /subsystem:console
# ADD LINK32 ..\lib\wxscriptd.lib python24_d.lib lua.lib lualib.lib tolua.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\minimal2.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint\lib" /libpath:"c:\ucc\lib" /debug /subsystem:console

!ENDIF

# Begin Target

# Name "minimal2 - Win32 Unicode Release"
# Name "minimal2 - Win32 Unicode Debug"
# Name "minimal2 - Win32 Release"
# Name "minimal2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\tests\test2\Test.cpp
# End Source File
# Begin Source File

SOURCE=.\..\tests\test2\stackwalker.cpp
# End Source File
# End Group
# End Target
# End Project

