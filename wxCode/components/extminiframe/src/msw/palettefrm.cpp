/////////////////////////////////////////////////////////////////////////////
// Name:        msw/palettefrm.cpp
// Purpose:     wxExtMiniFrame, wxExtMiniFrame, wxMiniButton
//              wxCloseBox, wxMaximizeBox, wxMinimizeBox, wxCollapseBox
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
#endif


// includes
#include <wx/wx.h>
#include <wx/settings.h>
#include <wx/tokenzr.h>
#include <wx/defs.h>
#include <wx/object.h>
#include <wx/frame.h>
#include <wx/dcscreen.h>

#include "wx/palettefrm.h"
#include "wx/minibtn.h"


// this is required for the wxGet/SetWindowUserData
#include <wx/msw/private.h>

// used in the ::DrawCaption Win32 API...
#ifndef DC_GRADIENT
#define DC_GRADIENT         0x0020
#endif


// RTTI class declarations
#ifdef wxEXTMINIFRM_USE_EXTMINIFRM
IMPLEMENT_DYNAMIC_CLASS(wxExtMiniFrame, wxFrame)
#endif

#ifdef wxEXTMINIFRM_USE_MINIBTN
IMPLEMENT_ABSTRACT_CLASS(wxMiniButton, wxObject)
#endif

#ifdef wxEXTMINIFRM_USE_MAINFRAME
IMPLEMENT_DYNAMIC_CLASS(wxMainFrame, wxFrame)
#endif








// ------------------------------------
// wxMAINFRAME
// ------------------------------------

#ifdef wxEXTMINIFRM_USE_MAINFRAME

void wxMainFrame::OnActivate(wxActivateEvent &event)
{
    // log this event
    wxEXTMF_LOG(wxT("wxMainFrame::OnActivate - status: %d"), event.GetActive());

    if (event.GetActive()) {
        
        // let the base version handle this
        wxMainFrameBase::OnActivate(event);		
    }

    // let other event handlers process this...
}

bool wxMainFrame::MSWOnNcActivate(bool bGoingToBeActive, WXHWND hwnd)
{
    // log this event
    wxEXTMF_LOG(wxT("wxMainFrame::MSWOnNcActivate - bGoingToBeActive = %d"), 
        bGoingToBeActive);

    if (!bGoingToBeActive) {
        
        // scan the palettes
        for (int i=0; i < GetPalCount(); i++) {
            wxExtMiniFrame *p = (wxExtMiniFrame *)GetPal(i);
            
            if (p->GetHandle() == (WXWidget)hwnd) {

                wxEXTMF_LOG(wxT("wxMainFrame::MSWOnNcActivate - ")
                    wxT("A mainframe's extminiframe is being ACTIVATED..."));

                // the caller must not allow this message to be processed
                // by DefWindowProc: the title bar of this window must
                // *not* be updated !!!
                return TRUE;
            }
        }
    }

    // let DefWindowProc to handle this msg
    return FALSE;
}

bool wxMainFrame::MSWOnActivate(bool bGoingToBeActive, WXHWND hwnd)
{
    // log this event
    wxEXTMF_LOG(wxT("wxMainFrame::MSWOnActivate - bGoingToBeActive = %d"), 
        bGoingToBeActive);

    // if this window is being deactived and the window which
    // is going to be active, is not one of our wxExtMiniFrame...
    if (!bGoingToBeActive && !IsOneOfExtMiniFrame((WXWidget)hwnd)) {
        
        // notify all wxExtMiniFrames that we are not active any longer...			
        for (int i=0; i < GetPalCount(); i++)
            GetPal(i)->SetAsInactive();

        wxEXTMF_LOG(wxT("wxMainFrame::MSWOnActivate - deactivated *all* miniframes"));
        //Refresh();
    }
    
    // let DefWindowProc to handle this msg
    return FALSE;
}

