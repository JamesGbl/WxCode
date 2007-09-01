/////////////////////////////////////////////////////////////////////////////
// Name:        awx/toolbar.h
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: toolbar.h,v 1.4 2004/08/30 11:42:54 jb Exp $
// Copyright:   (c) 2003,2004 Joachim Buermann
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#ifndef __WXMTB_H
#define __WXMTB_H

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

/*!
  \class awxToolbar
  A wxToolbar like class which together with awxButton realizes
  transparent toolbar buttons with any (fixed coded) background.
  This implementation was initiated by the insufficient display of
  deactivated toolbar buttons in windows. (Possibly someone can extend
  awxToolbar with a GTK2 theme support - I am not a GTK expert
  myself).
  The text labelling can be optionally switched on and off.
 */
class awxToolbar : public wxWindow
{
protected:
    wxBitmap* m_bitmap;
    wxBoxSizer* m_sizer;
    wxPoint m_pos;
    wxSize m_size;
    bool m_painted;
    bool m_theme;
    wxCoord m_extraSpace;
    virtual void DrawOnBitmap();
    void InitBase();
public:
    awxToolbar(wxWindow* parent,
			wxWindowID id);
    awxToolbar() { InitBase(); };
    ~awxToolbar();
    void Add(wxWindow* widget);
    void EnableItem(int id,bool enable);
    void EnableTheme(bool enable) {
	   m_theme = enable;
    };
    wxBitmap GetBackgroundRegion(int id);
    bool HasTheme() {return m_theme;};
    void OnEraseBackground(wxEraseEvent& event);
    void OnPaint(wxPaintEvent &event);
    void OnSizeEvent(wxSizeEvent& event);
    void Redraw();
    void SetExtraSpace(wxCoord space) {
	   m_extraSpace = space;
    };
    DECLARE_EVENT_TABLE()
private:
    DECLARE_DYNAMIC_CLASS(awxToolbar)
};

#endif
