# Microsoft Developer Studio Project File - Name="rarstream_vc6_wxrar" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=wxrar - Win32 Debug ENDIAN_big
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rarstream_vc6_wxrar.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rarstream_vc6_wxrar.mak" CFG="wxrar - Win32 Debug ENDIAN_big"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxrar - Win32 DLL Unicode Release ENDIAN_little" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxrar - Win32 DLL Unicode Release ENDIAN_big" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxrar - Win32 DLL Unicode Debug ENDIAN_little" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxrar - Win32 DLL Unicode Debug ENDIAN_big" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxrar - Win32 DLL Release ENDIAN_little" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxrar - Win32 DLL Release ENDIAN_big" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxrar - Win32 DLL Debug ENDIAN_little" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxrar - Win32 DLL Debug ENDIAN_big" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxrar - Win32 Unicode Release ENDIAN_little" (based on "Win32 (x86) Static Library")
!MESSAGE "wxrar - Win32 Unicode Release ENDIAN_big" (based on "Win32 (x86) Static Library")
!MESSAGE "wxrar - Win32 Unicode Debug ENDIAN_little" (based on "Win32 (x86) Static Library")
!MESSAGE "wxrar - Win32 Unicode Debug ENDIAN_big" (based on "Win32 (x86) Static Library")
!MESSAGE "wxrar - Win32 Release ENDIAN_little" (based on "Win32 (x86) Static Library")
!MESSAGE "wxrar - Win32 Release ENDIAN_big" (based on "Win32 (x86) Static Library")
!MESSAGE "wxrar - Win32 Debug ENDIAN_little" (based on "Win32 (x86) Static Library")
!MESSAGE "wxrar - Win32 Debug ENDIAN_big" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxrar - Win32 DLL Unicode Release ENDIAN_little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Release\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Release\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "..\..\include" /d "_UNICODE" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d SILENT
# ADD RSC /l 0x409 /i "..\..\include" /d "_UNICODE" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d SILENT
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /pdb:"..\..\lib\wxrar.pdb"
# ADD LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /pdb:"..\..\lib\wxrar.pdb"

!ELSEIF  "$(CFG)" == "wxrar - Win32 DLL Unicode Release ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Release\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Release\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "..\..\include" /d "_UNICODE" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d SILENT
# ADD RSC /l 0x409 /i "..\..\include" /d "_UNICODE" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d SILENT
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /pdb:"..\..\lib\wxrar.pdb"
# ADD LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /pdb:"..\..\lib\wxrar.pdb"

!ELSEIF  "$(CFG)" == "wxrar - Win32 DLL Unicode Debug ENDIAN_little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Debug\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Debug\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "..\..\include" /d "_UNICODE" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d _DEBUG
# ADD RSC /l 0x409 /i "..\..\include" /d "_UNICODE" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /debug /pdb:"..\..\lib\wxrar.pdb"
# ADD LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /debug /pdb:"..\..\lib\wxrar.pdb"

!ELSEIF  "$(CFG)" == "wxrar - Win32 DLL Unicode Debug ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Debug\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Debug\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "..\..\include" /d "_UNICODE" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d _DEBUG
# ADD RSC /l 0x409 /i "..\..\include" /d "_UNICODE" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /debug /pdb:"..\..\lib\wxrar.pdb"
# ADD LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /debug /pdb:"..\..\lib\wxrar.pdb"

!ELSEIF  "$(CFG)" == "wxrar - Win32 DLL Release ENDIAN_little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Release\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Release\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "..\..\include" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d SILENT
# ADD RSC /l 0x409 /i "..\..\include" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d SILENT
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25_core.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /pdb:"..\..\lib\wxrar.pdb"
# ADD LINK32 wxmsw25_core.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /pdb:"..\..\lib\wxrar.pdb"

!ELSEIF  "$(CFG)" == "wxrar - Win32 DLL Release ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Release\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Release\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "..\..\include" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d SILENT
# ADD RSC /l 0x409 /i "..\..\include" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d SILENT
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25_core.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /pdb:"..\..\lib\wxrar.pdb"
# ADD LINK32 wxmsw25_core.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /pdb:"..\..\lib\wxrar.pdb"

!ELSEIF  "$(CFG)" == "wxrar - Win32 DLL Debug ENDIAN_little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Debug\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Debug\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "..\..\include" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d _DEBUG
# ADD RSC /l 0x409 /i "..\..\include" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /debug /pdb:"..\..\lib\wxrar.pdb"
# ADD LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /debug /pdb:"..\..\lib\wxrar.pdb"

!ELSEIF  "$(CFG)" == "wxrar - Win32 DLL Debug ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Debug\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Debug\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "..\..\include" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d _DEBUG
# ADD RSC /l 0x409 /i "..\..\include" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /debug /pdb:"..\..\lib\wxrar.pdb"
# ADD LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\..\lib\wxrar.dll" /implib:"..\..\lib\wxrarstub.lib" /debug /pdb:"..\..\lib\wxrar.pdb"

