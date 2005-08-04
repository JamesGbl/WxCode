/////////////////////////////////////////////////////////////////////////////
// Name:        wx/palettefrm.h
// Purpose:     wxPaletteFrame, wxMainFrame, wxMiniButton
// Author:      Francesco Montorsi
// Created:     2004/03/03
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////




/*

  wxMainFrame & wxPaletteFrame features
  -------------------------------------

  1) the ability to be activated/deactivated when the main frame
     activation state changes (wxMSW only): that is, when the
	 mainframe is active, then also all the wxPaletteFrame(s) are
	 always drawn as active.

  2) the ability to contain any type of children, and to use
     wxSizers to layout them; this allows the creation of
     wxT("advanced toolbars") (e.g. like MSOffice's ones)

  3) the ability to draw any type of wxMiniButton in its caption 
     area (standard minibuttons are already provided)

  4) the ability to be rolled/unrolled (maybe using wxCollapseBox);
     useful to temporary make smaller big wxPaletteFrame windows

  5) the ability to show a context menu, when right-clicked in the
     caption bar, to allow the user to show/hide a specific palette

  6) the ability to save/load the layout of the wxPaletteFrames
     in the given wxConfig object

  7) the cross-platformness (wxMSW, wxX11 and wxGTK)




  1) DESIGN issues (that is, wxT("How I wasted my time trying various approaches"))
  ----------------------------------------------------------------------------

  Another approach for the implementation of a window such as wxPaletteFrame
  could be to derive a class from wxFrame and then, as I tried, to emulate the
  active state of the mainframe drawing a fake title bar over the real bar.
  This approach has two problems:
    1) under Win32 it generates some flickering, specially for big palettes
	2) under GTK it cannot be implemented because there's no way to draw the
	   mainframe's titlebar as active when it is not (but this problem is
	   present also in the current implementation !!!)

  Another first approach could also be to avoid accepting the focus on the 
  palette window and, even while the user is dragging/resizing the window,
  continuosly transfer focus from the palette to the main frame: like the
  previous solution, in this way flickering would produce a very nasty
  effect... (at least on Win32 where I tried it)



   

  2) FOCUS issues on Win32
  ------------------------

  One of the major features of wxPaletteFrame is its ability to keep 
  always the same activation state of the wxMainFrame associated with  
  an user-intuitive interface.

  I managed to implement this feature on Win32 only for one reason:
  on GTK we miss some important information (the window which is going
  to be activated, for example) for events such as activation & nonclient 
  activation...

  This feature is much more difficult to achieve than it would seems
  because one fix rule of Win32 is that only one window at time can
  be the active window. There's no way to have two active windows
  at the same time... however, wxPaletteFrame uses the same trick used
  by classes like CMiniFrameWnd in the MFC (that is, the classes used
  to create detachable toolbars): when the active window is the parent
  window, the wxPaletteFrame draws itself with the color of an active
  caption even if it's not.

  The focus synchronization system must be able to handle the focus
  changes summed up in this table:

      [0] = another app or a child of [1], which is not [2]
      [1] = the wxMainFrame which owns [2]
      [2] = a wxPaletteFrame owned by [1]
  

   focus   |            what must be done            | the function which
   change  |                                         |       does it
 ----------+-----------------------------------------+-------------------
  [0]->[1] | [1] becomes ACTIVE;                     | wxMainFrame::
           | all wxPaletteFrames must be ACTIVATED   |   OnActivate 
           |                                         |              
  [1]->[0] | [1] becomes INACTIVE;                   | wxMainFrame::
           | all wxPaletteFrames must be DEACTIVATED |   OnActivate 
           |                                         |              
  [1]->[2] | [1] doesn't update its titlebar (that   | wxMainFrame::
           | is, it keeps its ACTIVE color);         |   MSWOnNcActivate
           |                                         |                  
  [2]->[1] | [1] should already be drawn as ACTIVE   | wxPaletteFrame:: 
           | [2] must keep its ACTIVE color          |   OnActivate     
           |                                         |                  
  [2]->[0] | [2] should notify [1] that it must be   | wxPaletteFrame:: 
           | drawn as INACTIVE like all its palettes |   MSWOnActivate  
           |                                         |                  
  [0]->[2] | [2] should notify [1] that it must be   | wxPaletteFrame:: 
           | drawn as ACTIVE like all its palettes   |   MSWOnActivate  

*/



