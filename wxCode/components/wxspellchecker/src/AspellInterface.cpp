#include "AspellInterface.h"
#include "SpellCheckUserInterface.h"

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/msgdlg.h>
#include <wx/dir.h>
#include <wx/filename.h>

AspellInterface::AspellInterface(wxSpellCheckUserInterface* pDlg /* = NULL */)
{
  m_AspellConfig = NULL;
  m_AspellSpeller = NULL;
  m_AspellChecker = NULL;

  m_pSpellUserInterface = pDlg;
  if (m_pSpellUserInterface != NULL)
    m_pSpellUserInterface->SetSpellCheckEngine(this);

  SetDefaultOptions();

  InitializeSpellCheckEngine();
}

AspellInterface::~AspellInterface()
{
	if (m_bPersonalDictionaryModified)
	{
	  if (wxYES == ::wxMessageBox("Would you like to save any of your changes to your personal dictionary?", "Save Changes", wxYES_NO | wxICON_QUESTION))
      m_AspellWrapper.AspellSpellerSaveAllWordLists(m_AspellSpeller);
	}

  if (m_AspellChecker != NULL)
    m_AspellWrapper.DeleteAspellDocumentChecker(m_AspellChecker);

  if (m_AspellConfig != NULL)
    m_AspellWrapper.DeleteAspellConfig(m_AspellConfig);

  if (m_AspellSpeller != NULL)
    m_AspellWrapper.DeleteAspellSpeller(m_AspellSpeller);

  if (m_pSpellUserInterface != NULL)
  {
    delete m_pSpellUserInterface;
    m_pSpellUserInterface = NULL;
  }
  
  m_AspellWrapper.Unload();
}

int AspellInterface::InitializeSpellCheckEngine()
{
  if (!m_AspellWrapper.LoadFunctions())
    return FALSE;
  
	if (m_AspellConfig == NULL)
		m_AspellConfig = m_AspellWrapper.NewAspellConfig();
	
	if (m_AspellConfig == NULL)
		return FALSE;

  AspellCanHaveError* ret = m_AspellWrapper.NewAspellSpeller(m_AspellConfig);
  if (m_AspellWrapper.AspellError(ret) != 0)
	{
    ::wxMessageBox(wxString::Format("Error: %s\n",m_AspellWrapper.AspellErrorMessage(ret)));
    m_AspellWrapper.DeleteAspellCanHaveError(ret);
    return FALSE;
  }
  m_AspellSpeller = m_AspellWrapper.ToAspellSpeller(ret);
  
  ApplyOptions(); // Not really sure where the best place to do this is...
  
	return TRUE;
}

int AspellInterface::UninitializeSpellCheckEngine()
{
  m_AspellWrapper.Unload();

	return TRUE;
}

int AspellInterface::SetOption(SpellCheckEngineOption& Option)
{
	if (m_AspellConfig == NULL)
		m_AspellConfig = m_AspellWrapper.NewAspellConfig();

	if (m_AspellConfig == NULL)
		return FALSE;
		
	m_AspellWrapper.AspellConfigReplace(m_AspellConfig, Option.GetName(), Option.GetValueAsString());

	return TRUE;
}

bool AspellInterface::IsWordInDictionary(const wxString& strWord)
{
  if (m_AspellSpeller == NULL)
    return false;

  return (m_AspellWrapper.AspellSpellerCheck(m_AspellSpeller, strWord, strWord.Length()) == 1);
}

