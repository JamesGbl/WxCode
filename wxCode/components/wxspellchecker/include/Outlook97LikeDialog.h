#ifndef __OUTLOOK_SPELL_CHECK_DIALOG__
#define __OUTLOOK_SPELL_CHECK_DIALOG__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "SpellCheckDialogInterface.h"

// modelless SpellChecker dialog
class Outlook97LikeDialog : public wxSpellCheckDialogInterface
{
public:
    Outlook97LikeDialog(wxWindow *parent, wxSpellCheckEngineInterface* SpellChecker = NULL);
		~Outlook97LikeDialog();

		// Code handling the interface
		void OnCheckWord(wxCommandEvent& event);
		void OnReplaceWord(wxCommandEvent& event);
		void OnIgnoreWord(wxCommandEvent& event);
		void OnReplaceAll(wxCommandEvent& event);
		void OnIgnoreAll(wxCommandEvent& event);
		void OnAddWordToCustomDictionary(wxCommandEvent& event);
		void OnChangeSuggestionSelection(wxCommandEvent& event);
		void OnDblClkSuggestionSelection(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnClickOptions(wxCommandEvent& event);
    
    virtual void SetMispelledWord(const wxString& strMispelling);
 		void CreateDialog();

private:
    DECLARE_EVENT_TABLE()
};


#endif  // __OUTLOOK_SPELL_CHECK_DIALOG__
