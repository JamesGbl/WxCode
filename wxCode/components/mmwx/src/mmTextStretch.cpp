//! \file mmTextStretch.cpp
//
// Name     : mmTextStretch
// Purpose  : A wxTextCtrl that can be stretched and/or moved by
//            using the left mouse button.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#include <wx/mmwxdef.h>
#include "wx/mmTextStretch.h"

#ifdef __MMDEBUG__
	WXDLLIMPEXP_DATA_MMWX(extern wxTextCtrl*) gDebug;    //!< Global debug output window.
#endif

BEGIN_EVENT_TABLE(mmTextStretch, wxWindow)
    EVT_MOUSE_EVENTS(mmTextStretch::OnMouse)
    EVT_PAINT(mmTextStretch::OnPaint)
    EVT_KEY_DOWN(mmTextStretch::OnKeyDown)
    EVT_MOUSE_CAPTURE_LOST(mmTextStretch::OnCaptureLost)
END_EVENT_TABLE()

/*! \brief Constructor.
 *
 * \param parent wxWindow*				The parent window.
 * \param id 		const wxWindowID	The ID of this window.
 * \param value 	wxString&					The text to display.
 * \param pos 	const wxPoint&			The button's position.
 * \param size 	const wxSize&			The button's size.
 * \param style 	const longint				The button's style.
 *
 */
mmTextStretch::mmTextStretch(wxWindow *parent,
                             const wxWindowID id,
                             const wxString &value,
                             const wxPoint &pos,
                             const wxSize &size,
                             const long style)
    : wxWindow(parent, id, pos, size, style | wxSIMPLE_BORDER)
{
    mStyle = style;
    mLeftIsDown = FALSE;
    mTopLeftWasPressed = FALSE;
    mBottomRightWasPressed = FALSE;
    mNormalCursor   = new wxCursor(wxCURSOR_ARROW);
    mSizeNWSECursor = new wxCursor(wxCURSOR_SIZENWSE);
    mSizeNSCursor   = new wxCursor(wxCURSOR_SIZENS);
    mSizeWECursor   = new wxCursor(wxCURSOR_SIZEWE);
    mMoveCursor     = new wxCursor(wxCURSOR_HAND);

    int w, h;
    GetClientSize(&w, &h);
    mMargin = 3;
    mGridSize = 10;
    mTextCtrl = new wxTextCtrl(this, id, value, wxDefaultPosition, wxDefaultSize, style | wxSIMPLE_BORDER);
    wxLayoutConstraints *con = new wxLayoutConstraints;
    con->top.SameAs(this, wxTop, mMargin);
    con->bottom.SameAs(this, wxBottom, mMargin);
    con->left.SameAs(this, wxLeft, mMargin);
    con->right.SameAs(this, wxRight, mMargin);
    mTextCtrl->SetConstraints(con);

    SetAutoLayout(TRUE);
    Layout();
} // Constructor

/*! \brief Destructor.
 */
mmTextStretch::~mmTextStretch(void)
{
} // Destructor

/*! \brief A key was pressed.
 *
 * \param event wxKeyEvent&	A reference to a wxKeyEvent object.
 * \return void
 *
 */
void mmTextStretch::OnKeyDown(wxKeyEvent &event)
{
    if(event.ControlDown())
    {
        mCtrlIsDown = TRUE;
        SetCursor(*mMoveCursor);
        wxPoint tp = GetPosition();
    }
    else
    {
        mCtrlIsDown = FALSE;
        if(!mLeftIsDown)
            SetCursor(*mNormalCursor);
    }
    event.Skip();
} // OnKeyDown

/*! \brief A mouse event occurred.
 *
 * \param event wxMouseEvent&	A reference to a wxMouseEvent object.
 * \return void
 *
 */
