# Microsoft Developer Studio Project File - Name="lua_luac" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=luac - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lua_luac.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lua_luac.mak" CFG="luac - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "luac - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "luac - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "luac - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\lib\luac"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\lib\luac"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /I "..\include" /I "..\src\" /w /O1 /Fd..\lib\luac.pdb /D "WIN32" /D "_CONSOLE" /D "LUA_OPNAMES" /c
# ADD CPP /nologo /FD /MD /GR /GX /I "..\include" /I "..\src\" /w /O1 /Fd..\lib\luac.pdb /D "WIN32" /D "_CONSOLE" /D "LUA_OPNAMES" /c
# ADD BASE RSC /l 0x405 /d "_CONSOLE" /i "..\include" /i "..\src\" /d LUA_OPNAMES
# ADD RSC /l 0x405 /d "_CONSOLE" /i "..\include" /i "..\src\" /d LUA_OPNAMES
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\lualib.lib /nologo /machine:i386 /out:"..\lib\luac.exe" /subsystem:console
# ADD LINK32 ..\lib\lualib.lib /nologo /machine:i386 /out:"..\lib\luac.exe" /subsystem:console

!ELSEIF  "$(CFG)" == "luac - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\lib\luac"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\lib\luac"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /I "..\include" /I "..\src\" /W4 /Od /Zi /Gm /GZ /Fd..\lib\luac.pdb /D "WIN32" /D "_CONSOLE" /D "_DEBUG" /D "LUA_OPNAMES" /c
# ADD CPP /nologo /FD /MDd /GR /GX /I "..\include" /I "..\src\" /W4 /Od /Zi /Gm /GZ /Fd..\lib\luac.pdb /D "WIN32" /D "_CONSOLE" /D "_DEBUG" /D "LUA_OPNAMES" /c
# ADD BASE RSC /l 0x405 /d "_CONSOLE" /i "..\include" /i "..\src\" /d "_DEBUG" /d LUA_OPNAMES
# ADD RSC /l 0x405 /d "_CONSOLE" /i "..\include" /i "..\src\" /d "_DEBUG" /d LUA_OPNAMES
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\lualib.lib /nologo /machine:i386 /out:"..\lib\luac.exe" /subsystem:console /debug
# ADD LINK32 ..\lib\lualib.lib /nologo /machine:i386 /out:"..\lib\luac.exe" /subsystem:console /debug

!ENDIF

# Begin Target

# Name "luac - Win32 Release"
# Name "luac - Win32 Debug"
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

SOURCE=.\..\src\luac\luac.c
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
# Begin Source File

SOURCE=.\..\src\luac\print.c
# End Source File
# End Group
# End Target
# End Project

