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
}

MySpellInterface::~MySpellInterface()
{
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

  wxString strAffixFile = GetAffixFileName();
  wxString strDictionaryFile = GetDictionaryFileName();
  
  if ((strAffixFile != wxEmptyString) && (strDictionaryFile != wxEmptyString))
    m_pMySpell = new MySpell(strAffixFile.c_str(), strDictionaryFile.c_str());
  
  m_bEngineInitialized = (m_pMySpell != NULL);
  return m_bEngineInitialized;
}

int MySpellInterface::UninitializeSpellCheckEngine()
{
  wxDELETE(m_pMySpell);
  m_bEngineInitialized = false;
  return true;
}

int MySpellInterface::SetOption(SpellCheckEngineOption& Option)
{
  // MySpell doesn't really have any options that I know of other than the affix and
  // dictionary files.  To change those, a new MySpell instance must be created though
  
  // First make sure that either the affix or dict file have changed
  if (Option.GetName() == _T("dictionary-path"))
  {
    m_strDictionaryPath = Option.GetValueAsString();
    PopulateDictionaryMap(&m_DictionaryLookupMap, m_strDictionaryPath);

    //SpellCheckEngineOption LanguageOption(_T("language"), _T("Language"), GetSelectedLanguage());
  
    /*
    StringToStringMap::iterator start = m_DictionaryLookupMap.begin();
    StringToStringMap::iterator stop = m_DictionaryLookupMap.end();
    while (start != stop)
    {
      LanguageOption.AddPossibleValue((*start).first);
      start++;
    }
    */
    //AddOptionToMap(LanguageOption);
    
    //return true;  // Even though the option didn't change, it isn't an error, so return true
  }
  else if (Option.GetName() == _T("language"))
  {
    //return true;  // Even though the option didn't change, it isn't an error, so return true
  }
  else
    return false; // We don't understand this option so return the error
  
  // We'll something changed so tear down the old spell check engine and create a new one
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
      
      bool bReplaceFromMap = false;
      StringToStringMap::iterator WordFinder = m_AlwaysReplaceMap.find(token);
      if (WordFinder != m_AlwaysReplaceMap.end())
        bReplaceFromMap = true;

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

void MySpellInterface::PopulateDictionaryMap(StringToStringMap* pLookupMap, const wxString& strDictionaryPath)
{
  if (pLookupMap == NULL)
    pLookupMap = &m_DictionaryLookupMap;
  
  pLookupMap->clear();

  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Afrikaans (South Africa)"), _("af_ZA"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Bulgarian (Bulgaria)"), _("bg_BG"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Catalan (Spain)"), _("ca_ES"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Czech (Czech Republic)"), _("cs_CZ"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Welsh (Wales)"), _("cy_GB"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Danish (Denmark)"), _("da_DK"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("German (Austria)"), _("de_AT"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("German (Switzerland)"), _("de_CH"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("German (Germany- orig dict)"), _("de_DE"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("German (Germany-old & neu ortho.)"), _("de_DE_comb"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("German (Germany-neu ortho.)"), _("de_DE_neu"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Greek (Greece)"), _("el_GR"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("English (Australia)"), _("en_AU"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("English (Canada)"), _("en_CA"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("English (United Kingdom)"), _("en_GB"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("English (New Zealand)"), _("en_NZ"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("English (United States)"), _("en_US"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Esperanto (anywhere)"), _("eo_l3"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Spanish (Spain-etal)"), _("es_ES"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Spanish (Mexico)"), _("es_MX"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Faroese (Faroe Islands)"), _("fo_FO"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("French (France)"), _("fr_FR"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Irish (Ireland)"), _("ga_IE"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Scottish Gaelic (Scotland)"), _("gd_GB"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Galician (Spain)"), _("gl_ES"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Hebrew (Israel)"), _("he_IL"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Croatian (Croatia)"), _("hr_HR"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Hungarian (Hungary)"), _("hu_HU"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Interlingua (x-register)"), _("ia"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Indonesian (Indonesia)"), _("id_ID"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Italian (Italy)"), _("it_IT"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Kurdish (Turkey)"), _("ku_TR"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Latin (x-register)"), _("la"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Lithuanian (Lithuania)"), _("lt_LT"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Latvian (Latvia)"), _("lv_LV"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Malagasy (Madagascar)"), _("mg_MG"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Maori (New Zealand)"), _("mi_NZ"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Malay (Malaysia)"), _("ms_MY"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Norwegian Bokmaal (Norway)"), _("nb_NO"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Dutch (Netherlands)"), _("nl_NL"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Norwegian Nynorsk (Norway)"), _("nn_NO"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Chichewa (Malawi)"), _("ny_MW"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Polish (Poland)"), _("pl_PL"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Portuguese (Brazil)"), _("pt_BR"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Portuguese (Portugal)"), _("pt_PT"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Romanian (Romania)"), _("ro_RO"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Russian (Russia)"), _("ru_RU"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Russian ye (Russia)"), _("ru_RU_ie"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Russian yo (Russia)"), _("ru_RU_yo"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Kiswahili (Africa)"), _("rw_RW"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Slovak (Slovakia)"), _("sk_SK"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Slovenian (Slovenia)"), _("sl_SI"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Swedish (Sweden)"), _("sv_SE"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Kiswahili (Africa)"), _("sw_KE"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Tetum (Indonesia)"), _("tet_ID"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Tagalog (Philippines)"), _("tl_PH"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Setswana (Africa)"), _("tn_ZA"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Ukrainian (Ukraine)"), _("uk_UA"));
  AddDictionaryElement(pLookupMap, strDictionaryPath, _("Zulu (Africa)"), _("zu_ZA"));

  // Add the custom MySpell dictionary entries to the map
  StringToStringMap::iterator start = m_CustomMySpellDictionaryMap.begin();
  StringToStringMap::iterator stop = m_CustomMySpellDictionaryMap.end();
  while (start != stop)
  {
    AddDictionaryElement(pLookupMap, strDictionaryPath, (*start).first, (*start).second);
    start++;
  }
}

void MySpellInterface::UpdatePossibleValues(SpellCheckEngineOption& OptionDependency, SpellCheckEngineOption& OptionToUpdate)
{
  if ((OptionDependency.GetName().IsSameAs(_T("dictionary-path"))) && (OptionToUpdate.GetName().IsSameAs(_T("language"))))
  {
    StringToStringMap tempLookupMap;
    wxString strDictionaryPath = OptionDependency.GetValueAsString();
    PopulateDictionaryMap(&tempLookupMap, strDictionaryPath);

    StringToStringMap::iterator start = tempLookupMap.begin();
    StringToStringMap::iterator stop = tempLookupMap.end();
    while (start != stop)
    {
      OptionToUpdate.AddPossibleValue((*start).first);
      start++;
    }
  }
  else
  {
    ::wxMessageBox(wxString::Format(_T("Unsure how to update the possible values for %s based on the value of %s"), OptionDependency.GetText().c_str(), OptionToUpdate.GetText().c_str()));
  }
}

void MySpellInterface::AddDictionaryElement(StringToStringMap* pLookupMap, const wxString& strDictionaryPath, const wxString& strDictionaryName, const wxString& strDictionaryFileRoot)
{
  wxFileName strAffixFileName(strDictionaryPath + wxFILE_SEP_PATH + strDictionaryFileRoot + _(".aff"));
  wxFileName strDictionaryFileName(strDictionaryPath + wxFILE_SEP_PATH + strDictionaryFileRoot + _(".dic"));
  if (strAffixFileName.FileExists() && strDictionaryFileName.FileExists())
  {
    (*pLookupMap)[strDictionaryName] = strDictionaryFileRoot;
  }
}

wxString MySpellInterface::GetSelectedLanguage()
{
  OptionsMap::iterator it = m_Options.find(_("language"));
  if (it != m_Options.end())
  {
    return it->second.GetValueAsString();
  }
  else
  {
    return wxEmptyString;
  }
}

wxString MySpellInterface::GetAffixFileName()
{
  wxString strLanguage = GetSelectedLanguage();
  if (strLanguage != wxEmptyString)
  {
    return GetAffixFileName(strLanguage);
  }
  else
  {
    return wxEmptyString;
  }
}

wxString MySpellInterface::GetAffixFileName(const wxString& strDictionaryName)
{
  StringToStringMap::iterator finder = m_DictionaryLookupMap.find(strDictionaryName);
  if (finder != m_DictionaryLookupMap.end())
  {
    return (m_strDictionaryPath + wxFILE_SEP_PATH + (*finder).second + _(".aff"));
  }
  else
  {
    return wxEmptyString;
  }
}

wxString MySpellInterface::GetDictionaryFileName()
{
  wxString strLanguage = GetSelectedLanguage();
  if (strLanguage != wxEmptyString)
  {
    return GetDictionaryFileName(strLanguage);
  }
  else
  {
    return wxEmptyString;
  }
}

wxString MySpellInterface::GetDictionaryFileName(const wxString& strDictionaryName)
{
  StringToStringMap::iterator finder = m_DictionaryLookupMap.find(strDictionaryName);
  if (finder != m_DictionaryLookupMap.end())
  {
    return (m_strDictionaryPath + wxFILE_SEP_PATH + (*finder).second + _(".dic"));
  }
  else
  {
    return wxEmptyString;
  }
}

void MySpellInterface::AddCustomMySpellDictionary(const wxString& strDictionaryName, const wxString& strDictionaryFileRoot)
{
  m_CustomMySpellDictionaryMap[strDictionaryName] = strDictionaryFileRoot;
}

void MySpellInterface::OpenPersonalDictionary(const wxString& strPersonalDictionaryFile)
{
  m_PersonalDictionary.SetDictionaryFileName(strPersonalDictionaryFile);
  m_PersonalDictionary.LoadPersonalDictionary();
}

///////////// Options /////////////////
// "dictionary-path" - location of dictionary files
// "language" - selected language

