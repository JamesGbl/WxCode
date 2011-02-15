/////////////////////////////////////////////////////////////////////////////
// Name:        mycomp.h
// Purpose:     shared build defines
// Author:      Name Surname
// Created:     someyear/somemonth/someday
// RCS-ID:      $Id$
// Copyright:   (c) YEAR Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_MYCOMP_DEFS_H_
#define _WX_MYCOMP_DEFS_H_

// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxMYCOMP_MAJOR          1
#define wxMYCOMP_MINOR          0
#define wxMYCOMP_RELEASE        0

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_MYCOMP_VERSION(major,minor,release) \
    (wxMYCOMP_MAJOR > (major) || \
    (wxMYCOMP_MAJOR == (major) && wxMYCOMP_MINOR > (minor)) || \
    (wxMYCOMP_MAJOR == (major) && wxMYCOMP_MINOR == (minor) && wxMYCOMP_RELEASE >= (release)))


// Defines for shared builds.
// Simple reference for using these macros and for writin components
// which support shared builds:
//
// 1) use the WXDLLIMPEXP_MYCOMP in each class declaration:
//          class WXDLLIMPEXP_MYCOMP myCompClass {   [...]   };
//
// 2) use the WXDLLIMPEXP_MYCOMP in the declaration of each global function:
//          WXDLLIMPEXP_MYCOMP int myGlobalFunc();
//
// 3) use the WXDLLIMPEXP_DATA_MYCOMP() in the declaration of each global
//    variable:
//          WXDLLIMPEXP_DATA_MYCOMP(int) myGlobalIntVar;
//
#ifdef WXMAKINGDLL_MYCOMP
    #define WXDLLIMPEXP_MYCOMP                  WXEXPORT
    #define WXDLLIMPEXP_DATA_MYCOMP(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_MYCOMP                  WXIMPORT
    #define WXDLLIMPEXP_DATA_MYCOMP(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_MYCOMP
    #define WXDLLIMPEXP_DATA_MYCOMP(type)	    type
#endif

#endif // _WX_MYCOMP_DEFS_H_

