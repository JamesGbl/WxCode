/////////////////////////////////////////////////////////////////////////////
// Name:        wx/resizec.h
// Purpose:     wxResizeableControl base class and derived classes
//              for resizeable pictures
// Author:      Markus Greither
// Modified by:
// Created:     11/10/02
// RCS-ID:      $Id: resizec.h,v 1.7 2005-10-02 19:43:46 frm Exp $
// Copyright:   (c) Markus Greither
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __RESIZECONTROL__
#define __RESIZECONTROL__

#if defined(__WXMSW__) && wxUSE_METAFILE && wxUSE_ENH_METAFILE
#include "wx/metafile.h"
#endif

#include "wx/resizecdef.h"
#include <wx/scrolwin.h>

// defined later...
class WXDLLIMPEXP_RESIZEC wxResizeableControlCanvas;


// ----------------------------------------------------------------------------
// wxResizeableControl
// ----------------------------------------------------------------------------

//! Class for zoomable windows
/*! Stores the original size of the window, so that
    Zooming is always computed from this data, and thus
    rounding errors are avoided.
*/
class wxZoomData
{
    //! Original window size
    wxSize m_orgSize;
    //! Original window pos
    wxPoint m_orgPos;
    //! Current window size
    wxRect m_currRect;
    //! Current zoom factor
    float m_zoom;
 public:
    //! Ctor
    wxZoomData() : m_zoom(1) {}
    //! Ctor
    wxZoomData(const wxSize &size,const wxPoint &pos)
     : m_orgSize(size),m_orgPos(pos),m_zoom(1) {}
    //! Set new zoom factor (changes size only)
    void SetZoomSize(wxWindow *Window,float zoom)
    {
        m_zoom = zoom;
        m_currRect = wxRect(m_currRect.GetPosition(),
                            wxSize(int(m_orgSize.x*m_zoom),
                                   int(m_orgSize.y*m_zoom)));
        Window->SetSize(m_currRect.GetSize());
    }
    //! Set new zoom factor (position and size)
    /*! curxoffs and curyoffs are the current offset values (e. g.
        scroll position) in absolute size (unzoomed)
        newxoffs and newyoffs are the new offset values
    */
    void SetZoomRect(wxWindow *Window,float zoom,
                     int curxoffs,int curyoffs,
                     int newxoffs,int newyoffs);
    //! Changes zoom factor without resizing the window
    void SetCurrentZoom(float zoom)
    {
        m_zoom = zoom;
        m_orgPos.x = int(m_orgPos.x/zoom);
        m_orgPos.y = int(m_orgPos.y/zoom);
        m_orgSize.x = int(m_orgSize.x/zoom);
        m_orgSize.y = int(m_orgSize.y/zoom);
    }
    //! Returns the set zoom factor
    float GetZoom() {return m_zoom;}
    //! Updates the original window size
    void SetSize(int width,int height);
    //! Updates the original window position
    void Move(int xpos,int ypos);
};

