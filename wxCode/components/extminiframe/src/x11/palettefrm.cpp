/////////////////////////////////////////////////////////////////////////////
// Name:        x11/palettefrm.cpp
// Purpose:     wxExtMiniFrame
// Author:      Francesco Montorsi
// Created:     2004/03/03
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


// includes
#include "wx/wx.h"
#include "wx/settings.h"
#include "wx/tokenzr.h"
#include "wx/defs.h"
#include "wx/object.h"
#include "wx/frame.h"
#include "wx/dcscreen.h"
#include "wx/window.h"

// X11 includes
#include "wx/settings.h"
#include "wx/x11/private.h"
#include "wx/unix/utilsx11.h"
#include "X11/Xatom.h"


#include "wx/palettefrm.h"
#include "wx/minibtn.h"





// RTTI class declarations
IMPLEMENT_DYNAMIC_CLASS(wxExtMiniFrame, wxFrame)



// a little utility
#define X11_GET_ATOM(d, name, result)		*result = XInternAtom(d, name, FALSE)





// ----------------------------------------
// Global x11 functions
// ----------------------------------------

void wxCreateWMClientLeader(Display *d, int scr)
{
    wxEXTMF_LOG(wxT("CreateWMClientLeader()"));
    
    if ( x11_wm_client_leader ) return;
    
    x11_client_leader =	XCreateSimpleWindow( d, RootWindow(d, scr), 0, 0, 1, 1, 0, 0, 0 );
    
    // set client leader property to itself
    XChangeProperty(d, x11_client_leader, x11_wm_client_leader,
        XA_WINDOW, 32, PropModeReplace, (unsigned char *)&x11_wm_client_leader, 1 );
    
    // If we are session managed, inform the window manager about it
    char session[32];
    strcpy(session, wxT("123"));
    XChangeProperty( d,
        x11_client_leader, x11_sm_client_id,
        XA_STRING, 8, PropModeReplace,
        (unsigned char *)session, strlen(session));
}

void wxInitX11Atoms(Display *d)
{
    wxEXTMF_LOG(wxT("InitX11Atoms()"));
    
    X11_GET_ATOM( d, wxT("WM_PROTOCOLS"), &x11_wm_protocols );
    X11_GET_ATOM( d, wxT("WM_DELETE_WINDOW"), &x11_wm_delete_window );
    X11_GET_ATOM( d, wxT("WM_STATE"), &x11_wm_state );
    X11_GET_ATOM( d, wxT("WM_CHANGE_STATE"), &x11_wm_change_state );
    X11_GET_ATOM( d, wxT("WM_TAKE_FOCUS"), &x11_wm_take_focus );
    X11_GET_ATOM( d, wxT("WM_CLIENT_LEADER"), &x11_wm_client_leader);
    X11_GET_ATOM( d, wxT("WM_WINDOW_ROLE"), &x11_window_role);
    X11_GET_ATOM( d, wxT("SM_CLIENT_ID"), &x11_sm_client_id);
    X11_GET_ATOM( d, wxT("_NET_WM_PING"), &x11_net_wm_ping );
    X11_GET_ATOM( d, wxT("_MOTIF_WM_HINTS"), &x11_xa_motif_wm_hints );
    
    X11_GET_ATOM( d, wxT("_NET_WM_STATE"), &x11_net_wm_state );	
    X11_GET_ATOM( d, wxT("_NET_WM_WINDOW_TYPE"), &x11_net_wm_window_type );
    X11_GET_ATOM( d, wxT("_NET_WM_WINDOW_TYPE_NORMAL"), &x11_net_wm_window_type_normal );
    X11_GET_ATOM( d, wxT("_NET_WM_WINDOW_TYPE_DIALOG"), &x11_net_wm_window_type_dialog );
    X11_GET_ATOM( d, wxT("_NET_WM_WINDOW_TYPE_TOOLBAR"), &x11_net_wm_window_type_toolbar );
    X11_GET_ATOM( d, wxT("_NET_WM_WINDOW_TYPE_MENU"), &x11_net_wm_window_type_menu );
    X11_GET_ATOM( d, wxT("_NET_WM_WINDOW_TYPE_UTILITY"), &x11_net_wm_window_type_utility );
    X11_GET_ATOM( d, wxT("_NET_WM_WINDOW_TYPE_SPLASH"), &x11_net_wm_window_type_splash );
    X11_GET_ATOM( d, wxT("_KDE_NET_WM_WINDOW_TYPE_OVERRIDE"), &x11_net_wm_window_type_override );
}






