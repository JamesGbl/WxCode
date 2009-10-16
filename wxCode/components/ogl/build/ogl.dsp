# Microsoft Developer Studio Project File - Name="ogl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ogl - Win32 DLL Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ogl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ogl.mak" CFG="ogl - Win32 DLL Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ogl - Win32 DLL Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ogl - Win32 DLL Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ogl - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ogl - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "ogl - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\src\ogl\..\..\..\lib\vc6_dll"
# PROP BASE Intermediate_Dir "vc6_mswuddll\ogl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc6_dll"
# PROP Intermediate_Dir "vc6_mswuddll\ogl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /I "..\..\src\ogl\..\..\..\lib\vc6_dll\mswud" /I "..\..\src\ogl\..\..\..\include" /I "..\..\src\ogl\..\..\include" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "WXMAKINGDLL_OGL" /Fp"vc6_mswuddll\wxprec_ogldll.pch" /Yu"wx/wxprec.h" /Fd"..\..\src\ogl\..\..\..\lib\vc6_dll\wxmsw28ud_ogl_vc_custom.pdb" /FD /EHsc /c
# ADD CPP /MDd /W4 /Gm /GR /Zi /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "WXMAKINGDLL_OGL" /Yu"precomp.h" /Fd"..\lib\vc6_dll\wxmsw28ud_ogl_vc_custom.pdb" /FD /EHsc /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "WXMAKINGDLL_OGL" /mktyplib203 /win32
# ADD MTL /nologo /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "WXMAKINGDLL_OGL" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /i "..\..\src\ogl\..\..\..\lib\vc6_dll\mswud" /i "..\..\src\ogl\..\..\..\include" /i "..\..\src\ogl\..\..\include" /d "_DEBUG" /d "__WXDEBUG__" /d "_UNICODE" /d WXDLLNAME=wxmsw28ud_ogl_vc_custom /d "WXUSINGDLL" /d "WXMAKINGDLL_OGL"
# ADD RSC /l 0x409 /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /d "_DEBUG" /d "__WXDEBUG__" /d "_UNICODE" /d WXDLLNAME=wxmsw28ud_ogl_vc_custom /d "WXUSINGDLL" /d "WXMAKINGDLL_OGL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib wxmsw28ud_core.lib wxbase28ud.lib /nologo /dll /debug /machine:I386 /out:"..\..\src\ogl\..\..\..\lib\vc6_dll\wxmsw28ud_ogl_vc_custom.dll" /implib:"..\..\src\ogl\..\..\..\lib\vc6_dll\wxmsw28ud_ogl.lib" /libpath:"..\..\src\ogl\..\..\..\lib\vc6_dll"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /dll /debug /machine:I386 /out:"..\lib\vc6_dll\wxmsw28ud_ogl_vc_custom.dll" /implib:"..\lib\vc6_dll\wxmsw28ud_ogl.lib" /libpath:"$(WXWIN)\lib\vc_dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ogl - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\src\ogl\..\..\..\lib\vc6_dll"
# PROP BASE Intermediate_Dir "vc6_mswddll\ogl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc6_dll"
# PROP Intermediate_Dir "vc6_mswddll\ogl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /I "..\..\src\ogl\..\..\..\lib\vc6_dll\mswd" /I "..\..\src\ogl\..\..\..\include" /I "..\..\src\ogl\..\..\include" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "WXMAKINGDLL_OGL" /Fp"vc6_mswddll\wxprec_ogldll.pch" /Yu"wx/wxprec.h" /Fd"..\..\src\ogl\..\..\..\lib\vc6_dll\wxmsw28d_ogl_vc_custom.pdb" /FD /EHsc /c
# ADD CPP /MDd /W4 /Gm /GR /Zi /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "WXMAKINGDLL_OGL" /Yu"precomp.h" /Fd"..\lib\vc6_dll\wxmsw28d_ogl_vc_custom.pdb" /FD /EHsc /c
# SUBTRACT CPP /nologo
MTL=midl.exe
# ADD BASE MTL /nologo /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "WXMAKINGDLL_OGL" /mktyplib203 /win32
# ADD MTL /nologo /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "WXMAKINGDLL_OGL" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /i "..\..\src\ogl\..\..\..\lib\vc6_dll\mswd" /i "..\..\src\ogl\..\..\..\include" /i "..\..\src\ogl\..\..\include" /d "_DEBUG" /d "__WXDEBUG__" /d WXDLLNAME=wxmsw28d_ogl_vc_custom /d "WXUSINGDLL" /d "WXMAKINGDLL_OGL"
# ADD RSC /l 0x409 /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /d "_DEBUG" /d "__WXDEBUG__" /d WXDLLNAME=wxmsw28d_ogl_vc_custom /d "WXUSINGDLL" /d "WXMAKINGDLL_OGL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib wxmsw28d_core.lib wxbase28d.lib /nologo /dll /debug /machine:I386 /out:"..\..\src\ogl\..\..\..\lib\vc6_dll\wxmsw28d_ogl_vc_custom.dll" /implib:"..\..\src\ogl\..\..\..\lib\vc6_dll\wxmsw28d_ogl.lib" /libpath:"..\..\src\ogl\..\..\..\lib\vc6_dll"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /dll /debug /machine:I386 /out:"..\lib\vc6_dll\wxmsw28d_ogl_vc_custom.dll" /implib:"..\lib\vc6_dll\wxmsw28d_ogl.lib" /libpath:"$(WXWIN)\lib\vc_dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ogl - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\src\ogl\..\..\..\lib\vc6_lib"
# PROP BASE Intermediate_Dir "vc6_mswu\ogl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc6_lib"
# PROP Intermediate_Dir "vc6_mswu\ogl"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W4 /GR /O2 /I "..\..\src\ogl\..\..\..\lib\vc6_lib\mswu" /I "..\..\src\ogl\..\..\..\include" /I "..\..\src\ogl\..\..\include" /D "_LIB" /D "_UNICODE" /Fp"vc6_mswu\wxprec_ogllib.pch" /Yu"wx/wxprec.h" /Fd"..\..\src\ogl\..\..\..\lib\vc6_lib\wxmsw28u_ogl.pdb" /FD /EHsc /c
# ADD CPP /MT /W4 /GR /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /D "_LIB" /D "_UNICODE" /Yu"precomp.h" /Fd"..\lib\vc6_lib\wxmsw28u_ogl.pdb" /FD /EHsc /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\src\ogl\..\..\..\lib\vc6_lib\wxmsw28u_ogl.lib"
# ADD LIB32 /nologo /out:"..\lib\vc6_lib\wxmsw28u_ogl.lib"

