#include "SpellCheckUserInterface.h"

wxSpellCheckUserInterface::wxSpellCheckUserInterface(wxSpellCheckEngineInterface* pSpellChecker /*= NULL*/)
{
  m_strMispelledWord = "";
	m_strReplaceWithText = "";
  m_strContext = "";
	m_pSpellCheckEngine = pSpellChecker;
  
  m_nLastAction = ACTION_INITIAL;
}

wxSpellCheckUserInterface::~wxSpellCheckUserInterface()
{
}
