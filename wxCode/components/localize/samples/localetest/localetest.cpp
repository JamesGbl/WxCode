//////////////////////////////////////////////////////////////////////////////
// File:        localetest.cpp
// Purpose:     Localetest application
// Maintainer:  Otto Wyss
// Created:     2004-10-01
// RCS-ID:      $Id: localetest.cpp,v 1.1 2005-09-11 10:15:48 wyo Exp $
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
#include <wx/fs_zip.h>   // ZIP filesystem support
#include <wx/html/helpctrl.h> // Html help support
#include <wx/intl.h>     // internationalisation

//! wxCode headers
#include "wx/localize.h" // localize support

// detecting memory leaks on Windows with _CrtSetBreakAlloc (<memory_number>)
// #include <crtdbg.h>

//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "localetest.xpm"
#endif


//============================================================================
// declarations
//============================================================================

const wxString APP_NAME = _T("LocaleTest");
const wxString APP_SYNOPSIS = _("Tests localisation");
const wxString APP_DESCR = _("\
This application tests if the localisation is correct. If it is correct all the texts \n\
should be shown in the choosen language, else all texts are still shown in english.\
");

const wxString APP_MAINT = _T("Otto Wyss");
const wxString APP_VENDOR = _T("wxCode");
const wxString APP_COPYRIGTH = _T("(C) 2004 wxCode");
const wxString APP_LICENCE = _T("wxWindows");

const wxString APP_VERSION = _T("0.2.2");
const wxString APP_BUILD = _T(__DATE__);

const wxString APP_WEBSITE = _T("http://wyoguide.sourceforge.net");
const wxString APP_MAIL = _T("http://wyoguide.sourceforge.net/feedback.php");


//----------------------------------------------------------------------------
//! global application name
wxString g_appname;

//! application directory
wxString g_appDirectory;

//! global help provider
wxHtmlHelpController *g_help = NULL;
wxString g_helppath;

//----------------------------------------------------------------------------
//! application
class App: public wxApp {
    friend class AppAbout;
    friend class AppFrame;

public:
    // standard overrides
    bool OnInit();

    //! application exit function
    virtual int OnExit ();

private:
    //! variable for internationalization
    wxLocale m_locale;

    AppFrame *m_frame;

};


//----------------------------------------------------------------------------
//! about box of the application APP_VENDOR-APP_NAME
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


//----------------------------------------------------------------------------
//! frame of the application APP_VENDOR-APP_NAME.
class AppFrame: public wxFrame {

public:
    //! constructor
    AppFrame (const wxString &title);

    //! destructor
    ~AppFrame ();

    //! common event handlers
    void OnClose (wxCloseEvent &event);
    void OnAbout (wxCommandEvent &event);
    void OnExit (wxCommandEvent &event);
    void OnHelp (wxCommandEvent &event);

    //! help controller
    wxHtmlHelpController& GetHelpController() {return *g_help; }

private:
    //! creates the application menu bar
    void CreateMenu ();

    // test window
    wxTextCtrl *m_test;

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

#if defined(__WINDOWS__)
    // detecting memory leaks on Windows with _CrtSetBreakAlloc (<memory_number>)
    //_CrtSetBreakAlloc (<memory_number>);
#endif

    // set application and vendor name
    SetAppName (APP_NAME);
    SetVendorName (APP_VENDOR);
    g_appname.Append (APP_NAME);

    // initialize localisazion
    wxString appPath = g_appDirectory;
    if (appPath.IsEmpty()) appPath = wxFileName(argv[0]).GetPath (wxPATH_GET_VOLUME);
    wxString appName = wxFileName(argv[0]).GetName();
#ifdef __WXMAC__
    appPath += wxFileName::GetPathSeparator() + appName + _T(".app/Contents/SharedSupport"));
#endif
    m_locale.Init();
    m_locale.AddCatalogLookupPathPrefix (appPath);
    m_locale.AddCatalog (appName);

    // initializes help (Html help needs the Zip filestytem!?!)
    wxFileSystem::AddHandler (new wxZipFSHandler);
    g_help = new wxHtmlHelpController;
    wxString helpfile = wxLocalize (appPath, appName + _T(".htb"));
    if (wxFileExists (helpfile)) {
        g_help->AddBook (helpfile);
    }

    // create application frame
    m_frame = new AppFrame (APP_NAME);

    // open application frame
    m_frame->Layout ();
    m_frame->Show (true);
    SetTopWindow (m_frame);

    return true;
}

