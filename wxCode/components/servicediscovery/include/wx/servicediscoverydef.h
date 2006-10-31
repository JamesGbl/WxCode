/////////////////////////////////////////////////////////////////////////////
// Name:        servicediscoverydef.h
// Purpose:     shared build defines
// Author:      Hank Schultz
// Created:     2006/10/30
// Revision   	$Rev$
// Changed by	$Author$
// Modified		$Date$
// Copyright:   (c) 2006 Cedrus Corporation
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_SERVICE_DISCOVERY_DEFS_H_
#define _WX_SERVICE_DISCOVERY_DEFS_H_

// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxSERVICEDISCOVERY_MAJOR          1
#define wxSERVICEDISCOVERY_MINOR          0
#define wxSERVICEDISCOVERY_RELEASE        0

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_SERVICEDISCOVERY_VERSION(major,minor,release) \
    (wxSERVICEDISCOVERY_MAJOR > (major) || \
    (wxSERVICEDISCOVERY_MAJOR == (major) && wxSERVICEDISCOVERY_MINOR > (minor)) || \
    (wxSERVICEDISCOVERY_MAJOR == (major) && wxSERVICEDISCOVERY_MINOR == (minor) && wxSERVICEDISCOVERY_RELEASE >= (release)))


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
#ifdef WXMAKINGDLL_SERVICEDISCOVERY
    #define WXDLLIMPEXP_SERVICEDISCOVERY                  WXEXPORT
    #define WXDLLIMPEXP_DATA_SERVICEDISCOVERY(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_SERVICEDISCOVERY                  WXIMPORT
    #define WXDLLIMPEXP_DATA_SERVICEDISCOVERY(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_SERVICEDISCOVERY
    #define WXDLLIMPEXP_DATA_SERVICEDISCOVERY(type)	    type
#endif

#endif // _WX_SERVICE_DISCOVERY_DEFS_H_

