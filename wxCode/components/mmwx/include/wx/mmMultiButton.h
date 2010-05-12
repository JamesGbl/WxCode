//! \file mmMultiButton.h
//
// Name     : mmMultiButton
// Purpose  : A clickable button that can
//            - have a bitmap and/or a text label.
//            - function as a toggle ("on/off") button.
//            - turn active/inactive on entering/leaving it with the mouse.
//            - add a drop-down arrow to the bitmap in a separate section
//              (DROPDOWN) or as part of the bitmap (WHOLEDROPDOWN).
//
//            NOTE:
//            - mmMB_DROPDOWN and mmMB_WHOLEDROPDOWN cannot be used at
//              the same time.
//            - mmMB_STATIC and mmMB_AUTODRAW are mutually exclusive.
//
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#ifndef _MMMULTIBUTTON_INCLUDED
#define _MMMULTIBUTTON_INCLUDED

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/mmwxdef.h>

// mmMultiButton styles (re-use wxFrame/wxDialog style flags)

#define mmMB_FOCUS         					0x0400 		// Change bitmap on enter/leave.
#define mmMB_SELECT        					0x0800 		// Change bitmap on select.
#define mmMB_TOGGLE        					0x1000 		// Change bitmap on toggle.
#define mmMB_DROPDOWN      				0x2000 		// Draw a drop-down arrow in separate box.
#define mmMB_WHOLEDROPDOWN			0x4000			// Draw a drop-down arrow on the bitmap.
#define mmMB_STATIC        						0x8000 		// Do not react on mouse events.
#define mmMB_NO_AUTOSIZE   				0x0020 		// Button should use the given size, and not resize itself to fit bitmap/label.
#define mmMB_AUTODRAW      				0x0010 		// Automatically manage drawing of bitmap, // label and border.

// MultiButton events

const int mmEVT_MULTIBUTTON_FIRST = wxEVT_FIRST + 5400;

const wxEventType mmEVT_TOGGLE           					= mmEVT_MULTIBUTTON_FIRST + 1;
const wxEventType mmEVT_WHOLEDROP_TOGGLE 	= mmEVT_MULTIBUTTON_FIRST + 2;
const wxEventType mmEVT_DROP_TOGGLE      			= mmEVT_MULTIBUTTON_FIRST + 3;

WXDLLIMPEXP_DATA_MMWX(extern wxBitmap) gDownBM;		//!< The down arrow bitmap.
WXDLLIMPEXP_DATA_MMWX(extern wxBitmap) gDisableBM;	//!< The disabled bitmap.

//////////////////////////////////////////////////////////////////////////////

/*! \brief Multi-function button.
 */
class WXDLLIMPEXP_MMWX mmMultiButton: public wxWindow
{
    public:
        mmMultiButton(wxWindow *parent,
                      const wxWindowID id = -1,
                      const wxString &lab = wxEmptyString,
                      const wxBitmap &defaultBitmap= wxNullBitmap,
                      const wxPoint &pos = wxDefaultPosition,
                      const wxSize &size = wxDefaultSize,
                      const long int style = 0);
        // Constructor.
        // o parent: The button's parent.
        // o id: Button identifier.
        // o lab: Optional label.
        // o bitmapDefault: Optional default icon
        // o pos: Position.
        // o size: Size.
        // o style: The following styles are available:
        //   mmMB_AUTODRAW : Automatically manage drawing of bitmap,
        //   		       label and border.
        //   mmMB_FOCUS    : Change icon on enter/leave.
        //   mmMB_SELECT   : Change icon on select.
        //   mmMB_TOGGLE   : Change icon on toggle.
        //   mmMB_STATIC   : Do not react on mouse events.
        ~mmMultiButton();
        // Destructor.

        void     SetDefaultBitmap(wxBitmap &bm);
        void     SetSelectedBitmap(wxBitmap &bm);
        void     SetFocusBitmap(wxBitmap &bm);
        void     SetToggleBitmap(wxBitmap &bm);

        void     SetLabel(wxString str); // Change button label.
        void     SetStyle(const long style); // Change style.
        void     SetFocus(const bool hasFocus);
        void     SetSelected(const bool isSelected);
        void     SetToggleDown(const bool isToggleDown);
        void     SetWholeDropToggleDown(const bool isWholeDropToggleDown);
        void     SetDropToggleDown(const bool isDropToggleDown);

        /*! \brief Get the button's label.
         *
         * \return wxString	The label.
         *
         */
        wxString GetLabel() const
        {
            return mLabelStr;
        }
        // Return the label of the button (if any).

        /*! \brief Check whether the button is toggled or not.
         *
         * \return bool	True if toggled, otherwise false.
         *
         */
        bool     IsToggleDown() const
        {
            return mIsToggleDown;
        }

        /*! \brief Check whether the WHOLEDROP flag is set and the button is pressed.
         *
         * \return bool	Returns TRUE if the button is pressed and WHOLEDROP is set, false otherwise.
         *
         */
        bool IsWholeDropPressed() const
        {
            return mIsWholeDropToggleDown;
        }
        //
        /*! \brief Check whether the drop arrow is pressed.
         *
         * \return bool	Returns TRUE if the drop-down arrow is pressed, false otherwise.
         *
         */
        bool IsDropToggleDown() const
        {
            return mIsDropToggleDown;
        }
        // Return TRUE if the drop down arrow is pressed
        /*! \brief Check whether the button is selected.
         *
         * \return bool	Returns TRUE if the button is selected, false otherwise.
         *
         */
        bool IsSelected() const
        {
            return mIsSelected;
        }
        // Return TRUE if the button is selected
        /*! \brief Check whether the button has the focus.
         *
         * \return bool	Returns TRUE if the button is focused, false otherwise.
         *
         */
        bool HasFocus() const
        {
            return mHasFocus;
        }
        // Return TRUE if the button has focus
        bool MouseIsOnButton();
        // Returns TRUE if mouse pointer is over the button

        bool     Enable(bool enable);

    private:
        DECLARE_EVENT_TABLE()

        void     OnPaint(wxPaintEvent &event);
        void     OnMouse(wxMouseEvent &event);
        void 	OnCaptureLost(wxMouseCaptureLostEvent &event);

        void     RedrawBitmaps(wxDC &dc);
        void     RedrawLabel(wxDC &dc);
        void     RedrawBorders(wxDC &dc);

        void     FindAndSetSize();

        long     mStyle;											//!< The control's style.
        wxString mLabelStr;								//!< The button label.

        bool     mHasFocus;              					//!< TRUE if mouse is over button.
        bool     mIsActivated;           					//!< TRUE if button is activated.
        bool     mIsToggleDown;         				//!< TRUE if button is toggled down.
        bool     mIsWholeDropToggleDown;	//!< TRUE if mouse down over drop arrow
        bool     mIsDropToggleDown;      		//!< TRUE if mouse down over drop arrow
        bool     mIsSelected;            					//!< TRUE if button is selected.

        int      mBorderSize;								//!< The border size.
        int      mMarginSize;								//!< The margin size.

        wxBitmap mDefaultBitmap;					//!< The default bitmap.
        wxBitmap mSelectedBitmap;				//!< The bitmap to show when selected.
        wxBitmap mFocusBitmap;					//!< The bitmap to show when focused.
        wxBitmap mToggleBitmap;					//!< The bitmap to show when toggled.
}; // class mmMultiButton

#endif

