/////////////////////////////////////////////////////////////////////////////
// Name:        splitwindow.h
// Purpose:     wxSplitWindow
// Maintainer:  Wyo
// Created:     2006-04-18
// RCS-ID:      $Id: splitwindow.h,v 1.2 2006-05-07 14:30:29 wyo Exp $
// Copyright:   (c) 2006 wxCode
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#ifndef _MY_SPLITWINDOW_H_
#define _MY_SPLITWINDOW_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "splitwindow.h"
#endif

//----------------------------------------------------------------------------
// information
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------
//! wxWidgets headers
#include <wx/window.h>                      // base class declaration
#include <wx/containr.h>                    // wxControlContainer

class WXDLLEXPORT wxSplitWindowEvent;

//============================================================================
// declarations
//============================================================================

//----------------------------------------------------------------------------
//! constands
#define wxSP_NOBORDER         0x0000
#define wxSP_NOSASH           0x0010
#define wxSP_PERMIT_UNSPLIT   0x0040
#define wxSP_LIVE_UPDATE      0x0080
#define wxSP_3DSASH           0x0100
#define wxSP_3DBORDER         0x0200
#define wxSP_NO_XP_THEME      0x0400
#define wxSP_BORDER           wxSP_3DBORDER
#define wxSP_3D               (wxSP_3DBORDER | wxSP_3DSASH)

enum wxSplitMode
{
    wxSPLIT_HORIZONTAL = 1,
    wxSPLIT_VERTICAL
};

enum
{
    wxSPLIT_DRAG_NONE,
    wxSPLIT_DRAG_DRAGGING,
    wxSPLIT_DRAG_LEFT_DOWN
};

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_SPLITWINDOW_SASH_POS_CHANGED, 850)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_SPLITWINDOW_SASH_POS_CHANGING, 851)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_SPLITWINDOW_DOUBLECLICKED, 852)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_SPLITWINDOW_UNSPLIT, 853)
END_DECLARE_EVENT_TYPES()

//----------------------------------------------------------------------------
//! wxSplitWindow

class WXDLLEXPORT wxSplitWindow: public wxWindow {

public:

    //! default constructor
    wxSplitWindow () { Init(); }

    //! create constructor
    wxSplitWindow (wxWindow *parent,
                   wxWindowID id = wxID_ANY,
                   const wxPoint &pos = wxDefaultPosition,
                   const wxSize &size = wxDefaultSize,
                   long style = wxSP_LIVE_UPDATE,
                   const wxString &name = _T("SplitWindow")) {
        Init();
        Create (parent, id, pos, size, style, name);
    }

    virtual ~wxSplitWindow();

    //! function create
    bool Create (wxWindow *parent,
                 wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxSP_LIVE_UPDATE,
                 const wxString& name = _T("SplitWindow"));

    //! get the top/left, bottom/right window
    wxWindow *GetWindow1() const { return m_windowOne; }
    wxWindow *GetWindow2() const { return m_windowTwo; }

    //! initialize both windows, default split vertical
    virtual bool Initialize (wxWindow *window1,
                             wxWindow *window2,
                             int sashPosition = 0,
                             int mode = wxSPLIT_VERTICAL) {
        wxASSERT_MSG (mode == wxSPLIT_VERTICAL || mode == wxSPLIT_HORIZONTAL,
                      _T("invalid split mode"));
        return DoSplit ((wxSplitMode)mode, window1, window2, sashPosition);
    }

    // obsolete, just for compatibility
    virtual bool SplitVertically (wxWindow *window1,
                                  wxWindow *window2,
                                  int sashPosition = 0)
        { return DoSplit(wxSPLIT_VERTICAL, window1, window2, sashPosition); }
    virtual bool SplitHorizontally (wxWindow *window1,
                                    wxWindow *window2,
                                    int sashPosition = 0)
        { return DoSplit(wxSPLIT_HORIZONTAL, window1, window2, sashPosition); }

    // Removes the specified (or second) window from the view
    // Doesn't actually delete the window.
    bool Unsplit(wxWindow *toRemove = (wxWindow *) NULL);

    // Replaces one of the windows with another one (neither old nor new
    // parameter should be NULL)
    bool ReplaceWindow(wxWindow *winOld, wxWindow *winNew);

    // Make sure the child window sizes are updated. This is useful
    // for reducing flicker by updating the sizes before a
    // window is shown, if you know the overall size is correct.
    void UpdateSize();

    // Is the window split?
    bool IsSplit() const { return (m_sashPosition > 0 && m_sashPosition < GetWindowSize()); }

    //! get/set border size
    int GetBorderSize() const;
    void SetBorderSize (int WXUNUSED(width)) { }

