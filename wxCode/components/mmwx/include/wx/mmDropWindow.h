//
// Name     : mmDropWindow
// Purpose  : This is a button that can drop down any window.
//            DOES NOT WORK FOR MODAL DIALOGS!
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#ifndef _MMDROPWINDOW_H_INCLUDED_
#define _MMDROPWINDOW_H_INCLUDED_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "mmMultiButton.h"

//////////////////////////////////////////////////////////////////////////////

bool MouseIsInside(wxWindow *window);

//////////////////////////////////////////////////////////////////////////////

class mmDropWindow : public mmMultiButton
{
        //friend class mmChildHandler;
    public:
        mmDropWindow(wxWindow *parent,
                     const wxWindowID id,
                     const wxString  &titleStr,
                     const wxBitmap  &titleBitmap,
                     const wxPoint   &pos         = wxDefaultPosition,
                     const wxSize    &size        = wxDefaultSize,
                     const long int   style       = 0,
                     wxWindow  *childWindow = NULL,
                     const wxSize    &childSize   = wxDefaultSize);
        // Constructor.
        // o parent     : Parent window
        // o id         : Window id
        // o titleStr   : Optional label for drop-down button
        // o titleBitmap: Optional bitmap for drop-down button
        // o pos        : Window position
        // o size       : Window size
        // o style      : Window style
        // o childWindow: An optional window to be dropped down.
        //                (Can also be set later by SetChild()).
        // o childSize  : Size of the child. If a child is set later with
        //                SetChild(), childSize is ignored.

        ~mmDropWindow(); // Destructor

        void      SetChild(wxWindow *childWin,
                           const wxSize &childSize = wxDefaultSize);
        // Set the window to be dropped down. The size and style of the
        // child is used, and that given in the constructor is discarded.
        wxWindow *GetChild() {
            return mChild;
        }
        // Returns a pointer to the window to be dropped down.
        void      ToggleChild();
        // Toggle displays the child window.

    private:
        DECLARE_EVENT_TABLE()

        void    OnDrop(wxCommandEvent &event);
        wxPoint CalculateWindowPos();

        wxFrame *mParent;
        long int  mStyle;
        wxWindow *mChild;
        wxSize    mChildSize;
        long int  mChildStyle;
        bool      mChildHandlerIsPushed;
}; // mmDropWindow

//////////////////////////////////////////////////////////////////////////////

// Event handler for kill focus and mouse events for the child window
class mmChildHandler : public wxEvtHandler
{
    public:
        mmChildHandler(mmDropWindow *btn, wxWindow *child);

    private:
        DECLARE_EVENT_TABLE()

        void OnChildKillFocus(wxFocusEvent&);
        void OnMouseEnterLeaveChild(wxMouseEvent&);
        void OnMouse(wxMouseEvent&);
        void OnCaptureLost(wxMouseCaptureLostEvent &event);

        mmDropWindow *mButton;
        wxWindow     *mChild;
}; // mmChild

#endif

