#include "MySpellCheckDialog.h"

#include "resource.h"

// ----------------------------------------------------------------------------
// MySpellingDialog
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MySpellingDialog, wxSpellCheckDialogInterface)
    //EVT_CLOSE   (                    MySpellingDialog::OnClose)
		EVT_BUTTON(IDC_BUTTON_RECHECK_PAGE, MySpellingDialog::OnRecheckPage)
		EVT_BUTTON(IDC_BUTTON_CHECK_WORD, MySpellingDialog::OnCheckWord)
		EVT_BUTTON(IDC_BUTTON_REPLACE_WORD, MySpellingDialog::OnReplaceWord)
		EVT_BUTTON(IDC_BUTTON_IGNORE_WORD, MySpellingDialog::OnIgnoreWord)
		EVT_BUTTON(IDC_BUTTON_REPLACE_ALL, MySpellingDialog::OnReplaceAll)
		EVT_BUTTON(IDC_BUTTON_IGNORE_ALL, MySpellingDialog::OnIgnoreAll)
		EVT_BUTTON(IDC_BUTTON_ADD_WORD, MySpellingDialog::OnAddWordToCustomDictionary)
		EVT_BUTTON(IDC_BUTTON_EDIT_CUSTOM_DICT, MySpellingDialog::OnEditCustomDictionary)
		//EVT_BUTTON(IDC_BUTTON_, MySpellingDialog::On)
    EVT_LISTBOX(IDC_LIST_SUGGESTIONS, MySpellingDialog::OnChangeSuggestionSelection) 
    EVT_LISTBOX_DCLICK(IDC_LIST_SUGGESTIONS, MySpellingDialog::OnDblClkSuggestionSelection)
    EVT_BUTTON(wxID_CLOSE, MySpellingDialog::OnClose)
END_EVENT_TABLE()


MySpellingDialog::MySpellingDialog(wxWindow *parent, wxSpellCheckEngineInterface* pSpellChecker)
  : wxSpellCheckDialogInterface(parent, pSpellChecker)
{
	m_strReplaceWithText = "";

	CreateDialog();
}

MySpellingDialog::~MySpellingDialog()
{
}

void MySpellingDialog::OnClose(wxCommandEvent& event)
{
  m_nLastAction = ACTION_CLOSE;
  EndModal(true);
}