//! Class for moveable and resizable child windows
/*!
    This class draws resize rectangles on each side and corner when focused.
    Dragging these rectangles resizes the window.

    This class needs to be overloaded and Paint() defined with the graphics
    rendering.

    This would make a perfect abstract class, but the
    definition of wxDynamicCast() doesn't allow this.
*/
class wxResizeableControl : public wxWindow
{
 public:
    //! Flags for resize mode
    enum SizeMoveModes
    {
        Top = 0,
        Bottom,
        Left,
        Right,
        TopLeft,
        BottomLeft,
        TopRight,
        BottomRight,
        MoveWin,
        MaxMode
    };
    //! Size box dimension
    enum SizeRads
    {
        SizeXRad = 5,
        SizeYRad = 5
    };
    //! Ctor
    wxResizeableControl() : wxWindow(), m_curId(-2) { }
    //! Ctor
    wxResizeableControl(wxWindow *AParent, int AnId,const wxPoint &pos,
                        const wxSize &size,long style = 0,
                        const wxString &name = wxPanelNameStr);
    //! Dtor
    virtual ~wxResizeableControl() {}
    //! Paint function for printing and screen output
    virtual void Paint(wxDC &WXUNUSED(dc),bool WXUNUSED(Printing),wxSize &WXUNUSED(size)) {}
    //! Returns original size of control
    virtual wxSize GetOriginalSize() {return wxSize(-1,-1);}
    //! Returns with/height ration
    virtual float GetRatio()
    {
        return 0;
    }
    //! Returns the zoom data object
    wxZoomData &GetZoomData() {return m_zoomData;}
    //! Set new zoom
    void SetZoom(float Zoom)
    {
      m_zoomData.SetZoomSize(this,Zoom);
    }
    //! Set new zoom factor, but do not resize/move
    void SetCurrentZoom(float Zoom)
    {
      m_zoomData.SetCurrentZoom(Zoom);
    }
    //! Same as FocusRectCoord but with wxRect parameter
    inline void DrawFocusRect(wxDC &dc,wxRect rct)
    {
        FocusRectCoord(dc,rct.x,rct.y,rct.width,rct.height);
    }
    //! Make sure the zoom data gets updated when the window is moved
    void DoSetSize(int x, int y,int width, int height,
                   int sizeFlags = wxSIZE_AUTO)
    {
        if ((x != -1) || (y != -1))
            m_zoomData.Move(x,y);
        if ((width != -1) || (height != -1))
            m_zoomData.SetSize(width,height);
        wxWindow::DoSetSize(x,y,width,height,sizeFlags);
    }

	//! Returns the canvas which contains this resizeable control.
	wxResizeableControlCanvas *GetCanvas();

// Event Handlers

    //! Responds to show sizers event
    void OnShowSizers(wxCommandEvent &event);
    //! Responds to hide sizers event
    void OnHideSizers(wxCommandEvent &event);
    //! Responds to set cursor event
    void OnSetCursor(wxSetCursorEvent &event);
    //! Responds to left button up event
    void OnLButtonUp(wxMouseEvent &event);
    //! Responds to left button down event
    void OnLButtonDown(wxMouseEvent &event);
    //! Responds to mouse move event
    void OnMouseMove(wxMouseEvent &event);
    //! Kill focus event hides the size rectangles
    void OnKillFocus(wxFocusEvent &event);
    //! Set focus event draws size rectangles
    void OnSetFocus(wxFocusEvent &event);
    //! Cut event deletes the window
    void OnEditCut(wxCommandEvent &ce);
    //! Key down event of WXK_DELETE deletes the window
    void OnKeyDown(wxKeyEvent &event);
    //! Reponds to size event
    void OnSize(wxSizeEvent &event);
    //! Reponds to move event
    void OnMove(wxMoveEvent &event);
    //! Delete this window, let parent know
    bool Destroy();

 protected:
    //! Zoom data
    wxZoomData m_zoomData;
    //! Current cursor
    /*! necessary for MSW fake stock cursors (e.g. wxCURSOR_SIZING)
        together with wxSetCursorEvent(), as the current cursor is not
        buffered in this case and temporary cursors are automatically
        unloaded from windows (provided my cursor patch that prevents
        cursor memory leaks will get implemented) */
    wxCursor m_csr;
    //! Current cursor id (-1 for non-stock cursor)
    long m_curId;
    //! Flag for mouse capture
    int m_capt;
    //! Cached mouse cursor positions
    wxPoint m_curpos,m_lastcurpos;
    //! Saved move mode from left button down
    int m_movemode;
    //! Flags for focus and move state
    bool m_hasfocus,m_moved;

    //! Function that draws a focus rectangle
    void FocusRectCoord(wxDC &DC,wxCoord x1,wxCoord y1,wxCoord w,wxCoord h);
    //! Returns x size
    int GetSizeX(int Mode);
    //! Returns y size
    int GetSizeY(int Mode);
    //! Draw ored rectangle of current new size (redraw to remove)
    void DrawMoveRect(wxPoint hp,int Mode,float Ratio = 0);
    //! Return new rectangle size, based on mouse position hp
    wxRect NewRect(wxPoint hp,int Mode,float Ratio = 0);
    //! Draw size boxes for resize
    void DrawSizeRect(wxDC &dc);
    //! Determine in which size box the pouse position hp is if any
    int PointInSizeRect(wxPoint hp);
    //! Perform hiding/showing size rectangles
    void ChangeSizerView(bool show);

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxResizeableControl)
};

