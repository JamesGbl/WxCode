#include "Outlook97LikeDialog.h"
#include "SpellCheckerOptionsDialog.h"

#include "resource.h"

// ----------------------------------------------------------------------------
// Outlook97LikeDialog
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(Outlook97LikeDialog, wxSpellCheckDialogInterface)
    //EVT_CLOSE   (                    Outlook97LikeDialog::OnClose)
		EVT_BUTTON(IDC_BUTTON_CHECK_WORD, Outlook97LikeDialog::OnCheckWord)
		EVT_BUTTON(IDC_BUTTON_REPLACE_WORD, Outlook97LikeDialog::OnReplaceWord)
		EVT_BUTTON(IDC_BUTTON_IGNORE_WORD, Outlook97LikeDialog::OnIgnoreWord)
		EVT_BUTTON(IDC_BUTTON_REPLACE_ALL, Outlook97LikeDialog::OnReplaceAll)
		EVT_BUTTON(IDC_BUTTON_IGNORE_ALL, Outlook97LikeDialog::OnIgnoreAll)
		EVT_BUTTON(IDC_BUTTON_ADD_WORD, Outlook97LikeDialog::OnAddWordToCustomDictionary)
		//EVT_BUTTON(IDC_BUTTON_, Outlook97LikeDialog::On)
    EVT_LISTBOX(IDC_LIST_SUGGESTIONS, Outlook97LikeDialog::OnChangeSuggestionSelection) 
    EVT_LISTBOX_DCLICK(IDC_LIST_SUGGESTIONS, Outlook97LikeDialog::OnDblClkSuggestionSelection)
    EVT_BUTTON(IDC_BUTTON_OPTIONS, Outlook97LikeDialog::OnClickOptions)
    EVT_BUTTON(wxID_CLOSE, Outlook97LikeDialog::OnClose)
END_EVENT_TABLE()


Outlook97LikeDialog::Outlook97LikeDialog(wxWindow *parent, wxSpellCheckEngineInterface* pSpellChecker)
                : wxSpellCheckDialogInterface(parent, pSpellChecker)
{
	m_strReplaceWithText = "";

	CreateDialog();
}

Outlook97LikeDialog::~Outlook97LikeDialog()
{
}

void Outlook97LikeDialog::OnClose(wxCommandEvent& event)
{
  m_nLastAction = ACTION_CLOSE;
  EndModal(true);
}

void Outlook97LikeDialog::CreateDialog()
{
	wxBoxSizer* pTopSizer = new wxBoxSizer(wxVERTICAL);

	// Now add the controls
	
	// First the mispelling section
	wxFlexGridSizer* pMispellingSizer = new wxFlexGridSizer(2, 5, 5);
	pMispellingSizer->AddGrowableCol(1);

	pMispellingSizer->Add(new wxStaticText(this, -1, "Not in Dictionary:", wxDefaultPosition));
	pMispellingSizer->Add(new wxTextCtrl(this, IDC_TEXT_MISPELLED_WORD, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxTextValidator(wxFILTER_NONE, &m_strMispelledWord)), 1, wxEXPAND | wxALIGN_CENTER);
	pMispellingSizer->Add(new wxStaticText(this, -1, "Change to:", wxDefaultPosition), 0, wxEXPAND | wxGROW | wxALIGN_CENTER);
	pMispellingSizer->Add(new wxTextCtrl(this, IDC_TEXT_REPLACE_WITH, "", wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NONE, &m_strReplaceWithText)), 1, wxEXPAND | wxALIGN_CENTER);
	// Now the suggestions section
	pMispellingSizer->Add(new wxStaticText(this, -1, "Suggestions:", wxDefaultPosition), 0);

  wxBoxSizer* pSuggestionsSizer = new wxBoxSizer(wxHORIZONTAL);
	pSuggestionsSizer->Add(new wxListBox(this, IDC_LIST_SUGGESTIONS, wxDefaultPosition, wxSize(200, 100)), 1, wxEXPAND | wxGROW | wxRIGHT, 5);

  wxFlexGridSizer* pSuggestionButtonGridSizer = new wxFlexGridSizer(2, 5, 5);
	
	pSuggestionButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_IGNORE_WORD, "Ignore"), 0, wxEXPAND);
	pSuggestionButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_IGNORE_ALL, "Ignore All"), 0, wxEXPAND);
	pSuggestionButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_REPLACE_WORD, "Change"), 0, wxEXPAND);
	pSuggestionButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_REPLACE_ALL, "Change All"), 0, wxEXPAND);
	pSuggestionButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_ADD_WORD, "Add"), 0, wxEXPAND);
  pSuggestionButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_CHECK_WORD, "Suggest"), 0, wxEXPAND);
	pSuggestionButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_OPTIONS, "Options..."), 0, wxEXPAND);
	pSuggestionButtonGridSizer->Add(new wxButton(this, wxID_CLOSE, "Close"), 0, wxEXPAND);
	pSuggestionsSizer->Add(pSuggestionButtonGridSizer);

  pMispellingSizer->Add(pSuggestionsSizer);

	pTopSizer->Add(pMispellingSizer, 1, wxEXPAND | wxALL, 10);

	// Now attach the main sizer to the window
	SetSizer(pTopSizer);
	pTopSizer->SetSizeHints(this);
}

