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

#include "wx/webupdatedlg.h"
#include <wx/filesys.h>
#include <wx/fs_inet.h>
#include <wx/wfstream.h>

// these are the info required by wxWebUpdate classes about the 
// application to update...
#define VERSION				wxT("1.5.0")
#define APP_NAME			wxT("wxWebUpdate ADVANCED sample")
#define PACKAGE_NUM			1
#define PACKAGE_NAME		wxT("myapp")	// just to show this can be different from APP_NAME
#define SCRIPT_LOCATION		wxT("http://wxcode.sourceforge.net/components/webupdate/script2.xml")

// our list of local packages; used only by wxWebUpdateDlg.
wxWebUpdateLocalPackage g_packageList[PACKAGE_NUM];



#ifdef __WXMSW__
#include <crtdbg.h>
#define mcDUMP_ON_EXIT		\
	{ _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); }
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
	void OnUpdateCheckWithDlg(wxCommandEvent& event);

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
    EVT_MENU(Minimal_UpdateCheckWithDlg, MyFrame::OnUpdateCheckWithDlg)
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
#ifdef __WXMSW__
	mcDUMP_ON_EXIT;			// for debugging only
#endif

    // create the main application window
    MyFrame *frame = new MyFrame(APP_NAME);

#if __WXDEBUG__
	// create an useful log window
	wxLogWindow *pwindow = new wxLogWindow(frame, wxT("log"));
	pwindow->GetFrame()->Move(50, 50+350);
	pwindow->GetFrame()->SetSize(800, 300);

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);
#endif

	wxSocketBase::Initialize() ;
	wxFileSystem::AddHandler(new wxInternetFSHandler);	

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
	sz->Add(new wxStaticText(panel, -1, 
		wxString(wxT("Program version: ")) + VERSION), 0, wxGROW | wxALL, 5);
    sz->Add(new wxTextCtrl(panel, -1, 
		wxT("This program provides an example of wxUpdateCheck features:\n")
		wxT("\t- wxUpdateCheck: downloads a simple text file from your web server and")
		wxT("parses it to retrieve info about new updates.\n")
		wxT("\t- wxWebUpdateDlg: a dialog with all advanced update options.\n"),
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
    msg.Printf( wxT("This is a demonstration of wxUpdateCheck & wxWebUpdate program.\n\n")
				wxT("This sample program ..."));

    wxMessageBox(msg, wxT("About ") + wxString(APP_NAME), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnUpdateCheckSimple(wxCommandEvent &)
{
	// we want to open the script file I've put on wxCode server
	wxWebUpdateXMLScript script(SCRIPT_LOCATION);
	if (!script.IsOk()) {
		wxMessageBox(wxString(wxT("Cannot open the XML update script at: ")) + 
					SCRIPT_LOCATION, wxT("Error"), wxOK | wxICON_ERROR);
		return;
	}

	wxWebUpdatePackage *update = script.GetPackage(PACKAGE_NAME);
	if (!update) {
		wxMessageBox(wxT("Cannot find the 'myapp' package in the XML update script !"),
					wxT("Error"), wxOK | wxICON_ERROR);
		return;
	}

	if (update->Check(VERSION) == wxWUCF_UPDATED) {
		
		// no updates available
		wxMessageBox(wxT("This program is up to date ;-)\n"),
					wxT("Update check successful"));
		delete update;
		return;
	}
	
	int res = wxMessageBox(wxT("The webserver holds an updated version of this app;\n\n") 
		wxT("Local version: ") + wxString(VERSION) + wxT("\t\tLatest version: ") + 
		update->GetLatestVersion() +
		wxT("\n\nDo you want to download it ?"),
		wxT("Update check successful"), wxYES_NO | wxICON_EXCLAMATION);
	
	if (res == wxNO) {
		delete update;
		return;
	}
	
	wxWebUpdateDownload download = update->GetDownloadPackage();
	if (!download.IsOkForThisPlatform()) {
		wxMessageBox(wxString(wxT("The XML script does not support this platform (")) +
			wxWebUpdateDownload::GetThisPlatformString() + wxT(")"),
			wxT("Error"), wxOK | wxICON_ERROR);
		delete update;
		return;	
	} 
	
	
	// we now have:
	// - a valid XML webupdate script
	// - the permission by the user to download the update
	// - the link for the download of the updated package for this platform
	// that's all ;-)
	wxFileSystem fs;
	wxFSFile *updatefile = fs.OpenFile(download.GetDownloadString());
	if (!updatefile){
		wxMessageBox(wxT("Cannot download the update file from:\n\n") +
			download.GetDownloadString(), wxT("Error"), wxOK | wxICON_ERROR);
		return;
	}
	
	wxInputStream *stream = updatefile->GetStream();
	if (!stream){
		wxMessageBox(wxT("Cannot download the update file from:\n\n") +
			download.GetDownloadString(), wxT("Error"), wxOK | wxICON_ERROR);
		return;
	}
	
	wxFileOutputStream out(wxFileName::CreateTempFileName(APP_NAME));
	out.Write(*stream);
	if (!out.IsOk()){
		wxMessageBox(wxT("Cannot download the update file from:\n\n") +
			download.GetDownloadString(), wxT("Error"), wxOK | wxICON_ERROR);
		return;
	}
	
	wxMessageBox(wxT("Download was successful; now I'm going to install the update..."), 
		wxT("Success"), wxOK | wxICON_QUESTION);
	
	// cleanup
	delete updatefile;
	delete update;
}

void MyFrame::OnUpdateCheckWithDlg(wxCommandEvent &)
{
	// this sample has only one package to handle
	g_packageList[0].m_strName = PACKAGE_NAME;
	g_packageList[0].m_version = VERSION;

	wxWebUpdateDlg dlg(this, APP_NAME, SCRIPT_LOCATION, g_packageList, 1);
	dlg.CenterOnScreen();
	dlg.ShowModal();
}