// ------------------------------------
// wxEXTMINIFRAME
// ------------------------------------

wxExtMiniFrame::wxExtMiniFrame(wxMainFrameBase* parent,
                            wxWindowID id,
                            const wxString &title,
                            const wxPoint& pos,
                            const wxSize& size,
                            long style,
                            const wxString& name)		
{
    // just create the window...
    Create(parent, id, title, pos, size, style, name);
}

bool wxExtMiniFrame::Create(wxMainFrameBase* parent,
                            wxWindowID id,
                            const wxString &title,
                            const wxPoint& pos,
                            const wxSize& size,
                            long style,
                            const wxString& name)
{
    Init();		// this sets to false/zero/NULL all variables...
    
    wxEXTMF_LOG(wxT("wxExtMiniFrameX11::Create - creation started"));
    
    // remove invalid styles...
    style &= wxEXTMINIFRM_STYLE_MASK;
    
    long toremove = wxSTAY_ON_TOP | wxTOPLEVEL_EX_DIALOG | wxSIMPLE_BORDER | wxSUNKEN_BORDER;
    style &= ~toremove;
    style |= wxNO_BORDER;
    
    // init base class variables	
    wxTopLevelWindow::Init();
    m_windowStyle = style;
    m_parent = parent;
    SetName(name);
    
    m_windowId = id == -1 ? NewControlId() : id;
    
    if (parent)
        parent->AddChild(this);
    wxTopLevelWindows.Append(this);
    
    wxEXTMF_LOG(wxT("setting some basics"));
    
    // actually create the window...
    if (GetExtraStyle() & wxTOPLEVEL_EX_DIALOG)
        m_backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
    else
        m_backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_APPWORKSPACE);
    
    m_hasBgCol = TRUE;
    
    m_x = pos.x;
    if (m_x < -1)
        m_x = 10;
    
    m_y = pos.y;
    if (m_y < 0)
        m_y = 10;
    
    m_width = size.x;
    if (m_width < 0)
        m_width = 500;
    
    m_height = size.y;
    if (m_height < 0)
        m_height = 380;
    
    // this contains the X11 specific stuff
    wxEXTMF_LOG(wxT("setting as toolbar"));
    SetAsToolbar(0, 0, 0);
    
    // if we would set auto layout to TRUE, we would get Layout() automatically
    // called each time the window is resized, but then wxFrame::OnSize would
    // not automatically resize the child window...
    SetAutoLayout(FALSE);
    
    // save window title
    wxEXTMF_LOG(wxT("setting the title"));
    SetTitle(title);
    
    // add the standard minibuttons required (in RIGHT order !!!)
    wxEXTMF_LOG(wxT("wxExtMiniFrameX11::Create - adding minibuttons (if any)"));
    AddMiniButtonFromStyle(style);
    
    // set a smaller font for this window
    SetFont(wxFont(10, wxSWISS, wxNORMAL, wxNORMAL));
    
    // set some defaults (this would be done normally by wxExtMiniFrameBase::Create
    // but here we do not call it, because it would call the wxFrame::Create function
    // which we need to override... so, we must set them ourselves)
    m_pClientWnd = NULL;
    m_bRolled = FALSE;
    m_bShowCtxMenu = TRUE;
    m_bDragging = FALSE;
    m_ptDragAnchor = wxPoint(-1, -1);
    
    wxEXTMF_LOG(wxT("wxExtMiniFrameX11::Create - ")
        wxT("A wxExtMiniFrameX11 has been successfully created..."));
    
    return TRUE;
}