#ifndef _WX_PALETTEFRM_BASE_H_
#define _WX_PALETTEFRM_BASE_H_


// required includes
#include "wx/palettefrmdef.h"
#include <wx/config.h>
#include <wx/frame.h>



// wxPaletteFrame defines
// ----------------------

// this is done to ensure compability with wxWidgets < 2.5.1
#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX						wxSYSTEM_MENU
#endif

#ifndef wxFULL_REPAINT_ON_RESIZE
#define	wxFULL_REPAINT_ON_RESIZE		0 /* expand to nothing */
#endif


// the space left between two minibuttons in a wxPaletteFrame caption
#define wxPALETTEFRM_BTN_GAP			2

// button height is calculated as:   
//        wxPALETTEFRM_BTN_HEIGHT_MULT * GetParent()->GetTitleHeight()
#define wxPALETTEFRM_BTN_HEIGHT_MULT	0.8

// button width is calculated as:
//        wxPALETTEFRM_BTN_RATIO*GetButtonHeight()
//
// NOTE: this value should be near 1.0 because the button's Draw() functions
//       expect the button to be similar to a square
#define wxPALETTEFRM_BTN_RATIO			1.2

// under Win32 this is the DWORD which is attached to each wxPaletteFrame
// with the wxSetWindowUserData() to be able to identify it through the
// wxGetWindowUserData()...
#define wxPALETTEFRM_MARKER				0xF0F0F0F0





// Conditional compilation (used for paletteframe development only)
// -------------------------------------------------------------------

// if this is defined, a lot of wxLogDebug calls will be enabled
#ifdef __WXDEBUG__
//	#define wxPALETTEFRM_FULLDEBUG 			1
#endif

// if not defined wxPaletteFrame will be replaced by wxMiniFrame
#define wxPALETTEFRM_USE_PALETTEFRM

// if not defined, wxMainFrame will be replaced by wxFrame
#define wxPALETTEFRM_USE_MAINFRAME

// if not defined, wxMiniButton won't be used at all
#define wxPALETTEFRM_USE_MINIBTN

// if we want a very fast redraw, then we must exclude the gradient fill
// in the caption bar of the paletteframes...
//#define wxPALETTEFRM_FASTREDRAW




// wxPaletteFrame & wxMainFrame styles
// (the new styles use values which don't conflict with
//  the wxWidgets standard styles)
// -----------------------------------

// the default style for a wxPaletteFrame window
#ifdef __WXMSW__
	#define wxPALETTEFRM_DEFAULT_STYLE		(wxCAPTION | wxRESIZE_BORDER |		\
											wxCLOSE_BOX |						\
											wxFRAME_FLOAT_ON_PARENT |			\
											wxFRAME_TOOL_WINDOW |	 			\
											wxCLIP_CHILDREN | 					\
											wxSYSTEM_MENU)
#else

	// on GTK we must absolutely avoid the wxFRAME_TOOL_WINDOW style flag
	#define wxPALETTEFRM_DEFAULT_STYLE		((wxDEFAULT_FRAME_STYLE & ~wxMAXIMIZE_BOX) & ~wxMINIMIZE_BOX)
#endif

										
// the mask which is used to filter out unsupported window style
// while creating wxPaletteFrame windows
#define wxPALETTEFRM_STYLE_MASK			0xFFFFFFFFFFFFFFFF
/*(wxCAPTION | wxRESIZE_BORDER |		\
										wxFULL_REPAINT_ON_RESIZE |			\
										wxPOPUP_WINDOW |					\
										wxCLOSE_BOX | wxMINIMIZE_BOX |		\
										wxMAXIMIZE_BOX | wxSTAY_ON_TOP |	\
										wxFRAME_FLOAT_ON_PARENT |			\
										wxFRAME_TOOL_WINDOW | wxTINY_CAPTION_HORIZ)*/

// the default style for a wxMainFrame window
#define wxDEFAULT_MAINFRAME_STYLE		(wxMINIMIZE_BOX | wxMAXIMIZE_BOX |	\
										wxRESIZE_BORDER | wxSYSTEM_MENU |	\
										wxCAPTION | wxCLOSE_BOX |  			\
										wxCLIP_CHILDREN)


