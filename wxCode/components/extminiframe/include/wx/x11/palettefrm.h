/////////////////////////////////////////////////////////////////////////////
// Name:        wx/x11/palettefrm.h
// Purpose:     wxExtMiniFrame
// Author:      Francesco Montorsi
// Created:     2004/03/03
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#ifndef _WX_PALETTEFRM_H_
#define _WX_PALETTEFRM_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "palettefrm.h"
#endif


// X11 stuff
// ------------------

// contains the X11 API declarations
#include "X11/Xutil.h"

// miscellaneous atoms required
static Atom	x11_wm_protocols		= 0;	// window manager protocols
static Atom	x11_wm_delete_window	= 0;	// delete window protocol
static Atom	x11_wm_take_focus		= 0;	// take focus window protocol
static Atom	x11_net_wm_ping			= 0;	// _NET_WM_PING protocol

static Atom	x11_wm_state			= 0;
static Atom	x11_wm_change_state		= 0;
static Atom	x11_wm_client_leader	= 0;
static Atom	x11_sm_client_id		= 0;
static Atom	x11_xa_motif_wm_hints	= 0;
static Atom	x11_window_role			= 0;

// NET WM support
static Atom	x11_net_wm_state				= 0;
static Atom x11_net_wm_window_type   		= 0;
static Atom x11_net_wm_window_type_normal	= 0;
static Atom x11_net_wm_window_type_dialog	= 0;
static Atom x11_net_wm_window_type_toolbar	= 0;
static Atom	x11_net_wm_window_type_menu		= 0;
static Atom	x11_net_wm_window_type_utility	= 0;
static Atom x11_net_wm_window_type_splash   = 0;
static Atom x11_net_wm_window_type_override	= 0;	// KDE extension

// client leader window
static Window x11_client_leader = 0;

// Inits all the STATIC ATOMs declared above
void wxInitX11Atoms(Display *d);	

// Inits the x11_wm_client_leader window
void wxCreateWMClientLeader(Display *, int);




// The main frame which owns wxExtMiniFrames.
// Applications using wxExtMiniFrame should derive their own
// frames from this class instead of wxFrame.
//
// A extminiframe window
class WXDLLEXPORT wxExtMiniFrame : public wxExtMiniFrameBase
{
protected:		// internal utilities

	// TRUE if the user is moving the window
	bool m_bDragging;
	
	// if m_bDragging is TRUE, this is the point, in window's coord., where the user
	// clicked on the title to drag the window.
	wxPoint m_ptDragAnchor;
	
	// the most important function: this one does all the hard work
	// setting special X11 atoms and properties which allow this
	// window to draw the decorations itself (usually they are
	// drawn by the Window Manager).
	// NOTE: this cannot be a global/static function nor it can take
	//       the window to set as toolbar as argument because it
	//       needs to access some protected variables stored in
	//       wxTopLevelWindowX11
	void SetAsToolbar(Display *d, Window wnd, int screen);

public:

	// Default constructor.
	wxExtMiniFrame() {}
	wxExtMiniFrame(
		wxMainFrameBase *parent,
		wxWindowID id,
		const wxString &title = wxT("wxExtMiniFrame"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxEXTMINIFRM_DEFAULT_STYLE,
		const wxString& name = wxT("wxExtMiniFrame"));

	bool Create(wxMainFrameBase *parent,
			wxWindowID id,
			const wxString& title,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxEXTMINIFRM_DEFAULT_STYLE,
			const wxString& name = wxT("wxExtMiniFrame"));

	
	// these three functions are required to implement ex-novo
	// the drag system to allow the user to move the paletteframe
	virtual bool OnPosLeftDown(const wxPoint &pos);
	virtual bool OnPosLeftUp(const wxPoint &pos);
	virtual void OnMotion(wxMouseEvent &event);	
	
	// this function requires some 'hacking'
	virtual void Roll();
	
private:
	DECLARE_DYNAMIC_CLASS( wxExtMiniFrame )
};


#endif		// _WX_PALETTEFRM_BASE_H_
