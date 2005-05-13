# Microsoft Developer Studio Project File - Name="hypertest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=hypertest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "hypertest.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "hypertest.mak" CFG="hypertest - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "hypertest - Win32 DebugUniv" (based on "Win32 (x86) Application")
!MESSAGE "hypertest - Win32 ReleaseUniv" (based on "Win32 (x86) Application")
!MESSAGE "hypertest - Win32 DebugDev" (based on "Win32 (x86) Application")
!MESSAGE "hypertest - Win32 ReleaseDev" (based on "Win32 (x86) Application")
!MESSAGE "hypertest - Win32 DebugUnicode" (based on "Win32 (x86) Application")
!MESSAGE "hypertest - Win32 ReleaseUnicode" (based on "Win32 (x86) Application")
!MESSAGE "hypertest - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "hypertest - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hypertest - Win32 DebugUniv"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugUniv"
# PROP BASE Intermediate_Dir "DebugUniv"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugUniv"
# PROP Intermediate_Dir "DebugUniv"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "$(WXDEVEL)\include" /I "$(WXDEVEL)\lib\vc_lib\mswd" /I "..\..\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /D "__WXUNIVERSAL__" /D "__WXMSW__" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /i "$(WXDEVEL)\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib winmm.lib wsock32.lib wxbase25d.lib wxbase25d_net.lib wxbase25d_odbc.lib wxbase25d_xml.lib wxexpatd.lib wxjpegd.lib wxmswuniv25d_adv.lib wxmswuniv25d_core.lib wxmswuniv25d_dbgrid.lib wxmswuniv25d_gl.lib wxmswuniv25d_html.lib wxpngd.lib wxregexd.lib wxtiffd.lib wxzlibd.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"$(WXDEVEL)\lib\vc_lib"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "hypertest - Win32 ReleaseUniv"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUniv"
# PROP BASE Intermediate_Dir "ReleaseUniv"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUniv"
# PROP Intermediate_Dir "ReleaseUniv"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(WXDEVEL)\include" /I "$(WXDEVEL)\lib\vc_lib\msw" /I "..\..\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXUNIVERSAL__" /D "__WXMSW__" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x807 /i "$(WXDEVEL)\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib wxbase25.lib wxbase25_net.lib wxbase25_odbc.lib wxbase25_xml.lib wxexpat.lib wxjpeg.lib wxmswuniv25_adv.lib wxmswuniv25_core.lib wxmswuniv25_dbgrid.lib wxmswuniv25_gl.lib wxmswuniv25_html.lib wxpng.lib wxregex.lib wxtiff.lib wxzlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBCMT.lib" /libpath:"$(WXDEVEL)\lib\vc_lib"
# SUBTRACT LINK32 /profile /map /debug

!ELSEIF  "$(CFG)" == "hypertest - Win32 DebugDev"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugDev"
# PROP BASE Intermediate_Dir "DebugDev"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDev"
# PROP Intermediate_Dir "DebugDev"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "$(WXDEVEL)\include" /I "$(WXDEVEL)\lib\vc_lib\mswd" /I "..\..\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /D "__WXMSW__" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /i "$(WXDEVEL)\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib winmm.lib wsock32.lib wxbase25d.lib wxbase25d_net.lib wxbase25d_odbc.lib wxbase25d_xml.lib wxexpatd.lib wxjpegd.lib wxmsw25d_adv.lib wxmsw25d_core.lib wxmsw25d_dbgrid.lib wxmsw25d_gl.lib wxmsw25d_html.lib wxpngd.lib wxregexd.lib wxtiffd.lib wxzlibd.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"$(WXDEVEL)\lib\vc_lib"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "hypertest - Win32 ReleaseDev"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseDev"
# PROP BASE Intermediate_Dir "ReleaseDev"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseDev"
# PROP Intermediate_Dir "ReleaseDev"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(WXDEVEL)\include" /I "$(WXDEVEL)\lib\vc_lib\msw" /I "..\..\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXMSW__" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x807 /i "$(WXDEVEL)\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib wxbase25.lib wxbase25_net.lib wxbase25_odbc.lib wxbase25_xml.lib wxexpat.lib wxjpeg.lib wxmsw25_adv.lib wxmsw25_core.lib wxmsw25_dbgrid.lib wxmsw25_gl.lib wxmsw25_html.lib wxpng.lib wxregex.lib wxtiff.lib wxzlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBCMT.lib" /libpath:"$(WXDEVEL)\lib\vc_lib"
# SUBTRACT LINK32 /profile /map /debug

