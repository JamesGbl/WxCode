/////////////////////////////////////////////////////////////////////////////
// Name:        palettefrmcmn.cpp
// Purpose:     wxExtMiniFrameBase, wxMainFrameBase, wxMiniButtonBase
// Author:      Francesco Montorsi
// Created:     2004/03/03
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/window.h"
    #include "wx/settings.h"	
    #include "wx/defs.h"
    #include "wx/object.h"
    #include "wx/frame.h"
    #include "wx/dcscreen.h"
    #include "wx/menu.h"
    #include "wx/log.h"
    #include "wx/panel.h"
#endif


// includes
#include "wx/tokenzr.h"		// this is not included by wxprec.h
#include "wx/palettefrm.h"
#include "wx/minibtn.h"


// intercept minibtn clicks
#ifdef wxEXTMINIFRM_USE_EXTMINIFRM

BEGIN_EVENT_TABLE( wxExtMiniFrameBase, wxFrame )
#ifdef wxEXTMINIFRM_USE_MINIBTN
    EVT_CLOSEBOX_CLICKED( -1, wxExtMiniFrameBase::OnCloseBox )
    EVT_COLLAPSEBOX_CLICKED( -1, wxExtMiniFrameBase::OnCollapseBox )
#endif
END_EVENT_TABLE()

#endif		// wxEXTMINIFRM_USE_EXTMINIFRM





// ------------------------------------
// wxMAINFRAMEBASE
// ------------------------------------

#ifdef wxEXTMINIFRM_USE_MAINFRAME

void wxMainFrameBase::Init()
{
    m_pPalCtxMenu = NULL;	
    m_bUpdateMainMenubar = FALSE;

    // by default, these are enabled
    m_bCtxMenuEnabled = TRUE;
    m_bCtxMenuHandlingEnabled = TRUE;
}

bool wxMainFrameBase::IsOneOfExtMiniFrame(WXWidget win) const
{
    // scan children
    for (int i=0; i < GetPalCount(); i++) {
        wxExtMiniFrameBase *p = (wxExtMiniFrameBase *)GetPal(i);
        if (win == p->GetHandle())
            return TRUE;
    }

    // handle not found...
    return FALSE;
}

void wxMainFrameBase::AddChild(wxWindowBase *newchild)
{
    // if the new child is a wxExtMiniFrameBase, we add it to
    // our special list, too.
    // Obviously, here we cannot use IsOneOfExtMiniFrame() because
    // that function works with m_palList...
    if (newchild->IsKindOf(CLASSINFO(wxExtMiniFrame)))
        m_palList.Add((wxExtMiniFrameBase*)newchild);

    wxFrame::AddChild(newchild);
}

void wxMainFrameBase::RemoveChild(wxWindowBase *newchild)
{
    // if the new child is a wxExtMiniFrameBase, we add it to
    // our special list, too.
    // Obviously, here we cannot use IsOneOfExtMiniFrame() because
    // that function works with m_palList...
    if (newchild->IsKindOf(CLASSINFO(wxExtMiniFrame)))
        m_palList.Remove((wxExtMiniFrameBase*)newchild);

    wxFrame::RemoveChild(newchild);
}

void wxMainFrameBase::InitPalContextMenu()
{
    if (m_pPalCtxMenu != NULL)
        return;

    m_pPalCtxMenu = new wxMenu;

    // fill the context menu with the titles of the palettes
    // owned by this frame
    for (int i=0; i < GetPalCount(); i++)
        m_pPalCtxMenu->AppendCheckItem(GetPalMenuBaseId()+i, GetPal(i)->GetTitle());

    // and sync it...
    SyncPalContextMenu();
    wxEXTMF_LOG(wxT("wxMainFrameBase::InitPalContextMenu - created the context menu with %d menu items"), 
                        m_pPalCtxMenu->GetMenuItemCount());
}

void wxMainFrameBase::SyncPalContextMenu()
{
    if (!m_bCtxMenuEnabled)
        return;

    // check show state of each palette and update ctx menu
    for (int i=0; i < GetPalCount(); i++)
        m_pPalCtxMenu->Check(GetPalMenuBaseId()+i, GetPal(i)->IsShown());
}

void wxMainFrameBase::SyncMainMenubar()
{
    wxASSERT(m_bUpdateMainMenubar);

    for (int i=0; i < GetPalCount(); i++) {		

        // update main menu bar (if a menuitem with such an ID is not
        // found, an ASSERT will fail !!!)
        GetMenuBar()->Check(GetPalMenuBaseId()+i, GetPal(i)->IsShown());
    }
}

void wxMainFrameBase::CleanupPalContextMenu()
{
    // just delete the context menu and set it to NULL
    wxDELETE(m_pPalCtxMenu);
}

void wxMainFrameBase::OnActivate(wxActivateEvent &event)
{
    // log this event
    wxEXTMF_LOG(wxT("wxMainFrameBase::OnActivate - status: %d"), event.GetActive());

    if (event.GetActive()) {
        
        // notify all our wxExtMiniFrameBases that we are going
        // to be active...
        for (int i=0; i < GetPalCount(); i++)
            GetPal(i)->SetAsActive();

        wxEXTMF_LOG(wxT("wxMainFrameBase::OnActivate - activated *all* wxExtMiniFrames"));
        
    }
        
    // let other event handlers process this...
}

