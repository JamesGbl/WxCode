/////////////////////////////////////////////////////////////////////////////
// Name:        awx/outbar.h
// Purpose:     Outlook like bar widget
// Author:      Joachim Buermann
// Id:          $Id: outbar.h,v 1.1.1.1 2004/08/13 11:44:50 jb Exp $
// Copyright:   (c) 2003,2004 Joachim Buermann
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#ifndef __OUTBAR_H
#define __OUTBAR_H

#include <wx/bitmap.h>
#include <wx/scrolwin.h>
#include <wx/wx.h>

class wxOutBarItem : public wxWindow
{
protected:
    int m_check;
    
    wxString m_label;
    wxBitmap* m_bitmap;
    // one bitmap for each state, disabled:0, enabled:1 
    wxBitmap* m_icon[2];

    wxFont* m_font;
    void DrawItem(wxDC& dc);
    void DrawOnBitmap();
    void OnMouseLeftDown(wxMouseEvent& event);
    void Redraw();
public:
    wxOutBarItem(wxWindow* parent,
			  wxWindowID id,
			  const wxPoint& pos,
			  const wxSize& size,
			  char** xpmimage,
			  const wxString& label);
    ~wxOutBarItem();
    wxString& GetLabel() { return m_label; }
    void OnErase(wxEraseEvent& event);
    void OnPaint(wxPaintEvent &event);
    void Check(bool check) {m_check = check;};
    DECLARE_EVENT_TABLE()
};

class wxOutBar : public wxScrolledWindow
{
protected:
    wxOutBarItem* m_selected;
    wxBoxSizer* m_sizer;
    wxSize m_itemSize;
    int m_items;
    wxSize m_margins;
public:
    wxOutBar(wxWindow* parent,
		   wxWindowID id,
		   const wxPoint& pos,
		   const wxSize& size);
    ~wxOutBar();
    void Add(wxOutBarItem* item);
    wxSize& GetMargins() { return m_margins; }
    wxOutBarItem* GetSelectedItem() {return m_selected;};
    void OnChoice(wxCommandEvent& event);
    void SetMargins(const wxSize& margins) {
	   m_margins = margins;
    }
    bool SetSelection(const wxChar* byName);
    DECLARE_EVENT_TABLE()
};

#endif
