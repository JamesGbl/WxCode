# Microsoft Developer Studio Project File - Name="wxxml2_wxxml2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=wxxml2 - Win32 Default
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxxml2_wxxml2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxxml2_wxxml2.mak" CFG="wxxml2 - Win32 Default"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxxml2 - Win32 DLL Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxxml2 - Win32 DLL Unicode" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxxml2 - Win32 DLL Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxxml2 - Win32 DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxxml2 - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxxml2 - Win32 Default" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxxml2 - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxxml2_dll.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxxml2_dll.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /i "c:\libxml2\include" /i c:\iconv\include
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /i "c:\libxml2\include" /i c:\iconv\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:i386 /out:"..\lib\wxxml2_dll.dll" /libpath:"..\lib" /implib:"..\lib\wxxml2ud.lib" /debug /libpath:"c:\libxml2\lib" /libpath:"c:\iconv\lib"
# ADD LINK32 /nologo /dll /machine:i386 /out:"..\lib\wxxml2_dll.dll" /libpath:"..\lib" /implib:"..\lib\wxxml2ud.lib" /debug /libpath:"c:\libxml2\lib" /libpath:"c:\iconv\lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 DLL Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxxml2_dll.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxxml2_dll.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /i "c:\libxml2\include" /i c:\iconv\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /i "c:\libxml2\include" /i c:\iconv\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:i386 /out:"..\lib\wxxml2_dll.dll" /libpath:"..\lib" /implib:"..\lib\wxxml2u.lib" /libpath:"c:\libxml2\lib" /libpath:"c:\iconv\lib"
# ADD LINK32 /nologo /dll /machine:i386 /out:"..\lib\wxxml2_dll.dll" /libpath:"..\lib" /implib:"..\lib\wxxml2u.lib" /libpath:"c:\libxml2\lib" /libpath:"c:\iconv\lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxxml2_dll.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxxml2_dll.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /i "c:\libxml2\include" /i c:\iconv\include
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /i "c:\libxml2\include" /i c:\iconv\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:i386 /out:"..\lib\wxxml2_dll.dll" /libpath:"..\lib" /implib:"..\lib\wxxml2d.lib" /debug /libpath:"c:\libxml2\lib" /libpath:"c:\iconv\lib"
# ADD LINK32 /nologo /dll /machine:i386 /out:"..\lib\wxxml2_dll.dll" /libpath:"..\lib" /implib:"..\lib\wxxml2d.lib" /debug /libpath:"c:\libxml2\lib" /libpath:"c:\iconv\lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxxml2_dll.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxxml2_dll.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /i "c:\libxml2\include" /i c:\iconv\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /i "c:\libxml2\include" /i c:\iconv\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:i386 /out:"..\lib\wxxml2_dll.dll" /libpath:"..\lib" /implib:"..\lib\wxxml2.lib" /libpath:"c:\libxml2\lib" /libpath:"c:\iconv\lib"
# ADD LINK32 /nologo /dll /machine:i386 /out:"..\lib\wxxml2_dll.dll" /libpath:"..\lib" /implib:"..\lib\wxxml2.lib" /libpath:"c:\libxml2\lib" /libpath:"c:\iconv\lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxxml2ud.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxxml2ud.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxxml2ud.lib"
# ADD LIB32 /nologo /out:"..\lib\wxxml2ud.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxxml2u.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxxml2u.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxxml2u.lib"
# ADD LIB32 /nologo /out:"..\lib\wxxml2u.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxxml2d.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxxml2d.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxxml2d.lib"
# ADD LIB32 /nologo /out:"..\lib\wxxml2d.lib"

!ELSEIF  "$(CFG)" == "wxxml2 - Win32 Default"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxxml2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxxml2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxxml2.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxxml2.pdb /I "c:\libxml2\include" /I "c:\iconv\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxxml2.lib"
# ADD LIB32 /nologo /out:"..\lib\wxxml2.lib"

!ENDIF

# Begin Target

# Name "wxxml2 - Win32 DLL Unicode Debug"
# Name "wxxml2 - Win32 DLL Unicode"
# Name "wxxml2 - Win32 DLL Debug"
# Name "wxxml2 - Win32 DLL"
# Name "wxxml2 - Win32 Unicode Debug"
# Name "wxxml2 - Win32 Unicode"
# Name "wxxml2 - Win32 Debug"
# Name "wxxml2 - Win32 Default"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\dtd.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\xml2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\include\wx\dtd.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\xml2.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\xml2def.h
# End Source File
# End Group
# End Target
# End Project

