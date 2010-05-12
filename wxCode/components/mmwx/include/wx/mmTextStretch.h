//! \file mmTextStretch.h
//
// Name     : mmTextStretch
// Purpose  : A wxTextCtrl that can be stretched and/or moved by using the left mouse button.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#ifndef _MMTEXTSTRETCH_INCLUDED
#define _MMTEXTSTRETCH_INCLUDED

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/mmwxdef.h>

//////////////////////////////////////////////////////////////////////////////

#define mmTS_SIZE_TOP        			0x001
#define mmTS_SIZE_BOTTOM     		0x002
#define mmTS_SIZE_LEFT       			0x004
#define mmTS_SIZE_RIGHT      			0x008
#define mmTS_SIZE_ALL        			(mmTS_SIZE_TOP | mmTS_SIZE_BOTTOM | mmTS_SIZE_LEFT | mmTS_SIZE_RIGHT)
#define mmTS_MOVE_VERTICAL   	0x010
#define mmTS_MOVE_HORIZONTAL	0x020
#define mmTS_MOVE_BOTH       		(mmTS_MOVE_VERTICAL | mmTS_MOVE_HORIZONTAL)
#define mmTS_SIZE_NOKEY      		0x040
#define mmTS_MOVE_NOKEY      	0x080
#define mmTS_SNAP_TO_GRID    		0x100

//////////////////////////////////////////////////////////////////////////////

/*! \brief A stretchable text control.
 */
class WXDLLIMPEXP_MMWX mmTextStretch: public wxWindow
{
    public:
        mmTextStretch(wxWindow *parent,
                      const wxWindowID id    = -1,
                      const wxString  &value = wxEmptyString,
                      const wxPoint   &pos   = wxDefaultPosition,
                      const wxSize    &size  = wxDefaultSize,
                      const long       style = 0);
        // Constructor.
        ~mmTextStretch();
        // Destructor.

        /*! \brief Set the margin width.
         *
         * \param margin int	The width.
         * \return void
         *
         */
        void SetMargin(int margin)
        {
            mMargin = margin;
        }
        /*! \brief Set the grid size.
         *
         * \param gridSize int	The grid size.
         * \return void
         *
         */
        void SetGridSize(int gridSize)
        {
            mGridSize = gridSize;
        }
        /*! \brief Set the snap-to-grid mode.
         *
         * \param snap bool	On if true, otherwise off.
         * \return void
         *
         */
        void SetSnapToGrid(bool snap)
        {
            if(snap)
                mStyle |= mmTS_SNAP_TO_GRID;
            else
                mStyle &= !mmTS_SNAP_TO_GRID;
        }

    private:
        DECLARE_EVENT_TABLE()

        void OnMouse(wxMouseEvent &event);
        void OnCaptureLost(wxMouseCaptureLostEvent &event);
        void OnPaint(wxPaintEvent &event);
        void OnKeyDown(wxKeyEvent &event);

        bool MouseIsOnBorder();
        bool MouseIsOnTopLeft();
        bool MouseIsOnBottomRight();
        bool MouseIsOnTop();
        bool MouseIsOnBottom();
        bool MouseIsOnLeft();
        bool MouseIsOnRight();

        wxTextCtrl *mTextCtrl;									//!< The text control.
        long 			mStyle;											//!< The control's style.
        int  				mMargin;										//!< The margin width.
        int  				mGridSize;									//!< The grid size.
        bool 			mTopLeftWasPressed;				//!< Whether the top left border was clicked.
        bool 			mBottomRightWasPressed;	//!< Whether the bottom right border was clicked.
        bool 			mTopWasPressed;					//!< Whether the top border was clicked.
        bool 			mBottomWasPressed;				//!< Whether the bottom border was clicked.
        bool 			mLeftWasPressed;					//!< Whether the left border was clicked.
        bool 			mRightWasPressed;					//!< Whether the right border was clicked.
        bool 			mLeftIsDown;								//!< Whether the left mouse button is down.
        bool 			mCtrlIsDown;								//!< Whether the CTRL key is down.
        wxCursor	*mNormalCursor;						//!< The normal mouse cursor.
        wxCursor 	*mSizeNWSECursor; 				//!< NorthWest/SouthEast resizing cursor.
        wxCursor 	*mSizeNSCursor;   					//!< North/South resizing cursor.
        wxCursor 	*mSizeWECursor;   					//!< West/East resizing cursor.
        wxCursor 	*mMoveCursor;							//!< Movement cursor.
}; // class mmTextStretch

#endif

