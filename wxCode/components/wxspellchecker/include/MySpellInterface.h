#ifndef __MYSPELL_CHECK_INTERFACE__
#define __MYSPELL_CHECK_INTERFACE__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/string.h"

// spell checker/thingie
#include "wx/process.h"
#include "wx/txtstrm.h"
#include "wx/file.h"

#include "SpellCheckUserInterface.h"

#include "MySpell/myspell.hxx"

// Get rid of the warning about identifiers being truncated in the debugger.  Using the STL collections
// will produce this everywhere.  Must disable at beginning of stdafx.h because it doesn't work if
// placed elsewhere.
#ifdef __VISUALC__
  #pragma warning(disable:4786)
#endif

class MySpellInterface : public wxSpellCheckEngineInterface
{
public:
  MySpellInterface(wxSpellCheckUserInterface* pDlg = NULL);
  ~MySpellInterface();

	// Spell Checker functions
	int InitializeSpellCheckEngine();
	int UninitializeSpellCheckEngine();
	int SetDefaultOptions();
  int SetOption(SpellCheckEngineOption& Option);
	virtual wxString CheckSpelling(wxString strText);
  wxArrayString GetSuggestions(const wxString& strMisspelledWord);

	virtual bool IsWordInDictionary(const wxString& strWord);
  virtual int AddWordToDictionary(const wxString& strWord);
  virtual int RemoveWordFromDictionary(const wxString& strWord);
  virtual wxArrayString GetWordListAsArray();

private:  
	MySpell* m_pMySpell;
  
  wxString m_strAffixFile;
  wxString m_strDictionaryFile;
  wxArrayString m_PersonalDictionary;
  bool LoadPersonalDictionary();
  bool SavePersonalDictionary();
  
  static const wxString Personal_Dictionary_Filename;
};

#endif  // __MYSPELL_CHECK_INTERFACE__
