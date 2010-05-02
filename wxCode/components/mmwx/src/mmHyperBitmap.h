//
// Name     : mmHyperBitmap
// Purpose  : A clickable bitmap that performs an action when clicked.
//            The default action is to bring up the default browser.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//

#ifndef _MMHYPERBITMAP_INCLUDED
#define _MMHYPERBITMAP_INCLUDED

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

class mmHyperBitmap: public wxWindow
{
  public:
    mmHyperBitmap(wxWindow* parent,
		const wxWindowID id     = -1,
		const wxBitmap&  bitmap = wxNullBitmap,
		const wxString&  url    = "",
                const wxPoint&   pos    = wxDefaultPosition,
                const wxSize&    size   = wxDefaultSize,
		const long       style  = 0);
		// Constructor.
   ~mmHyperBitmap(void);
		  // Destructor.

    void     SetBitmap(wxBitmap& bm)          { mBitmap = bm; Refresh(); }
    void     SetURL(wxString& url)            { mURL = url; }
    void     SetVisited(const bool isVisited) { mIsVisited = isVisited;
                                                Refresh(); }
    wxBitmap& GetBitmap(void)                  { return mBitmap; }
    wxString  GetURL(void) const               { return mURL; }
    bool      GetVisited(void) const           { return mIsVisited; }

  private:
    DECLARE_EVENT_TABLE()

    void      OnMouse(wxMouseEvent& event);
    void      OnPaint(wxPaintEvent& event);
    void      HyperExec(const bool isURL);

    wxBitmap  mBitmap;
    long      mStyle;
    wxString  mURL;
    int       mMargin;
    bool      mIsVisited;
    bool      mLeftIsDown;
    wxColour* mVisitedColour;
    wxColour* mUnvisitedColour;
    wxCursor* mEnterCursor;
    wxCursor* mLeaveCursor;
}; // class mmHyperBitmap

#endif

