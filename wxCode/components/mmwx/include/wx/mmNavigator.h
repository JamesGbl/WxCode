//! \file mmNavigator.h
//
// Name:      mmNavigator.h
// Purpose:   Include file for mmNavigator class
// Author:    Arne Morken
// Copyright: (C) 2000-2002, MindMatters, www.mindmatters.no
// Licence:   wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#ifndef _WXNAVIGATOR_H_INCLUDED_
#define _WXNAVIGATOR_H_INCLUDED_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/mmwxdef.h>
#include "mmMultiButton.h"

// Navigator events
const wxEventType mmEVT_NAVIGATOR_PREV2	= wxEVT_FIRST + 6031;
const wxEventType mmEVT_NAVIGATOR_PREV  	= wxEVT_FIRST + 6032;
const wxEventType mmEVT_NAVIGATOR_NEXT  	= wxEVT_FIRST + 6033;
const wxEventType mmEVT_NAVIGATOR_NEXT2	= wxEVT_FIRST + 6034;

/*! \brief Nav. buttons IDs.
 */
enum {
    mmID_NAV_PREV2 = 6050,
    mmID_NAV_PREV,
    mmID_NAV_NEXT,
    mmID_NAV_NEXT2
};

WXDLLIMPEXP_DATA_MMWX(extern wxBitmap) *gPrevDblBitmap;		//!< The double back arrow bitmap.
WXDLLIMPEXP_DATA_MMWX(extern wxBitmap) *gPrevBitmap;			//!< The back arrow bitmap.
WXDLLIMPEXP_DATA_MMWX(extern wxBitmap) *gNextBitmap;			//!< The forward arrow bitmap.
WXDLLIMPEXP_DATA_MMWX(extern wxBitmap) *gNextDblBitmap;	//!< The double forward arrow bitmap.
/////////////////////////////////////////////////////////////////////////////
// mmNavigator
/////////////////////////////////////////////////////////////////////////////

/*! \brief Navigation control.
 */
class WXDLLIMPEXP_MMWX mmNavigator : public wxWindow
//  A simple 'navigation' toolbar.
{
    public:
        mmNavigator(wxWindow *parent,
                    const wxWindowID id      = -1,
                    wxWindow  		*userWin = NULL,
                    const bool       prev    = true,
                    const bool       next    = true,
                    const bool       prevDbl = true,
                    const bool       nextDbl = true,
                    const wxPoint   &pos     = wxDefaultPosition,
                    const wxSize    &size    = wxDefaultSize,
                    const long       style   = 0);
        // Constructor.
        // The meaning of the parameters are:
        //  o parent: The button's parent.
        //  o id: Button identifier.
        //  o win: Optional window to be placed between buttons
        //  o prev: Whether a 'previous' button should be available.
        //  o next: Whether a 'next' button should be available.
        //  o prevDouble:
        //  o nextDouble:
        //  o pos: Position.
        //  o size: Size.
        //  o style: No special styles are available.
        //  o name: Name.
        ~mmNavigator();
        // Destructor

        virtual bool  SetBackgroundColour(const wxColour &colour);
        // Set the background colour of the navigator (including
        // the buttons)

        void SetWindow(wxWindow *win);
        // Set the navigator's optional window.
        /*! \brief Get a pointer to the navigator's optional window, if any.
         *
         * \return wxWindow*	The window pointer.
         *
         */
        wxWindow *GetWindow() const
        {
            return mWindow;
        }
        // Return the navigator's optional window, if any.
        /*! \brief Get a pointer to the 'previous-double' button.
         *
         * \return mmMultiButton*	The button pointer.
         *
         */
        mmMultiButton *GetPrevDoubleButton()
        {
            return mPrevDblButton;
        }
        // Return a pointer to the 'previous-double' button.
        /*! \brief Get a pointer to the 'previous' button.
         *
         * \return mmMultiButton*	The button pointer.
         *
         */
        mmMultiButton *GetPrevButton()
        {
            return mPrevButton;
        }
        // Return a pointer to the 'previous' button.
        /*! \brief Get a pointer to the 'next' button.
         *
         * \return mmMultiButton*	The button pointer.
         *
         */
        mmMultiButton *GetNextButton()
        {
            return mNextButton;
        }
        // Return a pointer to the 'next' button.
        /*! \brief Get a pointer to the 'next-double' button.
         *
         * \return mmMultiButton*	The button pointer.
         *
         */
        mmMultiButton *GetNextDoubleButton()
        {
            return mNextDblButton;
        }
        // Return a pointer to the 'next-double' button.

    private:
        DECLARE_EVENT_TABLE()

        void OnButton(wxCommandEvent &event);
        void GenerateEvent(wxEventType type);

        wxWindow       	*mWindow;				//!< The window type to navigate.
        mmMultiButton *mPrevDblButton;	//!< The back arrow button.
        mmMultiButton *mPrevButton;			//!< The double back arrow button.
        mmMultiButton *mNextButton;			//!< The forward arrow button.
        mmMultiButton *mNextDblButton;	//!< The double forward arrow button.
}; // class mmNavigator

/////////////////////////////////////////////////////////////////////////////
// mmNavigatorEvent
/////////////////////////////////////////////////////////////////////////////

/*! \brief Navigation control event.
 */
class mmNavigatorEvent : public wxCommandEvent
{
    public:
        mmNavigatorEvent(mmNavigator *nav, wxEventType type);
}; // mmNavigatorEvent

#define EVT_NAVIGATOR_PREV2(id,fn) \
    DECLARE_EVENT_TABLE_ENTRY( mmEVT_NAVIGATOR_PREV2, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &fn, (wxObject*)NULL ),
#define EVT_NAVIGATOR_PREV(id,fn) \
    DECLARE_EVENT_TABLE_ENTRY( mmEVT_NAVIGATOR_PREV,    id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &fn, (wxObject*)NULL ),
#define EVT_NAVIGATOR_NEXT(id,fn) \
    DECLARE_EVENT_TABLE_ENTRY( mmEVT_NAVIGATOR_NEXT,    id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &fn, (wxObject*)NULL ),
#define EVT_NAVIGATOR_NEXT2(id,fn) \
    DECLARE_EVENT_TABLE_ENTRY( mmEVT_NAVIGATOR_NEXT2, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &fn, (wxObject*)NULL ),

#endif

