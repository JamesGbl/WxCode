
/*
 * util.h
 * ______________________________________________________________
 * This software is dual licensed under either the GNU GPL
 * version 2 or at your option any other version
 * ______________________________________________________________
 * or the wxWindows license.
 * Just a quick reminder of what that means:
 *
 * This software is released under the GNU GPL licence
 * with a few exceptins applied, check the wxWindows licence
 * to see what those are.
 * ______________________________________________________________
 *
 * Visit: http://opensource.org/ to see both-
 * the GNU GPL and wxWindows licences.
 *
 * This software has absolutely no warranty, express or implied.
 *
 * Just so you know, I don't care if you change the code
 * don't email me if you did someting to it.
 *
 * If you edit a function to change its behavior, it would be courtious
 * to others to let them know that the file is not an official release,
 * but you don't have to do that either.
 *
 * You must not misrepresent the origins of this software, if you distribute
 * it, let the user know where to get it and that you where not the original
 * creator. (Except for any code you add obviously)
 *
 * This notice may not be changed in any way and must remain at the top of every
 * source file.
 *
 * XSTC was developed by Nuklear Zelph
 * copyright (C) 2006
 */

#ifndef UTIL_H
#define UTIL_H

#ifndef XSTC_NO_CONF
 #include "xstc_conf.h"
#endif

#ifdef XSTC_USE_WXSCINTILLA

 #define XSTC_CLASS wxScintilla
 #include <wx/wxscintilla.h>
 #define XSTC_EVENT wxScintillaEvent
 #define XSTC_EVENT_FUNCT wxScintillaEventFunction
 #define XSTC_EVENT_DEF(Cx)  wxEVT_SCI_ ## Cx
 #define XSTC_DEF(Cy) wxSCI_ ## Cy

//These macros define a neutral function that is both version and library independant. They do not
//conform to the actual library function names, but using them will make sure you don't need to
//worry about which version of library code you are using.

#define SetCaretBk SetCaretLineBackground
#define GetCaretBk GetCaretLineBackground

#ifndef XSTC_NO_ALPHA
 #define GetCaretALPHA GetCaretLineBackgroundAlpha
 #define SetCaretALPHA SetCaretLineBackgroundAlpha
#endif

#endif //XSTC_USE_WXSCINTILLA
//Above macros are for wxScintilla, do not edit this section!

#ifdef XSTC_USE_WXSTC

 #define XSTC_CLASS wxStyledTextCtrl
 #include <wx/stc/stc.h>
 #define XSTC_EVENT wxStyledTextEvent
 #define XSTC_EVENT_FUNCT wxStyledTextEventFunction
 #define XSTC_EVENT_DEF(Cx) wxEVT_STC_ ## Cx
 #define XSTC_DEF(Cy) wxSTC_ ## Cy

//wxStyledTextCtrl does not support these languages

 //Scintilla 1.77
 #define XSTC_NO_ASYMPTOTE
 #define XSTC_NO_R
 #define XSTC_NO_MAGIK
 #define XSTC_NO_POWERSHELL
 #define XSTC_NO_MYSQL
 #define XSTC_NO_PO
 //scintilla 1.78
 #define XSTC_NO_TAL
 #define XSTC_NO_COBOL
 #define XSTC_NO_TACL
 #define XSTC_NO_SORCUS
 #define XSTC_NO_POWERPRO
 #define XSTC_NO_NIMROD
 #define XSTC_NO_SML
 //scintilla 2.20
 #define XSTC_NO_MARKDOWN
 #define XSTC_NO_TXT2TAGS
 
 #if wxMAJOR_VERSION == 2
 #if wxMINOR_VERSION < 9
 //these languages are present in 2.9 but not 2.8
 #define XSTC_NO_ABAQUS
 #define XSTC_NO_CMAKE
 #define XSTC_NO_D
 #define XSTC_NO_GAP
 #define XSTC_NO_PLM
 #define XSTC_NO_PROGRESS
 #endif
 #endif
 

 #define XSTC_NO_MARK_AVAILABLE
 #define XSTC_NO_MARK_LEFTRECT

#if wxMAJOR_VERSION <= 2
#if wxMINOR_VERSION > 6
 #define XSTC_LVL //Tells us if we need to use the new or old version of the function names.
#endif
#endif

//These macros define a neutral function that is both version and library independant. They do not
//conform to the actual library function names, but using them will make sure you don't need to
//worry about which version of library code you are using.

#ifdef XSTC_LVL
 #define SetCaretBk SetCaretLineBackground
 #define GetCaretBk GetCaretLineBackground
#ifndef XSTC_NO_ALPHA
 #define GetCaretALPHA GetCaretLineBackAlpha
 #define SetCaretALPHA SetCaretLineBackAlpha
#endif //XSTC_NO_ALPHA
#else
 #define SetCaretBk SetCaretLineBack
 #define GetCaretBk GetCaretLineBack
#endif //XSTC_LVL

#undef XSTC_LVL

#endif //XSTC_USE_WXSTC
//Above macros are for wxStyledTextCtrl, do not edit this section!

#endif //UTIL_H
