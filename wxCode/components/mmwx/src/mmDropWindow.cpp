//! \file mmDropWindow.cpp
//
// Name     : mmDropWindow
// Purpose  : This is a button that can drop down any wxWindow.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#include <wx/mmwxdef.h>
#include "wx/mmDropWindow.h"

#ifdef __MMDEBUG__
	WXDLLIMPEXP_DATA_MMWX(extern wxTextCtrl*) gDebug;    //!< Global debug output window.
#endif

//////////////////////////////////////////////////////////////////////////////

/*! \brief Check to see if the mouse is inside a window.
 *
 * \param window 	wxWindow*	The parent window.
 *
 */
WXDLLIMPEXP_MMWX bool MouseIsInside(wxWindow *window)
{
    // Check to see if mouse button is inside or outside window
    if(!window)
        return FALSE;
    int cx = 0, cy = 0;
    int cw = 0, ch = 0;
    window->ClientToScreen(&cx, &cy);
    window->GetSize(&cw, &ch);
    int mpx, mpy;
    ::wxGetMousePosition(&mpx, &mpy);
    if(mpx >= cx && mpx <= cx + cw && mpy >= cy && mpy <= cy + ch)
        return TRUE;
    return FALSE;
} // MouseIsInside

//////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(mmDropWindow, mmMultiButton)
    EVT_COMMAND(-1, mmEVT_WHOLEDROP_TOGGLE, mmDropWindow::OnDrop)
    EVT_COMMAND(-1, mmEVT_DROP_TOGGLE,     mmDropWindow::OnDrop)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////////

/*! \brief Constructor.
 *
 * \param parent 			wxWindow*				The parent window.
 * \param id 					const wxWindowID	The ID of this window.
 * \param titleStr 			const wxString&		The text to display on the button.
 * \param titleBitmap 		const wxBitmap&		The bitmap to display on the button.
 * \param pos 				const wxPoint&			The button's position.
 * \param size 				const wxSize&			The button's size.
 * \param style 				const longint				The button's style.
 * \param childWindow 	wxWindow*				The child window.
 * \param childSize 		const wxSize&			The child window's size
 *
 */
mmDropWindow::mmDropWindow(wxWindow *parent,
                           const wxWindowID id,
                           const wxString  &titleStr,
                           const wxBitmap  &titleBitmap,
                           const wxPoint   &pos,
                           const wxSize    &size,
                           const long int   style,
                           wxWindow  *childWindow,
                           const wxSize    &childSize)
    : mmMultiButton(parent, id, titleStr, titleBitmap, pos, size, style | mmMB_AUTODRAW)
{
    mParent = new wxFrame(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP | wxSIMPLE_BORDER | wxNO_3D);
    mChildHandlerIsPushed = FALSE;
    SetChild(childWindow, childSize);
} // Constructor

/*! \brief Destructor.
 */
mmDropWindow::~mmDropWindow()
{
    if(mParent) mParent->Destroy();
} // Destructor

/*! \brief Set the button's child window.
 *
 * \param childWindow 	wxWindow*		The child window.
 * \param childSize 		const wxSize&	The child window's size.
 * \return void
 *
 */
void mmDropWindow::SetChild(wxWindow *childWindow, const wxSize &childSize)
{
    mChild = childWindow;
    if(!mParent || !mChild)
        return;
    if(childSize.GetWidth() != -1 && childSize.GetHeight() != -1)
        mChildSize = childSize; // Use specified size, if given...
    else if(mChild)
        mChildSize = mChild->GetSize(); // ...otherwise use child's size
    else
        mChildSize = wxDefaultSize;
    mParent->SetSize(mChildSize);
    mChild->SetSize(mChildSize);
    mChild->Reparent(mParent);
    if(!mChildHandlerIsPushed)
    {
        mChildHandlerIsPushed = TRUE; // Push event handlers only once!
        mChild->PushEventHandler(new mmChildHandler(this, mChild));
    }
} // SetChild

