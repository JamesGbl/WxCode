# Microsoft Developer Studio Project File - Name="wxSheetLib_wx24" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wxSheetLib_wx24 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxSheetLib_wx24.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxSheetLib_wx24.mak" CFG="wxSheetLib_wx24 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxSheetLib_wx24 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxSheetLib_wx24 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF "$(CFG)" == "wxSheetLib_wx24 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_msw_wx24"
# PROP BASE Intermediate_Dir "vc_msw_wx24"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_msw_wx24"
# PROP Intermediate_Dir "vc_msw_wx24"
# PROP Target_Dir ""
LINK32=link.exe
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /Fdvc_msw_wx24\wxSheetLib_wx24.pdb /c
# ADD CPP /nologo /MD /W3 /GX /O1 /Ob2 /I "..\include" /I "$(WXWIN)\include" /I "." /I "$(WXWIN)\lib/msw" /I "$(WXWIN)\contrib\include" /D "WIN32" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D "__HACK_MY_MSDEV40__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "__WX__" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x809 
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"vc_msw_wx24\wxSheetLib_wx24.lib"
# ADD LIB32 /nologo /out:"vc_msw_wx24\wxSheetLib_wx24.lib"

!ELSEIF "$(CFG)" == "wxSheetLib_wx24 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswd_wx24"
# PROP BASE Intermediate_Dir "vc_mswd_wx24"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswd_wx24"
# PROP Intermediate_Dir "vc_mswd_wx24"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LINK32=link.exe
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /Fdvc_mswd_wx24\wxSheetLib_wx24.pdb  /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\include" /I "$(WXWIN)\include" /I "." /I "$(WXWIN)\lib/mswd" /I "$(WXWIN)\contrib\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D DEBUG=1 /D "__WXDEBUG__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "__WX__" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"vc_mswd_wx24\wxSheetLibd_wx24.lib"
# ADD LIB32 /nologo /out:"vc_mswd_wx24\wxSheetLibd_wx24.lib"

!ENDIF 

# Begin Target

# Name "wxSheetLib_wx24 - Win32 Release"
# Name "wxSheetLib_wx24 - Win32 Debug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sheet.cpp
# End Source File
# Begin Source File

SOURCE=.\sheetctl.cpp
# End Source File
# Begin Source File

SOURCE=.\sheetsel.cpp
# End Source File
# Begin Source File

SOURCE=.\sheetspt.cpp
# End Source File
# Begin Source File

SOURCE=..\include\wx\sheet\sheet.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\sheet\sheetctl.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\sheet\sheetsel.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\sheet\sheetspt.h
# End Source File
# End Target
# End Project


