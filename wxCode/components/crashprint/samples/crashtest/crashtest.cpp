//////////////////////////////////////////////////////////////////////////////
// File:        crashtest.cpp
// Purpose:     CrashTest application
// Maintainer:  Otto Wyss
// Created:     2004-10-01
// RCS-ID:      $Id: crashtest.cpp,v 1.9 2006-08-17 20:21:22 wyo Exp $
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

const wxString APP_NAME = _T("wxCrashPrint");
const wxString APP_VENDOR = _T("wxCode");
const wxString APP_VERSION = _T("1.0.0");
const wxString APP_MAINT = _T("Otto Wyss");
const wxString APP_LICENCE = _T("wxWindows");
const wxString APP_COPYRIGTH = _T("(C) 2005 Otto Wyss");

const wxString APP_DESCR = _("\
wxCrashPrint formats and prints a report in case the application \n\
crashes on Linux. Also included is the implementation of the \n\
BlackBox.dll which does the same on Windows.\
");
const wxString APP_WEBSITE = _T("http://wxcode.sourceforge.net/");

const wxString APP_INFOS = _("\
This application is derived from the demo sample of wyoGuide.\
");
const wxString APP_WYOGUIDE = _T("http://wyoguide.sourceforge.net");


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
    friend class AppAbout;

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


//----------------------------------------------------------------------------
//! about box of the application
class AppAbout: public wxDialog {

public:
    //! constructor
    AppAbout (wxWindow *parent, int milliseconds = 0);

    //! destructor
    ~AppAbout ();

    // event handlers
    void OnTimerEvent (wxTimerEvent &event);

private:
    // timer
    wxTimer *m_timer;

    DECLARE_EVENT_TABLE()
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

    // about box shown for 3 seconds
    AppAbout (NULL, 3000);

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


//----------------------------------------------------------------------------
// AppAbout
//----------------------------------------------------------------------------

const int myID_ABOUTTIMER = wxID_HIGHEST + 1;
BEGIN_EVENT_TABLE (AppAbout, wxDialog)
    EVT_TIMER (myID_ABOUTTIMER, AppAbout::OnTimerEvent)
END_EVENT_TABLE ()

AppAbout::AppAbout (wxWindow *parent,
                    int milliseconds)
        : wxDialog (parent, -1, _("About..."),
                    wxDefaultPosition, wxDefaultSize,
                    wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP) {

    // set timer if any
    m_timer = NULL;
    if (milliseconds > 0) {
        m_timer = new wxTimer (this, myID_ABOUTTIMER);
        m_timer->Start (milliseconds, wxTIMER_ONE_SHOT);
    }

    // about info
    wxFlexGridSizer *aboutinfo = new wxFlexGridSizer (2, 0, 2);
    aboutinfo->Add (new wxStaticText(this, -1, _("Vendor: ")));
    aboutinfo->Add (new wxStaticText(this, -1, APP_VENDOR));
    aboutinfo->Add (new wxStaticText(this, -1, _("Version: ")));
    aboutinfo->Add (new wxStaticText(this, -1, APP_VERSION));
    aboutinfo->Add (new wxStaticText(this, -1, _("wxWidgets: ")));
    aboutinfo->Add (new wxStaticText(this, -1, wxVERSION_STRING));
    aboutinfo->Add (new wxStaticText(this, -1, _("Written by: ")));
    aboutinfo->Add (new wxStaticText(this, -1, APP_MAINT));
    aboutinfo->Add (new wxStaticText(this, -1, _("Licence type: ")));
    aboutinfo->Add (new wxStaticText(this, -1, APP_LICENCE));
    aboutinfo->Add (new wxStaticText(this, -1, _("Copyright: ")));
    aboutinfo->Add (new wxStaticText(this, -1, APP_COPYRIGTH));

    // about icontitle//info
    wxBoxSizer *aboutpane = new wxBoxSizer (wxHORIZONTAL);
    wxBitmap bitmap = wxBitmap(wxICON (crashtest));
    aboutpane->Add (new wxStaticBitmap (this, -1, bitmap),
                    1, wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 20);
    aboutpane->Add (aboutinfo, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    aboutpane->Add (20, 0);

    // about complete
    wxBoxSizer *totalpane = new wxBoxSizer (wxVERTICAL);
    totalpane->Add (0, 10);
    wxStaticText *appname = new wxStaticText(this, -1, APP_NAME);
    appname->SetFont (wxFont (20, wxDEFAULT, wxNORMAL, wxBOLD));
    totalpane->Add (appname, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 40);
    totalpane->Add (0, 10);
    totalpane->Add (aboutpane, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    totalpane->Add (new wxStaticText(this, -1, wxGetTranslation(APP_DESCR)),
                    0, wxALIGN_LEFT | wxLEFT | wxRIGHT, 10);
    totalpane->Add (0, 6);
    totalpane->Add (new wxStaticText(this, -1, APP_WEBSITE),
                    0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    totalpane->Add (new wxStaticText(this, -1, wxGetTranslation(APP_INFOS)),
                    0, wxALIGN_LEFT | wxLEFT | wxRIGHT, 10);
    totalpane->Add (0, 6);
    totalpane->Add (new wxStaticText (this, -1, APP_WYOGUIDE),
                    0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    wxButton *okButton = new wxButton (this, wxID_OK, _("OK"));
    okButton->SetDefault();
    totalpane->Add (okButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit (totalpane);
    CentreOnParent();
    ShowModal();
}

AppAbout::~AppAbout () {
    if (m_timer)  {
        delete m_timer;
        m_timer = NULL;
    }
}

//----------------------------------------------------------------------------
// event handlers
void AppAbout::OnTimerEvent (wxTimerEvent &WXUNUSED(event)) {
    if (m_timer) delete m_timer;
    m_timer = NULL;
    EndModal (wxID_OK);
}