// ----------------------------------------------------------------------------
// wxPictureControl
// ----------------------------------------------------------------------------

//! Abstract class for pictures
/*! This would make a perfect abstract class, but the
    definition of wxDynamicCast() doesn't allow this.
*/
class wxPictureControl : public wxResizeableControl
{
 public:
    //! Ctor
    wxPictureControl() : wxResizeableControl() {};
    //! Ctor
    wxPictureControl(wxWindow *AParent, int AnId,const wxPoint &pos,
                      const wxSize &size,long style = 0,
                      const wxString &name = wxPanelNameStr)
      : wxResizeableControl(AParent,AnId,pos,size,style,name) {};
    //! Return picture type string
    virtual const wxChar *GetPictureType() {return wxT("NOTYPE");}
    //! Return picture data size
    virtual long GetPictureSize() {return 0;}
    //! Copy picture data into pt
    virtual long GetPictureData(char *,long) {return 0;}

// Event handlers

    //! Reset original size if available
    void OnRevert(wxCommandEvent &event);
    //! Show context menu
    void OnRightDown(wxMouseEvent &event);
    //! Respond to system color palette change event
    void OnPaletteChanged(wxSysColourChangedEvent &event);
    //! Cut event
    void OnEditCut(wxCommandEvent &ce);
    //! Copy event
    virtual void OnEditCopy(wxCommandEvent &WXUNUSED(ce)) {}
    //! Enable cut event
    void CeEditCut(wxUpdateUIEvent &ce)
    {
        ce.Enable(true);
    }
    //! Enable copy event
    void CeEditCopy(wxUpdateUIEvent &ce)
    {
        ce.Enable(true);
    }
    //! Reponds to size event
    void OnSize(wxSizeEvent &event);
    //! Responds to paint event
    void OnPaint(wxPaintEvent &event);

 protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxPictureControl)
};


#if defined(__WXMSW__) && wxUSE_METAFILE && wxUSE_ENH_METAFILE
// ----------------------------------------------------------------------------
// wxMetafileControl
// ----------------------------------------------------------------------------

//! Class for EnhMetaFile pictures
class wxMetafileControl : public wxPictureControl
{
 public:
    //! Ctor
    wxMetafileControl() : wxPictureControl() {};
    //! Ctor
    wxMetafileControl(wxWindow *AParent, int AnId,WXHANDLE Meta,
                      const wxPoint &pos,
                      const wxSize &size,long style = 0,
                      const wxString &name = wxPanelNameStr);
    //! Ctor with raw picture data
    wxMetafileControl(wxWindow *AParent, int AnId,char *Data,int Size,
                      const wxPoint &pos,const wxSize &size,long style = 0,
                      const wxString &name = wxPanelNameStr);
    //! Dtor
    virtual ~wxMetafileControl();
    //! Return picture type name
    const wxChar *GetPictureType()
    {
        return wxT("EnhMetaFile");
    }
    //! Returns picture data size
    long GetPictureSize();
    //! Copy picture data into data
    long GetPictureData(char *data,long n);
    //! Return the picture in bitmap format
    const wxEnhMetaFile &GetMetafile() { return m_metafile; }
    //! Paint function for printing
    void Paint(wxDC &dc,bool Printing,wxSize &size);
    //! Returns with/height ratio
    float GetRatio();
    //! Returns original size of control
    wxSize GetOriginalSize();

// Event handlers

    //! Responds to copy event
    void OnEditCopy(wxCommandEvent &event);

 protected:
    //! Displayed metafile
    wxEnhMetaFile m_metafile;

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxMetafileControl)
};

#endif

// ----------------------------------------------------------------------------
// wxBitmapControl
// ----------------------------------------------------------------------------

