#include "SpellCheckerOptionsDialog.h"
#include <wx/statline.h>
#include <wx/spinctrl.h>
#include <wx/filename.h>

#include "open.xpm"

BEGIN_EVENT_TABLE(SpellCheckerOptionsDialog, wxDialog)
END_EVENT_TABLE()

SpellCheckerOptionsDialog::SpellCheckerOptionsDialog(wxWindow* pParent, const wxString& strCaption, OptionsMap* pOptionsMap)
  : wxDialog(pParent, -1, strCaption, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  // Put each option in the member options map
  for (OptionsMap::iterator it = pOptionsMap->begin(); it != pOptionsMap->end(); it++)
    m_ModifiedOptions[it->first] = it->second;
  
  CreateControls();
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
  Centre();
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

    wxButton* item6 = new wxButton( item1, wxID_OK, _T("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    item6->SetDefault();
    item5->Add(item6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* item7 = new wxButton( item1, wxID_CANCEL, _T("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    item5->Add(item7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end SpellCheckerOptionsDialog content construction
}

void SpellCheckerOptionsDialog::PopulateOptionsSizer(wxSizer* pSizer)
{
  for (OptionsMap::iterator it = m_ModifiedOptions.begin(); it != m_ModifiedOptions.end(); it++)
  {
    SpellCheckEngineOption* pCurrentOption = &(it->second);
        // Add the options to the sizer label first, then the value
    // It would be great to add validators and file browser buttons
    // for the options (all wrapped with the value in a horizontal box sizer)
    //  but don't worry about that for now
    if (pCurrentOption->GetShowOption())
    {
      int nOptionType = pCurrentOption->GetOptionType();
      wxString strName = pCurrentOption->GetName();
      // Label
      if (nOptionType != SpellCheckEngineOption::BOOLEAN)
        pSizer->Add(new wxStaticText(this, -1, pCurrentOption->GetText() + _T(":")), 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
      else
        pSizer->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5); // Spacer
      
      // Value
      if (nOptionType == SpellCheckEngineOption::STRING)
      {
        // If pCurrentOption->GetPossibleValuesArray()->GetCount() > 0, we should probably display a wxChoice control
        // rather than an edit field, populate the choices from the array contents and default to pCurrentOption->GetValueAsString()
        if (pCurrentOption->GetPossibleValuesArray()->GetCount() > 0)
        {
          wxString* ChoiceStrings = NULL;
          wxChoice* pChoice = new wxChoice( this, -1, wxDefaultPosition, wxDefaultSize, 0, ChoiceStrings, 0, wxDefaultValidator, strName);
          VariantArray* pArray = pCurrentOption->GetPossibleValuesArray();
          for (unsigned int i=0; i<pArray->GetCount(); i++)
            pChoice->Append(pArray->Item(i));
          //Set the default item IF it's in the wxChoice
          if (pChoice->FindString(pCurrentOption->GetValueAsString()) != -1)
            pChoice->SetStringSelection(pCurrentOption->GetValueAsString());
          // Finally add this wxChoice control to the dialog
          pSizer->Add(pChoice, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
        }
        else
        {
          wxTextCtrl* item5 = new wxTextCtrl( this, -1, pCurrentOption->GetValueAsString(), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, strName);
          pSizer->Add(item5, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
        }
      }
      else if (nOptionType == SpellCheckEngineOption::LONG)
      {
        wxSpinCtrl* item7 = new wxSpinCtrl( this, -1, pCurrentOption->GetValueAsString(), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, pCurrentOption->GetLongValue(), strName);
        pSizer->Add(item7, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
      }
      else if (nOptionType == SpellCheckEngineOption::DOUBLE)
      {
        wxTextCtrl* item9 = new wxTextCtrl( this, -1, pCurrentOption->GetValueAsString(), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, strName);
        pSizer->Add(item9, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
      }
      else if (nOptionType == SpellCheckEngineOption::BOOLEAN)
      {
        wxCheckBox* item11 = new wxCheckBox( this, -1, pCurrentOption->GetText(), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, strName);
        item11->SetValue(pCurrentOption->GetBoolValue());
        pSizer->Add(item11, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
      }
      else if ((nOptionType == SpellCheckEngineOption::DIR) ||
        (nOptionType == SpellCheckEngineOption::FILE))
      {
        wxBoxSizer* item13 = new wxBoxSizer(wxHORIZONTAL);
        pSizer->Add(item13, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    
        wxTextCtrl* item14 = new wxTextCtrl( this, -1, pCurrentOption->GetValueAsString(), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, strName);
        item13->Add(item14, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    
        wxBitmap OpenBitmap(open_xpm);
        wxBitmapButton* item15 = new wxBitmapButton( this, -1, OpenBitmap, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, strName + _T("-browse"));
        item13->Add(item15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
        
        // Add an event handler so that the button presents a wxFileDialog or wxDirDialog is presented
        if (nOptionType == SpellCheckEngineOption::DIR)
        {
          Connect(item15->GetId(),  wxEVT_COMMAND_BUTTON_CLICKED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &SpellCheckerOptionsDialog::OnBrowseForDir);
        }
        else
        {
          Connect(item15->GetId(),  wxEVT_COMMAND_BUTTON_CLICKED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &SpellCheckerOptionsDialog::OnBrowseForFile);
        }
      }
      else
      {
        wxTextCtrl* item5 = new wxTextCtrl( this, -1, pCurrentOption->GetValueAsString(), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, strName);
        pSizer->Add(item5, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
      };
    }
  }
}

void SpellCheckerOptionsDialog::OnBrowseForDir(wxCommandEvent& event)
{
  // First get the event control name, then remove the "-browse" suffix to determine the
  //  string to default to
  TransferDataFromWindow();
  
  wxString strBrowseName = ((wxWindow*)(event.GetEventObject()))->GetName();
  wxString strOptionName = strBrowseName.Left(strBrowseName.Length() - wxString(_T("-browse")).Length());
  wxWindow* pControl = wxWindow::FindWindowByName(strOptionName, this);
  wxString strDefaultDir = "";
  if (pControl)
  {
    strDefaultDir = ((wxTextCtrl*)pControl)->GetValue();
  }
  wxDirDialog DirDialog(this,  _T("Choose a directory"), strDefaultDir);
  if (DirDialog.ShowModal() == wxID_OK)
  {
    m_ModifiedOptions[strOptionName].SetValue(DirDialog.GetPath());
    TransferDataToWindow();
  }
}

void SpellCheckerOptionsDialog::OnBrowseForFile(wxCommandEvent& event)
{
  TransferDataFromWindow();
  wxString strBrowseName = ((wxWindow*)(event.GetEventObject()))->GetName();
  wxString strOptionName = strBrowseName.Left(strBrowseName.Length() - wxString(_T("-browse")).Length());
  wxWindow* pControl = wxWindow::FindWindowByName(strOptionName, this);
  wxString strDefaultDir = _T("");
  wxString strDefaultFileName = _T("");
  if (pControl)
  {
    wxFileName FileName(((wxTextCtrl*)pControl)->GetValue());
    strDefaultDir = FileName.GetPath();
    strDefaultFileName = FileName.GetFullName();
  }
  wxFileDialog FileDialog(this, _T("Choose a file"), strDefaultDir, strDefaultFileName);
  if (FileDialog.ShowModal() == wxID_OK)
  {
    m_ModifiedOptions[strOptionName].SetValue(FileDialog.GetPath());
    TransferDataToWindow();
  }
}

bool SpellCheckerOptionsDialog::TransferDataFromWindow()
{
  // Loop through the m_ModifiedOptions, find the dialog control by name,
  // and set the m_ModifiedOptions value based on the control value
  double dblValue = 0.00;
  for (OptionsMap::iterator it = m_ModifiedOptions.begin(); it != m_ModifiedOptions.end(); it++)
  {
    SpellCheckEngineOption* pCurrentOption = &(it->second);
    if (pCurrentOption->GetShowOption())
    {
      wxString strName = pCurrentOption->GetName();
      wxWindow* pControl = wxWindow::FindWindowByName(strName, this);
      if (pControl)
      {
        switch (pCurrentOption->GetOptionType())
        {
        case SpellCheckEngineOption::STRING:
        case SpellCheckEngineOption::DIR:
        case SpellCheckEngineOption::FILE:
          pCurrentOption->SetValue(((wxTextCtrl*)pControl)->GetValue());
          break;
        case SpellCheckEngineOption::LONG:
          pCurrentOption->SetValue((long)((wxSpinCtrl*)pControl)->GetValue());
          break;
        case SpellCheckEngineOption::DOUBLE:
          ((wxTextCtrl*)pControl)->GetValue().ToDouble(&dblValue);
          pCurrentOption->SetValue(dblValue);
          break;
        case SpellCheckEngineOption::BOOLEAN:
          pCurrentOption->SetValue(((wxCheckBox*)pControl)->GetValue());
          break;
        default:
          return false;
          break;
        }
      }
    }
  }
  return true;
}

bool SpellCheckerOptionsDialog::TransferDataToWindow()
{
  // Loop through the m_ModifiedOptions, find the dialog control by name,
  // and set the control value based on the m_ModifiedOptions value
  for (OptionsMap::iterator it = m_ModifiedOptions.begin(); it != m_ModifiedOptions.end(); it++)
  {
    SpellCheckEngineOption* pCurrentOption = &(it->second);
    if (pCurrentOption->GetShowOption())
    {
      wxString strName = pCurrentOption->GetName();
      wxWindow* pControl = wxWindow::FindWindowByName(strName, this);
      if (pControl)
      {
        switch (pCurrentOption->GetOptionType())
        {
        case SpellCheckEngineOption::STRING:
        case SpellCheckEngineOption::DIR:
        case SpellCheckEngineOption::FILE:
          ((wxTextCtrl*)pControl)->SetValue(pCurrentOption->GetValueAsString());
          break;
        case SpellCheckEngineOption::LONG:
          ((wxSpinCtrl*)pControl)->SetValue(pCurrentOption->GetLongValue());
          break;
        case SpellCheckEngineOption::DOUBLE:
          ((wxTextCtrl*)pControl)->SetValue(pCurrentOption->GetValueAsString());
          break;
        case SpellCheckEngineOption::BOOLEAN:
          ((wxCheckBox*)pControl)->SetValue(pCurrentOption->GetBoolValue());
          break;
        default:
          return false;
          break;
        }
      }
    }
  }
  return true;
}
