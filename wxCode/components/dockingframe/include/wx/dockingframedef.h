/////////////////////////////////////////////////////////////////////////////
// Name:        mycomp.h
// Purpose:     shared build defines
// Author:      Name Surname
// Created:     someyear/somemonth/someday
// RCS-ID:      $Id: dockingframedef.h,v 1.1 2006-08-31 18:02:24 bart59 Exp $
// Copyright:   (c) YEAR Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_DOCKINGFRAME_DEFS_H_
#define _WX_DOCKINGFRAME_DEFS_H_

// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxDOCKINGFRAME_MAJOR          1
#define wxDOCKINGFRAME_MINOR          0
#define wxDOCKINGFRAME_RELEASE        0

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_DOCKINGFRAME_VERSION(major,minor,release) \
    (wxDOCKINGFRAME_MAJOR > (major) || \
    (wxDOCKINGFRAME_MAJOR == (major) && wxDOCKINGFRAME_MINOR > (minor)) || \
    (wxDOCKINGFRAME_MAJOR == (major) && wxDOCKINGFRAME_MINOR == (minor) && wxDOCKINGFRAME_RELEASE >= (release)))


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
#ifdef WXMAKINGDLL_DOCKINGFRAME
    #define WXDLLIMPEXP_DOCKINGFRAME                WXEXPORT
    #define WXDLLIMPEXP_DATA_DOCKINGFRAME(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_DOCKINGFRAME                 WXIMPORT
    #define WXDLLIMPEXP_DATA_DOCKINGFRAME(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_DOCKINGFRAME
    #define WXDLLIMPEXP_DATA_DOCKINGFRAME(type)	    type
#endif

#endif // _WX_DOCKINGFRAME_DEFS_H_

