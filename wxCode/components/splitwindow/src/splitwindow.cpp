//////////////////////////////////////////////////////////////////////////////
// File:        splitwindow.cpp
// Purpose:     wxSplitWindow
// Maintainer:  Wyo
// Created:     2004-12-21
// RCS-ID:      $Id: splitwindow.cpp,v 1.1 2005-05-24 16:02:43 wyo Exp $
// Copyright:   (c) 2004 wxCode
// Licence:     wxWindows
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// information
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "splitwindow.h"
#endif

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

//? TODO  #if wxUSE_SPLITWINDOW

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all 'standard' wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifdef __WXMAC__
    #include <wx/mac/private.h>
#endif

// wxWidgets headers
#include <wx/mimetype.h> // mimetype support
#include <wx/renderer.h> // renderer support
#include <wx/settings.h>
#include <wx/splitter.h>

// splitwindow headers
#include "wx/splitwindow.h"   // wxSplitWindow control


//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------


//============================================================================
// declarations
//============================================================================


//============================================================================
// implementation
//============================================================================

//----------------------------------------------------------------------------
// wxSplitWindow
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS (wxSplitWindow, wxWindow)

BEGIN_EVENT_TABLE (wxSplitWindow, wxWindow)
    EVT_MOUSE_EVENTS (wxSplitWindow::OnMouse)
    EVT_PAINT (       wxSplitWindow::OnPaint)
    EVT_SIZE (        wxSplitWindow::OnSize)
#if defined( __WXMSW__ ) || defined( __WXMAC__)
    EVT_SET_CURSOR (  wxSplitWindow::OnSetCursor)
#endif // wxMSW || wxMac
END_EVENT_TABLE()

bool wxSplitWindow::Create (wxWindow *parent,
                            wxWindowID id,
                            const wxPoint &pos,
                            const wxSize &size,
                            long style,
                            const wxString &name) {

    // allow TABbing from one window to the other
    style |= wxTAB_TRAVERSAL;

    // we draw our border ourselves to blend the sash with it
    style &= ~wxBORDER_MASK;
    style |= wxBORDER_NONE;

    if (!wxWindow::Create (parent, id, pos, size, style, name)) return false;

    // don't erase the background, it's pointless as we overwrite it anyhow
    SetBackgroundStyle (wxBG_STYLE_CUSTOM);

    return true;
}

void wxSplitWindow::Init () {
    m_windowOne = (wxWindow*)NULL;
    m_windowTwo = (wxWindow*)NULL;
    m_orientation = (wxOrientation)0;
    m_sashPos = 0;
    m_sashImpact = 0;
    m_minWindowSize = 0;
    m_sashWidth = 3; // TODO make constant

    m_currentWidth = 0;
    m_currentSash = 0;
    m_dragMode = wxSPLIT_DRAG_NONE;
    m_oldPos = 0;
    m_isHotSensitive = false;
}

bool wxSplitWindow::Initialize (wxWindow* one, wxWindow* two, wxOrientation orientation,
                                int pos, int impact) {
    m_windowOne = one;
    m_windowTwo = two;
    m_orientation = orientation;
    m_sashPos = pos;
    m_sashImpact = wxMax (0, wxMin (impact, 100));

    SizeWindows();
    return true;
}


//----------------------------------------------------------------------------
// event handlers

void wxSplitWindow::OnInternalIdle() {
    wxWindow::OnInternalIdle();
    SizeWindows();
}

void wxSplitWindow::OnMouse (wxMouseEvent& event) {
    int pos = m_orientation == wxVERTICAL? (int)event.GetX(): (int)event.GetY();

    // left down means dragging the sash if hit
    if (event.LeftDown()) {
        if (SashHitTest (pos)) {
            m_dragMode = wxSPLIT_DRAG_DRAGGING;
            CaptureMouse();
            SetResizeCursor();
            m_oldPos = pos;
            return;
        }

    // left up during dragging means ending
    }else if (event.LeftUp() && m_dragMode == wxSPLIT_DRAG_DRAGGING) {
        m_dragMode = wxSPLIT_DRAG_NONE;
        ReleaseMouse();
        SetCursor (*wxSTANDARD_CURSOR);
        SetSashPositionAndNotify (AdjustSashPosition (m_sashPos + (pos - m_oldPos)));
        m_oldPos = pos;
        SizeWindows();
        return;

    }else if ((event.Moving() || event.Leaving() || event.Entering()) &&
              (m_dragMode == wxSPLIT_DRAG_NONE)) {
        if (event.Leaving() || !SashHitTest (pos)) {
            SetCursor (*wxSTANDARD_CURSOR);
            RedrawIfHotSensitive (false);
        }else{
            SetResizeCursor ();
            RedrawIfHotSensitive (true);
        }
        return;

    }else if (event.Dragging() && (m_dragMode == wxSPLIT_DRAG_DRAGGING)) {
        if (pos == m_oldPos) return; // nothing to do
        SetSashPositionAndNotify (AdjustSashPosition (m_sashPos + (pos - m_oldPos)));
        m_oldPos = pos;
        return;

    }
}

void wxSplitWindow::OnPaint (wxPaintEvent& WXUNUSED(event)) {
    wxPaintDC dc(this);
    DrawSash(dc);
}

