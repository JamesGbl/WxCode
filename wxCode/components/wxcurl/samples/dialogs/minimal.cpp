////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWindows sample
// Author:      Francesco Montorsi
// Modified by:
// Created:     04/01/07
// RCS-ID:      $Id: minimal.cpp,v 1.2 2005/11/08 11:23:59 frm Exp $
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "mondrian.xpm"
#endif


#include "wx/filename.h"
#include "wx/curl/dialog.h"
#include "www.xpm"


// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();

    int OnExit();
};

// ----------------------------------------------------------------------------
// constants	
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT,

    // add here new menu IDs
    Minimal_Download,
    Minimal_Upload,

    // styles:
    Minimal_Elapsed_time,
    Minimal_Estimated_time,
    Minimal_Remaining_time,
    Minimal_Speed,
    Minimal_Size,
    Minimal_Url,

    Minimal_Can_abort,
    Minimal_Can_start,
    Minimal_Can_pause,
    Minimal_Auto_close
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);
    ~MyFrame();

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    // add here event handlers
    void OnDownload(wxCommandEvent &ev);
    void OnUpload(wxCommandEvent &ev);

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
    EVT_MENU(Minimal_Download, MyFrame::OnDownload)
    EVT_MENU(Minimal_Upload, MyFrame::OnUpload)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// wxT('Main program') equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame(_T("Minimal wxWindows App"));
/*
#if 1
    // create an useful log window
    wxLogWindow *pwindow = new wxLogWindow(frame, wxT("log"));
    pwindow->GetFrame()->Move(50, 50+350);
    pwindow->GetFrame()->SetSize(800, 300);
#endif
*/

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

int MyApp::OnExit()
{
    return 0;
}


// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxPoint(50, 50), wxSize(500, 300))
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

#if wxUSE_MENUS
    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(Minimal_Download, _T("Download dialog..."), _T("Shows wxCurlDownloadDialog."));	
    menuFile->Append(Minimal_Upload, _T("Upload dialog..."), _T("Shows wxCurlUploadDialog."));
    menuFile->AppendSeparator();
    menuFile->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));	

    wxMenu *menuDialog = new wxMenu;
    menuDialog->AppendCheckItem(Minimal_Elapsed_time, _T("Show elapsed time"));
    menuDialog->AppendCheckItem(Minimal_Estimated_time, _T("Show elapsed time"));
    menuDialog->AppendCheckItem(Minimal_Remaining_time, _T("Show elapsed time"));
    menuDialog->AppendSeparator();
    menuDialog->AppendCheckItem(Minimal_Speed, _T("Show elapsed time"));
    menuDialog->AppendCheckItem(Minimal_Size, _T("Show elapsed time"));
    menuDialog->AppendCheckItem(Minimal_Url, _T("Show elapsed time"));
    menuDialog->AppendSeparator();
    menuDialog->AppendCheckItem(Minimal_Can_abort, _T("Show elapsed time"));
    menuDialog->AppendCheckItem(Minimal_Can_start, _T("Show elapsed time"));
    menuDialog->AppendCheckItem(Minimal_Can_pause, _T("Show elapsed time"));
    menuDialog->AppendSeparator();
    menuDialog->AppendCheckItem(Minimal_Auto_close, _T("Show elapsed time"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _T("&File"));
    menuBar->Append(menuDialog, _T("&Dialog style"));
    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(_T("Welcome to wxWindows!"));
#endif // wxUSE_STATUSBAR
}

MyFrame::~MyFrame()
{
}



// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( wxT("This is a little demonstration of wxCurl dialogs by Francesco Montorsi.\n"));
    wxMessageBox(msg, _T("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnDownload(wxCommandEvent& WXUNUSED(event))
{
    wxCurlDownloadDialog dlg(wxT("http://switch.dl.sourceforge.net/sourceforge/bakefile/bakefile-0.2.2-setup.exe"),
                            NULL,
                            wxT("Downloading"),
                            wxT("asdfasfdsafdas asd fads fasd fasdfas fasd fsad "),
                            wxBitmap(www_xpm),
                            this,
                    wxDLD_CAN_START|wxDLD_CAN_PAUSE|wxDLD_CAN_ABORT|wxDLD_SHOW_ALL);
    switch (dlg.StartModal())
    {
    case wxDRF_SUCCESS: wxLogDebug(wxT("success")); break;
    case wxDRF_USER_ABORTED: wxLogDebug(wxT("user aborted")); break;
    case wxDRF_FAILED: wxLogDebug(wxT("failed")); break;
    }

    wxLogDebug(wxT("download event!\n\t")
            wxT("URL: %s\n\t")
            wxT("Speed: %s\n\t")
            wxT("Bytes downloaded: %lu/%lu (%s/%s)\n\t")
            wxT("Percent: %.4f%%\n\t")
            wxT("Elapsed time: %s\n\t")
            wxT("Estimated total time: %s\n\t")
            wxT("Estimated remaining time: %s"),
            ev.GetURL().c_str(),
            ev.GetHumanReadableSpeed().c_str(),
            (unsigned long)ev.GetDownloadedBytes(),
            (unsigned long)ev.GetTotalBytes(),
            ev.GetHumanReadableDownloadedBytes().c_str(),
            ev.GetHumanReadableTotalBytes().c_str(),
            ev.GetPercent(),
            ev.GetElapsedTime().Format().c_str(),
            ev.GetEstimatedTime().Format().c_str(),
            ev.GetEstimatedRemainingTime().Format().c_str());
}


