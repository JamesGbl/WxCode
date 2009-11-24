
/*
 * util.h
 *
 * this file is licensed under the wxWindows licence
 * just a quick reminder of what that means:
 *
 * This software is released under the GNU GPL licence
 * with a few exceptins applied, check the wxWindows licence
 * to see what those are
 *
 * visit: http://opensource.org/ to see both
 * the GNU GPL and wxWindows licences.
 *
 * this software has absolutely no warranty, express or implied
 *
 * just so you know, i don't care if you change the code
 * don't email me if you did someting to it.
 *
 * no need to mark changes, you obviously may want to change the color
 * settings, it would only be a real pain if you had to mark them.
 *
 * if you edit a function to change its behavior, it would be courtious
 * to others to let them know that the file is not an official release,
 * but you don't have to do that either.
 *
 * you must not misrepresent the origins of this software, if you distribute
 * it, let the user know where to get it and that you where not the original
 * creator. (except for any code you add obviously)
 *
 * this notice may not be changed in any way and must remain at the top of every
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

//these macros define a neutral function that is both version and library independant. they do not
//conform to the actual library function names, but using them will make sure you don't need to
//worry about which version of library code you are using.

#define SetCaretBk SetCaretLineBackground
#define GetCaretBk GetCaretLineBackground

#ifndef XSTC_NO_ALPHA
 #define GetCaretALPHA GetCaretLineBackgroundAlpha
 #define SetCaretALPHA SetCaretLineBackgroundAlpha
#endif

#endif //XSTC_USE_WXSCINTILLA
//above macros are for wxScintilla, do not edit this section

#ifdef XSTC_USE_WXSTC

 #define XSTC_CLASS wxStyledTextCtrl
 #include <wx/stc/stc.h>
 #define XSTC_EVENT wxStyledTextEvent
 #define XSTC_EVENT_FUNCT wxStyledTextEventFunction
 #define XSTC_EVENT_DEF(Cx) wxEVT_STC_ ## Cx
 #define XSTC_DEF(Cy) wxSTC_ ## Cy

//wxStyledTextCtrl does not support these languages

//scintilla 1.77 not all of these where added to scintilla then, i need to check it out
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

 #define XSTC_NO_MARK_AVAILABLE
 #define XSTC_NO_MARK_LEFTRECT

//these macros define a neutral function that is both version and library independant. they do not
//conform to the actual library function names, but using them will make sure you don't need to
//worry about which version of library code you are using.


#if wxMAJOR_VERSION <= 2
#if wxMINOR_VERSION > 6
 #define XSTC_LVL
#endif
#endif

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
//above macros are for wxStyledTextCtrl, do not edit this section

#endif //UTIL_H