void wxSplitWindow::OnSize (wxSizeEvent& event) {
    // only process this message if we're not iconized - otherwise iconizing
    // and restoring a window containing the splitter has a funny side effect
    // of changing the splitter position!
    wxWindow *parent = wxGetTopLevelParent(this);
    wxTopLevelWindow *winTop = wxDynamicCast(parent, wxTopLevelWindow);
    if (!winTop) wxFAIL_MSG (_T("should have a top level parent!"));
    if (winTop->IsIconized()) {
        m_currentWidth = 0;
        event.Skip();
        return;
    }

    int w, h;
    GetClientSize (&w, &h);
    int border = wxRendererNative::Get().GetSplitterParams(this).border;
    int width = (m_orientation == wxVERTICAL? w: h) - 2*border;
    if (m_currentWidth > m_sashPos) {
        int delta = (width - m_currentWidth)*m_sashImpact/100;
        if (delta != 0) {
            SetSashPositionAndNotify (AdjustSashPosition (m_sashPos + delta));
        }
    }
    m_currentWidth = width;

    // size windows
    SizeWindows();
}

#if defined( __WXMSW__ ) || defined( __WXMAC__)
// this is currently called (and needed) under MSW only...
void wxSplitWindow::OnSetCursor(wxSetCursorEvent& event) {
    // if we don't do it, the resizing cursor might be set for child window:
    // and like this we explicitly say that our cursor should not be used for
    // children windows which overlap us
    int pos = m_orientation == wxVERTICAL? (int)event.GetX(): (int)event.GetY();
    if (SashHitTest (pos), 0) {
        event.Skip(); // default processing is ok
    }
    //else: do nothing, in particular, don't call Skip()
}
#endif // wxMSW || wxMac

//----------------------------------------------------------------------------
// private functions

void wxSplitWindow::DrawSash (wxDC& dc) {
    wxRendererNative::Get().DrawSplitterBorder (this, dc, GetClientRect());

    // don't draw sash if we're not split
    if (m_sashPos == 0) return;

    wxRendererNative::Get().DrawSplitterSash (this, dc, GetClientSize(),
                                              m_sashPos, m_orientation,
                                              m_isHotSensitive? (int)wxCONTROL_CURRENT : 0);
}

void wxSplitWindow::RedrawIfHotSensitive (bool isHotSensitive) {
    if (wxRendererNative::Get().GetSplitterParams(this).isHotSensitive) {
        m_isHotSensitive = isHotSensitive;
        wxClientDC dc (this);
        DrawSash (dc);
    }
}

void wxSplitWindow::SetResizeCursor() {
    SetCursor (wxCursor(m_orientation == wxVERTICAL? wxCURSOR_SIZEWE: wxCURSOR_SIZENS));
}

int wxSplitWindow::AdjustSashPosition (int pos) {
    int w, h;
    GetClientSize (&w, &h);
    int border = wxRendererNative::Get().GetSplitterParams(this).border;
    int width = (m_orientation == wxVERTICAL? w: h) - 2*border;
    pos = ((border + m_minWindowSize) <= pos)? pos: 0;
    pos = ((width - m_minWindowSize) >= pos)? pos: 0;
    return pos;
}

int wxSplitWindow::AdjustSashWidth (int width) {
    int w, h;
    GetClientSize (&w, &h);
    int border = wxRendererNative::Get().GetSplitterParams(this).border;
    int max = (m_orientation == wxVERTICAL? w: h) - m_minWindowSize - 4*border;
    width = (width >= 0)? width: 0;
    width = (max <= width)? width: max;
    return width;
}

int wxSplitWindow::AdjustMinWindowSize (int min) {
    int w, h;
    GetClientSize (&w, &h);
    int border = wxRendererNative::Get().GetSplitterParams(this).border;
    int max = (m_orientation == wxVERTICAL? w: h) - m_sashWidth - 4*border;
    min = (min >= 0)? min: 0;
    min = (max <= min)? min: max;
    return min;
}

bool wxSplitWindow::SetSashPositionAndNotify (int pos) {
    if (m_sashPos == pos) return false; // No sash
    m_sashPos = pos;
    return true;
}

bool wxSplitWindow::SashHitTest (int pos, int tolerance) {
    if (m_sashPos == 0) return false; // No sash
    return (pos >= m_sashPos-tolerance) && (pos <= m_sashPos+m_sashWidth+tolerance);
}

void wxSplitWindow::SizeWindows() {
    if (!m_windowOne || !m_windowTwo) return;

    int w, h;
    GetClientSize (&w, &h);
    int border = wxRendererNative::Get().GetSplitterParams(this).border;

    // compute new sizes
    int x2, y2, w1, h1, w2, h2;
    if (m_orientation == wxVERTICAL) {
        w1 = m_sashPos - border;
        w2 = w - w1 - m_sashWidth - 2*border;
        h1 = h - 2*border;
        h2 = h1;
        x2 = m_sashPos + m_sashWidth;
        y2 = border;
    }else{
        w1 = w - 2*border;
        w2 = w1;
        h1 = m_sashPos - border;
        h2 = h - h1 - m_sashWidth - 2*border;
        x2 = border;
        y2 = m_sashPos + m_sashWidth;
    }

    // set sizes and draw sash
    m_windowOne->SetSize (border, border, w1, h1);
    m_windowTwo->SetSize (x2, y2, w2, h2);
    wxClientDC dc (this);
    DrawSash (dc);

}

//? #endif // wxUSE_SPLITWINDOW
