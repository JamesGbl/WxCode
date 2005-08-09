# Microsoft Developer Studio Project File - Name="iconv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=iconv - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "iconv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "iconv.mak" CFG="iconv - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "iconv - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "iconv - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "iconv - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "iconv - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "iconv - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\iconv"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\iconv"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "..\include" /Fd..\lib\iconv.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "HAVE_CONFIG_H" /D ENABLE_RELOCATABLE=1 /D "IN_LIBRARY" /D "NO_XMALLOC" /D set_relocation_prefix=libiconv_set_relocation_prefix /D relocate=libiconv_relocate /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "..\include" /Fd..\lib\iconv.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "HAVE_CONFIG_H" /D ENABLE_RELOCATABLE=1 /D "IN_LIBRARY" /D "NO_XMALLOC" /D set_relocation_prefix=libiconv_set_relocation_prefix /D relocate=libiconv_relocate /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\iconv.lib"
# ADD LIB32 /nologo /out:"..\lib\iconv.lib"

!ELSEIF  "$(CFG)" == "iconv - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\iconv"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\iconv"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\iconv.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "HAVE_CONFIG_H" /D ENABLE_RELOCATABLE=1 /D "IN_LIBRARY" /D "NO_XMALLOC" /D set_relocation_prefix=libiconv_set_relocation_prefix /D relocate=libiconv_relocate /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\iconv.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "_DEBUG" /D "HAVE_CONFIG_H" /D ENABLE_RELOCATABLE=1 /D "IN_LIBRARY" /D "NO_XMALLOC" /D set_relocation_prefix=libiconv_set_relocation_prefix /D relocate=libiconv_relocate /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\iconv.lib"
# ADD LIB32 /nologo /out:"..\lib\iconv.lib"

!ELSEIF  "$(CFG)" == "iconv - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\iconv"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\iconv"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "..\include" /Fd..\lib\iconv.pdb /D "WIN32" /D "_LIB" /D "HAVE_CONFIG_H" /D ENABLE_RELOCATABLE=1 /D "IN_LIBRARY" /D "NO_XMALLOC" /D set_relocation_prefix=libiconv_set_relocation_prefix /D relocate=libiconv_relocate /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "..\include" /Fd..\lib\iconv.pdb /D "WIN32" /D "_LIB" /D "HAVE_CONFIG_H" /D ENABLE_RELOCATABLE=1 /D "IN_LIBRARY" /D "NO_XMALLOC" /D set_relocation_prefix=libiconv_set_relocation_prefix /D relocate=libiconv_relocate /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\iconv.lib"
# ADD LIB32 /nologo /out:"..\lib\iconv.lib"

!ELSEIF  "$(CFG)" == "iconv - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\iconv"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\iconv"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\iconv.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "HAVE_CONFIG_H" /D ENABLE_RELOCATABLE=1 /D "IN_LIBRARY" /D "NO_XMALLOC" /D set_relocation_prefix=libiconv_set_relocation_prefix /D relocate=libiconv_relocate /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "..\include" /Zi /Gm /GZ /Fd..\lib\iconv.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "HAVE_CONFIG_H" /D ENABLE_RELOCATABLE=1 /D "IN_LIBRARY" /D "NO_XMALLOC" /D set_relocation_prefix=libiconv_set_relocation_prefix /D relocate=libiconv_relocate /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\iconv.lib"
# ADD LIB32 /nologo /out:"..\lib\iconv.lib"

!ENDIF

# Begin Target

# Name "iconv - Win32 Unicode Release"
# Name "iconv - Win32 Unicode Debug"
# Name "iconv - Win32 Release"
# Name "iconv - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\lib\iconv.c
# End Source File
# Begin Source File

SOURCE=.\..\libcharset\lib\localcharset.c
# End Source File
# Begin Source File

SOURCE=.\..\lib\relocatable.c
# End Source File
# End Group
# End Target
# End Project