void MySpellingDialog::CreateDialog()
{
	wxBoxSizer* pTopSizer = new wxBoxSizer(wxVERTICAL);

	// Now add the controls
	
	// First the mispelling section
	wxFlexGridSizer* pMispellingSizer = new wxFlexGridSizer(3, 5, 5);

	pMispellingSizer->Add(new wxStaticText(this, -1, "Mispelled Word:", wxDefaultPosition));
	pMispellingSizer->Add(new wxTextCtrl(this, IDC_TEXT_MISPELLED_WORD, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxTextValidator(wxFILTER_NONE, &m_strMispelledWord)), 1, wxEXPAND | wxALIGN_CENTER);
	pMispellingSizer->Add(new wxButton(this, IDC_BUTTON_RECHECK_PAGE, "Recheck Page"), 0, wxEXPAND);
	pMispellingSizer->Add(new wxStaticText(this, -1, "Replace with:", wxDefaultPosition), 0, wxEXPAND | wxGROW | wxALIGN_CENTER);
	pMispellingSizer->Add(new wxTextCtrl(this, IDC_TEXT_REPLACE_WITH, "", wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NONE, &m_strReplaceWithText)), 1, wxEXPAND | wxALIGN_CENTER);
	pMispellingSizer->Add(new wxButton(this, IDC_BUTTON_CHECK_WORD, "Check Word"), 0, wxEXPAND);
	pMispellingSizer->AddGrowableCol(1);
	
	pTopSizer->Add(pMispellingSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

	// Now the suggestions section
	pTopSizer->Add(new wxStaticText(this, -1, "Suggestions:", wxDefaultPosition), 0, wxLEFT | wxRIGHT | wxTOP, 10);
	wxBoxSizer* pSuggestionSizer = new wxBoxSizer(wxHORIZONTAL);
	pSuggestionSizer->Add(new wxListBox(this, IDC_LIST_SUGGESTIONS, wxDefaultPosition, wxSize(200, 100)), 1, wxEXPAND | wxGROW | wxRIGHT, 5);
	wxBoxSizer* pSuggestionButtonSizer = new wxBoxSizer(wxVERTICAL);
	wxFlexGridSizer* pSuggestionButtonGridSizer = new wxFlexGridSizer(2, 5, 5);
	
	pSuggestionButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_REPLACE_WORD, "Replace"), 0, wxEXPAND);
	pSuggestionButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_IGNORE_WORD, "Ignore"), 0, wxEXPAND);
	pSuggestionButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_REPLACE_ALL, "Replace All"), 0, wxEXPAND);
	pSuggestionButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_IGNORE_ALL, "Ignore All"), 0, wxEXPAND);
	pSuggestionButtonSizer->Add(pSuggestionButtonGridSizer);
	
	pSuggestionButtonSizer->Add(10, 10, 1, wxEXPAND | wxGROW);
	pSuggestionButtonSizer->Add(new wxStaticText(this, -1, "Custom Dictionary:", wxDefaultPosition), 0, wxALL, 5);
	wxGridSizer* pCustomDictionaryButtonGridSizer = new wxGridSizer(2, 5, 5);
	pCustomDictionaryButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_ADD_WORD, "Add Word"), 0, wxEXPAND);
	pCustomDictionaryButtonGridSizer->Add(new wxButton(this, IDC_BUTTON_EDIT_CUSTOM_DICT, "Edit..."), 0, wxEXPAND);
	pSuggestionButtonSizer->Add(pCustomDictionaryButtonGridSizer);

	pSuggestionSizer->Add(pSuggestionButtonSizer);
	
	pTopSizer->Add(pSuggestionSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
	
	// Finally the bottom section
	pTopSizer->Add(new wxStaticText(this, -1, "Language:", wxDefaultPosition), 0, wxLEFT | wxRIGHT | wxTOP, 10);
	wxBoxSizer* pBottomRowSizer = new wxBoxSizer(wxHORIZONTAL);
	pBottomRowSizer->Add(new wxComboBox(this, IDC_COMBO_LANGUAGE), 1, wxEXPAND);
	pBottomRowSizer->Add(new wxButton(this, wxID_CLOSE, "Close"), 0, wxEXPAND | wxLEFT, 5);
	
	pTopSizer->Add(pBottomRowSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
	
  // Add a context section
  pTopSizer->Add(new wxTextCtrl(this, IDC_TEXT_CONTEXT, "", wxDefaultPosition, wxSize(320,100), wxTE_MULTILINE | wxTE_READONLY | wxTE_NOHIDESEL), 0, wxEXPAND | wxALIGN_CENTER);

	// Now attach the main sizer to the window
	SetSizer(pTopSizer);
	pTopSizer->SetSizeHints(this);
}

void MySpellingDialog::OnRecheckPage(wxCommandEvent& event)
{
	// Spell check the whole document
}

void MySpellingDialog::OnCheckWord(wxCommandEvent& event)
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

void MySpellingDialog::OnReplaceWord(wxCommandEvent& event)
{
	// Replace this word with the value of the "Replace With:" field value.
	//	For Aspell, send this replacement information to aspell_speller_store_repl (optionally)
	TransferDataFromWindow();
  m_nLastAction = ACTION_REPLACE;
  Show(FALSE);
}

void MySpellingDialog::OnIgnoreWord(wxCommandEvent& event)
{
	// Assume that the word is spelled correctly
  m_nLastAction = ACTION_IGNORE;
  Show(FALSE);
}

void MySpellingDialog::OnReplaceAll(wxCommandEvent& event)
{
	// Set a flag indicating that the user doesn't want to look at any more mispelling words
	//	I guess that in this case, we always accept the first suggestion.
	// Possibly the meaning for this one is actually that we should replace all occurrences of THIS WORD only
	//	and send this replacement information to aspell_speller_store_repl (optionally)
  m_nLastAction = ACTION_REPLACE_ALWAYS;
  Show(FALSE);
}

void MySpellingDialog::OnIgnoreAll(wxCommandEvent& event)
{
	// Set a flag indicating that the user doesn't want to look at any more mispelling words
	//	assume that all words are spelled correctly
	// Possibly the meaning for this one is actually that we should ignore all occurrences of THIS WORD only
  m_nLastAction = ACTION_IGNORE_ALWAYS;
  Show(FALSE);
}

void MySpellingDialog::OnAddWordToCustomDictionary(wxCommandEvent& event)
{
	// Nothing really needed for this other than adding the word to the custom dictionary and closing the dialog
  if (m_pSpellCheckEngine != NULL)
  {
    if (!(m_pSpellCheckEngine->AddWordToDictionary(m_strMispelledWord)))
      ::wxMessageBox(_T("There was an error adding \"" + m_strMispelledWord + "\" to the personal dictionary"));
  }
  Show(FALSE);
}

void MySpellingDialog::OnEditCustomDictionary(wxCommandEvent& event)
{
	// Bring up the "Edit Custom Dictionary" dialog
  MyPersonalDictionaryDialog* pCustomDictionaryDlg = new MyPersonalDictionaryDialog(this, m_pSpellCheckEngine);
  pCustomDictionaryDlg->ShowModal();
  delete pCustomDictionaryDlg;
}

void MySpellingDialog::OnChangeLanguage(wxCommandEvent& event)
{
	// Set the option on the spell checker library to use this newly-selected language and get a new list of suggestions
}

void MySpellingDialog::OnChangeSuggestionSelection(wxCommandEvent& event)
{
	// When the selection in the "suggestions:" list box changes, than we want to update the "Replace with:" edit field to match this selection
  wxListBox* pListBox = (wxListBox*)FindWindow(IDC_LIST_SUGGESTIONS);
  if (pListBox)
  {
    m_strReplaceWithText = pListBox->GetStringSelection();
    TransferDataToWindow();
  }
}

void MySpellingDialog::OnDblClkSuggestionSelection(wxCommandEvent& event)
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

void MySpellingDialog::SetMispelledWord(const wxString& strMispelling)
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
      /*
      // This code worked in the 2.4.x branch
      pContextText->SetValue(Context.GetContext());
      pContextText->SetSelection(Context.GetOffset(), Context.GetOffset() + Context.GetLength());
      pContextText->SetStyle(Context.GetOffset(), Context.GetOffset() + Context.GetLength(), wxTextAttr(*wxRED, *wxLIGHT_GREY));
    */
      wxString strContext = Context.GetContext();
      pContextText->SetValue(strContext.Left(Context.GetOffset()));
      wxColour originalTextColour = pContextText->GetDefaultStyle().GetTextColour();
      pContextText->SetDefaultStyle(wxTextAttr(*wxRED));
      pContextText->AppendText(strContext.Mid(Context.GetOffset(), Context.GetLength()));
      pContextText->SetDefaultStyle(wxTextAttr(originalTextColour));
      pContextText->AppendText(strContext.Right(strContext.Length() - (Context.GetOffset() + Context.GetLength())));
    }
  }
  TransferDataToWindow();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    MyPersonalDictionaryDialog
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyPersonalDictionaryDialog, wxDialog)
  EVT_BUTTON(IDC_BUTTON_ADD_TO_DICT, MyPersonalDictionaryDialog::AddWordToPersonalDictionary)
  EVT_BUTTON(IDC_BUTTON_REPLACE_IN_DICT, MyPersonalDictionaryDialog::ReplaceInPersonalDictionary)
  EVT_BUTTON(IDC_BUTTON_REMOVE_FROM_DICT, MyPersonalDictionaryDialog::RemoveFromPersonalDictionary)
  EVT_BUTTON(wxID_CLOSE, MyPersonalDictionaryDialog::OnClose)
