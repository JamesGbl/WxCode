#ifndef __SPELL_CHECK_USER_INTERFACE__
#define __SPELL_CHECK_USER_INTERFACE__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/string.h"

#include "SpellCheckEngineInterface.h"

class wxSpellCheckUserInterface
{
public:
  wxSpellCheckUserInterface(wxSpellCheckEngineInterface* pSpellChecker = NULL);
	virtual ~wxSpellCheckUserInterface();

  enum  {
        ACTION_INITIAL = 0,
		    ACTION_REPLACE,
    		ACTION_IGNORE,
		    ACTION_REPLACE_ALWAYS,
    		ACTION_IGNORE_ALWAYS,
		    ACTION_CLOSE 
    		};

protected:
  wxSpellCheckEngineInterface* m_pSpellCheckEngine;

  wxString m_strMispelledWord;
	wxString m_strReplaceWithText;
  wxString m_strContext;

  int m_nLastAction;

public:
  virtual void SetMispelledWord(const wxString& strMispelling) { m_strMispelledWord = strMispelling; }
	wxString GetMispelledWord() { return m_strMispelledWord; }
	wxString GetReplacementText() { return m_strReplaceWithText; }
	wxString GetContext() { return m_strContext; }
	int GetLastAction() { return m_nLastAction; }
	void SetSpellCheckEngine(wxSpellCheckEngineInterface* pEngine) { m_pSpellCheckEngine = pEngine; }
	virtual int PresentSpellCheckUserInterface(const wxString& strMispelling) = 0;
};

#endif  // __SPELL_CHECK_USER_INTERFACE__
