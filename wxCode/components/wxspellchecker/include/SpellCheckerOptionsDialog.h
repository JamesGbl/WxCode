#ifndef __SPELL_CHECKER_OPTIONS_DIALOG__
#define __SPELL_CHECKER_OPTIONS_DIALOG__

#include "SpellCheckEngineInterface.h"

class SpellCheckerOptionsDialog : public wxDialog
{
public:
  SpellCheckerOptionsDialog(wxWindow* pParent, const wxString& strCaption, OptionsMap* pOptionsMap);
  
  void OnOK(wxCommandEvent& event);
private:
  void CreateControls();
  void PopulateOptionsSizer(wxSizer* pSizer);
  
  OptionsMap* m_pOptionsMap;
};

#endif // __SPELL_CHECKER_OPTIONS_DIALOG__
