# Microsoft Developer Studio Project File - Name="wxscintilla" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wxscintilla - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "wxscintilla.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "wxscintilla.mak" CFG="wxscintilla - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "wxscintilla - Win32 DebugUniv" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscintilla - Win32 ReleaseUniv" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscintilla - Win32 DebugDev" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscintilla - Win32 ReleaseDev" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscintilla - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxscintilla - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugUniv"
# PROP BASE Intermediate_Dir "DebugUniv"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugUniv"
# PROP Intermediate_Dir "DebugUniv"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "$(WXDEVEL)\include" /I "..\include" /I "$(WXDEVEL)\lib\vc_lib\mswunivd" /I "..\src\scintilla\include" /I "..\src\scintilla\src" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "__WX__" /D "SCI_LEXER" /D "LINK_LEXERS" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "$(WXDEVEL)\include" /I "..\include" /I "$(WXDEVEL)\lib\vc_lib\mswunivd" /I "..\src\scintilla\include" /I "..\src\scintilla\src" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "__WX__" /D "SCI_LEXER" /D "LINK_LEXERS" /FD /GZ /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscintillaunivd.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscintillaunivd.lib"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUniv"
# PROP BASE Intermediate_Dir "ReleaseUniv"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUniv"
# PROP Intermediate_Dir "ReleaseUniv"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /O1 /GR /GX /I "$(WXDEVEL)\include" /I "..\include" /I "$(WXDEVEL)\lib\vc_lib\mswuniv" /W4 /I "..\src\scintilla\include" /I "..\src\scintilla\src" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WX__" /D "SCI_LEXER" /D "LINK_LEXERS" /c
# ADD CPP /nologo /FD /MD /O1 /GR /GX /I "$(WXDEVEL)\include" /I "..\include" /I "$(WXDEVEL)\lib\vc_lib\mswuniv" /W4 /I "..\src\scintilla\include" /I "..\src\scintilla\src" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WX__" /D "SCI_LEXER" /D "LINK_LEXERS" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscintillauniv.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscintillauniv.lib"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugDev"
# PROP BASE Intermediate_Dir "DebugDev"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDev"
# PROP Intermediate_Dir "DebugDev"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Od /GR /GX /I "$(WXDEVEL)\include" /I "..\include" /I "$(WXDEVEL)\lib\vc_lib\mswd" /W4 /Zi /Gm /GZ /I "..\src\scintilla\include" /I "..\src\scintilla\src" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "__WX__" /D "SCI_LEXER" /D "LINK_LEXERS" /c
# ADD CPP /nologo /FD /MDd /Od /GR /GX /I "$(WXDEVEL)\include" /I "..\include" /I "$(WXDEVEL)\lib\vc_lib\mswd" /W4 /Zi /Gm /GZ /I "..\src\scintilla\include" /I "..\src\scintilla\src" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "__WX__" /D "SCI_LEXER" /D "LINK_LEXERS" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscintilladevd.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscintilladevd.lib"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseDev"
# PROP BASE Intermediate_Dir "ReleaseDev"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseDev"
# PROP Intermediate_Dir "ReleaseDev"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /O1 /GR /GX /I "$(WXDEVEL)\include" /I "..\include" /I "$(WXDEVEL)\lib\vc_lib\msw" /W4 /I "..\src\scintilla\include" /I "..\src\scintilla\src" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WX__" /D "SCI_LEXER" /D "LINK_LEXERS" /c
# ADD CPP /nologo /FD /MD /O1 /GR /GX /I "$(WXDEVEL)\include" /I "..\include" /I "$(WXDEVEL)\lib\vc_lib\msw" /W4 /I "..\src\scintilla\include" /I "..\src\scintilla\src" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WX__" /D "SCI_LEXER" /D "LINK_LEXERS" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscintilladev.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscintilladev.lib"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "$(WXSTABLE)\include" /I "..\include" /I "$(WXSTABLE)\lib\mswd" /I "..\src\scintilla\include" /I "..\src\scintilla\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D DEBUG=1 /D "__WXDEBUG__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "SCI_LEXER" /D "LINK_LEXERS" /D "__WX__" /YX /FD /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscintillad.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscintillad.lib"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /Ob2 /I "$(WXSTABLE)\include" /I "..\include" /I "$(WXSTABLE)\lib\msw" /I "..\src\scintilla\include" /I "..\src\scintilla\src" /D "WIN32" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D "__HACK_MY_MSDEV40__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "SCI_LEXER" /D "LINK_LEXERS" /D "__WX__" /YX /FD /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\wxscintilla.lib"
# ADD LIB32 /nologo /out:"..\lib\wxscintilla.lib"

!ENDIF

# Begin Target

# Name "wxscintilla - Win32 DebugUniv"
# Name "wxscintilla - Win32 ReleaseUniv"
# Name "wxscintilla - Win32 DebugDev"
# Name "wxscintilla - Win32 ReleaseDev"
# Name "wxscintilla - Win32 Debug"
# Name "wxscintilla - Win32 Release"
# Begin Group "wxScintilla Src"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\src\PlatWX.cpp

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\ScintillaWX.cpp

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\wxscintilla.cpp

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# End Group
# Begin Group "wxScintilla Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\PlatWX.h
# End Source File
# Begin Source File

SOURCE=..\src\ScintillaWX.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\wxscintilla.h
# End Source File
# End Group
# Begin Group "Scintilla"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\scintilla\include\Accessor.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\AutoComplete.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\AutoComplete.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\CallTip.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\CallTip.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\CellBuffer.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\CellBuffer.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\ContractionState.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\ContractionState.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\Document.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\Document.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\DocumentAccessor.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\DocumentAccessor.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\Editor.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\Editor.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\Indicator.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\Indicator.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\KeyMap.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\KeyMap.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\KeyWords.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\include\KeyWords.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexAda.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexAPDL.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexAsm.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexAsn1.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexAU3.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexAVE.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexBaan.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexBash.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexBullant.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexCLW.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexConf.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexCPP.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexCrontab.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexCSS.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexEiffel.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexErlang.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexEScript.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexForth.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexFortran.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexGui4Cli.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexHTML.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexKix.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexLisp.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexLout.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexLua.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexMatlab.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexMetapost.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexMMIXAL.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexMPT.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexMSSQL.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexNsis.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexOthers.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexPascal.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexPB.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexPerl.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexPOV.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexPS.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexPython.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexRuby.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexScriptol.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexSpecman.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexSQL.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexTex.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexVB.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexVerilog.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexVHDL.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LexYAML.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LineMarker.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\LineMarker.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\include\Platform.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\PropSet.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\include\PropSet.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\RESearch.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\RESearch.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\include\SciLexer.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\include\Scintilla.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\ScintillaBase.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\ScintillaBase.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\include\ScintillaWidget.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\include\SString.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\Style.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\Style.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\StyleContext.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\StyleContext.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\SVector.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\UniConversion.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\UniConversion.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\ViewStyle.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\ViewStyle.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\WindowAccessor.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\include\WindowAccessor.h
# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\XPM.cxx

!IF  "$(CFG)" == "wxscintilla - Win32 DebugUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseUniv"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 DebugDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 ReleaseDev"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxscintilla - Win32 Release"

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\scintilla\src\XPM.h
# End Source File
# End Group
# End Target
# End Project