long wxMainFrame::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    // intercept some messages...
    switch (nMsg) {
    case WM_NCACTIVATE:

        // check if we must allow DefWindowProc to process this msg ...
        if (MSWOnNcActivate(wParam != 0, (WXHWND)lParam))
            return TRUE;

        // yes, the msg must be processed...
        break;

    case WM_ACTIVATE:

        // this window is changing its activation state...
        MSWOnActivate((LOWORD(wParam) != 0), (WXHWND)lParam);		
        break;
    }

    // let base class do its processing...
    return wxFrame::MSWWindowProc(nMsg, wParam, lParam);
}

#endif		// wxEXTMINIFRM_USE_MAINFRAME




#ifdef wxEXTMINIFRM_USE_EXTMINIFRM

// ------------------------------------
// wxEXTMINIFRAME
// ------------------------------------

wxExtMiniFrame::wxExtMiniFrame(wxMainFrame* parent,
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

bool wxExtMiniFrame::Create(wxMainFrame *parent,
        wxWindowID id,
        const wxString &title,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxString& name)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrame::Create - creation started"));

    // actually create the window...
    wxExtMiniFrameBase::Create(parent, id, title, pos, size, style, name);

    // set this marker to allow wxExtMiniFrame::IsExtMiniFrame to be
    // able to identify wxExtMiniFrame just using an WXHWND
    int data = wxEXTMINIFRM_MARKER;
#if wxCHECK_VERSION(2, 5, 1)
    wxSetWindowUserData((HWND)GetHandle(), &data);
#else
    ::SetWindowLong((HWND)GetHandle(), GWL_USERDATA, (long)(&data));
#endif

    // NEVER SET A DIFFERENT FONT FOR THIS WINDOW !!!!!!!
    // THIS WOULD LEAD TO A PROBLEM WITH UNICODE CHARACTERS SHOWN NOT ONLY IN
    // THE WINDOW TITLE BUT ALSO IN THE WINDOW'S CHILDREN !!!
    // set a smaller font for this window
    // just to simulate msDev style
    //SetFont(wxFont(8, wxSWISS, wxNORMAL, wxNORMAL, FALSE, wxT("MS Sans Serif")));

    wxEXTMF_LOG(wxT("wxExtMiniFrame::Create - ")
        wxT("A wxExtMiniFrame has been successfully created..."));

    return TRUE;
}

int wxExtMiniFrame::GetTitleHeight() const
{
    if (HasFlag(wxFRAME_TOOL_WINDOW))
        return GetSystemMetrics(SM_CYSMCAPTION);//-GetBorderSize();
    return GetSystemMetrics(SM_CYCAPTION);//-GetBorderSize();
}

int wxExtMiniFrame::GetBorderSize() const
{
    if (HasFlag(wxRESIZE_BORDER) ||
        HasFlag(wxSUNKEN_BORDER))
        return 2;
    return 1;
}

bool wxExtMiniFrame::IsExtMiniFrame(WXHWND win)
{
    // something like:
    //
    // wxChar buf1[64], buf2[64];
    // GetClassName((WXHWND)GetHandle(), buf1, 64);
    // GetClassName(win, buf2, 64);
    // return !(strcmp(buf1, buf2));
    //
    // doesn't work because the class names can be different if the
    // wxFULL_REPAINT_ON_RESIZE flag is present in one window and
    // not in the other...

    // see wxExtMiniFrame::Create() for more info	
#if wxCHECK_VERSION(2, 5, 1)
    int n = (int)wxGetWindowUserData((HWND)win);
#else
    int n = ::GetWindowLong((HWND)win, GWL_USERDATA);
#endif
    return (n & wxEXTMINIFRM_MARKER) != 0;

}

void wxExtMiniFrame::MSWInitNCMouseEvent(wxMouseEvent &ev, WXLPARAM lParam, bool transform)
{
    POINTS pts = MAKEPOINTS(lParam);
    wxPoint tmp(pts.x, pts.y);
    if (transform) tmp = ScreenToFrame(tmp);

    // this one is included in wxMSW port
    InitMouseEvent(ev, tmp.x, tmp.y, NULL);
}

void wxExtMiniFrame::SendSizeEvent()
{
    RECT r = wxGetWindowRect(GetHwnd());
    
    (void)::PostMessage(GetHwnd(), WM_SIZE,
        SIZE_RESTORED,
        MAKELPARAM(r.right - r.left, r.bottom - r.top));
}




