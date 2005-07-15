# Microsoft Developer Studio Project File - Name="tolua_toluaexe" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=toluaexe - Win32 Default
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tolua_toluaexe.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tolua_toluaexe.mak" CFG="toluaexe - Win32 Default"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "toluaexe - Win32 Default" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "toluaexe - Win32 Default"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\lib\toluaexe"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\lib\toluaexe"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "..\include" /I "..\src\lib" /I "..\src\bin" /I "c:\lua\include" /D "WIN32" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "..\include" /I "..\src\lib" /I "..\src\bin" /I "c:\lua\include" /D "WIN32" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_CONSOLE" /i "..\include" /i "..\src\lib" /i "..\src\bin" /i c:\lua\include
# ADD RSC /l 0x409 /d "_CONSOLE" /i "..\include" /i "..\src\lib" /i "..\src\bin" /i c:\lua\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\tolua.lib lua.lib lualib.lib /nologo /machine:i386 /out:"..\lib\toluaexe.exe" /subsystem:console /libpath:"c:\lua\lib"
# ADD LINK32 ..\lib\tolua.lib lua.lib lualib.lib /nologo /machine:i386 /out:"..\lib\toluaexe.exe" /subsystem:console /libpath:"c:\lua\lib"

!ENDIF

# Begin Target

# Name "toluaexe - Win32 Default"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\bin\tolua.c
# End Source File
# Begin Source File

SOURCE=.\..\src\bin\toluabind.c
# End Source File
# End Group
# End Target
# End Project

