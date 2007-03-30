/////////////////////////////////////////////////////////////////////////////
/// @File        wxRubberBand.cpp
/// @Brief     wxRubberBand Minimal wxWidgets sample
/// @Author      Lucien Schreiber (Based on Julian Smart Minimal Sample)
// Modified by:
// Created:     25/10/2006
// RCS-ID:      
// Copyright:   (c) Lucien Schreiber CREALP
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
/*! \mainpage wxRubberSample 
 * 
 \author Lucien Schreiber
 * 
 \date  (c) CREALP 2006
 * 
 *
 * \section intro_sec Introduction
 *
 * This program is a small sample based on th wxWidgets Minimal sample for describing the wxRubberBand Class.
 The wxRubberBand Classs allow interactive selection of an image part.User will found wxRubberBand class documentation in this 
 document.
 *
 *
 \image html capture_linux_sml.jpg "Figure 1 : Show the interface of wxRubberBand under Linux (Ubuntu 6.10, wxGTK 2.7.1)"

\section reference_sec Reference 

Some other usefull links could be found there :

wxwidgets, a GUI LGPL library (http://www.wxwidgets.org)

wxCode, a code and component depot for wxWidgets (http://www.wxcode.org)
 
 */

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/dcbuffer.h>
#include <wx/rubberband/wxRubberBand.h>
// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "../art/sample.xpm"
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
    MyFrame(const wxString& title);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& WXUNUSED(event));
    void OnAbout(wxCommandEvent& WXUNUSED(event));
	void OnPaint (wxPaintEvent & event);
	void OnSizeChange (wxSizeEvent & WXUNUSED(event));
	void OnMoveMouse (wxMouseEvent & event);
	void OnLeftDownMouse (wxMouseEvent & event);

	// rubber menu 
	void OnRubberMenuSize(wxCommandEvent & event);
	void OnRubberMenuStyle (wxCommandEvent & event);

private:
	// Draw all the rectangles
	void DrawAllRectangles(const wxSize & size, wxDC & dc);
	/** \brief store the RubberBand */
	wxRubberBand m_RubberBand;
	/** \brief data associed with the RubberBand (store x and y origin)*/
	wxPoint m_rubberbandorigin;
	/** \brief contains the pen for the rubber band */
	wxPen m_rubberbandPen;
	
    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
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
    // Rubber band menu items
    RubberBandPenSolid = wxID_HIGHEST+1,
    RubberBandPenDotted = wxID_HIGHEST+2, 
    RubberBandSizeSmall = wxID_HIGHEST+3,
    RubberBandsizeBig	= wxID_HIGHEST+4
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
    EVT_PAINT (MyFrame::OnPaint)
    EVT_SIZE (MyFrame::OnSizeChange)
    EVT_MOTION (MyFrame::OnMoveMouse)
    EVT_LEFT_DOWN (MyFrame::OnLeftDownMouse)
    EVT_MENU (RubberBandPenDotted,MyFrame::OnRubberMenuStyle)
    EVT_MENU (RubberBandPenSolid,MyFrame::OnRubberMenuStyle)
    EVT_MENU (RubberBandSizeSmall,MyFrame::OnRubberMenuSize)
    EVT_MENU (RubberBandsizeBig,MyFrame::OnRubberMenuSize) 
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
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

/** \brief 'Main program' equivalent: the program execution "starts" here */
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame(_T("wxRubberBand wxWidgets App"));

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
       : wxFrame(NULL, wxID_ANY, title)
{
	

	// set the frame icon
    SetIcon(wxICON(sample));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));

    fileMenu->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

	// the wxRubberBand Menu
	wxMenu *RubberBandMenu = new wxMenu;
	RubberBandMenu->Append (RubberBandPenSolid, _T("&Solid Rubber Band"),
							_T("Set a solid RubberBand"), wxITEM_CHECK);
	RubberBandMenu->Append (RubberBandPenDotted, _T("&Dotted Rubber Band"),
							_T("Set a dotted RubberBand"), wxITEM_CHECK);
	RubberBandMenu->Check (RubberBandPenDotted, TRUE);
	RubberBandMenu->AppendSeparator();
	RubberBandMenu->Append (RubberBandSizeSmall, _T("S&mall Rubber Band"),
							_T("Set a Small RubberBand"),wxITEM_CHECK);
	RubberBandMenu->Check (RubberBandSizeSmall,TRUE);
	RubberBandMenu->Append (RubberBandsizeBig, _T("&Big Rubber Band"),
							_T("Set a big Rubber Band"),wxITEM_CHECK);

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, _T("&File"));
	menuBar->Append(RubberBandMenu,_T("&Rubber Band"));
	menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(_T("Welcome to wxRubberBand Sample"));
