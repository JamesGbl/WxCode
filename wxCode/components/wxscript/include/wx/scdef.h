/////////////////////////////////////////////////////////////////////////////
// Name:        wxscript.h
// Purpose:     shared build defines
// Author:      Francesco Montorsi
// Created:     2005/7/14
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_WXSCRIPT_DEFS_H_
#define _WX_WXSCRIPT_DEFS_H_

// Defines for shared builds.
// Simple reference for using these macros and for writin components
// which support shared builds:
//
// 1) use the WXDLLIMPEXP_WXSCRIPT in each class declaration:
//          class WXDLLIMPEXP_WXSCRIPT WXSCRIPTClass {   [...]   };
//
// 2) use the WXDLLIMPEXP_WXSCRIPT in the declaration of each global function:
//          WXDLLIMPEXP_WXSCRIPT int myGlobalFunc();
//
// 3) use the WXDLLIMPEXP_DATA_WXSCRIPT() in the declaration of each global
//    variable:
//          WXDLLIMPEXP_DATA_WXSCRIPT(int) myGlobalIntVar;
//
#ifdef WXMAKINGDLL_WXSCRIPT
    #define WXDLLIMPEXP_WXSCRIPT                  WXEXPORT
    #define WXDLLIMPEXP_DATA_WXSCRIPT(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_WXSCRIPT                  WXIMPORT
    #define WXDLLIMPEXP_DATA_WXSCRIPT(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_WXSCRIPT
    #define WXDLLIMPEXP_DATA_WXSCRIPT(type)	    type
#endif

#endif // _WX_WXSCRIPT_DEFS_H_

