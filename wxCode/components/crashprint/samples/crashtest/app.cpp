//////////////////////////////////////////////////////////////////////////////
// File:        crashtest.cpp
// Purpose:     CrashTest application
// Maintainer:  Otto Wyss
// Created:     2004-10-01
// RCS-ID:      $Id: app.cpp,v 1.5 2004-10-08 16:05:15 wyo Exp $
// Copyright:   (c) wxCode
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

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

//! standard header

//! directFB headers

//! wxWidgets headers

#if defined(__linux__)
#include "wx/crashprint.h"
#endif

//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "app.xpm"
#endif


//============================================================================
// declarations
//============================================================================

const wxString APP_NAME = _T("CrashTest");
const wxString APP_DESCR = _(
    "This application tests if the crash print is correct."
);

const wxString APP_MAINT = _T("Otto Wyss");
const wxString APP_VENDOR = _T("wxCode");
const wxString APP_COPYRIGTH = _T("(C) 2004 wxCode");
const wxString APP_LICENCE = _T("wxWindows");

const wxString APP_VERSION = _T("0.1.beta");
const wxString APP_BUILD = _T(__DATE__);

const wxString APP_WEBSITE = _T("http://wxcode.sourceforge.net");
const wxString APP_MAIL = _T("http://wxcode.sourceforge.net/feedback.php");

const wxString APP_INFOS = _(
    "If you like this app and want to help just subscribe to the users mailing \n"
    "and ask what you can do."
);


//----------------------------------------------------------------------------
//! global application name
wxString g_appname;

#if defined(__linux__)
//! crashprint
wxCrashPrint g_crashprint;
#endif

//----------------------------------------------------------------------------
//! application
class App: public wxApp {

public:
    App();

    // standard overrides
    bool OnInit();

    //! fatal exeption handling
    void OnFatalException();

private:

};


//============================================================================
// implementation
//============================================================================

IMPLEMENT_APP (App)

//----------------------------------------------------------------------------
// App
//----------------------------------------------------------------------------

App::App () {
}

bool App::OnInit () {

    // set application and vendor name
    SetAppName (APP_NAME);
    SetVendorName (APP_VENDOR);
    g_appname.Append (APP_NAME);

    // print welcome
    printf (_T("%s (%s)\n"), g_appname.c_str(), APP_VERSION.c_str());
    printf (_T("%s\n\n"), APP_DESCR.c_str());

    // simulate crash
#if defined(__linux__)
    g_crashprint.Report();
#endif

    return false;
}

void App::OnFatalException () {
#if defined(__linux__)
    g_crashprint.Report();
#endif
}
