# Microsoft Developer Studio Project File - Name="wxscript_test1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=test1 - Win32 PYTHON_VER_2_2 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxscript_test1.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxscript_test1.mak" CFG="test1 - Win32 PYTHON_VER_2_2 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "test1 - Win32 PYTHON_VER_2_6 Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_6 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_6 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_6 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_5 Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_5 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_5 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_5 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_4 Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_4 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_4 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_4 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_3 Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_3 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_3 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_3 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_2 Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_2 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_2 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test1 - Win32 PYTHON_VER_2_2 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_6 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptu.lib python26.lib lua.lib lualib.lib tolua.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptu.lib python26.lib lua.lib lualib.lib tolua.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_6 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptud.lib python26_d.lib lua.lib lualib.lib tolua.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptud.lib python26_d.lib lua.lib lualib.lib tolua.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_6 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
# ADD RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscript.lib python26.lib lua.lib lualib.lib tolua.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscript.lib python26.lib lua.lib lualib.lib tolua.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_6 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptd.lib python26_d.lib lua.lib lualib.lib tolua.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptd.lib python26_d.lib lua.lib lualib.lib tolua.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_5 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptu.lib python25.lib lua.lib lualib.lib tolua.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptu.lib python25.lib lua.lib lualib.lib tolua.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_5 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptud.lib python25_d.lib lua.lib lualib.lib tolua.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptud.lib python25_d.lib lua.lib lualib.lib tolua.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_5 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
# ADD RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscript.lib python25.lib lua.lib lualib.lib tolua.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscript.lib python25.lib lua.lib lualib.lib tolua.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_5 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptd.lib python25_d.lib lua.lib lualib.lib tolua.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptd.lib python25_d.lib lua.lib lualib.lib tolua.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_4 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptu.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptu.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_4 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptud.lib python24_d.lib lua.lib lualib.lib tolua.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptud.lib python24_d.lib lua.lib lualib.lib tolua.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_4 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
# ADD RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscript.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscript.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_4 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptd.lib python24_d.lib lua.lib lualib.lib tolua.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptd.lib python24_d.lib lua.lib lualib.lib tolua.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_3 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptu.lib python23.lib lua.lib lualib.lib tolua.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptu.lib python23.lib lua.lib lualib.lib tolua.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_3 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptud.lib python23_d.lib lua.lib lualib.lib tolua.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptud.lib python23_d.lib lua.lib lualib.lib tolua.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_3 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
# ADD RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscript.lib python23.lib lua.lib lualib.lib tolua.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscript.lib python23.lib lua.lib lualib.lib tolua.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_3 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptd.lib python23_d.lib lua.lib lualib.lib tolua.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptd.lib python23_d.lib lua.lib lualib.lib tolua.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_2 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswu" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptu.lib python22.lib lua.lib lualib.lib tolua.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptu.lib python22.lib lua.lib lualib.lib tolua.lib wxmsw25u_core.lib wxmsw25u_html.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_2 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswud" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptud.lib python22_d.lib lua.lib lualib.lib tolua.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptud.lib python22_d.lib lua.lib lualib.lib tolua.lib wxmsw25ud_core.lib wxmsw25ud_html.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_2 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
# ADD RSC /l 0x409 /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\msw" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscript.lib python22.lib lua.lib lualib.lib tolua.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscript.lib python22.lib lua.lib lualib.lib tolua.lib wxmsw25_core.lib wxmsw25_html.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test1 - Win32 PYTHON_VER_2_2 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test1"
# PROP BASE Intermediate_Dir "msvc6prj\test1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test1"
# PROP Intermediate_Dir "msvc6prj\test1"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "..\include" /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\tests\test1\test1.pdb /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
# ADD RSC /l 0x409 /d "_DEBUG" /i "..\include" /i "c:\wxWidgets\include" /i "c:\wxWidgets\lib\vc_lib\mswd" /i "c:\Python\include" /i "c:\lua\include" /i "c:\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d "_CONSOLE" /d "__WXDEBUG__" /d __WXMSW__
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptd.lib python22_d.lib lua.lib lualib.lib tolua.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptd.lib python22_d.lib lua.lib lualib.lib tolua.lib wxmsw25d_core.lib wxmsw25d_html.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test1\test1.exe" /libpath:"..\lib" /libpath:"c:\wxWidgets\lib\vc_lib" /debug /libpath:"c:\Python\libs" /libpath:"c:\lua\lib" /libpath:"c:\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ENDIF

# Begin Target

# Name "test1 - Win32 PYTHON_VER_2_6 Unicode Release"
# Name "test1 - Win32 PYTHON_VER_2_6 Unicode Debug"
# Name "test1 - Win32 PYTHON_VER_2_6 Release"
# Name "test1 - Win32 PYTHON_VER_2_6 Debug"
# Name "test1 - Win32 PYTHON_VER_2_5 Unicode Release"
# Name "test1 - Win32 PYTHON_VER_2_5 Unicode Debug"
# Name "test1 - Win32 PYTHON_VER_2_5 Release"
# Name "test1 - Win32 PYTHON_VER_2_5 Debug"
# Name "test1 - Win32 PYTHON_VER_2_4 Unicode Release"
# Name "test1 - Win32 PYTHON_VER_2_4 Unicode Debug"
# Name "test1 - Win32 PYTHON_VER_2_4 Release"
# Name "test1 - Win32 PYTHON_VER_2_4 Debug"
# Name "test1 - Win32 PYTHON_VER_2_3 Unicode Release"
# Name "test1 - Win32 PYTHON_VER_2_3 Unicode Debug"
# Name "test1 - Win32 PYTHON_VER_2_3 Release"
# Name "test1 - Win32 PYTHON_VER_2_3 Debug"
# Name "test1 - Win32 PYTHON_VER_2_2 Unicode Release"
# Name "test1 - Win32 PYTHON_VER_2_2 Unicode Debug"
# Name "test1 - Win32 PYTHON_VER_2_2 Release"
# Name "test1 - Win32 PYTHON_VER_2_2 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\tests\test1\Test.cpp
# End Source File
# End Group
# End Target
# End Project