void mmTextStretch::OnMouse(wxMouseEvent &event)
{
    if(event.Leaving())
    {
        if(!mLeftIsDown)
            SetCursor(*mNormalCursor);
    }
    else if(event.Entering() && !mCtrlIsDown)
    {
        if((MouseIsOnTopLeft() || MouseIsOnBottomRight()) && !mLeftIsDown)
        {
            if((mStyle & mmTS_SIZE_TOP    && mStyle & mmTS_SIZE_LEFT) || (mStyle & mmTS_SIZE_BOTTOM && mStyle & mmTS_SIZE_RIGHT))
                SetCursor(*mSizeNWSECursor);
        }
        else if((MouseIsOnTop() || MouseIsOnBottom()) && !mLeftIsDown)
        {
            if(mStyle & mmTS_SIZE_TOP || mStyle & mmTS_SIZE_BOTTOM)
                SetCursor(*mSizeNSCursor);
        }
        else if((MouseIsOnLeft() || MouseIsOnRight()) && !mLeftIsDown)
        {
            if(mStyle & mmTS_SIZE_LEFT || mStyle & mmTS_SIZE_RIGHT)
                SetCursor(*mSizeWECursor);
        }
    }
    else if(event.LeftDown())
    {
        if(MouseIsOnTopLeft()     && (mStyle & mmTS_SIZE_TOP    && mStyle & mmTS_SIZE_LEFT))
            mTopLeftWasPressed = TRUE;
        else if(MouseIsOnBottomRight() && (mStyle & mmTS_SIZE_BOTTOM && mStyle & mmTS_SIZE_RIGHT))
            mBottomRightWasPressed = TRUE;
        if(MouseIsOnLeft() && (mStyle & mmTS_SIZE_LEFT))
            mLeftWasPressed = TRUE;
        else if(MouseIsOnRight() && (mStyle & mmTS_SIZE_RIGHT))
            mRightWasPressed = TRUE;
        else if(MouseIsOnTop() && (mStyle & mmTS_SIZE_TOP))
            mTopWasPressed = TRUE;
        else if(MouseIsOnBottom() && (mStyle & mmTS_SIZE_BOTTOM))
            mBottomWasPressed = TRUE;
        mLeftIsDown = TRUE;
        CaptureMouse();
    }
    else if(event.LeftUp())
    {
        mLeftIsDown = FALSE;
        mCtrlIsDown = FALSE;
        mTopLeftWasPressed = FALSE;
        mBottomRightWasPressed = FALSE;
        mTopWasPressed = FALSE;
        mBottomWasPressed = FALSE;
        mLeftWasPressed = FALSE;
        mRightWasPressed = FALSE;
        if(!MouseIsOnBorder())
            SetCursor(*mNormalCursor);
        ReleaseCapture();
    }
    else if(event.Dragging() /*&& mLeftIsDown*/)
    {
        CaptureMouse();
        int mpx, mpy;
        ::wxGetMousePosition(&mpx, &mpy);
        GetParent()->ScreenToClient(&mpx, &mpy);
        if(mTopLeftWasPressed || mBottomRightWasPressed || mTopWasPressed || mBottomWasPressed || mLeftWasPressed || mRightWasPressed)
        {
            int x = 0, y = 0, w = 0, h = 0;
            wxSize  ts = GetSize();
            wxPoint tp = GetPosition();
            if(mpx < 0)
                mpx = 0;
            if(mpy < 0)
                mpy = 0;
            if(mTopLeftWasPressed)
            {
                if(!(mStyle & mmTS_SNAP_TO_GRID) ||
                        (abs(mpy - tp.y) > mGridSize || abs(mpx - tp.x) > mGridSize))
                {
                    x = mpx;
                    y = mpy;
                    w = ts.GetWidth() + tp.x - x;
                    h = ts.GetHeight() + tp.y - y;
                }
                else
                {
                    x = tp.x;
                    y = tp.y;
                    w = ts.GetWidth();
                    h = ts.GetHeight();
                }
            }
            else if(mBottomRightWasPressed)
            {
                if(!(mStyle & mmTS_SNAP_TO_GRID) || (abs(mpy - tp.y - ts.GetHeight()) > mGridSize || abs(mpx - tp.x - ts.GetWidth()) > mGridSize))
                {
                    x = tp.x;
                    y = tp.y;
                    w = mpx - x;
                    h = mpy - y;
                }
                else
                {
                    x = tp.x;
                    y = tp.y;
                    w = ts.GetWidth();
                    h = ts.GetHeight();
                }
            }
            else if(mTopWasPressed)
            {
                x = tp.x;
                if(!(mStyle & mmTS_SNAP_TO_GRID) || abs(mpy - tp.y) > mGridSize)
                    y = mpy;
                else
                    y = tp.y;
                w = ts.GetWidth();
                h = ts.GetHeight() + tp.y - y;
            }
            else if(mBottomWasPressed)
            {
                x = tp.x;
                if(mCtrlIsDown && (mStyle & mmTS_SNAP_TO_GRID) && abs(mpy - tp.y - ts.GetHeight()) > mGridSize)
                    y = mpy - ts.GetHeight();
                else
                    y = tp.y;
                w = ts.GetWidth();
                if(!(mStyle & mmTS_SNAP_TO_GRID) || abs(mpy - tp.y - ts.GetHeight()) > mGridSize)
                    h = mpy - y;
                else
                    h = ts.GetHeight();
            }
            else if(mLeftWasPressed)
            {
                if(!(mStyle & mmTS_SNAP_TO_GRID) || abs(mpx - tp.x) > mGridSize)
                    x = mpx;
                else
                    x = tp.x;
                y = tp.y;
                w = ts.GetWidth() + tp.x - x;
                h = ts.GetHeight();
            }
            else if(mRightWasPressed)
            {
                if(mCtrlIsDown && (mStyle & mmTS_SNAP_TO_GRID) && abs(mpx - tp.x - ts.GetWidth()) > mGridSize)
                    x = mpx - ts.GetWidth();
                else
                    x = tp.x;
                y = tp.y;
                h = ts.GetHeight();
                if(!(mStyle & mmTS_SNAP_TO_GRID) || abs(mpx - tp.x - ts.GetWidth()) > mGridSize)
                    w = mpx - x;
                else
                    w = ts.GetWidth();
            }
            if(w > mMargin && h > mMargin)
            {
                if(mCtrlIsDown)
                    Move(x, y);
                else
                    SetSize(x, y, w, h);
                GetParent()->ClearBackground();   // This method..
                GetParent()->Refresh(); // ...gives flicker
                Layout();
            }
        }
    }

    event.Skip();
} // OnMouse

