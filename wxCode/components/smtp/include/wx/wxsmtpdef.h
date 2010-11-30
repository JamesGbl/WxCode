/****************************************************************************

 Project     :
 Author      :
 Description :

 VERSION INFORMATION:
 File    : $Source$
 Version : $Revision$
 Date    : $Date$
 Author  : $Author$
 Licence : wxWidgets licence

****************************************************************************/

#ifndef _WX_SMTP_DEFS_H_
#define _WX_SMTP_DEFS_H_

// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxSMTP_MAJOR          1
#define wxSMTP_MINOR          13
#define wxSMTP_RELEASE        0

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_SMTP_VERSION(major,minor,release) \
    (wxSMTP_MAJOR > (major) || \
    (wxSMTP_MAJOR == (major) && wxSMTP_MINOR > (minor)) || \
    (wxSMTP_MAJOR == (major) && wxSMTP_MINOR == (minor) && wxSMTP_RELEASE >= (release)))


// Defines for shared builds.
// Simple reference for using these macros and for writin components
// which support shared builds:
//
// 1) use the WXDLLIMPEXP_SMTP in each class declaration:
//          class WXDLLIMPEXP_SMTP myCompClass {   [...]   };
//
// 2) use the WXDLLIMPEXP_SMTP in the declaration of each global function:
//          WXDLLIMPEXP_MYCOMP int myGlobalFunc();
//
// 3) use the WXDLLIMPEXP_DATA_SMTP() in the declaration of each global
//    variable:
//          WXDLLIMPEXP_DATA_SMTP(int) myGlobalIntVar;
//
#ifdef WXMAKINGDLL_SMTP
    #define WXDLLIMPEXP_SMTP                    WXEXPORT
    #define WXDLLIMPEXP_DATA_SMTP(type)         WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_SMTP                    WXIMPORT
    #define WXDLLIMPEXP_DATA_SMTP(type)         WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_SMTP
    #define WXDLLIMPEXP_DATA_SMTP(type)         type
#endif

#endif // _WX_SMTP_DEFS_H_
