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


// IMPORTANT NOTE FOR KEYBINDER USERS: 
// This sample application is used not only as a sample to teach the basics of
// keybinder but also as a test application.
// So, you'll find some references to wxBinderApp below; this aspect of wxKeyBinder
// is still under "construction" so please ignore all references to it...
// (also note that wxBinderApp is a "plug-in" and the fact it still does not work
//  does not mean that keybinder doesn't work too... keybinder works perfectly
//  as it is even without binderapp)


// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "wx/keybinder.h"
#include "wx/menuutils.h"
#include "wx/config.h"


// you can compile this application as a minimal app undefining the following line
#define wxUSE_KEYBINDER			1

// this mode does not work completely on wxGTK
//#define wxUSE_BINDERAPP		1

// this activates some samples of incorrect usage of wxKeyBinder
//#define wxINCORRECT_USAGE



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
class MyApp : public 
#ifdef wxUSE_BINDERAPP
	wxBinderApp
#else
	wxApp
#endif
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();

	//int FilterEvent(wxEvent &);
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
	Minimal_ShowKeyProfiles,
	Minimal_UseTreeCtrl,
	Minimal_EnableProfileEdit,
	Minimal_ShowAddRemoveProfile,

	Minimal_Load,
	Minimal_Save,

	Minimal_Dummy
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
	wxTextCtrl *text;

protected:
	wxADD_KEYBINDER_SUPPORT();

public:

	// the array of key profiles used by this sample app
	wxKeyProfileArray arr;

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

	void OnLoad(wxCommandEvent &);
	void OnSave(wxCommandEvent &);

	void OnDummy(wxCommandEvent &);

	// an utility function
	void UpdateArr(wxKeyProfileArray &r);

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
    MyDialog(wxKeyProfileArray &arr, wxWindow *parent, const wxString& title, int);
	~MyDialog();

	void OnApply( wxCommandEvent &ev );

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

class MyTextCtrl : public wxTextCtrl
{
public:
    // ctor(s)
    MyTextCtrl(wxWindow *parent, wxWindowID id, const wxString& value,
		const wxPoint &pos, const wxSize &size, long style)
		: wxTextCtrl(parent, id, value, pos, size, style) {}
	~MyTextCtrl() {}

	void OnCharEvent( wxKeyEvent &ev );

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

    EVT_MENU(Minimal_Load, MyFrame::OnLoad)
    EVT_MENU(Minimal_Save, MyFrame::OnSave)
    EVT_MENU(Minimal_Dummy, MyFrame::OnDummy)

END_EVENT_TABLE()


BEGIN_EVENT_TABLE(MyDialog, wxDialog)
	EVT_BUTTON(wxID_APPLY, MyDialog::OnApply)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(MyTextCtrl, wxTextCtrl)
	EVT_CHAR(MyTextCtrl::OnCharEvent)
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

void MyTextCtrl::OnCharEvent(wxKeyEvent &ev)
{
	// just for debugging purpose
	wxLogDebug(wxT("MyTextCtrl::OnCharEvent - received [%d]"), ev.GetKeyCode());
	ev.Skip();
}


// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	SetAppName(wxT("Keybinder sample"));

    // create the main application window
    MyFrame *frame = new MyFrame(wxT("Minimal wxWindows App"));

#ifdef __WXDEBUG__
	// create an useful log window
	wxLogWindow *pwindow = new wxLogWindow(frame, wxT("log"));
	pwindow->GetFrame()->Move(50, 50+350);
	pwindow->GetFrame()->SetSize(800, 300);
#endif

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

#ifdef wxUSE_BINDERAPP
	SetGlobalHandler(frame);
	SetGlobalBinder(frame->arr.Item(0));
#endif

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

#if 1
	// create a child panel
	wxPanel *panel = new wxPanel(this);
	wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

	// containing a wxStaticText
	box->Add(new wxStaticText(panel, -1, 
			wxT("This is the keybinder sample program.\n\n")
			wxT("This little sample supports two key profiles that ")
			wxT("you can edit through the 'File|KeyBindings' command.\n")
			wxT("In particular, they differ for the shortcuts bind to the 'Shortcut #1' command.\n")
			wxT("Enjoy this sample !")), 0, wxALL, 5);
	