bool wxMainFrameBase::ProcessEvent(wxEvent &ev)
{
    if (ev.GetEventType() == wxEVT_ACTIVATE)
        OnActivate((wxActivateEvent &)ev);

    if (ev.GetEventType() == wxEVT_UPDATE_UI &&
        ev.GetId() >= GetPalMenuBaseId() &&
        ev.GetId() < GetPalMenuBaseId()+GetPalCount()) {

        if (m_bUpdateMainMenubar) SyncMainMenubar();
        if (m_bCtxMenuEnabled) SyncPalContextMenu();
    }
    
    // check if we received a context menu item event
    if (ev.GetEventType() == wxEVT_COMMAND_MENU_SELECTED &&
        ev.GetId() >= GetPalMenuBaseId() &&
        ev.GetId() < GetPalMenuBaseId()+GetPalCount() &&

        // these two flags can disable the automatic handling...
        m_bCtxMenuEnabled && m_bCtxMenuHandlingEnabled) {
        
        wxEXTMF_LOG(wxT("wxMainFrameBase::ProcessEvent() - going to handle a menu command..."));

        int palid = ev.GetId()-GetPalMenuBaseId();

        // just invert the show state for that palette...
        GetPal(palid)->Show(!GetPal(palid)->IsShown());
        SyncPalContextMenu();
    }

    return wxFrame::ProcessEvent(ev);
}

#endif		// wxEXTMINIFRM_USE_MAINFRAME




// ------------------------------------
// wxEXTMINIFRAMEBASE
// ------------------------------------

#ifdef wxEXTMINIFRM_USE_EXTMINIFRM

wxExtMiniFrameBase::wxExtMiniFrameBase(wxMainFrameBase* parent,
        wxWindowID id,
        const wxString &title,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxString& name)		
{
    Init();

    // just create the window...
    Create(parent, id, title, pos, size, style, name);
}

void wxExtMiniFrameBase::Init()
{
    m_bIsActive = FALSE;
    m_bRolled = FALSE;
    m_pClientWnd = NULL;
    m_bShowCtxMenu = FALSE;
}

bool wxExtMiniFrameBase::Create(wxMainFrameBase* parent,
        wxWindowID id,
        const wxString &title,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxString& name)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::Create - creation started"));

    // remove invalid styles...
    //style &= wxEXTMINIFRM_STYLE_MASK;
    //style |= wxFULL_REPAINT_ON_RESIZE;

    // actually create the window...
    wxFrame::Create(parent, id, title, pos, size, style, name);

    // if we would set auto layout to TRUE, we would get Layout() automatically
    // called each time the window is resized, but then wxFrame::OnSize would
    // not automatically resize the child window...
    SetAutoLayout(FALSE);

    // save window title
    SetTitle(title);
    
#ifdef wxEXTMINIFRM_USE_MINIBTN	
    // add the standard minibuttons required (in RIGHT order !!!)
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::Create - adding minibuttons (if any)"));
    AddMiniButtonFromStyle(style);
#endif

    // set a smaller font for this window
    //SetFont(wxFont(10, wxSWISS, wxNORMAL, wxNORMAL));

    m_pClientWnd = NULL;
    m_bRolled = FALSE;

    // this is one of the few places where m_bIsActive must be
    // modified directly...
    m_bIsActive = TRUE;

    // by default, show the context menu
    m_bShowCtxMenu = TRUE;

    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::Create - ")
        wxT("A wxExtMiniFrameBase has been successfully created..."));

    return TRUE;
}

wxExtMiniFrameBase::~wxExtMiniFrameBase()
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase: destroyed"));
    
#ifdef wxEXTMINIFRM_USE_MINIBTN
    // delete our minibuttons: they wouldn't be deleted automatically
    // because the array is not a wxObjArray and it contains only pointers
    for (int i=0; i < GetButtonCount(); i++)
        delete (wxMiniButtonBase *)m_arrButtons.Item(i);
#endif
    // client should be a child window so it should be deleted automatically;
    // destruction can proceed...
}





// --------------------------------------
// wxExtMiniFrameBase - static utilities
// --------------------------------------

void wxExtMiniFrameBase::DrawSunkenBorders(wxDC &dc, const wxRect &rect)
{
    // draw shades creating the following pattern:
    // LLLLLLLLLLLLLLLLLLLLL
    // LWWWWWWWWWWWWWWWWWWGB		L = light grey
    // LW                 GB		W = white
    // LGGGGGGGGGGGGGGGGGGGB        G = grey
    // LBBBBBBBBBBBBBBBBBBBB        B = black
    dc.SetPen(*wxLIGHT_GREY_PEN);
    dc.DrawLine(rect.x, rect.y, rect.x+rect.width, rect.y);
    dc.DrawLine(rect.x, rect.y, rect.x, rect.y+rect.height);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(rect.x+1, rect.y+1, rect.x+rect.width-1, rect.y+1);
    dc.DrawLine(rect.x+1, rect.y+2, rect.x+1, rect.y+rect.height-1);
    dc.SetPen(*wxGREY_PEN);
    dc.DrawLine(rect.x+rect.width-2, rect.y+1, rect.x+rect.width-2, rect.y+rect.height-1);
    dc.DrawLine(rect.x+1, rect.y+rect.height-2, rect.x+rect.width-2, rect.y+rect.height-2);
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawLine(rect.x, rect.y+rect.height-1, rect.x+rect.width, rect.y+rect.height-1);
    dc.DrawLine(rect.x+rect.width-1, rect.y, rect.x+rect.width-1, rect.y+rect.height);
}

