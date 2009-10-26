# Microsoft Developer Studio Project File - Name="wxSheetDemo_wx24" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=wxSheetDemo_wx24 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxSheetDemo_wx24.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxSheetDemo_wx24.mak" CFG="wxSheetDemo_wx24 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxSheetDemo_wx24 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "wxSheetDemo_wx24 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF "$(CFG)" == "wxSheetDemo_wx24 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_msw_wx24"
# PROP BASE Intermediate_Dir "vc_msw_wx24"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_msw_wx24"
# PROP Intermediate_Dir "vc_msw_wx24"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /YX /FD /Fdvc_msw_wx24\wxSheetDemo.pdb /c
# ADD CPP /nologo /W4 /O2 /I "$(WXWIN)\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D WIN32 /D WINVER=0x400 /D _MT /D wxUSE_GUI=1 /YX /FD /Fdvc_msw_wx24\wxSheetDemo.pdb /c /MD  /I"$(WXWIN)\lib\msw" /I "." /I "$(WXWIN)\contrib\include" /I "..\..\include"
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /I "$(WXWIN)\include"
# ADD RSC /l 0x409 /d "NDEBUG" /I "$(WXWIN)\include"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /machine:I386 /out:"vc_msw_wx24\wxSheetDemo.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /machine:I386 zlib.lib regex.lib png.lib jpeg.lib tiff.lib wxmsw.lib /libpath:"$(WXWIN)\lib" /libpath:"$(WXWIN)\contrib\lib" /out:"vc_msw_wx24\wxSheetDemo.exe"

!ELSEIF "$(CFG)" == "wxSheetDemo_wx24 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswd_wx24"
# PROP BASE Intermediate_Dir "vc_mswd_wx24"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswd_wx24"
# PROP Intermediate_Dir "vc_mswd_wx24"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /YX /FD /Fdvc_mswd_wx24\wxSheetDemo.pdb /c
# ADD CPP /nologo /W4 /Zi /Od /I "$(WXWIN)\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D WIN32 /D WINVER=0x400 /D _MT /D wxUSE_GUI=1 /YX /FD /Fdvc_mswd_wx24\wxSheetDemo.pdb /c /MDd /D "__WXDEBUG__" /D "WXDEBUG=1"  /I"$(WXWIN)\lib\mswd" /I "." /I "$(WXWIN)\contrib\include" /I "..\..\include"
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /I "$(WXWIN)\include"
# ADD RSC /l 0x409 /d "_DEBUG" /I "$(WXWIN)\include"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /out:"vc_mswd_wx24\wxSheetDemo.exe" 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept zlibd.lib regexd.lib pngd.lib jpegd.lib tiffd.lib wxmswd.lib /libpath:"$(WXWIN)\lib" /libpath:"$(WXWIN)\contrib\lib" /out:"vc_mswd_wx24\wxSheetDemo.exe"

!ENDIF 

# Begin Target

# Name "wxSheetDemo_wx24 - Win32 Release"
# Name "wxSheetDemo_wx24 - Win32 Debug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sheetdemo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sheet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sheetctl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sheetsel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\sheetspt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\sheet\sheet.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\sheet\sheetctl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\sheet\sheetsel.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\sheet\sheetspt.h
# End Source File
# End Group
# End Target
# End Project