void Outlook97LikeDialog::OnCheckWord(wxCommandEvent& event)
{
	// Check if this word "replace with" word passes the spell check
	// Populate the "Suggestions:" list box if it fails
  TransferDataFromWindow();
  if (m_pSpellCheckEngine != NULL)
  {
    wxListBox* pListBox = (wxListBox*)FindWindow(IDC_LIST_SUGGESTIONS);
    if (pListBox)
    {
      // Get a list of suggestions to populate the list box
      wxArrayString SuggestionArray = m_pSpellCheckEngine->GetSuggestions(m_strReplaceWithText);
      pListBox->Clear();
      if (SuggestionArray.GetCount() > 0)
      {
        // Add each suggestion to the list
        for (unsigned int nCtr = 0; nCtr < SuggestionArray.GetCount(); nCtr++)
          pListBox->Append(SuggestionArray[nCtr]);
        // If the previous word had no suggestions than the box may be disabled, enable it here to be safe
        pListBox->Enable(TRUE);
      }
      else
      {
        pListBox->Append("(no suggestions)");
        pListBox->Enable(FALSE);
      }
    }
  }
  TransferDataToWindow();
}

void Outlook97LikeDialog::OnReplaceWord(wxCommandEvent& event)
{
	// Replace this word with the value of the "Replace With:" field value.
	//	For Aspell, send this replacement information to aspell_speller_store_repl (optionally)
	TransferDataFromWindow();
  m_nLastAction = ACTION_REPLACE;
  Show(FALSE);
}

void Outlook97LikeDialog::OnIgnoreWord(wxCommandEvent& event)
{
	// Assume that the word is spelled correctly
  m_nLastAction = ACTION_IGNORE;
  Show(FALSE);
}

void Outlook97LikeDialog::OnReplaceAll(wxCommandEvent& event)
{
	// Set a flag indicating that the user doesn't want to look at any more mispelling words
	//	I guess that in this case, we always accept the first suggestion.
	// Possibly the meaning for this one is actually that we should replace all occurrences of THIS WORD only
	//	and send this replacement information to aspell_speller_store_repl (optionally)
  m_nLastAction = ACTION_REPLACE_ALWAYS;
  Show(FALSE);
}

void Outlook97LikeDialog::OnIgnoreAll(wxCommandEvent& event)
{
	// Set a flag indicating that the user doesn't want to look at any more mispelling words
	//	assume that all words are spelled correctly
	// Possibly the meaning for this one is actually that we should ignore all occurrences of THIS WORD only
  m_nLastAction = ACTION_IGNORE_ALWAYS;
  Show(FALSE);
}

