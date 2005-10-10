#include "AspellInterface.h"
#include "SpellCheckUserInterface.h"

#include <wx/config.h>

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
  m_bPersonalDictionaryModified = false;

  m_pSpellUserInterface = pDlg;
  if (m_pSpellUserInterface != NULL)
    m_pSpellUserInterface->SetSpellCheckEngine(this);

  SetDefaultOptions();
}

AspellInterface::~AspellInterface()
{
  wxASSERT_MSG(m_AspellWrapper.IsLoaded(), _T("Aspell library wrapper isn't loaded and Aspell interface can't be properly cleaned up"));
  
  SaveUserOptions();
  
  if (m_AspellWrapper.IsLoaded())  // If the Aspell library wrapper isn't loaded then we can't properly clean up
  {
    if (m_bPersonalDictionaryModified)
    {
      //if (wxYES == ::wxMessageBox("Would you like to save any of your changes to your personal dictionary?", "Save Changes", wxYES_NO | wxICON_QUESTION))
        //m_AspellWrapper.AspellSpellerSaveAllWordLists(m_AspellSpeller);
      m_PersonalDictionary.SavePersonalDictionary();
    }
  
    if (m_AspellChecker != NULL)
    {
      m_AspellWrapper.DeleteAspellDocumentChecker(m_AspellChecker);
      m_AspellChecker = NULL;
    }
  
    if (m_AspellConfig != NULL)
    {
      m_AspellWrapper.DeleteAspellConfig(m_AspellConfig);
      m_AspellConfig = NULL;
    }
  
    if (m_AspellSpeller != NULL)
    {
      m_AspellWrapper.DeleteAspellSpeller(m_AspellSpeller);
      m_AspellSpeller = NULL;
    }
  
    if (m_pSpellUserInterface != NULL)
    {
      delete m_pSpellUserInterface;
      m_pSpellUserInterface = NULL;
    }
  }
  
  m_AspellWrapper.Unload();
}

int AspellInterface::InitializeSpellCheckEngine()
{
  m_PersonalDictionary.LoadPersonalDictionary();
  
  if (!m_AspellWrapper.LoadFunctions())
    return FALSE;
  
	if (m_AspellConfig == NULL)
		m_AspellConfig = m_AspellWrapper.NewAspellConfig();
	
	if (m_AspellConfig == NULL)
		return FALSE;

  ApplyOptions();
  
  AspellCanHaveError* ret = m_AspellWrapper.NewAspellSpeller(m_AspellConfig);
  if (m_AspellWrapper.AspellError(ret) != 0)
	{
    ::wxMessageBox(wxString::Format("Error: %s\n",m_AspellWrapper.AspellErrorMessage(ret)));
    m_AspellWrapper.DeleteAspellCanHaveError(ret);
    return FALSE;
  }
  m_AspellSpeller = m_AspellWrapper.ToAspellSpeller(ret);
  
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
	
  wxString strValue = Option.GetValueAsString();
  // wxVariant returns booleans as "0" or "1", and Aspell expects "true" or "false"
  if (Option.GetOptionType() == SpellCheckEngineOption::BOOLEAN)
    strValue = (strValue == _T("0")) ? _T("false") : _T("true");
    
	m_AspellWrapper.AspellConfigReplace(m_AspellConfig, Option.GetName(), strValue);

	return TRUE;
}

void AspellInterface::UpdatePossibleValues(SpellCheckEngineOption& OptionDependency, SpellCheckEngineOption& OptionToUpdate)
{
  if (m_AspellWrapper.IsLoaded())  // If the Aspell library wrapper isn't loaded then we can't properly clean up
  {
    if ((OptionDependency.GetName().IsSameAs(_T("dict-dir"))) && (OptionToUpdate.GetName().IsSameAs(_T("lang"))))
    {
      AspellConfig* config = m_AspellWrapper.NewAspellConfig();
      AspellDictInfoList* dlist;
      AspellDictInfoEnumeration* dels;
      const AspellDictInfo* entry;
    
      m_AspellWrapper.AspellConfigReplace(config, OptionDependency.GetName(), OptionDependency.GetValueAsString());
      
      // The returned pointer should _not_ need to be deleted
      dlist = m_AspellWrapper.GetAspellDictInfoList(config);
    
      // The Config is no longer needed
      m_AspellWrapper.DeleteAspellConfig(config);
    
      dels = m_AspellWrapper.AspellDictInfoListElements(dlist);
    
      OptionToUpdate.GetPossibleValuesArray()->Clear();
      while ( (entry = m_AspellWrapper.AspellDictInfoEnumerationNext(dels)) != 0) 
      {
        OptionToUpdate.AddPossibleValue(wxString(entry->name));
      }
    
      m_AspellWrapper.DeleteAspellDictInfoEnumeration(dels);
    }
    else
    {
      ::wxMessageBox(wxString::Format(_T("Unsure how to update the possible values for %s based on the value of %s"), OptionDependency.GetText().c_str(), OptionToUpdate.GetText().c_str()));
    }
  }
}

