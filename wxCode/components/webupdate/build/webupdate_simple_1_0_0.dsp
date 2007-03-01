# Microsoft Developer Studio Project File - Name="webupdate_simple_1_0_0" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=simple_1_0_0 - Win32 Static ANSI Release Multilib
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "webupdate_simple_1_0_0.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "webupdate_simple_1_0_0.mak" CFG="simple_1_0_0 - Win32 Static ANSI Release Multilib"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "simple_1_0_0 - Win32 DLL Unicode Debug Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 DLL Unicode Debug Multilib" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 DLL Unicode Release Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 DLL Unicode Release Multilib" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 DLL ANSI Debug Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 DLL ANSI Debug Multilib" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 DLL ANSI Release Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 DLL ANSI Release Multilib" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 Static Unicode Debug Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 Static Unicode Debug Multilib" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 Static Unicode Release Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 Static Unicode Release Multilib" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 Static ANSI Debug Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 Static ANSI Debug Multilib" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 Static ANSI Release Monolithic" (based on "Win32 (x86) Application")
!MESSAGE "simple_1_0_0 - Win32 Static ANSI Release Multilib" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "simple_1_0_0 - Win32 DLL Unicode Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmswud_dll\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmswud_dll\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290ud.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290ud.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 DLL Unicode Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmswud_dll\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmswud_dll\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290ud_core.lib wxbase290ud.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290ud_core.lib wxbase290ud.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 DLL Unicode Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmswu_dll\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmswu_dll\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290u.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290u.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 DLL Unicode Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmswu_dll\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmswu_dll\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290u_core.lib wxbase290u.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290u_core.lib wxbase290u.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 DLL ANSI Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmswd_dll\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmswd_dll\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290d.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290d.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 DLL ANSI Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmswd_dll\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmswd_dll\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290d_core.lib wxbase290d.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290d_core.lib wxbase290d.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 DLL ANSI Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmsw_dll\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmsw_dll\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 DLL ANSI Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmsw_dll\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmsw_dll\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290_core.lib wxbase290.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290_core.lib wxbase290.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 Static Unicode Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmswud\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmswud\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290ud.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290ud.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 Static Unicode Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmswud\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmswud\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290ud_core.lib wxbase290ud.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290ud_core.lib wxbase290ud.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 Static Unicode Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmswu\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmswu\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290u.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290u.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 Static Unicode Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmswu\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmswu\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290u_core.lib wxbase290u.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290u_core.lib wxbase290u.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 Static ANSI Debug Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmswd\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmswd\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290d.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290d.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 Static ANSI Debug Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmswd\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmswd\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290d_core.lib wxbase290d.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290d_core.lib wxbase290d.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 Static ANSI Release Monolithic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmsw\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmsw\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "simple_1_0_0 - Win32 Static ANSI Release Multilib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "samples\simple\v1.0.0"
# PROP BASE Intermediate_Dir "vcmsw\simple_1_0_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "samples\simple\v1.0.0"
# PROP Intermediate_Dir "vcmsw\simple_1_0_0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /D "WIN32" /D "_WINDOWS" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
# ADD RSC /l 0x409 /d "_WINDOWS" /i samples\simple\v1.0.0
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw290_core.lib wxbase290.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows
# ADD LINK32 wxmsw290_core.lib wxbase290.lib /nologo /machine:i386 /out:"samples\simple\v1.0.0\simple.exe" /subsystem:windows

!ENDIF

# Begin Target

# Name "simple_1_0_0 - Win32 DLL Unicode Debug Monolithic"
# Name "simple_1_0_0 - Win32 DLL Unicode Debug Multilib"
# Name "simple_1_0_0 - Win32 DLL Unicode Release Monolithic"
# Name "simple_1_0_0 - Win32 DLL Unicode Release Multilib"
# Name "simple_1_0_0 - Win32 DLL ANSI Debug Monolithic"
# Name "simple_1_0_0 - Win32 DLL ANSI Debug Multilib"
# Name "simple_1_0_0 - Win32 DLL ANSI Release Monolithic"
# Name "simple_1_0_0 - Win32 DLL ANSI Release Multilib"
# Name "simple_1_0_0 - Win32 Static Unicode Debug Monolithic"
# Name "simple_1_0_0 - Win32 Static Unicode Debug Multilib"
# Name "simple_1_0_0 - Win32 Static Unicode Release Monolithic"
# Name "simple_1_0_0 - Win32 Static Unicode Release Multilib"
# Name "simple_1_0_0 - Win32 Static ANSI Debug Monolithic"
# Name "simple_1_0_0 - Win32 Static ANSI Debug Multilib"
# Name "simple_1_0_0 - Win32 Static ANSI Release Monolithic"
# Name "simple_1_0_0 - Win32 Static ANSI Release Multilib"
# Begin Group "WebUpdater stuff"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\webupdatedlg.xrc
# End Source File
# Begin Source File

SOURCE=..\..\src\webupdater.exe
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\samples\simple\v1.0.0\minimal.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\simple\v1.0.0\minimal.rc
# End Source File
# End Group
# End Target
# End Project

