# Microsoft Developer Studio Project File - Name="keybinder_keybinder" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=keybinder - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "keybinder_keybinder.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "keybinder_keybinder.mak" CFG="keybinder - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "keybinder - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "keybinder - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "keybinder - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "keybinder - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "keybinder - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\keybinder"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\keybinder"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /Fd..\lib\keybinderu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /Fd..\lib\keybinderu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\keybinderu.lib"
# ADD LIB32 /nologo /out:"..\lib\keybinderu.lib"

!ELSEIF  "$(CFG)" == "keybinder - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\keybinder"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\keybinder"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /Zi /Gm /GZ /Fd..\lib\keybinderud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /Zi /Gm /GZ /Fd..\lib\keybinderud.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\keybinderud.lib"
# ADD LIB32 /nologo /out:"..\lib\keybinderud.lib"

!ELSEIF  "$(CFG)" == "keybinder - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\keybinder"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\keybinder"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /Fd..\lib\keybinder.pdb /D "WIN32" /D "_LIB" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O2 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /Fd..\lib\keybinder.pdb /D "WIN32" /D "_LIB" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\keybinder.lib"
# ADD LIB32 /nologo /out:"..\lib\keybinder.lib"

!ELSEIF  "$(CFG)" == "keybinder - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\keybinder"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\keybinder"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /Zi /Gm /GZ /Fd..\lib\keybinderd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /Zi /Gm /GZ /Fd..\lib\keybinderd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\keybinderd.lib"
# ADD LIB32 /nologo /out:"..\lib\keybinderd.lib"

!ENDIF

# Begin Target

# Name "keybinder - Win32 Unicode Release"
# Name "keybinder - Win32 Unicode Debug"
# Name "keybinder - Win32 Release"
# Name "keybinder - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\keybinder.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\menuutils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\include\wx\keybinder.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\menuutils.h
# End Source File
# End Group
# End Target
# End Project

