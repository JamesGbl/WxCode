//////////////////////////////////////////////////////////////////////////////
// File:        mimetypes.h
// Purpose:     wxMimeTypes control
// Maintainer:  Wyo
// Created:     2004-11-22
// RCS-ID:      $Id: mimetypes.h,v 1.1 2004-11-22 18:41:53 wyo Exp $
// Copyright:   (c) 2004 wxCode
// Licence:     wxWindows
//////////////////////////////////////////////////////////////////////////////

#ifndef _MY_MIMETPYES_H_
#define _MY_MIMETPYES_H_

#ifdef __GNUG__
    #pragma implementation "mimetypes.h"
#endif

//----------------------------------------------------------------------------
// information
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------
//! wxWidgets headers


//============================================================================
// declarations
//============================================================================

//----------------------------------------------------------------------------
//!


//----------------------------------------------------------------------------
//! wxMimeTypes
class wxMimeTypes {

public:

    //! constructor
    wxMimeTypes ();

    // get/set settings
    wxString GetMimeType (const wxString& filename) const
    wxString GetMimeTypeByContent (const wxString& filename) const
    wxString GetMimeTypeByFilename (const wxString& filename) const

    wxString GetApplication (const wxString& mimetype) const
    bool SetApplication (const wxString& mimetype, const wxString& filename)

    wxMimeTypeData* GetData (const wxString& mimetype) const
    bool SetData (const wxString& mimetype, wxMimeTypeData* data, bool force = false)

    wxIcon GetIcon ( onst wxString& mimetype) const
    bool SetIcon (const wxString& mimetype, const wxIcon& icon)

private:

    DECLARE_EVENT_TABLE()
};

#endif // _MY_MIMETPYES_H_

