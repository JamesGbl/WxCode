/////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWindows sample
// Author:      Julian Smart
// Modified by: Bruce Phillips 2001/05/30
// Created:     04/01/98
// RCS-ID:      $Id: minimal1.cpp,v 1.1.1.1 2003-12-28 21:05:26 wyo Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// Summary of changes from minimal.cpp
// ============================================================================

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// Changes from minimal.cpp pertaining to the usage of the custom control
// are bracketed in this fashion.
// This demonstrates the "normal" usage for the control.
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Changes from minimal.cpp pertaining to the toggle menu item are
// bracketed in this fashion.
// The menu's purpose is to test getting and setting the control's
// characteristics (size, position, colors, etc) and thus also
// demonstrates the function calls a program would use for those
// purposes.
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// the make file is also changed to compile & link-in the custom control

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef __GNUG__
    #pragma implementation "minimal.cpp"
    #pragma interface "minimal.cpp"
#endif

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

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// include wxWindows files needed for toggle menu item handlers
#include <wx/colordlg.h>
#include <wx/fontdlg.h>		
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// include the custom control's header
#include "toggle.h"
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------
// the application icon
#if defined(__WXGTK__) || defined(__WXMOTIF__)
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

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// event handlers for the toggle menu items
	void OnToggleToggle(wxCommandEvent& event);
	void OnToggleVisible(wxCommandEvent& event);
	void OnToggleEnable(wxCommandEvent& event);
	void OnToggleForeColour(wxCommandEvent& event);
	void OnToggleBackColour(wxCommandEvent& event);
	void OnToggleSetFont(wxCommandEvent& event);
	void OnToggleSetLabel(wxCommandEvent& event);
	void OnToggleGrowSize(wxCommandEvent& event);
	void OnToggleShrinkSize(wxCommandEvent& event);
	void OnToggleMoveUp(wxCommandEvent& event);
	void OnToggleMoveDown(wxCommandEvent& event);
	void OnToggleMoveLeft(wxCommandEvent& event);
	void OnToggleMoveRight(wxCommandEvent& event);
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// an event handler to handle the custom control's event
	void OnToggle(wxCommandEvent& event);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

private:

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// a private variable for the menuBar - needed for toggle menu items
	wxMenuBar *menuBar;
// a private variable for the toggle - needed for toggle menu items
	wxCustomToggleCtrl *m_toggle;
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = 1,
    Minimal_About,

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// id tags for the toggle menu items
	tToggle,
	tVisible,
	tEnable,
	tColour,
	tForeColour,
	tBackColour,
	tFont,
	tLabel,
	tSize,
	tSizePlus,
	tSizeMinus,
	tMove,
	tMoveUp,
	tMoveDown,
	tMoveLeft,
	tMoveRight,
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// an id tag for the custom control
	ToggleId
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

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

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// entries for the toggle menu items
	EVT_MENU(tToggle,    MyFrame::OnToggleToggle)
	EVT_MENU(tVisible,   MyFrame::OnToggleVisible)
	EVT_MENU(tEnable,    MyFrame::OnToggleEnable)
	EVT_MENU(tForeColour,MyFrame::OnToggleForeColour)
	EVT_MENU(tBackColour,MyFrame::OnToggleBackColour)
	EVT_MENU(tFont,      MyFrame::OnToggleSetFont)
	EVT_MENU(tLabel,     MyFrame::OnToggleSetLabel)
	EVT_MENU(tSizePlus,  MyFrame::OnToggleGrowSize)
	EVT_MENU(tSizeMinus, MyFrame::OnToggleShrinkSize)
	EVT_MENU(tMoveUp,    MyFrame::OnToggleMoveUp)
	EVT_MENU(tMoveDown,  MyFrame::OnToggleMoveDown)
	EVT_MENU(tMoveLeft,  MyFrame::OnToggleMoveLeft)
	EVT_MENU(tMoveRight, MyFrame::OnToggleMoveRight)
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// the entry for the custom control's event
	EVT_CUSTOM_TOGGLE(ToggleId,MyFrame::OnToggle)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
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
    MyFrame *frame = new MyFrame("Minimal wxWindows App",
                                 wxPoint(50, 50), wxSize(450, 340));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(TRUE);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
