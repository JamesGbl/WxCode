////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        WinServiceDef.h                                               //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef WINSERVICEDEF_H
#define WINSERVICEDEF_H



// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.

#define wxWINSERVICE_MAJOR    0
#define wxWINSERVICE_MINOR    0
#define wxWINSERVICE_RELEASE  0



// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release

#define wxCHECK_WINSERVICE_VERSION(major,minor,release) \
    (wxWINSERVICE_MAJOR > (major) || \
    (wxWINSERVICE_MAJOR == (major) && wxWINSERVICE_MINOR > (minor)) || \
    (wxWINSERVICE_MAJOR == (major) && wxWINSERVICE_MINOR == (minor) && wxWINSERVICE_RELEASE >= (release)))



// Defines for shared builds.
// Simple reference for using these macros and for writin components
// which support shared builds:
//
// 1) use the WXDLLIMPEXP_WINSERVICE in each class declaration:
//          class WXDLLIMPEXP_WINSERVICE myCompClass {   [...]   };
//
// 2) use the WXDLLIMPEXP_WINSERVICE in the declaration of each global function:
//          WXDLLIMPEXP_WINSERVICE int myGlobalFunc();
//
// 3) use the WXDLLIMPEXP_DATA_WINSERVICE() in the declaration of each global
//    variable:
//          WXDLLIMPEXP_DATA_WINSERVICE(int) myGlobalIntVar;


#ifdef WXMAKINGDLL_WINSERVICE
    #define WXDLLIMPEXP_WINSERVICE                  WXEXPORT
    #define WXDLLIMPEXP_DATA_WINSERVICE(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_WINSERVICE                  WXIMPORT
    #define WXDLLIMPEXP_DATA_WINSERVICE(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_WINSERVICE
    #define WXDLLIMPEXP_DATA_WINSERVICE(type)	    type
#endif



#endif