/*! \brief The child window lost the mouse capture.
 *
 * \param event wxMouseCaptureLostEvent&	A reference to a wxMouseCaptureLostEvent object.
 * \return void
 *
 */
void mmTextStretch::OnCaptureLost(wxMouseCaptureLostEvent &event)
{
    if(HasCapture()) {
        ReleaseMouse();
    }
}

/*! \brief Check whether the mouse is hovering over one of the control's borders.
 *
 * \return bool True or false.
 *
 */
bool mmTextStretch::MouseIsOnBorder()
{
    int w, h;
    GetClientSize(&w, &h);
    int cx = 0, cy = 0;
    ClientToScreen(&cx, &cy);
    int mpx, mpy;
    ::wxGetMousePosition(&mpx, &mpy);
    if((mpx >= cx && mpx <= cx + mMargin && mpy >= cy && mpy <= cy + h) ||
            (mpx >= cx + w - mMargin && mpx <= cx + w && mpy >= cy && mpy <= cy + h) ||
            (mpx >= cx && mpx <= cx + w && mpy >= cy && mpy <= cy + mMargin) ||
            (mpx >= cx && mpx <= cx + w && mpy >= cy + h - mMargin && mpy <= cy + h))
        return TRUE;
    return FALSE;
}

/*! \brief Check whether the mouse is on the top left corner.
 *
 * \return bool True or false.
 *
 */
bool mmTextStretch::MouseIsOnTopLeft()
{
    int cx = 0, cy = 0;
    ClientToScreen(&cx, &cy);
    int mpx, mpy;
    ::wxGetMousePosition(&mpx, &mpy);
    int marg = 5;
    if(mpx >= cx && mpx <= cx + marg && mpy >= cy && mpy <= cy + marg)
        return TRUE;
    return FALSE;
}

/*! \brief Check whether the mouse is on the bottom right corner.
 *
 * \return bool True or false.
 *
 */