// ------------------------------------
// wxEXTMINIFRAME - static utilities
// ------------------------------------

void wxExtMiniFrame::DrawCaption(wxExtMiniFrameBase *pwnd, wxDC& dc, const wxRect &rect,
                            bool icon, bool active)
{
    wxASSERT_MSG(pwnd, wxT("Cannot call this function without a wxExtMiniFrame valid pointer"));

    RECT rc;
    rc.left = rect.x;
    rc.top = rect.y;
    rc.right = rect.x+rect.width;
    rc.bottom = rect.y+rect.height;

    int flags = 
#ifndef wxEXTMINIFRM_FASTREDRAW
        DC_GRADIENT | 
#endif
        DC_TEXT | DC_SMALLCAP;
    if (icon) flags |= DC_ICON;
    if (active) flags |= DC_ACTIVE;

    // the icon will be drawn only if:
    // 1) "icon" is TRUE
    // 2) the window has a small icon associated
    ::DrawCaption((HWND)pwnd->GetHWND(), (HDC)dc.GetHDC(), &rc, flags);
    
    // log
    wxEXTMF_LOG(wxT("wxExtMiniFrame::DrawCaption - active: %d"), active);
}






// ------------------------------------
// wxEXTMINIFRAME - MSW functions
// ------------------------------------

WXDWORD wxExtMiniFrame::MSWGetStyle(long fl, WXDWORD *ex) const
{
    // the WS_POPUP style is very important to allow the frame window 
    // to float on its parent and not to be clipped againt parent's
    // client area...
    return (wxWindow::MSWGetStyle(fl, ex) | WS_POPUP) & (~WS_CHILD);
}

void wxExtMiniFrame::MSWOnActivate(bool bGoingToBeActive, WXHWND win)
{
    if (!bGoingToBeActive) {

        // we are going to lost the focus...
        if ((WXWidget)win != GetParent()->GetHandle()) {

            // ... and the window which is going to receive it,
            // is not our parent...
            if (!IsExtMiniFrame(win)) {

                // ... nor it is another wxExtMiniFrame...
                wxEXTMF_LOG(wxT("wxExtMiniFrame::MSWOnActivate - ")
                    wxT("A non wxExtMiniFrame/wxMainFrame is going to be activated"));

                // ... we must then notify the parent which will deactivate
                // itself and all its palettes...
                ::SendMessage((HWND)GetParent()->GetHandle(), WM_NCACTIVATE, FALSE, 0);
                ::SendMessage((HWND)GetParent()->GetHandle(), WM_ACTIVATE, WA_INACTIVE, (LPARAM)win);
            }
        }

    } else {

        // we are becoming active and the previous window...
        if ((WXWidget)win != GetParent()->GetHandle()) {
            
            // ... was not our parent...
            wxEXTMF_LOG(wxT("wxExtMiniFrame::MSWOnActivate - ")
                wxT("A wxExtMiniFrame has been activated by a non-wxMainFrame"));

            if (!IsExtMiniFrame(win)) {
            
                // ... and it was not a wxExtMiniFrame: notify the parent that we must become active...
                ::SendMessage((HWND)GetParent()->GetHandle(), WM_ACTIVATE, WA_ACTIVE, (LPARAM)win);
            }
        }
    }
}

