//
// Name     : mmDropMenu
// Purpose  : This is a button that can drop down a menu.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//

#include "mmDropMenu.h"

#ifdef __MMDEBUG__
extern wxTextCtrl* gDebug;
#endif

BEGIN_EVENT_TABLE(mmDropMenu,mmMultiButton)
  EVT_COMMAND(-1,mmEVT_WHOLEDROP_TOGGLE,mmDropMenu::OnMouse)
  EVT_COMMAND(-1,mmEVT_DROP_TOGGLE,     mmDropMenu::OnMouse)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////////

mmDropMenu::mmDropMenu(wxWindow* parent,
                           const wxWindowID id,
                           const wxString&  titleStr,
			   const wxBitmap&  titleBitmap,
                           const wxPoint&   pos,
                           const wxSize&    size,
                           const long int   style,
                                 wxMenu*    childMenu)
: mmMultiButton(parent,id,titleStr,titleBitmap,pos,size,style|mmMB_AUTODRAW)
{
  mStyle  = style;
  mToggleByChild  = FALSE;
  mToggleByButton = FALSE;
  SetChild(childMenu);
} // Constructor

mmDropMenu::~mmDropMenu()
{
} // Destructor

void mmDropMenu::SetChild(wxMenu* childMenu)
{
  mChild = childMenu;
} // SetChild

void mmDropMenu::OnMouse(wxMouseEvent& event)
{
  if (event.GetEventType() == mmEVT_DROP_TOGGLE ||
      event.GetEventType() == mmEVT_WHOLEDROP_TOGGLE)
    if (IsDropToggleDown() || IsWholeDropPressed())
      ToggleChild();
} // OnMouse

void mmDropMenu::ToggleChild()
{
  int border_w,border_h;
  if (mChild)
  {
    PopupMenu(mChild,0,GetClientSize().y);
    if (!MouseIsOnButton())
    {
      SetWholeDropToggleDown(FALSE);
      SetDropToggleDown(FALSE);
    }
  }
} // ToggleChild

bool mmDropMenu::MouseIsOnButton()
{
  int cx=0,cy=0;
  int cw=0,ch=0;
  ClientToScreen(&cx,&cy);
  GetSize(&cw,&ch);
  int mpx,mpy;
  ::wxGetMousePosition(&mpx,&mpy);
  if (mpx >= cx && mpx <= cx + cw &&
      mpy >= cy && mpy <= cy + ch)
    return TRUE;
  return FALSE;
}