// This function loops through the document and check each word.
wxString AspellInterface::CheckSpelling(wxString strText)
{
  if (m_AspellSpeller == NULL)
    return "";

	//int nCorrect = m_AspellWrapper.AspellSpellerCheck(m_AspellSpeller, strText, strText.Length());
  /* Set up the document checker */
  AspellCanHaveError* ret = m_AspellWrapper.NewAspellDocumentChecker(m_AspellSpeller);
  if (m_AspellWrapper.AspellError(ret) != 0) {
    ::wxMessageBox(wxString::Format("Error: %s\n",m_AspellWrapper.AspellErrorMessage(ret)));
    return "";
  }
  m_AspellChecker = m_AspellWrapper.ToAspellDocumentChecker(ret);

  m_AspellWrapper.AspellDocumentCheckerProcess(m_AspellChecker , strText, -1);

  int nDiff = 0;

  AspellToken token;
  /* Now find the misspellings in the line */
  while (token = m_AspellWrapper.AspellDocumentCheckerNextMisspelling(m_AspellChecker), token.len != 0)
  {
		token.offset += nDiff;
		wxString strBadWord = strText.Mid(token.offset, token.len);

		// If this word is in the always ignore list, then just move on
		if (m_AlwaysIgnoreList.Index(strBadWord) != wxNOT_FOUND)
			continue;

		bool bReplaceFromMap = FALSE;
		StringToStringMap::iterator WordFinder = m_AlwaysReplaceMap.find(strBadWord);
		if (WordFinder != m_AlwaysReplaceMap.end())
			bReplaceFromMap = TRUE;
		
		int nUserReturnValue = 0;
		if (!bReplaceFromMap)
		{
	    // Define the context of the word
  	  DefineContext(strText, token.offset, token.len);

    	// Print out the misspelling and get a replasment from the user 
	    // Present the dialog so the user can tell us what to do with this word
  	  nUserReturnValue = GetUserCorrection(strBadWord);  //Show function will show the dialog and not return until the user makes a decision
		}
		
		if (nUserReturnValue == wxSpellCheckUserInterface::ACTION_CLOSE)
		{
			break;
		}
		else if ((nUserReturnValue == wxSpellCheckUserInterface::ACTION_REPLACE) || bReplaceFromMap)
		{
			wxString strReplacementText = (bReplaceFromMap) ? (*WordFinder).second : m_pSpellUserInterface->GetReplacementText();
			// Increase/Decreate the character difference so that the next loop is on track
			nDiff += strReplacementText.Length() - token.len;
			// Let the spell checker know what the correct replacement was
			m_AspellWrapper.AspellSpellerStoreReplacement(m_AspellSpeller, strBadWord, token.len,
																strReplacementText, strReplacementText.Length());
			m_bPersonalDictionaryModified = TRUE;	// Storing this information modifies the dictionary
	    // Replace the misspelled word with the replacement */
			strText.replace(token.offset, token.len, strReplacementText);
		}
  }

  m_AspellWrapper.DeleteAspellDocumentChecker(m_AspellChecker);
  m_AspellChecker = NULL;

//	return (nCorrect != 0);
	
  return strText;
}

wxArrayString AspellInterface::GetSuggestions(const wxString& strMisspelledWord)
{
  wxArrayString wxReturnArray;
  const AspellWordList * suggestions = m_AspellWrapper.AspellSpellerSuggest(m_AspellSpeller, strMisspelledWord, strMisspelledWord.Length());

  AspellStringEnumeration * elements = m_AspellWrapper.AspellWordListElements(suggestions);

  const char * word;
  wxReturnArray.IsEmpty();
  while ( (word = m_AspellWrapper.AspellStringEnumerationNext(elements)) != NULL )
  {
    // add to suggestion list
    wxReturnArray.Add(word);
  }
  m_AspellWrapper.DeleteAspellStringEnumeration(elements);

  return wxReturnArray;
}

int AspellInterface::AddWordToDictionary(const wxString& strWord)
{
	m_AspellWrapper.AspellSpellerAddToPersonal(m_AspellSpeller, strWord, strWord.Length());
	m_bPersonalDictionaryModified = TRUE;
  return TRUE;
}

