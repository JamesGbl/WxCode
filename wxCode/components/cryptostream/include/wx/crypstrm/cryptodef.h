/////////////////////////////////////////////////////////////////////////////
// Name:        cryptodef.h
// Purpose:     Shared build defines
// Author:      Giovanni Mittone
// Created:     2009/01/11
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Giovanni Mittone
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef CRYPTO_DEFS_H
#define CRYPTO_DEFS_H


// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxCRYPTOSTREAM_MAJOR          1
#define wxCRYPTOSTREAM_MINOR          0
#define wxCRYPTOSTREAM_RELEASE        0

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_CRYPTOSTREAM_VERSION(major,minor,release) \
    (wxCRYPTOSTREAM_MAJOR > (major) || \
    (wxCRYPTOSTREAM_MAJOR == (major) && wxCRYPTOSTREAM_MINOR > (minor)) || \
    (wxCRYPTOSTREAM_MAJOR == (major) && wxCRYPTOSTREAM_MINOR == (minor) && wxCRYPTOSTREAM_RELEASE >= (release)))

// for shared builds
#ifdef WXMAKINGDLL_CRYPTOSTREAM
    #define WXDLLIMPEXP_CRYPTOSTREAM                WXEXPORT
    #define WXDLLIMPEXP_DATA_CRYPTOSTREAM(type)     WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_CRYPTOSTREAM                WXIMPORT
    #define WXDLLIMPEXP_DATA_CRYPTOSTREAM(type)     WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_CRYPTOSTREAM
    #define WXDLLIMPEXP_DATA_CRYPTOSTREAM(type)	    type
#endif


#endif // CRYPTO_DEFS_H
