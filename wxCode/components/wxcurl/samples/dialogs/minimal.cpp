////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxCURL sample
// Author:      Francesco Montorsi
// Modified by:
// Created:     04/01/07
// RCS-ID:      $Id$
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
#include "wx/wfstream.h"
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

    Minimal_Verbose = wxID_HIGHEST+1,
    Minimal_Download,
    Minimal_Upload,
    Minimal_ConnSettings,


    // transfer styles:
    Minimal_Elapsed_time,
    Minimal_Estimated_time,
    Minimal_Remaining_time,
    Minimal_Speed,
    Minimal_Size,
    Minimal_Url,
    Minimal_ConnSettingsAuth,
    Minimal_ConnSettingsPort,
    Minimal_ConnSettingsProxy,

    Minimal_Can_abort,
    Minimal_Can_start,
    Minimal_Can_pause,
    Minimal_Auto_close,

    Minimal_Bitmap,

    Minimal_CheckAll = wxID_APPLY,
    Minimal_UnCheckAll = wxID_CANCEL,


    // conn settings styles:
    Minimal_Authentication,
    Minimal_Port,
    Minimal_Proxy
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
    void OnDownload(wxCommandEvent &ev);
    void OnUpload(wxCommandEvent &ev);
    void OnConnSettings(wxCommandEvent &ev);
    void OnCheckAll(wxCommandEvent& event);

    int GetTransferStyle() const;
    int GetConnSettingsStyle() const;

    void LogResult(wxCurlDialogReturnFlag flag);

protected:

    wxMenu *m_menuTransferDlg, *m_menuFile, *m_menuConnSettingsDlg;

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
    EVT_MENU(Minimal_ConnSettings, MyFrame::OnConnSettings)

    EVT_MENU(Minimal_CheckAll, MyFrame::OnCheckAll)
    EVT_MENU(Minimal_UnCheckAll, MyFrame::OnCheckAll)

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

#ifdef __WXDEBUG__
    // create an useful log window
    wxLogWindow *pwindow = new wxLogWindow(frame, wxT("Log messages from wxCURL"));
    pwindow->GetFrame()->Move(50, 50+350);
    pwindow->GetFrame()->SetSize(800, 300);
#endif


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

    m_menuFile = new wxMenu;
#ifdef __WXDEBUG__
    m_menuFile->AppendCheckItem(Minimal_Verbose, _T("Be verbose"), _T("Does something only in debug builds!"));
    m_menuFile->Check(Minimal_Verbose, true);
    m_menuFile->AppendSeparator();
#endif
    m_menuFile->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));	

    m_menuTransferDlg = new wxMenu;
    m_menuTransferDlg->AppendCheckItem(Minimal_Elapsed_time, _T("Show elapsed time"));
    m_menuTransferDlg->AppendCheckItem(Minimal_Estimated_time, _T("Show estimated total time"));
    m_menuTransferDlg->AppendCheckItem(Minimal_Remaining_time, _T("Show estimated remaining time"));
    m_menuTransferDlg->AppendSeparator();
    m_menuTransferDlg->AppendCheckItem(Minimal_Speed, _T("Show transfer speed"));
    m_menuTransferDlg->AppendCheckItem(Minimal_Size, _T("Show how much was transferred so far"));
    m_menuTransferDlg->AppendCheckItem(Minimal_Url, _T("Show the URL of the transfer"));
    m_menuTransferDlg->AppendSeparator();
    m_menuTransferDlg->AppendCheckItem(Minimal_ConnSettingsAuth, _T("Allow the user to change authentication settings"));
    m_menuTransferDlg->AppendCheckItem(Minimal_ConnSettingsPort, _T("Allow the user to change port for the transfer"));
    m_menuTransferDlg->AppendCheckItem(Minimal_ConnSettingsProxy, _T("Allow the user to change proxy settings"));
    m_menuTransferDlg->AppendSeparator();
    m_menuTransferDlg->AppendCheckItem(Minimal_Can_abort, _T("Transfer can be aborted"));
    m_menuTransferDlg->AppendCheckItem(Minimal_Can_start, _T("Transfer do not start automatically"));
    m_menuTransferDlg->AppendCheckItem(Minimal_Can_pause, _T("Transfer can be paused"));
    m_menuTransferDlg->AppendSeparator();
    m_menuTransferDlg->AppendCheckItem(Minimal_Auto_close, _T("Auto-close dialog at completion"));
    m_menuTransferDlg->AppendSeparator();
    m_menuTransferDlg->AppendCheckItem(Minimal_Bitmap, _T("Show bitmap in the dialog"));
    m_menuTransferDlg->AppendSeparator();
    m_menuTransferDlg->Append(Minimal_CheckAll, _T("Check all"), _T("check all previous menu items"));    
    m_menuTransferDlg->Append(Minimal_UnCheckAll, _T("Uncheck all"), _T("uncheck all previous menu items"));    
    m_menuTransferDlg->AppendSeparator();
    m_menuTransferDlg->Append(Minimal_Download, _T("Download dialog..."), _T("Shows wxCurlDownloadDialog."));  
    m_menuTransferDlg->Append(Minimal_Upload, _T("Upload dialog..."), _T("Shows wxCurlUploadDialog."));

    m_menuConnSettingsDlg = new wxMenu;
    m_menuConnSettingsDlg->AppendCheckItem(Minimal_Authentication, _T("Show authentication fields"));
    m_menuConnSettingsDlg->AppendCheckItem(Minimal_Port, _T("Show port field"));
    m_menuConnSettingsDlg->AppendCheckItem(Minimal_Proxy, _T("Show proxy fields"));
    m_menuConnSettingsDlg->AppendSeparator();
    m_menuConnSettingsDlg->Append(Minimal_ConnSettings, _T("Connection settings..."), _T("Shows wxCurlConnectionSettingsDialog."));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(m_menuFile, _T("&File"));
    menuBar->Append(m_menuTransferDlg, _T("&Transfer dialogs"));
    menuBar->Append(m_menuConnSettingsDlg, _T("&Connection settings dialog"));
    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(_T("Welcome to wxWindows!"));
