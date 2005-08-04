/////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWindows sample
// Author:      Julian Smart
// Modified by: Francesco Montorsi
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////



// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "wx/palettefrm.h"
#include "wx/minibtn.h"
#include "wx/minifram.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif



// ============================================================================
// declarations
// ============================================================================

// first of all, decide if we can use the system...
#if defined(__VISUALC__) && defined(__WXDEBUG__)
	#define mcDETECT_MEMORY_LEAKS
#endif


#ifdef mcDETECT_MEMORY_LEAKS

	// "crtdbg.h" is included only with MSVC++ and Borland, I think...
	// "stackwalker.h" instead, contains a set of stack walker functions
	// created by Jochen Kalmbach (thanks !!!) which allow to read the
	// intercept unhandled exceptions and memory-leaks. 
	// To be used, the file must be part of the project; this is why
	// it's contained (with its CPP counterpart) in the folder of this
	// test program. Anyway,  you can find it also online at:
	//     http://www.codeproject.com/tools/leakfinder.asp
	#include <crtdbg.h>

	// define some useful macros
	#define mcDUMP_ON_EXIT		{ _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); }

	
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;


	// this little class is used to access Stackwalker functions
	// without changing a line of code...
	class mcLeakDetector {

	public:
		mcLeakDetector() { mcDUMP_ON_EXIT; }
		~mcLeakDetector() {}
	};

	// ...infact, instancing a STATIC mcLeakDetector class, we
	// can start memory-leak detection at the very beginning of
	// the program (when the main() or winmain() has not been
	// called yet, that is, when the framework is creating the
	// static variables of the program) and end it at the very
	// end of the program (when, after the main() or winmain(),
	// the framework removes the static variables).
	static mcLeakDetector detector;

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
	virtual int OnExit();
	
	wxPaletteFrame *pal1, *pal2;	
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

	Minimal_Pal1,
	Minimal_Pal2
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxMainFrame
{
protected:

	int GetPalMenuBaseId() const		 { return Minimal_Pal1; }

public:
    // ctor(s)
    MyFrame(const wxString& title);
	~MyFrame();

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

	void OnPalette1(wxCommandEvent &);
	void OnPalette2(wxCommandEvent &);

	void OnMinimizeBox(wxCommandEvent &);

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
BEGIN_EVENT_TABLE(MyFrame, wxMainFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)

	// we don't even need to intercept these menu events, because wxMainFrame
	// will take care of it... (but we can disable this feature using the
	// wxMainFrame::EnableContextMenuHandling(FALSE) function)
    //EVT_MENU(Minimal_Pal1,  MyFrame::OnPalette1)
    //EVT_MENU(Minimal_Pal2, MyFrame::OnPalette2)

#ifdef wxPALETTEFRM_USE_MINIBTN
	// intercept a minibtn click just to show how it's done...
	EVT_MINIMIZEBOX_CLICKED(-1, MyFrame::OnMinimizeBox)
#endif

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

wxPaletteFrame *CreateNewPaletteWindow(wxMainFrame *parent, const wxString &str)
{
	// create the new palette window
	wxPaletteFrame *my = new wxPaletteFrame(parent, -1, str, 
							wxPoint(200, 200), wxSize(100, 100));	

	// put something in its client window
#if 1

#ifdef wxPALETTEFRM_USE_PALETTEFRM
	wxWindow *win = my->CreateClient();	
#else
	wxWindow *win = new wxPanel(my, -1);
#endif	
	
	wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
	box->Add(new wxStaticText(win, -1, wxT("Hi; this is a wxPaletteFrame which contains a\n")
										wxT("wxBoxSizer with some common controls just to show\n")
										wxT("you that it can contain anything you want...")), 0, wxALL, 5);
	box->Add(new wxTextCtrl(win, -1, wxT("You can type here what you want..."), 
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE), 2, wxGROW | wxALL, 5);

	wxBoxSizer *box2 = new wxBoxSizer(wxHORIZONTAL);
	box2->Add(new wxButton(win, -1, wxT("I'm a dummy button")), 1, wxGROW);

	box->Add(box2, 1, wxALL | wxALIGN_CENTER | wxGROW, 5);
	win->SetSizer(box);
	box->SetSizeHints(win);
		
#ifdef wxPALETTEFRM_USE_PALETTEFRM
	// this must be called at the end of the palette creation
	my->SetSizeHintsUsingClientHints();
#endif	
	
#endif

	return my;
}


// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame(_T("Minimal wxWindows App"));

	// create an useful log window
	wxLogWindow *pwindow = new wxLogWindow(frame, wxT("log"));
	pwindow->GetFrame()->Move(50, 50+350);
	pwindow->GetFrame()->SetSize(800, 300);

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);
	

	// VERY IMPORTANT: to make the GTK version of wxPaletteFrame work
	// correctly, you have to show all wxPaletteFrames *after* you've
	// shown the main frame.

