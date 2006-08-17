//////////////////////////////////////////////////////////////////////////////
// File:        hypertest.cpp
// Purpose:     hypertest application
// Maintainer:  Otto Wyss
// Created:     2005-05-12
// RCS-ID:      $Id: hypertest.cpp,v 1.8 2006-08-17 20:21:42 wyo Exp $
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

//! wxCode headers
#include "wx/hyperlink.h" // wxHyperLink control

//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "hypertest.xpm"
#endif


//============================================================================
// declarations
//============================================================================

const wxString APP_NAME = _T("wxHyperLink");
const wxString APP_VENDOR = _T("wxCode");
const wxString APP_VERSION = _T("1.0.0");
const wxString APP_MAINT = _T("Otto Wyss");
const wxString APP_LICENCE = _T("wxWindows");
const wxString APP_COPYRIGTH = _T("(C) 2005 Otto Wyss");

const wxString APP_DESCR = _("\
wxHyperLink shows a text element which links to an URL. Clicking \n\
on the URL activates the standard browser (via mimetype) and \n\
opens the URL. No checks on the URL is done. The shown URL might \n\
be different from the opened URL.\
");
const wxString APP_WEBSITE = _T("http://wxcode.sourceforge.net/");
const wxString APP_WEBPAGE = _T("components/hyperlink/website/index.html");

const wxString APP_INFOS = _("\
This application is derived from the demo sample of wyoGuide.\
");
const wxString APP_WYOGUIDE = _T("http://wyoguide.sourceforge.net");


//----------------------------------------------------------------------------
//! global application name
wxString g_appname;


//----------------------------------------------------------------------------
//! class declarations
class AppFrame;


//----------------------------------------------------------------------------
//! application
class App: public wxApp {
    friend class AppAbout;
    friend class AppFrame;

public:
    // standard overrides
    virtual bool OnInit();

    //! application exit function
    virtual int OnExit ();

private:
    AppFrame *m_frame;

};

// created dynamically by wxWidgets
DECLARE_APP (App);


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


//----------------------------------------------------------------------------
//! frame of the application
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

private:
    //! creates the application menu bar
    void CreateMenu ();

    // split window
    wxPanel *m_test;

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

    // set application and vendor name
    SetAppName (APP_NAME);
    SetVendorName (APP_VENDOR);
    g_appname.Append (APP_NAME);

    // about box shown for 3 seconds
    AppAbout (NULL, 3000);

    // create application frame
    m_frame = new AppFrame (APP_NAME);

    // open application frame
    m_frame->Layout ();
    m_frame->Show (true);
    SetTopWindow (m_frame);

    return true;
}

int App::OnExit () {
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
    wxBitmap bitmap = wxBitmap(wxICON (hypertest));
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
    totalpane->Add (new wxStaticText(this, -1, APP_DESCR),
                    0, wxALIGN_LEFT | wxLEFT | wxRIGHT, 10);
    totalpane->Add (0, 6);
    wxHyperLink *website = new wxHyperLink (this, -1, APP_WEBSITE);
    wxString url = APP_WEBSITE;
    url.Append (APP_WEBPAGE);
    website->SetURL (url);
    totalpane->Add (website, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    totalpane->Add (new wxStaticText(this, -1, APP_INFOS),
                    0, wxALIGN_LEFT | wxLEFT | wxRIGHT, 10);
    totalpane->Add (0, 6);
    totalpane->Add (new wxHyperLink (this, -1, APP_WYOGUIDE),
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
END_EVENT_TABLE ()

AppFrame::AppFrame (const wxString &title)
        : wxFrame ((wxFrame *)NULL, -1, title, wxDefaultPosition, wxSize(320,240),
                    wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE) {

    // set icon and background
    SetIcon (wxICON (hypertest));

    // create menu
    CreateMenu ();

    // Test panel
    m_test = new wxPanel (this, -1, wxDefaultPosition, wxDefaultSize,
                          wxTAB_TRAVERSAL|wxCLIP_CHILDREN|wxNO_BORDER);
    wxBoxSizer *totalpane = new wxBoxSizer (wxVERTICAL);
    wxHyperLink *website = new wxHyperLink (m_test, -1, APP_WEBSITE);
    totalpane->Add (website, 0, wxALIGN_CENTER | wxALL, 20);

    m_test->SetSizerAndFit (totalpane);

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

// private functions
void AppFrame::CreateMenu () {

    // File menu
    wxMenu *menuFile = new wxMenu;
    menuFile->Append (wxID_EXIT, _("&Quit\tCtrl+Q"));

    // Help menu
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append (wxID_ABOUT, _("&About ...\tShift+F1"));

    // construct menu
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append (menuFile, _("&File"));
    menuBar->Append (menuHelp, _("&Help"));
    SetMenuBar (menuBar);

}