// The hitcodes returned by wxPaletteFrame::OnHitTest
// frame hit test return values taken from wx/toplevel.h
enum wxPaletteFrameHitCode
{
    wxPFHT_TOPLEVEL_NOWHERE         = 0x00000000,
    wxPFHT_TOPLEVEL_CLIENT_AREA     = 0x00000001,
    wxPFHT_TOPLEVEL_ICON            = 0x00000002,
    wxPFHT_TOPLEVEL_TITLEBAR        = 0x00000004,

    wxPFHT_TOPLEVEL_BORDER_N        = 0x00000010,
    wxPFHT_TOPLEVEL_BORDER_S        = 0x00000020,
    wxPFHT_TOPLEVEL_BORDER_E        = 0x00000040,
    wxPFHT_TOPLEVEL_BORDER_W        = 0x00000080,
    wxPFHT_TOPLEVEL_BORDER_NE       = wxPFHT_TOPLEVEL_BORDER_N | wxPFHT_TOPLEVEL_BORDER_E,
    wxPFHT_TOPLEVEL_BORDER_SE       = wxPFHT_TOPLEVEL_BORDER_S | wxPFHT_TOPLEVEL_BORDER_E,
    wxPFHT_TOPLEVEL_BORDER_NW       = wxPFHT_TOPLEVEL_BORDER_N | wxPFHT_TOPLEVEL_BORDER_W,
    wxPFHT_TOPLEVEL_BORDER_SW       = wxPFHT_TOPLEVEL_BORDER_S | wxPFHT_TOPLEVEL_BORDER_W,
    wxPFHT_TOPLEVEL_ANY_BORDER      = 0x000000F0,
};



// wxPaletteFrame macros
// ---------------------

// a little useful macro
#ifndef wxSAFE_DELETE
#define wxSAFE_DELETE(x)				{ if (x) delete x; x = NULL; }
#endif

// for debugging purpose
#ifdef wxPALETTEFRM_FULLDEBUG
#define wxPALETTE_LOG			wxLogDebug
#else
#ifdef __GNUG__
#define wxPALETTE_LOG(...)		/* expand to nothing */
#else
#define wxPALETTE_LOG			/* expand to nothing */
#endif
#endif

#ifndef wxPALETTEFRM_USE_MAINFRAME
	#define wxMainFrameBase			wxFrame
#endif

#ifndef wxPALETTEFRM_USE_PALETTEFRM
	#define wxPaletteFrameBase		wxMiniFrame
#endif


// this will be defined later...
class wxMainFrameBase;
class wxMiniButtonBase;
class wxWindowDC;





#ifdef wxPALETTEFRM_USE_PALETTEFRM

// The main frame which owns wxPaletteFrames.
// Applications using wxPaletteFrame should derive their own
// frames from this class instead of wxFrame.
//
// A palette window
class WXDLLIMPEXP_PALETTEFRM wxPaletteFrameBase : public wxFrame
{
private:		// this should never be touched directly !!!

	// TRUE if this window is currently drawn as active.
	// Use the #SetAsActive and SetAsInactive functions to
	// change the value of this variable.
	//
	// NOTE: changing this variable means immediately change
	//       the color of the caption bar of this window.
	bool m_bIsActive;
	
protected:		// member variables

	// The array of minibuttons associated with this window.
	wxArrayPtrVoid m_arrButtons;

	// The window contained in this frame.
	wxWindow *m_pClientWnd;

	// TRUE if the window is currently rolled up.
	bool m_bRolled;

	// The size of the client window before last roll-up.
	// This variable is used to restore the size when unrolling.
	wxSize m_szLast;

	// The size hints of the palette window before last roll-up.
	wxSize m_szLastHints;

	// If TRUE the wxPaletteFrame shows the context menu returned by 
	// wxMainFrame::GetPalCtxMenu when the user right-clicks the caption bar.
	bool m_bShowCtxMenu;

protected:		// internal utilities

	// Returns the position of the mouse cursor in screen coordinates.
	wxPoint GetScrPosition(wxMouseEvent &ev) const {
		return ClientToScreen(ev.GetPosition());
	}

