#include "MySpellInterface.h"

#include <wx/filename.h>
#include <wx/tokenzr.h>
#include <wx/textfile.h>
#include <wx/config.h>

MySpellInterface::MySpellInterface(wxSpellCheckUserInterface* pDlg /* = NULL */)
{
  m_pSpellUserInterface = pDlg;

  if (m_pSpellUserInterface != NULL)
    m_pSpellUserInterface->SetSpellCheckEngine(this);

  m_pMySpell = NULL;
  m_bPersonalDictionaryModified = false;

  SetDefaultOptions();

  m_PersonalDictionary.LoadPersonalDictionary();
  
  InitializeSpellCheckEngine();
}

MySpellInterface::~MySpellInterface()
{
  SaveUserOptions();

  if (m_bPersonalDictionaryModified)
  {
    //if (wxYES == ::wxMessageBox("Would you like to save any of your changes to your personal dictionary?", "Save Changes", wxYES_NO | wxICON_QUESTION))
      m_PersonalDictionary.SavePersonalDictionary();
  }
  
  UninitializeSpellCheckEngine();

  if (m_pSpellUserInterface != NULL)
  {
    delete m_pSpellUserInterface;
    m_pSpellUserInterface = NULL;
  }
}

int MySpellInterface::InitializeSpellCheckEngine()
{
  UninitializeSpellCheckEngine();
 
  m_pMySpell= new MySpell(m_strAffixFile.c_str(), m_strDictionaryFile.c_str());
  
  return (m_pMySpell != NULL);
}

int MySpellInterface::UninitializeSpellCheckEngine()
{
  if (m_pMySpell)
  {
    delete m_pMySpell;
    m_pMySpell = NULL;
  }
  return TRUE;
}

int MySpellInterface::SetDefaultOptions()
{
  wxConfigBase* pConfig = wxConfigBase::Get();

  wxString strPath = _T("/wxSpellChecker-") + GetSpellCheckEngineName();
  pConfig->SetPath(strPath);

	m_strAffixFile = pConfig->Read(_T("affix-file"), wxString::Format(_T("%s%c%s"), ::wxGetCwd().c_str(), wxFileName::GetPathSeparator(), _T("en_US.aff")));
	m_strDictionaryFile = pConfig->Read(_T("dict-file"), wxString::Format(_T("%s%c%s"), ::wxGetCwd().c_str(), wxFileName::GetPathSeparator(), _T("en_US.dic")));
  
  SpellCheckEngineOption AffixFileOption(_T("affix-file"), _T("Affix File"), m_strAffixFile, SpellCheckEngineOption::FILE);
  AddOptionToMap(AffixFileOption);
  SpellCheckEngineOption DictFileOption(_T("dict-file"), _T("Dictionary File"), m_strDictionaryFile, SpellCheckEngineOption::FILE);
  AddOptionToMap(DictFileOption);

  return TRUE;
}

int MySpellInterface::SetOption(SpellCheckEngineOption& Option)
{
  // MySpell doesn't really have any options that I know of other than the affix and
  // dictionary files.  To change those, a new MySpell instance must be created though
  
  // First make sure that either the affix or dict file have changed
  if (Option.GetName() == _T("affix-file"))
  {
    if (m_strAffixFile == Option.GetValueAsString())
      return true;  // Even though the option didn't change, it isn't an error, so return true
    else
      m_strAffixFile = Option.GetValueAsString();
  }
  else if (Option.GetName() == _T("dict-file"))
  {
    if (m_strDictionaryFile == Option.GetValueAsString())
      return true;  // Even though the option didn't change, it isn't an error, so return true
    else
      m_strDictionaryFile = Option.GetValueAsString();
  }
  else
    return false; // We don't understand this option so return the error
  
  // We'll something changed so tear down the old spell check engine and create a new one
  UninitializeSpellCheckEngine();
  return InitializeSpellCheckEngine();

}

