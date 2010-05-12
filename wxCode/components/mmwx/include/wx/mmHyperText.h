//! \file mmHyperText.h
//
// Name     : mmHyperText
// Purpose  : A clickable text that performs an action when clicked.
//            The default action is to bring up the default browser.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#ifndef _MMHYPERTEXT_INCLUDED
#define _MMHYPERTEXT_INCLUDED

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/mmwxdef.h>
#include <wx/mimetype.h>

//////////////////////////////////////////////////////////////////////////////

#define mmHT_NO_AUTOSIZE       0x200 // The window should use the given size,
// and not resize itself to fit label

//////////////////////////////////////////////////////////////////////////////

/*! \brief Hyperlinked text.
 */
class WXDLLIMPEXP_MMWX mmHyperText: public wxWindow
{
    public:
        mmHyperText(wxWindow *parent,
                    const wxWindowID id    = -1,
                    const wxString  &str   = wxEmptyString,
                    const wxString  &url   = wxEmptyString,
                    const wxPoint   &pos   = wxDefaultPosition,
                    const wxSize    &size  = wxDefaultSize,
                    const long       style = 0);
        // Constructor.
        ~mmHyperText();
        // Destructor.

        /*! \brief Set the text to display.
         *
         * \param txt wxString&	The text.
         * \return void
         *
         */
        void SetText(wxString &txt)
        {
            mText = txt;
            Refresh();
        }
        /*! \brief Set the URL to link to.
         *
         * \param url wxString&	The URL.
         * \return void
         *
         */
        void SetURL(wxString &url)
        {
            mURL = url;
        }
        /*! \brief Set whether the URL has been visited.
         *
         * \param isVisited const bool
         * \return void
         *
         */
         void SetVisited(const bool isVisited)
        {
            mIsVisited = isVisited;
            Refresh();
        }
        /*! \brief Get the displayed text.
         *
         * \return wxString	The text.
         *
         */
        wxString GetText() const
        {
            return mText;
        }
        /*! \brief Get the URL.
         *
         * \return wxString	The URL
         *
         */
        wxString GetURL() const
        {
            return mURL;
        }
        /*! \brief Check whether the URL has been visited.
         *
         * \return bool	True if visited, false otherwise.
         *
         */
        bool GetVisited() const
        {
            return mIsVisited;
        }

    private:
        DECLARE_EVENT_TABLE()

        void OnMouse(wxMouseEvent &event);
        void OnPaint(wxPaintEvent &event);
        void HyperExec(const bool isURL);

        wxString  	mText;							//!< The displayed text.
        long     		mStyle;							//!< The control's style.
        wxString  	mURL;							//!< The URL to link to.
        int       		mMargin;						//!< The selection margin width.
        bool     		mIsVisited;					//!< Whether the URL has been visited.
        bool     		mLeftIsDown;				//!< Whether the left mouse button is down.
        wxColour 	*mVisitedColour;			//!< The link's visited colour.
        wxColour 	*mUnvisitedColour;	//!< The link's default colour.
        wxCursor 	*mEnterCursor;			//!< The cursor to use when leaving the bitmap.
        wxCursor 	*mLeaveCursor;			//!< The cursor to use whilst hovering over the bitmap.
}; // class mmHyperText

#endif