/*! \brief
 *
 * \param event wxCommandEvent&	A reference to a wxCommandEvent object.
 * \return void
 *
 */
void mmDropWindow::OnDrop(wxCommandEvent &event)
{
#ifdef __MMDEBUG__
    *gDebug << wxT("mmDropWindow::OnDrop,type=") << event.GetEventType() << wxT("\n");
#endif
    if(event.GetEventType() == mmEVT_DROP_TOGGLE || event.GetEventType() == mmEVT_WHOLEDROP_TOGGLE)
        ToggleChild();
} // OnDrop

/*! \brief Toggle the child window.
 *
 * \return void
 *
 */
void mmDropWindow::ToggleChild()
{
    if(!mParent || !mChild)
        return;
    if(!mParent->IsShown())
    {
        mParent->Move(ClientToScreen(wxPoint(0, GetSize().y)));
        mParent->Show(TRUE);
    }
    else
        //mParent->Dismiss();
        mParent->Show(FALSE);
    if(MouseIsInside(mParent))
    {
        SetWholeDropToggleDown(TRUE);
        SetDropToggleDown(TRUE);
        SetFocus(TRUE);
    }
    mChild->SetFocus();
    if(HasCapture()) {
        ReleaseMouse();
    }
} // ToggleChild

//////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(mmChildHandler, wxEvtHandler)
    EVT_KILL_FOCUS(mmChildHandler::OnChildKillFocus)
    EVT_ENTER_WINDOW(mmChildHandler::OnMouseEnterLeaveChild)
    EVT_LEAVE_WINDOW(mmChildHandler::OnMouseEnterLeaveChild)
    EVT_MOUSE_EVENTS(mmChildHandler::OnMouse)
    EVT_MOUSE_CAPTURE_LOST(mmChildHandler::OnCaptureLost)
END_EVENT_TABLE()

/*! \brief Constructor.
 *
 * \param button mmDropWindow*	The drop window button hosting the window.
 * \param child 	wxWindow*			The window that's being handled.
 *
 */
mmChildHandler::mmChildHandler(mmDropWindow *button, wxWindow *child)
{
    mButton = button;
    mChild = child;
} // mmChildHandler

/*! \brief The child window lost focus.
 *
 * \param event wxFocusEvent&	A reference to a wxFocusEvent object.
 * \return void
 *
 */
void mmChildHandler::OnChildKillFocus(wxFocusEvent &event)
{
    if(!mChild)
        return;
    if(!MouseIsInside(mChild))
    {
        if(mButton && !mButton->MouseIsOnButton())
        {
            mChild->GetParent()->Show(FALSE);
            mButton->SetWholeDropToggleDown(FALSE);
            mButton->SetDropToggleDown(FALSE);
            mButton->SetFocus(FALSE);
        }
    }
    event.Skip();
} // OnChildKillFocus

/*! \brief The mouse entered or left the child window.
 *
 * \param event wxMouseEvent&	A reference to a wxMouseEvent object.
 * \return void
 *
 */
void mmChildHandler::OnMouseEnterLeaveChild(wxMouseEvent &event)
{
    if(event.GetEventType() == wxEVT_ENTER_WINDOW)
    {
        if(mChild) {
            mChild->SetFocus();
        }
    }
    else if(event.GetEventType() == wxEVT_LEAVE_WINDOW)
    {
        ReleaseCapture();
    }
    event.Skip();
} // OnMouseEnterLeaveChild

/*! \brief A mouse event occurred.
 *
 * \param event wxMouseEvent&	A reference to a wxMouseEvent object.
 * \return void
 *
 */
void mmChildHandler::OnMouse(wxMouseEvent &event)
{
    event.Skip();
}

/*! \brief The child window lost the mouse capture.
 *
 * \param event wxMouseCaptureLostEvent&	A reference to a wxMouseCaptureLostEvent object.
 * \return void
 *
 */
void mmChildHandler::OnCaptureLost(wxMouseCaptureLostEvent &event)
{
    if(mChild->HasCapture()) {
        ReleaseCapture();
    }
}