void wxExtMiniFrameBase::DrawCaption(wxExtMiniFrameBase *pwnd, wxDC& dc, const wxRect &rect,
                                    bool /*icon*/, bool active)
{
    wxASSERT_MSG(pwnd, wxT("Cannot call this function without a wxExtMiniFrameBase valid pointer"));

    // eventually find the title height	
    int titleheight = pwnd->GetTitleHeight();
    int bs = pwnd->GetBorderSize();
    
    // this works good... but it's not needed
    // wxBrush backGround(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_BTNFACE), wxSOLID);
    
    // these two following lines seems not to work...
    // int colourflag = (active ? wxSYS_COLOUR_ACTIVECAPTION : wxSYS_COLOUR_INACTIVECAPTION);	
    // wxBrush foreGround(wxSystemSettings::GetSystemColour(colourflag), wxSOLID);
    wxBrush foreGround(wxColour(0, 0, 128), wxSOLID);
    wxCoord w, h;

    // fill the caption bar
    dc.SetBrush(foreGround);
    dc.DrawRectangle(rect.x+bs, rect.y+bs, rect.width-bs*2, titleheight-bs);
    dc.SetFont(pwnd->GetFont());
    dc.GetTextExtent(pwnd->GetTitle(), &w, &h);

    // draw the title on the caption bar
    dc.SetTextForeground(*wxWHITE);
    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.DrawText(pwnd->GetTitle(), rect.x+bs*3, rect.y+bs+(titleheight-h)/2-1);
    
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::DrawCaption - active: %d"), active);
}






// ------------------------------------
// wxExtMiniFrameBase - miscellaneous
// ------------------------------------

int wxExtMiniFrameBase::GetTitleHeight() const
{
    return wxEXTMINIFRM_TITLE_HEIGHT;
}

int wxExtMiniFrameBase::GetBorderSize() const
{
    return wxEXTMINIFRM_BORDER_SIZE;      // see wxExtMiniFrameBase::DrawSunkenBorders
}

#ifdef wxEXTMINIFRM_USE_MINIBTN

void wxExtMiniFrameBase::AddMiniButtonFromStyle(long style)
{
    if (style & wxCLOSE_BOX) {
        AddMiniButton(new wxCloseBox(this));
        wxEXTMF_LOG(wxT("wxExtMiniFrameBase::Create - added a wxCloseBox"));
    }
    
    if (style & wxMAXIMIZE_BOX) {
        AddMiniButton(new wxMaximizeBox(this));
        wxEXTMF_LOG(wxT("wxExtMiniFrameBase::Create - added a wxMaximizeBox"));
    }
    
    if (style & wxMINIMIZE_BOX) {
        AddMiniButton(new wxMinimizeBox(this));
        wxEXTMF_LOG(wxT("wxExtMiniFrameBase::Create - added a wxMinimizeBox"));
    }
}

void wxExtMiniFrameBase::AddMiniButton(wxMiniButtonBase *pBtn)
{
    // add the new minibutton to our list
    m_arrButtons.Add(pBtn);

    // and relayout the minibuttons...
    MoveMiniButtons();
}

wxPoint wxExtMiniFrameBase::GetButtonPos(int i) const
{
    if (i >= GetButtonCount())
        return wxPoint(-1, -1);

    int offset = GetBorderSize()*2;
    int y = GetBorderSize()*2;

    // get the width of all the previous buttons
    for (int c=0; c <= i; c++)
        offset += GetButton(i)->GetSize().GetWidth()+wxEXTMINIFRM_BTN_GAP;
    
#ifdef __WXX11__
    y += 2;		// a little correction to make them better placed
#endif

    return wxPoint(GetSize().GetWidth()-offset, y);
}

#endif		// wxEXTMINIFRM_USE_MINIBTN

void wxExtMiniFrameBase::SetAsActive(bool bNewState)
{
    bool old = m_bIsActive;
    m_bIsActive = bNewState;

    // avoid flickering...
    if (old != bNewState) Draw();
}

wxExtMiniFrameHitCode wxExtMiniFrameBase::HitTest(const wxPoint &pos)
{
    int xPos = pos.x; 
    int yPos = pos.y; 
    
    wxSize sz = GetSize();
    int bs = GetBorderSize()*2;			// this *2 make resizing easier
    int h = sz.GetHeight(), w = sz.GetWidth();
    
    // clip points which are outside window's area
    if (xPos < 0 || yPos < 0 || yPos > h || xPos > w)
        return wxPFHT_TOPLEVEL_NOWHERE;
    
    // check for borders, if we are not rolled-up:
    // if we return one of the border-hit flag, win32 will automatically
    // add resizing support for this window but if we are rolled-up, we
    // cannot be resized...
    if (!IsRolled()) {
        if (yPos >= bs && yPos <= h-bs) {
            
            if (xPos <= bs) return wxPFHT_TOPLEVEL_BORDER_W;
            if (xPos >= w-bs) return wxPFHT_TOPLEVEL_BORDER_E;
            
        } else if (yPos <= bs) {
            
            if (xPos <= bs) return wxPFHT_TOPLEVEL_BORDER_NW;
            if (xPos >= w-bs) return wxPFHT_TOPLEVEL_BORDER_NE;
            return wxPFHT_TOPLEVEL_BORDER_N;
            
        } else if (yPos >= h-bs) {
            
            if (xPos <= bs) return wxPFHT_TOPLEVEL_BORDER_SW;
            if (xPos >= w-bs) return wxPFHT_TOPLEVEL_BORDER_SE;
            return wxPFHT_TOPLEVEL_BORDER_S;
        }
    }
    
    // check for the caption rect
    if (xPos > bs && xPos < w-bs && yPos > bs && yPos < bs+GetTitleHeight())
        return wxPFHT_TOPLEVEL_TITLEBAR;

    // the point must be in the client area...
    return wxPFHT_TOPLEVEL_CLIENT_AREA;
}






