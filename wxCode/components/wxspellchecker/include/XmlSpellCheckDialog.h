#ifndef __XML_SPELL_CHECK_DIALOG__
#define __XML_SPELL_CHECK_DIALOG__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "SpellCheckDialogInterface.h"

// modelless SpellChecker dialog
class XmlSpellCheckDialog : public wxDialog, public wxSpellCheckUserInterface
{
public:
    XmlSpellCheckDialog(wxWindow *parent, wxString strResourceFile, wxString strDialogResource, wxString strWordListResource, wxSpellCheckEngineInterface* SpellChecker = NULL);
		~XmlSpellCheckDialog();

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

    virtual void SetMispelledWord(const wxString& strMispelling);

    virtual int PresentSpellCheckUserInterface(const wxString& strMispelling);
 		void CreateDialog(wxWindow* pParent);
    void OnOptions(wxCommandEvent& event);

private:
    DECLARE_EVENT_TABLE()

    wxString m_strResourceFile;
    wxString m_strDialogResource;
    wxString m_strWordListResource;
};

class XmlPersonalDictionaryDialog : public wxDialog
{
public:
  XmlPersonalDictionaryDialog(wxWindow* parent, wxString strResourceFile, wxString strResource, wxSpellCheckEngineInterface* pEngine);
  ~XmlPersonalDictionaryDialog();

  void CreateDialog(wxWindow* pParent);
  void PopulatePersonalWordListBox();
  void AddWordToPersonalDictionary(wxCommandEvent& event);
  void ReplaceInPersonalDictionary(wxCommandEvent& event);
  void RemoveFromPersonalDictionary(wxCommandEvent& event);
  void OnClose(wxCommandEvent& event);

protected:
  wxSpellCheckEngineInterface* m_pSpellCheckEngine;
  wxString m_strResourceFile;
  wxString m_strDialogResource;

private:
    DECLARE_EVENT_TABLE()
};

#endif  // __XML_SPELL_CHECK_DIALOG__

