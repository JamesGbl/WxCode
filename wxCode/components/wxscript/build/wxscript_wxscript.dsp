# Microsoft Developer Studio Project File - Name="wxscript_wxscript" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wxscript - Win32 PYTHON_VER_2_2 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxscript_wxscript.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxscript_wxscript.mak" CFG="wxscript - Win32 PYTHON_VER_2_2 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxscript - Win32 PYTHON_VER_2_6 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_6 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_6 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_6 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_5 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_5 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_5 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_5 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_4 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_4 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_4 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_4 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_3 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_3 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_3 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_3 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_2 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_2 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_2 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 PYTHON_VER_2_2 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_6 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscriptu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscriptu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptu.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptu.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_6 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptud.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptud.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_6 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscript.pdb /D "WIN32" /D "_LIB" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscript.pdb /D "WIN32" /D "_LIB" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscript.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscript.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_6 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptd.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptd.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_5 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscriptu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscriptu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptu.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptu.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_5 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptud.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptud.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_5 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscript.pdb /D "WIN32" /D "_LIB" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscript.pdb /D "WIN32" /D "_LIB" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscript.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscript.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_5 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptd.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptd.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_4 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscriptu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscriptu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptu.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptu.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_4 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptud.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptud.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_4 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscript.pdb /D "WIN32" /D "_LIB" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscript.pdb /D "WIN32" /D "_LIB" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscript.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscript.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_4 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptd.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptd.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_3 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscriptu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscriptu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptu.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptu.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_3 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptud.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptud.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_3 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscript.pdb /D "WIN32" /D "_LIB" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscript.pdb /D "WIN32" /D "_LIB" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscript.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscript.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_3 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptd.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptd.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_2 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscriptu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscriptu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptu.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptu.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_2 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptud.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptud.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_2 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscript.pdb /D "WIN32" /D "_LIB" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\lib\wxscript.pdb /D "WIN32" /D "_LIB" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscript.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscript.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 PYTHON_VER_2_2 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptd.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptd.lib"

!ENDIF

# Begin Target

# Name "wxscript - Win32 PYTHON_VER_2_6 Unicode Release"
# Name "wxscript - Win32 PYTHON_VER_2_6 Unicode Debug"
# Name "wxscript - Win32 PYTHON_VER_2_6 Release"
# Name "wxscript - Win32 PYTHON_VER_2_6 Debug"
# Name "wxscript - Win32 PYTHON_VER_2_5 Unicode Release"
# Name "wxscript - Win32 PYTHON_VER_2_5 Unicode Debug"
# Name "wxscript - Win32 PYTHON_VER_2_5 Release"
# Name "wxscript - Win32 PYTHON_VER_2_5 Debug"
# Name "wxscript - Win32 PYTHON_VER_2_4 Unicode Release"
# Name "wxscript - Win32 PYTHON_VER_2_4 Unicode Debug"
# Name "wxscript - Win32 PYTHON_VER_2_4 Release"
# Name "wxscript - Win32 PYTHON_VER_2_4 Debug"
# Name "wxscript - Win32 PYTHON_VER_2_3 Unicode Release"
# Name "wxscript - Win32 PYTHON_VER_2_3 Unicode Debug"
# Name "wxscript - Win32 PYTHON_VER_2_3 Release"
# Name "wxscript - Win32 PYTHON_VER_2_3 Debug"
# Name "wxscript - Win32 PYTHON_VER_2_2 Unicode Release"
# Name "wxscript - Win32 PYTHON_VER_2_2 Unicode Debug"
# Name "wxscript - Win32 PYTHON_VER_2_2 Release"
# Name "wxscript - Win32 PYTHON_VER_2_2 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\sccint.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\sclua.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\scpython.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\script.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\scunderc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\include\wx\sccint.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\sclua.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\scpython.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\script.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\scunderc.h
# End Source File
# End Group
# End Target
# End Project

