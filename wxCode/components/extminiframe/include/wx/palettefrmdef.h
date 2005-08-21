/////////////////////////////////////////////////////////////////////////////
// Name:        palettefrmdef.h
// Purpose:     shared build defines
// Author:      Francesco Montorsi
// Created:     2005/8/14
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_PALETTEFRM_DEFS_H_
#define _WX_PALETTEFRM_DEFS_H_

// Defines for shared builds.
// Simple reference for using these macros and for writin components
// which support shared builds:
//
// 1) use the WXDLLIMPEXP_WXEXTMINIFRAME in each class declaration:
//          class WXDLLIMPEXP_WXEXTMINIFRAME PALETTEFRMClass {   [...]   };
//
// 2) use the WXDLLIMPEXP_WXEXTMINIFRAME in the declaration of each global function:
//          WXDLLIMPEXP_WXEXTMINIFRAME int myGlobalFunc();
//
// 3) use the WXDLLIMPEXP_DATA_PALETTEFRM() in the declaration of each global
//    variable:
//          WXDLLIMPEXP_DATA_PALETTEFRM(int) myGlobalIntVar;
//
#ifdef WXMAKINGDLL_WXEXTMINIFRAME
    #define WXDLLIMPEXP_WXEXTMINIFRAME                  WXEXPORT
    #define WXDLLIMPEXP_DATA_WXEXTMINIFRAME(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_WXEXTMINIFRAME                  WXIMPORT
    #define WXDLLIMPEXP_DATA_WXEXTMINIFRAME(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_WXEXTMINIFRAME
    #define WXDLLIMPEXP_DATA_WXEXTMINIFRAME(type)	    type
#endif

#endif // _WX_PALETTEFRM_DEFS_H_