!ELSEIF  "$(CFG)" == "wxrar - Win32 Unicode Release ENDIAN_little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Release\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Release\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\wxrar.lib"
# ADD LIB32 /nologo /out:"..\..\lib\wxrar.lib"

!ELSEIF  "$(CFG)" == "wxrar - Win32 Unicode Release ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Release\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Release\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\wxrar.lib"
# ADD LIB32 /nologo /out:"..\..\lib\wxrar.lib"

!ELSEIF  "$(CFG)" == "wxrar - Win32 Unicode Debug ENDIAN_little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Debug\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Debug\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\wxrar.lib"
# ADD LIB32 /nologo /out:"..\..\lib\wxrar.lib"

!ELSEIF  "$(CFG)" == "wxrar - Win32 Unicode Debug ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Debug\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Debug\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\wxrar.lib"
# ADD LIB32 /nologo /out:"..\..\lib\wxrar.lib"

!ELSEIF  "$(CFG)" == "wxrar - Win32 Release ENDIAN_little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Release\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Release\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\wxrar.lib"
# ADD LIB32 /nologo /out:"..\..\lib\wxrar.lib"

!ELSEIF  "$(CFG)" == "wxrar - Win32 Release ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Release\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Release\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "..\..\include" /W4 /O2 /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\wxrar.lib"
# ADD LIB32 /nologo /out:"..\..\lib\wxrar.lib"

!ELSEIF  "$(CFG)" == "wxrar - Win32 Debug ENDIAN_little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Debug\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Debug\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\wxrar.lib"
# ADD LIB32 /nologo /out:"..\..\lib\wxrar.lib"

!ELSEIF  "$(CFG)" == "wxrar - Win32 Debug ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib"
# PROP BASE Intermediate_Dir "Debug\wxrar"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Debug\wxrar"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "..\..\include" /W4 /Od /Gm /Zi /Fd..\..\lib\wxrar.pdb /D "WIN32" /D "_LIB" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\wxrar.lib"
# ADD LIB32 /nologo /out:"..\..\lib\wxrar.lib"

!ENDIF

# Begin Target

# Name "wxrar - Win32 DLL Unicode Release ENDIAN_little"
# Name "wxrar - Win32 DLL Unicode Release ENDIAN_big"
# Name "wxrar - Win32 DLL Unicode Debug ENDIAN_little"
# Name "wxrar - Win32 DLL Unicode Debug ENDIAN_big"
# Name "wxrar - Win32 DLL Release ENDIAN_little"
# Name "wxrar - Win32 DLL Release ENDIAN_big"
# Name "wxrar - Win32 DLL Debug ENDIAN_little"
# Name "wxrar - Win32 DLL Debug ENDIAN_big"
# Name "wxrar - Win32 Unicode Release ENDIAN_little"
# Name "wxrar - Win32 Unicode Release ENDIAN_big"
# Name "wxrar - Win32 Unicode Debug ENDIAN_little"
# Name "wxrar - Win32 Unicode Debug ENDIAN_big"
# Name "wxrar - Win32 Release ENDIAN_little"
# Name "wxrar - Win32 Release ENDIAN_big"
# Name "wxrar - Win32 Debug ENDIAN_little"
# Name "wxrar - Win32 Debug ENDIAN_big"
# Begin Group "RAR Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\..\src\unrar\archive.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\arcread.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\cmddata.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\consio.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\crc.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\crypt.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\dll.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\encname.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\errhnd.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\extinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\extract.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\filcreat.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\file.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\filefn.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\filestr.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\find.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\getbits.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\global.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\int64.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\isnt.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\list.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\match.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\options.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\pathfn.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\rarvm.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\rawread.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\rdwrfn.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\resource.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\rijndael.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\savepos.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\scantree.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\sha1.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\smallfn.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\strfn.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\strlist.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\system.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\timefn.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\ulinks.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\unicode.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\unpack.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\volume.cpp
# End Source File
# End Group
# Begin Group "RAR Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\..\src\unrar\archive.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\arcread.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\cmddata.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\consio.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\crc.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\crypt.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\dll.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\encname.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\errhnd.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\extinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\extract.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\filcreat.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\file.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\filefn.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\find.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\getbits.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\global.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\int64.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\isnt.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\list.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\match.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\options.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\pathfn.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\rarvm.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\rawread.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\rdwrfn.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\resource.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\rijndael.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\savepos.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\sha1.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\smallfn.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\strfn.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\strlist.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\system.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\timefn.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\ulinks.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\unicode.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\unpack.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\unrar\volume.hpp
# End Source File
# End Group
# End Target
# End Project

