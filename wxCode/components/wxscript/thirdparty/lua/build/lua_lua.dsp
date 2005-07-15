# Microsoft Developer Studio Project File - Name="lua_lua" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=lua - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lua_lua.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lua_lua.mak" CFG="lua - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lua - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "lua - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lua - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\lib\lua"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\lib\lua"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /I "..\include" /I "..\src\" /w /O1 /Fd..\lib\lua.pdb /D "WIN32" /D "_LIB" /c
# ADD CPP /nologo /FD /MD /GR /GX /I "..\include" /I "..\src\" /w /O1 /Fd..\lib\lua.pdb /D "WIN32" /D "_LIB" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\lua.lib"
# ADD LIB32 /nologo /out:"..\lib\lua.lib"

!ELSEIF  "$(CFG)" == "lua - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\lib\lua"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\lib\lua"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /I "..\include" /I "..\src\" /W4 /Od /Zi /Gm /GZ /Fd..\lib\lua.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /GX /I "..\include" /I "..\src\" /W4 /Od /Zi /Gm /GZ /Fd..\lib\lua.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\lua.lib"
# ADD LIB32 /nologo /out:"..\lib\lua.lib"

!ENDIF

# Begin Target

# Name "lua - Win32 Release"
# Name "lua - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\lapi.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lcode.c
# End Source File
# Begin Source File

SOURCE=.\..\src\ldebug.c
# End Source File
# Begin Source File

SOURCE=.\..\src\ldo.c
# End Source File
# Begin Source File

SOURCE=.\..\src\ldump.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lfunc.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lgc.c
# End Source File
# Begin Source File

SOURCE=.\..\src\llex.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lmem.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lobject.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lopcodes.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lparser.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lstate.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lstring.c
# End Source File
# Begin Source File

SOURCE=.\..\src\ltable.c
# End Source File
# Begin Source File

SOURCE=.\..\src\ltests.c
# End Source File
# Begin Source File

SOURCE=.\..\src\ltm.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lundump.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lvm.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lzio.c
# End Source File
# End Group
# End Target
# End Project

