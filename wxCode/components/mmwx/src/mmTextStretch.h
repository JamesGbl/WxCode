//
// Name     : mmTextStretch
// Purpose  : A wxTextCtrl that can be stretched and/or moved by
//            using the left mouse button.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//

#ifndef _MMTEXTSTRETCH_INCLUDED
#define _MMTEXTSTRETCH_INCLUDED

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//////////////////////////////////////////////////////////////////////////////

#define mmTS_SIZE_TOP        0x001
#define mmTS_SIZE_BOTTOM     0x002
#define mmTS_SIZE_LEFT       0x004
#define mmTS_SIZE_RIGHT      0x008
#define mmTS_SIZE_ALL        (mmTS_SIZE_TOP | mmTS_SIZE_BOTTOM | mmTS_SIZE_LEFT | mmTS_SIZE_RIGHT)
#define mmTS_MOVE_VERTICAL   0x010
#define mmTS_MOVE_HORIZONTAL 0x020
#define mmTS_MOVE_BOTH       (mmTS_MOVE_VERTICAL | mmTS_MOVE_HORIZONTAL)
#define mmTS_SIZE_NOKEY      0x040
#define mmTS_MOVE_NOKEY      0x080
#define mmTS_SNAP_TO_GRID    0x100

//////////////////////////////////////////////////////////////////////////////

class mmTextStretch: public wxWindow
{
  public:
    mmTextStretch(wxWindow* parent,
		  const wxWindowID id    = -1,
		  const wxString&  value = "",
                  const wxPoint&   pos   = wxDefaultPosition,
                  const wxSize&    size  = wxDefaultSize,
		  const long       style = 0);
		  // Constructor.
   ~mmTextStretch(void);
		  // Destructor.

    void SetMargin(int margin)     { mMargin = margin; }
    void SetGridSize(int gridSize) { mGridSize = gridSize; }
    void SetSnapToGrid(bool snap)  { if (snap) 
                                       mStyle |= mmTS_SNAP_TO_GRID;
                                     else
                                       mStyle &= !mmTS_SNAP_TO_GRID;
                                   }

  private:
    DECLARE_EVENT_TABLE()

    void OnMouse(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);

    bool MouseIsOnBorder();
    bool MouseIsOnTopLeft();
    bool MouseIsOnBottomRight();
    bool MouseIsOnTop();
    bool MouseIsOnBottom();
    bool MouseIsOnLeft();
    bool MouseIsOnRight();
      
    wxTextCtrl* mTextCtrl;
    long mStyle;
    int  mMargin;
    int  mXdelta,mYdelta;
    int  mGridSize;
    bool mSnapToGrid;
    bool mTopLeftWasPressed;
    bool mBottomRightWasPressed;
    bool mTopWasPressed;
    bool mBottomWasPressed;
    bool mLeftWasPressed;
    bool mRightWasPressed;
    bool mLeftIsDown;
    bool mCtrlIsDown;
    wxCursor* mNormalCursor;
    wxCursor* mSizeNWSECursor; // NorthWest-SouthEast
    wxCursor* mSizeNSCursor;   // NorthSouth
    wxCursor* mSizeWECursor;   // WestEast
    wxCursor* mMoveCursor;
}; // class mmTextStretch

#endif