    //! get/set minimum pane size
    int GetMinimumPaneSize() const { return m_minimumPaneSize; }
    void SetMinimumPaneSize (int min);

    //! get/set sash gravity
    double GetSashGravity() const { return m_sashGravity; }
    void SetSashGravity(double gravity);

    //! get/set sash position
    int GetSashPosition() const { return m_sashPosition; }
    void SetSashPosition (int position, bool redraw = true);

    //! get/set sash size
    int GetSashSize() const;
    void SetSashSize (int width) { m_sashSize = width; }

    //! get/set split mode
    wxSplitMode GetSplitMode() const { return m_splitMode; };
    void SetSplitMode (int mode) {
        wxASSERT_MSG (mode == wxSPLIT_VERTICAL || mode == wxSPLIT_HORIZONTAL,
                      _T("invalid split mode"));
        m_splitMode = (wxSplitMode)mode;
    }

    // NB: the OnXXX() functions below are for backwards compatibility only,
    //     don't use them in new code but handle the events instead!

    // called when the sash position is about to change, may return a new value
    // for the sash or -1 to prevent the change from happening at all
    virtual int OnSashPositionChanging(int newSashPosition);

    // Called when the sash position is about to be changed, return
    // false from here to prevent the change from taking place.
    // Repositions sash to minimum position if pane would be too small.
    // newSashPosition here is always positive or zero.
    virtual bool OnSashPositionChange(int newSashPosition);

    // If the sash is moved to an extreme position, a subwindow
    // is removed from the splitwindow window, and the app is
    // notified. The app should delete or hide the window.
    virtual void OnUnsplit(wxWindow *removed);

    // Called when the sash is double-clicked.
    // The default behaviour is to remove the sash if the
    // minimum pane size is zero.
    virtual void OnDoubleClickSash(int x, int y);

////////////////////////////////////////////////////////////////////////////
// Implementation

    // Paints the border and sash
    void OnPaint(wxPaintEvent& event);

    // Handles mouse events
    void OnMouseEvent(wxMouseEvent& ev);

    // Adjusts the panes
    void OnSize(wxSizeEvent& event);

    // In live mode, resize child windows in idle time
    void OnInternalIdle();

    // Draws the sash
    virtual void DrawSash(wxDC& dc);

    // Draws the sash tracker (for whilst moving the sash)
    virtual void DrawSashTracker(int x, int y);

    // Tests for x, y over sash
    virtual bool SashHitTest(int x, int y, int tolerance = 5);

    // Resizes subwindows
    virtual void SizeWindows();

    void SetNeedUpdating(bool needUpdating) { m_needUpdating = needUpdating; }
    bool GetNeedUpdating() const { return m_needUpdating ; }

#ifdef __WXMAC__
    virtual bool MacClipGrandChildren() const { return true ; }
#endif

protected:
    // event handlers
#if defined(__WXMSW__) || defined(__WXMAC__)
    void OnSetCursor(wxSetCursorEvent& event);
#endif // wxMSW

    // send the given event, return false if the event was processed and vetoed
    // by the user code
    inline bool DoSendEvent(wxSplitWindowEvent& event);

    // common part of all ctors
    void Init();

    // common part of SplitVertically() and SplitHorizontally()
    bool DoSplit(wxSplitMode mode,
                 wxWindow *window1, wxWindow *window2,
                 int sashPosition);

    // adjusts sash position with respect to min. pane and window sizes
    int AdjustSashPosition(int sashPos) const;

    // get either width or height depending on the split mode
    int GetWindowSize() const;

    // convert the user specified sash position which may be > 0 (as is), < 0
    // (specifying the size of the right pane) or 0 (use default) to the real
    // position to be passed to DoSetSashPosition()
    int ConvertSashPosition(int sashPos) const;

    // set the real sash position, sashPos here must be positive
    //
    // returns true if the sash position has been changed, false otherwise
    bool DoSetSashPosition(int sashPos);

    // set the sash position and send an event about it having been changed
    void SetSashPositionAndNotify(int sashPos);

    // callbacks executed when we detect that the mouse has entered or left
    // the sash
    virtual void OnEnterSash();
    virtual void OnLeaveSash();

    // set the cursor appropriate for the current split mode
    void SetResizeCursor();

    // redraw the splitwindow if its "hotness" changed if necessary
    void RedrawIfHotSensitive(bool isHot);

    // return the best size of the splitwindow equal to best sizes of its
    // subwindows
    virtual wxSize DoGetBestSize() const;


