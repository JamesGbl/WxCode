#include "SpellCheckerOptionsDialog.h"
#include <wx/statline.h>
#include <wx/spinctrl.h>

SpellCheckerOptionsDialog::SpellCheckerOptionsDialog(wxWindow* pParent, const wxString& strCaption, OptionsMap* pOptionsMap)
  : wxDialog(pParent, -1, strCaption, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m_pOptionsMap = pOptionsMap;
  CreateControls();
}

void SpellCheckerOptionsDialog::CreateControls()
{    
////@begin SpellCheckerOptionsDialog content construction

    SpellCheckerOptionsDialog* item1 = this;

    wxBoxSizer* item2 = new wxBoxSizer(wxVERTICAL);
    item1->SetSizer(item2);
    item1->SetAutoLayout(TRUE);

    wxFlexGridSizer* item3 = new wxFlexGridSizer(2, 2, 0, 0);
    item3->AddGrowableCol(1);
    PopulateOptionsSizer(item3);
    item2->Add(item3, 1, wxGROW|wxALL, 5);

    wxStaticLine* item4 = new wxStaticLine( item1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    item2->Add(item4, 0, wxGROW|wxALL, 5);

    wxBoxSizer* item5 = new wxBoxSizer(wxHORIZONTAL);
    item2->Add(item5, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* item6 = new wxButton( item1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    item6->SetDefault();
    item5->Add(item6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* item7 = new wxButton( item1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    item5->Add(item7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end SpellCheckerOptionsDialog content construction
}

void SpellCheckerOptionsDialog::PopulateOptionsSizer(wxSizer* pSizer)
{
  if (m_pOptionsMap != NULL)
  {
    OptionsMap::iterator it;
    for (it = m_pOptionsMap->begin(); it != m_pOptionsMap->end(); it++)
    {
      SpellCheckEngineOption CurrentOption = it->second;
      // Add the options to the sizer label first, then the value
      // It would be great to add validators and file browser buttons
      // for the options (all wrapped with the value in a horizontal box sizer)
      //  but don't worry about that for now
      int nOptionType = CurrentOption.GetOptionType();
      // Label
      if (nOptionType != SpellCheckEngineOption::BOOLEAN)
        pSizer->Add(new wxStaticText(this, -1, CurrentOption.GetText() + " (" + wxString::Format("%d", nOptionType) + ") :"));
      else
        pSizer->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5); // Spacer
      
      // Value
      if (nOptionType == SpellCheckEngineOption::STRING)
      {
        wxTextCtrl* item5 = new wxTextCtrl( this, -1, CurrentOption.GetValueAsString(), wxDefaultPosition, wxDefaultSize, 0 );
        pSizer->Add(item5, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
      }
      else if (nOptionType == SpellCheckEngineOption::LONG)
      {
        wxSpinCtrl* item7 = new wxSpinCtrl( this, -1, CurrentOption.GetValueAsString(), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
        pSizer->Add(item7, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
      }
      else if (nOptionType == SpellCheckEngineOption::DOUBLE)
      {
        wxTextCtrl* item9 = new wxTextCtrl( this, -1, CurrentOption.GetValueAsString(), wxDefaultPosition, wxDefaultSize, 0 );
        pSizer->Add(item9, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
      }
      else if (nOptionType == SpellCheckEngineOption::BOOLEAN)
      {
        wxCheckBox* item11 = new wxCheckBox( this, -1, CurrentOption.GetText() + " (" + wxString::Format("%d", nOptionType) + ")", wxDefaultPosition, wxDefaultSize, 0 );
        item11->SetValue(CurrentOption.GetBoolValue());
        pSizer->Add(item11, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
      }
      else if ((nOptionType == SpellCheckEngineOption::DIR) ||
        (nOptionType == SpellCheckEngineOption::FILE))
      {
        wxBoxSizer* item13 = new wxBoxSizer(wxHORIZONTAL);
        pSizer->Add(item13, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    
        wxTextCtrl* item14 = new wxTextCtrl( this, -1, CurrentOption.GetValueAsString(), wxDefaultPosition, wxDefaultSize, 0 );
        item13->Add(item14, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    
        wxButton* item15 = new wxButton( this, -1, _("..."), wxDefaultPosition, wxDefaultSize, 0 );
        item13->Add(item15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
      }
      else
      {
        wxTextCtrl* item5 = new wxTextCtrl( this, -1, CurrentOption.GetValueAsString(), wxDefaultPosition, wxDefaultSize, 0 );
        pSizer->Add(item5, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
      };
    }
  }
}

void SpellCheckerOptionsDialog::OnOK(wxCommandEvent& event)
{
  ::wxMessageBox("Go ahead!");
}
