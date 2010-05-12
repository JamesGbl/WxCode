//! \file mmwxdef.h
/////////////////////////////////////////////////////////////////////////////
// Name:        mmwxdef.h
// Purpose:     shared build defines
// Author:      Gary Harris
// Created:     2010/5/11
// RCS-ID:      $Id: mmwxdef.h 505 2007-03-31 10:31:46Z frm $
// Copyright:   (c) 2010 Gary Harris
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_MMWX_DEFS_H_
#define _WX_MMWX_DEFS_H_

// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxMMWX_MAJOR          2
#define wxMMWX_MINOR           0
#define wxMMWX_RELEASE      0

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_MMWX_VERSION(major,minor,release) \
    (wxMMWX_MAJOR > (major) || \
    (wxMMWX_MAJOR == (major) && wxMMWX_MINOR > (minor)) || \
    (wxMMWX_MAJOR == (major) && wxMMWX_MINOR == (minor) && wxMMWX_RELEASE >= (release)))


// Defines for shared builds.
// Simple reference for using these macros and for writing components
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
#ifdef WXMAKINGDLL_MMWX
    #define WXDLLIMPEXP_MMWX                  WXEXPORT
    #define WXDLLIMPEXP_DATA_MMWX(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_MMWX                  WXIMPORT
    #define WXDLLIMPEXP_DATA_MMWX(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_MMWX
    #define WXDLLIMPEXP_DATA_MMWX(type)	    type
#endif


#endif // _WX_MMWX_DEFS_H_

