/////////////////////////////////////////////////////////////////////////////
// Name:        DlgLang.cpp
// Purpose:     
// Author:      Ernesto Rangel Dallet
// Modified by: 
// Created:     12/04/05 20:11:36
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "DlgLang.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include ".\dlglang.h"

IMPLEMENT_DYNAMIC_CLASS( DlgLang, wxDialog )

BEGIN_EVENT_TABLE( DlgLang, wxDialog )

    EVT_COMBOBOX( ID_COMBOBOX, DlgLang::OnComboboxSelected )

END_EVENT_TABLE()


DlgLang::DlgLang( )
{
}

DlgLang::DlgLang( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

bool DlgLang::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    m_pCboLangs = NULL;
    m_SelLang = _("English");

    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );
    CreateControls();

    // select english by default
    m_pCboLangs->SetSelection(m_pCboLangs->FindString(_("English")));
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
    return true;
}

void DlgLang::CreateControls()
{    
    DlgLang* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Select the language for the application"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString m_pCboLangsStrings[] = {
        _("English"),
        _("Spanish")
    };
    m_pCboLangs = new wxComboBox( itemDialog1, ID_COMBOBOX, _T(""), wxDefaultPosition, wxDefaultSize, 2, m_pCboLangsStrings, wxCB_READONLY );
    itemBoxSizer2->Add(m_pCboLangs, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_RIGHT|wxLEFT|wxTOP, 5);

    wxButton* itemButton6 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

}

bool DlgLang::ShowToolTips()
{
    return true;
}


void DlgLang::OnComboboxSelected( wxCommandEvent& event )
{
    m_SelLang = m_pCboLangs->GetStringSelection();
}

wxString DlgLang::GetSelLanguage(void)
{
    ShowModal();
    return m_SelLang;
}
