//////////////////////////////////////////////////////////////////////////////
// File:        mimetypes.cpp
// Purpose:     wxMimeTypes control
// Maintainer:  Wyo
// Created:     2004-11-22
// RCS-ID:      $Id: mimetypes.cpp,v 1.1 2004-11-22 18:46:59 wyo Exp $
// Copyright:   (c) 2004 wxCode
// Licence:     wxWindows
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// information
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all 'standard' wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// wxWidgets headers

// mimetypes headers
#include "wx/mimetypes.h" // wxMimeTypes control


//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------


//============================================================================
// declarations
//============================================================================


//============================================================================
// implementation
//============================================================================

//----------------------------------------------------------------------------
// wxMimeTypes
//----------------------------------------------------------------------------

void wxMimeTypes::wxMimeTypes () {

}

//----------------------------------------------------------------------------
// event handlers

//----------------------------------------------------------------------------
// settings functions

wxString GetMimeType (const wxString& filename) const {
}

wxString GetMimeTypeByContent (const wxString& filename) const {
}

wxString GetMimeTypeByFilename (const wxString& filename) const {
}

wxString GetApplication (const wxString& mimetype) const {
}

bool SetApplication (const wxString& mimetype, const wxString& filename) {
}

wxMimeTypeData* GetData (const wxString& mimetype) const {
}

bool SetData (const wxString& mimetype, wxMimeTypeData* data, bool force = false)

wxIcon GetIcon ( onst wxString& mimetype) const {
}

bool SetIcon (const wxString& mimetype, const wxIcon& icon) {
}


//----------------------------------------------------------------------------
// private functions