// --------------------------------------
// wxExtMiniFrameBase - layout load/save
// --------------------------------------

void wxExtMiniFrameBase::SaveLayout(wxConfigBase *p, const wxString &keypath) const
{
    wxString keyname(keypath);
    if (keyname == wxEmptyString)
        keyname = GetTitle();

    // ... and then write it in the given config object....
    p->Write(keyname, EncodeLayout());
}

bool wxExtMiniFrameBase::LoadLayout(wxConfigBase *p, const wxString &keypath)
{
    wxString keyname(keypath);
    if (keyname == wxEmptyString)
        keyname = GetTitle();

    // ... and then write it in the given config object....
    wxString todecode = p->Read(keyname, wxEmptyString);
    if (todecode != wxEmptyString) 
        DecodeLayout(todecode);
    else
        return FALSE;	// retain current layout...
    return TRUE;
}

wxString wxExtMiniFrameBase::EncodeLayout() const
{
    // encode our layout in a wxString in this way:
    //
    //                 "x;y cx;cy rollstate showstate"
    // where
    //          x;y = position of the window in screen coords
    //          cx;cy = size of the unrolled window 
    //          rollstate = 1 or 0 for m_bRolled == TRUE or FALSE
    //          showstate = 1 or 0 if the window is visible/hidden
    //
    // are all separed with a simple space

    return wxString::Format(wxT("%d;%d %d;%d %d %d"), 
        GetPosition().x, GetPosition().y,
        GetUnRolledSize().GetWidth(), GetUnRolledSize().GetHeight(), 
        IsRolled(),
        IsShown());
}

void wxExtMiniFrameBase::DecodeLayout(const wxString &str)
{
    // see wxExtMiniFrameBase::EncodeLayout for more info about the
    // encoding format of the given string...

    // read the string tokens
    wxStringTokenizer tknzr(str, wxT(" "));
    wxASSERT_MSG(tknzr.CountTokens() == 4, wxT(""));
    wxString pos = tknzr.GetNextToken();
    wxString size = tknzr.GetNextToken();
    wxString rollstate = tknzr.GetNextToken();
    wxString showstate = tknzr.GetNextToken();

    // read pos & size
    unsigned long x, y, cx, cy;
    pos.BeforeFirst(wxT(';')).ToULong(&x);
    pos.AfterFirst(wxT(';')).ToULong(&y);
    size.BeforeFirst(wxT(';')).ToULong(&cx);
    size.AfterFirst(wxT(';')).ToULong(&cy);

    // now, modify the window to actually apply the just decoded layout
    Move(x, y);

    cx = ((int)cx > GetMinWidth()) ? cx : GetMinWidth();
    cy = ((int)cy > GetMinHeight()) ? cy : GetMinHeight();
    SetSize(cx, cy);

    // roll/unroll this window
    if (rollstate.Last() == wxT('1'))
        Roll();
    else if (rollstate.Last() == wxT('0'))
        UnRoll();

    // show/hide this window
    if (showstate.Last() == wxT('1'))
        Show();
    else if (showstate.Last() == wxT('0'))
        Hide();
}






// ----------------------------------------
// wxExtMiniFrameBase - wxWindow overrides
// ----------------------------------------

bool wxExtMiniFrameBase::ProcessEvent(wxEvent &event)
{	
    int type = event.GetEventType();

    // Event pre-processing
    // ---------------------

    if (type == wxEVT_SIZE)
        OnSize((wxSizeEvent &)event);
        
#ifndef __WXMSW__

    // on wxMSW the wxExtMiniFrame::MSWWindowProc will call the
    // mouse event handlers

    if (type == wxEVT_LEFT_DOWN)
        OnLeftDown((wxMouseEvent &)event);
    if (type == wxEVT_LEFT_UP)
        OnLeftUp((wxMouseEvent &)event);
        
    if (type == wxEVT_RIGHT_DOWN)
        OnRightDown((wxMouseEvent &)event);
    if (type == wxEVT_RIGHT_UP)
        OnRightUp((wxMouseEvent &)event);
                
    if (type == wxEVT_MOTION)
        OnMotion((wxMouseEvent &)event);

#endif

    // process the event as usual
    bool b = wxFrame::ProcessEvent(event);


    // Event post-processing
    // ----------------------

#ifndef __WXGTK__
    // these events must be intercepted even on win32
    // (that is, even if we are already intercepting them with
    // MSWWindowProc....)
    if (type == wxEVT_ACTIVATE)
        OnActivate((wxActivateEvent &)event);

    if (type == wxEVT_CLOSE_WINDOW)
        OnClose((wxCloseEvent &)event);

    // postprocess those events which are required by our system...
    if (type == wxEVT_PAINT ||
        type == wxEVT_NC_PAINT || 
        type == wxEVT_CHILD_FOCUS)
        OnPaint((wxPaintEvent &)event);		// we could call directly Draw()...	
#endif

    return b;
}

