#include "SentryLikeDialog.h"

#include "resource.h"

// ----------------------------------------------------------------------------
// SentryLikeDialog
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(SentryLikeDialog, wxSpellCheckDialogInterface)
    //EVT_CLOSE   (                    SentryLikeDialog::OnClose)
		EVT_BUTTON(IDC_BUTTON_RECHECK_PAGE, SentryLikeDialog::OnRecheckPage)
		EVT_BUTTON(IDC_BUTTON_CHECK_WORD, SentryLikeDialog::OnCheckWord)
		EVT_BUTTON(IDC_BUTTON_REPLACE_WORD, SentryLikeDialog::OnReplaceWord)
		EVT_BUTTON(IDC_BUTTON_IGNORE_WORD, SentryLikeDialog::OnIgnoreWord)
		EVT_BUTTON(IDC_BUTTON_REPLACE_ALL, SentryLikeDialog::OnReplaceAll)
		EVT_BUTTON(IDC_BUTTON_IGNORE_ALL, SentryLikeDialog::OnIgnoreAll)
		EVT_BUTTON(IDC_BUTTON_ADD_WORD, SentryLikeDialog::OnAddWordToCustomDictionary)
		EVT_BUTTON(IDC_BUTTON_EDIT_CUSTOM_DICT, SentryLikeDialog::OnEditCustomDictionary)
		//EVT_BUTTON(IDC_BUTTON_, SentryLikeDialog::On)
    EVT_LISTBOX(IDC_LIST_SUGGESTIONS, SentryLikeDialog::OnChangeSuggestionSelection) 
    EVT_LISTBOX_DCLICK(IDC_LIST_SUGGESTIONS, SentryLikeDialog::OnDblClkSuggestionSelection)
    EVT_BUTTON(IDC_BUTTON_OPTIONS, SentryLikeDialog::OnClickOptions)
    EVT_BUTTON(wxID_CLOSE, SentryLikeDialog::OnClose)
END_EVENT_TABLE()


SentryLikeDialog::SentryLikeDialog(wxWindow *parent, wxSpellCheckEngineInterface* pSpellChecker)
  : wxSpellCheckDialogInterface(parent, pSpellChecker)
{
	m_strReplaceWithText = "";

	CreateDialog();
}

void SentryLikeDialog::OnClose(wxCommandEvent& event)
{
  m_nLastAction = ACTION_CLOSE;
  EndModal(true);
}

