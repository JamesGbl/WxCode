#include "SpellCheckCmdLineInterface.h"

SpellCheckCmdLineInterface::SpellCheckCmdLineInterface(wxSpellCheckEngineInterface* SpellChecker /*= NULL*/)
  : wxSpellCheckUserInterface(SpellChecker)
{
}

SpellCheckCmdLineInterface::~SpellCheckCmdLineInterface()
{
}

int SpellCheckCmdLineInterface::PresentSpellCheckUserInterface(const wxString& strMispelling)
{
  SetMispelledWord(strMispelling);
		
	m_nLastAction = ACTION_INITIAL;

  PrintMispelling();
  PrintSuggestions();
  GetFeedback();

	return m_nLastAction;
}

void SpellCheckCmdLineInterface::PrintMispelling()
{
  if (m_pSpellCheckEngine)
  {
    wxSpellCheckEngineInterface::MispellingContext Context = m_pSpellCheckEngine->GetCurrentMispellingContext();
    wxString strContext = Context.GetContext();
    // Append the closing marker first since the opening marker would shift where the closing marker would have to go
    strContext.insert(Context.GetOffset() + Context.GetLength(), "<-**");
    strContext.insert(Context.GetOffset(), "**->");
    printf("%s\n", strContext.c_str());
  }
}

void SpellCheckCmdLineInterface::PrintSuggestions()
{
  wxPrintf("Suggestions: \n");

  if (m_pSpellCheckEngine)
  {
    wxArrayString SuggestionArray = m_pSpellCheckEngine->GetSuggestions(m_strMispelledWord);
    if (SuggestionArray.GetCount() > 0)
    {
      // Add each suggestion to the list
			// The (nCtr < 5) clause is to make the list of suggestions manageable to the user
      for (unsigned int nCtr = 0; (nCtr < SuggestionArray.GetCount()) && (nCtr < 5); nCtr++)
        wxPrintf(" '%s'", SuggestionArray[nCtr].c_str());
    }
    else
    {
      wxPrintf(" (no suggestions)\n");
    }
  }
}

void SpellCheckCmdLineInterface::GetFeedback()
{
  wxChar strReplacement[256];
  wxPrintf(_T("\nReplacement? : \n"));
  if ( !wxFgets(strReplacement, WXSIZEOF(strReplacement), stdin) )
    m_nLastAction = ACTION_IGNORE; /* ignore the current misspelling */
  else
	{
  	strReplacement[wxStrlen(strReplacement) - 1] = '\0';
		if (wxStrlen(strReplacement) == 0)
		{
    	m_nLastAction = ACTION_IGNORE; /* ignore the current misspelling */
		}
		else
		{
	    m_nLastAction = ACTION_REPLACE;
			m_strReplaceWithText = strReplacement;
		}
	}
}

