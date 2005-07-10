/////////////////////////////////////////////////////////////////////////////
// Name:        WXXML2.h
// Purpose:     shared build defines
// Author:      Your Name
// Created:     someyear/somemonth/someday
// RCS-ID:      $Id$
// Copyright:   (c) YEAR Your Name
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_WXXML2_DEFS_H_
#define _WX_WXXML2_DEFS_H_

// Defines for shared builds.
// Simple reference for using these macros and for writin components
// which support shared builds:
//
// 1) use the WXDLLIMPEXP_WXXML2 in each class declaration:
//          class WXDLLIMPEXP_WXXML2 WXXML2Class {   [...]   };
//
// 2) use the WXDLLIMPEXP_WXXML2 in the declaration of each global function:
//          WXDLLIMPEXP_WXXML2 int myGlobalFunc();
//
// 3) use the WXDLLIMPEXP_DATA_WXXML2() in the declaration of each global
//    variable:
//          WXDLLIMPEXP_DATA_WXXML2(int) myGlobalIntVar;
//
#ifdef WXMAKINGDLL_WXXML2
    #define WXDLLIMPEXP_WXXML2                  WXEXPORT
    #define WXDLLIMPEXP_DATA_WXXML2(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_WXXML2                  WXIMPORT
    #define WXDLLIMPEXP_DATA_WXXML2(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_WXXML2
    #define WXDLLIMPEXP_DATA_WXXML2(type)	    type
#endif

#endif // _WX_WXXML2_DEFS_H_

