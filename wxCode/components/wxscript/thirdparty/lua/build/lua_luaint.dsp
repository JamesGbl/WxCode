# Microsoft Developer Studio Project File - Name="lua_luaint" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=luaint - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lua_luaint.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lua_luaint.mak" CFG="luaint - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "luaint - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "luaint - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "luaint - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\lib\luaint"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\lib\luaint"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /I "..\include" /I "..\src\" /w /O1 /Fd..\lib\luaint.pdb /D "WIN32" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /GX /I "..\include" /I "..\src\" /w /O1 /Fd..\lib\luaint.pdb /D "WIN32" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x405 /d "_CONSOLE" /i "..\include" /i ..\src\
# ADD RSC /l 0x405 /d "_CONSOLE" /i "..\include" /i ..\src\
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\lua.lib ..\lib\lualib.lib /nologo /machine:i386 /out:"..\lib\luaint.exe" /subsystem:console
# ADD LINK32 ..\lib\lua.lib ..\lib\lualib.lib /nologo /machine:i386 /out:"..\lib\luaint.exe" /subsystem:console

!ELSEIF  "$(CFG)" == "luaint - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\lib\luaint"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\lib\luaint"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /I "..\include" /I "..\src\" /W4 /Od /Zi /Gm /GZ /Fd..\lib\luaint.pdb /D "WIN32" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /GX /I "..\include" /I "..\src\" /W4 /Od /Zi /Gm /GZ /Fd..\lib\luaint.pdb /D "WIN32" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x405 /d "_CONSOLE" /i "..\include" /i "..\src\" /d _DEBUG
# ADD RSC /l 0x405 /d "_CONSOLE" /i "..\include" /i "..\src\" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\lua.lib ..\lib\lualib.lib /nologo /machine:i386 /out:"..\lib\luaint.exe" /subsystem:console /debug
# ADD LINK32 ..\lib\lua.lib ..\lib\lualib.lib /nologo /machine:i386 /out:"..\lib\luaint.exe" /subsystem:console /debug

!ENDIF

# Begin Target

# Name "luaint - Win32 Release"
# Name "luaint - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\lua\lua.c
# End Source File
# End Group
# End Target
# End Project

