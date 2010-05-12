//! \file mmDropMenu.h
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

#ifndef _MMDROPMENU_H_INCLUDED_
#define _MMDROPMENU_H_INCLUDED_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/mmwxdef.h>
#include "mmMultiButton.h"

//////////////////////////////////////////////////////////////////////////////

/*! \brief An mmMultiButton with drop-down menu.
 */
class WXDLLIMPEXP_MMWX mmDropMenu : public mmMultiButton
{
        friend class mmChildHandler;
    public:
        mmDropMenu(wxWindow *parent,
                   const wxWindowID id,
                   const wxString  &titleStr,
                   const wxBitmap  &titleBitmap,
                   const wxPoint   &pos         = wxDefaultPosition,
                   const wxSize    &size        = wxDefaultSize,
                   const long int   style       = 0,
                   wxMenu    *childMenu   = NULL);
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

        void SetChild(wxMenu *childWin);
        // Set the menu to be dropped down.
        /*! \brief Get a pointer to the child menu.
         *
         * \return wxMenu*	The menu pointer.
         *
         */
        wxMenu *GetChild() {
            return mChild;
        }
        // Returns a pointer to the window to be dropped down.
        void ToggleChild();
        // Toggle displays the child window.

    private:
        DECLARE_EVENT_TABLE()

        void OnMouse(wxCommandEvent &event);

        wxMenu *mChild;	//!< The child menu.
}; // mmDropMenu


#endif

