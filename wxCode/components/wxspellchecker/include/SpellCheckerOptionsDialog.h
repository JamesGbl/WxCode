#ifndef __SPELL_CHECKER_OPTIONS_DIALOG__
#define __SPELL_CHECKER_OPTIONS_DIALOG__

#include "SpellCheckEngineInterface.h"

class SpellCheckerOptionsDialog : public wxDialog
{
public:
  SpellCheckerOptionsDialog(wxWindow* pParent, const wxString& strCaption, OptionsMap* pOptionsMap);
  
  void OnOK(wxCommandEvent& event);
  void OnBrowseForDir(wxCommandEvent& event);
  void OnBrowseForFile(wxCommandEvent& event);
  OptionsMap* GetModifiedOptions() { return &m_ModifiedOptions; }

  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();

  private:
  void CreateControls();
  void PopulateOptionsSizer(wxSizer* pSizer);
  
  OptionsMap m_ModifiedOptions;

  DECLARE_EVENT_TABLE()
};

#endif // __SPELL_CHECKER_OPTIONS_DIALOG__
