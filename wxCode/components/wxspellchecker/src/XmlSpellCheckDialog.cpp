#include "XmlSpellCheckDialog.h"
#include <wx/xrc/xmlres.h>

#include <wx/listbox.h>
#include <wx/valgen.h>

#include "SpellCheckerOptionsDialog.h"
/*
  The following control IDs are used

  // Spell Check Dialog
  ButtonRecheckPage
  ButtonCheckWord
  ButtonReplaceWord
  ButtonIgnoreWord
  ButtonReplaceAll
  ButtonIgnoreAll
  ButtonAddWord
  ButtonEditCustomDist
  ButtonOptions
  ListBoxSuggestions
  ButtonClose
  Either TextMisspelledWord or StaticMisspelledWord (wxTextCtrl or wxStaticText)
  TextReplaceWith
  TextContext

  // Personal Dictionary Dialog (if used)
  ListPersonalWords
  TextNewPersonalWord
*/

// ----------------------------------------------------------------------------
// XmlSpellCheckDialog
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(XmlSpellCheckDialog, wxSpellCheckDialogInterface)
/*
  //EVT_CLOSE   (                    XmlSpellCheckDialog::OnClose)
		EVT_BUTTON(XRCID("ButtonRecheckPage"), XmlSpellCheckDialog::OnRecheckPage)
		EVT_BUTTON(XRCID("ButtonCheckWord"), XmlSpellCheckDialog::OnCheckWord)
		EVT_BUTTON(XRCID("ButtonReplaceWord"), XmlSpellCheckDialog::OnReplaceWord)
		EVT_BUTTON(XRCID("ButtonIgnoreWord"), XmlSpellCheckDialog::OnIgnoreWord)
		EVT_BUTTON(XRCID("ButtonReplaceAll"), XmlSpellCheckDialog::OnReplaceAll)
		EVT_BUTTON(XRCID("ButtonIgnoreAll"), XmlSpellCheckDialog::OnIgnoreAll)
		EVT_BUTTON(XRCID("ButtonAddWord"), XmlSpellCheckDialog::OnAddWordToCustomDictionary)
		EVT_BUTTON(XRCID("ButtonEditCustomDist"), XmlSpellCheckDialog::OnEditCustomDictionary)
    EVT_LISTBOX(XRCID("ListBoxSuggestions"), XmlSpellCheckDialog::OnChangeSuggestionSelection) 
    EVT_LISTBOX_DCLICK(XRCID("ListBoxSuggestions"), XmlSpellCheckDialog::OnDblClkSuggestionSelection)
    EVT_BUTTON(XRCID("ButtonClose"), XmlSpellCheckDialog::OnClose)
*/
END_EVENT_TABLE()


XmlSpellCheckDialog::XmlSpellCheckDialog(wxWindow *parent, wxString strDialogResource, wxString strWordListResource, wxSpellCheckEngineInterface* pSpellChecker)
  : wxSpellCheckUserInterface(pSpellChecker) // DON'T call wxDialog constructor
{
	m_strReplaceWithText = "";
  m_strDialogResource = strDialogResource;
  m_strWordListResource = strWordListResource;

	CreateDialog(parent);
}

XmlSpellCheckDialog::~XmlSpellCheckDialog()
{
}

int XmlSpellCheckDialog::PresentSpellCheckUserInterface(const wxString& strMispelling)
{
  // If the dialog is being presented, then default the previous action flag first
  SetMispelledWord(strMispelling);
		
	m_nLastAction = ACTION_INITIAL;

  ShowModal();

	return m_nLastAction;
}

void XmlSpellCheckDialog::OnClose(wxCommandEvent& event)
{
  m_nLastAction = ACTION_CLOSE;
  EndModal(true);
}

