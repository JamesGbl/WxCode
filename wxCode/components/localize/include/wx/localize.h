//////////////////////////////////////////////////////////////////////////////
// File:        localize.h
// Purpose:     wxLocalize class
// Maintainer:  Wyo
// Created:     2005-03-31
// RCS-ID:      $Id: localize.h,v 1.1 2005-09-11 10:14:09 wyo Exp $
// Copyright:   (c) 2005 wxCode
// Licence:     wxWindows
//////////////////////////////////////////////////////////////////////////////

#ifndef _WX_LOCALIZE_H_
#define _WX_LOCALIZE_H_

#ifdef __GNUG__
    #pragma implementation "localize.h"
#endif

//----------------------------------------------------------------------------
// information
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------
//! wxWidgets headers
#include <wx/string.h>   // strings support


//============================================================================
// declarations
//============================================================================


//----------------------------------------------------------------------------
// GetSystemLanguageCanonical
static wxString GetSystemLanguageCanonical();

//----------------------------------------------------------------------------
//! wxLocalize
wxString wxLocalize (const wxString& searchPath,
                     const wxString& fullName,
                     const wxString& lanugage = wxEmptyString);

#endif // _WX_LOCALIZE_H_

