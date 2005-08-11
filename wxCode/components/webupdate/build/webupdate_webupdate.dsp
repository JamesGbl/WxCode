# Microsoft Developer Studio Project File - Name="webupdate_webupdate" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=webupdate - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "webupdate_webupdate.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "webupdate_webupdate.mak" CFG="webupdate - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "webupdate - Win32 DLL Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "webupdate - Win32 DLL Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "webupdate - Win32 DLL Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "webupdate - Win32 DLL Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "webupdate - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "webupdate - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "webupdate - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "webupdate - Win32 Release" (based on "Win32 (x86) Static Library")
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
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdateud.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdateud.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WEBUPDATE" /i ..\..\wxhttpengine\include
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WEBUPDATE" /i ..\..\wxhttpengine\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26ud_xrc.lib wxbase26ud_xml.lib wxbase26ud_net.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\webupdateud.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\webupdateud.lib" /debug /libpath:"..\..\wxhttpengine\lib"
# ADD LINK32 wxmsw26ud_xrc.lib wxbase26ud_xml.lib wxbase26ud_net.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\webupdateud.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\webupdateud.lib" /debug /libpath:"..\..\wxhttpengine\lib"

!ELSEIF  "$(CFG)" == "webupdate - Win32 DLL Unicode Release"

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
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdateu.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdateu.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WEBUPDATE" /i ..\..\wxhttpengine\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WEBUPDATE" /i ..\..\wxhttpengine\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26u_xrc.lib wxbase26u_xml.lib wxbase26u_net.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\webupdateu.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\webupdateu.lib" /libpath:"..\..\wxhttpengine\lib"
# ADD LINK32 wxmsw26u_xrc.lib wxbase26u_xml.lib wxbase26u_net.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\webupdateu.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\webupdateu.lib" /libpath:"..\..\wxhttpengine\lib"

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
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdated.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdated.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WEBUPDATE" /i ..\..\wxhttpengine\include
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WEBUPDATE" /i ..\..\wxhttpengine\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26d_xrc.lib wxbase26d_xml.lib wxbase26d_net.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\webupdated.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\webupdated.lib" /debug /libpath:"..\..\wxhttpengine\lib"
# ADD LINK32 wxmsw26d_xrc.lib wxbase26d_xml.lib wxbase26d_net.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\webupdated.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\webupdated.lib" /debug /libpath:"..\..\wxhttpengine\lib"

!ELSEIF  "$(CFG)" == "webupdate - Win32 DLL Release"

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
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdate.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdate.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WEBUPDATE" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WEBUPDATE" /i ..\..\wxhttpengine\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WEBUPDATE" /i ..\..\wxhttpengine\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26_xrc.lib wxbase26_xml.lib wxbase26_net.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\webupdate.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\webupdate.lib" /libpath:"..\..\wxhttpengine\lib"
# ADD LINK32 wxmsw26_xrc.lib wxbase26_xml.lib wxbase26_net.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\webupdate.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\webupdate.lib" /libpath:"..\..\wxhttpengine\lib"

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
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdateud.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdateud.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\webupdateud.lib"
# ADD LIB32 /nologo /out:"..\lib\webupdateud.lib"

!ELSEIF  "$(CFG)" == "webupdate - Win32 Unicode Release"

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
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdateu.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdateu.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
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
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdated.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\webupdated.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\webupdated.lib"
# ADD LIB32 /nologo /out:"..\lib\webupdated.lib"

!ELSEIF  "$(CFG)" == "webupdate - Win32 Release"

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
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdate.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\webupdate.pdb /I "..\..\wxhttpengine\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
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
# Name "webupdate - Win32 DLL Unicode Release"
# Name "webupdate - Win32 DLL Debug"
# Name "webupdate - Win32 DLL Release"
# Name "webupdate - Win32 Unicode Debug"
# Name "webupdate - Win32 Unicode Release"
# Name "webupdate - Win32 Debug"
# Name "webupdate - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\checkedlistctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\download.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\installer.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\md5.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\stdactions.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\webupdate.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\webupdatectrl.cpp
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

SOURCE=.\..\include\wx\download.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\installer.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\md5.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\stdactions.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\webupdate.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\webupdatectrl.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\webupdatedef.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\webupdatedlg.h
# End Source File
# End Group
# End Target
# End Project

