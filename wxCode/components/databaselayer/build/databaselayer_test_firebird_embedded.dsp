# Microsoft Developer Studio Project File - Name="databaselayer_test_firebird_embedded" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=test_firebird_embedded - Win32 Static ANSI Release Multilib
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "databaselayer_test_firebird_embedded.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "databaselayer_test_firebird_embedded.mak" CFG="test_firebird_embedded - Win32 Static ANSI Release Multilib"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "test_firebird_embedded - Win32 DLL Unicode Debug Monolithic" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 DLL Unicode Debug Multilib" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 DLL Unicode Release Monolithic" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 DLL Unicode Release Multilib" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 DLL ANSI Debug Monolithic" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 DLL ANSI Debug Multilib" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 DLL ANSI Release Monolithic" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 DLL ANSI Release Multilib" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 Static Unicode Debug Monolithic" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 Static Unicode Debug Multilib" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 Static Unicode Release Monolithic" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 Static Unicode Release Multilib" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 Static ANSI Debug Monolithic" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 Static ANSI Debug Multilib" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 Static ANSI Release Monolithic" (based on "Win32 (x86) Console Application")
!MESSAGE "test_firebird_embedded - Win32 Static ANSI Release Multilib" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "test_firebird_embedded - Win32 DLL Unicode Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmswud_dll\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmswud_dll\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28ud_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /libpath:".\lib"
# ADD LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28ud_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 DLL Unicode Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmswud_dll\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmswud_dll\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28ud_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /libpath:".\lib"
# ADD LINK32 wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28ud_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 DLL Unicode Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmswu_dll\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmswu_dll\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28u_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /libpath:".\lib"
# ADD LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28u_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 DLL Unicode Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmswu_dll\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmswu_dll\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28u_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /libpath:".\lib"
# ADD LINK32 wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28u_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 DLL ANSI Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmswd_dll\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmswd_dll\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28d_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /libpath:".\lib"
# ADD LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28d_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 DLL ANSI Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmswd_dll\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmswd_dll\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28d_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /libpath:".\lib"
# ADD LINK32 wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28d_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 DLL ANSI Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmsw_dll\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmsw_dll\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /libpath:".\lib"
# ADD LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 DLL ANSI Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmsw_dll\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmsw_dll\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /libpath:".\lib"
# ADD LINK32 wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_dll\wxcode_msw28_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 Static Unicode Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmswud\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmswud\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28ud_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /libpath:".\lib"
# ADD LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28ud_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 Static Unicode Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmswud\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmswud\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28ud_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /libpath:".\lib"
# ADD LINK32 wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28ud_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 Static Unicode Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmswu\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmswu\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28u_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /libpath:".\lib"
# ADD LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28u_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 Static Unicode Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmswu\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmswu\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28u_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /libpath:".\lib"
# ADD LINK32 wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28u_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 Static ANSI Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmswd\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmswd\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28d_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /libpath:".\lib"
# ADD LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28d_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 Static ANSI Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmswd\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmswd\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /W4 /I "..\include" /I "..\tests" /Zi /Gm /GZ /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "_DEBUG" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28d_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /libpath:".\lib"
# ADD LINK32 wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28d_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 Static ANSI Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmsw\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmsw\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /libpath:".\lib"
# ADD LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /libpath:".\lib"

!ELSEIF  "$(CFG)" == "test_firebird_embedded - Win32 Static ANSI Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests"
# PROP BASE Intermediate_Dir "vcmsw\test_firebird_embedded"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests"
# PROP Intermediate_Dir "vcmsw\test_firebird_embedded"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\tests" /Fd..\tests\runner_firebird_embedded.pdb /I ".\include" /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /D "USE_FIREBIRD" /D "USE_FIREBIRD_EMBEDDED" /c
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\tests" /d "USE_FIREBIRD" /d "USE_FIREBIRD_EMBEDDED" /i .\include
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /libpath:".\lib"
# ADD LINK32 wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib ..\lib\vc_lib\wxcode_msw28_databaselayer_firebird.lib fbclient.lib /nologo /machine:i386 /out:"..\tests\runner_firebird_embedded.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /libpath:".\lib"

!ENDIF

# Begin Target

# Name "test_firebird_embedded - Win32 DLL Unicode Debug Monolithic"
# Name "test_firebird_embedded - Win32 DLL Unicode Debug Multilib"
# Name "test_firebird_embedded - Win32 DLL Unicode Release Monolithic"
# Name "test_firebird_embedded - Win32 DLL Unicode Release Multilib"
# Name "test_firebird_embedded - Win32 DLL ANSI Debug Monolithic"
# Name "test_firebird_embedded - Win32 DLL ANSI Debug Multilib"
# Name "test_firebird_embedded - Win32 DLL ANSI Release Monolithic"
# Name "test_firebird_embedded - Win32 DLL ANSI Release Multilib"
# Name "test_firebird_embedded - Win32 Static Unicode Debug Monolithic"
# Name "test_firebird_embedded - Win32 Static Unicode Debug Multilib"
# Name "test_firebird_embedded - Win32 Static Unicode Release Monolithic"
# Name "test_firebird_embedded - Win32 Static Unicode Release Multilib"
# Name "test_firebird_embedded - Win32 Static ANSI Debug Monolithic"
# Name "test_firebird_embedded - Win32 Static ANSI Debug Multilib"
# Name "test_firebird_embedded - Win32 Static ANSI Release Monolithic"
# Name "test_firebird_embedded - Win32 Static ANSI Release Multilib"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tests\MiniCppUnit.cxx
# End Source File
# Begin Source File

SOURCE=..\tests\TestsRunner.cxx
# End Source File
# Begin Source File

SOURCE=..\tests\databaselayer_tests.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tests\MiniCppUnit.hxx
# End Source File
# End Group
# End Target
# End Project

