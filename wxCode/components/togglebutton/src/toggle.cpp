/////////////////////////////////////////////////////////////////////////////
// Name:        wxCustomToggleCtrl.cpp
// Purpose:     a toggle button - eg of a wxWindow ver. 2 custom control
// Author:      Bruce Phillips
// Modified by:
// Created:     05/30/2001
// RCS-ID:
// Copyright:   (c) Bruce Phillips
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ==========================================================================
// headers
// ==========================================================================

//------------------------------------------------------------------------
// the standard wxWindow pragmas and includes

#ifdef __GNUG__
    #pragma implementation "toggle.h"
#endif

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

//------------------------------------------------------------------------
// the toggle's header

#include "toggle.h"

// ==========================================================================
// event table (the events wxCustomToggleCtrl ** RECEIVES **)
// ==========================================================================

BEGIN_EVENT_TABLE(wxCustomToggleCtrl,wxControl)
	EVT_PAINT(wxCustomToggleCtrl::OnPaint)
	EVT_LEFT_DOWN(wxCustomToggleCtrl::OnLeftClick)
END_EVENT_TABLE()
	// note that "LEFT_DOWN" isn't really the same as a left click
	// (a LEFT_DOWN followed by a LEFT_UP with the mouse over the
	// control for both events). That said, unless the control must
	// process double click and/or drag events, treating "down" as
	// a "click" is usually sufficient (and much eaisier).

// ==========================================================================
// implemetation
// ==========================================================================

IMPLEMENT_DYNAMIC_CLASS(wxCustomToggleCtrl,wxControl)

//------------------------------------------------------------------------
// public functions
//------------------------------------------------------------------------
wxCustomToggleCtrl::wxCustomToggleCtrl()
	: wxControl()
{
// Default constructor.

	Init();
}

//------------------------------------------------------------------------
wxCustomToggleCtrl::wxCustomToggleCtrl(wxWindow* parent,
		wxWindowID id,
		const wxString& label,
		const wxPoint& pos,
		const wxSize& size,
		long style,
		const wxValidator& val,
		const wxString& name)
	: wxControl()
{
// Constructor, creating and showing a toggle.

	Init();
	Create(parent,id,label,pos,size,style,val,name);
}

//------------------------------------------------------------------------
wxCustomToggleCtrl::~wxCustomToggleCtrl()
{
//Destructor, destroying the toggle. Nothing needed in this case.
}

//------------------------------------------------------------------------
bool wxCustomToggleCtrl::Create(wxWindow* parent,
		wxWindowID id,
		const wxString& label,
		const wxPoint& pos,
		const wxSize& size,
		long style,
		const wxValidator& val,
		const wxString& name)
{
// Creates the toggle for two-step construction.

	style |= wxNO_BORDER;
		// wxMSW will add a simple border otherwise
		// not required for wxGTK

	wxControl::Create(parent,id,pos,size,style,val,name);
		// note that the wxControl (but not the wxWindow) has already
		// been constructed by one of the two constructors above
		// *prior* to this call (otherwise, this fuction
		// (wxControl::Create()) wouldn't exist to be called!).
		
	SetLabel(label);
		// SetLabel() from the wxControl class
		// in wxMSW, wxControl re-uses the wxWindow label variable
		// while wxGTK actually stores a second variable directly
		// in wxControl. As a result, calling this before the
		// wxControl::Create() call in wxMSW has no effect and
		// the label will *NOT* be set until a subsequent
		// SetLabel() call is issued!
	
	if (size == wxDefaultSize) SetSize(DoGetBestSize());
		// SetSize() from the wxWindow class
		// since the DoGetBestSize() routine uses the label
		// to determine the size, need to call this manually
		// after SetLabel() is called.
	
	SetBackgroundColour(parent->GetBackgroundColour());
	SetForegroundColour(parent->GetForegroundColour());
	SetFont(parent->GetFont());
		// in wxMSW the default yields a white background and black
		// foreground while wxGTK follows the parent's settings. Of
		// course, one really shouldn't assume any particular
		// behavior constructing a "custom" control, but rather,
		// should explicitly set things as desired. In this case,
		// these characteristics are set equal to the parent's.
	
	return (TRUE);
}

//------------------------------------------------------------------------
bool wxCustomToggleCtrl::GetValue() const
{
// Gets the state of the toggle : TRUE means depressed.

	return (m_down);
}

//------------------------------------------------------------------------
void wxCustomToggleCtrl::SetValue(const bool state)
{
// Sets the toggle to the given state : TRUE means depressed.

	m_down = state;
	Refresh();		
		// Refresh() from the wxWindow class
}
	
