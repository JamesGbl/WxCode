#ifndef __SENTRY_LIKE_SPELL_CHECK_DIALOG__
#define __SENTRY_LIKE_SPELL_CHECK_DIALOG__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "SpellCheckDialogInterface.h"

// modelless SpellChecker dialog
class SentryLikeDialog : public wxSpellCheckDialogInterface
{
public:
    SentryLikeDialog(wxWindow *parent, wxSpellCheckEngineInterface* SpellChecker = NULL);

		// Code handling the interface
		void OnRecheckPage(wxCommandEvent& event);
		void OnCheckWord(wxCommandEvent& event);
		void OnReplaceWord(wxCommandEvent& event);
		void OnIgnoreWord(wxCommandEvent& event);
		void OnReplaceAll(wxCommandEvent& event);
		void OnIgnoreAll(wxCommandEvent& event);
		void OnAddWordToCustomDictionary(wxCommandEvent& event);
		void OnEditCustomDictionary(wxCommandEvent& event);
		void OnChangeLanguage(wxCommandEvent& event);
		void OnChangeSuggestionSelection(wxCommandEvent& event);
		void OnDblClkSuggestionSelection(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnClickOptions(wxCommandEvent& event);
    
    virtual void SetMispelledWord(const wxString& strMispelling);
 		void CreateDialog();

private:
    DECLARE_EVENT_TABLE()
};

class MyPersonalSentryLikeDictionaryDialog : public wxDialog
{
public:
  MyPersonalSentryLikeDictionaryDialog(wxWindow* parent, wxSpellCheckEngineInterface* pEngine);
  ~MyPersonalSentryLikeDictionaryDialog();

  void CreateDialog();
  void PopulatePersonalWordListBox();
  void AddWordToPersonalDictionary(wxCommandEvent& event);
  void ReplaceInPersonalDictionary(wxCommandEvent& event);
  void RemoveFromPersonalDictionary(wxCommandEvent& event);
  void OnClose(wxCommandEvent& event);

protected:
  wxSpellCheckEngineInterface* m_pSpellCheckEngine;

private:
    DECLARE_EVENT_TABLE()
};

#endif  // __SENTRY_LIKE_SPELL_CHECK_DIALOG__