	// and a text control
	text = new MyTextCtrl(panel, -1, 
			wxT("This text control is used to show that a wxKeyBinder requires to be ")
			wxT("attached to all the child windows if you want to be sure to intercept ")
			wxT("all hotkeys..."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	box->Add(text, 1, wxALL | wxGROW, 5);
	panel->SetSizer(box);

#endif

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *menuFile = new wxMenu;
	
    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));
	
	wxString str = wxT("A dummy shortcut description; ")
		wxT("add/delete/edit all the shortcuts you want to this test command.");
	
	wxMenu *sub = new wxMenu;
	sub->Append(Minimal_Shortcut1, _T("Shortcut #1\tCtrl+1"), str);
	sub->Append(Minimal_Shortcut2, _T("Shortcut #2\tCtrl+2"), str);
	sub->Append(Minimal_Shortcut3, _T("Shortcut #3\tCtrl+3"), str);
	
	wxString str2 = wxT("A wxKeyConfigPanel build flag...");
	
	menuFile->Append(-1, wxT("Shortcuts"), sub);
	menuFile->AppendSeparator();
	menuFile->Append(Minimal_Keybindings, _T("Keybindings...\tF8"), _T("Pop up a wxKeyConfigPanel..."));
	menuFile->AppendCheckItem(Minimal_ShowKeyProfiles, _T("Show profiles"), str2);
	menuFile->AppendCheckItem(Minimal_ShowAddRemoveProfile, _T("Show add/remove profile buttons"), str2);
	menuFile->AppendCheckItem(Minimal_UseTreeCtrl, _T("Use a tree ctrl"), str2);
	menuFile->AppendCheckItem(Minimal_EnableProfileEdit, _T("Enable profile editing"), str2);
	menuFile->AppendSeparator();
	
	menuFile->Append(Minimal_Save, _T("Save the keybindings...\tCtrl+S"), _T("Saves the current keybindings using wxConfig"));
	menuFile->Append(Minimal_Load, _T("Load last keybindings...\tCtrl+L"), _T("Loads the current keybindings using wxConfig"));
	menuFile->AppendSeparator();

	menuFile->Check(Minimal_ShowKeyProfiles, TRUE);
	menuFile->Check(Minimal_ShowAddRemoveProfile, TRUE);
	menuFile->Check(Minimal_UseTreeCtrl, FALSE);
	menuFile->Check(Minimal_EnableProfileEdit, TRUE);
	menuFile->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));	

#ifdef wxINCORRECT_USAGE
	menuFile->Append(Minimal_Dummy, wxT("Dummy menu item\tENTER"), wxT(""));
#endif
	
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
	wxKeyProfile *pPrimary, *pSecondary;

	pPrimary = new wxKeyProfile(wxT("Primary"), wxT("Our primary keyprofile"));
	pPrimary->ImportMenuBarCmd(menuBar);

	pSecondary = new wxKeyProfile(*pPrimary);
	pSecondary->SetName(wxT("Secondary"));
	pSecondary->SetDesc(wxT("Our secondary keyprofile"));

	// just to show some features....
	pPrimary->AddShortcut(Minimal_Shortcut1, wxKeyBind(wxT("CTRL+SHIFT+E")));

	// and to make some differences between the two key profiles
	pSecondary->GetCmd(Minimal_Shortcut1)->GetShortcut(0)->Set(wxKeyBind(wxT("ALT+F10")));
	pSecondary->GetCmd(Minimal_Shortcut2)->GetShortcut(0)->Set(wxKeyBind(wxT("ALT+F11")));
	pSecondary->GetCmd(Minimal_Shortcut3)->GetShortcut(0)->Set(wxKeyBind(wxT("ALT+F12")));

	// put both keyprofiles into our array
	arr.Add(pPrimary);
	arr.Add(pSecondary);

	// by now, attach to this window the primary keybinder
	arr.SetSelProfile(0);
	UpdateArr(arr);
