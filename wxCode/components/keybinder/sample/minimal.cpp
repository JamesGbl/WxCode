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
#include "wx/keybinder.h"
#include "wx/menuutils.h"

#define wxUSE_KEYBINDER		1

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

	Minimal_Shortcut1,
	Minimal_Shortcut2,
	Minimal_Shortcut3,
	Minimal_Keybindings,
	Minimal_ShowKeyProfiles
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
protected:
	wxADD_KEYBINDER_SUPPORT();

public:

	// THE KEYPROFILES DO NOT NEED TO BE STATIC (EVEN IF THESE ONES ARE)	

	// our default keyprofile
	static wxKeyProfile *pPrimary;

	// our secondary keyprofile
	static wxKeyProfile *pSecondary;

public:
    // ctor(s)
    MyFrame(const wxString& title);
	~MyFrame();

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

	void OnShortcut1(wxCommandEvent &);
	void OnShortcut2(wxCommandEvent &);
	void OnShortcut3(wxCommandEvent &);
	void OnKeybindings(wxCommandEvent &);

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

class MyDialog : public wxDialog
{
public:
	wxKeyConfigPanel *m_p;

public:
    // ctor(s)
    MyDialog(wxWindow *parent, const wxString& title);
	~MyDialog();

    // event handlers (these functions should _not_ be virtual)
	void OnApply( wxCommandEvent &ev );

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

	// we don't even need to intercept these menu events, because wxFrame
	// will take care of it... (but we can disable this feature using the
	// wxFrame::EnableContextMenuHandling(FALSE) function)
    EVT_MENU(Minimal_Keybindings,  MyFrame::OnKeybindings)
    EVT_MENU(Minimal_Shortcut1, MyFrame::OnShortcut1)
    EVT_MENU(Minimal_Shortcut2, MyFrame::OnShortcut2)
    EVT_MENU(Minimal_Shortcut3, MyFrame::OnShortcut3)

END_EVENT_TABLE()


BEGIN_EVENT_TABLE(MyDialog, wxDialog)
	EVT_BUTTON(wxID_APPLY, MyDialog::OnApply)
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

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame(_T("Minimal wxWindows App"));


	// create an useful log window
	wxLogWindow *pwindow = new wxLogWindow(frame, "log");
	pwindow->GetFrame()->Move(50, 50+350);
	pwindow->GetFrame()->SetSize(800, 300);

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

wxKeyProfile *MyFrame::pPrimary = NULL;
wxKeyProfile *MyFrame::pSecondary = NULL;


// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxPoint(50, 50), wxSize(500, 300))
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

#if 1
	// create a child panel
	wxPanel *panel = new wxPanel(this);
	wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
	box->Add(new wxStaticText(panel, -1, 
			"This is the keybinder sample program.\n\n"
			"This little sample supports two key profiles that "
			"you can edit through the 'File|KeyBindings' command.\n"
			"In particular, they differ for the shortcuts to the 'Shortcut #1'.\n"
			"Enjoy this sample !"), 0, wxALL, 5);
	panel->SetSizer(box);
#endif

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *menuFile = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));

	wxString str = "A dummy shortcut description; "
					"add/delete/edit all the shortcuts you want to this test command.";
	menuFile->Append(Minimal_Shortcut1, _T("Shortcut #1\tCtrl+1"), _T(str));
	menuFile->Append(Minimal_Shortcut2, _T("Shortcut #2\tCtrl+2"), _T(str));
	menuFile->Append(Minimal_Shortcut3, _T("Shortcut #3\tCtrl+3"), _T(str));
	menuFile->AppendSeparator();
	menuFile->Append(Minimal_Keybindings, _T("Keybindings\tF8"), _T(""));
	menuFile->AppendCheckItem(Minimal_ShowKeyProfiles, _T("Show profiles"), _T(""));
	menuFile->Check(Minimal_ShowKeyProfiles, TRUE);
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


	// init the keybinder