long wxExtMiniFrame::MSWGetHitTest(WXLPARAM lParam)
{
    // extract point coords
    POINTS pts = MAKEPOINTS(lParam);
    wxPoint pos(pts.x, pts.y);

    // convert them
    pos = ScreenToFrame(pos);

    // perform hit test using the common wxExtMiniFrameBase function
    long r = HitTest(pos);
    
    // convert from OnHitTest codes to Win32 codes
    switch (r) {
    case wxPFHT_TOPLEVEL_CLIENT_AREA:
        r = HTCLIENT; break;
    case wxPFHT_TOPLEVEL_TITLEBAR:
        r = HTCAPTION; break;
    case wxPFHT_TOPLEVEL_BORDER_N:
        r = HTTOP; break;
    case wxPFHT_TOPLEVEL_BORDER_S:
        r = HTBOTTOM; break;
    case wxPFHT_TOPLEVEL_BORDER_W:
        r = HTLEFT; break;
    case wxPFHT_TOPLEVEL_BORDER_E:
        r = HTRIGHT; break;
    case wxPFHT_TOPLEVEL_BORDER_NE:
        r = HTTOPRIGHT; break;
    case wxPFHT_TOPLEVEL_BORDER_NW:
        r = HTTOPLEFT; break;
    case wxPFHT_TOPLEVEL_BORDER_SE:
        r = HTBOTTOMRIGHT; break;
    case wxPFHT_TOPLEVEL_BORDER_SW:
        r = HTBOTTOMLEFT; break;
        
    default:
        wxFAIL_MSG(wxT("Unrecognized flag..."));
    }
    
#ifdef wxEXTMINIFRM_FULLDEBUG
    
    // convert the hit test result in a string to ouput
    wxString debug;
    switch (r) {
    case HTCAPTION:
        debug = wxT("CAPTION"); break;
    case HTLEFT:
        debug = wxT("LEFT BORDER"); break;
    case HTRIGHT:
        debug = wxT("RIGHT BORDER"); break;
    case HTBOTTOM:
        debug = wxT("BOTTOM BORDER"); break;
    case HTTOP:
        debug = wxT("TOP BORDER"); break;
    case HTNOWHERE:
        debug = wxT("NOWHERE"); break;
    default:
        debug = wxT("TWO BORDERS JUNCTION");
    }
    
    wxEXTMF_LOG(wxT("wxExtMiniFrame::OnHitTest - %s"), debug);
#endif

    return r;
}

long wxExtMiniFrame::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    // we must perform our own hit testing because this class derives
    // directly from wxWindow and not from wxFrame (to understand
    // why, see the info in the header file)
    if (nMsg == WM_NCHITTEST) {

        long r = MSWGetHitTest(lParam);

        // if we return HTCAPTION when the cursor is over the caption bar,
        // Windows will automatically handle all the drag-stuff...
        return r;
    }


    if (nMsg == WM_ACTIVATE) {

        // we need to process this msg because wxEVT_ACTIVATE doesn't
        // allow us to know which window is going to be activated/
        // deactivated...
        MSWOnActivate(LOWORD(wParam) != 0, (WXHWND) lParam);
    }


    // if we would use this to avoid that the mainframe loses its activation
    // state, then this window wouldn't be able to be dragged...
    //
    // if (nMsg == WM_MOUSEACTIVATE && LOWORD(lParam) != HTCLIENT)
    // 		return MA_NOACTIVATEANDEAT;


    // to allow the correct work of wxMiniButtons...
    if (nMsg == WM_NCLBUTTONDOWN && wParam == HTCAPTION) {
        
        wxMouseEvent event(wxEVT_LEFT_DOWN);
        MSWInitNCMouseEvent(event, lParam);
        OnLeftDown((wxMouseEvent &)event);
    }

    // when we are capturing the cursor & the user clicks the caption
    // bar, we receive WM_LBUTTONUP messages, not WM_NCLBUTTONUP...
    if (nMsg == WM_LBUTTONUP || 
        (nMsg == WM_NCLBUTTONUP && wParam == HTCAPTION)) {

        wxMouseEvent event(wxEVT_LEFT_UP);
        MSWInitNCMouseEvent(event, lParam, FALSE);
        OnLeftUp((wxMouseEvent &)event);
    }

    // to allow the context menu...
    if (nMsg == WM_NCRBUTTONUP && wParam == HTCAPTION) {

        wxMouseEvent event(wxEVT_RIGHT_UP);
        MSWInitNCMouseEvent(event, lParam, FALSE);
        OnRightUp((wxMouseEvent &)event);
    }

    // is this used anymore ?
    if (nMsg == WM_SYSCOMMAND && (wParam == SC_MAXIMIZE ||
        wParam == SC_MINIMIZE || wParam == SC_RESTORE))
        Layout();

    return wxWindow::MSWWindowProc(nMsg, wParam, lParam);
}




