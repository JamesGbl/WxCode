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

#include <wx/mimetype.h>

//////////////////////////////////////////////////////////////////////////////

#define mmHT_NO_AUTOSIZE       0x200 // The window should use the given size,
// and not resize itself to fit label

//////////////////////////////////////////////////////////////////////////////

class mmHyperText: public wxWindow
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
        ~mmHyperText(void);
        // Destructor.

        void     SetText(wxString &txt)           {
            mText = txt;
            Refresh();
        }
        void     SetURL(wxString &url)            {
            mURL = url;
        }
        void     SetVisited(const bool isVisited) {
            mIsVisited = isVisited;
            Refresh();
        }
        wxString GetText(void) const              {
            return mText;
        }
        wxString GetURL(void) const               {
            return mURL;
        }
        bool     GetVisited(void) const           {
            return mIsVisited;
        }

    private:
        DECLARE_EVENT_TABLE()

        void      OnMouse(wxMouseEvent &event);
        void      OnPaint(wxPaintEvent &event);
        void      HyperExec(const bool isURL);

        wxString  mText;
        long      mStyle;
        wxString  mURL;
        int       mMargin;
        bool      mIsVisited;
        bool      mLeftIsDown;
        wxColour *mVisitedColour;
        wxColour *mUnvisitedColour;
        wxCursor *mEnterCursor;
        wxCursor *mLeaveCursor;
}; // class mmHyperText

#endif

