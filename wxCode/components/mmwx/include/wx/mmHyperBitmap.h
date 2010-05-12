//! \file mmHyperBitmap.h
//
// Name     : mmHyperBitmap
// Purpose  : A clickable bitmap that performs an action when clicked.
//            The default action is to bring up the default browser.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#ifndef _MMHYPERBITMAP_INCLUDED
#define _MMHYPERBITMAP_INCLUDED

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

#define mmHT_NO_AUTOSIZE       0x200 // The window should use the given size, and not resize itself to fit label

//////////////////////////////////////////////////////////////////////////////

/*! \brief Hyperlinked bitmap.
 */
class WXDLLIMPEXP_MMWX mmHyperBitmap: public wxWindow
{
    public:
        mmHyperBitmap(wxWindow *parent,
                      const wxWindowID id     = -1,
                      const wxBitmap  &bitmap = wxNullBitmap,
                      const wxString  &url    = wxEmptyString,
                      const wxPoint   &pos    = wxDefaultPosition,
                      const wxSize    &size   = wxDefaultSize,
                      const long       style  = 0);
        // Constructor.
        ~mmHyperBitmap();
        // Destructor.

        /*! \brief Set the bitmap to use.
         *
         * \param bm wxBitmap&	The bitmap.
         * \return void
         *
         */
        void SetBitmap(wxBitmap &bm)
        {
            mBitmap = bm;
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
        /*! \brief Get the bitmap.
         *
         * \return wxBitmap&	The bitmap.
         *
         */
        wxBitmap &GetBitmap()
        {
            return mBitmap;
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

        void	OnMouse(wxMouseEvent &event);
        void	OnPaint(wxPaintEvent &event);
        void	HyperExec(const bool isURL);

        wxBitmap  mBitmap;			//!< The hyperlinked bitmap.
        long     	mStyle;				//!< The control's style.
        wxString  	mURL;				//!< The URL text.
        int       		mMargin;			//!< The selection margin width.
        bool     	mIsVisited;		//!< Whether the URL has been visited.
        bool     	mLeftIsDown;	//!< Whether the left mouse button is down.
        wxCursor 	*mEnterCursor;	//!< The cursor to use when leaving the bitmap.
        wxCursor 	*mLeaveCursor;	//!< The cursor to use whilst hovering over the bitmap.
}; // class mmHyperBitmap

#endif

