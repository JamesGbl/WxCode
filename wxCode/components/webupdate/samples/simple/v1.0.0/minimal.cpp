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

#define VERSION				wxT("1.0.0")
#define APP_NAME			wxT("wxWebUpdate SIMPLE sample")

void wxUpdateWebUpdaterIfRequired();

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
#include "wx/textfile.h"

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
	wxUpdateWebUpdaterIfRequired();

    // create the main application window
    SetAppName(APP_NAME);
    MyFrame *frame = new MyFrame(GetAppName());

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);
    
    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
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

#if 1			// this section just makes the sample nicer 
				// (you don't need to add it to your program)

	// load our simple datafile
	wxString text;
	wxTextFile t;

	// when running the sample using the MSVC6PRJ the working directory is build/
	// this is bad since we need as working directory the sample's folder 
	// (otherwise: 
	//  - we would need to override all standard paths of WebUpdater 
	//    (using something like --xrc=../samples/simple/v1.0.0/webupdatedlg.xrc ...)
	//  - the $(programdir) WebUpdater keyword would point to the build/ folder
	//    and we would need to override it in the local.xml script
	// ) so we just refuse to run from build/ which keeps this sample simple.
	//
	// If you want to run this sample from MSVC6PRJ folder then you should change
	// the working directory using Project->Settings->Debug->'Working directory'
	// editbox, setting it to ..\samples\simple\vVERSION_OF_THIS_SAMPLE
	wxASSERT_MSG(wxGetCwd().Right(5) != wxT("build"), 
		wxT("You must set the correct working directory before running this sample !")
		wxT("See the comments in the source file of this minimal sample for more info."));

	if (!t.Open(wxT("simpledata.txt")))
		wxMessageBox(wxT("Couldn't load my datafile !"), wxT("Error"));
	else
		for (int i=0; i < (int)t.GetLineCount(); i++)
			text += t.GetLine(i) + wxT("\n");

	wxPanel *panel = new wxPanel(this, -1);
    wxSizer *sz = new wxBoxSizer(wxVERTICAL);

	// create the wxTextCtrl where the file structure is shown
	wxFont f(*wxNORMAL_FONT);
	wxStaticText *st = new wxStaticText(panel, -1, wxT("Version of this program: ") VERSION,
									wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	f.SetWeight(wxFONTWEIGHT_BOLD);
	st->SetFont(f);
	sz->Add(st, 0, wxGROW | wxALL, 5);
    sz->Add(new wxTextCtrl(panel, -1, 
		wxT("This program provides an example of the WebUpdate component simplest features:\n")
		wxT(" 1) all the configuration settings of the webupdater are saved in an XML file (the 'local script').\n\n")
		wxT(" 2) all the informations about the updates are saved in another XML file (the 'remote script') which ")
		wxT("can be placed in your webserver or on a CDROM or other support.\n\n")
		wxT(" 3) the webupdater is able to replace the program executable since it runs when ")
  		wxT("the target program, i.e. the program to update, is not running.\n\n")
		wxT(" 4) the GUI of the WebUpdater can be chosen at run-time since it loads it from an XRC file. ")
		wxT("The name of the XRC file loaded is fixed but the name of the resource loaded is defined in the ")
		wxT("local XML script so that you can easily change the GUI of the WebUpdater keeping untouched the EXE.\n")
		wxT("In fact, the only difference between the two 'File' menuitems is that they tell WebUpdater to use ")
		wxT("different dialogs for the update process...\n\n")
    	wxT("For more info and for the full list of WebUpdater features, look at the WebUpdate documentation."),
		wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE), 2, wxGROW);

	sz->Add(new wxStaticText(panel, -1, wxT("A simple datafile of this program: ")), 0, wxGROW | wxALL, 5);
    sz->Add(new wxTextCtrl(panel, -1, text,
		wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE), 1, wxGROW);
    
	panel->SetSizer(sz);
    sz->SetSizeHints(panel);
	sz->SetSizeHints(this);
	SetSize(GetMinSize().GetWidth()*3, (int)(GetMinSize().GetHeight()*1.1));
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



/* 
   THESE ARE THE ONLY REAL MODIFICATION REQUIRED TO INTEGRATE WEBUPDATER
   IN YOUR PROGRAM USING IT IN THE SIMPLEST WAY (SEE WEBUPDATER DOCS)
*/
#include "wx/filename.h"

// call this in the event handler used to show the wxWebUpdateDlg
void wxUpdateAndExit(wxFrame *caller, 
					bool savelog = FALSE,
     				bool restart = TRUE,
     				const wxString &xrc = wxEmptyString, 	// --xrc option won't be given using wxEmptyString
         			const wxString &res = wxEmptyString,	// --res option won't be given using wxEmptyString
            		const wxString &xml = wxEmptyString,	// --xml option won't be given using wxEmptyString
         			const wxString &uri = wxEmptyString)	// --uri option won't be given using wxEmptyString
{
	wxString opts;
 
 	if (savelog)
  		opts += wxT(" --savelog");
    if (restart)
    	opts += wxT(" --restart");
    if (!xrc.IsEmpty())
     	opts += wxT(" --xrc=") + xrc;
    if (!res.IsEmpty())
    	opts += wxT(" --res=") + res;
 	if (!xml.IsEmpty())
  		opts += wxT(" --xml=") + xml;
 	if (!uri.IsEmpty())
  		opts += wxT(" --uri=") + uri;

#ifdef __WXMSW__
	wxExecute(wxT("webupdater.exe") + opts);
#else	
	wxExecute(wxT("./webupdater") + opts);
#endif
	caller->Close(true);
}

// to be called in your wxApp::OnInit()
void wxUpdateWebUpdaterIfRequired()
{
#ifdef __WXMSW__
	wxString newupdater = wxT("_webupdater.exe"), oldupdater = wxT("webupdater.exe");
#else
	wxString newupdater = wxT("_webupdater"), oldupdater = wxT("webupdater");	
#endif
	if (wxFileName::FileExists(newupdater)) {
		wxRemoveFile(oldupdater);
		wxRenameFile(newupdater, oldupdater);
	}
}

void MyFrame::OnUpdateCheckSimple(wxCommandEvent &)
{
	wxUpdateAndExit(this, TRUE, TRUE, wxT("webupdatedlg.xrc"), wxT("wxWebUpdateSimpleDlg"));
}

void MyFrame::OnUpdateCheckAdv(wxCommandEvent &)
{
	wxUpdateAndExit(this, TRUE, TRUE, wxT("webupdatedlg.xrc"), wxT("wxWebUpdateDlg"));
}