!ELSEIF  "$(CFG)" == "ogl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\src\ogl\..\..\..\lib\vc6_lib"
# PROP BASE Intermediate_Dir "vc6_msw\ogl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc6_lib"
# PROP Intermediate_Dir "vc6_msw\ogl"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W4 /GR /O2 /I "..\..\src\ogl\..\..\..\lib\vc6_lib\msw" /I "..\..\src\ogl\..\..\..\include" /I "..\..\src\ogl\..\..\include" /D "_LIB" /Fp"vc6_msw\wxprec_ogllib.pch" /Yu"wx/wxprec.h" /Fd"..\..\src\ogl\..\..\..\lib\vc6_lib\wxmsw28_ogl.pdb" /FD /EHsc /c
# ADD CPP /MT /W4 /GR /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /D "_LIB" /Yu"precomp.h" /Fd"..\lib\vc6_lib\wxmsw28_ogl.pdb" /FD /EHsc /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\src\ogl\..\..\..\lib\vc6_lib\wxmsw28_ogl.lib"
# ADD LIB32 /nologo /out:"..\lib\vc6_lib\wxmsw28_ogl.lib"

!ENDIF 

# Begin Target

# Name "ogl - Win32 DLL Unicode Debug"
# Name "ogl - Win32 DLL Debug"
# Name "ogl - Win32 Unicode Release"
# Name "ogl - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\basic.cpp
# End Source File
# Begin Source File

SOURCE=..\src\basic2.cpp
# End Source File
# Begin Source File

SOURCE=..\src\bmpshape.cpp
# End Source File
# Begin Source File

SOURCE=..\src\canvas.cpp
# End Source File
# Begin Source File

SOURCE=..\src\composit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\constrnt.cpp
# End Source File
# Begin Source File

SOURCE=..\src\divided.cpp
# End Source File
# Begin Source File

SOURCE=..\src\drawn.cpp
# End Source File
# Begin Source File

SOURCE=..\src\lines.cpp
# End Source File
# Begin Source File

SOURCE=..\src\link.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\src\mfutils.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ogldiag.cpp
# End Source File
# Begin Source File

SOURCE=..\src\oglmisc.cpp
# End Source File
# Begin Source File

SOURCE=..\src\precomp.cpp
# ADD CPP /Yc"precomp.h"
# End Source File
# End Group
# End Target
# End Project
