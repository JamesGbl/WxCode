/////////////////////////////////////////////////////////////////////////////
// Name:        mycomp.h
// Purpose:     shared build defines
// Author:      Your Name
// Created:     someyear/somemonth/someday
// RCS-ID:      $Id$
// Copyright:   (c) YEAR Your Name
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_MYCOMP_DEFS_H_
#define _WX_MYCOMP_DEFS_H_

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
#ifdef WXMAKINGDLL_MYCOMP
    #define WXDLLIMPEXP_MYCOMP                  WXEXPORT
    #define WXDLLIMPEXP_DATA_MYCOMP(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_MYCOMP                  WXIMPORT
    #define WXDLLIMPEXP_DATA_MYCOMP(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_MYCOMP
    #define WXDLLIMPEXP_DATA_MYCOMP(type)	    type
#endif

#endif // _WX_MYCOMP_DEFS_H_