bool mmTextStretch::MouseIsOnBottomRight()
{
    int w, h;
    GetClientSize(&w, &h);
    int cx = 0, cy = 0;
    ClientToScreen(&cx, &cy);
    int mpx, mpy;
    ::wxGetMousePosition(&mpx, &mpy);
    int marg = 5;
    if(mpx >= cx + w - marg && mpx <= cx + w && mpy >= cy + h - marg && mpy <= cy + h)
        return TRUE;
    return FALSE;
}

/*! \brief Check whether the mouse is on the left border.
 *
 * \return bool True or false.
 *
 */
bool mmTextStretch::MouseIsOnLeft()
{
    int w, h;
    GetClientSize(&w, &h);
    int cx = 0, cy = 0;
    ClientToScreen(&cx, &cy);
    int mpx, mpy;
    ::wxGetMousePosition(&mpx, &mpy);
    int marg = 5;
    if(mpx >= cx && mpx <= cx + marg && mpy >= cy + marg && mpy <= cy + h - marg)
        return TRUE;
    return FALSE;
}

/*! \brief Check whether the mouse is on the right border.
 *
 * \return bool True or false.
 *
 */
bool mmTextStretch::MouseIsOnRight()
{
    int w, h;
    GetClientSize(&w, &h);
    int cx = 0, cy = 0;
    ClientToScreen(&cx, &cy);
    int mpx, mpy;
    ::wxGetMousePosition(&mpx, &mpy);
    int marg = 5;
    if(mpx >= cx + w - marg && mpx <= cx + w && mpy >= cy + marg && mpy <= cy + h - marg)
        return TRUE;
    return FALSE;
}

/*! \brief Check whether the mouse is on the top border.
 *
 * \return bool True or false.
 *
 */
bool mmTextStretch::MouseIsOnTop()
{
    int w, h;
    GetClientSize(&w, &h);
    int cx = 0, cy = 0;
    ClientToScreen(&cx, &cy);
    int mpx, mpy;
    ::wxGetMousePosition(&mpx, &mpy);
    int marg = 5;
    if(mpx >= cx + marg && mpx <= cx + w - marg && mpy >= cy && mpy <= cy + marg)
        return TRUE;
    return FALSE;
}

/*! \brief Check whether the mouse is on the bottom border.
 *
 * \return bool True or false.
 *
 */
bool mmTextStretch::MouseIsOnBottom()
{
    int w, h;
    GetClientSize(&w, &h);
    int cx = 0, cy = 0;
    ClientToScreen(&cx, &cy);
    int mpx, mpy;
    ::wxGetMousePosition(&mpx, &mpy);
    int marg = 5;
    if(mpx >= cx + marg && mpx <= cx + w - marg && mpy >= cy + h - marg && mpy <= cy + h)
        return TRUE;
    return FALSE;
}

/*! \brief A paint event occurred.
 *
 * \param event wxPaintEvent&	A reference to a wxPaintEvent object.
 * \return void
 *
 */
void mmTextStretch::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    int w, h;
    GetClientSize(&w, &h);
    if(mStyle & mmTS_SIZE_RIGHT && mStyle & mmTS_SIZE_BOTTOM)
    {
        dc.SetPen(*wxMEDIUM_GREY_PEN);
        dc.DrawLine(w - 3, h, w, h - 3);
        dc.DrawLine(w - 5, h, w, h - 5);
        dc.DrawLine(w - 7, h, w, h - 7);
        dc.SetPen(*wxWHITE_PEN);
        dc.DrawLine(w - 4, h, w, h - 4);
        dc.DrawLine(w - 6, h, w, h - 6);
        dc.DrawLine(w - 8, h, w, h - 8);
    }
    if(mStyle & mmTS_SIZE_LEFT && mStyle & mmTS_SIZE_TOP)
    {
        dc.SetPen(*wxMEDIUM_GREY_PEN);
        dc.DrawLine(0, 2, 3, -1);
        dc.DrawLine(0, 4, 5, -1);
        dc.DrawLine(0, 6, 7, -1);
        dc.SetPen(*wxWHITE_PEN);
        dc.DrawLine(0, 3, 4, -1);
        dc.DrawLine(0, 5, 6, -1);
        dc.DrawLine(0, 7, 8, -1);
    }
} // OnPaint