//! Class for Bitmaps
class wxBitmapControl : public wxPictureControl
{
 public:
    //! Ctor
    wxBitmapControl() : wxPictureControl(),m_bitmap(0) {};
    //! Ctor
    wxBitmapControl(wxWindow *AParent, int AnId,const wxBitmap &Data,
                    const wxPoint &pos,
                    const wxSize &size,long style = 0,
                    const wxString &name = wxPanelNameStr);
#ifdef __WXMSW__
    //! Ctor with raw picture data in DIB format
    wxBitmapControl(wxWindow *AParent, int AnId,char *Data,int Size,
                    const wxPoint &pos,const wxSize &size,long style = 0,
                    const wxString &name = wxPanelNameStr);
#endif
    //! Dtor
    virtual ~wxBitmapControl();
#ifdef __WXMSW__
    //! Return picture type name
    const wxChar *GetPictureType()
    {
        return wxT("Dib");
    }
    //! Returns picture data size
    long GetPictureSize();
    //! Copy picture data into data
    long GetPictureData(char *data,long n);
#endif
    //! Return the picture in bitmap format
    const wxBitmap &GetBitmap() { return *m_bitmap; }
    //! Paint function for printing
    void Paint(wxDC &dc,bool Printing,wxSize &size);
    //! Returns width/height ratio
    float GetRatio();
    //! Returns original size of control
    wxSize GetOriginalSize();

// Event handlers

    //! Responds to copy event
    void OnEditCopy(wxCommandEvent &event);

 protected:
    //! Bitmap to be displayed
    wxBitmap *m_bitmap;

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxBitmapControl)
};

// ----------------------------------------------------------------------------
// wxResizeableParentControl
// ----------------------------------------------------------------------------

//! Derived class from wxResizableControl that has a child
class wxResizeableParentControl : public wxResizeableControl
{
  protected:
    //! Child window which is automatically resized
    wxWindow *m_child;

  public:
    //! Ctor
    wxResizeableParentControl() : wxResizeableControl() {m_child = 0;}
    //! Ctor
    wxResizeableParentControl(wxWindow *AParent, int AnId,
                              const wxPoint &pos,
                              const wxSize &size,long style = 0,
                              const wxString &name = wxPanelNameStr)
      : wxResizeableControl(AParent,AnId,pos,size,style,name)
    {
        m_child = 0;
    }

// Event handlers
    //! Responds to size event
    void OnSize(wxSizeEvent &event);

	//! Draws size rect when this window has the focus
	//! (like when the user uses one of our sizer to resize the child window).
    void OnPaint(wxPaintEvent &event);

	//! Drops sibling events to the child window.
	void OnSiblingChange(wxCommandEvent &event);

    //! Sets the given window as our managed window.
	//! A wxResizeableParentControl can contain only a single managed child window
	//! at time.
    void SetManagedChild(wxWindow *child)
    {
        m_child = child;
        if (m_child)
            m_child->SetParent(this);

		// required to adapt this window to this child
        wxSizeEvent evt(GetSize());
        AddPendingEvent(evt);
    }

 protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxResizeableParentControl)
};

// ----------------------------------------------------------------------------
// wxResizeableChildTextCtrl
// ----------------------------------------------------------------------------

//! Text control that enables the size boxes of a parent wxResizeableParentControl on focus
class wxResizeableChildTextCtrl : public wxTextCtrl
{
   public:
    //! Ctor
    wxResizeableChildTextCtrl() : wxTextCtrl() {}
    //! Ctor
    wxResizeableChildTextCtrl(wxWindow *AParent, int AnId,const wxString &text,
                               const wxPoint &pos = wxDefaultPosition,
                               const wxSize &size = wxDefaultSize,long style = 0,
                               const wxString &name = wxPanelNameStr)
      : wxTextCtrl(AParent,AnId,text,pos,size,style,wxDefaultValidator,name)
    {
    }
    //! Enable parent sizers on set focus
    void OnSetFocus(wxFocusEvent &);
    //! Disable parent sizers on set focus
    void OnKillFocus(wxFocusEvent &);

    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// wxResizeableControlCanvas
// ----------------------------------------------------------------------------

WX_DECLARE_USER_EXPORTED_LIST(wxResizeableControl, wxResizeableControlList, WXDLLIMPEXP_RESIZEC);

//! Parent window class for wxResizableControl child windows.
/*! This is a demo implementation that shows the typical operations
    that are necessary to support wxResizableControl child windows.
*/
class wxResizeableControlCanvas : public wxScrolledWindow
{
protected:

    //! Flag for scroll status
    bool m_scrollflag;

public:
    //! Ctor
    wxResizeableControlCanvas() : wxScrolledWindow() {}
    //! Ctor
    wxResizeableControlCanvas(wxWindow *AParent, int AnId,
                              const wxPoint &pos = wxDefaultPosition,
                              const wxSize &size = wxDefaultSize,long style = 0,
                              const wxString &name = wxPanelNameStr)
      : wxScrolledWindow(AParent,AnId,pos,size,style,name) 
    {
      m_scrollflag = false;
    }


	//! Returns the list of our resizeable controls.
	wxResizeableControlList &GetControlList()
		{ return (wxResizeableControlList&)GetChildren(); }

    //! Respond to child window notification
    void OnChildWindowChange(wxCommandEvent &);
    //! Update scroll range on idle
    void OnIdle(wxIdleEvent &);
    //! Update scrollbars
    void UpdateScrollRange();

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxResizeableControlCanvas)
};

// ----------------------------------------------------------------------------
// Events
// ----------------------------------------------------------------------------

// helper macro
#define EVT_RESIZEABLE(evtname, id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_COMMAND_##evtname, id, -1, \
        (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction) \
        & fn, \
        (wxObject *) NULL \
    ),

//! Events from child windows of wxResizeableControlCanvas
/*! These events notify the parent window of changes, so that it can
    update itself accordingly (recalculate Scrollbars, set document modified
    flag and so on).
*/
BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_CHILD_MOVED, 520)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_CHILD_CLOSED, 521)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_CHILD_RESIZED, 522)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_CHILD_CREATED, 523)
END_DECLARE_EVENT_TYPES()

#define EVT_CHILD_MOVED(id, fn) \
    EVT_RESIZEABLE(CHILD_MOVED, id, fn)

#define EVT_CHILD_CLOSED(id, fn) \
    EVT_RESIZEABLE(CHILD_CLOSED, id, fn)

#define EVT_CHILD_RESIZED(id, fn) \
    EVT_RESIZEABLE(CHILD_RESIZED, id, fn)

#define EVT_CHILD_CREATED(id, fn) \
    EVT_RESIZEABLE(CHILD_CREATED, id, fn)

#define EVT_CHILD_CHANGED(id, fn)	\
	EVT_CHILD_CREATED(id, fn)		\
    EVT_CHILD_MOVED(id, fn)			\
    EVT_CHILD_RESIZED(id, fn)		\
    EVT_CHILD_CLOSED(id, fn)

						


//! Events from sibling windows
/*! These events notify the resizeable controls of changes in other controls
    placed in the same wxResizeableControlCanvas.
*/
BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(wxEVT_COMMAND_SIBLING_MOVED, 524)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_SIBLING_CLOSED, 525)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_SIBLING_RESIZED, 526)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_SIBLING_CREATED, 527)
END_DECLARE_EVENT_TYPES()

#define EVT_SIBLING_MOVED(id, fn) \
    EVT_RESIZEABLE(SIBLING_MOVED, id, fn)

#define EVT_SIBLING_CLOSED(id, fn) \
    EVT_RESIZEABLE(SIBLING_CLOSED, id, fn)

#define EVT_SIBLING_RESIZED(id, fn) \
    EVT_RESIZEABLE(SIBLING_RESIZED, id, fn)

#define EVT_SIBLING_CREATED(id, fn) \
    EVT_RESIZEABLE(SIBLING_CREATED, id, fn)

#define EVT_SIBLING_CHANGED(id, fn)	\
	EVT_SIBLING_CREATED(id, fn)		\
    EVT_SIBLING_MOVED(id, fn)		\
    EVT_SIBLING_RESIZED(id, fn)		\
    EVT_SIBLING_CLOSED(id, fn)

//! Events from child windows of wxResizeableControl
/*! Allow child windows of wxResizeableControl to set/unset the m_hasfocus
    flag in wxResizeableControl, so that the size blocks are shown even when
    the focus is in the child control
*/
BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_SHOW_SIZERS, 528)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_HIDE_SIZERS, 529)
END_DECLARE_EVENT_TYPES()

#define EVT_SHOW_SIZERS(id, fn) \
    EVT_RESIZEABLE(SHOW_SIZERS, id, fn)

#define EVT_HIDE_SIZERS(id, fn) \
    EVT_RESIZEABLE(HIDE_SIZERS, id, fn)


#endif // __RESIZECONTROL__