	// Returns the rectangle in screen coordnates.
	wxRect GetScrWindowRect() const {
		return wxRect(GetPosition(), GetSize());
	}

#ifdef wxPALETTEFRM_USE_MINIBTN
	// Checks the style for the wxMINIMIZE_BOX, wxMAXIMIZE_BOX, wxCOLLAPSE_BOX, 
	// wxCLOSE_BOX styles and then adds the required minibuttons...
	virtual void AddMiniButtonFromStyle(long style);

	// Returns the position of the i-th minibutton on this window.
	virtual wxPoint GetButtonPos(int i) const;
	
	// Sync the buttons pos with the coords returned by GetButtonPos
	virtual void MoveMiniButtons();

	// Returns the n-th minibutton of this window.
	wxMiniButtonBase *GetButton(int n) const	{ return (wxMiniButtonBase *)m_arrButtons[n]; }

	// Returns the number of minibuttons contained in this window.
	int GetButtonCount() const					{ return (int)m_arrButtons.GetCount(); }
#endif
	
	// Encodes the layout of this window in a wxString object and
	// then returns it.
	virtual wxString EncodeLayout() const;

	// Decodes the layout of the given wxString and applies it to
	// this window.
	virtual void DecodeLayout(const wxString &);

	// Draws a frame in the given DC for the given window in the given
	// rect with the given activation state and eventually with an icon.
	virtual void DrawCaption(wxPaletteFrameBase *, wxDC &, const wxRect &rc,
		bool drawicon = TRUE, bool drawactive = TRUE);

	// Inits to zero/NULL all the wxPaletteFrameBase internal variables
	void Init();

public:

	// Default constructor.
	wxPaletteFrameBase() { Init(); }
	wxPaletteFrameBase(
		wxMainFrameBase *parent,
		wxWindowID id,
		const wxString &title = wxT("Palette"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxPALETTEFRM_DEFAULT_STYLE,
		const wxString& name = wxT("wxPaletteFrame"));

	bool Create(wxMainFrameBase *parent,
			wxWindowID id,
			const wxString& title,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxPALETTEFRM_DEFAULT_STYLE,
			const wxString& name = wxT("wxPaletteFrame"));

	// Destructor.
	virtual ~wxPaletteFrameBase();



	// General functions
	// ----------------------

	// Returns TRUE if this window must be currently drawn as active;
	// this function also checks the linked windows' activation state
	// to determine if this window must be marked as active.
	virtual bool IsToDrawAsActive() const	 { return m_bIsActive; }

#ifdef wxPALETTEFRM_USE_MINIBTN
	// Adds a button. Buttons are added in right-to-left order.
	virtual void AddMiniButton(wxMiniButtonBase *pBtn);

	// Draws the minibuttons of this window. The given DC must be
	// a wxWindowDC for this class.
	virtual void DrawMiniButtons(wxDC &);
#endif

	// Returns the height (in pixels) of the caption bar of this frame;
	// that is the distance from the top pixels of the caption (not the
	// border) and the first pixel which is placed in the client area.
	virtual int GetTitleHeight() const;

	// Returns the width of the borders of this frame. This value is NOT
	// considered in the #GetTitleHeight() function.
	virtual int GetBorderSize() const;

	// Sets this window as in/active (even if it's not); if the activation
	// state has changed (that is, the window was deactived), then a
	// refresh is immediately performed.
	void SetAsActive(bool bNewState = TRUE);

	// Like #SetAsActive but obviously does the opposite work.
	void SetAsInactive()					{ SetAsActive(FALSE); }

	// Enables/disables context menu,
	void EnableCtxMenu(bool b = TRUE)		{ m_bShowCtxMenu = b; }
	
	// Returns one of the wxPaletteFrameHitCode indicating the position
	// of the mouse cursor over this palette frame.
	virtual wxPaletteFrameHitCode HitTest(const wxPoint &pos);

		


	// Layout load/save
	// -----------------

	// Saves the layout of this window in the given wxConfig object and
	// in the given key name. 
	// If the keyname is wxEmptyString, then the title of this window is
	// used as the key to write the layout.
	void SaveLayout(wxConfigBase *p, const wxString &keyname = wxEmptyString) const;

	// Does the opposite of #SaveLayout returning TRUE if the layout was
	// found in the given wxConfig object and correctly applied to the window.
	bool LoadLayout(wxConfigBase *p, const wxString &keyname = wxEmptyString);



	// Static utilities
	// ----------------------
		
	// Draws a sunken border in the given rect
	static void DrawSunkenBorders(wxDC &, const wxRect &);



	// Event handlers (really, an event table is not used for
	// this class due to some problems it has on Win32 with
	// the current version of wxWidgets... thus, events are
	// captured in the ProcessEvent() function).
	// --------------------------------------------------------

	void OnPaint(wxPaintEvent &);		// just calls Draw()
	virtual void OnMotion( wxMouseEvent & ) {}

	void OnSize( wxSizeEvent& event );
	void OnActivate( wxActivateEvent &event );
	void OnClose( wxCloseEvent & );

	void OnLeftDown( wxMouseEvent& event );		// this block of functions just call...
	void OnLeftUp( wxMouseEvent& event );
	void OnRightDown( wxMouseEvent& event );
	void OnRightUp( wxMouseEvent& event );
	
	virtual bool OnPosLeftDown( const wxPoint & );		// ... these functions
	virtual bool OnPosLeftUp( const wxPoint & );
	virtual bool OnPosRightDown( const wxPoint & );
	virtual bool OnPosRightUp( const wxPoint & );

	void OnCollapseBox(wxCommandEvent &);
	void OnCloseBox(wxCommandEvent &);



	// wxWindow overrides
	// -------------------

	// Draws the window and the minibuttons. This function
	// can be virtual and this is why it is used instead of
	// OnPaint (which cannot be virtual since it's an event
	// handler) when possible...
	virtual void Draw(wxWindowDC *dc = NULL, bool bDrawCaption = TRUE, bool bDrawMiniBtn = TRUE);

	// Repositionates the minibuttons of this frame.
	virtual bool Layout();

	// Intercepts some events required for this class to make its job.
	virtual bool ProcessEvent(wxEvent &event);

	// Updates the window forcing a dummy resize.
	virtual void Update();
	
	// Destroys this window adding it to the list of the windows
	// which must be destroyed in idle time (this is a top level window:
	// it cannot be destroyed like all other windows).
	virtual bool Destroy();

	// Is this window a top level one?
    virtual bool IsTopLevel() const			{ return TRUE; }



	// coordinates translation

	// Converts client coordinates to frame coordinates, that is the
	// coordinates referred to the upper-left point of this window
	// (without taking in count borders & caption).
	virtual wxPoint ClientToFrame(const wxPoint &pt) const;
	virtual wxPoint ScreenToFrame(const wxPoint &pos) const;

#ifndef __WXGTK__	
    virtual void DoGetClientSize( int *width, int *height ) const;
    virtual void DoSetClientSize( int width, int height );

    virtual wxPoint GetClientAreaOrigin() const;
#endif


	// Client managers
	// ----------------------------

	// Creates a simple wxPanel which is used as client window for this
	// frame and then returns it.
	// Inside a wxPanel you can use place any other window, eventually
	// using sizers to handle them. This makes a wxPaletteFrame a
	// flexible object which can be used as advanced toolbar.
	wxWindow *CreateClient(long style = wxNO_BORDER);

	// Sets the client for this tool window.
	virtual void SetClient(wxWindow *pWnd);

	// Returns the client window.
	virtual wxWindow *GetClient()					{ return m_pClientWnd; }

	// Returns a const pointer to the client window.
	virtual const wxWindow *GetClient() const		{ return m_pClientWnd; }

	// Sets the size hints for this window using the size hints of the
	// client window.
	virtual void SetSizeHintsUsingClientHints();



	// Roll/Unroll feature
	// ----------------------------

	// Returns the size that this window is set to when it's rolled.
	// By default this is the lenght (in pixels) of the title + 60 pixels.
	virtual wxSize GetRolledSize() const;

	// Returns the size of the window previous to the last rolling.
	virtual wxSize GetUnRolledSize() const;

	virtual void Roll();
	virtual void UnRoll();
	virtual bool IsRolled() const		 { return m_bRolled; }	

	
private:
	// even if we overloaded the ProcessEvent() function, we can still
	// use without problems the event tables...
	DECLARE_EVENT_TABLE()
};

#endif		// wxPALETTEFRM_USE_PALETTEFRM




#ifdef wxPALETTEFRM_USE_MAINFRAME

// Implements a wxFrame that can handle any number of wxPaletteFrame
// as children. 
class WXDLLIMPEXP_PALETTEFRM wxMainFrameBase : public wxFrame
{
protected:		// member variables