void SentryLikeDialog::CreateDialog()
{
	wxBoxSizer* pTopSizer = new wxBoxSizer(wxVERTICAL);

	// Now add the controls
	
	wxBoxSizer* pJustUnderTopSizer = new wxBoxSizer(wxHORIZONTAL);

	// First the mispelling section
	wxBoxSizer* pMispellingSizer = new wxBoxSizer(wxVERTICAL);

	pMispellingSizer->Add(new wxStaticText(this, -1, "Not in Dictionary:", wxDefaultPosition));
	pMispellingSizer->Add(new wxTextCtrl(this, IDC_TEXT_MISPELLED_WORD, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxTextValidator(wxFILTER_NONE, &m_strMispelledWord)), 0, wxEXPAND | wxALIGN_CENTER);
	pMispellingSizer->Add(new wxStaticText(this, -1, "Suggestions:", wxDefaultPosition), 0, wxEXPAND);
	pMispellingSizer->Add(new wxListBox(this, IDC_LIST_SUGGESTIONS, wxDefaultPosition, wxSize(200, 100)), 1, wxEXPAND | wxGROW | wxRIGHT, 5);
	pMispellingSizer->Add(new wxStaticText(this, -1, "Add words to:", wxDefaultPosition), 0, wxEXPAND);
	pMispellingSizer->Add(new wxComboBox(this, IDC_COMBO_DICTIONARY), 0, wxEXPAND);
	pJustUnderTopSizer->Add(pMispellingSizer, 1, wxEXPAND | wxALL, 5);
	
	wxBoxSizer* pRightButtonSizer = new wxBoxSizer(wxVERTICAL);
	pRightButtonSizer->Add(new wxButton(this, IDC_BUTTON_IGNORE_WORD, "Ignore"), 0, wxEXPAND | wxALL, 5);
	pRightButtonSizer->Add(new wxButton(this, IDC_BUTTON_IGNORE_ALL, "Ignore All"), 0, wxEXPAND | wxALL, 5);
	pRightButtonSizer->Add(new wxButton(this, IDC_BUTTON_ADD_WORD, "Add Word"), 0, wxEXPAND | wxALL, 5);
	pRightButtonSizer->Add(new wxButton(this, IDC_BUTTON_REPLACE_WORD, "Replace"), 0, wxEXPAND | wxALL, 5);
	pRightButtonSizer->Add(new wxButton(this, IDC_BUTTON_REPLACE_ALL, "Replace All"), 0, wxEXPAND | wxALL, 5);
	pRightButtonSizer->Add(new wxButton(this, IDC_BUTTON_CHECK_WORD, "Check Word"), 0, wxEXPAND | wxALL, 5);
	
	pJustUnderTopSizer->Add(pRightButtonSizer, 0, wxEXPAND | wxGROW);

	pTopSizer->Add(pJustUnderTopSizer, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

	// Now the suggestions section
	wxBoxSizer* pBottomButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	pBottomButtonSizer->Add(new wxButton(this, -1/*IDC_BUTTON_EDIT_CUSTOM_DICT*/, "Undo"), 0, wxEXPAND | wxALL, 5);
	pBottomButtonSizer->Add(new wxButton(this, IDC_BUTTON_OPTIONS, "Options"), 0, wxEXPAND | wxALL, 5);
	pBottomButtonSizer->Add(new wxButton(this, IDC_BUTTON_EDIT_CUSTOM_DICT, "Dictionaries"), 0, wxEXPAND | wxALL, 5);
	pBottomButtonSizer->Add(new wxButton(this, -1/*IDC_BUTTON_EDIT_CUSTOM_DICT*/, "Help"), 0, wxEXPAND | wxALL, 5);
	pBottomButtonSizer->Add(new wxButton(this, wxID_CLOSE, "Cancel"), 0, wxEXPAND | wxALL, 5);
	
	pTopSizer->Add(pBottomButtonSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
	
	// Now attach the main sizer to the window
	SetSizer(pTopSizer);
	pTopSizer->SetSizeHints(this);
}

void SentryLikeDialog::OnRecheckPage(wxCommandEvent& event)
{
	// Spell check the whole document
}

void SentryLikeDialog::OnCheckWord(wxCommandEvent& event)
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
      wxArrayString SuggestionArray = m_pSpellCheckEngine->GetSuggestions(m_strMispelledWord);
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

void SentryLikeDialog::OnReplaceWord(wxCommandEvent& event)
{
	// Replace this word with the value of the "Replace With:" field value.
	//	For Aspell, send this replacement information to aspell_speller_store_repl (optionally)
	TransferDataFromWindow();
  m_nLastAction = ACTION_REPLACE;
  Show(FALSE);
}

void SentryLikeDialog::OnIgnoreWord(wxCommandEvent& event)
{
	// Assume that the word is spelled correctly
  m_nLastAction = ACTION_IGNORE;
  Show(FALSE);
}

void SentryLikeDialog::OnReplaceAll(wxCommandEvent& event)
{
	// Set a flag indicating that the user doesn't want to look at any more mispelling words
	//	I guess that in this case, we always accept the first suggestion.
	// Possibly the meaning for this one is actually that we should replace all occurrences of THIS WORD only
	//	and send this replacement information to aspell_speller_store_repl (optionally)
  m_nLastAction = ACTION_REPLACE_ALWAYS;
  Show(FALSE);
}

void SentryLikeDialog::OnIgnoreAll(wxCommandEvent& event)
{
	// Set a flag indicating that the user doesn't want to look at any more mispelling words
	//	assume that all words are spelled correctly
	// Possibly the meaning for this one is actually that we should ignore all occurrences of THIS WORD only
  m_nLastAction = ACTION_IGNORE_ALWAYS;
  Show(FALSE);
}

void SentryLikeDialog::OnAddWordToCustomDictionary(wxCommandEvent& event)
{
	// Nothing really needed for this other than adding the word to the custom dictionary and closing the dialog
  if (m_pSpellCheckEngine != NULL)
  {
    m_pSpellCheckEngine->AddWordToDictionary(m_strMispelledWord);
  }
  Show(FALSE);
}

void SentryLikeDialog::OnEditCustomDictionary(wxCommandEvent& event)
{
	// Bring up the "Edit Custom Dictionary" dialog
  MyPersonalSentryLikeDictionaryDialog* pCustomDictionaryDlg = new MyPersonalSentryLikeDictionaryDialog(this, m_pSpellCheckEngine);
  pCustomDictionaryDlg->ShowModal();
  delete pCustomDictionaryDlg;
}

void SentryLikeDialog::OnChangeLanguage(wxCommandEvent& event)
{
	// Set the option on the spell checker library to use this newly-selected language and get a new list of suggestions
}

void SentryLikeDialog::OnChangeSuggestionSelection(wxCommandEvent& event)
{
	// When the selection in the "suggestions:" list box changes, than we want to update the "Replace with:" edit field to match this selection
  wxListBox* pListBox = (wxListBox*)FindWindow(IDC_LIST_SUGGESTIONS);
  if (pListBox)
  {
    m_strMispelledWord = pListBox->GetStringSelection();
    TransferDataToWindow();
  }
}

void SentryLikeDialog::OnDblClkSuggestionSelection(wxCommandEvent& event)
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

void SentryLikeDialog::SetMispelledWord(const wxString& strMispelling)
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

void SentryLikeDialog::OnClickOptions(wxCommandEvent& event)
{
  m_pSpellCheckEngine->PresentOptions();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    MyPersonalSentryLikeDictionaryDialog
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyPersonalSentryLikeDictionaryDialog, wxDialog)
  EVT_BUTTON(IDC_BUTTON_ADD_TO_DICT, MyPersonalSentryLikeDictionaryDialog::AddWordToPersonalDictionary)
  EVT_BUTTON(IDC_BUTTON_REPLACE_IN_DICT, MyPersonalSentryLikeDictionaryDialog::ReplaceInPersonalDictionary)
  EVT_BUTTON(IDC_BUTTON_REMOVE_FROM_DICT, MyPersonalSentryLikeDictionaryDialog::RemoveFromPersonalDictionary)
  EVT_BUTTON(wxID_CLOSE, MyPersonalSentryLikeDictionaryDialog::OnClose)
END_EVENT_TABLE()

MyPersonalSentryLikeDictionaryDialog::MyPersonalSentryLikeDictionaryDialog(wxWindow* parent, wxSpellCheckEngineInterface* pEngine)
  : wxDialog(parent, -1, wxString("Personal Dictionary"),wxDefaultPosition, wxSize(230,175), wxDEFAULT_DIALOG_STYLE) 
{
  m_pSpellCheckEngine = pEngine;
  CreateDialog();
}

MyPersonalSentryLikeDictionaryDialog::~MyPersonalSentryLikeDictionaryDialog()
{
}

void MyPersonalSentryLikeDictionaryDialog::CreateDialog()
{
	wxBoxSizer* pTopSizer = new wxBoxSizer(wxVERTICAL);
  
  pTopSizer->Add(10, 10);
  wxBoxSizer* pPadLabelSizer = new wxBoxSizer(wxHORIZONTAL);
  pPadLabelSizer->Add(10, 10);
	pPadLabelSizer->Add(new wxStaticText(this, -1, "New Word:", wxDefaultPosition));
  pTopSizer->Add(pPadLabelSizer);

  wxBoxSizer* pNewWordSizer = new wxBoxSizer(wxHORIZONTAL);
  pNewWordSizer->Add(10, 0);
	pNewWordSizer->Add(new wxTextCtrl(this, IDC_TEXT_NEW_PERSONAL_WORD, "", wxDefaultPosition), 1, wxEXPAND | wxGROW);
  pNewWordSizer->Add(new wxButton(this, IDC_BUTTON_ADD_TO_DICT, "Add"), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
  pTopSizer->Add(pNewWordSizer, 0, wxEXPAND | wxGROW, 10);
  pTopSizer->Add(10, 10);

  wxBoxSizer* pPadLabelSizer2 = new wxBoxSizer(wxHORIZONTAL);
  pPadLabelSizer2->Add(10, 10);
  pPadLabelSizer2->Add(new wxStaticText(this, -1, "Words in dictionary:", wxDefaultPosition));
  pTopSizer->Add(pPadLabelSizer2);
  wxBoxSizer* pWordListSizer = new wxBoxSizer(wxHORIZONTAL);

  pWordListSizer->Add(new wxListBox(this, IDC_LIST_PERSONAL_WORDS, wxDefaultPosition, wxSize(200, 150)), 1, wxEXPAND | wxGROW | wxLEFT | wxRIGHT | wxBOTTOM, 10);

  wxBoxSizer* pWordButtonsSizer = new wxBoxSizer(wxVERTICAL);
  pWordButtonsSizer->Add(new wxButton(this, IDC_BUTTON_REPLACE_IN_DICT, "Replace"), 0, wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM, 10);
  pTopSizer->Add(5, 5);
  pWordButtonsSizer->Add(new wxButton(this, IDC_BUTTON_REMOVE_FROM_DICT, "Remove"), 0, wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM, 10);
  pTopSizer->Add(5, 5);
  pWordButtonsSizer->Add(new wxButton(this, wxID_CLOSE, "Close"), 0, wxEXPAND | wxTOP | wxRIGHT, 10);
  pWordListSizer->Add(pWordButtonsSizer, 0, wxEXPAND | wxGROW, 10);

  pTopSizer->Add(pWordListSizer, 1, wxEXPAND);

  // Now attach the main sizer to the window
	SetSizer(pTopSizer);
	pTopSizer->SetSizeHints(this);

  PopulatePersonalWordListBox();
}

void MyPersonalSentryLikeDictionaryDialog::PopulatePersonalWordListBox()
{
  if (m_pSpellCheckEngine != NULL)
  {
    wxListBox* pListBox = (wxListBox*)FindWindow(IDC_LIST_PERSONAL_WORDS);
    if (pListBox)
    {
      // Get a list of suggestions to populate the list box
      wxArrayString PersonalWords = m_pSpellCheckEngine->GetWordListAsArray();
      pListBox->Clear();

      // Add each suggestion to the list
      for (unsigned int nCtr = 0; nCtr < PersonalWords.GetCount(); nCtr++)
        pListBox->Append(PersonalWords[nCtr]);
      // If the previous word had no suggestions than the box may be disabled, enable it here to be safe
      pListBox->Enable(TRUE);
    }
  }
}

void MyPersonalSentryLikeDictionaryDialog::AddWordToPersonalDictionary(wxCommandEvent& event)
{
  if (m_pSpellCheckEngine != NULL)
  {
    TransferDataFromWindow();
    wxWindow* pText = FindWindow(IDC_TEXT_NEW_PERSONAL_WORD);
    if (pText != NULL)
    {
      wxString strNewWord = pText->GetLabel();
      if (!strNewWord.Trim().IsEmpty())
      m_pSpellCheckEngine->AddWordToDictionary(strNewWord);
    }
    PopulatePersonalWordListBox();
  }
}

void MyPersonalSentryLikeDictionaryDialog::ReplaceInPersonalDictionary(wxCommandEvent& event)
{
}

void MyPersonalSentryLikeDictionaryDialog::RemoveFromPersonalDictionary(wxCommandEvent& event)
{
}

void MyPersonalSentryLikeDictionaryDialog::OnClose(wxCommandEvent& event)
{
  EndModal(true);
}
