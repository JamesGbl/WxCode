////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWindows sample
// Author:      Julian Smart
// Modified by: Francesco Montorsi
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

#ifdef __WXMSW__
	#define wxUPDATE_AND_EXIT(uri)					\
		wxExecute(wxT("webupdater.exe ") uri);		\
		Close(true);
#else
	#define wxUPDATE_AND_EXIT(uri)					\
		wxExecute(wxT("./webupdater ") uri);		\
		Close(true);
#endif

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

				// these were added by me
	Minimal_UpdateCheckSimple,
	Minimal_UpdateCheckWithDlg,
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

	void OnUpdateCheckSimple(wxCommandEvent& event);
	void OnUpdateCheckAdv(wxCommandEvent& event);

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

    EVT_MENU(Minimal_UpdateCheckSimple, MyFrame::OnUpdateCheckSimple)
    EVT_MENU(Minimal_UpdateCheckWithDlg, MyFrame::OnUpdateCheckAdv)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)



// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// wxT('Main program') equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    SetAppName(wxT("wxWebUpdate SIMPLE sample"));
    MyFrame *frame = new MyFrame(GetAppName());

#ifdef __WXDEBUG__
	// create an useful log window
	wxLogWindow *pwindow = new wxLogWindow(frame, wxT("log"));
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

#if 1    
	wxPanel *panel = new wxPanel(this, -1);
    wxSizer *sz = new wxBoxSizer(wxVERTICAL);

	// create the wxTextCtrl where the file structure is shown
	sz->Add(new wxStaticText(panel, -1, wxT("Program EXE version: 1.0.0")), 0, wxGROW | wxALL, 5);
    sz->Add(new wxTextCtrl(panel, -1, 
		wxT("This program provides an example of the WebUpdate component simplest features:\n")
		wxT(" - all the configuration settings of the webupdater are saved in an XML file.\n")
		wxT(" - the webupdater is able to replace the program executable since it runs when ")
  		wxT("the target program, i.e. the program to update, is not running.\n\n")
    	wxT("For more info look at the WebUpdate documentation."),
		wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE), 1, wxGROW);
    panel->SetSizer(sz);
    sz->SetSizeHints(panel);
#endif

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(Minimal_UpdateCheckSimple, _T("Check for updates (simple)"), 
            _T("Checks for updates and eventually downloads the update version..."));	
    menuFile->Append(Minimal_UpdateCheckWithDlg, _T("Check for updates (advanced)"), 
            _T("Checks for updates and eventually downloads the update version..."));	
    menuFile->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));	
    
    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _T("&File"));
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
    msg.Printf( wxT("This is a demonstration of wxWebUpdate component.\n\n")
				wxT("Look at WebUpdate docs for a quick guide to integration of the ")
    			wxT("WebUpdater application into your program."));

    wxMessageBox(msg, wxT("About ") + wxGetApp().GetAppName(), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnUpdateCheckSimple(wxCommandEvent &)
{
	wxUPDATE_AND_EXIT(wxT("local.xml"));
}

void MyFrame::OnUpdateCheckAdv(wxCommandEvent &)
{
	wxUPDATE_AND_EXIT(wxT("local2.xml"));
}