	// The menu to show when the user right-clicks one of the captionbar
	// of a child wxPaletteFrame.
	wxMenu *m_pPalCtxMenu;

	// The flag used to enable/disable the context menu
	bool m_bCtxMenuEnabled;

	// If TRUE the menu commands related to the context menu of the palettes
	// are automatically handled.
	bool m_bCtxMenuHandlingEnabled;

	// The flag used to decide if we must search the main menu bar of
	// this wxFrame window to update the menu items with the same IDs
	// of those in the paletteframes' context menu.
	// If in the main menubar there are no menu items with the same IDs of 
	// those menu items shown in the paletteframes' context menu, then this
	// flag *must* be set to FALSE, otherwise an ASSERT will fail.
	bool m_bUpdateMainMenubar;

	// The list of the wxPaletteFrame owned by this window.
	wxArrayPtrVoid m_palList;


protected:		// utilities

	// Inits to zero/NULL/false all the wxMainFrameBase variables
	void Init();

public:

    // ctor(s)
    wxMainFrameBase() { Init(); }
    wxMainFrameBase(
		wxWindow* parent,
		wxWindowID id,
		const wxString &title = wxT("wxMainFrame"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_MAINFRAME_STYLE,
		const wxString& name = wxT("wxMainFrame"))
		: wxFrame(parent, id, title, pos, size, style, name) { Init(); }
	virtual ~wxMainFrameBase() { CleanupPalContextMenu(); }



	// wxMainFrame miscellaneous functions
	// ------------------------------------

	// Returns TRUE if the given handle is one of the wxPaletteFrame owned
	// by this window.
	bool IsOneOfPalettes(WXWidget h) const;

	// Handles the wxEVT_ACTIVATE event.
	virtual void OnActivate(wxActivateEvent &event);

	// Synchronizes the context menu of the paletteframes with their
	// actual show state...
	void SyncPalContextMenu();

	// Handles updates of the context menu 
	void SyncMainMenubar();

	// Enables/disables the context menu; when it is disabled and the user
	// right-clicks the wxPaletteFrames' caption bar, nothing happens.
	void EnableContextMenu(bool b = TRUE)			{ m_bCtxMenuEnabled = b; }

	// Enables/disables the automatic handling of the context menu commands.
	void EnableContextMenuHandling(bool b = TRUE)	{ m_bCtxMenuHandlingEnabled = b; }

	// This is very useful to enable the automatic update of those menu items
	// of the main menubar with the same IDs of the menu items shown in the
	// wxPaletteFrameBase's context menu, to keep the two menus synchronized.
	// See the m_bUpdateMainMenubar description above for more info.
	void EnableMainMenubarUpdate(bool b = TRUE)		{ m_bUpdateMainMenubar = b; }

	// Returns the context menu activation flag
	bool isContextMenuEnabled() const		{ return m_bCtxMenuEnabled; }

	// Returns the main menubar update activation flag
	bool isMainMenubarUpdateEnabled() const	{ return m_bUpdateMainMenubar; }

	// Returns the menu to be shown as context menu in the wxPaletteFrames.
	wxMenu *GetPalContextMenu()				{ return m_pPalCtxMenu; }

	// Returns the number of wxPaletteFrame owned by this window.
	int GetPalCount() const					{ return (int)m_palList.GetCount(); }

	// Returns the n-th wxPaletteFrame owned by this window.
	wxPaletteFrameBase *GetPal(int n) const	{ return (wxPaletteFrameBase *)m_palList.Item(n); }

	// Returns the base ID to use for the menu items in the context menu
	// of the wxPaletteFrame(s).
	virtual int GetPalMenuBaseId() const		{ return wxID_HIGHEST+1; }

	// Initializes the context menu with the list of the titles of the wxPaletteFrames
	virtual void InitPalContextMenu();

	// Undoes what #InitPalContextMenu() does.
	virtual void CleanupPalContextMenu();
	
	

	// wxFrame overrides
	// ------------------

	virtual void AddChild(wxWindowBase *newchild);
    virtual void RemoveChild(wxWindowBase *child);
	virtual bool ProcessEvent(wxEvent &ev);
};

#endif		// wxPALETTEFRM_USE_MAINFRAME



#ifdef wxPALETTEFRM_USE_MINIBTN


// A minibutton which can be placed in the wxPaletteFrame caption.
// This class doesn't derive from wxWindow because of some problems
// it would have if it was a window, for example being hidden by 
// wxPaletteFrame caption even if it is a wxPaletteFrame's child
// window...
class WXDLLIMPEXP_PALETTEFRM wxMiniButtonBase : public wxEvtHandler//wxObject
{
protected:		// member variables

