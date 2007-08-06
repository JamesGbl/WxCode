//
// Copyright (C) 2007 Arne Steinarson <arst at users dot sourceforge dot net>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must
//    not claim that you wrote the original software. If you use this
//    software in a product, an acknowledgment in the product
//    documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must
//    not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source
//    distribution.
//



#ifndef _WX_RLAYOUT_DEFS_H_
#define _WX_RLAYOUT_DEFS_H_

// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxRLAYOUT_MAJOR          0
#define wxRLAYOUT_MINOR          5
#define wxRLAYOUT_RELEASE        0

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_RLAYOUT_VERSION(major,minor,release) \
    (wxRLAYOUT_MAJOR > (major) || \
    (wxRLAYOUT_MAJOR == (major) && wxRLAYOUT_MINOR > (minor)) || \
    (wxRLAYOUT_MAJOR == (major) && wxRLAYOUT_MINOR == (minor) && wxRLAYOUT_RELEASE >= (release)))


// Defines for shared builds.
// Simple reference for using these macros and for writin components
// which support shared builds:
//
// 1) use the WXDLLIMPEXP_RLAYOUT in each class declaration:
//          class WXDLLIMPEXP_RLAYOUT myCompClass {   [...]   };
//
// 2) use the WXDLLIMPEXP_RLAYOUT in the declaration of each global function:
//          WXDLLIMPEXP_RLAYOUT int myGlobalFunc();
//
// 3) use the WXDLLIMPEXP_DATA_RLAYOUT() in the declaration of each global
//    variable:
//          WXDLLIMPEXP_DATA_RLAYOUT(int) myGlobalIntVar;
//
#ifdef WXMAKINGDLL_RLAYOUT
    #define WXDLLIMPEXP_RLAYOUT                  WXEXPORT
    #define WXDLLIMPEXP_DATA_RLAYOUT(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_RLAYOUT                  WXIMPORT
    #define WXDLLIMPEXP_DATA_RLAYOUT(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_RLAYOUT
    #define WXDLLIMPEXP_DATA_RLAYOUT(type)	    type
#endif

#endif // _WX_RLAYOUT_DEFS_H_