void wxExtMiniFrame::SetAsToolbar(Display *, Window, int)
{
    // get some global utils
    Display *d = wxGlobalDisplay();
    int scr = DefaultScreen( d );
    Visual *xvisual = DefaultVisual( d, scr );
    Window xparent = RootWindow( d, scr );
    
    // set the background color of this window
    Colormap cm = DefaultColormap( d, scr );
    m_backgroundColour.CalcPixel( (WXColormap) cm );
    
    // set the border & background color
    XSetWindowAttributes xattributes;
    long xattributes_mask = 
        CWBorderPixel | CWBackPixel;
    
    xattributes.background_pixel = m_backgroundColour.GetPixel();
    xattributes.border_pixel = BlackPixel( d, scr );	
    
    // set the event mask
    xattributes_mask |= CWEventMask;
    xattributes.event_mask = 
        ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
        ButtonMotionMask | EnterWindowMask | LeaveWindowMask | PointerMotionMask |
        KeymapStateMask | FocusChangeMask | ColormapChangeMask | StructureNotifyMask |
        PropertyChangeMask;
    
    // create a simple window (not a toolbar yet !!!)
    Window id = XCreateWindow( d, xparent, m_x, m_y, m_width, m_height,
        0, DefaultDepth(d, scr), InputOutput, 
        xvisual, xattributes_mask, &xattributes );
    XUnmapWindow(d, id);		// is this important ?
    
    // set wxWidgets basics
    m_mainWindow = (WXWindow) id;
    m_clientWindow = (WXWindow) id;
    
    wxAddWindowToTable( id, (wxWindow*)this );
    
    // init x11 atoms if they were not init before
    if (!x11_net_wm_window_type_toolbar) wxInitX11Atoms(d);
    
    // NET window types
    long net_wintypes[7] = { 0, 0, 0, 0, 0, 0, 0 };
    int curr_wintype = 0;
    
    // NET window states
    long net_winstates[6] = { 0, 0, 0, 0, 0, 0 };
    int curr_winstate = 0;
    
    struct {
        ulong flags, functions, decorations;
        long input_mode;
        ulong status;
    } mwmhints;
    
    mwmhints.flags = mwmhints.functions = 0L;
    mwmhints.input_mode = 0L;
    mwmhints.status = 0L;
    mwmhints.decorations = 1L << 1;
    mwmhints.flags |= (1L << 1); // MWM_HINTS_DECORATIONS
    
    // override netwm type - quick and easy for KDE noborder
    net_wintypes[curr_wintype++] = x11_net_wm_window_type_override;
    net_wintypes[curr_wintype++] = x11_net_wm_window_type_toolbar;
    net_wintypes[curr_wintype++] = x11_net_wm_window_type_utility;
    net_wintypes[curr_wintype++] = x11_net_wm_window_type_dialog;
    net_wintypes[curr_wintype++] = x11_net_wm_window_type_normal;
    
    // The _NET_WM_STATE_ABOVE window state is not necessary: under GNOME
    // it would make the paletteframe always above all the other windows...
    //     net_winstates[curr_winstate++] = x11_net_wm_state_above;
    
    
    // this tells the WM to mantain the wxExtMiniFrame always above the parent
    // (which is the application's main frame)
    XSetTransientForHint( d, id, (Window)GetParent()->GetMainWindow() );
    
    XSizeHints size_hints;
    size_hints.flags = PWinGravity;
    size_hints.win_gravity = NorthWestGravity;
    
    XWMHints wm_hints;			// window manager hints
    wm_hints.input = True;
    wm_hints.initial_state = NormalState;
    wm_hints.flags = InputHint | StateHint;
    
    // create the client leader if it has not been created yet...
    if ( !x11_wm_client_leader )
        wxCreateWMClientLeader(d, scr);
    
    wm_hints.window_group = x11_wm_client_leader;
    wm_hints.flags |= WindowGroupHint;
    
    XClassHint class_hint;
    class_hint.res_name = wxT("myApp"); // application name
    class_hint.res_class = wxT("wxExtMiniFrame");	// application class
    
    wxEXTMF_LOG(wxT("wxExtMiniFrameX11::SetAsToolbar() - setting WM prop"));
    XSetWMProperties( d, id, 0, 0, 0, 0, &size_hints, &wm_hints, &class_hint );
    
    Atom protocols[4];
    int n = 0;
    protocols[n++] = x11_wm_delete_window;	// support del window protocol
    protocols[n++] = x11_wm_take_focus;	// support take focus window protocol
    protocols[n++] = x11_net_wm_ping;	// support _NET_WM_PING protocol
    
    wxEXTMF_LOG(wxT("wxExtMiniFrameX11::SetAsToolbar() - setting WM protocols"));
    XSetWMProtocols( d, id, protocols, n );
    
    wxEXTMF_LOG(wxT("wxExtMiniFrameX11::SetAsToolbar() - going to change various properties"));
    
    // set mwm hints
    if ( mwmhints.flags != 0l )
        XChangeProperty(d, id, x11_xa_motif_wm_hints, x11_xa_motif_wm_hints, 32,
        PropModeReplace, (unsigned char *) &mwmhints, 5);
    
    // set _NET_WM_WINDOW_TYPE
    if (curr_wintype > 0)
        XChangeProperty(d, id, x11_net_wm_window_type, XA_ATOM, 32, PropModeReplace,
        (unsigned char *) net_wintypes, curr_wintype);
    
    // set _NET_WM_WINDOW_STATE
    if (curr_winstate > 0)
        XChangeProperty(d, id, x11_net_wm_state, XA_ATOM, 32, PropModeReplace,
        (unsigned char *) net_winstates, curr_winstate);
    
    // declare the widget's object name as window role
    char str[64];
    strcpy(str, GetName());
    XChangeProperty( d, id,
        x11_window_role, XA_STRING, 8, PropModeReplace,
        (unsigned char *)str, GetName().Len());
    
    // set client leader property
    XChangeProperty( d, id, x11_wm_client_leader,
        XA_WINDOW, 32, PropModeReplace,
        (unsigned char *)&x11_wm_client_leader, 1 );
    
    // set a stacking order option: is this influent ?
    XWindowChanges xwc;
    xwc.stack_mode = Above;
    xwc.sibling = (Window)GetParent()->GetMainWindow();
    xwc.x = xwc.y = 0;
    xwc.width = xwc.height = 0;
    xwc.border_width = 0;
    XConfigureWindow(d, id, CWStackMode, &xwc);	
}

