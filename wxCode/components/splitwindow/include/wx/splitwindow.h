//////////////////////////////////////////////////////////////////////////////
// File:        splitwindow.h
// Purpose:     wxSplitWindow
// Maintainer:  Wyo
// Created:     2004-12-21
// RCS-ID:      $Id: splitwindow.h,v 1.1 2005-05-24 16:02:42 wyo Exp $
// Copyright:   (c) 2004 wxCode
// Licence:     wxWindows
//////////////////////////////////////////////////////////////////////////////

#ifndef _MY_SPLITWINDOW_H_
#define _MY_SPLITWINDOW_H_

#ifdef __GNUG__
    #pragma implementation "splitwindow.h"
#endif

//----------------------------------------------------------------------------
// information
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------
//! wxWidgets headers
#include <wx/event.h>
#include <wx/window.h>


//============================================================================
// declarations
//============================================================================

//----------------------------------------------------------------------------
//!

//----------------------------------------------------------------------------
//! wxSplitWindow
class wxSplitWindow: public wxWindow {

DECLARE_DYNAMIC_CLASS (wxSplitWindow)

public:

    //! default constructor
    wxSplitWindow () { Init(); }

    //! create constructor
    wxSplitWindow (wxWindow *parent,
                   wxWindowID id = -1,
                   const wxPoint &pos = wxDefaultPosition,
                   const wxSize &size = wxDefaultSize,
                   long style = 0,
                   const wxString &name = _T("SplitWindow")) {
        Init ();
        Create (parent, id, pos, size, style, name);
    }

    // function create
    bool Create (wxWindow *parent,
                 wxWindowID id = -1,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = 0,
                 const wxString &name = _T("SplitWindow"));

    // get the top/left, bottom/right window
    wxWindow *GetWindow1() const { return m_windowOne; }
    wxWindow *GetWindow2() const { return m_windowTwo; }

    // get/set settings
    wxOrientation GetOrientation() const { return m_orientation; };
    void SetOrientation (wxOrientation orientation) {
        wxASSERT_MSG (orientation==wxVERTICAL || orientation==wxHORIZONTAL, _T("invalid orientation") );
        m_orientation = orientation;
    }

    int GetSashPosition() const { return m_sashPos; }
    void SetSashPosition (int pos) { m_sashPos = AdjustSashPosition (pos); }

    int GetSashImpact() const { return m_sashImpact; }
    void SetSashImpact (int impact) { m_sashImpact = impact; }

    int GetSashWidth() const { return m_sashWidth; }
    void SetSashWidth (int width) { m_sashWidth = AdjustSashWidth (width); }

    int GetMinWindowSize() const { return m_minWindowSize; }
    void SetMinWindowSize (int min) { m_minWindowSize = AdjustMinWindowSize (min); }

    // creates split window
    bool Initialize (wxWindow *one, wxWindow *two, wxOrientation orientation = wxVERTICAL,
                     int pos = 0, int impact = 0);

private:
    void Init ();

    // Handles mouse events
    void OnMouse (wxMouseEvent& event);

    // Paints the border and sash
    void OnPaint (wxPaintEvent& event);

    // Adjusts the panes
    void OnSize (wxSizeEvent& event);

#if defined( __WXMSW__ ) || defined( __WXMAC__)
    void OnSetCursor (wxSetCursorEvent& event);
#endif // wxMSW || wxMac

    // resize child windows in idle time
#if !wxCHECK_VERSION(2, 5, 0)
    void OnIdle( wxIdleEvent &event );
#endif
    void OnInternalIdle();

    void DrawSash (wxDC& dc);
    void RedrawIfHotSensitive (bool isHotSensitive);
    void SetResizeCursor();
    int AdjustSashPosition (int pos);
    int AdjustSashWidth (int width);
    int AdjustMinWindowSize (int min);
    bool SetSashPositionAndNotify (int pos);
    bool SashHitTest (int pos, int tolerance = 1);
    void SizeWindows();

    //! variables
    wxWindow* m_windowOne;
    wxWindow* m_windowTwo;

    wxOrientation m_orientation;
    int m_sashPos;
    int m_sashImpact;
    int m_sashWidth;
    int m_minWindowSize;

    int m_currentWidth;
    int m_currentSash;
    int m_dragMode;
    int m_oldPos;
    int m_isHotSensitive;

#if !wxCHECK_VERSION(2, 5, 0)
    wxPen m_penBlack;
    wxPen m_penDarkGrey;
    wxPen m_penLightGrey;
    wxPen m_penHighlight;
    int GetSplitterParams (const wxWindow *win);
    void DrawSplitterBorder (wxWindow *win, wxDC& dc, const wxRect& rect);
    void DrawSplitterSash (wxWindow *win, wxDC& dc, const wxSize& size, wxCoord position, wxOrientation orient);
    void DrawShadedRect (wxDC& dc, wxRect *rect, const wxPen& pen1, const wxPen& pen2);
#endif

    DECLARE_EVENT_TABLE()
};

#endif // _MY_SPLITWINDOW_H_