// ------------------------------------
// wxEXTMINIFRAME - event handlers
// ------------------------------------

void wxExtMiniFrame::OnActivate(wxActivateEvent &ev)
{
    wxEXTMF_LOG(wxT("wxExtMiniFrame::OnActivate [%d]: status: %d"), 
        GetHandle(), ev.GetActive());
    
    if (ev.GetActive()) {
        
        // we have been activated; notify this to our wxMainFrame
        // activating it too (it will then reflect activation
        // state change to us and to all other wxExtMiniFrames it owns)
        ::SendMessage((HWND)GetParent()->GetHandle(), WM_ACTIVATE, 
                                    WA_ACTIVE, (LPARAM)GetHandle());
    } else {
        
        // just refresh...
        Draw();
    }
}



// ---------------------------------
// wxEXTMINIFRAME - Roll/Unroll
// ---------------------------------

void wxExtMiniFrame::Roll()
{
    /**************************************** identic to wxExtMiniFrameBase::Roll() */
    if (IsRolled())
        return;		// already rolled

    // save the current size (it will be restored in UnRoll)
    m_szLast = GetClientSize();

    // for some reason this is required...
    m_szLast.y -= GetTitleHeight();

    m_szLastHints = wxSize(m_minWidth, m_minHeight);

    // hide the client window and change our size
    SetSizeHints(0, 0);
    GetClient()->Hide();
    SetSize(GetRolledSize());
    m_bRolled = TRUE;
    /********************************************************************************/


    // we cannot remove the wxRESIZE_BORDER but the OnHitTest function
    // will care to make the user unable to resize this window when
    // we are rolled up...
    //
    // something like, would cause some problems of child-positionining:
    // SetWindowStyle(GetWindowStyle() & ~wxRESIZE_BORDER | wxSUNKEN_BORDER);

    // we need to move buttons in the right place...
    Layout();

    // force parent refresh so that this extminiframe window will get its
    // activation messages and focus will not go out of sync...
    GetParent()->Refresh();

    // DO NOT REFRESH OURSELVES...
    // this->Refresh();

#ifdef wxEXTMINIFRM_USE_MINIBTN
    // just the minibuttons (in particular, the wxCollapseBox button)
    wxWindowDC dc(this);
    DrawMiniButtons(dc);
#endif
}

#endif      // wxEXTMINIFRM_USE_EXTMINIFRM




#ifdef wxEXTMINIFRM_USE_MINIBTN

// ------------------------------------
// wxMINIBUTTON
// ------------------------------------

void wxMiniButton::DrawMSWStdButton(wxDC &dc, int x, int y, unsigned int mainstyle)
{
    // now that we have chosen the width (and thus height)
    // of this button, we can draw it
    RECT rc;
    rc.left = x;
    rc.top = y;
    rc.right = x+GetSize().GetWidth();
    rc.bottom = y+GetSize().GetHeight();

    int style = mainstyle;
    style |= (m_bPressed ? DFCS_PUSHED : 0);
    style |= (m_bEnabled ? 0 : DFCS_INACTIVE);

    DrawFrameControl((HDC)dc.GetHDC(), &rc,	DFC_CAPTION, style);
}

void wxMiniButton::DrawButtonFrame(wxDC &dc, int x, int y)
{
    RECT rc;
    rc.left = x;
    rc.top = y;
    rc.right = x+GetSize().GetWidth();
    rc.bottom = y+GetSize().GetHeight();

    int style = 0;
    style |= (m_bPressed ? DFCS_PUSHED : 0);
    style |= (m_bEnabled ? 0 : DFCS_INACTIVE);

    DrawFrameControl((HDC)dc.GetHDC(), &rc, DFC_BUTTON, DFCS_BUTTONPUSH | style);
}

int wxMiniButton::GetBorderSize() const
{
    // DrawCaptionControl API always draws lines with a tickness of
    // three pixels...
    return 3;
}

#endif		// wxEXTMINIFRM_USE_MINIBTN

