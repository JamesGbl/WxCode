# Microsoft Developer Studio Project File - Name="wxscript_wxscript" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wxscript - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxscript_wxscript.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxscript_wxscript.mak" CFG="wxscript - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxscript - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxscript - Win32 Unicode Release"

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
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O1 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Fd..\lib\wxscriptu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O1 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Fd..\lib\wxscriptu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptu.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptu.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 Unicode Debug"

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
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptud.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptud.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 Release"

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
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O1 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Fd..\lib\wxscript.pdb /D "WIN32" /D "_LIB" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O1 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Fd..\lib\wxscript.pdb /D "WIN32" /D "_LIB" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscript.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscript.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 Debug"

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
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\Python\include" /I "c:\lua\include" /I "c:\tolua\include" /I "c:\cint\include" /I "c:\ucc\include" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /c
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

# Name "wxscript - Win32 Unicode Release"
# Name "wxscript - Win32 Unicode Debug"
# Name "wxscript - Win32 Release"
# Name "wxscript - Win32 Debug"
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