#ifdef __WXMAC__
    // we need this in order to allow the about menu relocation, since ABOUT is
    // not the default id of the about menu
    wxApp::s_macAboutMenuItemId = Minimal_About;
#endif

    // set the frame icon
    SetIcon(wxICON(mondrian));

    // create a menu bar
    wxMenu *menuFile = new wxMenu("", wxMENU_TEAROFF);

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About...\tCtrl-A", "Show about dialog");

    menuFile->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// "menuBar" as a private class variable
    //wxMenuBar *menuBar = new wxMenuBar();
    menuBar = new wxMenuBar();
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    menuBar->Append(menuFile, "&File");
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// add the toggle menu
	wxMenu *colourMenu = new wxMenu;	// colour sub-menu
		colourMenu->Append(tForeColour,"&Foreground","Set the toggle's foreground colour");
		colourMenu->Append(tBackColour,"&Background","Set the toggle's background colour");
	wxMenu *sizeMenu = new wxMenu;	// size sub-menu
		sizeMenu->Append(tSizePlus,"&Increase","Increase the toggle's size");
		sizeMenu->Append(tSizeMinus,"&Decrease","Decrease the toggle's size");
	wxMenu *moveMenu = new wxMenu;	// move sub-menu
		moveMenu->Append(tMoveUp,"&Up","Move the toggle upward");
		moveMenu->Append(tMoveDown,"&Down","Move the toggle downward");
		moveMenu->Append(tMoveLeft,"&Left","Move the toggle leftward");
		moveMenu->Append(tMoveRight,"&Right","Move the toggle rightward");
	wxMenu *toggleMenu = new wxMenu;	// finally, the toggle menu
		toggleMenu->Append(tToggle,"&Toggle","Toggle the toggle");
		toggleMenu->Append(tVisible,"&Visible","Show/hide the toggle",TRUE);
			toggleMenu->Check(tVisible,TRUE);
		toggleMenu->Append(tEnable,"&Enable","Enable/disable the toggle",TRUE);
			toggleMenu->Check(tEnable,TRUE);
		toggleMenu->Append(tColour,"&Colour",colourMenu,"Change the toggle's colours");
		toggleMenu->Append(tFont,"&Font","Change the toggle's font");
		toggleMenu->Append(tLabel,"&Label","Change the toggle's label");
		toggleMenu->Append(tSize,"&Size",sizeMenu,"Change the toggle's size");
		toggleMenu->Append(tMove,"&Move",moveMenu,"Change the toggle's position");
	menuBar->Append(toggleMenu,"&Toggle");
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Welcome to wxWindows!");
#endif // wxUSE_STATUSBAR

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// add a panel and then the custom control
	wxPanel *panel = new wxPanel(this);
	m_toggle = new wxCustomToggleCtrl(panel,ToggleId,"test",wxPoint(20,20));
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
}


// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("This is the about dialog of minimal sample.\n")
                _T("Welcome to %s"), wxVERSION_STRING);

    wxMessageBox(msg, "About Minimal", wxOK | wxICON_INFORMATION, this);
}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// event handlers for the toggle menu items
void MyFrame::OnToggleToggle(wxCommandEvent& event)
{
	m_toggle->SetValue(!(m_toggle->GetValue()));
}
void MyFrame::OnToggleVisible(wxCommandEvent& event)
{
	m_toggle->Show(event.IsChecked());
}
void MyFrame::OnToggleEnable(wxCommandEvent& event)
{
	m_toggle->Enable(event.IsChecked());
#if !defined(__WXGTK__)
	m_toggle->Refresh();
#endif
}
void MyFrame::OnToggleForeColour(wxCommandEvent& event)
{
	wxColour c = wxGetColourFromUser(this,m_toggle->GetForegroundColour());
	if (c.Ok()) {
		m_toggle->SetForegroundColour(c);
#if !defined(__WXGTK__)
		m_toggle->Refresh();
#endif
	}
}
void MyFrame::OnToggleBackColour(wxCommandEvent& event)
{
	wxColour c = wxGetColourFromUser(this,m_toggle->GetBackgroundColour());
	if (c.Ok()) {
		m_toggle->SetBackgroundColour(c);
#if !defined(__WXGTK__)
		m_toggle->Refresh();
#endif
	}
}
void MyFrame::OnToggleSetFont(wxCommandEvent& event)
{
	wxFontData data;
	data.SetInitialFont(m_toggle->GetFont());
	data.SetColour(m_toggle->GetForegroundColour());

	wxFontDialog dialog(this, &data);
	if (dialog.ShowModal() == wxID_OK) {
		wxFontData retData = dialog.GetFontData();
		m_toggle->SetFont(retData.GetChosenFont());
		m_toggle->SetForegroundColour(retData.GetColour());
		m_toggle->Refresh();
	}

}
void MyFrame::OnToggleSetLabel(wxCommandEvent& event)
{
	wxString s = wxGetTextFromUser("Enter text for the toggle's label :",
		"Change Toggle Label",m_toggle->GetLabel(),this);
	if (!(s.IsEmpty())) {
		m_toggle->SetLabel(s);
#if !defined(__WXGTK__)
		m_toggle->Refresh();
#endif
	}
}
void MyFrame::OnToggleGrowSize(wxCommandEvent& event)
{
	int w,h, wp, hp;
	m_toggle->GetSize(&w,&h);
	if (w <= 10) w++; else w += (int)(w * 0.1);
	if (h <= 10) h++; else h += (int)(h * 0.1);
	m_toggle->SetSize(w,h);
#if !defined(__WXGTK__)
	m_toggle->Refresh();
#endif
	
	w += (int)(w * 0.1); h += (int)(h * 0.1);
	(m_toggle->GetParent())->GetSize(&wp,&hp);
	if (wp < w || hp < h) menuBar->Enable(tSizePlus,FALSE);
	
	if (!(menuBar->IsEnabled(tSizeMinus)))
		menuBar->Enable(tSizeMinus,TRUE);
}
void MyFrame::OnToggleShrinkSize(wxCommandEvent& event)
{
	int w,h;
	m_toggle->GetSize(&w,&h);
	w -= (int)(w * 0.1); h -= (int)(h * 0.1);
	m_toggle->SetSize(w,h);
#if !defined(__WXGTK__)
	m_toggle->Refresh();
#endif
	
	// note that minimum possible size is (9,9) when subtracting 10%
	if (w < 10 && h < 10)
		menuBar->Enable(tSizeMinus,FALSE);
	
	if (!(menuBar->IsEnabled(tSizePlus)))
		menuBar->Enable(tSizePlus,TRUE);
}
void MyFrame::OnToggleMoveUp(wxCommandEvent& event)
{
	int x,y,w,h;
	m_toggle->GetSize(&w,&h);
	m_toggle->GetPosition(&x,&y);
	y -= 10;
	m_toggle->SetSize(x,y,w,h);
}
void MyFrame::OnToggleMoveDown(wxCommandEvent& event)
{
	int x,y,w,h;
	m_toggle->GetSize(&w,&h);
	m_toggle->GetPosition(&x,&y);
	y += 10;
	m_toggle->SetSize(x,y,w,h);
}
void MyFrame::OnToggleMoveLeft(wxCommandEvent& event)
{
	int x,y,w,h;
	m_toggle->GetSize(&w,&h);
	m_toggle->GetPosition(&x,&y);
	x -= 10;
	m_toggle->SetSize(x,y,w,h);
}
void MyFrame::OnToggleMoveRight(wxCommandEvent& event)
{
	int x,y,w,h;
	m_toggle->GetSize(&w,&h);
	m_toggle->GetPosition(&x,&y);
	x += 10;
	m_toggle->SetSize(x,y,w,h);
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// an event handler for the custom control
void MyFrame::OnToggle(wxCommandEvent& event)
{
	wxString msg;
	
	if (event.GetInt() == wxCUSTOM_TOGGLE_IS_UP)
		msg = "The toggle is up";
	else
		msg = "The toggle is down";
		
#if wxUSE_STATUSBAR
	SetStatusText(msg);
#else
	wxMessageBox(msg,"Toggle Status", wxOK | wxICON_INFORMATION, this);
#endif
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
