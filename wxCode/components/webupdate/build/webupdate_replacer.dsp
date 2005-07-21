# Microsoft Developer Studio Project File - Name="webupdate_replacer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=replacer - Win32 Default
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "webupdate_replacer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "webupdate_replacer.mak" CFG="replacer - Win32 Default"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "replacer - Win32 DLL Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "replacer - Win32 DLL Unicode" (based on "Win32 (x86) Console Application")
!MESSAGE "replacer - Win32 DLL Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "replacer - Win32 DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "replacer - Win32 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "replacer - Win32 Unicode" (based on "Win32 (x86) Console Application")
!MESSAGE "replacer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "replacer - Win32 Default" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "replacer - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".."
# PROP BASE Intermediate_Dir "msvc6prj\replacer"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".."
# PROP Intermediate_Dir "msvc6prj\replacer"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /Od /Zi /Gm /GZ /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /Od /Zi /Gm /GZ /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "_CONSOLE" /d _DEBUG
# ADD RSC /l 0x409 /d "_CONSOLE" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console /debug
# ADD LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console /debug

!ELSEIF  "$(CFG)" == "replacer - Win32 DLL Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".."
# PROP BASE Intermediate_Dir "msvc6prj\replacer"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".."
# PROP Intermediate_Dir "msvc6prj\replacer"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /O2 /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "NDEBUG" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /O2 /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "NDEBUG" /c
# ADD BASE RSC /l 0x409 /d "_CONSOLE" /d NDEBUG
# ADD RSC /l 0x409 /d "_CONSOLE" /d NDEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console
# ADD LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console

!ELSEIF  "$(CFG)" == "replacer - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".."
# PROP BASE Intermediate_Dir "msvc6prj\replacer"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".."
# PROP Intermediate_Dir "msvc6prj\replacer"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /Od /Zi /Gm /GZ /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /Od /Zi /Gm /GZ /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "_CONSOLE" /d _DEBUG
# ADD RSC /l 0x409 /d "_CONSOLE" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console /debug
# ADD LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console /debug

!ELSEIF  "$(CFG)" == "replacer - Win32 DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".."
# PROP BASE Intermediate_Dir "msvc6prj\replacer"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".."
# PROP Intermediate_Dir "msvc6prj\replacer"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /O2 /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "NDEBUG" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /O2 /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "NDEBUG" /c
# ADD BASE RSC /l 0x409 /d "_CONSOLE" /d NDEBUG
# ADD RSC /l 0x409 /d "_CONSOLE" /d NDEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console
# ADD LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console

!ELSEIF  "$(CFG)" == "replacer - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".."
# PROP BASE Intermediate_Dir "msvc6prj\replacer"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".."
# PROP Intermediate_Dir "msvc6prj\replacer"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /Od /Zi /Gm /GZ /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /Od /Zi /Gm /GZ /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "_CONSOLE" /d _DEBUG
# ADD RSC /l 0x409 /d "_CONSOLE" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console /debug
# ADD LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console /debug

!ELSEIF  "$(CFG)" == "replacer - Win32 Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".."
# PROP BASE Intermediate_Dir "msvc6prj\replacer"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".."
# PROP Intermediate_Dir "msvc6prj\replacer"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /O2 /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "NDEBUG" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /O2 /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "NDEBUG" /c
# ADD BASE RSC /l 0x409 /d "_CONSOLE" /d NDEBUG
# ADD RSC /l 0x409 /d "_CONSOLE" /d NDEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console
# ADD LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console

!ELSEIF  "$(CFG)" == "replacer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".."
# PROP BASE Intermediate_Dir "msvc6prj\replacer"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".."
# PROP Intermediate_Dir "msvc6prj\replacer"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /Od /Zi /Gm /GZ /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /Od /Zi /Gm /GZ /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "_CONSOLE" /d _DEBUG
# ADD RSC /l 0x409 /d "_CONSOLE" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console /debug
# ADD LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console /debug

!ELSEIF  "$(CFG)" == "replacer - Win32 Default"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".."
# PROP BASE Intermediate_Dir "msvc6prj\replacer"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".."
# PROP Intermediate_Dir "msvc6prj\replacer"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /O2 /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "NDEBUG" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /O2 /Fd..\replacer.pdb /D "WIN32" /D "_CONSOLE" /D "NDEBUG" /c
# ADD BASE RSC /l 0x409 /d "_CONSOLE" /d NDEBUG
# ADD RSC /l 0x409 /d "_CONSOLE" /d NDEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console
# ADD LINK32 /nologo /machine:i386 /out:"..\replacer.exe" /subsystem:console

!ENDIF

# Begin Target

# Name "replacer - Win32 DLL Unicode Debug"
# Name "replacer - Win32 DLL Unicode"
# Name "replacer - Win32 DLL Debug"
# Name "replacer - Win32 DLL"
# Name "replacer - Win32 Unicode Debug"
# Name "replacer - Win32 Unicode"
# Name "replacer - Win32 Debug"
# Name "replacer - Win32 Default"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\replacer\replacer.c
# End Source File
# End Group
# End Target
# End Project

