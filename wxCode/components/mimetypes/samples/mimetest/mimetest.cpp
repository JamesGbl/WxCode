//////////////////////////////////////////////////////////////////////////////
// File:        mimetest.cpp
// Purpose:     mimetest application
// Maintainer:  Otto Wyss
// Created:     2005-03-07
// RCS-ID:      $Id: mimetest.cpp,v 1.8 2006-08-17 20:22:19 wyo Exp $
// Copyright:   (c) 2005 wxCode
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
#include <wx/filedlg.h>  // file selector dialog
#include <wx/filename.h> // file name support
#include <wx/textdlg.h>  // text from user dialog

//! wxCode headers
#include "wx/mimetypes.h" // wxMimeType control

// detecting memory leaks on Windows with _CrtSetBreakAlloc (<memory_number>)
// #include <crtdbg.h>

//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "mimetest.xpm"
#endif


//============================================================================
// declarations
//============================================================================

const wxString APP_NAME = _T("wxMimeTypes");
const wxString APP_VENDOR = _T("wxCode");
const wxString APP_VERSION = _T("1.0.0");
const wxString APP_MAINT = _T("Otto Wyss");
const wxString APP_LICENCE = _T("wxWindows");
const wxString APP_COPYRIGTH = _T("(C) 2005 Otto Wyss");

const wxString APP_DESCR = _("\
wxMimeTypes allows to access all information of the mime type \n\
of an application. An application can check if its mime type \n\
is correctly set and if not to install the mime type.\
");
const wxString APP_WEBSITE = _T("http://wxcode.sourceforge.net/");

const wxString APP_INFOS = _("\
This application is derived from the demo sample of wyoGuide.\
");
const wxString APP_WYOGUIDE = _T("http://wyoguide.sourceforge.net");


// menu id's
enum {
    myID_EXISTS = wxID_HIGHEST,
    myID_MATCH,
    myID_ISSTANDARD,
    myID_GETDATA,
};


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

    void OnTest (wxCommandEvent &event);

private:
    //! creates the application menu bar
    void CreateMenu ();

    wxMimeType* m_mime;
    wxTextCtrl* m_data;

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
    wxBitmap bitmap = wxBitmap(wxICON (mimetest));
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


//----------------------------------------------------------------------------
// AppFrame
//----------------------------------------------------------------------------

BEGIN_EVENT_TABLE (AppFrame, wxFrame)
    // common events
    EVT_CLOSE (                      AppFrame::OnClose)
    // file events
    EVT_MENU (wxID_EXIT,             AppFrame::OnExit)
    // test events
    EVT_MENU (myID_EXISTS,           AppFrame::OnTest)
    EVT_MENU (myID_MATCH,            AppFrame::OnTest)
    EVT_MENU (myID_ISSTANDARD,       AppFrame::OnTest)
    EVT_MENU (myID_GETDATA,          AppFrame::OnTest)
    // help events
    EVT_MENU (wxID_ABOUT,            AppFrame::OnAbout)
END_EVENT_TABLE ()

AppFrame::AppFrame (const wxString &title)
        : wxFrame ((wxFrame *)NULL, -1, title, wxDefaultPosition, wxDefaultSize,
                    wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE) {

    // set icon and background
    SetIcon (wxICON (mimetest));

    // create menu
    CreateMenu ();

    // get mime type
    m_data = new wxTextCtrl (this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

}

AppFrame::~AppFrame () {
}

// common event handlers
void AppFrame::OnClose (wxCloseEvent &event) {
    Destroy();
}

void AppFrame::OnAbout (wxCommandEvent &WXUNUSED(event)) {
    AppAbout (this);
}

void AppFrame::OnExit (wxCommandEvent &WXUNUSED(event)) {
    Close (true);
}

void AppFrame::OnTest (wxCommandEvent &event) {

    // ask for file return if none
    wxString file = wxFileSelector(wxFileSelectorPromptStr);
    if (file.IsEmpty() || !wxFileExists (file)) return; // no file

    // create mime type
    m_mime = new wxMimeType();
    wxMimeTypeData data;
    data.extension = wxFileName (file).GetExt();
    data.filename = wxFileName (file).GetFullName();
    data.description = _T("");
    data.appname = _T("");
    if ((event.GetId() == myID_MATCH) ||
        (event.GetId() == myID_ISSTANDARD)) {
        data.appname = wxGetTextFromUser (_T("Enter the application name"));
    }

    // check mime type
    wxString text;
    if (event.GetId() == myID_EXISTS) {
        if (m_mime->Exists (&data)) {
            text = wxString::Format (_T("Mime type of %s exists\n\n"), data.extension.c_str());
        }else{
            text = wxString::Format (_T("Mime type of %s doesn't exist\n\n"), data.extension.c_str());
        }
    }else if (event.GetId() == myID_MATCH) {
        if (m_mime->Match (&data)) {
            text = wxString::Format (_T("Mime type of %s matches\n\n"), data.extension.c_str());
        }else{
            text = wxString::Format (_T("Mime type of %s doesn't match\n\n"), data.extension.c_str());
        }
    }else if (event.GetId() == myID_ISSTANDARD) {
        if (m_mime->IsStandard (&data)) {
            text = wxString::Format (_T("Mime type of %s is standard\n\n"), data.extension.c_str());
        }else{
            text = wxString::Format (_T("Mime type of %s is not standard\n\n"), data.extension.c_str());
        }
    }else if (event.GetId() == myID_GETDATA) {
        if (m_mime->GetData (&data)) {
            text = wxString::Format (_T("Extension: %s\nDescription: %s\nAppname: %s\n\n"),
                                     data.extension.c_str(), data.description.c_str(), data.appname.c_str());
        }else{
            text = wxString::Format (_T("Mime type of %s doesn't exist\n\n"), data.extension.c_str());
        }
    }
    m_data->SetValue (text);
}

// private functions
void AppFrame::CreateMenu () {

    // File menu
    wxMenu *menuFile = new wxMenu;
    menuFile->Append (wxID_EXIT, _("&Quit\tCtrl+Q"));

    // Test menu
    wxMenu *menuTest = new wxMenu;
    menuTest->Append (myID_EXISTS, _("&Exists"));
    menuTest->Append (myID_MATCH, _("&Match"));
    menuTest->Append (myID_ISSTANDARD, _("&Is standard"));
    menuTest->Append (myID_GETDATA, _("&Get data"));

    // Help menu
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append (wxID_ABOUT, _("&About ...\tShift+F1"));

    // construct menu
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append (menuFile, _("&File"));
    menuBar->Append (menuTest, _("&Test"));
    menuBar->Append (menuHelp, _("&Help"));
    SetMenuBar (menuBar);

}