#if 0
	wxFrame *my = new wxFrame(frame, -1, wxT("ciao"), 
							wxPoint(200, 200), wxSize(100, 100));	
	new wxTextCtrl(my, -1);
	my->Show();
#else

	// 1) create & personalize the wxPaletteFrame(s)
	pal1 = CreateNewPaletteWindow(frame, wxT("wxPaletteFrame #1"));
	pal2 = CreateNewPaletteWindow(frame, wxT("wxPaletteFrame #2"));
	
#ifdef wxPALETTEFRM_USE_MINIBTN
	pal1->AddMiniButton(new wxCollapseBox(pal1));	
	pal2->AddMiniButton(new wxMaximizeBox(pal2));
	pal2->AddMiniButton(new wxMinimizeBox(pal2));
#endif

	// 2) try to load layouts
	wxConfig *pcfg = new wxConfig(wxT("wxPaletteFrame sample"));
	bool b = TRUE;

#ifdef wxPALETTEFRM_USE_PALETTEFRM
#ifndef __WXX11__
	b &= pal1->LoadLayout(pcfg);
	b &= pal2->LoadLayout(pcfg);
#endif
#else
	b = FALSE;
#endif
		
	if (!b) {
		
		// 3) eventually set default position 
		pal1->Move(500, 80);
		pal2->Move(500, 320);
	
		// 4) show the palettes
		pal1->Show();
		pal2->Show();
	}

	delete pcfg;
	
	// 5) set up palette's context menu and enable it
#ifdef wxPALETTEFRM_USE_MAINFRAME
	frame->InitPalContextMenu();
	frame->EnableContextMenu(); 			// this would not be necessary but I want to show its existence
	frame->EnableContextMenuHandling();		// also this one is already enabled by default...
	frame->EnableMainMenubarUpdate();		// .. this one is necessary, instead
#endif

	frame->SetFocus();
#endif

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

int MyApp::OnExit()
{
	return wxApp::OnExit();
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------


// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxMainFrame(NULL, wxID_ANY, title, wxPoint(50, 50), wxSize(400, 300))
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

#if 1
	// create a child panel
	wxPanel *panel = new wxPanel(this);
	wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
	box->Add(new wxStaticText(panel, -1, 
			wxT("As you can see, the wxPaletteFrame-derived classes\n")
			wxT("can contain not only any type of client windows\n")
			wxT("but they can also have special wxMiniButtons like\n")
			wxT("the wxCollapseBox you can see in the caption bar of\n")
			wxT("the first palette window...")), 0, wxALL, 5);
	wxTextCtrl *main = new wxTextCtrl(panel, -1, wxT(""), wxDefaultPosition, 
		wxDefaultSize, wxTE_MULTILINE);
	box->Add(main, 1, wxALL | wxGROW, 5);
	panel->SetSizer(box);
#endif

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *menuFile = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));

	menuFile->AppendCheckItem(Minimal_Pal1, _T("Show palette #1"), _T(""));
	menuFile->AppendCheckItem(Minimal_Pal2, _T("Show palette #2"), _T(""));
	menuFile->AppendSeparator();

    menuFile->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));	

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
#ifdef wxPALETTEFRM_USE_PALETTEFRM
#ifndef __WXX11__
	// 7) save layouts
	wxConfig *pcfg = new wxConfig(wxT("wxPaletteFrame sample"));
	wxGetApp().pal1->SaveLayout(pcfg);
	wxGetApp().pal2->SaveLayout(pcfg);
	delete pcfg;
#endif
#endif
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
    msg.Printf( _T("This is the About dialog of the minimal sample.\n")
                _T("Welcome to %s"), wxVERSION_STRING);

    wxMessageBox(msg, _T("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnMinimizeBox(wxCommandEvent &)
{
	wxMessageBox(wxT("Hi !!\nYou clicked a wxMinimizeBox."), wxT("Dummy msg"), wxOK, this);
}