bool wxExtMiniFrameBase::Destroy()
{
    wxEXTMF_LOG(wxT("wxExtMiniFrame::Destroy"));
    
    // copied from wxTopLevelWindowBase::Destroy()
    // because this class is like wxFrame and thus it needs to have a
    // delayed destruction.
    //
    // NOTE: the wxMainFrameBase::RemoveChild() would be called automatically
    //       by wxWindowBase::~wxWindowBase but we need to call it here
    //       because if we let wxWindowBase do it, wxMainFrameBase::RemoveChild
    //       will be called with a pointer to a partially removed window:
    //       wxMainFrameBase would not recognize that the given pointer is
    //       a wxExtMiniFrameBase...

    // delayed destruction: the frame will be deleted during the next idle
    // loop iteration
    if ( !wxPendingDelete.Member(this) )
        wxPendingDelete.Append(this);

    // but hide it immediately
    Hide();

    // see above
    GetParent()->RemoveChild(this);

    return TRUE;
}

wxPoint wxExtMiniFrameBase::ClientToFrame(const wxPoint &pt) const
{
    wxPoint newpt(pt);

    // adjust the point coordinates with the caption sizes
    newpt.x += GetBorderSize();
    newpt.y += GetTitleHeight()+GetBorderSize();

    return newpt;
}

wxPoint wxExtMiniFrameBase::ScreenToFrame(const wxPoint &pos) const
{
    // just transform to frame coords: don't subtract border sizes
    return wxPoint(pos.x-GetPosition().x, pos.y-GetPosition().y);
}

#ifndef __WXGTK__

wxPoint wxExtMiniFrameBase::GetClientAreaOrigin() const
{
    // this is not a common wxWindow but a wxFrame-like window
    return wxPoint(GetBorderSize(), GetBorderSize()+GetTitleHeight());
}

void wxExtMiniFrameBase::DoGetClientSize(int *width, int *height) const
{
    wxSize sz = GetSize();
    wxPoint pt = GetClientAreaOrigin();
#ifdef __WXGTK__
    int bs = 0;
#else
    int bs = GetBorderSize();
#endif
    
    if (width) *width = sz.x - pt.x - bs;
    if (height) *height = sz.y - pt.y - bs;
}

void wxExtMiniFrameBase::DoSetClientSize(int width, int height)
{
    // call GetClientAreaOrigin() to take the caption bar in count
    wxPoint pt = GetClientAreaOrigin();
    width += pt.x;
    height += pt.y;
    
    wxFrame::DoSetClientSize(width, height);
}

#endif


#ifdef wxEXTMINIFRM_USE_MINIBTN

void wxExtMiniFrameBase::MoveMiniButtons()
{
#ifdef __WXDEBUG__
    wxString str = wxT("wxExtMiniFrameBase::MoveMiniButtons - ");
    if (GetButtonCount() > 0)
        str += wxString::Format(wxT("first is at %d;%d"), GetButtonPos(0).x, GetButtonPos(0).y);
    else
        str += wxT("no minibuttons in this palette...");
    wxEXTMF_LOG(str);
#endif

    // repositionate all the buttons
    for (int i=0; i < GetButtonCount(); i++)
        GetButton(i)->Move(GetButtonPos(i));
}

#endif		// wxEXTMINIFRM_USE_MINIBTN

bool wxExtMiniFrameBase::Layout()
{
    // under x11 this function is called a LOT of times...
    //MoveMiniButtons();
    
    // don't forget to call base class' implementation
    return wxFrame::Layout();
}

void wxExtMiniFrameBase::Update()
{
    //MoveMiniButtons();

    // for some reason, we must force a call to wxFrame::OnSize
    //SendSizeEvent();

    // don't forget to call base class' implementation
    wxFrame::Update();
}





// -------------------------------------
// wxExtMiniFrameBase - event handlers
// -------------------------------------

void wxExtMiniFrameBase::OnCloseBox(wxCommandEvent &)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::OnCloseBox()"));

    // just hide the owner of this minibutton...
    Hide();

    // ...instead of closing it because this is
    // faster and because, if the user re-shows the parent
    // window in this same session, he will probably expect
    // to see untouched all the data he left: so, if we'll
    // close it, we would have to save it... better hide !!!
    // Close(TRUE);
}

void wxExtMiniFrameBase::OnCollapseBox(wxCommandEvent &)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::OnCollapseBox()"));
    
    // our parent is a wxExtMiniFrame !!!
    if (!IsRolled())
        Roll();
    else
        UnRoll();
}

void wxExtMiniFrameBase::OnActivate(wxActivateEvent &ev)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::OnActivate [%d]: status: %d"), 
        GetHandle(), ev.GetActive());
    
    if (!ev.GetActive()) {
        
        // just refresh...
        Draw();
    }
}

void wxExtMiniFrameBase::OnClose(wxCloseEvent &)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::OnClose()"));
    
    // just destroy this window: this class never vetoes
    // the closure...
    Destroy();
}

void wxExtMiniFrameBase::OnPaint(wxPaintEvent &)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::OnPaint"));
    
    // get the caption area rect
    wxRect captionarea(GetBorderSize(), GetBorderSize(), 
                    GetSize().GetWidth(), GetTitleHeight());

#ifdef wxEXTMINIFRM_USE_MINIBTN
    // get the minibtn area rect
    const int firstx = GetButtonPos(0).x-5;
    const int lastx = GetButtonPos(GetButtonCount()-1).x-5;
    int x = lastx > firstx ? firstx : lastx;
    if (x <= 0) x = GetBorderSize();
    wxRect minibtnarea(x, GetBorderSize(), GetSize().GetWidth(), GetTitleHeight());
