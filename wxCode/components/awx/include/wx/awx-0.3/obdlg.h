/////////////////////////////////////////////////////////////////////////////
// Name:        awx/obdlg.h
// Purpose:     Outlook like dialog
// Author:      Joachim Buermann
// Id:          $Id: obdlg.h,v 1.1.1.1 2004/08/13 11:44:50 jb Exp $
// Copyright:   (c) 2003,2004 Joachim Buermann
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#ifndef __PREFDLG_H
#define __PREFDLG_H

#include <wx/wx.h>
#include <wx/awx-0.3/outbar.h>

#define wxOB_DEFAULT_STYLE 0
#define wxOB_APPLY_BUTTON 1
#define wxOB_HELP_BUTTON 2

class wxOutbarPanel : public wxPanel
{
public:
    wxOutbarPanel(wxWindow* parent,
			   wxWindowID id = -1,
			   const wxPoint& pos = wxDefaultPosition,
			   const wxSize& size = wxDefaultSize) :
	   wxPanel(parent,id,pos,size)
	   {};
    virtual void OnApply(wxCommandEvent& WXUNUSED(event)) {};
    virtual void OnHelp(wxCommandEvent& WXUNUSED(event)) {};
};

class wxOutbarDialog : public wxDialog
{
protected:
    enum {maxItems = 16};
    wxBoxSizer* m_right;
    wxOutbarPanel* m_panel;
    wxOutbarPanel* m_pages[maxItems];
    wxStaticText* m_headline;
    wxString m_headlines[maxItems];
    wxOutBar* m_ob;
    int m_items;
    int m_selected;
    void OnApply(wxCommandEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);
    void SetPage(int pageno);
public:
    wxOutbarDialog(wxWindow* parent,
			    wxWindowID id,
			    const wxChar* title,
			    const wxSize& pageSize = wxSize(400,200),
			    const int outbarStyle = wxOB_DEFAULT_STYLE);
    ~wxOutbarDialog();
    bool AddPage(char** xpmimage,
			  const wxChar* label,
			  wxOutbarPanel* newPage,
			  const wxChar* headline = NULL);
    wxOutbarPanel* GetEmptyPage() {return m_panel;};
    wxOutbarPanel* GetPage(int no) {
	   if((unsigned)no < (unsigned)m_items) return m_pages[no];
	   else return NULL;
    };
    wxOutbarPanel* GetSelectedPage() {return m_pages[m_selected];};
    void SetSelection(const wxChar* byName);
    void OnChoice(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()
};

#endif
