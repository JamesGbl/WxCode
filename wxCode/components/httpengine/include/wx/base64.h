/////////////////////////////////////////////////////////////////////////////
// Name:        base64.h
// Purpose:     Base64 encoding function for HTTP Authentication
//					Code originated from PHP.net source
// Author:      Angelo Mandato
// Created:     2005/08/10
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Angelo Mandato (http://www.spaceblue.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BASE64_H_
#define _WX_BASE64_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "base64.h"
#endif

#include "wx/httpenginedef.h"
#include <wx/string.h>

extern const char base64_table[];
extern const char base64_pad;
extern const short base64_reverse_table[256];

//! Returns string base64 encoded
// Based on code from PHP library:
WXDLLIMPEXP_HTTPENGINE wxString wxBase64Encode(const wxString &str );

//! Returns string base64 decoded
// Based on code from PHP library:
WXDLLIMPEXP_HTTPENGINE wxString wxBase64Decode(const wxString &str);

#endif
