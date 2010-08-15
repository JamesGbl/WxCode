
/*
 * xstc_conf.h
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

#ifndef XSTC_CONF_H
#define XSTC_CONF_H

/****************************************************************************************************
 *                                                                                                  *
 *                                                                                                  *
 *  This block explains the macros below, there are comments by macro definitions to help as well   *
 *                                                                                                  *
 *  NOTE: Most of the macros defined are to disable functionality rather than enable it.            *
 *                                                                                                  *
 *                                                                                                  *
 *  WXMAKINGDLL Standard wxWidgets macro, define this if you are building the library.              *
 *                                                                                                  *
 *  WXDLLIMPEXP_XSTC If WXMAKINGDLL is defined then wxEXPORT is used else it is nothing.            *
 *                                                                                                  *
 *  XSTC_USE_WXSCINTILLA  These macros are all you will need to make the library work, unless you   *
 *  XSTC_USE_WXSTC        want to cut out some of the libraries functionality                       *
 *                                                                                                  *
 *  XSTC_CLASS Defines the class used. Either wxScintilla or wxStyledTextCtrl.                      *
 *             It is defined automatically unless you set XSTC_NO_CONF at the command line          *
 *             You must modify uncomment the apropriate line, or change it for your                 *
 *             subclasses version.                                                                  *
 *                                                                                                  *
 *  XSTC_EVENT Defines the event type, ie wxStyledTextEvent                                         *
 *                                                                                                  *
 *  XSTC_EVENT_FUNCT Defines the event_function for connect()                                       *
 *                                                                                                  *
 *  XSTC_EVENT_DEF() Concatonates the correct event prefix wxEVT_STC_ or wxEVT_SCI_.                *
 *                   That way the library and anyone who uses these macros are library independant  *
 *                                                                                                  *
 *  XSTC_DEF() Same as the above, but this is for everything else, so use it for style definitions  *
 *             to make your code library independant. (That is why these macros where created.      *
 *                                                                                                  *
 *  XSTC_NO_KEYS If defined this will remove all automatic keyword initialization functionality     *
 *               from XSTC. You will have to deal with all keywords code yourself. Utilizing this   *
 *               does not limit you from reusing the keyword arrays.                                *
 *                                                                                                  *
 *  XSTC_NO_KEYS_DEFINED This leaves the keyword subsystem in place, but does not do any of the     *
 *                       automatic language initialization, that means they will all be empty and   *
 *                       you will be responsible for filling them yourself, they may be filenames.  *
 *                                                                                                  *
 *  XSTC_NO_ALPHA Disables alpha functionality for versions that do not support it.                 *
 *                                                                                                  *
 *  XSTC_NO_TABSPACE Disables the utility function for converting between spaces and tabs           *
 *                   and anywhere it is used in the code. Also removes spaceconv and spaces         *
 *                   variables from the codebase.                                                   *
 *                                                                                                  *
 *  XSTC_NO_TRIMTRAIL Disables the utility function for trimming trailing whitespace at end of      *
 *                    files and all instances in the code. Also removes trimspaces varible.         *
 *                                                                                                  *
 *  XSTC_NO_MARK_FULLRECT Scintilla 1.69 added this style.                                          *
 *                                                                                                  *
 *  XSTC_NO_MARK_LEFTRECT Another later added style. 1.78?                                          *
 *                                                                                                  *
 *  XSTC_NO_MARK_AVAILABLE Another later added style. 1.78?                                         *
 *                                                                                                  *
 *  XSTC_NO_[LEX]NAME This allows lexer setup functions to be removed and not utilized in the       *
 *                    codebase. That way the code can support current scintilla and still work      *
 *                    for older versions. Use the function name minus the lex prefix.               *
 *                    ie XSTC_NO_PASCAL                                                             *
 *                                                                                                  *
 *  XSTC_H_FLAT A commandline macro to allow compiling with the header in the same directory as     *
 *              the sources.                                                                        *
 *                                                                                                  *
 *  GetCaretALPHA An api independant way to get Caret line highlight alpha level.                   *
 *                                                                                                  *
 *  SetCaretALPHA An api independant way to set Caret line highlight alpha level.                   *
 *                                                                                                  *
 *  SetCaretBk An api independant way to set the color for caret line highlight.                    *
 *                                                                                                  *
 *  GetCaretBk An api independant way to get the color for caret line highlight.                    *
 *                                                                                                  *
 *  XSTC_NO_AUTOSAVEAS If you want to deal with all of the save errors yourself, define this and    *
 *                     no save dialog will be shown when saving fails, an errorcode will always     *
 *                     be passed instead, if save was unsucessful.                                  *
 *                                                                                                  *
 *  XSTC_NO_CONFIG Disables the config loading routines and removes them from the sources.          *
 *                                                                                                  *
 *                                                                                                  *
 ****************************************************************************************************/


//#define WXMAKINGDLL //define this if you are building the library

#ifdef WXMAKINGDLL
 #define WXDLLIMPEXP_XSTC WXEXPORT
 #define WXMAKINGDLL_SCI //this if for wxScintilla
 #define WXMAKINGDLL_STC //this is for wxstyledtextctrl
 #define SCI_LEXER //Tells Scintilla to build lexer modules and coloring support.
 #define LINK_LEXERS //Builds the lexers directly into the library and initializes them.
#else
 #define WXDLLIMPEXP_XSTC
#endif

#ifndef DEBUG
 #define NDEBUG
#endif

#define XSTC_USE_WXSCINTILLA //define this if you are using wxScintilla
//#define XSTC_USE_WXSTC       //define this if you are using wxStyledTextCtrl
//define one or the other

#endif //XSTC_CONF_H
