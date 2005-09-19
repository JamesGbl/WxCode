//////////////////////////////////////////////////////////////////////////////
// File:        mimetypes.h
// Purpose:     wxMimeType control
// Maintainer:  Wyo
// Created:     2005-03-08
// RCS-ID:      $Id: mimetypes.h,v 1.4 2005-09-19 17:52:05 wyo Exp $
// Copyright:   (c) 2005 wxCode
// Licence:     wxWindows
//////////////////////////////////////////////////////////////////////////////

#ifndef _MY_MIMETPYES_H_
#define _MY_MIMETPYES_H_

#ifdef __GNUG__
    #pragma implementation "mimetype.h"
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
//! wxMimeTypeData
struct wxMimeTypeData {
    wxString mimetype;
    wxString filename;
    wxString extension;
    wxString magicId;
    wxString creator;
    wxString description;
    wxString appname;
    wxIcon icon;
};


//----------------------------------------------------------------------------
//! wxMimeType
class wxMimeType {

public:

    //! constructor
    wxMimeType ();

    // add/remove mime type
    bool Add (const wxString& mimetype, wxMimeTypeData* data, bool standard = false);
    bool Remove (wxMimeTypeData* data);

    // test mime type
    bool Exists (wxMimeTypeData* data);
    bool Match (wxMimeTypeData* data); // matches appname
    bool IsStandard (wxMimeTypeData* data); // and is the standard application

    // get data of mime type
    bool GetData (wxMimeTypeData* data);
    wxString GetMimeType (wxMimeTypeData* data);

private:

};

#endif // _MY_MIMETPYES_H_

