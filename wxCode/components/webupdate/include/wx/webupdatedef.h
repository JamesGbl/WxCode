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

#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>
#endif

// Conditional compilation
// -----------------------

//! If this define is set to 1, then the wxCheckedListCtrl will be used
//! instead of a simple wxListCtrl.
#ifndef wxUSE_CHECKEDLISTCTRL
    #define wxUSE_CHECKEDLISTCTRL                   1
#endif

//! If this define is set to 1, then the wxMD5 classes will be used and 
//! compiled (see md5.cpp for license over MD5 algorithm).
#ifndef wxUSE_MD5
    #define wxUSE_MD5                               1
#endif

//! If this define is set to 1, then the wxHTTPBuilder component dependency
//! will be enabled.
#ifndef wxUSE_HTTPENGINE
    #define wxUSE_HTTPENGINE                        0
#endif

// for shared builds
// NOTE: these were useful only when originally the WebUpdate component
//       was built as a library for full integration into the application
//       to update (see the design FAQ in the docs for more info).
#ifdef WXMAKINGDLL_WEBUPDATE
    #define WXDLLIMPEXP_WEBUPDATE               WXEXPORT
    #define WXDLLIMPEXP_DATA_WEBUPDATE(type)    WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_WEBUPDATE WXIMPORT
    #define WXDLLIMPEXP_DATA_WEBUPDATE(type)    WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_WEBUPDATE
    #define WXDLLIMPEXP_DATA_WEBUPDATE(type)    type
#endif

#endif // _WX_WEBUPDATE_DEFS_H_

