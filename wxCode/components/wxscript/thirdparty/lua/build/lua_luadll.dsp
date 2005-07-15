# Microsoft Developer Studio Project File - Name="lua_luadll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=luadll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lua_luadll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lua_luadll.mak" CFG="luadll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "luadll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "luadll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "luadll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\lib\luadll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\lib\luadll"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /GX /I "..\include" /I "..\src\" /w /O1 /Fd..\lib\luadll.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "LUA_SHARED" /c
# ADD CPP /nologo /FD /MD /GR /GX /I "..\include" /I "..\src\" /w /O1 /Fd..\lib\luadll.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "LUA_SHARED" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "LUA_SHARED" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "LUA_SHARED" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\include" /i "..\src\" /d LUA_SHARED
# ADD RSC /l 0x405 /i "..\include" /i "..\src\" /d LUA_SHARED
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:i386 /out:"..\lib\luadll.dll" /implib:"..\lib\luadll.lib"
# ADD LINK32 /nologo /dll /machine:i386 /out:"..\lib\luadll.dll" /implib:"..\lib\luadll.lib"

!ELSEIF  "$(CFG)" == "luadll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\lib\luadll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\lib\luadll"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /GX /I "..\include" /I "..\src\" /W4 /Od /Zi /Gm /GZ /Fd..\lib\luadll.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "LUA_SHARED" /c
# ADD CPP /nologo /FD /MDd /GR /GX /I "..\include" /I "..\src\" /W4 /Od /Zi /Gm /GZ /Fd..\lib\luadll.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "LUA_SHARED" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "LUA_SHARED" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "LUA_SHARED" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\include" /i "..\src\" /d "_DEBUG" /d LUA_SHARED
# ADD RSC /l 0x405 /i "..\include" /i "..\src\" /d "_DEBUG" /d LUA_SHARED
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:i386 /out:"..\lib\luadll.dll" /implib:"..\lib\luadll.lib" /debug
# ADD LINK32 /nologo /dll /machine:i386 /out:"..\lib\luadll.dll" /implib:"..\lib\luadll.lib" /debug

!ENDIF

# Begin Target

# Name "luadll - Win32 Release"
# Name "luadll - Win32 Debug"
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

