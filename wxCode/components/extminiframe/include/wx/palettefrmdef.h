/////////////////////////////////////////////////////////////////////////////
// Name:        PALETTEFRM.h
// Purpose:     shared build defines
// Author:      Your Name
// Created:     someyear/somemonth/someday
// RCS-ID:      $Id$
// Copyright:   (c) YEAR Your Name
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_PALETTEFRM_DEFS_H_
#define _WX_PALETTEFRM_DEFS_H_

// Defines for shared builds.
// Simple reference for using these macros and for writin components
// which support shared builds:
//
// 1) use the WXDLLIMPEXP_PALETTEFRM in each class declaration:
//          class WXDLLIMPEXP_PALETTEFRM PALETTEFRMClass {   [...]   };
//
// 2) use the WXDLLIMPEXP_PALETTEFRM in the declaration of each global function:
//          WXDLLIMPEXP_PALETTEFRM int myGlobalFunc();
//
// 3) use the WXDLLIMPEXP_DATA_PALETTEFRM() in the declaration of each global
//    variable:
//          WXDLLIMPEXP_DATA_PALETTEFRM(int) myGlobalIntVar;
//
#ifdef WXMAKINGDLL_PALETTEFRM
    #define WXDLLIMPEXP_PALETTEFRM                  WXEXPORT
    #define WXDLLIMPEXP_DATA_PALETTEFRM(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_PALETTEFRM                  WXIMPORT
    #define WXDLLIMPEXP_DATA_PALETTEFRM(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_PALETTEFRM
    #define WXDLLIMPEXP_DATA_PALETTEFRM(type)	    type
#endif

#endif // _WX_PALETTEFRM_DEFS_H_

