/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/palettefrm.h
// Purpose:     wxPaletteFrame, wxMainFrame, wxPaletteButton, wxMiniButton
//              wxCloseBox, wxMaximizeBox, wxMinimizeBox, wxCollapseBox
// Author:      Francesco Montorsi
// Created:     2004/03/03
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#ifndef _WX_PALETTEFRM_
#define _WX_PALETTEFRM_


// this is required under Win32 because wxPaletteFrame and wxMiniButton,
// under Win32, use some Win32 APIs to draw faster and with the Win32
// look & feel their captions & borders...
// 
// VERY IMPORTANT: DO NOT INCLUDE windows.h HERE !!!
//                 It would lead to some strange, difficult to detect (!!!)
//                 bugs, at least with MSVC++ 6.0.... instead we will include
//                 the wx/defs.h file that defines the WXHWND type.
#include <wx/defs.h>


// these will be defined later
class wxMainFrame;


#ifndef wxPALETTEFRM_USE_MAINFRAME
	#define wxMainFrame			wxFrame
#endif

#ifndef wxPALETTEFRM_USE_PALETTEFRM
	#define wxPaletteFrame		wxMiniFrame
#endif



#ifdef wxPALETTEFRM_USE_PALETTEFRM

class WXDLLIMPEXP_PALETTEFRM wxPaletteFrame : public wxPaletteFrameBase
{
protected:		// internal utilities

	// Draws a frame in the given DC for the given window in the given
	// rect with the given activation state and eventually with an icon.
	virtual void DrawCaption(wxPaletteFrameBase *, wxDC &, const wxRect &rc,
		bool drawicon = TRUE, bool drawactive = TRUE);

public:

	// Default constructor.
	wxPaletteFrame() {}
	wxPaletteFrame(
		wxMainFrame* parent,
		wxWindowID id,
		const wxString &title = wxT("Palette"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxPALETTEFRM_DEFAULT_STYLE,
		const wxString& name = wxT("wxPaletteFrame"));

	bool Create(wxMainFrame *parent,
			wxWindowID id,
			const wxString& title,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxPALETTEFRM_DEFAULT_STYLE,
			const wxString& name = wxT("wxPaletteFrame"));

	// Destructor.
	virtual ~wxPaletteFrame() {}



	// General functions
	// ----------------------

	// Returns the height (in pixels) of the caption bar of this frame;
	// that is the distance from the top pixels of the caption (not the
	// border) and the first pixel which is placed in the client area.
	int GetTitleHeight() const;

	// Returns the width of the borders of this frame. This value is NOT
	// considered in the #GetTitleHeight() function.
	int GetBorderSize() const;



	// Static utilities
	// ----------------------

	// Returns TRUE if the given handle belongs to a wxPaletteFrame.
	static bool IsPaletteFrame(WXHWND h);




	// wxWindow overrides
	// -------------------

	// Sends a fake resize messages to this window. Works on Win32 only.
	virtual void SendSizeEvent();

	// needs to use a win32 api
	void OnActivate( wxActivateEvent &event );

	// Handles the WM_ACTIVATE message
	void MSWOnActivate(bool bGoingToBeActive, WXHWND hwnd);

	// Inits a wxMouseEvent object generated from a NC window message...
	void MSWInitNCMouseEvent(wxMouseEvent &, WXLPARAM lParam, bool transform = TRUE);

	// This function is overloaded to add the WS_POPUP style which cannot
	// be specified through wxWidgets styles (neither using wxPOPUP_WINDOW);
	// so, DON'T TOUCH THIS FUNCTION !!!!
    virtual WXDWORD MSWGetStyle(long flags, WXDWORD *exstyle = NULL) const;

	// This function is overridden to intercept some msgs...
    virtual long MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

	// Returns the Win32 hit test code converting it from the one returned
	// by wxPaletteFrameBase::HitTest which is a wxPaletteFrameHitCode value.
	virtual long MSWGetHitTest(WXLPARAM lParam);



	// Roll/Unroll feature
	// ----------------------------

	void Roll();


private:
	DECLARE_DYNAMIC_CLASS( wxPaletteFrame )
};

#endif		// wxPALETTEFRM_USE_PALETTEFRM



#ifdef wxPALETTEFRM_USE_MAINFRAME

class WXDLLIMPEXP_PALETTEFRM wxMainFrame : public wxMainFrameBase
{
public:

    // ctor(s)
    wxMainFrame() {}
    wxMainFrame(
		wxWindow* parent,
		wxWindowID id,
		const wxString &title = wxT("wxMainFrame"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_MAINFRAME_STYLE,
		const wxString& name = wxT("wxMainFrame"))
		: wxMainFrameBase(parent, id, title, pos, size, style, name) {}
	virtual ~wxMainFrame() {}


	void OnActivate(wxActivateEvent &event);

	// called on WM_NCACTIVATE
	bool MSWOnNcActivate(bool bGoingToBeActive, WXHWND hwnd);

	// handles the WM_ACTIVATE message
	bool MSWOnActivate(bool bGoingToBeActive, WXHWND hwnd);

	// intercept WM_NCACTIVATE & WM_ACTIVATE messages...
	long MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

private:
	DECLARE_DYNAMIC_CLASS( wxMainFrame )
};

#endif		// wxPALETTEFRM_USE_MAINFRAME




#ifdef wxPALETTEFRM_USE_MINIBTN

class WXDLLIMPEXP_PALETTEFRM wxMiniButton : public wxMiniButtonBase
{
public:

	wxMiniButton() {}
	wxMiniButton(wxPaletteFrameBase *parent, int bSmall = -1)
		: wxMiniButtonBase(parent, bSmall) {}
	virtual ~wxMiniButton() {}


	// these ones uses the Win32 APIs
	void DrawMSWStdButton(wxDC &dc, int x, int y, unsigned int mainstyle);
	void DrawButtonFrame(wxDC &dc, int x, int y);	
	int GetBorderSize() const;

private:
	DECLARE_ABSTRACT_CLASS( wxMiniButton )
};

#endif		// wxPALETTEFRM_USE_MINIBTN

#endif		// _WX_PALETTEFRM_