int App::OnExit () {
    // delete help
    if (g_help) delete g_help;

    return 0;
}

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
    aboutinfo->Add (new wxStaticText(this, -1, _("Written by: ")),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, APP_MAINT),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, _("Version: ")),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, APP_VERSION),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, _("Licence type: ")),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, APP_LICENCE),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, _("Copyright: ")),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, APP_COPYRIGTH),0, wxALIGN_LEFT);

    // about icontitle//info
    wxBoxSizer *aboutpane = new wxBoxSizer (wxHORIZONTAL);
    wxBitmap bitmap = wxBitmap(wxICON (hypertest));
    aboutpane->Add (new wxStaticBitmap (this, -1, bitmap),
                    0, wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 40);
    aboutpane->Add (aboutinfo, 1, wxEXPAND);
    aboutpane->Add (60, 0);

    // about complete
    wxBoxSizer *testpane = new wxBoxSizer (wxVERTICAL);
    testpane->Add (0, 20);
    wxStaticText *appname = new wxStaticText(this, -1, APP_NAME);
    appname->SetFont (wxFont (24, wxDEFAULT, wxNORMAL, wxBOLD));
    testpane->Add (appname, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 40);
    testpane->Add (0, 10);
    testpane->Add (aboutpane, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    testpane->Add (new wxStaticText(this, -1, _(APP_SYNOPSIS)),
                    0, wxALIGN_LEFT | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    testpane->Add (new wxStaticText(this, -1, _(APP_DESCR)),
                    0, wxALIGN_LEFT | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    testpane->Add (new wxStaticText(this, -1, APP_WEBSITE),
                    0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    wxButton *okButton = new wxButton (this, wxID_OK, _("OK"));
    okButton->SetDefault();
    testpane->Add (okButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit (testpane);
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


//----------------------------------------------------------------------------
// AppFrame
//----------------------------------------------------------------------------

BEGIN_EVENT_TABLE (AppFrame, wxFrame)
    // common events
    EVT_CLOSE (                      AppFrame::OnClose)
    // file events
    EVT_MENU (wxID_EXIT,             AppFrame::OnExit)
    // help events
    EVT_MENU (wxID_ABOUT,            AppFrame::OnAbout)
    EVT_MENU (wxID_HELP,             AppFrame::OnHelp)
    EVT_MENU (wxID_HELP_CONTENTS,    AppFrame::OnHelp)
END_EVENT_TABLE ()

AppFrame::AppFrame (const wxString &title)
        : wxFrame ((wxFrame *)NULL, -1, title, wxDefaultPosition, wxSize(480,240),
                    wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE) {

    // set icon and background
    SetIcon (wxICON (hypertest));

    // about box shown for 3 seconds
//?    AppAbout (this, 3000);

    // create menu
    CreateMenu ();

    // Test window
    m_test = new wxTextCtrl (this, -1, APP_DESCR, wxDefaultPosition, wxDefaultSize,
                                       wxTE_MULTILINE|wxTE_WORDWRAP|wxTE_READONLY);

}

AppFrame::~AppFrame () {
}

// common event handlers
void AppFrame::OnClose (wxCloseEvent &WXUNUSED(event)) {
    Destroy();
}

void AppFrame::OnAbout (wxCommandEvent &WXUNUSED(event)) {
    AppAbout (this);
}

void AppFrame::OnExit (wxCommandEvent &WXUNUSED(event)) {
    Close (true);
}

void AppFrame::OnHelp (wxCommandEvent &WXUNUSED(event)) {
    wxWindow *active = wxGetActiveWindow();
    wxString helptext;
    while (active && helptext.IsEmpty()) {
        helptext = active->GetHelpText();
        active = GetParent();
    }
    g_help->DisplayContents();
}

// private functions
void AppFrame::CreateMenu () {

    // File menu
    wxMenu *menuFile = new wxMenu;
    menuFile->Append (wxID_EXIT, _("&Quit\tCtrl+Q"));

    // Help menu
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append (wxID_HELP_CONTENTS, _("&Content\tF1"));
    menuHelp->Append (wxID_HELP, _("&Index\tF1"));
#ifndef __WXMAC__
    menuHelp->AppendSeparator();
#else
    wxApp::s_macAboutMenuItemId = wxID_ABOUT;
#endif
    menuHelp->Append (wxID_ABOUT, _("&About ...\tShift+F1"));

    // construct menu
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append (menuFile, _("&File"));
    menuBar->Append (menuHelp, _("&Help"));
    SetMenuBar (menuBar);

}
