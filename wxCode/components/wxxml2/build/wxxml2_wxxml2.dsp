# Microsoft Developer Studio Project File - Name="wxxml2_wxxml2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wxxml2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxxml2_wxxml2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxxml2_wxxml2.mak" CFG="wxxml2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxxml2 - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxxml2 - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O1 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\libxml2\include" /I "c:\iconv\include" /Fd..\lib\wxxml2u.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "_UNICODE" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O1 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswu" /I "..\include" /I "c:\libxml2\include" /I "c:\iconv\include" /Fd..\lib\wxxml2u.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "_UNICODE" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxxml2u.lib"
# ADD LIB32 /nologo /out:"..\lib\wxxml2u.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\libxml2\include" /I "c:\iconv\include" /Zi /Gm /GZ /Fd..\lib\wxxml2ud.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_UNICODE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswud" /I "..\include" /I "c:\libxml2\include" /I "c:\iconv\include" /Zi /Gm /GZ /Fd..\lib\wxxml2ud.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_UNICODE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxxml2ud.lib"
# ADD LIB32 /nologo /out:"..\lib\wxxml2ud.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /w /O1 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\libxml2\include" /I "c:\iconv\include" /Fd..\lib\wxxml2.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /GX /w /O1 /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\msw" /I "..\include" /I "c:\libxml2\include" /I "c:\iconv\include" /Fd..\lib\wxxml2.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxxml2.lib"
# ADD LIB32 /nologo /out:"..\lib\wxxml2.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\libxml2\include" /I "c:\iconv\include" /Zi /Gm /GZ /Fd..\lib\wxxml2d.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /GX /W4 /Od /I "c:\wxWidgets\include" /I "c:\wxWidgets\lib\vc_lib\mswd" /I "..\include" /I "c:\libxml2\include" /I "c:\iconv\include" /Zi /Gm /GZ /Fd..\lib\wxxml2d.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxxml2d.lib"
# ADD LIB32 /nologo /out:"..\lib\wxxml2d.lib"

!ENDIF

# Begin Target

# Name "wxxml2 - Win32 Unicode Release"
# Name "wxxml2 - Win32 Unicode Debug"
# Name "wxxml2 - Win32 Release"
# Name "wxxml2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\dtd.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\xml2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\include\wx\dtd.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\xml2.h
# End Source File
# End Group
# End Target
# End Project