bool wxExtMiniFrame::OnPosLeftDown(const wxPoint &pos)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameX11::OnPosLeftDown()"));
    
    if (wxExtMiniFrameBase::OnPosLeftDown(pos))
        return TRUE;
    
    // check if the user clicked on the title bar
    wxExtMiniFrameHitCode hc = HitTest(pos);
    switch (hc) {
    case wxPFHT_TOPLEVEL_TITLEBAR:		// start dragging
        if (m_ptDragAnchor.x == -1)
            m_ptDragAnchor = pos;
        
        m_bDragging = TRUE;
        wxEXTMF_LOG(wxT("wxExtMiniFrameX11::OnPosLeftDown - draganchor %d;%d"), 
            m_ptDragAnchor.x, m_ptDragAnchor.y);		
        CaptureMouse();
        return TRUE;
    }
    
    return FALSE;		
}

void wxExtMiniFrame::OnMotion(wxMouseEvent &event)
{
    //wxPoint pos = event.GetPosition();
    //pos = ClientToScreen(pos);
    
    // Sometimes the event we get contains invalid coordinates: better
    // use the global wxGetMousePosition() function which returns a wxPoint
    // directly in screen coords.
    wxPoint pos = wxGetMousePosition();	
    
    if (event.LeftIsDown() && m_bDragging) {
        
        if (m_ptDragAnchor.x == -1)
            m_ptDragAnchor = pos;
        
        wxEXTMF_LOG(wxT("wxExtMiniFrameX11::OnMotion - draganchor %d;%d - pos %d;%d"), 
            m_ptDragAnchor.x, m_ptDragAnchor.y, pos.x, pos.y);
        
        Move(pos.x - m_ptDragAnchor.x, 
            pos.y - m_ptDragAnchor.y);
        
    } else {
        
        // reset drag variables
        m_bDragging = FALSE;
        m_ptDragAnchor = wxPoint(-1, -1);
        
        pos = ScreenToClient(pos);
        
        // for some reason these are required
        pos.x += GetBorderSize()*2;
        pos.y += GetTitleHeight()*2+GetBorderSize()*2;
        
        // check where the mouse is placed
        wxExtMiniFrameHitCode hc = HitTest(pos);
        
        switch (hc) {
        /*case wxPFHT_TOPLEVEL_BORDER_W:		// resizing has not been implemented yet...
        case wxPFHT_TOPLEVEL_BORDER_E:
        SetCursor(*wxHOURGLASS_CURSOR);
        break;
        
        case wxPFHT_TOPLEVEL_BORDER_N:
        case wxPFHT_TOPLEVEL_BORDER_S:
        SetCursor(*wxHOURGLASS_CURSOR);
        break;	
        
        case wxPFHT_TOPLEVEL_BORDER_NE:
        case wxPFHT_TOPLEVEL_BORDER_SW:
        SetCursor(*wxHOURGLASS_CURSOR);
        break;
        
        case wxPFHT_TOPLEVEL_BORDER_NW:
        case wxPFHT_TOPLEVEL_BORDER_SE:
        SetCursor(*wxHOURGLASS_CURSOR);
            break;*/
            
        default:
            SetCursor(*wxSTANDARD_CURSOR);
        }
        
        wxEXTMF_LOG(wxT("wxExtMiniFrameX11::OnMotion - POSITION: %d;%d HITCODE: %d"), pos.x, pos.y, hc);
    }
}

