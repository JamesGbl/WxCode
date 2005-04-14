//////////////////////////////////////////////////////////////////////////////
// File:        crashtest.cpp
// Purpose:     CrashTest application
// Maintainer:  Otto Wyss
// Created:     2004-10-01
// RCS-ID:      $Id: crashtest.cpp,v 1.7 2005-04-14 19:41:52 wyo Exp $
// Copyright:   (c) 2004 wxCode
// Licence:     wxWindows
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

//! wxWidgets headers
#include <wx/dynlib.h>   // dynamic library support

//! wxCode headers
#if defined(__linux__)
#include "wx/crashprint.h" // crashprint support
#endif

// detecting memory leaks on Windows with _CrtSetBreakAlloc (<memory_number>)
// #include <crtdbg.h>

//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "crashtest.xpm"
#endif


//============================================================================
// declarations
//============================================================================

const wxString APP_NAME = _T("CrashTest");
const wxString APP_DESCR = _("\
This application tests if the crash print is correct.\
");

const wxString APP_MAINT = _T("Otto Wyss");
const wxString APP_VENDOR = _T("wxCode");
const wxString APP_COPYRIGTH = _T("(C) 2004 wxCode");
const wxString APP_LICENCE = _T("wxWindows");

const wxString APP_VERSION = _T("0.2.1");

const wxString APP_WEBSITE = _T("http://wxcode.sourceforge.net");
const wxString APP_MAIL = _T("http://wxcode.sourceforge.net/feedback.php");

const wxString APP_INFOS = _("\
If you like this app and want to help just subscribe to the users mailing \n\
and ask what you can do.\
");


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
    // standard overrides
    bool OnInit();

    //! application exit function
    virtual int OnExit ();

#if defined(__linux__)
    //! fatal exeption handling
    void OnFatalException();
#endif

private:
#if defined(__WINDOWS__)
    //! BlackBox dll (crash handling)
    wxDynamicLibrary  m_blackboxDll;
#endif

};


//============================================================================
// implementation
//============================================================================

IMPLEMENT_APP (App)

//----------------------------------------------------------------------------
// App
//----------------------------------------------------------------------------

bool App::OnInit () {

    // detecting memory leaks on Windows with _CrtSetBreakAlloc (<memory_number>)
    //_CrtSetBreakAlloc (<memory_number>);

#if defined(__WINDOWS__)
    // BlackBox dll (crash handling)
    m_blackboxDll.Load (_T("BlackBox.dll"));
#endif

#if defined(__linux__)
    // fatal exceptions handling
    wxHandleFatalExceptions (true);
#endif

    // set application and vendor name
    SetAppName (APP_NAME);
    SetVendorName (APP_VENDOR);
    g_appname.Append (APP_NAME);

    // print welcome
    wxPrintf (_T("%s (%s)\n"), g_appname.c_str(), APP_VERSION.c_str());
    wxPrintf (_T("%s\n\n"), APP_DESCR.c_str());

    // force crash
    wxWindow *w = NULL; w->Show();

    return false;
}

int App::OnExit () {
#if defined(__WINDOWS__)
    // unload BlackBox dll
    if (m_blackboxDll.IsLoaded()) m_blackboxDll.Unload ();
#endif

    return 0;
}

#if defined(__linux__)
void App::OnFatalException () {
    g_crashprint.Report();
}
#endif
