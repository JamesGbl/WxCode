//! \file mmDropMenu.cpp
//
// Name     : mmDropMenu
// Purpose  : This is a button that can drop down a menu.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#include <wx/mmwxdef.h>
#include "wx/mmDropMenu.h"

#ifdef __MMDEBUG__
	WXDLLIMPEXP_DATA_MMWX(extern wxTextCtrl*) gDebug;    //!< Global debug output window.
#endif

BEGIN_EVENT_TABLE(mmDropMenu, mmMultiButton)
    EVT_COMMAND(-1, mmEVT_WHOLEDROP_TOGGLE, mmDropMenu::OnMouse)
    EVT_COMMAND(-1, mmEVT_DROP_TOGGLE, mmDropMenu::OnMouse)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////////

/*! \brief Constructor.
 *
 * \param parent 		wxWindow*				The parent window.
 * \param id 				const wxWindowID	The ID of this window.
 * \param titleStr 		const wxString&		The text to display on the button.
 * \param titleBitmap 	const wxBitmap&		The bitmap to display on the button.
 * \param pos 			const wxPoint&			The button's position.
 * \param size 			const wxSize&			The button's size.
 * \param style 			const longint				The button's style.
 * \param childMenu 	wxMenu*					The menu to display.
 *
 */
mmDropMenu::mmDropMenu(wxWindow *parent,
                       const wxWindowID id,
                       const wxString  &titleStr,
                       const wxBitmap  &titleBitmap,
                       const wxPoint   &pos,
                       const wxSize    &size,
                       const long int   style,
                       wxMenu    *childMenu)
    : mmMultiButton(parent, id, titleStr, titleBitmap, pos, size, style | mmMB_AUTODRAW)
{
    SetChild(childMenu);
} // Constructor

/*! \brief Destructor.
 */
mmDropMenu::~mmDropMenu()
{
} // Destructor

/*! \brief Set the child menu that will be displayed.
 *
 * \param childMenu wxMenu*		The child menu.
 * \return void
 *
 */
void mmDropMenu::SetChild(wxMenu *childMenu)
{
    mChild = childMenu;
} // SetChild

/*! \brief A mouse event occurred.
 *
 * \param event wxCommandEvent&	A reference to a wxCommandEvent object.
 * \return void
 *
 */
void mmDropMenu::OnMouse(wxCommandEvent &event)
{
    if(event.GetEventType() == mmEVT_DROP_TOGGLE || event.GetEventType() == mmEVT_WHOLEDROP_TOGGLE)
        if(IsDropToggleDown() || IsWholeDropPressed())
            ToggleChild();
} // OnMouse

/*! \brief Toggle the child menu.
 *
 * \return void
 *
 */
void mmDropMenu::ToggleChild()
{
    if(mChild)
    {
        PopupMenu(mChild, 0, GetClientSize().y);
        if(!MouseIsOnButton())
        {
            SetWholeDropToggleDown(FALSE);
            SetDropToggleDown(FALSE);
        }
    }
} // ToggleChild


