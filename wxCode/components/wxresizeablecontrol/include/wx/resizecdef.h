/////////////////////////////////////////////////////////////////////////////
// Name:        resizecdef.h
// Purpose:     shared build defines
// Author:      Francesco Montorsi
// Created:     2005/8/15
// RCS-ID:      $Id: resizecdef.h,v 1.2 2005-09-12 19:00:19 frm Exp $
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_RESIZEC_DEFS_H_
#define _WX_RESIZEC_DEFS_H_

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
#ifdef WXMAKINGDLL_WXRESIZEABLECONTROL
    #define WXDLLIMPEXP_RESIZEC                  WXEXPORT
    #define WXDLLIMPEXP_DATA_RESIZEC(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_RESIZEC                  WXIMPORT
    #define WXDLLIMPEXP_DATA_RESIZEC(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_RESIZEC
    #define WXDLLIMPEXP_DATA_RESIZEC(type)	     type
#endif


#define wxUSE_TEXTBOX_XHTML_SUPPORT				1

#endif // _WX_RESIZEC_DEFS_H_