#if wxUSE_KEYBINDER
	pPrimary = new wxKeyProfile("Primary", "Our primary keyprofile");
	pSecondary = new wxKeyProfile("Secondary", "Our secondary keyprofile");
	
	pPrimary->ImportMenuBarCmd(menuBar);
	pSecondary->DeepCopy(pPrimary);

	// just to show some features....
	pPrimary->AddShortcut(Minimal_Shortcut1, wxKeyBind("CTRL+SHIFT+E"));

	// and to make some differences between the two key profiles
	pSecondary->GetCmd(Minimal_Shortcut1)->GetShortcut(0)->Set(wxKeyBind("ALT+F10"));

	// by now, attach to this window the primary keybinder
	pPrimary->Attach(this);	

#endif
}


MyFrame::~MyFrame()
{
#if wxUSE_KEYBINDER
	wxSAFE_DELETE(pPrimary);
	wxSAFE_DELETE(pSecondary);
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

void MyFrame::OnShortcut1(wxCommandEvent &)
{ wxMessageBox("Hi !!\nThis is shortcut #1.", "Dummy msg", wxOK, this); }

void MyFrame::OnShortcut2(wxCommandEvent &)
{ wxMessageBox("Hi !!\nThis is shortcut #2.", "Dummy msg", wxOK, this); }

void MyFrame::OnShortcut3(wxCommandEvent &)
{ wxMessageBox("Hi !!\nThis is shortcut #3.", "Dummy msg", wxOK, this); }

void MyFrame::OnKeybindings(wxCommandEvent &)
{
	MyDialog dlg(this, "Keybindings");

	// does the user wants to enable key profiles ?
	dlg.m_p->EnableKeyProfiles(GetMenuBar()->IsChecked(Minimal_ShowKeyProfiles));
	dlg.ShowModal();
}





// ----------------------------------------------------------------------------
// keybindings dialog: a super-simple wrapper for wxKeyConfigPanel
// ----------------------------------------------------------------------------

MyDialog::MyDialog(wxWindow *parent, const wxString &title) :
	wxDialog(parent, -1, title, wxDefaultPosition, wxDefaultSize, 
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)	
{
	// we can do our task in two ways:
	// 1) we can use wxKeyConfigPanel::ImportMenuBarCmd which gives
	//    better appearances (for me, at least, :-))
	// 2) we can use wxKeyConfigPanel::ImportKeyBinderCmd

	// Note also that if we create the wxKeyConfigPanel without the 
	// binder (using NULL), setting it later with
	//           m_p->SetBinder(MyFrame::pPrimary, TRUE);
	// we would use implicitly the wxKeyConfigPanel::ImportKeyBinderCmd	


	// create a simple wxKeyConfigPanel
	m_p = new wxKeyConfigPanel(this, MyFrame::pPrimary, TRUE);
	m_p->AddProfile(MyFrame::pSecondary);

	// with this command we populate the wxTreeCtrl widget of the panel
	m_p->ImportMenuBarCmd(((wxFrame*)parent)->GetMenuBar());

	// and embed it in a little sizer
	wxBoxSizer *main = new wxBoxSizer(wxVERTICAL);
	main->Add(m_p, 1, wxGROW);
	SetSizer(main);
	main->SetSizeHints(this);

	// this is a little modification to make dlg look nicer
	wxSize sz(GetSizer()->GetMinSize());
	SetSize(-1, -1, sz.GetWidth()*1.5, sz.GetHeight());
}

MyDialog::~MyDialog()
{
}

void MyDialog::OnApply( wxCommandEvent & )
{
	// wxKeyConfigPanel keeps user modifications in a temporary buffer:
	// now copy that buffer to our keybinder
	m_p->ApplyChanges();

	// and attach the right keybinder to our parent
	MyFrame::pPrimary->Detach(GetParent());
	MyFrame::pSecondary->Detach(GetParent());
	m_p->GetSelProfile()->Attach(GetParent());

	// if we don't catch this buttonpress, wxKeyConfigPanel would just
	// apply changes without exiting...
	EndModal( wxID_OK );		// exit this dialog
}



