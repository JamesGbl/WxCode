# Microsoft Developer Studio Project File - Name="wxscript_wxscript" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wxscript - Win32 Release WX_MONOLITHIC_0
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxscript_wxscript.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxscript_wxscript.mak" CFG="wxscript - Win32 Release WX_MONOLITHIC_0"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxscript - Win32 DLL Unicode Debug WX_MONOLITHIC_1" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxscript - Win32 DLL Unicode Debug WX_MONOLITHIC_0" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxscript - Win32 DLL Unicode Release WX_MONOLITHIC_1" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxscript - Win32 DLL Unicode Release WX_MONOLITHIC_0" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxscript - Win32 DLL Debug WX_MONOLITHIC_1" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxscript - Win32 DLL Debug WX_MONOLITHIC_0" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxscript - Win32 DLL Release WX_MONOLITHIC_1" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxscript - Win32 DLL Release WX_MONOLITHIC_0" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxscript - Win32 Unicode Debug WX_MONOLITHIC_1" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 Unicode Debug WX_MONOLITHIC_0" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 Unicode Release WX_MONOLITHIC_1" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 Unicode Release WX_MONOLITHIC_0" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 Debug WX_MONOLITHIC_1" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 Debug WX_MONOLITHIC_0" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 Release WX_MONOLITHIC_1" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscript - Win32 Release WX_MONOLITHIC_0" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxscript - Win32 DLL Unicode Debug WX_MONOLITHIC_1"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lua.lib lualib.lib tolua.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscriptud.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscriptud.lib" /debug /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"
# ADD LINK32 lua.lib lualib.lib tolua.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscriptud.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscriptud.lib" /debug /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"

!ELSEIF  "$(CFG)" == "wxscript - Win32 DLL Unicode Debug WX_MONOLITHIC_0"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lua.lib lualib.lib tolua.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscriptud.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscriptud.lib" /debug /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"
# ADD LINK32 lua.lib lualib.lib tolua.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscriptud.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscriptud.lib" /debug /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"

!ELSEIF  "$(CFG)" == "wxscript - Win32 DLL Unicode Release WX_MONOLITHIC_1"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscriptu.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscriptu.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lua.lib lualib.lib tolua.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscriptu.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscriptu.lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"
# ADD LINK32 lua.lib lualib.lib tolua.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscriptu.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscriptu.lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"

!ELSEIF  "$(CFG)" == "wxscript - Win32 DLL Unicode Release WX_MONOLITHIC_0"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscriptu.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscriptu.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lua.lib lualib.lib tolua.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscriptu.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscriptu.lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"
# ADD LINK32 lua.lib lualib.lib tolua.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscriptu.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscriptu.lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"

!ELSEIF  "$(CFG)" == "wxscript - Win32 DLL Debug WX_MONOLITHIC_1"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lua.lib lualib.lib tolua.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscriptd.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscriptd.lib" /debug /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"
# ADD LINK32 lua.lib lualib.lib tolua.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscriptd.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscriptd.lib" /debug /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"

!ELSEIF  "$(CFG)" == "wxscript - Win32 DLL Debug WX_MONOLITHIC_0"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lua.lib lualib.lib tolua.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscriptd.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscriptd.lib" /debug /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"
# ADD LINK32 lua.lib lualib.lib tolua.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscriptd.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscriptd.lib" /debug /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"

!ELSEIF  "$(CFG)" == "wxscript - Win32 DLL Release WX_MONOLITHIC_1"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscript.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscript.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lua.lib lualib.lib tolua.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscript.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscript.lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"
# ADD LINK32 lua.lib lualib.lib tolua.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscript.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscript.lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"

!ELSEIF  "$(CFG)" == "wxscript - Win32 DLL Release WX_MONOLITHIC_0"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscript.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscript.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_WXSCRIPT" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "WXMAKINGDLL_WXSCRIPT" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "c:\perl\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d wxSCRIPT_NO_PYTHON
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lua.lib lualib.lib tolua.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscript.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscript.lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"
# ADD LINK32 lua.lib lualib.lib tolua.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\wxscript.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /implib:"..\lib\wxscript.lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\perl\lib" /libpath:"c:\cint" /libpath:"c:\ucc"

!ELSEIF  "$(CFG)" == "wxscript - Win32 Unicode Debug WX_MONOLITHIC_1"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptud.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptud.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 Unicode Debug WX_MONOLITHIC_0"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptud.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptud.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptud.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 Unicode Release WX_MONOLITHIC_1"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscriptu.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscriptu.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptu.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptu.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 Unicode Release WX_MONOLITHIC_0"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscriptu.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscriptu.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptu.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptu.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 Debug WX_MONOLITHIC_1"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptd.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptd.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 Debug WX_MONOLITHIC_0"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /Zi /Gm /GZ /Fd..\lib\wxscriptd.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscriptd.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscriptd.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 Release WX_MONOLITHIC_1"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscript.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscript.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscript.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscript.lib"

!ELSEIF  "$(CFG)" == "wxscript - Win32 Release WX_MONOLITHIC_0"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "msvc6prj\wxscript"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "msvc6prj\wxscript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscript.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /Fd..\lib\wxscript.pdb /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "c:\perl\include" /I "c:\cint" /I "c:\ucc" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "wxSCRIPT_NO_PYTHON" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscript.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscript.lib"

!ENDIF

# Begin Target

# Name "wxscript - Win32 DLL Unicode Debug WX_MONOLITHIC_1"
# Name "wxscript - Win32 DLL Unicode Debug WX_MONOLITHIC_0"
# Name "wxscript - Win32 DLL Unicode Release WX_MONOLITHIC_1"
# Name "wxscript - Win32 DLL Unicode Release WX_MONOLITHIC_0"
# Name "wxscript - Win32 DLL Debug WX_MONOLITHIC_1"
# Name "wxscript - Win32 DLL Debug WX_MONOLITHIC_0"
# Name "wxscript - Win32 DLL Release WX_MONOLITHIC_1"
# Name "wxscript - Win32 DLL Release WX_MONOLITHIC_0"
# Name "wxscript - Win32 Unicode Debug WX_MONOLITHIC_1"
# Name "wxscript - Win32 Unicode Debug WX_MONOLITHIC_0"
# Name "wxscript - Win32 Unicode Release WX_MONOLITHIC_1"
# Name "wxscript - Win32 Unicode Release WX_MONOLITHIC_0"
# Name "wxscript - Win32 Debug WX_MONOLITHIC_1"
# Name "wxscript - Win32 Debug WX_MONOLITHIC_0"
# Name "wxscript - Win32 Release WX_MONOLITHIC_1"
# Name "wxscript - Win32 Release WX_MONOLITHIC_0"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\src\sccint.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\sclua.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\scperl.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\scpython.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\script.cpp
# End Source File
# Begin Source File

SOURCE=.\..\src\scunderc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\include\wx\sccint.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\scdef.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\sclua.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\scperl.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\scpython.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\script.h
# End Source File
# Begin Source File

SOURCE=.\..\include\wx\scunderc.h
# End Source File
# End Group
# End Target
# End Project