wxArrayString AspellInterface::GetWordListAsArray()
{
  wxArrayString wxReturnArray;
  const AspellWordList* PersonalWordList = m_AspellWrapper.AspellSpellerPersonalWordList(m_AspellSpeller);

  AspellStringEnumeration* elements = m_AspellWrapper.AspellWordListElements(PersonalWordList);

  const char * word;
  wxReturnArray.IsEmpty();
  while ( (word = m_AspellWrapper.AspellStringEnumerationNext(elements)) != NULL )
  {
    // add to suggestion list
    wxReturnArray.Add(word);
  }
  m_AspellWrapper.DeleteAspellStringEnumeration(elements); 

  return wxReturnArray;
}

void AspellInterface::PresentOptions()
{
  wxLog* OldLog = wxLog::SetActiveTarget(new wxLogStderr);

  AspellKeyInfoEnumeration* elements = m_AspellWrapper.AspellConfigPossibleElements(m_AspellConfig, FALSE);

  wxString strMessage;
  const struct AspellKeyInfo* info;
  while ( (info = m_AspellWrapper.AspellKeyInfoEnumerationNext(elements)) != NULL )
  {
    // add to configuration list
    wxString strLine = wxString::Format("%s : %s : %s : %s: %s\r\n", info->name, info->def, info->desc, info->otherdata, m_AspellWrapper.AspellConfigRetrieve(m_AspellConfig, info->name));
    ::wxLogMessage(strLine);
    strMessage += strLine;
  }
  m_AspellWrapper.DeleteAspellKeyInfoEnumeration(elements); 
  ::wxMessageBox(strMessage);

  delete wxLog::SetActiveTarget(OldLog);
}

///////////// Options /////////////////
//
// Aspell Options:
//  ignore -> ignore case
//	sug-mode -> suggestion mode ("ultra", "fast", "normal", "bad-spellers")
//	filter -> adds or removes a filter
//	mode -> set filter mode ("none", "url", "email", "sgml", or "tex")
//	encoding -> encoding of input text
//	conf -> main configuration file
//	conf-dir -> path to main configuration file
//	data-dir -> path to language data files
//	local-data-dir -> alternative location of language data files
//	dict-dir -> path to main word list
//	lang -> language to use
//	personal -> personal word list file name
//	extra-dicts -> extra dictionaries to use
int AspellInterface::SetDefaultOptions()
{
	wxString strDataDir = wxString::Format(_T("%s%c%s"), ::wxGetCwd().c_str(), wxFileName::GetPathSeparator(), _T("data"));
	wxString strDictDir = wxString::Format(_T("%s%c%s"), ::wxGetCwd().c_str(), wxFileName::GetPathSeparator(), _T("dict"));
  wxString strLanguage = _T("en_US");

  SpellCheckEngineOption LanguageOption(_T("lang"), _T("Language Code"), strLanguage); // A list of possible values would be good here
  AddOptionToMap(LanguageOption);
  SpellCheckEngineOption DataDirOption(_T("data-dir"), _T("Language Data File Directory"), strDataDir, SpellCheckEngineOption::DIR);
  AddOptionToMap(DataDirOption);
  SpellCheckEngineOption DictDirOption(_T("dict-dir"), _T("Language Word List Directory"), strDictDir, SpellCheckEngineOption::DIR);
  AddOptionToMap(DictDirOption);
  
  SpellCheckEngineOption SuggestionModeOption(_T("sug-mode"), _T("Suggestion Mode"), wxString(_T("normal")));
  SuggestionModeOption.AddPossibleValue(wxString(_T("ultra")));
  SuggestionModeOption.AddPossibleValue(wxString(_T("fast")));
  SuggestionModeOption.AddPossibleValue(wxString(_T("normal")));
  SuggestionModeOption.AddPossibleValue(wxString(_T("bad-spellers")));
  AddOptionToMap(SuggestionModeOption);

  SpellCheckEngineOption IgnoreCaseOption(_T("ignore-case"), _T("Ignore Case"), false);
  AddOptionToMap(IgnoreCaseOption);

  
  // One thing to note is that the absence of certain files (en.dat and en_phonet.dat in the case of english)
  //  in the data-dir directory causes the aspell library to have an "abnormal termination message under windows)

  return true;
}