bool wxExtMiniFrame::OnPosLeftUp(const wxPoint &pos)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameX11::OnPosLeftUp"));
    m_bDragging = FALSE;
    m_ptDragAnchor = wxPoint(-1, -1);
    
    // we must be sure to have captured the mouse, or we will get
    // an assert failure...
    if (GetCapture() == this)
        ReleaseMouse();
    
    return wxExtMiniFrameBase::OnPosLeftUp(pos);
}

void wxExtMiniFrame::Roll()
{
    if (IsRolled())
        return;		// already rolled
    
    // save the current size (it will be restored in UnRoll)
    m_szLast = GetClientSize();
    m_szLastHints = wxSize(m_minWidth, m_minHeight);
    
    // wxX11 for some reason needs to have border size added here...
    m_szLast.x += GetBorderSize();
    m_szLast.y += GetBorderSize();
    
    // for some strange reason, the SetSize() command below will also reset to 0;0
    // the coordinates of this window: better hide it temporaneously to avoid a
    // flickering quite annoying for the user...
    // the only drawback is that for a moment, the focus will go to our mainframe
    // and then (see the Show() at the end of this fnc) will return to us...
    Hide();
    
    // hide the client window and change our size
    SetSizeHints(0, 0);
    GetClient()->Hide();
    
    // cause of the bug above explained (position reset when calling SetSize()) we must
    // save our position so we can restore it later...
    wxPoint pos = GetPosition();
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::Roll() - before rolling I was at %d;%d"), pos.x, pos.y);
    
    
    wxSize newsz = GetRolledSize();
    
    // this SetSize will also move the window: I tried to replace this with the
    // x11 API
    //    XResizeWindow(wxGlobalDisplay(), (Window)m_mainWindow, newsz.x, newsz.y);   
    // but then I get other errors...
    SetSize(newsz);
    MoveMiniButtons();
    m_bRolled = TRUE;
    
    // restore our old pos	
    Move(pos);
    
    // we can be shown again...
    Show();
}