bool AspellInterface::IsWordInDictionary(const wxString& strWord)
{
  if (m_AspellSpeller == NULL)
    return false;

  return ((m_AspellWrapper.AspellSpellerCheck(m_AspellSpeller, strWord, strWord.Length()) == 1) || (m_PersonalDictionary.IsWordInDictionary(strWord)));
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

    // Use the generic PersonalDictionary until Aspell adds API to remove words from the personal dictionary
    if (m_PersonalDictionary.IsWordInDictionary(strBadWord))
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
			m_bPersonalDictionaryModified = true;	// Storing this information modifies the dictionary
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
  /*
  * AspellSpellerAddToPersonal returns
  *  0 if the word was added to the dictionary
  *  1 if the word was already in the dictionary
  *  -1 if there was an error adding the word to the dictionary
  */
/*
	int nReturn = m_AspellWrapper.AspellSpellerAddToPersonal(m_AspellSpeller, strWord, strWord.Length());

	m_bPersonalDictionaryModified = true;
  return (nReturn != -1);
*/
  
  // Use the generic PersonalDictionary until Aspell adds API to remove words from the personal dictionary
  m_PersonalDictionary.AddWord(strWord);
  m_bPersonalDictionaryModified = true;
  return true;
}

int AspellInterface::RemoveWordFromDictionary(const wxString& strWord)
{
  // Use the generic PersonalDictionary until Aspell adds API to remove words from the personal dictionary
  m_PersonalDictionary.RemoveWord(strWord);
  m_bPersonalDictionaryModified = true;
  return true;
}

wxArrayString AspellInterface::GetWordListAsArray()
{
/*  
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
*/  
  
  // Use the generic PersonalDictionary until Aspell adds API to remove words from the personal dictionary
  return m_PersonalDictionary.GetWordListAsArray();
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
  wxConfigBase* pConfig = wxConfigBase::Get();

  wxString strPath = _T("/wxSpellChecker-") + GetSpellCheckEngineName();
  pConfig->SetPath(strPath);
  
	wxString strDataDir = pConfig->Read(_T("data-dir"), wxString::Format(_T("%s%c%s"), ::wxGetCwd().c_str(), wxFileName::GetPathSeparator(), _T("data")));
	wxString strDictDir = pConfig->Read(_T("dict-dir"), wxString::Format(_T("%s%c%s"), ::wxGetCwd().c_str(), wxFileName::GetPathSeparator(), _T("dict")));
  wxString strLanguage = pConfig->Read(_T("lang"), _T("en"));

  // Double check that the directory exists.  Otherwise we might have bad settings from a previous installation
  if (wxFileName::DirExists(strDataDir) == false)
    strDataDir = wxString::Format(_T("%s%c%s"), ::wxGetCwd().c_str(), wxFileName::GetPathSeparator(), _T("data"));
  if (wxFileName::DirExists(strDictDir) == false)
    strDictDir = wxString::Format(_T("%s%c%s"), ::wxGetCwd().c_str(), wxFileName::GetPathSeparator(), _T("dict"));

  SpellCheckEngineOption LanguageOption(_T("lang"), _T("Language Code"), strLanguage); // A list of possible values would be good here
  LanguageOption.SetDependency(_T("dict-dir"));
  AddOptionToMap(LanguageOption);
  SpellCheckEngineOption DataDirOption(_T("data-dir"), _T("Language Data File Directory"), strDataDir, SpellCheckEngineOption::DIR);
  AddOptionToMap(DataDirOption);
  SpellCheckEngineOption DictDirOption(_T("dict-dir"), _T("Language Word List Directory"), strDictDir, SpellCheckEngineOption::DIR);
  AddOptionToMap(DictDirOption);
  
  SpellCheckEngineOption SuggestionModeOption(_T("sug-mode"), _T("Suggestion Mode"), pConfig->Read(_T("sug-mode"), _T("normal")));
  SuggestionModeOption.AddPossibleValue(wxString(_T("ultra")));
  SuggestionModeOption.AddPossibleValue(wxString(_T("fast")));
  SuggestionModeOption.AddPossibleValue(wxString(_T("normal")));
  SuggestionModeOption.AddPossibleValue(wxString(_T("bad-spellers")));
  AddOptionToMap(SuggestionModeOption);

  /* - Not compatible with Aspell 0.60 library
  SpellCheckEngineOption FilterModeOption(_T("mode"), _T("Filter Mode"), pConfig->Read(_T("mode"), _T("none")));
  FilterModeOption.AddPossibleValue(wxString(_T("none")));
  FilterModeOption.AddPossibleValue(wxString(_T("url")));
  FilterModeOption.AddPossibleValue(wxString(_T("email")));
  FilterModeOption.AddPossibleValue(wxString(_T("sgml")));
  FilterModeOption.AddPossibleValue(wxString(_T("tex")));
  AddOptionToMap(FilterModeOption);
  */

  bool bIgnoreCase = false;
  pConfig->Read(_T("ignore-care"), &bIgnoreCase, false);
  SpellCheckEngineOption IgnoreCaseOption(_T("ignore-case"), _T("Ignore Case"), bIgnoreCase);
  AddOptionToMap(IgnoreCaseOption);

  
  // One thing to note is that the absence of certain files (en.dat and en_phonet.dat in the case of english)
  //  in the data-dir directory causes the aspell library to have an "abnormal termination message under windows)

  return true;
}
