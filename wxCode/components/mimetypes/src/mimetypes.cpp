//////////////////////////////////////////////////////////////////////////////
// File:        mimetypes.cpp
// Purpose:     wxMimeType control
// Maintainer:  Wyo
// Created:     2005-03-08
// RCS-ID:      $Id: mimetypes.cpp,v 1.3 2005-03-08 18:53:30 wyo Exp $
// Copyright:   (c) 2005 wxCode
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
#include "wx/mimetype.h" // wxFileType support

// mimetype headers
#include "wx/mimetypes.h" // wxMimeType control


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
// wxMimeType
//----------------------------------------------------------------------------

wxMimeType::wxMimeType () {
}

//----------------------------------------------------------------------------
// settings functions

bool wxMimeType::Add (const wxString& mimetype, wxMimeTypeData* data, bool standard) {

    wxFileTypeInfo info (mimetype, data->appname, wxEmptyString, 
                         data->description.c_str(), data->extension.c_str(), NULL);
    wxString shortDesc = _T("Test ") + data->extension + _T(" file");
    info.SetShortDesc (shortDesc);
    info.SetIcon (data->appname);
    wxFileType *filetype;
    filetype = wxTheMimeTypesManager->Associate (info);
    delete filetype;

    return true;
}

bool wxMimeType::Remove (wxMimeTypeData* data) {

    wxFileType *filetype;
    filetype = wxTheMimeTypesManager->GetFileTypeFromExtension (data->extension);
    if (!filetype) return false;
    filetype->Unassociate();
    delete filetype;

    return true;
}

bool wxMimeType::Exists (wxMimeTypeData* data) {

    wxFileType *filetype;
    filetype = wxTheMimeTypesManager->GetFileTypeFromExtension (data->extension);

    return (filetype != NULL);

}

bool wxMimeType::Match (wxMimeTypeData* data) {

    wxFileType *filetype;
    filetype = wxTheMimeTypesManager->GetFileTypeFromExtension (data->extension);
    if (!filetype) return false;
    wxString appname = filetype->GetOpenCommand (wxEmptyString);
    delete filetype;

    return (appname.Contains (data->appname));

}

bool wxMimeType::IsStandard (wxMimeTypeData* data) {

    // not implemented

    return false;

}

bool wxMimeType::GetData (wxMimeTypeData* data) const {

    wxFileType *filetype;
    filetype = wxTheMimeTypesManager->GetFileTypeFromExtension (data->extension);
    if (!filetype) return false;
    filetype->GetDescription (&data->description);
    data->appname = filetype->GetOpenCommand (wxEmptyString);
    data->appname.Remove (data->appname.Length()-4);

    return true;
}

wxString wxMimeType::GetMimeType (wxMimeTypeData* data) {

    // not implemented

    return wxEmptyString;
}

//----------------------------------------------------------------------------
// private functions