//------------------------------------------------------------------------
void wxCustomToggleCtrl::OnLeftClick(wxMouseEvent& WXUNUSED(event))
{
// Changes the state of the toggle and repaints the window.
// Then, sends out the wxEVT_COMMAND_CUSTOM_TOGGLED event.

	m_down = !m_down;
	Refresh();		
		// Refresh() from the wxWindow class

	wxCommandEvent eventOut(wxEVT_COMMAND_CUSTOM_TOGGLED,GetId());
		// GetId() from the wxWindow class
	
	if (m_down)	eventOut.SetInt(wxCUSTOM_TOGGLE_IS_DOWN);
	else			eventOut.SetInt(wxCUSTOM_TOGGLE_IS_UP);
		// For more complex event handling, you can derive a class from
		// wxCommandEvent and provide the additional functionality needed.
		// As is, wxCommandEvent provides set & get functions for int,
		// wxString, and void * types (as well as some internal types
		// used in the stock wxWindows controls (wxCheckBox, etc)).
	
	ProcessEvent(eventOut);			
		// ProcessEvent() from the wxEvtHandler class
		// this call will actually send out the event
}

//------------------------------------------------------------------------	
void wxCustomToggleCtrl::OnPaint(wxPaintEvent& WXUNUSED(event))
{
// Actually draws the toggle - a "minimal" 3D button in the raised
// or sunken state. The text (label) is "greyed" if the control is
// disabled.

	wxPaintDC dc((wxWindow *) this);
	int w, h;	GetSize(&w,&h);
		// GetSize() from the wxWindow class
	wxBrush brush(GetBackgroundColour(),wxSOLID);
		// GetBackgroundColour() from the wxWindow class
	
// set the DC's brushes, font, and text background colour
	dc.SetBackground(brush);		// brush for dc.Clear()
	dc.SetBrush(brush);
	dc.SetFont(GetFont());
		// GetFont() from the wxWindow class
	dc.SetTextBackground(GetBackgroundColour());
		// GetBackgroundColour() from the wxWindow class
	
// calculate the position for the text (label) = centered on the toggle
	int x, y;
	dc.GetTextExtent(GetLabel(),&x,&y); 	
		// GetLabel() from the wxControl class
	x = (w - x) / 2;
	y = (h - y) / 2;
		// yes, x and/or y can be negative and then the text will
		// get clipped at both ends by the DC
	
// start the actual drawing
	dc.BeginDrawing();
	
// erase the window area
	dc.Clear(); 				// clear the window to brush colour
	
// draw the border
	if (m_down) {			// draw sunken border
		dc.SetPen(*wxGREY_PEN);
			dc.DrawLine(0,h-1,0,0);		dc.DrawLine(0,0,w,0);
		dc.SetPen(*wxWHITE_PEN);
			dc.DrawLine(w-1,1,w-1,h-1);	dc.DrawLine(w-1,h-1,0,h-1);
		dc.SetPen(*wxBLACK_PEN);
			dc.DrawLine(1,h-2,1,1);		dc.DrawLine(1,1,w-1,1);
	} else {				// draw raised border
		dc.SetPen(*wxWHITE_PEN);
			dc.DrawLine(0,h-2,0,0); 		dc.DrawLine(0,0,w-1,0);
		dc.SetPen(*wxBLACK_PEN);
			dc.DrawLine(w-1,0,w-1,h-1);	dc.DrawLine(w-1,h-1,-1,h-1);
		dc.SetPen(*wxGREY_PEN);
			dc.DrawLine(2,h-2,w-2,h-2);	dc.DrawLine(w-2,h-2,w-2,1);
	}
	
// draw the text (label)
	if (IsEnabled())	// IsEnabled() from the wxWindow class
		dc.SetTextForeground(GetForegroundColour());
			// GetForegroundColour() from the wxWindow class	
	else	
		dc.SetTextForeground(wxGREY_PEN->GetColour());
		
	dc.DrawText(GetLabel(),x,y);
		// GetLabel() from the wxControl class
	
// end of drawing
	dc.EndDrawing();
}

//------------------------------------------------------------------------
// protected functions
//------------------------------------------------------------------------
wxSize wxCustomToggleCtrl::DoGetBestSize() const
{
// overrided wxControl::DoGetBestSize()
// calculates the "minimum" window size depending on the toggle's label

	int w,h;
	wxClientDC dc((wxWindow *) this);
	
	dc.SetFont(GetFont());
		// GetFont() from wxWindow class
	dc.GetTextExtent(GetLabel(),&w,&h);
		// GetLabel() from the wxControl class
	w += 16; h += 8;
		// some padding around the text + 4 for the button borders	
	
	return (wxSize(w,h));	
}

//------------------------------------------------------------------------
// private functions
//------------------------------------------------------------------------
void wxCustomToggleCtrl::Init()
{
// "standard" Init() function

	m_down = FALSE;	// toggle initially in the up position
}
