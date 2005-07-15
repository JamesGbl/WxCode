# Microsoft Developer Studio Project File - Name="tolua_tolua" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=tolua - Win32 Default
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tolua_tolua.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tolua_tolua.mak" CFG="tolua - Win32 Default"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tolua - Win32 Default" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tolua - Win32 Default"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\lib\tolua"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\lib\tolua"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /I "..\include" /I "..\src\lib" /I "..\src\bin" /I "c:\lua\include" /D "WIN32" /D "_LIB" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /I "..\include" /I "..\src\lib" /I "..\src\bin" /I "c:\lua\include" /D "WIN32" /D "_LIB" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\tolua.lib"
# ADD LIB32 /nologo /out:"..\lib\tolua.lib"

!ENDIF

# Begin Target

# Name "tolua - Win32 Default"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\lib\tolua_event.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lib\tolua_is.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lib\tolua_map.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lib\tolua_push.c
# End Source File
# Begin Source File

SOURCE=.\..\src\lib\tolua_to.c
# End Source File
# End Group
# End Target
# End Project

