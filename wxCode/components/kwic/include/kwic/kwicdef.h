/////////////////////////////////////////////////////////////////////////////
// Name:        kwicdef.h
// Purpose:     shared build defines
// Author:      Francesco Montorsi
// Created:     2005/07/10
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_KWIC_DEFS_H_
#define _WX_KWIC_DEFS_H_

// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxKWIC_MAJOR          0
#define wxKWIC_MINOR          3
#define wxKWIC_RELEASE        1

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_KWIC_VERSION(major,minor,release) \
    (wxKWIC_MAJOR > (major) || \
    (wxKWIC_MAJOR == (major) && wxKWIC_MINOR > (minor)) || \
    (wxKWIC_MAJOR == (major) && wxKWIC_MINOR == (minor) && wxKWIC_RELEASE >= (release)))

// for shared builds
#ifdef WXMAKINGDLL_KWIC
    #define WXDLLIMPEXP_KWIC                  WXEXPORT
    #define WXDLLIMPEXP_DATA_KWIC(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_KWIC                  WXIMPORT
    #define WXDLLIMPEXP_DATA_KWIC(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_KWIC
    #define WXDLLIMPEXP_DATA_KWIC(type)	    type
#endif

#endif // _WX_KWIC_DEFS_H_