#endif // wxUSE_STATUSBAR

    // start with all possible styles checked
    wxCommandEvent fake(wxEVT_COMMAND_MENU_SELECTED, Minimal_CheckAll);
    OnCheckAll(fake);

    for (int id = Minimal_Authentication; id <= Minimal_Proxy; id++)
        m_menuConnSettingsDlg->Check(id, true);
}

MyFrame::~MyFrame()
{
}

int MyFrame::GetTransferStyle() const
{
    int ret = 0;

    if (m_menuTransferDlg->IsChecked(Minimal_Elapsed_time)) ret |= wxCTDS_ELAPSED_TIME;
    if (m_menuTransferDlg->IsChecked(Minimal_Estimated_time)) ret |= wxCTDS_ESTIMATED_TIME;
    if (m_menuTransferDlg->IsChecked(Minimal_Remaining_time)) ret |= wxCTDS_REMAINING_TIME;
    if (m_menuTransferDlg->IsChecked(Minimal_Speed)) ret |= wxCTDS_SPEED;
    if (m_menuTransferDlg->IsChecked(Minimal_Size)) ret |= wxCTDS_SIZE;
    if (m_menuTransferDlg->IsChecked(Minimal_Url)) ret |= wxCTDS_URL;
    if (m_menuTransferDlg->IsChecked(Minimal_ConnSettingsAuth)) ret |= wxCTDS_CONN_SETTINGS_AUTH;
    if (m_menuTransferDlg->IsChecked(Minimal_ConnSettingsPort)) ret |= wxCTDS_CONN_SETTINGS_PORT;
    if (m_menuTransferDlg->IsChecked(Minimal_ConnSettingsProxy)) ret |= wxCTDS_CONN_SETTINGS_PROXY;
    if (m_menuTransferDlg->IsChecked(Minimal_Can_abort)) ret |= wxCTDS_CAN_ABORT;
    if (m_menuTransferDlg->IsChecked(Minimal_Can_start)) ret |= wxCTDS_CAN_START;
    if (m_menuTransferDlg->IsChecked(Minimal_Can_pause)) ret |= wxCTDS_CAN_PAUSE;
    if (m_menuTransferDlg->IsChecked(Minimal_Auto_close)) ret |= wxCTDS_AUTO_CLOSE;

    return ret;
}

int MyFrame::GetConnSettingsStyle() const
{
    int ret = 0;

    if (m_menuConnSettingsDlg->IsChecked(Minimal_Authentication)) ret |= wxCCSP_AUTHENTICATION_OPTIONS;
    if (m_menuConnSettingsDlg->IsChecked(Minimal_Port)) ret |= wxCCSP_PORT_OPTION;
    if (m_menuConnSettingsDlg->IsChecked(Minimal_Proxy)) ret |= wxCCSP_PROXY_OPTIONS;

    return ret;
}