!ELSEIF  "$(CFG)" == "hypertest - Win32 DebugUnicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugUnicode"
# PROP BASE Intermediate_Dir "DebugUnicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugUnicode"
# PROP Intermediate_Dir "DebugUnicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "$(WXDEVEL)\include" /I "$(WXDEVEL)\lib\vc_lib\mswud" /I "..\..\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /D "__WXMSW__" /D "_UNICODE" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /i "$(WXDEVEL)\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib winmm.lib wsock32.lib wxbase25ud.lib wxbase25ud_net.lib wxbase25ud_odbc.lib wxbase25ud_xml.lib wxexpatd.lib wxjpegd.lib wxmsw25ud_adv.lib wxmsw25ud_core.lib wxmsw25ud_dbgrid.lib wxmsw25ud_gl.lib wxmsw25ud_html.lib wxpngd.lib wxregexud.lib wxtiffd.lib wxzlibd.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"$(WXDEVEL)\lib\vc_lib"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "hypertest - Win32 ReleaseUnicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUnicode"
# PROP BASE Intermediate_Dir "ReleaseUnicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUnicode"
# PROP Intermediate_Dir "ReleaseUnicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(WXDEVEL)\include" /I "$(WXDEVEL)\lib\vc_lib\mswu" /I "..\..\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXMSW__" /D "_UNICODE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x807 /i "$(WXDEVEL)\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib wxbase25u.lib wxbase25u_net.lib wxbase25u_odbc.lib wxbase25u_xml.lib wxexpat.lib wxjpeg.lib wxmsw25u_adv.lib wxmsw25u_core.lib wxmsw25u_dbgrid.lib wxmsw25u_gl.lib wxmsw25u_html.lib wxpng.lib wxregexu.lib wxtiff.lib wxzlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBCMT.lib" /libpath:"$(WXDEVEL)\lib\vc_lib"
# SUBTRACT LINK32 /profile /map /debug

!ELSEIF  "$(CFG)" == "hypertest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "$(WXSTABLE)\include" /I "$(WXSTABLE)\lib\mswd" /I "..\..\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /D "__WXMSW__" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /i "$(WXSTABLE)\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib winmm.lib wsock32.lib jpegd.lib pngd.lib regexd.lib tiffd.lib zlibd.lib wxmswd.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"$(WXSTABLE)\lib"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "hypertest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(WXSTABLE)\include" /I "$(WXSTABLE)\lib\msw" /I "..\..\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXMSW__" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32  /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32  /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x807 /i "$(WXSTABLE)\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib wxmsw.lib jpeg.lib png.lib regex.lib tiff.lib zlib.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib wxmsw.lib jpeg.lib png.lib regex.lib tiff.lib zlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBCMT.lib" /libpath:"$(WXSTABLE)\lib"
# SUBTRACT LINK32 /profile /map /debug

!ENDIF

# Begin Target

# Name "hypertest - Win32 DebugUniv"
# Name "hypertest - Win32 ReleaseUniv"
# Name "hypertest - Win32 DebugDev"
# Name "hypertest - Win32 ReleaseDev"
# Name "hypertest - Win32 DebugUnicode"
# Name "hypertest - Win32 ReleaseUnicode"
# Name "hypertest - Win32 Debug"
# Name "hypertest - Win32 Release"
# Begin Source File

SOURCE=.\hypertest.cpp
# End Source File
# Begin Source File

SOURCE="..\..\src\hyperlink.cpp"
# End Source File
# End Target
# End Project