#endif

    bool drawcaption = FALSE;
    bool drawminibtn = FALSE;
    wxRegionIterator upd(GetUpdateRegion()); // get the update rect list
    while (upd)
    {
        wxRect rect(upd.GetRect());

        // avoid flickering when possible...
        if (rect.Intersects(captionarea))
            drawcaption = TRUE;
#ifdef wxEXTMINIFRM_USE_MINIBTN
        if (rect.Intersects(minibtnarea))
            drawminibtn = TRUE;
#endif
            
        upd++;
    }

    
    // draw the palette using the virtual function: that is,
    // derived class can override palette drawing
    Draw(NULL, drawcaption, drawminibtn);
}

#ifdef wxEXTMINIFRM_USE_MINIBTN

void wxExtMiniFrameBase::DrawMiniButtons(wxDC &dc)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::DrawMiniButtons"));	
    
    // draw all the minibuttons
    for (int i = 0; i < GetButtonCount(); ++i)
        GetButton(i)->Draw(dc);
}

#endif

void wxExtMiniFrameBase::OnSize(wxSizeEvent &)
{
    // let the base class process this event (like wxFrame, we must
    // perform a special automatic behaviour on sizing when a wxPalettFrame
    // contains one child only)...
    /*if (GetChildren().GetCount() == 1) {

        // the window which must be resized should be the client window
        // set throught the #SetClient function...
        wxWindow *thechild = GetChildren().Item(0)->GetData();

        // sending an event to the window would break sizing system;
        // don't do this:
        // 
        // wxSizeEvent ev(GetClientSize(), thechild->GetId());
        // thechild->ProcessEvent(ev);		
        thechild->SetSize(GetClientSize());		
    }*/

#ifdef wxEXTMINIFRM_USE_MINIBTN	
    // if we are resizing the window horizontally, then we
    // need to update minibtns;
    // HOW CAN I KNOW IF USER IS RESIZING HORIZONTALLY ?
    
    // re positionate mini buttons
    MoveMiniButtons();
#endif
    
#ifdef __WXMSW__
    Draw();
#endif
}

void wxExtMiniFrameBase::OnLeftDown(wxMouseEvent &ev)
{ OnPosLeftDown(ev.GetPosition()); }

void wxExtMiniFrameBase::OnLeftUp(wxMouseEvent &ev)
{ OnPosLeftUp(ev.GetPosition()); }

void wxExtMiniFrameBase::OnRightDown(wxMouseEvent &ev)
{ OnPosRightDown(ev.GetPosition()); }

void wxExtMiniFrameBase::OnRightUp(wxMouseEvent &ev)
{ OnPosRightUp(ev.GetPosition()); }

bool wxExtMiniFrameBase::OnPosLeftDown(const wxPoint &pos)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::OnLeftDown()"));

#ifdef wxEXTMINIFRM_USE_MINIBTN	
    wxPoint tmp = pos;
#ifndef __WXX11__
    tmp = ClientToFrame(tmp);
#endif

    // first of all, check if the user clicked on a minibutton...
    for (int i = 0; i < GetButtonCount(); i++) {
        if (GetButton(i)->OnLeftDown(tmp))
            return TRUE; // button hitted...
    }
#endif
    
    return FALSE;
}

bool wxExtMiniFrameBase::OnPosLeftUp(const wxPoint &pos)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::OnLeftUp()"));
    
#ifdef wxEXTMINIFRM_USE_MINIBTN
    wxPoint tmp = pos;
#ifndef __WXX11__	
    tmp = ClientToFrame(tmp);
#endif

    // first of all, check if the user clicked on a minibutton...
    for (int i = 0; i < GetButtonCount(); i++) {
        if (GetButton(i)->IsBeingPressed()) {

            GetButton(i)->OnLeftUp(tmp);
            return TRUE; // button hitted...
        }
    }
#endif

    return FALSE;
}

bool wxExtMiniFrameBase::OnPosRightDown(const wxPoint &)
{
    // default implementation does nothing

    return TRUE;
}

bool wxExtMiniFrameBase::OnPosRightUp(const wxPoint &pos)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameBase::OnPosRightUp - handling a click at [%d;%d] - showctxmenu: %d"), 
                    pos.x, pos.y, m_bShowCtxMenu);
    
#ifdef wxEXTMINIFRM_USE_MAINFRAME
    wxMainFrameBase *parent = (wxMainFrameBase *)GetParent();
#endif

    // if this window has context menu disabled...
    if (!m_bShowCtxMenu 
#ifdef wxEXTMINIFRM_USE_MAINFRAME
            || !parent->isContextMenuEnabled()
#endif
        ) {
        
        wxEXTMF_LOG(wxT("wxExtMiniFrameBase::OnPosRightUp - The context menu won't be shown..."));
        return TRUE;
    }
        
#ifdef wxEXTMINIFRM_USE_MAINFRAME
    wxPoint tmp = pos;
    wxMenu *ctx = parent->GetPalContextMenu();

    wxASSERT_MSG(ctx, wxT("This palette frame has the wxEXTMINIFRM_SHOW_CONTEXT_MENU ")
                    wxT("style set but the main frame doesn't provide a valid context menu..."));

#ifdef __WXMSW__
    tmp.x -= GetPosition().x;
    tmp.y -= GetPosition().y-GetClientAreaOrigin().y;