#endif // wxUSE_STATUSBAR

	// Create a Selection Box (Rubber Band)
	m_RubberBand = wxRubberBand(this);

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
    msg.Printf( _T("This is the About dialog of the wxRubberBand sample.\n")
                _T("Welcome to %s"), wxVERSION_STRING);

    wxMessageBox(msg, _T("About wxRubberSample"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnPaint(wxPaintEvent & event)
{
 	wxPaintDC dc(this);
    PrepareDC(dc);

	int xSize = 0, ySize = 0;
	dc.GetSize (&xSize,&ySize);

	// Create a buffer DC
	wxBufferedDC temp_dc(&dc,wxSize(xSize,ySize));

	// Draw the background rectangles
	DrawAllRectangles(wxSize(xSize,ySize), temp_dc);

	
}

/** this function draws all the nice rectangles*/
void MyFrame::DrawAllRectangles(const wxSize & size, wxDC & dc)
{
	wxBrush  myBrushes[] = {*wxBLUE_BRUSH,
							*wxGREEN_BRUSH,
							*wxWHITE_BRUSH,
							*wxBLACK_BRUSH,
							*wxMEDIUM_GREY_BRUSH,
							*wxLIGHT_GREY_BRUSH,
							*wxCYAN_BRUSH,
							*wxRED_BRUSH};

	int RectWidth = (int) size.x / 8;
	int RectHeight = (int) size.y / 2;

	for (int i=0;i<9;i++)
		{
		dc.SetBrush(wxBrush(myBrushes[i]));
		dc.DrawRectangle (i*RectWidth,0,RectWidth,RectHeight);
		dc.SetBrush(wxBrush(myBrushes[8-i]));
		dc.DrawRectangle (i*RectWidth,RectHeight,RectWidth,RectHeight);
		}
	
}

void MyFrame::OnSizeChange(wxSizeEvent & WXUNUSED(event))
{
	Refresh();
}

void MyFrame::OnMoveMouse(wxMouseEvent & event)
{
	wxPoint myActualPosition = event.GetPosition();
	if (event.LeftIsDown())
		{
		m_RubberBand.SetGeometry(	m_rubberbandorigin.x, m_rubberbandorigin.y,
									myActualPosition.x - m_rubberbandorigin.x,
									myActualPosition.y - m_rubberbandorigin.y);
		}
}

void MyFrame::OnLeftDownMouse(wxMouseEvent & event)
{
	m_rubberbandorigin= event.GetPosition();
}

void MyFrame::OnRubberMenuStyle(wxCommandEvent & event)
{
	// check the good menu
	// be sure the menu is checked;
	GetMenuBar()->Check(RubberBandPenDotted,FALSE);
	GetMenuBar()->Check(RubberBandPenSolid,FALSE);
	GetMenuBar()->Check(event.GetId(),TRUE);
	
	// choose a pen
	switch (event.GetId())
	{
	case RubberBandPenDotted:
		m_rubberbandPen.SetStyle(wxDOT);
		break;
	case RubberBandPenSolid:
		m_rubberbandPen.SetStyle(wxSOLID);
		break;

	}
	// Set the pen
	m_RubberBand.SetPen (m_rubberbandPen);
	// Clear the old rubber 
	m_RubberBand.ClearOldRubberRect();
	Refresh();
}

void MyFrame::OnRubberMenuSize(wxCommandEvent & event)
{
	// check the good menu
	// be sure the menu is checked;
	GetMenuBar()->Check(RubberBandSizeSmall,FALSE);
	GetMenuBar()->Check(RubberBandsizeBig,FALSE);
	GetMenuBar()->Check(event.GetId(),TRUE);

	// change the rubber menu according to the user choice
	switch (event.GetId())
	{
	case RubberBandSizeSmall:
		m_rubberbandPen.SetWidth(1);
		break;
	case RubberBandsizeBig:
		m_rubberbandPen.SetWidth(3);
		break;
	}
	// Set the pen
	m_RubberBand.SetPen (m_rubberbandPen);
	// Clear the old rubber 
	m_RubberBand.ClearOldRubberRect();
	Refresh();
}