#endif
}


MyFrame::~MyFrame()
{
	// wxKeyProfileArray clean up itself when being destroyed...
}


void MyFrame::UpdateArr(wxKeyProfileArray &r)
{
	// detach all
	r.DetachAll();	
	
	// enable & attach to this window only one
	r.GetSelProfile()->Enable(TRUE);

#ifndef wxUSE_BINDERAPP
	// VERY IMPORTANT: we should not use this function when we
	//                 have temporary children... they would
	//                 added to the binder and when they will be
	//                 deleted, the binder will reference invalid memory...
	r.GetSelProfile()->AttachRecursively(this);
	//r.UpdateAllCmd();		// not necessary
#endif
}




// event handlers

void MyFrame::OnDummy(wxCommandEvent &)		// used to test wxBinderApp
{ 
	wxChar c = wxT('a');
	text->AppendText(wxString(c, 1));
	/*wxKeyEvent ev(wxEVT_CHAR);
	ev.m_keyCode = (wxT('j'));
	text->ProcessEvent(ev);*/
}


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
{ wxMessageBox(wxT("Hi !!\nThis is shortcut #1."), wxT("Dummy msg"), wxOK, this); }

void MyFrame::OnShortcut2(wxCommandEvent &)
{ wxMessageBox(wxT("Hi !!\nThis is shortcut #2."), wxT("Dummy msg"), wxOK, this); }

void MyFrame::OnShortcut3(wxCommandEvent &)
{ wxMessageBox(wxT("Hi !!\nThis is shortcut #3."), wxT("Dummy msg"), wxOK, this); }


#if wxUSE_KEYBINDER

void MyFrame::OnKeybindings(wxCommandEvent &)
{
	bool btree = GetMenuBar()->IsChecked(Minimal_UseTreeCtrl);
	bool baddprofile = GetMenuBar()->IsChecked(Minimal_ShowAddRemoveProfile);
	bool bprofiles = GetMenuBar()->IsChecked(Minimal_ShowKeyProfiles);
	bool bprofileedit = GetMenuBar()->IsChecked(Minimal_EnableProfileEdit);

	// setup build flags
	int mode = btree ? wxKEYBINDER_USE_TREECTRL : wxKEYBINDER_USE_LISTBOX;
	if (baddprofile) mode |= wxKEYBINDER_SHOW_ADDREMOVE_PROFILE;
	if (bprofileedit) mode |= wxKEYBINDER_ENABLE_PROFILE_EDITING;

	int exitcode, sel;
	{	// we need to destroy MyDialog *before* the call to UpdateArr:()
		// otherwise the call to wxKeyBinder::AttachRecursively() which
		// is done inside UpdateArr() would attach to the binder all
		// MyDialog subwindows which are children of the main frame.
		// then, when the dialog is destroyed, wxKeyBinder holds
		// invalid pointers which will provoke a crash !!
	
		MyDialog dlg(arr, this, wxT("Keybindings"), mode | wxKEYBINDER_SHOW_APPLYBUTTON);

		// does the user wants to enable key profiles ?
		dlg.m_p->EnableKeyProfiles(bprofiles);

		if ((exitcode=dlg.ShowModal()) == wxID_OK) {

			// update our array (we gave a copy of it to MyDialog)
			MyFrame::arr = dlg.m_p->GetProfiles();
		}
	}
	
	if (exitcode == wxID_OK) {

		// select the right keyprofile
		UpdateArr(MyFrame::arr);
		
		sel = MyFrame::arr.GetSelProfileIdx();
		wxMessageBox(wxString::Format(wxT("Selected the %d-th profile (named '%s')."),
			sel+1, MyFrame::arr.Item(sel)->GetName().c_str()), 
			wxT("Profile selected"));
	}
}

