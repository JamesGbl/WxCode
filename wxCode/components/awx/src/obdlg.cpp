/////////////////////////////////////////////////////////////////////////////
// Name:        awx/obdlg.cpp
// Purpose:     Outlook like dialog
// Author:      Joachim Buermann
// Id:          $Id: obdlg.cpp,v 1.1.1.1 2004/08/13 11:45:36 jb Exp $
// Copyright:   (c) 2003,2004 Joachim Buermann
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "wx/awx-0.3/obdlg.h"
#include <wx/statline.h>

const int ID_FIRST = wxID_HIGHEST+1;

BEGIN_EVENT_TABLE(wxOutbarDialog, wxDialog)
    EVT_BUTTON(wxID_APPLY,wxOutbarDialog::OnApply)
    EVT_BUTTON(wxID_HELP,wxOutbarDialog::OnHelp)
    EVT_CHOICE(-1,wxOutbarDialog::OnChoice)
    EVT_ERASE_BACKGROUND(wxOutbarDialog::OnEraseBackground)
    EVT_SIZE(wxOutbarDialog::OnSize)
END_EVENT_TABLE()

wxOutbarDialog::wxOutbarDialog(wxWindow* parent,
						 wxWindowID id,
						 const wxChar* title,
						 const wxSize& pageSize,
						 const int outbarStyle) :
    wxDialog(parent,id,title,wxDefaultPosition,wxDefaultSize,
		   wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    m_items = 0;
    m_selected = 0;

    m_right = new wxBoxSizer(wxVERTICAL);
    m_headline = new wxStaticText(this,-1,wxT("Headline"),
						    wxDefaultPosition,wxDefaultSize,
						    wxNO_BORDER);
    m_headline->SetFont(wxFont(14,wxSWISS,wxNORMAL,wxNORMAL));

    wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *up = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *down = new wxBoxSizer(wxHORIZONTAL);
    // wxOutBar Selection widget
    m_ob = new wxOutBar(this,-1,wxDefaultPosition,wxDefaultSize);
    up->Add(m_ob,0,wxGROW|wxALL,0);

    // headline
    m_right->Add(m_headline,0,wxGROW|wxALL,4);
    m_right->Add(new wxStaticLine(this,-1),0,wxGROW|wxALL,4);

    // panel for the setting pages
    m_panel = new wxOutbarPanel(this,-1,wxDefaultPosition,pageSize);
    m_right->Add(m_panel,1,wxGROW|wxALL,4);

    up->Add(m_right,1,wxGROW|wxALL,0);
    // Buttons for Help, Apply, Ok and Cancel
    if(outbarStyle & wxOB_HELP_BUTTON) {
	   down->Add(new wxButton(this,wxID_HELP,_("Help")),0,wxALIGN_LEFT,0);
    }
    down->Add(-1,-1,1);
    if(outbarStyle & wxOB_APPLY_BUTTON) {
	   down->Add(new wxButton(this,wxID_APPLY,_("Apply")),0,wxALIGN_RIGHT,0);
    }
    down->Add(new wxButton(this,wxID_OK,_("OK")),0,wxALIGN_RIGHT,0);
    down->Add(new wxButton(this,wxID_CANCEL,_("Cancel")),0,wxALIGN_RIGHT,0);
    top->Add(up,1,wxGROW|wxALL,8);
    top->Add(down,0,wxGROW|wxALL,8);
    SetSizerAndFit(top);
};

wxOutbarDialog::~wxOutbarDialog()
{
    delete m_headline;
};

bool wxOutbarDialog::AddPage(char** xpmimage,
					    const wxChar* label,
					    wxOutbarPanel* newPage,
					    const wxChar* headline)
{
    if(m_items < maxItems) {
	   if(m_items) {
		  // hide the last one
		  m_pages[m_items-1]->Hide();
	   }
	   m_ob->Add(new wxOutBarItem(m_ob,m_items + ID_FIRST,
							wxDefaultPosition,wxDefaultSize,
							xpmimage,label));
	   m_pages[m_items] = newPage;

	   m_selected = m_items;
	   // if not headline use label instead
	   if(headline) {
		  m_headlines[m_items] = headline;
	   }
	   else {
		  m_headlines[m_items] = label;
	   }
	   m_items++;
	   SetSizerAndFit(GetSizer());
	   return true;
    }
    return false;
};

void wxOutbarDialog::OnApply(wxCommandEvent& event)
{
    GetSelectedPage()->OnApply(event);
};

void wxOutbarDialog::OnChoice(wxCommandEvent& event)
{
    int id = event.GetId() - ID_FIRST;
    if((!m_items) || ((unsigned)id >= maxItems)) {
	   return;
    }
    SetPage(id);
};

void wxOutbarDialog::OnEraseBackground(wxEraseEvent& event)
{
    if(m_items) {
	   GetSelectedPage()->SetSize(m_panel->GetSize());
	   event.Skip();
    }
};

void wxOutbarDialog::OnHelp(wxCommandEvent& event)
{
    GetSelectedPage()->OnHelp(event);
};

void wxOutbarDialog::OnSize(wxSizeEvent& event)
{
	if(m_items) {
		GetSelectedPage()->SetSize(m_panel->GetSize());
		event.Skip();
	}
};

void wxOutbarDialog::SetPage(int pageno)
{
    m_pages[m_selected]->Hide();
    m_selected = pageno;
    m_pages[m_selected]->Show();
    // set the headline
    m_headline->SetLabel(m_headlines[m_selected]);
};

void wxOutbarDialog::SetSelection(const wxChar* byName)
{
    if(m_ob->SetSelection(byName)) {
	   int id = m_ob->GetSelectedItem()->GetId() - ID_FIRST;
	   if((unsigned)id < maxItems) SetPage(id);
    }
};
