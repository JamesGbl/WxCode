/////////////////////////////////////////////////////////////////////////////
// Name:        wx/avahi/avahidef.h
// Purpose:     shared build defines
// Author:      Kia Emilien
// Created:     2007/02/18
// RCS-ID:      $Id: avahidef.h,v 1.4 2006/03/07 18:53:42 frm Exp $
// Copyright:   (c) 2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef AVAHIDEF_H_
#define AVAHIDEF_H_

// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxAVAHI_MAJOR          0
#define wxAVAHI_MINOR          2
#define wxAVAHI_RELEASE        0

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_AVAHI_VERSION(major,minor,release) \
    (wxAVAHI_MAJOR > (major) || \
    (wxAVAHI_MAJOR == (major) && wxAVAHI_MINOR > (minor)) || \
    (wxAVAHI_MAJOR == (major) && wxAVAHI_MINOR == (minor) && wxAVAHI_RELEASE >= (release)))


// Defines for shared builds.
// Simple reference for using these macros and for writin components
// which support shared builds:
//
// 1) use the WXDLLIMPEXP_AVAHI in each class declaration:
//          class WXDLLIMPEXP_AVAHI myCompClass {   [...]   };
//
// 2) use the WXDLLIMPEXP_AVAHI in the declaration of each global function:
//          WXDLLIMPEXP_AVAHI int myGlobalFunc();
//
// 3) use the WXDLLIMPEXP_DATA_AVAHI() in the declaration of each global
//    variable:
//          WXDLLIMPEXP_DATA_AVAHI(int) myGlobalIntVar;
//
#ifdef WXMAKINGDLL_AVAHI
    #define WXDLLIMPEXP_AVAHI                  WXEXPORT
    #define WXDLLIMPEXP_DATA_AVAHI(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_AVAHI                  WXIMPORT
    #define WXDLLIMPEXP_DATA_AVAHI(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_AVAHI
    #define WXDLLIMPEXP_DATA_AVAHI(type)       type
#endif

#endif /*AVAHIDEF_H_*/