void MyFrame::OnLoad(wxCommandEvent &)
{
	// before loading we must register in wxCmd arrays the various types
	// of commands we want wxCmd::Load to be able to recognize...	
	wxMenuCmd::Register(GetMenuBar());

	// clear our old array
	arr.Cleanup();

	wxConfig *cfg = new wxConfig(wxGetApp().GetAppName());
	if (arr.Load(cfg)) {

		// get the cmd count
		int total = 0;
		for (int i=0; i<arr.GetCount(); i++)
			total += arr.Item(i)->GetCmdCount();

		if (total == 0) {

			wxMessageBox(wxString::Format(
					wxT("No keyprofiles have been found...\n")
					wxT("A default keyprofile will be set.")));
			wxKeyProfile *p = new wxKeyProfile(wxT("Default"));
			p->ImportMenuBarCmd(GetMenuBar());
			arr.Add(p);

		} else {
		
			wxMessageBox(wxString::Format(
					wxT("All the [%d] keyprofiles have been correctly loaded ")
					wxT("(%d commands in total).\n")
					wxT("The %d-th loaded profile ('%s') will be applied."), 
					arr.GetCount(), total, arr.GetSelProfileIdx()+1, 
					arr.GetSelProfile()->GetName().c_str()),
						wxT("Success"));
		}

		// reattach this frame to the loaded keybinder
		UpdateArr(MyFrame::arr);

	} else {

		wxMessageBox(wxT("Something wrong while loading !"), 
			wxT("Error"), wxOK | wxICON_ERROR);
	}

	delete cfg;
}

void MyFrame::OnSave(wxCommandEvent &)
{
	wxConfig *cfg = new wxConfig(wxGetApp().GetAppName());
	if (arr.Save(cfg, wxEmptyString, TRUE)) {

		// get the cmd count
		int total = 0;
		for (int i=0; i<arr.GetCount(); i++)
			total += arr.Item(i)->GetCmdCount();
					
		wxMessageBox(wxString::Format(wxT("All the [%d] keyprofiles ([%d] commands ")
			wxT("in total) have been correctly saved."), arr.GetCount(), total), 
			wxT("Success"));

	} else {

		wxMessageBox(wxT("Something wrong while saving !"), wxT("Error"), 
			wxOK | wxICON_ERROR);
	}

	delete cfg;
}

#else

void MyFrame::OnKeybindings(wxCommandEvent &) {}
void MyFrame::OnLoad(wxCommandEvent &) {}
void MyFrame::OnSave(wxCommandEvent &) {}

#endif




// ----------------------------------------------------------------------------
// keybindings dialog: a super-simple wrapper for wxKeyConfigPanel
// ----------------------------------------------------------------------------

MyDialog::MyDialog(wxKeyProfileArray &prof, 
				   wxWindow *parent, const wxString &title, int mode) :
	wxDialog(parent, -1, title, wxDefaultPosition, wxDefaultSize, 
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)	
{
	// we can do our task in two ways:
	// 1) we can use wxKeyConfigPanel::ImportMenuBarCmd which gives
	//    better appearances (for me, at least, :-))
	// 2) we can use wxKeyConfigPanel::ImportKeyBinderCmd

	// STEP #1: create a simple wxKeyConfigPanel
	m_p = new wxKeyConfigPanel(this, wxID_ANY, wxDefaultPosition, 
                               wxDefaultSize, wxTAB_TRAVERSAL|mode);

	// STEP #2: add a profile array to the wxKeyConfigPanel
	m_p->AddProfiles(prof);

	// STEP #3: populate the wxTreeCtrl widget of the panel
	m_p->ImportMenuBarCmd(((wxFrame*)parent)->GetMenuBar());

	// and embed it in a little sizer
	wxBoxSizer *main = new wxBoxSizer(wxVERTICAL);
	main->Add(m_p, 1, wxGROW);
	SetSizer(main);
	main->SetSizeHints(this);

	// this is a little modification to make dlg look nicer
	wxSize sz(GetSizer()->GetMinSize());
	SetSize(-1, -1, (int)(sz.GetWidth()*1.3), (int)(sz.GetHeight()*1.1));
	CenterOnScreen();
}

MyDialog::~MyDialog()
{
}

void MyDialog::OnApply(wxCommandEvent &)
{ EndModal(wxID_OK); }

