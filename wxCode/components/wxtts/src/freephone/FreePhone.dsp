# Microsoft Developer Studio Project File - Name="FreePhone" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=FreePhone - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FreePhone.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FreePhone.mak" CFG="FreePhone - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FreePhone - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "FreePhone - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FreePhone - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\FreePhone_mfc.lib"

!ELSEIF  "$(CFG)" == "FreePhone - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\FreePhone_mfc_d.lib"

!ENDIF 

# Begin Target

# Name "FreePhone - Win32 Release"
# Name "FreePhone - Win32 Debug"
# Begin Source File

SOURCE=.\audio.cpp
# End Source File
# Begin Source File

SOURCE=.\broad_cats.cpp
# End Source File
# Begin Source File

SOURCE=.\buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\conv1.cpp
# End Source File
# Begin Source File

SOURCE=.\conv2.cpp
# End Source File
# Begin Source File

SOURCE=.\durations.cpp
# End Source File
# Begin Source File

SOURCE=.\durpros.cpp
# End Source File
# Begin Source File

SOURCE=.\durs.cpp
# End Source File
# Begin Source File

SOURCE=.\edin2sampa.cpp
# End Source File
# Begin Source File

SOURCE=.\english.cpp
# End Source File
# Begin Source File

SOURCE=.\freephone.cpp
# End Source File
# Begin Source File

SOURCE=.\fw.cpp
# End Source File
# Begin Source File

SOURCE=.\go.cpp
# End Source File
# Begin Source File

SOURCE=.\grammar.cpp
# End Source File
# Begin Source File

SOURCE=.\interface.cpp
# End Source File
# Begin Source File

SOURCE=.\load_diphs.cpp
# End Source File
# Begin Source File

SOURCE=.\make_wave.cpp
# End Source File
# Begin Source File

SOURCE=.\MFDict.cpp
# End Source File
# Begin Source File

SOURCE=.\nrl_edin.cpp
# End Source File
# Begin Source File

SOURCE=.\phon_rules.cpp
# End Source File
# Begin Source File

SOURCE=.\phoneme.cpp
# End Source File
# Begin Source File

SOURCE=.\pitch.cpp
# End Source File
# Begin Source File

SOURCE=.\prosody.cpp
# End Source File
# Begin Source File

SOURCE=.\read_rules.cpp
# End Source File
# Begin Source File

SOURCE=.\regerror.cpp
# End Source File
# Begin Source File

SOURCE=.\regexp.cpp
# End Source File
# Begin Source File

SOURCE=.\rule_engine.cpp
# End Source File
# Begin Source File

SOURCE=.\saynum.cpp
# End Source File
# Begin Source File

SOURCE=.\space.cpp
# End Source File
# Begin Source File

SOURCE=.\spellword.cpp
# End Source File
# Begin Source File

SOURCE=.\spnio.cpp
# End Source File
# Begin Source File

SOURCE=.\syllab.cpp
# End Source File
# Begin Source File

SOURCE=.\t2s.cpp
# End Source File
# Begin Source File

SOURCE=.\tags.cpp
# End Source File
# Begin Source File

SOURCE=.\transcribe.cpp
# End Source File
# Begin Source File

SOURCE=.\utils.cpp
# End Source File
# End Target
# End Project
