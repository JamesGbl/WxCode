# Microsoft Developer Studio Project File - Name="webupdate_webupdate" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=webupdate - Win32 Default
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "webupdate_webupdate.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "webupdate_webupdate.mak" CFG="webupdate - Win32 Default"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "webupdate - Win32 DLL Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "webupdate - Win32 DLL Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "webupdate - Win32 DLL Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "webupdate - Win32 DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "webupdate - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "webupdate - Win32 Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "webupdate - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "webupdate - Win32 Default" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "webupdate - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\webupdate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\webupdate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdateud.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdateud.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\webupdateud.lib"
# ADD LIB32 /nologo /out:"..\lib\webupdateud.lib"

!ELSEIF  "$(CFG)" == "webupdate - Win32 DLL Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\webupdate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\webupdate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdateu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdateu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\webupdateu.lib"
# ADD LIB32 /nologo /out:"..\lib\webupdateu.lib"

!ELSEIF  "$(CFG)" == "webupdate - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\webupdate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\webupdate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdated.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdated.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\webupdated.lib"
# ADD LIB32 /nologo /out:"..\lib\webupdated.lib"

!ELSEIF  "$(CFG)" == "webupdate - Win32 DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\webupdate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\webupdate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdate.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdate.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\webupdate.lib"
# ADD LIB32 /nologo /out:"..\lib\webupdate.lib"

!ELSEIF  "$(CFG)" == "webupdate - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\webupdate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\webupdate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdateud.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdateud.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\webupdateud.lib"
# ADD LIB32 /nologo /out:"..\lib\webupdateud.lib"

!ELSEIF  "$(CFG)" == "webupdate - Win32 Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\webupdate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\webupdate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdateu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdateu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\webupdateu.lib"
# ADD LIB32 /nologo /out:"..\lib\webupdateu.lib"

!ELSEIF  "$(CFG)" == "webupdate - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\webupdate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\webupdate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdated.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdated.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\webupdated.lib"
# ADD LIB32 /nologo /out:"..\lib\webupdated.lib"

!ELSEIF  "$(CFG)" == "webupdate - Win32 Default"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\webupdate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\webupdate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdate.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdate.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\webupdate.lib"
# ADD LIB32 /nologo /out:"..\lib\webupdate.lib"

!ENDIF

# Begin Target

# Name "webupdate - Win32 DLL Unicode Debug"
# Name "webupdate - Win32 DLL Unicode"
# Name "webupdate - Win32 DLL Debug"
# Name "webupdate - Win32 DLL"
# Name "webupdate - Win32 Unicode Debug"
# Name "webupdate - Win32 Unicode"
# Name "webupdate - Win32 Debug"
# Name "webupdate - Win32 Default"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\checkedlistctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\webupdate.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\webupdatedlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\include\wx\checkedlistctrl.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\webupdate.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\webupdatedlg.h
# End Source File
# End Group
# End Target
# End Project