void Outlook97LikeDialog::OnAddWordToCustomDictionary(wxCommandEvent& event)
{
	// Nothing really needed for this other than adding the word to the custom dictionary and closing the dialog
  if (m_pSpellCheckEngine != NULL)
  {
    if (!(m_pSpellCheckEngine->AddWordToDictionary(m_strMispelledWord)))
      ::wxMessageBox(_T("There was an error adding \"" + m_strMispelledWord + "\" to the personal dictionary"));
  }
  Show(FALSE);
}

void Outlook97LikeDialog::OnChangeSuggestionSelection(wxCommandEvent& event)
{
	// When the selection in the "suggestions:" list box changes, than we want to update the "Replace with:" edit field to match this selection
  wxListBox* pListBox = (wxListBox*)FindWindow(IDC_LIST_SUGGESTIONS);
  if (pListBox)
  {
    m_strReplaceWithText = pListBox->GetStringSelection();
    TransferDataToWindow();
  }
}

void Outlook97LikeDialog::OnDblClkSuggestionSelection(wxCommandEvent& event)
{
	// When the selection in the "suggestions:" list box changes, than we want to update the "Replace with:" edit field to match this selection
  wxListBox* pListBox = (wxListBox*)FindWindow(IDC_LIST_SUGGESTIONS);
  if (pListBox)
  {
    // Set the replace with text to that of the selected list item
    m_strReplaceWithText = pListBox->GetStringSelection();
    // To the spell check engine to replace the text
    
    // Close the dialog
    m_nLastAction = ACTION_REPLACE;
    Show(FALSE);
  }
}

void Outlook97LikeDialog::SetMispelledWord(const wxString& strMispelling)
{
  m_strMispelledWord = strMispelling;

  if (m_pSpellCheckEngine != NULL)
  {
    wxListBox* pListBox = (wxListBox*)FindWindow(IDC_LIST_SUGGESTIONS);
    if (pListBox)
    {
      // Get a list of suggestions to populate the list box
      wxArrayString SuggestionArray = m_pSpellCheckEngine->GetSuggestions(m_strMispelledWord);
      pListBox->Clear();
      if (SuggestionArray.GetCount() > 0)
      {
        // Add each suggestion to the list
        for (unsigned int nCtr = 0; nCtr < SuggestionArray.GetCount(); nCtr++)
          pListBox->Append(SuggestionArray[nCtr]);
        // If the previous word had no suggestions than the box may be disabled, enable it here to be safe
        pListBox->Enable(TRUE);
        // Default the value of the "replace with" text to be first item from the suggestions
        pListBox->SetSelection(0);
        m_strReplaceWithText = pListBox->GetString(0);
      }
      else
      {
        pListBox->Append("(no suggestions)");
        pListBox->Enable(FALSE);
      }
    }
    wxTextCtrl* pContextText = (wxTextCtrl*)FindWindow(IDC_TEXT_CONTEXT);
    if (pContextText)
    {
      wxSpellCheckEngineInterface::MispellingContext Context = m_pSpellCheckEngine->GetCurrentMispellingContext();
      pContextText->SetEditable(FALSE);
      pContextText->Clear();
      pContextText->SetValue(Context.GetContext());
      pContextText->SetSelection(Context.GetOffset(), Context.GetOffset() + Context.GetLength());
      pContextText->SetStyle(Context.GetOffset(), Context.GetOffset() + Context.GetLength(), wxTextAttr(*wxRED, *wxLIGHT_GREY));
    }
  }
  TransferDataToWindow();
}

void Outlook97LikeDialog::OnClickOptions(wxCommandEvent& event)
{
  // Create a really basic dialog that gets dynamically populated
  // with controls based on the m_pSpellCheckEngine->GetOptions();
  SpellCheckerOptionsDialog OptionsDialog(this, m_pSpellCheckEngine->GetSpellCheckEngineName() + _T(" Options"), m_pSpellCheckEngine);
  if (OptionsDialog.ShowModal() == wxID_OK)
  {
    // Set the modified options
    OptionsMap* pOptionsMap = OptionsDialog.GetModifiedOptions();
    if (pOptionsMap)
    {
      for (OptionsMap::iterator it = pOptionsMap->begin(); it != pOptionsMap->end(); it++)
        m_pSpellCheckEngine->AddOptionToMap(it->second);
    }
  }
}

