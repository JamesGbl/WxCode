/////////////////////////////////////////////////////////////////////////////
// Name:        wxCustomToggleCtrl.h
// Purpose:     a toggle button - eg of a wxWindow ver. 2 custom control
// Author:      Bruce Phillips
// Modified by:
// Created:     05/30/2001
// RCS-ID:
// Copyright:   (c) Bruce Phillips
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef WXCUSTOMTOGGLECTRL_H
#define WXCUSTOMTOGGLECTRL_H

#ifdef __GNUG__
    #pragma interface
#endif

// just in case it is not #included already
#include <wx/wx.h>

class wxCustomToggleCtrl : public wxControl
{
public:
// Default constructor.
	wxCustomToggleCtrl();
// Constructor, creating and showing a toggle.
	wxCustomToggleCtrl(wxWindow* parent,
		wxWindowID id,
		const wxString& label,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxValidator& val = wxDefaultValidator,
		const wxString& name = "customToggle");
//Destructor, destroying the toggle.
	~wxCustomToggleCtrl();
		
// Creates the toggle for two-step construction.
	bool Create(wxWindow* parent,
		wxWindowID id,
		const wxString& label,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxValidator& val = wxDefaultValidator,
		const wxString& name = "customToggle");
		
// Gets the state of the toggle : TRUE means depressed.	
	bool GetValue() const;
// Sets the toggle to the given state : TRUE means depressed.
	void SetValue(const bool state);

// overrided standard events handled differently for this control
	virtual void OnPaint(wxPaintEvent &event);
	virtual void OnLeftClick(wxMouseEvent &event);

protected:
// overrided version for wxControl:DoGetBestSize()
	virtual wxSize DoGetBestSize() const;
	
private:
	bool m_down;		// keeps track of toggle state
	
	void Init();		// "standard" Init() function
	
	DECLARE_DYNAMIC_CLASS(wxCustomToggleCtrl)
	DECLARE_EVENT_TABLE()
};

// the single event tag
#define wxEVT_COMMAND_CUSTOM_TOGGLED (wxID_HIGHEST + 1)

// the event table entry macro
#define EVT_CUSTOM_TOGGLE(id,func) \
	EVT_CUSTOM(wxEVT_COMMAND_CUSTOM_TOGGLED,id,func)

// constants for event handler functions to test against event.GetInt()
#define wxCUSTOM_TOGGLE_IS_UP 	0
#define wxCUSTOM_TOGGLE_IS_DOWN 	1

#endif	// #ifndef WXCUSTOMTOGGLECTRL_H