#else
    tmp.y -= GetTitleHeight();
#endif

    PopupMenu(ctx, tmp);
#endif						// wxEXTMINIFRM_USE_MAINFRAME	
    
    // don't delete the menu just popped up: the wxMainFrameBase
    // could decide to reuse it...

    return TRUE;
}







// -------------------------------------
// wxExtMiniFrameBase - client handlers
// -------------------------------------

wxWindow *wxExtMiniFrameBase::CreateClient(long style)
{
    SetClient(new wxPanel(this, -1, wxDefaultPosition,
        wxDefaultSize, style | wxNO_FULL_REPAINT_ON_RESIZE,
        wxT("wxExtMiniFrameBase client")));	

    return GetClient();
}

void wxExtMiniFrameBase::SetClient(wxWindow *pWnd)
{
    wxASSERT_MSG(pWnd != NULL && pWnd != m_pClientWnd,
        wxT("Invalid client window !!"));

    wxDELETE(m_pClientWnd);		// delete old one
    m_pClientWnd = pWnd;

    // VERY IMPORTANT: this is required for the client window to be shown
    m_pClientWnd->Move(0, 0);
}

void wxExtMiniFrameBase::SetSizeHintsUsingClientHints()
{
    // this function will resize this window taking in count everything:
    // borders, title and obviously the child (called by wxExtMiniFrameBase's
    // function as "client") window...
    Fit();
    
    // now that the har dowk has been done by Fit(), we can set the
    // size hints to the current size...
    wxSize size = GetSize();
    SetSizeHints(size.GetWidth(), size.GetHeight());
}






// ---------------------------------
// wxExtMiniFrameBase - Roll/Unroll
// ---------------------------------

void wxExtMiniFrameBase::Roll()
{
    if (IsRolled())
        return;		// already rolled

    // save the current size (it will be restored in UnRoll)
    m_szLast = GetClientSize();
    m_szLastHints = wxSize(m_minWidth, m_minHeight);

    // hide the client window and change our size
    SetSizeHints(0, 0);
    GetClient()->Hide();

    wxSize newsz = GetRolledSize();
    SetSize(newsz);
    m_bRolled = TRUE;

    // we cannot remove the wxRESIZE_BORDER but the OnHitTest function
    // will care to make the user unable to resize this window when
    // we are rolled up...
    //
    // something like, would cause some problems of child-positionining:
    // SetWindowStyle(GetWindowStyle() & ~wxRESIZE_BORDER | wxSUNKEN_BORDER);
#ifdef wxEXTMINIFRM_USE_MINIBTN
    // we need to move buttons in the right place...
    MoveMiniButtons();
#endif
    
    // this frame window must be refreshed...
    this->Refresh();
}

void wxExtMiniFrameBase::UnRoll()
{
    if (!IsRolled())
        return;		// already unrolled

    // restore conditions previous to Roll()
    GetClient()->Show();
    SetSizeHints(m_szLastHints.GetWidth(), m_szLastHints.GetHeight());
    SetClientSize(GetUnRolledSize());

    // send a size event
    wxSizeEvent ev(GetUnRolledSize(), GetId());
    ProcessEvent(ev);
    
    m_bRolled = FALSE;		// we are unrolled, now
}

wxSize wxExtMiniFrameBase::GetRolledSize() const
{
    wxScreenDC dc;
    int titlelenght;

    // get title lenght
    dc.SetFont(GetFont());
    dc.GetTextExtent(GetTitle(), &titlelenght, NULL);

    // and add it some space
    int bs = GetBorderSize()*2;
    return wxSize(titlelenght+60+bs, GetTitleHeight()+bs+2);
}

wxSize wxExtMiniFrameBase::GetUnRolledSize() const
{
    // if we are currently rolled, the unrolled size should
    // have been saved in m_szLast...
    if (IsRolled())
        return m_szLast;

    // we are unrolled: unrolled size == window size
    return GetSize();
}

void wxExtMiniFrameBase::Draw(wxWindowDC *dc, bool bDrawCaption, bool bDrawMiniBtn)
{
    bool clean = FALSE;
    
    // a wxWindowDC allow us to draw also the decorations (like the
    // caption of a window) under Win32...
    if (dc == NULL) {
        dc = new wxWindowDC(this);
        clean = TRUE;
    }

    int w, h, bw=GetBorderSize(), bh=bw, th=GetTitleHeight();
    GetSize(&w, &h);
    
    wxRect rc(bw, bh, w-bw*2, th);

    // draw this frame
    DrawSunkenBorders(*dc, wxRect(0, 0, w, h));

#ifdef wxEXTMINIFRM_USE_MINIBTN

    // redraw the left part of the caption bar only when we want to
    // do a fast redraw (this way, we can avoid to redraw minibtn
    // and have a performance gain)
#ifdef wxEXTMINIFRM_FASTREDRAW
    if (!bDrawMiniBtn)
        rc.width = GetButtonPos(GetButtonCount()-1).x-5;
#endif
#endif

    if (bDrawCaption)
        DrawCaption(this, *dc, rc, TRUE, IsToDrawAsActive());

#ifdef wxEXTMINIFRM_USE_MINIBTN		
#ifdef wxEXTMINIFRM_FASTREDRAW
    // draw also our minibuttons if caller has requested so...
    if (bDrawMiniBtn)
        DrawMiniButtons(*dc);
#else
    { bDrawMiniBtn = bDrawMiniBtn; }		// just to avoid warnings
    if (bDrawCaption)
        DrawMiniButtons(*dc);
#endif
#endif
    
    if (clean) delete dc;
}

