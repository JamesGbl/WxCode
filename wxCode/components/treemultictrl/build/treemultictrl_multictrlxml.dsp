# Microsoft Developer Studio Project File - Name="treemultictrl_multictrlxml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=multictrlxml - Win32 Default
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "treemultictrl_multictrlxml.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "treemultictrl_multictrlxml.mak" CFG="multictrlxml - Win32 Default"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "multictrlxml - Win32 DLL Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "multictrlxml - Win32 DLL Unicode" (based on "Win32 (x86) Application")
!MESSAGE "multictrlxml - Win32 DLL Debug" (based on "Win32 (x86) Application")
!MESSAGE "multictrlxml - Win32 DLL" (based on "Win32 (x86) Application")
!MESSAGE "multictrlxml - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "multictrlxml - Win32 Unicode" (based on "Win32 (x86) Application")
!MESSAGE "multictrlxml - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "multictrlxml - Win32 Default" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "multictrlxml - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "samples"
# PROP BASE Intermediate_Dir "samples\multictrlxml"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "samples"
# PROP Intermediate_Dir "samples\multictrlxml"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lib\tmclib.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_dll" /debug /subsystem:windows
# ADD LINK32 lib\tmclib.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_dll" /debug /subsystem:windows

!ELSEIF  "$(CFG)" == "multictrlxml - Win32 DLL Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "samples"
# PROP BASE Intermediate_Dir "samples\multictrlxml"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "samples"
# PROP Intermediate_Dir "samples\multictrlxml"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lib\tmclib.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_dll" /debug /subsystem:windows
# ADD LINK32 lib\tmclib.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_dll" /debug /subsystem:windows

!ELSEIF  "$(CFG)" == "multictrlxml - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "samples"
# PROP BASE Intermediate_Dir "samples\multictrlxml"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "samples"
# PROP Intermediate_Dir "samples\multictrlxml"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lib\tmclib.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_dll" /debug /subsystem:windows
# ADD LINK32 lib\tmclib.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_dll" /debug /subsystem:windows

!ELSEIF  "$(CFG)" == "multictrlxml - Win32 DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "samples"
# PROP BASE Intermediate_Dir "samples\multictrlxml"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "samples"
# PROP Intermediate_Dir "samples\multictrlxml"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lib\tmclib.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_dll" /debug /subsystem:windows
# ADD LINK32 lib\tmclib.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_dll" /debug /subsystem:windows

!ELSEIF  "$(CFG)" == "multictrlxml - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "samples"
# PROP BASE Intermediate_Dir "samples\multictrlxml"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "samples"
# PROP Intermediate_Dir "samples\multictrlxml"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lib\tmclib.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_lib" /debug /subsystem:windows
# ADD LINK32 lib\tmclib.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_lib" /debug /subsystem:windows

!ELSEIF  "$(CFG)" == "multictrlxml - Win32 Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "samples"
# PROP BASE Intermediate_Dir "samples\multictrlxml"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "samples"
# PROP Intermediate_Dir "samples\multictrlxml"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lib\tmclib.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_lib" /debug /subsystem:windows
# ADD LINK32 lib\tmclib.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_lib" /debug /subsystem:windows

!ELSEIF  "$(CFG)" == "multictrlxml - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "samples"
# PROP BASE Intermediate_Dir "samples\multictrlxml"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "samples"
# PROP Intermediate_Dir "samples\multictrlxml"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lib\tmclib.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_lib" /debug /subsystem:windows
# ADD LINK32 lib\tmclib.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_lib" /debug /subsystem:windows

!ELSEIF  "$(CFG)" == "multictrlxml - Win32 Default"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "samples"
# PROP BASE Intermediate_Dir "samples\multictrlxml"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "samples"
# PROP Intermediate_Dir "samples\multictrlxml"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\contrib\include\wx\treemultictrl" /Zi /Gm /GZ /Fdsamples\multictrlxml.pdb /D "WIN32" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_DEBUG" /D CHECKBOXVIEW=0 /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\contrib\include\wx\treemultictrl" /d "_DEBUG" /d CHECKBOXVIEW=0 /d _WINDOWS
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lib\tmclib.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_lib" /debug /subsystem:windows
# ADD LINK32 lib\tmclib.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"samples\multictrlxml.exe" /libpath:"$(WXWIN)\lib\vc_lib" /debug /subsystem:windows

!ENDIF

# Begin Target

# Name "multictrlxml - Win32 DLL Unicode Debug"
# Name "multictrlxml - Win32 DLL Unicode"
# Name "multictrlxml - Win32 DLL Debug"
# Name "multictrlxml - Win32 DLL"
# Name "multictrlxml - Win32 Unicode Debug"
# Name "multictrlxml - Win32 Unicode"
# Name "multictrlxml - Win32 Debug"
# Name "multictrlxml - Win32 Default"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\contrib\samples\xmlmapper\MultiCtrlTestXML.cpp
# End Source File
# End Group
# End Target
# End Project

