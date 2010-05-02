//
// Name     : mmDropWindow
// Purpose  : This is a button that can drop down any wxWindow.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//

#include "mmDropWindow.h"

#ifdef __MMDEBUG__
extern wxTextCtrl* gDebug;
#endif

//////////////////////////////////////////////////////////////////////////////

bool MouseIsInside(wxWindow* window)
{
  // Check to see if mouse button is inside or outside window
  if (!window)
    return FALSE;
  int cx=0,cy=0;
  int cw=0,ch=0;
  window->ClientToScreen(&cx,&cy);
  window->GetSize(&cw,&ch);
  int mpx,mpy;
  ::wxGetMousePosition(&mpx,&mpy);
  if (mpx >= cx && mpx <= cx + cw &&
      mpy >= cy && mpy <= cy + ch)
    return TRUE;
  return FALSE;
} // MouseIsInside

//////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(mmDropWindow,mmMultiButton)
  EVT_COMMAND(-1,mmEVT_WHOLEDROP_TOGGLE,mmDropWindow::OnDrop)
  EVT_COMMAND(-1,mmEVT_DROP_TOGGLE,     mmDropWindow::OnDrop)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////////

mmDropWindow::mmDropWindow(wxWindow* parent,
                           const wxWindowID id,
                           const wxString&  titleStr,
			   const wxBitmap&  titleBitmap,
                           const wxPoint&   pos,
                           const wxSize&    size,
                           const long int   style,
                                 wxWindow*  childWindow,
                           const wxSize&    childSize)
: mmMultiButton(parent,id,titleStr,titleBitmap,pos,size,style|mmMB_AUTODRAW)
{
  mStyle = style;
  mParent = new wxFrame(this,-1,"",wxDefaultPosition,wxDefaultSize,
                        wxSTAY_ON_TOP|wxSIMPLE_BORDER|wxNO_3D);
  //mParent = new wxPopupTransientWindow(this);
  mChildHandlerIsPushed = FALSE;
  SetChild(childWindow,childSize);
} // Constructor

mmDropWindow::~mmDropWindow()
{
  if (mParent) mParent->Destroy();
} // Destructor

void mmDropWindow::SetChild(wxWindow* childWindow,
                            const wxSize& childSize)
{
  mChild = childWindow;
  if (!mParent || !mChild)
    return;
  if (childSize.GetWidth() != -1 && childSize.GetHeight() != -1)
    mChildSize = childSize; // Use specified size, if given...
  else
  if (mChild)
    mChildSize = mChild->GetSize(); // ...otherwise use child's size
  else
    mChildSize = wxDefaultSize;
  mParent->SetSize(mChildSize);
  mChild->SetSize(mChildSize);
  mChild->Reparent(mParent);
  if (!mChildHandlerIsPushed)
  {
    mChildHandlerIsPushed = TRUE; // Push event handlers only once!
    mChild->PushEventHandler(new mmChildHandler(this,mChild));
  }
} // SetChild

void mmDropWindow::OnDrop(wxMouseEvent& event)
{
#ifdef __MMDEBUG__
  *gDebug<<"mmDropWindow::OnDrop,type="<<event.GetEventType()<<"\n";
#endif
  if (event.GetEventType() == mmEVT_DROP_TOGGLE ||
      event.GetEventType() == mmEVT_WHOLEDROP_TOGGLE)
    ToggleChild();
} // OnDrop

void mmDropWindow::ToggleChild()
{
  if (!mParent || !mChild)
    return;
  if (!mParent->IsShown())
  {
    //mParent->Position(ClientToScreen(wxPoint(0,GetSize().y)),wxSize(0,0));
    //mParent->Popup(mChild);
    mParent->Move(ClientToScreen(wxPoint(0,GetSize().y)));
    mParent->Show(TRUE);
  }
  else
    //mParent->Dismiss();
    mParent->Show(FALSE);
  if (MouseIsInside(mParent))
  {
    SetWholeDropToggleDown(TRUE);
    SetDropToggleDown(TRUE);
    SetFocus(TRUE);
  }
  //mChild->SetFocus();
  //ReleaseMouse();
} // ToggleChild

//////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(mmChildHandler, wxEvtHandler)
  EVT_KILL_FOCUS  (mmChildHandler::OnChildKillFocus)
  EVT_ENTER_WINDOW(mmChildHandler::OnMouseEnterLeaveChild)
  EVT_LEAVE_WINDOW(mmChildHandler::OnMouseEnterLeaveChild)
  EVT_MOUSE_EVENTS(mmChildHandler::OnMouse)
END_EVENT_TABLE()

mmChildHandler::mmChildHandler(mmDropWindow* button, wxWindow* child)
{
  mButton = button;
  mChild = child;
} // mmChildHandler

void mmChildHandler::OnChildKillFocus(wxFocusEvent& event)
{
  if (!mChild)
    return;
  if (!MouseIsInside(mChild))
  {
    if (mButton && !mButton->MouseIsOnButton())
    {
      mChild->GetParent()->Show(FALSE);
      mButton->SetWholeDropToggleDown(FALSE);
      mButton->SetDropToggleDown(FALSE);
      mButton->SetFocus(FALSE);
    }
  }
  event.Skip();
} // OnChildKillFocus

void mmChildHandler::OnMouseEnterLeaveChild(wxMouseEvent& event)
{
  if (event.GetEventType() == wxEVT_ENTER_WINDOW)
  {
    //mChild->CaptureMouse();
    if (mChild)
      mChild->SetFocus();
  }
  else
  if (event.GetEventType() == wxEVT_LEAVE_WINDOW)
  {
  }
  event.Skip();
} // OnMouseEnterLeaveChild

void mmChildHandler::OnMouse(wxMouseEvent& event)
{
  event.Skip();
}