#endif		// wxEXTMINIFRM_USE_EXTMINIFRM
                        



// ------------------------------------
// wxMINIBUTTONBASE
// ------------------------------------

#ifdef wxEXTMINIFRM_USE_MINIBTN

wxMiniButtonBase::wxMiniButtonBase(wxExtMiniFrameBase *parent, int id)
{
    Init();

    wxASSERT_MSG(parent, wxT("Cannot create a minibutton without a parent"));
    m_pParent = parent;

    // save the ID of this minibutton: it will be used to generate
    // the wxCommandEvents when the button is clicked...
    m_nId = id;	

    // this must be changed as soon as possible
    // (wxExtMiniFrameBase::AddMiniButton automatically calls the
    // wxExtMiniFrameBase::Layout function which repositionates all
    // the minibuttons)...
    m_ptPos = wxDefaultPosition;
}

void wxMiniButtonBase::Init()
{
    // some defaults
    m_bPressed = FALSE;
    m_bCapturing = FALSE;
    m_bEnabled = TRUE;
    m_bVisible = TRUE;

    m_nId = -1;
    m_pParent = NULL;
}

void wxMiniButtonBase::Draw(wxDC &dc)
{
    //wxExtMiniFrameBase::DrawSunkenBorder(dc, wxRect(m_ptPos.x, m_ptPos.y, wxMINIBTN_WIDTH, wxMINIBTN_HEIGHT), m_bPressed);
    Draw(dc, m_ptPos.x, m_ptPos.y);//+wxMINIBTN_BORDER_SIZE);//+(int)m_bPressed);
}

bool wxMiniButtonBase::HitTest(const wxPoint& pos)
{
    // check if the given point is in the rectangle of this minibutton
    return (pos.x >= m_ptPos.x && pos.y >= m_ptPos.y &&
        pos.x <= m_ptPos.x + GetSize().GetWidth() &&
        pos.y <= m_ptPos.y + GetSize().GetHeight());
}

bool wxMiniButtonBase::OnLeftDown(const wxPoint& pos)
{
    // have we already captured the mouse ? are we invisible ?
    if (!m_bVisible || m_bCapturing) return FALSE;	// then don't process this msg

    // if the user clicks on us and we are enabled...
    if (HitTest(pos) && m_bEnabled)
    {
        // capture the mouse
        m_pParent->CaptureMouse();
        m_bCapturing = TRUE;

        // we are being pressed
        m_bPressed = TRUE;
        wxEXTMF_LOG(wxT("wxMiniButtonBase::OnLeftDown - A minibutton is being pressed (%d;%d)"), pos.x, pos.y);

        // redraw ourselves...
        Refresh();
        return TRUE;
    }

    wxEXTMF_LOG(wxT("wxMiniButtonBase::OnLeftDown - this one was not clicked (%d;%d)"), pos.x, pos.y);
    
    return FALSE;
}

bool wxMiniButtonBase::OnLeftUp(const wxPoint &pos)
{
    if (!m_bVisible) return FALSE;

    // do we captured the mouse ?
    if (m_bCapturing && m_pParent->HasCapture()) {

        // yes... release it also if the click was not on our button...
        m_pParent->ReleaseMouse();
    }

    // we are not being pressed anymore...
    m_bCapturing = FALSE;
    m_bPressed = FALSE;
    wxEXTMF_LOG(wxT("wxMiniButtonBase::OnLeftUp - A minibutton has been released (%d;%d)"), pos.x, pos.y);

    // redraw ourselves...
    Refresh();

    // check if we must trigger the action attached to this button
    if (HitTest(pos) && m_bEnabled) {
    
        //wxEXTMF_LOG("wxMiniButtonBase::OnPosLeftUp - sending a wxEVT_COMMAND_MINIBTN_CLICKED event");
        
        //wxCommandEvent ce(wxEVT_COMMAND_MINIBTN_CLICKED, GetId());
        //GetParent()->AddPendingEvent(ce);
        Exec();
    }
    return TRUE;
}

wxSize wxMiniButtonBase::GetSize() const
{
    int w=-1,h=-1;
    
    // adaptive height & width
    h = (int)(m_pParent->GetTitleHeight()*wxEXTMINIFRM_BTN_HEIGHT_MULT);
    w = (int)(h*wxEXTMINIFRM_BTN_RATIO);

    return wxSize(w, h);
}

void wxMiniButtonBase::Refresh()
{
    wxWindowDC dc(m_pParent);
    Draw(dc);
}

void wxMiniButtonBase::DrawButtonFrame(wxDC &dc, int x, int y)
{
    wxRect rc(wxPoint(x, y), GetSize());
    wxExtMiniFrameBase::DrawSunkenBorders(dc, rc);
    
    // fill the interior
    if (!m_bPressed)
        dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    else
        dc.SetBrush(*wxGREY_BRUSH);
    dc.SetPen(*wxTRANSPARENT_PEN);
    rc.Inflate(-2, -2);
    //rc.Offset(1, 1);
    dc.DrawRectangle(rc);
}

int wxMiniButtonBase::GetBorderSize() const
{
    // we used wxExtMiniFrameBase::DrawSunkenBorders in DrawButtonFrame...
    return m_pParent->GetBorderSize();
}

#endif		// wxEXTMINIFRM_USE_MINIBTN


