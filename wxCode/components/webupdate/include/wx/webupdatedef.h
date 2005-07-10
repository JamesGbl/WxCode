/////////////////////////////////////////////////////////////////////////////
// Name:        webupdatedef.h
// Purpose:     shared build defines
// Author:      Francesco Montorsi
// Created:     2005/07/10
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_WEBUPDATE_DEFS_H_
#define _WX_WEBUPDATE_DEFS_H_


// Conditional compilation
// -----------------------

//! If this define is set to 1, then the wxCheckedListCtrl will be used
//! instead of a simple wxListCtrl.
#define wxWU_USE_CHECKEDLISTCTRL				1


// for shared builds
#ifdef WXMAKINGDLL_WEBUPDATE
    #define WXDLLIMPEXP_WEBUPDATE				WXEXPORT
    #define WXDLLIMPEXP_DATA_WEBUPDATE(type)	WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_WEBUPDATE WXIMPORT
    #define WXDLLIMPEXP_DATA_NWEBUPDATE(type)	WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_WEBUPDATE
    #define WXDLLIMPEXP_DATA_WEBUPDATE(type)	type
#endif

#endif // _WX_WEBUPDATE_DEFS_H_

