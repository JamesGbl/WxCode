#include "SpellCheckEngineInterface.h"
#include "SpellCheckUserInterface.h"

wxSpellCheckEngineInterface::wxSpellCheckEngineInterface()
{
	m_AlwaysReplaceMap.clear();
	m_AlwaysIgnoreList.IsEmpty();
	m_pSpellUserInterface = NULL;
	m_bPersonalDictionaryModified = FALSE;
}

wxSpellCheckEngineInterface::~wxSpellCheckEngineInterface()
{
	if (m_pSpellUserInterface != NULL)
  {
		delete m_pSpellUserInterface;
    m_pSpellUserInterface = NULL;
  }
}

void wxSpellCheckEngineInterface::SetSpellCheckUserInterface(wxSpellCheckUserInterface* pDlg)
{
  // delete the old user interface
  if (m_pSpellUserInterface != NULL)
    delete m_pSpellUserInterface;
  
  // set the new user interface and tell it that this is it's spell check engine
	m_pSpellUserInterface = pDlg;
	if (m_pSpellUserInterface != NULL)
		pDlg->SetSpellCheckEngine(this);
}

void wxSpellCheckEngineInterface::PresentOptions()
{
	// This function should present the options to the user and set new values
	//	based on the user input
	// An example would be displaying a dialog with all the user options and
	//	changing the options if the user clicked OK
}

int wxSpellCheckEngineInterface::GetUserCorrection(const wxString& strMisspelling)  //Show function will show the dialog and not return until the user makes a decision
{
  // Populate the listbox with suggestions
  // At this point, we have to wait for user input.  This is where having the dialog and the spell checking as different classes would be handy
	int nLastAction = m_pSpellUserInterface->PresentSpellCheckUserInterface(strMisspelling);

	if (nLastAction == wxSpellCheckUserInterface::ACTION_REPLACE_ALWAYS)
	{
		m_AlwaysReplaceMap[m_pSpellUserInterface->GetMispelledWord()] = m_pSpellUserInterface->GetReplacementText();
	}
	else if (nLastAction == wxSpellCheckUserInterface::ACTION_IGNORE_ALWAYS)
	{
		m_AlwaysIgnoreList.Add(m_pSpellUserInterface->GetMispelledWord());
	}
	else if (nLastAction == wxSpellCheckUserInterface::ACTION_CLOSE)
	{
		return wxSpellCheckUserInterface::ACTION_CLOSE;
	}
	
  return ((nLastAction == wxSpellCheckUserInterface::ACTION_REPLACE) || 
					(nLastAction == wxSpellCheckUserInterface::ACTION_REPLACE_ALWAYS)) ? wxSpellCheckUserInterface::ACTION_REPLACE : wxSpellCheckUserInterface::ACTION_IGNORE;
}


void wxSpellCheckEngineInterface::DefineContext(const wxString& strText, long nOffset, long nLength)
{
  // This is kind of a kludge, but to determine the context of this word,
  //  grab the 50 characters before the nOffset and the 50 characters + nLength after nOffset
  // Then, do a Find from the front and the back of this 100 + nLength character string looking
  //  for a space character.  Trim off this space character and all characters preceding/after 
  //  it based on if it's before or after the mispelled word.
  // Also, if there are NOT 50 characters before the nOffset don't bother trimming off at the space.
  // Likewise for less than 50 characters following the mispelled word.
  // Special allowances should be made to not have newline characters (\r or \n) in the context.

  wxString strLocalText = strText;
  strLocalText.Replace("\r", " ");
  strLocalText.Replace("\n", " ");
	
  long nStartPosition = 0;
  bool bTrimFront = FALSE;
  long nOffsetTrimmed = nOffset;
  if (nOffset > 50)
  {
    nStartPosition = nOffset - 50;
    nOffsetTrimmed = 50;
    bTrimFront = TRUE;
  }

  bool bTrimEnd = FALSE;
  long nEndPosition = wxSTRING_MAXLEN;
  if ((unsigned)(nStartPosition + nLength + 50) < strLocalText.Length())
  {
    nEndPosition = (nLength + 50);
    bTrimEnd = TRUE;
  }

	nEndPosition += nOffset - nStartPosition;  // Without this, we're only grabbing the number of characters for starting at the mispelled word
	wxString strContext;
	if ((unsigned)nEndPosition == wxSTRING_MAXLEN)
	  strContext = strLocalText.Mid(nStartPosition);
	else
	  strContext = strLocalText.Mid(nStartPosition, nEndPosition);
  // Remove characters before the first space character
  if (bTrimFront)
  {
    if (strContext.Contains(" "))
    {
      nOffsetTrimmed -= strContext.Find(' ') + 1; // Figure out how much of the offset was trimmed off by remove the characters before the first space
      strContext = strContext.AfterFirst(' ');
    }
  }

  // Remove characters before the first space character
  if (bTrimEnd)
  {
    if (strContext.Contains(" "))
      strContext = strContext.BeforeLast(' ');
  }

  m_Context.SetContext(strContext);
  m_Context.SetOffset(nOffsetTrimmed);
  m_Context.SetLength(nLength);
}

bool wxSpellCheckEngineInterface::AddOptionToMap(SpellCheckEngineOption& option)
{
  // Return a boolean indicating whether or now the spell check engine options actually changed
  bool bOptionsChanged = false;
  wxString strOptionName = option.GetName();
  
  if (!strOptionName.IsEmpty())
  {
    OptionsMap::iterator it = m_Options.find(strOptionName);
    // If either the option isn't in the map yet or the value is different, then update the options
    if ((it == m_Options.end()) || (it->second.GetValueAsString() != option.GetValueAsString()))
    {
      m_Options[strOptionName] = option;
      bOptionsChanged = true;
    }
  }
  return bOptionsChanged;
}

void wxSpellCheckEngineInterface::ApplyOptions()
{
  OptionsMap::iterator it;
  for (it = m_Options.begin(); it != m_Options.end(); it++)
  {
    SetOption(it->second);
  }
}