void XmlSpellCheckDialog::CreateDialog(wxWindow* pParent)
{
  // Load the XML resource
  wxXmlResource::Get()->InitAllHandlers();
  if (wxXmlResource::Get()->Load(_T("abiword.xrc")) == false)
    return;
  
  if (wxXmlResource::Get()->LoadDialog(this, pParent, m_strDialogResource) == false)
    return;
  
  // Verify that the controls we need are present
/*
  XRCID("ButtonRecheckPage")
  XRCID("ButtonCheckWord")
	XRCID("ButtonReplaceWord")
	XRCID("ButtonIgnoreWord")
	XRCID("ButtonReplaceAll")
	XRCID("ButtonIgnoreAll")
	XRCID("ButtonAddWord")
	XRCID("ButtonEditCustomDist")
  XRCID("ListBoxSuggestions")
  XRCID("ButtonClose")
  XRCID("TextContext")
*/

		//EVT_BUTTON(XRCID("ButtonRecheckPage"), XmlSpellCheckDialog::OnRecheckPage)
    if (XRCCTRL(*this,"ButtonRecheckPage",wxButton))
    {
      Connect(XRCID("ButtonRecheckPage"),  wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &XmlSpellCheckDialog::OnRecheckPage);
    }
    
		//EVT_BUTTON(XRCID("ButtonCheckWord"), XmlSpellCheckDialog::OnCheckWord)
    if (XRCCTRL(*this,"ButtonCheckWord",wxButton))
    {
      Connect(XRCID("ButtonCheckWord"),  wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &XmlSpellCheckDialog::OnCheckWord);
    }
    
		//EVT_BUTTON(XRCID("ButtonReplaceWord"), XmlSpellCheckDialog::OnReplaceWord)
    if (XRCCTRL(*this,"ButtonReplaceWord",wxButton))
    {
      Connect(XRCID("ButtonReplaceWord"),  wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &XmlSpellCheckDialog::OnReplaceWord);
    }
    
		//EVT_BUTTON(XRCID("ButtonIgnoreWord"), XmlSpellCheckDialog::OnIgnoreWord)
    if (XRCCTRL(*this,"ButtonIgnoreWord",wxButton))
    {
      Connect(XRCID("ButtonIgnoreWord"),  wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &XmlSpellCheckDialog::OnIgnoreWord);
    }
    
		//EVT_BUTTON(XRCID("ButtonReplaceAll"), XmlSpellCheckDialog::OnReplaceAll)
    if (XRCCTRL(*this,"ButtonReplaceAll",wxButton))
    {
      Connect(XRCID("ButtonReplaceAll"),  wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &XmlSpellCheckDialog::OnReplaceAll);
    }
    
		//EVT_BUTTON(XRCID("ButtonIgnoreAll"), XmlSpellCheckDialog::OnIgnoreAll)
    if (XRCCTRL(*this,"ButtonIgnoreAll",wxButton))
    {
      Connect(XRCID("ButtonIgnoreAll"),  wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &XmlSpellCheckDialog::OnIgnoreAll);
    }
    
		//EVT_BUTTON(XRCID("ButtonAddWord"), XmlSpellCheckDialog::OnAddWordToCustomDictionary)
    if (XRCCTRL(*this,"ButtonAddWord",wxButton))
    {
      Connect(XRCID("ButtonAddWord"),  wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &XmlSpellCheckDialog::OnAddWordToCustomDictionary);
    }
    
		//EVT_BUTTON(XRCID("ButtonEditCustomDist"), XmlSpellCheckDialog::OnEditCustomDictionary)
    if (XRCCTRL(*this,"ButtonEditCustomDist",wxButton))
    {
      Connect(XRCID("ButtonEditCustomDist"),  wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &XmlSpellCheckDialog::OnEditCustomDictionary);
    }
    
    
    if (XRCCTRL(*this,"ButtonOptions",wxButton))
    {
      Connect(XRCID("ButtonOptions"),  wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &XmlSpellCheckDialog::OnOptions);
    }

    //EVT_LISTBOX(XRCID("ListBoxSuggestions"), XmlSpellCheckDialog::OnChangeSuggestionSelection) 
    //EVT_LISTBOX_DCLICK(XRCID("ListBoxSuggestions"), XmlSpellCheckDialog::OnDblClkSuggestionSelection)
    if (XRCCTRL(*this,"ListBoxSuggestions",wxButton))
    {
      Connect(XRCID("ListBoxSuggestions"),  wxEVT_COMMAND_LISTBOX_SELECTED,
          (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &XmlSpellCheckDialog::OnChangeSuggestionSelection);
      Connect(XRCID("ListBoxSuggestions"),  wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,
          (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &XmlSpellCheckDialog::OnDblClkSuggestionSelection);
    }
    
    //EVT_BUTTON(XRCID("ButtonClose"), XmlSpellCheckDialog::OnClose)
    if (XRCCTRL(*this,"ButtonClose",wxButton))
    {
      Connect(XRCID("ButtonClose"),  wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &XmlSpellCheckDialog::OnClose);
    }

    // Add data transfer validators
    
    if (XRCCTRL(*this,"TextMisspelledWord",wxTextCtrl))
    {
      wxGenericValidator MisspellingValidator(&m_strMispelledWord);
      XRCCTRL(*this,"TextMisspelledWord",wxTextCtrl)->SetValidator(MisspellingValidator);
    }
    else if (XRCCTRL(*this,"StaticMisspelledWord",wxStaticText))
    {
      wxGenericValidator MisspellingValidator(&m_strMispelledWord);
      XRCCTRL(*this,"StaticMisspelledWord",wxStaticText)->SetValidator(MisspellingValidator);
    }

    if (XRCCTRL(*this,"TextReplaceWith",wxTextCtrl))
    {
      wxGenericValidator ReplaceWithValidator(&m_strReplaceWithText);
      XRCCTRL(*this,"TextReplaceWith",wxTextCtrl)->SetValidator(ReplaceWithValidator);
    }
}

void XmlSpellCheckDialog::OnRecheckPage(wxCommandEvent& event)
{
	// Spell check the whole document
}

void XmlSpellCheckDialog::OnCheckWord(wxCommandEvent& event)
{
	// Check if this word "replace with" word passes the spell check
	// Populate the "Suggestions:" list box if it fails
  TransferDataFromWindow();
  if (m_pSpellCheckEngine != NULL)
  {
    wxListBox* pListBox = XRCCTRL(*this, "ListBoxSuggestions", wxListBox);
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

void XmlSpellCheckDialog::OnReplaceWord(wxCommandEvent& event)
{
	// Replace this word with the value of the "Replace With:" field value.
	//	For Aspell, send this replacement information to aspell_speller_store_repl (optionally)
	TransferDataFromWindow();
  m_nLastAction = ACTION_REPLACE;
  Show(FALSE);
}

void XmlSpellCheckDialog::OnIgnoreWord(wxCommandEvent& event)
{
	// Assume that the word is spelled correctly
  m_nLastAction = ACTION_IGNORE;
  Show(FALSE);
}

void XmlSpellCheckDialog::OnReplaceAll(wxCommandEvent& event)
{
	// Set a flag indicating that the user doesn't want to look at any more mispelling words
	//	I guess that in this case, we always accept the first suggestion.
	// Possibly the meaning for this one is actually that we should replace all occurrences of THIS WORD only
	//	and send this replacement information to aspell_speller_store_repl (optionally)
  m_nLastAction = ACTION_REPLACE_ALWAYS;
  Show(FALSE);
}

void XmlSpellCheckDialog::OnIgnoreAll(wxCommandEvent& event)
{
	// Set a flag indicating that the user doesn't want to look at any more mispelling words
	//	assume that all words are spelled correctly
	// Possibly the meaning for this one is actually that we should ignore all occurrences of THIS WORD only
  m_nLastAction = ACTION_IGNORE_ALWAYS;
  Show(FALSE);
}

void XmlSpellCheckDialog::OnAddWordToCustomDictionary(wxCommandEvent& event)
{
	// Nothing really needed for this other than adding the word to the custom dictionary and closing the dialog
  if (m_pSpellCheckEngine != NULL)
  {
    m_pSpellCheckEngine->AddWordToDictionary(m_strMispelledWord);
  }
  Show(FALSE);
}

void XmlSpellCheckDialog::OnEditCustomDictionary(wxCommandEvent& event)
{
	// Bring up the "Edit Custom Dictionary" dialog
  XmlPersonalDictionaryDialog* pCustomDictionaryDlg = new XmlPersonalDictionaryDialog(this, m_strWordListResource, m_pSpellCheckEngine);
  pCustomDictionaryDlg->ShowModal();
  delete pCustomDictionaryDlg;
}

void XmlSpellCheckDialog::OnChangeLanguage(wxCommandEvent& event)
{
	// Set the option on the spell checker library to use this newly-selected language and get a new list of suggestions
}

void XmlSpellCheckDialog::OnChangeSuggestionSelection(wxCommandEvent& event)
{
	// When the selection in the "suggestions:" list box changes, than we want to update the "Replace with:" edit field to match this selection
  wxListBox* pListBox = XRCCTRL(*this, "ListBoxSuggestions", wxListBox);
  if (pListBox)
  {
    m_strReplaceWithText = pListBox->GetStringSelection();
    TransferDataToWindow();
  }
}

void XmlSpellCheckDialog::OnDblClkSuggestionSelection(wxCommandEvent& event)
{
	// When the selection in the "suggestions:" list box changes, than we want to update the "Replace with:" edit field to match this selection
  wxListBox* pListBox = XRCCTRL(*this, "ListBoxSuggestions", wxListBox);
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

void XmlSpellCheckDialog::SetMispelledWord(const wxString& strMispelling)
{
  m_strMispelledWord = strMispelling;

  if (m_pSpellCheckEngine != NULL)
  {
    wxListBox* pListBox = XRCCTRL(*this, "ListBoxSuggestions", wxListBox);
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
    wxTextCtrl* pContextText = XRCCTRL(*this, "TextContext", wxTextCtrl);
    if (pContextText)
    {
      wxSpellCheckEngineInterface::MispellingContext Context = m_pSpellCheckEngine->GetCurrentMispellingContext();
      pContextText->SetEditable(FALSE);
      pContextText->Clear();
      
      #if wxCHECK_VERSION(2, 5, 0)
        // This code works in the 2.5.x branch
        wxString strContext = Context.GetContext();
        pContextText->SetValue(strContext.Left(Context.GetOffset()));
        wxColour originalTextColour = pContextText->GetDefaultStyle().GetTextColour();
        pContextText->SetDefaultStyle(wxTextAttr(*wxRED));
        pContextText->AppendText(strContext.Mid(Context.GetOffset(), Context.GetLength()));
        pContextText->SetDefaultStyle(wxTextAttr(originalTextColour));
        pContextText->AppendText(strContext.Right(strContext.Length() - (Context.GetOffset() + Context.GetLength())));*/
      #else  
        // This code works in the 2.4.x branch
        pContextText->SetValue(Context.GetContext());        pContextText->SetSelection(Context.GetOffset(), Context.GetOffset() + Context.GetLength());        pContextText->SetStyle(Context.GetOffset(), Context.GetOffset() + Context.GetLength(), wxTextAttr(*wxRED, *wxLIGHT_GREY));
      #endif
    }
  }
  TransferDataToWindow();
}

void XmlSpellCheckDialog::OnOptions(wxCommandEvent& event)
{
  m_pSpellCheckEngine->PresentOptions();
  // Create a really basic dialog with a scrolled panel that gets dynamically populated
  // with controls based on the m_pSpellCheckEngine->GetOptions();
  SpellCheckerOptionsDialog OptionsDialog(this, "Options", m_pSpellCheckEngine->GetOptions());
  OptionsDialog.ShowModal();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    XmlPersonalDictionaryDialog
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(XmlPersonalDictionaryDialog, wxDialog)
  EVT_BUTTON(XRCID("ButtonAddToDict"), XmlPersonalDictionaryDialog::AddWordToPersonalDictionary)
  EVT_BUTTON(XRCID("ButtonReplaceInDict"), XmlPersonalDictionaryDialog::ReplaceInPersonalDictionary)
  EVT_BUTTON(XRCID("ButtonRemoveFromDict"), XmlPersonalDictionaryDialog::RemoveFromPersonalDictionary)
  EVT_BUTTON(XRCID("ButtonClose"), XmlPersonalDictionaryDialog::OnClose)
END_EVENT_TABLE()

XmlPersonalDictionaryDialog::XmlPersonalDictionaryDialog(wxWindow* parent, wxString strResource, wxSpellCheckEngineInterface* pEngine)
{
  m_pSpellCheckEngine = pEngine;
  m_strDialogResource = strResource;
  CreateDialog(parent);
}

XmlPersonalDictionaryDialog::~XmlPersonalDictionaryDialog()
{
}

void XmlPersonalDictionaryDialog::CreateDialog(wxWindow* pParent)
{
  // Load the XML resource
  wxXmlResource::Get()->InitAllHandlers();
  wxXmlResource::Get()->LoadDialog(this, pParent, m_strDialogResource);
  
  // Verify that the controls we need are present

  // Now present the word list to the user
  PopulatePersonalWordListBox();
}

void XmlPersonalDictionaryDialog::PopulatePersonalWordListBox()
{
  if (m_pSpellCheckEngine != NULL)
  {
    wxListBox* pListBox = XRCCTRL(*this, "ListPersonalWords", wxListBox);
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

void XmlPersonalDictionaryDialog::AddWordToPersonalDictionary(wxCommandEvent& event)
{
  if (m_pSpellCheckEngine != NULL)
  {
    TransferDataFromWindow();
    wxWindow* pText = XRCCTRL(*this, "TextNewPersonalWord", wxWindow);
    if (pText != NULL)
    {
      wxString strNewWord = pText->GetLabel();
      if (!strNewWord.Trim().IsEmpty())
      m_pSpellCheckEngine->AddWordToDictionary(strNewWord);
    }
    PopulatePersonalWordListBox();
  }
}

void XmlPersonalDictionaryDialog::ReplaceInPersonalDictionary(wxCommandEvent& event)
{
}

void XmlPersonalDictionaryDialog::RemoveFromPersonalDictionary(wxCommandEvent& event)
{
}

void XmlPersonalDictionaryDialog::OnClose(wxCommandEvent& event)
{
  EndModal(true);
}
