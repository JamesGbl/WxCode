//
// Name     : mmDropMenu
// Purpose  : This is a button that can drop down a menu.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//

#ifndef _MMDROPMENU_H_INCLUDED_
#define _MMDROPMENU_H_INCLUDED_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "mmMultiButton.h"

//////////////////////////////////////////////////////////////////////////////

class mmDropMenu : public mmMultiButton
{
  friend class mmChildHandler;
  public:
    mmDropMenu(wxWindow* parent,
               const wxWindowID id,
               const wxString&  titleStr,
	       const wxBitmap&  titleBitmap,
               const wxPoint&   pos         = wxDefaultPosition,
               const wxSize&    size        = wxDefaultSize,
               const long int   style       = 0,
	             wxMenu*    childMenu   = NULL);
    // Constructor.
    // o parent     : Parent window
    // o id         : Window id
    // o titleStr   : Optional label for drop-down button
    // o titleBitmap: Optional bitmap for drop-down button
    // o pos        : Window position
    // o size       : Window size
    // o style      : Window style
    // o childMenu  : The menu to be dropped down.
    //                (Can also be set later by SetChild()).

   ~mmDropMenu(); // Destructor

    void      SetChild(wxMenu* childWin);
              // Set the menu to be dropped down.
    wxMenu*   GetChild() { return mChild; }
              // Returns a pointer to the window to be dropped down.
    void      ToggleChild();
              // Toggle displays the child window.
    bool      MouseIsOnButton();
              // Returns TRUE if mouse pointer is over the button

  protected:
    void      RequestToggle();
              // Child requests toggle

  private:
    DECLARE_EVENT_TABLE()

    void    OnMouse(wxMouseEvent& event);
    wxPoint CalculateWindowPos(int* dropWidth, int* dropHeight);

    wxFrame*  mParent;
    long int  mStyle;
    wxMenu*   mChild;
    wxSize    mChildSize;
    long int  mChildStyle;
    bool      mChildHandlerIsPushed;
    bool      mToggleByChild;
    bool      mToggleByButton;
}; // mmDropMenu

//////////////////////////////////////////////////////////////////////////////

/*
// Event handler for kill focus and mouse events for the child window
class mmChildHandler : public wxEvtHandler
{
  public:
    mmChildHandler(mmDropWindow* btn, wxWindow* child);

  private:
    DECLARE_EVENT_TABLE()

    void OnChildKillFocus(wxFocusEvent&);
    void OnMouseEnterChild(wxMouseEvent&);
    bool MouseIsInside();

    mmDropWindow* mButton;
    wxWindow*     mChild;
}; // mmChild
*/

#endif

