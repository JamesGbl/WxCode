#include "SpellCheckDialogInterface.h"

wxSpellCheckDialogInterface::wxSpellCheckDialogInterface(wxWindow *parent, wxSpellCheckEngineInterface* SpellChecker /*= NULL*/)
  : wxDialog(parent, -1, wxString("Spell-Checker"),wxDefaultPosition, wxSize(330,245), wxDEFAULT_DIALOG_STYLE),
  wxSpellCheckUserInterface(SpellChecker)
{
}

wxSpellCheckDialogInterface::~wxSpellCheckDialogInterface()
{
}

int wxSpellCheckDialogInterface::PresentSpellCheckUserInterface(const wxString& strMispelling)
{
  // If the dialog is being presented, then default the previous action flag first
  SetMispelledWord(strMispelling);
		
	m_nLastAction = ACTION_INITIAL;

  ShowModal();

	return m_nLastAction;
}