	// The parent of this button
	wxPaletteFrameBase *m_pParent;

	// The position of this button in parent coordinates
	wxPoint m_ptPos;

	// The ID of this button.
	wxWindowID m_nId;

	bool m_bVisible;		// TRUE if this button is visible
	bool m_bEnabled;		// TRUE if this button is enabled
	bool m_bPressed;		// TRUE if this button is being pressed
	bool m_bCapturing;		// TRUE if this button has captured the mouse


protected:		// member functions

	// Draws the button label on the given DC at the given coordinates.
	// This function is called by the Draw(wxDC &) function which is
	// defined below...
	virtual void Draw(wxDC &, int, int) = 0;

	// Executes the action associated with this button.
	// This function should always return TRUE; FALSE is kept for some
	// special uses.
	virtual bool Exec() = 0;

	// Sets all wxMiniButtonBase variables to false/zero.
	void Init();

public:


	// Default constructor.
	// The parent window *must* be a wxPaletteFrameBase because we
	// need some wxPaletteFrame-specific functions	
	wxMiniButtonBase(wxPaletteFrameBase *parent, int id = -1);
	wxMiniButtonBase() { Init(); }

	virtual ~wxMiniButtonBase() {}



	// Window-like functions
	// ---------------------------

	// Set the position of the button.
	virtual void Move(const wxPoint& pos)		{ Move(pos.x, pos.y); }
	virtual void Move(int x, int y)				{ m_ptPos.x = x; m_ptPos.y = y; }

	// Returns the position of this button in the parent frame.
	virtual wxPoint GetPos() const				{ return m_ptPos; }

	// Returns the size of this button.
	virtual wxSize GetSize() const;

	// Returns the parent of this button.
	wxPaletteFrameBase *GetParent() const		{ return m_pParent; }

	// Returns TRUE if the given position was over the button.
	virtual bool HitTest(const wxPoint& pos);

	// Draws the button. Override this to implement
	// the desired appearance.
	virtual void Draw(wxDC &dc);

	// Redraws this button on the parent using a wxWindowDC.
	void Refresh();

	// Enable or disable the button.
	void Enable(bool enable)				{ m_bEnabled = enable; }

	// Returns TRUE if this button is pressed.
	bool IsBeingPressed() const				{ return m_bPressed; }

	// Returns TRUE if the button is visible.
	bool IsVisible() const					{ return m_bVisible; }

	// Shows/hide this button.
	void Show(bool b = TRUE)				{ m_bVisible = b; }



	// Event handlers-like functions
	// ----------------------------------

	// Responds to a left down event.
	bool OnLeftDown(const wxPoint& pos);

	// Responds to a left up event.
	bool OnLeftUp(const wxPoint& pos);



	// Draw utilities
	// -------------------------

	// Draws the frame of a button.
	void DrawButtonFrame(wxDC &, int x, int y);
	
	// Returns the thickness of the frame drawn by #DrawButtonFrame
	int GetBorderSize() const;
};

#endif		// wxPALETTEFRM_USE_MINIBTN




#if defined(__WXMSW__)
	
	// the msw/palettefrm.h header files derives a new class from
	// wxMiniButtonBase called wxMiniButton which is the base
	// class for the wxMiniButton below... so we do not need to
	// include here the generic minibtn.h header file...
    #include "wx/msw/palettefrm.h"

#elif defined(__WXGTK__)

    #include "wx/gtk/palettefrm.h"

	// no changes to this class are required...
    #define wxMainFrame				wxMainFrameBase

#elif defined(__WXX11__)

    #include "wx/x11/palettefrm.h"	
	
	// no changes to these classes are required...    
    #define wxMainFrame				wxMainFrameBase
	#define wxMiniButton			wxMiniButtonBase

#endif




#endif		// _WX_PALETTEFRM_BASE_H_
