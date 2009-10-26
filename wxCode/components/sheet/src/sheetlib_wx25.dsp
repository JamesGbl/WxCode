# Microsoft Developer Studio Project File - Name="wxSheetLib_wx25" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wxSheetLib_wx25 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxSheetLib_wx25.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxSheetLib_wx25.mak" CFG="wxSheetLib_wx25 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxSheetLib_wx25 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxSheetLib_wx25 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe


!IF  "$(CFG)" == "wxSheetLib_wx25 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_msw_wx25"
# PROP BASE Intermediate_Dir "vc_msw_wx25"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_msw_wx25"
# PROP Intermediate_Dir "vc_msw_wx25"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MD /O1 /GR /GX /W4 /Fdvc_msw_wx25\wxSheetLib_wx25.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WX__" /c
# ADD CPP /nologo /FD /GR /GX /MD /O1 /GR /GX /I "$(WXWIN)\include" /I "$(WXWIN)\lib\vc_lib\msw" /I "." /I "$(WXWIN)\contrib\include" /I "..\include" /W4 /Fdvc_msw_wx25\wxSheetLib_wx25.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WX__" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"vc_msw_wx25\wxSheetLib_wx25.lib"
# ADD LIB32 /nologo /out:"vc_msw_wx25\wxSheetLib_wx25.lib"

!ELSEIF  "$(CFG)" == "wxSheetLib_wx25 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswd_wx25"
# PROP BASE Intermediate_Dir "vc_mswd_wx25"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswd_wx25"
# PROP Intermediate_Dir "vc_mswd_wx25"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MDd /Od /GR /GX /W4 /Zi /Gm /GZ /Fdvc_mswd_wx25\wxSheetLib_wx25.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "__WX__" /c
# ADD CPP /nologo /FD /GR /GX /MDd /Od /GR /GX /I "$(WXWIN)\include" /I "$(WXWIN)\lib\vc_lib\mswd" /I "." /I "$(WXWIN)\contrib\include" /I "..\include" /W4 /Zi /Gm /GZ /Fdvc_mswd_wx25\wxSheetLib_wx25.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "__WX__" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"vc_mswd_wx25\wxSheetLibd_wx25.lib"
# ADD LIB32 /nologo /out:"vc_mswd_wx25\wxSheetLibd_wx25.lib"

!ENDIF

# Begin Target

# Name "wxSheetLib_wx25 - Win32 Release"
# Name "wxSheetLib_wx25 - Win32 Debug"

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
