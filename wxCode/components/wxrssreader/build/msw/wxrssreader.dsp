# Microsoft Developer Studio Project File - Name="wxrssreader" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=wxrssreader - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxrssreader.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxrssreader.mak" CFG="wxrssreader - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxrssreader - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "wxrssreader - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "wxrssreader - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "wxrssreader - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxrssreader - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Release\wxrssreader"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Release\wxrssreader"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /w /W0 /GR /O2 /D "WIN32" /D "_UNICODE" /D "_WINDOWS" /Fd"..\..\bin\wxrssreader.pdb" /FD /EHsc /c
# ADD CPP /nologo /MD /w /W0 /GR /O2 /D "WIN32" /D "_UNICODE" /D "_WINDOWS" /Fd"..\..\bin\wxrssreader.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "_WINDOWS"
# ADD RSC /l 0x409 /d "_UNICODE" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib wxmsw27u_core.lib wxbase27u.lib wxbase27u_net.lib wxmsw27u_adv.lib wxmsw27u_html.lib wxregex.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib wxmsw27u_core.lib wxbase27u.lib wxbase27u_net.lib wxmsw27u_adv.lib wxmsw27u_html.lib wxregex.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "wxrssreader - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Debug\wxrssreader"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Debug\wxrssreader"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\wxrssreader.pdb" /FD /EHsc /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\wxrssreader.pdb" /FD /EHsc /GZ /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /d "_WINDOWS"
# ADD RSC /l 0x409 /d "_UNICODE" /d "_DEBUG" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib wxmsw27ud_core.lib wxbase27ud.lib wxbase27ud_net.lib wxmsw27ud_adv.lib wxmsw27ud_html.lib wxregex.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib wxmsw27ud_core.lib wxbase27ud.lib wxbase27ud_net.lib wxmsw27ud_adv.lib wxmsw27ud_html.lib wxregex.lib /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "wxrssreader - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Release\wxrssreader"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Release\wxrssreader"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /w /W0 /GR /O2 /D "WIN32" /D "_WINDOWS" /Fd"..\..\bin\wxrssreader.pdb" /FD /EHsc /c
# ADD CPP /nologo /MD /w /W0 /GR /O2 /D "WIN32" /D "_WINDOWS" /Fd"..\..\bin\wxrssreader.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS"
# ADD RSC /l 0x409 /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib wxmsw27_core.lib wxbase27.lib wxbase27_net.lib wxmsw27_adv.lib wxmsw27_html.lib wxregex.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib wxmsw27_core.lib wxbase27.lib wxbase27_net.lib wxmsw27_adv.lib wxmsw27_html.lib wxregex.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "wxrssreader - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Debug\wxrssreader"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Debug\wxrssreader"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\wxrssreader.pdb" /FD /EHsc /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\wxrssreader.pdb" /FD /EHsc /GZ /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_WINDOWS"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib wxmsw27d_core.lib wxbase27d.lib wxbase27d_net.lib wxmsw27d_adv.lib wxmsw27d_html.lib wxregexd.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib wxmsw27d_core.lib wxbase27d.lib wxbase27d_net.lib wxmsw27d_adv.lib wxmsw27d_html.lib wxregexd.lib /nologo /subsystem:windows /debug /machine:I386

!ENDIF 

# Begin Target

# Name "wxrssreader - Win32 Unicode Release"
# Name "wxrssreader - Win32 Unicode Debug"
# Name "wxrssreader - Win32 Release"
# Name "wxrssreader - Win32 Debug"
# Begin Group "wxRSSReader Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\..\src\common\wxrssreader.cpp
# End Source File
# End Group
# End Target
# End Project
