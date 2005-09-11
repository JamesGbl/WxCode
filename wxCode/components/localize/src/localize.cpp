//////////////////////////////////////////////////////////////////////////////
// File:        localize.cpp
// Purpose:     wxLocalize
// Maintainer:  Wyo
// Created:     2005-03-31
// RCS-ID:      $Id: localize.cpp,v 1.1 2005-09-11 10:16:19 wyo Exp $
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
#include <wx/file.h>     // raw file io support
#include <wx/filename.h> // filename support

// localize headers
#include "wx/localize.h" // localize support


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
// GetSystemLanguageCanonical
//----------------------------------------------------------------------------

static wxString GetSystemLanguageCanonical() {

    // determine current system language
    const wxLanguageInfo *info = wxLocale::GetLanguageInfo (wxLocale::GetSystemLanguage());
    if (info) return info->CanonicalName;
    return wxEmptyString;
}


//----------------------------------------------------------------------------
// wxLocalize
//----------------------------------------------------------------------------

wxString wxLocalize (const wxString& searchPath, const wxString& fullName, const wxString& language) {

    wxString name = wxFileName (fullName).GetFullName();
    wxString lang = !language.IsEmpty()? language: GetSystemLanguageCanonical();
    wxString lg = lang.BeforeFirst ('_');
    bool sublocale = (lang != lg);
    wxString pathSep = wxFILE_SEP_PATH;

    wxString fileName;
    wxString paths = searchPath;
    while (!paths.IsEmpty()) {
        wxString path = paths.BeforeFirst (wxPATH_SEP[0]) + pathSep;
        paths = paths.AfterFirst (wxPATH_SEP[0]);
        fileName = path + lang + pathSep + _T("LC_MESSAGES") + pathSep + name;
        if (wxFileExists (fileName)) return fileName;
        if (sublocale) {
            fileName = path + lg + pathSep + _T("LC_MESSAGES") + pathSep + name;
            if (wxFileExists (fileName)) return fileName;
        }
        fileName = path + lang + pathSep + name;
        if (wxFileExists (fileName)) return fileName;
        if (sublocale) {
            fileName = path + lg + pathSep + name;
            if (wxFileExists (fileName)) return fileName;
        }
        fileName = path + name;
        if (wxFileExists (fileName)) return fileName;
    }

    return wxEmptyString;
};

