# Microsoft Developer Studio Project File - Name="lua_lualib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=lualib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lua_lualib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lua_lualib.mak" CFG="lualib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lualib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "lualib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lualib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\lib\lualib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\lib\lualib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /I "..\include" /I "..\src\" /w /O1 /Fd..\lib\lualib.pdb /D "WIN32" /D "_LIB" /c
# ADD CPP /nologo /FD /MD /GR /GX /I "..\include" /I "..\src\" /w /O1 /Fd..\lib\lualib.pdb /D "WIN32" /D "_LIB" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\lualib.lib"
# ADD LIB32 /nologo /out:"..\lib\lualib.lib"

!ELSEIF  "$(CFG)" == "lualib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\lib\lualib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\lib\lualib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /I "..\include" /I "..\src\" /W4 /Od /Zi /Gm /GZ /Fd..\lib\lualib.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /GX /I "..\include" /I "..\src\" /W4 /Od /Zi /Gm /GZ /Fd..\lib\lualib.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\lualib.lib"
# ADD LIB32 /nologo /out:"..\lib\lualib.lib"

!ENDIF

# Begin Target

# Name "lualib - Win32 Release"
# Name "lualib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\lib\lauxlib.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lib\lbaselib.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lib\ldblib.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lib\liolib.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lib\lmathlib.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lib\loadlib.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lib\lstrlib.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lib\ltablib.c
# End Source File
# End Group
# End Target
# End Project

