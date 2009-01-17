
/*
 * xstc_conf.h
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
 
#ifndef XSTC_CONF_H
#define XSTC_CONF_H

/****************************************************************************************************
 *                                                                                                  *
 *                                                                                                  *
 *   This block explains the macros below, there are comments by macro definitions to help as well  *
 *                                                                                                  *
 *  NOTE: most of the macros defined are to disable functionality rather than enable it.            *
 *                                                                                                  *
 *                                                                                                  *
 *  WXMAKINGDLL standard wxWidgets macro, define this if you are building the library. if not       *
 *              comment it out.                                                                     *
 *                                                                                                  *
 *  XSTC_USE_WXSCINTILLA   these macros are all you will need to make the library work, unless you  *
 *  XSTC_USE_WXSTC         want to redefine the libraries functionality                             *
 *                                                                                                  *
 *  XSTC_CLASS this macro defines the class used. either wxScintilla or wxStyledTextCtrl            *
 *             it is defined automatically unless you set XSTC_NO_CONF at the command line          *
 *                                                                                                  *
 *  XSTC_EVENT this macro defines the event type, thus wxStyledTextEvent                            *
 *                                                                                                  *
 *  XSTC_EVENT_DEF() this macro concatonates the correct event prefix wxEVT_STC_ or wxEVT_SCI_      *
 *                   this way the library and anyone who uses these macros are library independant  *
 *                                                                                                  *
 *  XSTC_DEF() same as the above, but this is for everything else, so use it for style definitions  *
 *             to make your code library independant. (that is why these macros where created.      *
 *                                                                                                  *
 *  XSTC_USE_CONFIG define this macro if you want XSTC to utilize an X.conf style                   *
 *                  auto-configuration method. otherwise you will have to decide and impliment the  *
 *                  lexers and color settings per document type.                                    *
 *                                                                                                  *
 *  XSTC_USE_REG this is only availible on windows. is is set automatically, but comment the line   *
 *               if you don't want to use it.                                                       *
 *                                                                                                  *
 *  XSTC_USE_XML this is an alternative to the configuration method if you prefer xml this is the   *
 *               way to go. it can be used in conjunction with the other, but is treaded as 2nd     *
 *               priority. it is based on TinyXML and you will need to supply the library yourself  *
 *                                                                                                  *
 *  XSTC_NO_KEYS if defined this will remove all automatic keyword initialization functionality     *
 *               from XSTC. you will have to deal with all keywords code yourself. utilizing this   *
 *               does not limit you from reinitializing the keyword arrays to something you need    *
 *               instead.                                                                           *
 *                                                                                                  *
 *  XSTC_NO_KEYS_DEFINED this leaves the keyword subsystem in place, but does not do any of the     *
 *                       automatic language initialization, that means they will all be empty and   *
 *                       you will be responsible for initializing them yourself.                    *
 *                                                                                                  *
 *  XSTC_NO_ALPHA disables alpha functionality for versions that do not support it.                 *
 *                                                                                                  *
 *  XSTC_NO_TABSPACE disables the utility function for converting between spaces and tabs.          *
 *                   and anywhere it is used in the code. also removes spaceconv and spaces         *
 *                   variables from the codebase.                                                   *
 *                                                                                                  *
 *  XSTC_NO_TRIMTRAIL disables the utility function for trimming trailing whitespace st end of      *
 *                    files and all instances in the code. also removes trimspaces varible.         *
 *                                                                                                  *
 *  XSTC_NO_MARK_FULLRECT scintilla 1.69 added this style.                                          *
 *                                                                                                  *
 *  XSTC_NO_[LEX]NAME this allows lexer setup functions to be removed and not utilized in the       *
 *                       codebase. that way the code can support current scintilla and still work   *
 *                       for older versions. use the function name minus the lex prefix.            *
 *                       //XSTC_NO_PASCAL                                                           *
 *                                                                                                  *
 *  XSTC_H_FLAT a commandline macro to allow compiling with the header in the same directory as     *
 *              the sources.                                                                        *
 *                                                                                                  *
 *  GetCaretALPHA an api independant way to get Caret line highlight alpha level.                   *
 *                                                                                                  *
 *  SetCaretALPHA an api independant way to set Caret line highlight alpha level.                   *
 *                                                                                                  *
 *  SetCaretBk an api independant way to set the color for caret line highlight.                    *
 *                                                                                                  *
 *  GetCaretBk an api independant way to get the color for caret line highlight.                    *
 *                                                                                                  *
 *  XSTC_NO_AUTOSAVEAS if you want to deal with all of the save errors yourself, define this and    *
 *                     no save dialog will be shown when saving fails, an errorcode will always     *
 *                     be passed instead.                                                           *
 *                                                                                                  *
 *                                                                                                  *
 ****************************************************************************************************/

#define WXMAKINGDLL //define this if you are building the library

#ifdef WXMAKINGDLL
 #define WXDLLIMPEXP_XSTC WXEXPORT
#else
 #define WXDLLIMPEXP_XSTC
#endif

#define XSTC_USE_WXSCINTILLA //define this if you are using wxScintilla
//#define XSTC_USE_WXSTC       //define this if you are using wxStyledTextCtrl
//define one or the other

#ifdef XSTC_USE_WXSCINTILLA
 #define XSTC_CLASS wxScintilla
 #include <wx/wxscintilla.h>
 #define XSTC_EVENT wxScintillaEvent
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
 #define XSTC_EVENT_DEF(Cx) wxEVT_STC_ ## Cx
 #define XSTC_DEF(Cy) wxSTC_ ## Cy

//wxStyledTextCtrl does not support these languages

 #define XSTC_NO_D
 #define XSTC_NO_CMAKE
 #define XSTC_NO_GAP
 #define XSTC_NO_PLM
 #define XSTC_NO_PROGRESS
 #define XSTC_NO_ABAQUS
 #define XSTC_NO_ASYMPTOTE
 #define XSTC_NO_R
 #define XSTC_NO_MAGIK
 #define XSTC_NO_POWERSHELL
 #define XSTC_NO_MYSQL
 #define XSTC_NO_PO

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

#define XSTC_USE_CONFIG //use the wxWidgets configuration class

#ifdef XSTC_USE_CONFIG
 #ifdef _WXMSW_
  #define XSTC_USE_REG //if we are under windows the registry configuration system is also availible
 #endif //_WXMSW_
#endif //XSTC_USE_CONFIG

#define XSTC_USE_XML //uses TinyXML for xml based configuration
//note, the configureation systems are made availible, but the user must set one up or nothing will be used.
//if both options are turned on, both subsystems will be used simultaniously
//this may be useful since either file can have any number of uniqe settings and the user has access to the
//config systems, so main application settings could be stored in a conf or reg and the coloring in an xml file.

//#define XSTC_NO_KEYS //disables all automatic keyword functionality
//#define XSTC_NO_KEYS_DEFINED //only disables setting the keyword arrays initially, this does not interfere with the subsystem.

//#define XSTC_NO_TABSPACE //disable the tabs/spaces conversion utility function
//#define XSTC_NO_TRIMTRAIL //disables the trim trailing whitespace utility functin and all instances it is used in the code.
//#define XSTC_NO_MARK_FULLRECT //some older versions of scintilla don't support this marker
//#define XSTC_NO_AUTOSAVEAS //disables the auto saveas functionality in savefilex
                             //the saveas variable will be ignored

#endif //XSTC_CONF_H
