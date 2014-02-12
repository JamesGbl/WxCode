/////////////////////////////////////////////////////////////////////////////
// Name:        stringformatter.h
// Purpose:     shared build defines
// Author:      Nathan Shaffer
// Created:     someyear/somemonth/someday
// RCS-ID:      $Id: stringformatterdef.h 505 2007-03-31 10:31:46Z frm $
// Copyright:   (c) 2013 Nathan Shaffer
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_STRING_FORMATTER_DEFS_H_
#define _WX_STRING_FORMATTER_DEFS_H_

// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxSTRING_FORMATTER_MAJOR          1
#define wxSTRING_FORMATTER_MINOR          0
#define wxSTRING_FORMATTER_RELEASE        1

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_STRING_FORMATTER_VERSION(major,minor,release) \
    (wxSTRING_FORMATTER_MAJOR > (major) || \
    (wxSTRING_FORMATTER_MAJOR == (major) && wxSTRING_FORMATTER_MINOR > (minor)) || \
    (wxSTRING_FORMATTER_MAJOR == (major) && wxSTRING_FORMATTER_MINOR == (minor) && wxSTRING_FORMATTER_RELEASE >= (release)))


// Defines for shared builds.
// Simple reference for using these macros and for writin components
// which support shared builds:
//
// 1) use the WXDLLIMPEXP_STRING_FORMATTER in each class declaration:
//          class WXDLLIMPEXP_STRING_FORMATTER myCompClass {   [...]   };
//
// 2) use the WXDLLIMPEXP_STRING_FORMATTER in the declaration of each global function:
//          WXDLLIMPEXP_STRING_FORMATTER int myGlobalFunc();
//
// 3) use the WXDLLIMPEXP_DATA_STRING_FORMATTER() in the declaration of each global
//    variable:
//          WXDLLIMPEXP_DATA_STRING_FORMATTER(int) myGlobalIntVar;
//
#ifdef WXMAKINGDLL_STRING_FORMATTER
    #define WXDLLIMPEXP_STRING_FORMATTER                  WXEXPORT
    #define WXDLLIMPEXP_DATA_STRING_FORMATTER(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_STRING_FORMATTER                  WXIMPORT
    #define WXDLLIMPEXP_DATA_STRING_FORMATTER(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_STRING_FORMATTER
    #define WXDLLIMPEXP_DATA_STRING_FORMATTER(type)	    type
#endif

#endif // _WX_STRING_FORMATTER_DEFS_H_

