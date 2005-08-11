# Microsoft Developer Studio Project File - Name="httpengine_httpengine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=httpengine - Win32 Default
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "httpengine_httpengine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "httpengine_httpengine.mak" CFG="httpengine - Win32 Default"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "httpengine - Win32 DLL Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "httpengine - Win32 DLL Unicode" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "httpengine - Win32 DLL Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "httpengine - Win32 DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "httpengine - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "httpengine - Win32 Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "httpengine - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "httpengine - Win32 Default" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "httpengine - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prjud_dll\httpengine"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prjud_dll\httpengine"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\httpengineud.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\httpengineud.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_HTTPENGINE
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_HTTPENGINE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase26ud_net.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\httpengineud.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\httpengineud.lib" /debug
# ADD LINK32 wxbase26ud_net.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\httpengineud.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\httpengineud.lib" /debug

!ELSEIF  "$(CFG)" == "httpengine - Win32 DLL Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prju_dll\httpengine"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prju_dll\httpengine"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\httpengineu.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\httpengineu.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_HTTPENGINE
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_HTTPENGINE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase26u_net.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\httpengineu.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\httpengineu.lib"
# ADD LINK32 wxbase26u_net.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\httpengineu.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\httpengineu.lib"

!ELSEIF  "$(CFG)" == "httpengine - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prjd_dll\httpengine"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prjd_dll\httpengine"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\httpengined.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\httpengined.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_HTTPENGINE
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_HTTPENGINE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase26d_net.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\httpengined.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\httpengined.lib" /debug
# ADD LINK32 wxbase26d_net.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\httpengined.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\httpengined.lib" /debug

!ELSEIF  "$(CFG)" == "httpengine - Win32 DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj_dll\httpengine"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj_dll\httpengine"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\httpengine.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\httpengine.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_HTTPENGINE" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_HTTPENGINE
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_HTTPENGINE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase26_net.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\httpengine.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\httpengine.lib"
# ADD LINK32 wxbase26_net.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\httpengine.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\httpengine.lib"

!ELSEIF  "$(CFG)" == "httpengine - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prjud_lib\httpengine"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prjud_lib\httpengine"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\httpengineud.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\httpengineud.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\httpengineud.lib"
# ADD LIB32 /nologo /out:"..\lib\httpengineud.lib"

!ELSEIF  "$(CFG)" == "httpengine - Win32 Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prju_lib\httpengine"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prju_lib\httpengine"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\httpengineu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\httpengineu.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\httpengineu.lib"
# ADD LIB32 /nologo /out:"..\lib\httpengineu.lib"

!ELSEIF  "$(CFG)" == "httpengine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prjd_lib\httpengine"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prjd_lib\httpengine"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\httpengined.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\httpengined.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\httpengined.lib"
# ADD LIB32 /nologo /out:"..\lib\httpengined.lib"

!ELSEIF  "$(CFG)" == "httpengine - Win32 Default"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj_lib\httpengine"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj_lib\httpengine"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\httpengine.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\httpengine.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\httpengine.lib"
# ADD LIB32 /nologo /out:"..\lib\httpengine.lib"

!ENDIF

# Begin Target

# Name "httpengine - Win32 DLL Unicode Debug"
# Name "httpengine - Win32 DLL Unicode"
# Name "httpengine - Win32 DLL Debug"
# Name "httpengine - Win32 DLL"
# Name "httpengine - Win32 Unicode Debug"
# Name "httpengine - Win32 Unicode"
# Name "httpengine - Win32 Debug"
# Name "httpengine - Win32 Default"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\..\src\authdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\..\src\httpbuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\..\src\httpbuilderthread.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\..\src\hyperlinkctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\..\src\pleasewaitdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\..\src\proxysettingsdlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\include\..\include\wx\authdlg.h
# End Source File
# Begin Source File

SOURCE=.\..\include\..\include\wx\base64.h
# End Source File
# Begin Source File

SOURCE=.\..\include\..\include\wx\httpbuilder.h
# End Source File
# Begin Source File

SOURCE=.\..\include\..\include\wx\httpbuilderthread.h
# End Source File
# Begin Source File

SOURCE=.\..\include\..\include\wx\httpenginedef.h
# End Source File
# Begin Source File

SOURCE=.\..\include\..\include\wx\hyperlinkctrl.h
# End Source File
# Begin Source File

SOURCE=.\..\include\..\include\wx\proxysettingsdlg.h
# End Source File
# End Group
# End Target
# End Project

