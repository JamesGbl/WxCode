# Microsoft Developer Studio Project File - Name="treemultictrl_tmclib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=tmclib - Win32 Default
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "treemultictrl_tmclib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "treemultictrl_tmclib.mak" CFG="tmclib - Win32 Default"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tmclib - Win32 DLL Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "tmclib - Win32 DLL Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "tmclib - Win32 DLL Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "tmclib - Win32 DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "tmclib - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "tmclib - Win32 Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "tmclib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "tmclib - Win32 Default" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tmclib - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "lib"
# PROP BASE Intermediate_Dir "lib\tmclib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "lib\tmclib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\tmclib.lib"
# ADD LIB32 /nologo /out:"lib\tmclib.lib"

!ELSEIF  "$(CFG)" == "tmclib - Win32 DLL Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "lib"
# PROP BASE Intermediate_Dir "lib\tmclib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "lib\tmclib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\tmclib.lib"
# ADD LIB32 /nologo /out:"lib\tmclib.lib"

!ELSEIF  "$(CFG)" == "tmclib - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "lib"
# PROP BASE Intermediate_Dir "lib\tmclib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "lib\tmclib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\tmclib.lib"
# ADD LIB32 /nologo /out:"lib\tmclib.lib"

!ELSEIF  "$(CFG)" == "tmclib - Win32 DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "lib"
# PROP BASE Intermediate_Dir "lib\tmclib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "lib\tmclib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\tmclib.lib"
# ADD LIB32 /nologo /out:"lib\tmclib.lib"

!ELSEIF  "$(CFG)" == "tmclib - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "lib"
# PROP BASE Intermediate_Dir "lib\tmclib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "lib\tmclib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\tmclib.lib"
# ADD LIB32 /nologo /out:"lib\tmclib.lib"

!ELSEIF  "$(CFG)" == "tmclib - Win32 Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "lib"
# PROP BASE Intermediate_Dir "lib\tmclib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "lib\tmclib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\tmclib.lib"
# ADD LIB32 /nologo /out:"lib\tmclib.lib"

!ELSEIF  "$(CFG)" == "tmclib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "lib"
# PROP BASE Intermediate_Dir "lib\tmclib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "lib\tmclib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\tmclib.lib"
# ADD LIB32 /nologo /out:"lib\tmclib.lib"

!ELSEIF  "$(CFG)" == "tmclib - Win32 Default"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "lib"
# PROP BASE Intermediate_Dir "lib\tmclib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "lib\tmclib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdlib\tmclib.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"lib\tmclib.lib"
# ADD LIB32 /nologo /out:"lib\tmclib.lib"

!ENDIF

# Begin Target

# Name "tmclib - Win32 DLL Unicode Debug"
# Name "tmclib - Win32 DLL Unicode"
# Name "tmclib - Win32 DLL Debug"
# Name "tmclib - Win32 DLL"
# Name "tmclib - Win32 Unicode Debug"
# Name "tmclib - Win32 Unicode"
# Name "tmclib - Win32 Debug"
# Name "tmclib - Win32 Default"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\contrib\src\treemultictrl\TreeMultiItemBase.cpp
# End Source File
# Begin Source File

SOURCE=.\..\contrib\src\treemultictrl\TreeMultiItemNode.cpp
# End Source File
# Begin Source File

SOURCE=.\..\contrib\src\treemultictrl\TreeMultiItemRoot.cpp
# End Source File
# Begin Source File

SOURCE=.\..\contrib\src\treemultictrl\TreeMultiItemWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\..\contrib\src\treemultictrl\wxTreeMultiCtrl.cpp
# End Source File
# End Group
# End Target
# End Project