wxString MySpellInterface::CheckSpelling(wxString strText)
{
  if (m_pMySpell == NULL)
    return "";

  int nDiff = 0;

  strText += " ";

  wxString strDelimiters = _T(" \t\r\n.,?!@#$%^&*()-=_+[]{}\\|;:\"<>/~0123456789");
  wxStringTokenizer tkz(strText, strDelimiters);
  while ( tkz.HasMoreTokens() )
  {
    wxString token = tkz.GetNextToken();
    int TokenStart = tkz.GetPosition() - token.Length() - 1;
    TokenStart += nDiff;  // Take into account any changes to the size of the strText
    
    // process token here
    if  (!(m_pMySpell->spell(token)))
    {
      // If this word is in the always ignore list, then just move on
      if (m_AlwaysIgnoreList.Index(token) != wxNOT_FOUND)
        continue;

      // If this word is in the personal dictionary, then just move on
      if (m_PersonalDictionary.IsWordInDictionary(token))
        continue;
      
      bool bReplaceFromMap = FALSE;
      StringToStringMap::iterator WordFinder = m_AlwaysReplaceMap.find(token);
      if (WordFinder != m_AlwaysReplaceMap.end())
        bReplaceFromMap = TRUE;

      int nUserReturnValue = 0;

      if (!bReplaceFromMap)
      {
        // Define the context of the word
        DefineContext(strText, TokenStart, token.Length());

        // Print out the misspelling and get a replasment from the user 
        // Present the dialog so the user can tell us what to do with this word
        nUserReturnValue = GetUserCorrection(token);  //Show function will show the dialog and not return until the user makes a decision
      }

      if (nUserReturnValue == wxSpellCheckUserInterface::ACTION_CLOSE)
      {
        break;
      }
      else if ((nUserReturnValue == wxSpellCheckUserInterface::ACTION_REPLACE) || bReplaceFromMap)
      {
        wxString strReplacementText = (bReplaceFromMap) ? (*WordFinder).second : m_pSpellUserInterface->GetReplacementText();
        // Increase/Decreate the character difference so that the next loop is on track
        nDiff += strReplacementText.Length() - token.Length();
        // Replace the misspelled word with the replacement */
        strText.replace(TokenStart, token.Length(), strReplacementText);
      }
    }
  }

  strText = strText.Left(strText.Len() - 1);

  return strText;
}

wxArrayString MySpellInterface::GetSuggestions(const wxString& strMisspelledWord)
{
  wxArrayString wxReturnArray;
  wxReturnArray.Empty();

  if (m_pMySpell)
  {
    char **wlst;

    int ns = m_pMySpell->suggest(&wlst,strMisspelledWord.c_str());
    for (int i=0; i < ns; i++)
    {
      wxReturnArray.Add(wlst[i]);
      free(wlst[i]);
    }
    free(wlst);
  }
  
  return wxReturnArray;
}

bool MySpellInterface::IsWordInDictionary(const wxString& strWord)
{
  if (m_pMySpell == NULL)
    return false;

  return ((m_pMySpell->spell(strWord) == 1) || (m_PersonalDictionary.IsWordInDictionary(strWord)));
}

int MySpellInterface::AddWordToDictionary(const wxString& strWord)
{
  m_PersonalDictionary.AddWord(strWord);
  m_bPersonalDictionaryModified = true;
  return true;
}

int MySpellInterface::RemoveWordFromDictionary(const wxString& strWord)
{
  m_PersonalDictionary.RemoveWord(strWord);
  m_bPersonalDictionaryModified = true;
  return true;
}

wxArrayString MySpellInterface::GetWordListAsArray()
{
  return m_PersonalDictionary.GetWordListAsArray();
}

// Since MySpell doesn't have a concept of a personal dictionary, we can create a file
// to hold new words and if spell check fails then we check this map before asking the user
// It's not the best (as it won't support the affix feature of MySpell), but it'll work