END_EVENT_TABLE()

MyPersonalDictionaryDialog::MyPersonalDictionaryDialog(wxWindow* parent, wxSpellCheckEngineInterface* pEngine)
  : wxDialog(parent, -1, wxString("Personal Dictionary"),wxDefaultPosition, wxSize(230,175), wxDEFAULT_DIALOG_STYLE) 
{
  m_pSpellCheckEngine = pEngine;
  CreateDialog();
}

MyPersonalDictionaryDialog::~MyPersonalDictionaryDialog()
{
}

void MyPersonalDictionaryDialog::CreateDialog()
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

void MyPersonalDictionaryDialog::PopulatePersonalWordListBox()
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

void MyPersonalDictionaryDialog::AddWordToPersonalDictionary(wxCommandEvent& event)
{
  if (m_pSpellCheckEngine != NULL)
  {
    TransferDataFromWindow();
    wxWindow* pText = FindWindow(IDC_TEXT_NEW_PERSONAL_WORD);
    if (pText != NULL)
    {
      wxString strNewWord = pText->GetLabel();
      if (!strNewWord.Trim().IsEmpty())
      {
        if (!(m_pSpellCheckEngine->AddWordToDictionary(strNewWord)))
          ::wxMessageBox(_T("There was an error adding \"" + strNewWord + "\" to the personal dictionary"));
      }
    }
    PopulatePersonalWordListBox();
  }
}

void MyPersonalDictionaryDialog::ReplaceInPersonalDictionary(wxCommandEvent& event)
{
}

void MyPersonalDictionaryDialog::RemoveFromPersonalDictionary(wxCommandEvent& event)
{
  if (m_pSpellCheckEngine != NULL)
  {
    TransferDataFromWindow();
    wxListBox* pListBox = (wxListBox*)FindWindow(IDC_LIST_PERSONAL_WORDS);
    if (pListBox)
    {
      wxString strNewWord = pListBox->GetStringSelection();
      if (!strNewWord.Trim().IsEmpty())
      {
        if (!(m_pSpellCheckEngine->RemoveWordFromDictionary(strNewWord)))
          ::wxMessageBox(_T("There was an error removing \"" + strNewWord + "\" to the personal dictionary"));
      }
    }
    PopulatePersonalWordListBox();
  }
}

void MyPersonalDictionaryDialog::OnClose(wxCommandEvent& event)
{
  EndModal(true);
}