    wxSplitMode m_splitMode;
    wxWindow*   m_windowOne;
    wxWindow*   m_windowTwo;
    int         m_dragMode;
    int         m_oldX;
    int         m_oldY;
    double      m_sashGravity;
    int         m_sashPosition; // Number of pixels from left or top
    int         m_sashSize;
    wxSize      m_lastSize;
    int         m_requestedSashPosition;
    int         m_sashPositionCurrent; // while dragging
    int         m_firstX;
    int         m_firstY;
    int         m_minimumPaneSize;
    wxCursor    m_sashCursorWE;
    wxCursor    m_sashCursorNS;
    wxPen      *m_sashTrackerPen;

    // when in live mode, set this to true to resize children in idle
    bool        m_needUpdating:1;
    bool        m_permitUnsplitAlways:1;
    bool        m_isHot:1;
    bool        m_checkRequestedSashPosition:1;

private:
    WX_DECLARE_CONTROL_CONTAINER();

    DECLARE_DYNAMIC_CLASS(wxSplitWindow)
    DECLARE_EVENT_TABLE()
    DECLARE_NO_COPY_CLASS(wxSplitWindow)
};

// ----------------------------------------------------------------------------
// event class and macros
// ----------------------------------------------------------------------------

// we reuse the same class for all splitwindow event types because this is the
// usual wxWin convention, but the three event types have different kind of
// data associated with them, so the accessors can be only used if the real
// event type matches with the one for which the accessors make sense
class WXDLLEXPORT wxSplitWindowEvent : public wxNotifyEvent
{
public:
    wxSplitWindowEvent(wxEventType type = wxEVT_NULL,
                    wxSplitWindow *splitwindow = (wxSplitWindow *)NULL)
        : wxNotifyEvent(type)
    {
        SetEventObject(splitwindow);
        if (splitwindow) m_id = splitwindow->GetId();
    }

    // SASH_POS_CHANGED methods

    // setting the sash position to -1 prevents the change from taking place at
    // all
    void SetSashPosition(int pos)
    {
        wxASSERT( GetEventType() == wxEVT_COMMAND_SPLITWINDOW_SASH_POS_CHANGED
                || GetEventType() == wxEVT_COMMAND_SPLITWINDOW_SASH_POS_CHANGING);

        m_data.pos = pos;
    }

    int GetSashPosition() const
    {
        wxASSERT( GetEventType() == wxEVT_COMMAND_SPLITWINDOW_SASH_POS_CHANGED
                || GetEventType() == wxEVT_COMMAND_SPLITWINDOW_SASH_POS_CHANGING);

        return m_data.pos;
    }

    // UNSPLIT event methods
    wxWindow *GetWindowBeingRemoved() const
    {
        wxASSERT( GetEventType() == wxEVT_COMMAND_SPLITWINDOW_UNSPLIT );

        return m_data.win;
    }

    // DCLICK event methods
    int GetX() const
    {
        wxASSERT( GetEventType() == wxEVT_COMMAND_SPLITWINDOW_DOUBLECLICKED );

        return m_data.pt.x;
    }

    int GetY() const
    {
        wxASSERT( GetEventType() == wxEVT_COMMAND_SPLITWINDOW_DOUBLECLICKED );

        return m_data.pt.y;
    }

private:
    friend class WXDLLEXPORT wxSplitWindow;

    // data for the different types of event
    union
    {
        int pos;            // position for SASH_POS_CHANGED event
        wxWindow *win;      // window being removed for UNSPLIT event
        struct
        {
            int x, y;
        } pt;               // position of double click for DCLICK event
    } m_data;

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxSplitWindowEvent)
};

typedef void (wxEvtHandler::*wxSplitWindowEventFunction)(wxSplitWindowEvent&);

#define wxSplitWindowEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxSplitWindowEventFunction, &func)

#define wx__DECLARE_SPLITTEREVT(evt, id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_SPLITWINDOW_ ## evt, id, wxSplitWindowEventHandler(fn))

#define EVT_SPLITTER_SASH_POS_CHANGED(id, fn) \
    wx__DECLARE_SPLITTEREVT(SASH_POS_CHANGED, id, fn)

#define EVT_SPLITTER_SASH_POS_CHANGING(id, fn) \
    wx__DECLARE_SPLITTEREVT(SASH_POS_CHANGING, id, fn)

#define EVT_SPLITTER_DCLICK(id, fn) \
    wx__DECLARE_SPLITTEREVT(DOUBLECLICKED, id, fn)

#define EVT_SPLITTER_UNSPLIT(id, fn) \
    wx__DECLARE_SPLITTEREVT(UNSPLIT, id, fn)

#endif // _MY_SPLITWINDOW_H_