void MyFrame::LogResult(wxCurlDialogReturnFlag flag)
{
    switch (flag)
    {
        case wxCDRF_SUCCESS: wxLogMessage(wxT("Transfer was successful!")); break;
        case wxCDRF_USER_ABORTED: wxLogMessage(wxT("Transfer has been user-aborted.")); break;
        case wxCDRF_FAILED: wxLogMessage(wxT("Transfer was failed - you should have already been advised.")); break;
    }
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

void MyFrame::OnCheckAll(wxCommandEvent& event)
{
    for (int id = Minimal_Elapsed_time; id <= Minimal_Bitmap; id++)
        m_menuTransferDlg->Check(id, event.GetId() == Minimal_CheckAll);
}

void MyFrame::OnDownload(wxCommandEvent& WXUNUSED(event))
{
    wxString url = wxGetTextFromUser(
                      wxT("Please the enter the URL of the resource to download:"), wxT("Type an URL"),
                      wxT("http://kent.dl.sourceforge.net/sourceforge/wxcode/wxCode.tar.bz2"), this);
    if (url.empty())
        return;     // user hit cancel

    wxBitmap bmp;
    if (m_menuTransferDlg->IsChecked(Minimal_Bitmap))
        bmp = wxBitmap(www_xpm);

    wxFileOutputStream fos(wxT("downloaded_stuff"));
    wxCurlDownloadDialog dlg(url, &fos,
                             wxT("Download dialog title"),
                             wxT("Your message goes here...\nNote that the bitmap below can be hidden/customized."),
                             bmp,
                             this,
                             GetTransferStyle());
    dlg.SetVerbose(m_menuFile->IsChecked(Minimal_Verbose));

    if (!dlg.IsOk())
        return;

    wxCurlDialogReturnFlag flag = dlg.RunModal();
    LogResult(flag);

    if (flag == wxCDRF_SUCCESS && fos.GetLength() < 10000)
    {
        fos.Close();

        int reply = wxMessageBox(wxT("Do you want to open the downloaded file with your default browser?"),
                                 wxT("Open it?"), wxYES_NO, this);
        if (reply == wxYES)
            wxLaunchDefaultBrowser(wxT("downloaded_stuff"));
    }
}

void MyFrame::OnUpload(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog dlg(this, wxT("Choose a file to upload"), wxT(""), wxT(""), 
                     wxT("All files (*.*)|*.*"), wxFD_OPEN);

    if (dlg.ShowModal()!=wxID_OK)
        return;

    wxString file = dlg.GetPath();
    wxString url = wxGetTextFromUser(
                      wxT("Please the enter the URL where the previously-selected file should go:"),
                      wxT("Type an URL"),
                      wxT("http://"), this);
    if (url.empty())
        return;     // user hit cancel

    wxFileInputStream is(file);
    if (!is.IsOk())
        return;

    wxLogDebug(wxT("Going to update %d bytes"), is.GetSize());

    wxBitmap bmp;
    if (m_menuTransferDlg->IsChecked(Minimal_Bitmap))
        bmp = wxBitmap(www_xpm);

    wxCurlUploadDialog dlg2(url, &is,
                             wxT("Upload dialog title"),
                             wxT("Your message goes here...\nNote that the bitmap below can be hidden/customized."),
                             bmp,
                             this,
                             GetTransferStyle());
    dlg2.SetVerbose(m_menuFile->IsChecked(Minimal_Verbose));

    if (!dlg2.IsOk())
        return;

    LogResult(dlg2.RunModal());
}

void MyFrame::OnConnSettings(wxCommandEvent& WXUNUSED(event))
{
    wxCurlConnectionSettingsDialog 
        dlg(wxT("Connection settings"),
            wxT("Your message goes here..."),
            this, GetConnSettingsStyle());

    wxCurlBase curl;
    dlg.RunModal(&curl);

    wxLogMessage(wxT("Summary of the connection settings:\nUsername is '%s'\nPassword is '%s'\n")
                 wxT("Port is '%d'\nUse proxy is '%d'\nProxy host is '%s'\nProxy username is '%s'\n")
                 wxT("Proxy password is '%s'\nProxy port is '%d'"),
                 curl.GetUsername().c_str(), curl.GetPassword().c_str(), curl.GetPort(),
                 (int)curl.UseProxy(), curl.GetProxyHost().c_str(), curl.GetProxyUsername().c_str(),
                 curl.GetProxyPassword().c_str(), curl.GetProxyPort());
}

