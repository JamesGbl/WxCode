/////////////////////////////////////////////////////////////////////////////
// Name:        keybinder.cpp
// Purpose:     wxKeyBind, wxCmd, wxKeyBinder, wxKeyConfigPanel
// Author:      Aleksandras Gluchovas
// Modified by: Francesco Montorsi
// Created:     2000/02/10
// RCS-ID:      $Id: keybinder.cpp,v 2.0
// Copyright:   (c) Aleksandras Gluchovas
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#ifdef __GNUG__
#pragma implementation "keybinder.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include "wx/wx.h"
	#include "wx/log.h"
#endif

// includes
#include "wx/statline.h"
#include "wx/keybinder.h"
#include "wx/menuutils.h"



// event table for wxKeyBinder
IMPLEMENT_CLASS(wxKeyBinder, wxEvtHandler)
BEGIN_EVENT_TABLE(wxKeyBinder, wxEvtHandler)

	// this is obviously the most important event handler; we don't
	// want to intercept wxEVT_KEY_UP because we don't need them:
	// a command must be immediately executed when one of its shourtcuts is
	// sent to the window.
	EVT_KEY_DOWN(wxKeyBinder::OnChar)

#if defined( __WXMSW__	)		// supported only on Win32
#if wxCHECK_VERSION(2, 5, 1)	// and from wxWidgets 2.5.1

	// I don't think this is needed because wxEVT_HOTKEY are generated
	// only in some special cases...
	EVT_HOTKEY(wxID_ANY, wxKeyBinder::OnChar)
#endif
#endif

END_EVENT_TABLE()



// event table for wxKeyMonitorTextCtrl
IMPLEMENT_CLASS(wxKeyMonitorTextCtrl, wxTextCtrl)
BEGIN_EVENT_TABLE(wxKeyMonitorTextCtrl, wxTextCtrl)

	// keydown events
	EVT_KEY_DOWN(wxKeyMonitorTextCtrl::OnKey)
    EVT_KEY_UP(wxKeyMonitorTextCtrl::OnKey)

END_EVENT_TABLE()



// event table for wxKeyConfigPanel
IMPLEMENT_CLASS(wxKeyConfigPanel, wxPanel)
BEGIN_EVENT_TABLE(wxKeyConfigPanel, wxPanel)

	// miscellaneous
	EVT_TREE_SEL_CHANGED(KEYBINDER_COMMANDS_BOX_ID, wxKeyConfigPanel::OnCommandSelected)
	EVT_TEXT(KEYBINDER_KEY_FIELD_ID, wxKeyConfigPanel::OnKeyPressed)
    EVT_LISTBOX(KEYBINDER_BINDINGS_BOX_ID, wxKeyConfigPanel::OnBindingSelected)
	EVT_COMBOBOX(KEYBINDER_KEYPROFILES_ID, wxKeyConfigPanel::OnProfileSelected)

	// buttons
	EVT_BUTTON(wxID_APPLY, wxKeyConfigPanel::OnApplyChanges)
    EVT_BUTTON(KEYBINDER_ASSIGN_KEY_ID, wxKeyConfigPanel::OnAssignKey)
    EVT_BUTTON(KEYBINDER_REMOVE_KEY_ID, wxKeyConfigPanel::OnRemoveKey)

	// during idle cycles, wxKeyConfigPanel checks if the wxKeyMonitorTextCtrl
	// associated must be cleared...	

END_EVENT_TABLE()






// ----------------------------------------------------------------------------
// wxKeyBind STATIC utilities
// ----------------------------------------------------------------------------

wxString wxKeyBind::NumpadKeyCodeToString(int keyCode)
{
	wxString res;
	
	switch (keyCode)
	{
		// NUMPAD KEYS
		// ---------------------------

	case WXK_NUMPAD0:
	case WXK_NUMPAD1:
	case WXK_NUMPAD2:
	case WXK_NUMPAD3:
	case WXK_NUMPAD4:
	case WXK_NUMPAD5:
	case WXK_NUMPAD6:
	case WXK_NUMPAD7:
	case WXK_NUMPAD8:
	case WXK_NUMPAD9:
		res << keyCode - WXK_NUMPAD0;
		break;

	case WXK_NUMPAD_SPACE:
		res << "SPACE"; break;
	case WXK_NUMPAD_TAB:
		res << "TAB"; break;
	case WXK_NUMPAD_ENTER:
		res << "ENTER"; break;

	case WXK_NUMPAD_F1:
	case WXK_NUMPAD_F2:
	case WXK_NUMPAD_F3:
	case WXK_NUMPAD_F4:
		res << "F" << keyCode - WXK_NUMPAD_F1;
		break;

	case WXK_NUMPAD_LEFT:
		res << "LEFT"; break;
	case WXK_NUMPAD_UP:
		res << "UP"; break;
	case WXK_NUMPAD_RIGHT:
		res << "RIGHT"; break;
	case WXK_NUMPAD_DOWN:
		res << "DOWN"; break;
		
	case WXK_NUMPAD_HOME:
		res << "HOME"; break;
	case WXK_NUMPAD_PRIOR:
		res << "PRIOR"; break;
	case WXK_NUMPAD_PAGEUP:
		res << "PAGEUP"; break;
	case WXK_NUMPAD_NEXT:
		res << "NEXT"; break;
	case WXK_NUMPAD_PAGEDOWN:
		res << "PAGEDOWN"; break;
	case WXK_NUMPAD_END:
		res << "END"; break;
	case WXK_NUMPAD_BEGIN:
		res << "BEGIN"; break;
	case WXK_NUMPAD_INSERT:
		res << "INSERT"; break;
	case WXK_NUMPAD_DELETE:
		res << "DELETE"; break;
	case WXK_NUMPAD_EQUAL:
		res << "="; break;
	case WXK_NUMPAD_MULTIPLY:
		res << "*"; break;
	case WXK_NUMPAD_ADD:
		res << "+"; break;
	case WXK_NUMPAD_SEPARATOR:
		res << "SEPARATOR"; break;
	case WXK_NUMPAD_SUBTRACT:
		res << "-"; break;
	case WXK_NUMPAD_DECIMAL:
		res << "."; break;
	case WXK_NUMPAD_DIVIDE:
		res << "/"; break;
	}

	return res;
}

wxString wxKeyBind::KeyCodeToString(int keyCode)
{
	wxString res;
	
	switch (keyCode)
	{
		// IGNORED KEYS
		// ---------------------------
	case WXK_START:
	case WXK_LBUTTON:
	case WXK_RBUTTON:
	case WXK_MBUTTON:
	case WXK_CLEAR:

	case WXK_PAUSE:
	case WXK_NUMLOCK:
	case WXK_SCROLL :
		wxLogDebug("wxKeyBind::KeyCodeToString - ignored key: [%d]", keyCode);
		return wxEmptyString;

		// these must be ABSOLUTELY ignored: they are key modifiers
		// we won't output any LOG message since these keys could be pressed
		// for long time while the user choose its preferred keycombination:
		// this would result into a long long queue of "ignored key" messages
		// which would be useless even in debug builds...
	case WXK_SHIFT:
	case WXK_CONTROL:
		return wxEmptyString;




		// FUNCTION KEYS
		// ---------------------------

	case WXK_F1: case WXK_F2:
	case WXK_F3: case WXK_F4:
	case WXK_F5: case WXK_F6:
	case WXK_F7: case WXK_F8:
	case WXK_F9: case WXK_F10:
	case WXK_F11: case WXK_F12:
	case WXK_F13: case WXK_F14:
    case WXK_F15: case WXK_F16:
    case WXK_F17: case WXK_F18:
    case WXK_F19: case WXK_F20:
    case WXK_F21: case WXK_F22:
    case WXK_F23: case WXK_F24:
		res << wxT('F') << keyCode - WXK_F1 + 1;
		break;
		


		// MISCELLANEOUS KEYS
		// ---------------------------

	case WXK_BACK:
        res << "BACK"; break;
	case WXK_TAB:
        res << "TAB"; break;
	case WXK_RETURN:
        res << "RETURN"; break;
	case WXK_ESCAPE:
        res << "ESCAPE"; break;
	case WXK_SPACE:
        res << "SPACE"; break;
	case WXK_DELETE:
        res << "DELETE"; break;
	case WXK_MULTIPLY:
		res << "*"; break;
	case WXK_ADD:
		res << "+"; break;
	case WXK_SEPARATOR:
		res << "SEPARATOR"; break;
	case WXK_SUBTRACT:
		res << "-"; break;
	case WXK_DECIMAL:
		res << "."; break;
	case WXK_DIVIDE:
		res << "/"; break;
	case WXK_PAGEUP:
		res << "PAGEUP"; break;
	case WXK_PAGEDOWN:
		res << "PAGEDOWN"; break;
	case WXK_LEFT:
        res << "LEFT"; break;
	case WXK_UP:
        res << "UP"; break;
	case WXK_RIGHT:
        res << "RIGHT"; break;
	case WXK_DOWN:
        res << "DOWN"; break;
	case WXK_SELECT:
        res << "SELECT"; break;
	case WXK_PRINT:
        res << "PRINT"; break;
	case WXK_EXECUTE:
        res << "EXECUTE"; break;
	case WXK_SNAPSHOT:
        res << "SNAPSHOT"; break;
	case WXK_INSERT:
        res << "INSERT"; break;
	case WXK_HELP:
        res << "HELP"; break;
	case WXK_CANCEL:
        res << "CANCEL"; break;
	case WXK_MENU:
        res << "MENU"; break;
	case WXK_CAPITAL:
        res << "CAPITAL"; break;
	case WXK_PRIOR:
        res << "PRIOR"; break;
	case WXK_NEXT:
        res << "NEXT"; break;
	case WXK_END:
        res << "END"; break;
	case WXK_HOME:
        res << "HOME"; break;
		


	default:

		// ASCII chars...
		if (wxIsalnum(keyCode))
		{
			res << (wxChar)keyCode;
			break;
			
		} else if ((res=NumpadKeyCodeToString(keyCode)) != wxEmptyString) {

			res << " (numpad)";		// so it is clear it's different from other keys
			break;

		} else {
			
			// we couldn't create a description for the given keycode...
			wxLogDebug("wxKeyBind::KeyCodeToString - unknown key: [%d]", keyCode);
			return wxEmptyString;
		}
	}
	
	return res;
}

int wxKeyBind::StringToKeyCode(const wxString &keyName)
{
	// a function key ?
	if (keyName.StartsWith("F") && keyName.Len() > 1) {

		long n;
		keyName.Right(keyName.Len()-1).ToLong(&n);
		return WXK_F1+n-1;
	}

	// a special key ?
	if (keyName == "BACK") return WXK_BACK;
	if (keyName == "ENTER") return WXK_RETURN;
	if (keyName == "RETURN") return WXK_RETURN;
	if (keyName == "TAB") return WXK_TAB;
	if (keyName == "ESCAPE") return WXK_ESCAPE;
	if (keyName == "SPACE") return WXK_SPACE;
	if (keyName == "DELETE") return WXK_DELETE;

	// it should be an ASCII key...
	return (int)keyName.GetChar(0);
}

wxString wxKeyBind::KeyModifierToString(int keyModifier)
{
    wxString result;

    if (keyModifier & wxACCEL_CTRL)
        result += "Ctrl+";
    if (keyModifier & wxACCEL_ALT)
        result += "Alt+";
    if (keyModifier & wxACCEL_SHIFT)
        result += "Shift+";

    return result;
}

int wxKeyBind::StringToKeyModifier(const wxString &keyModifier)
{
    int mod = 0;

	// this search must be case-insensitive
	wxString str = keyModifier;
	str.MakeUpper();

    if (str.Contains("ALT"))
        mod |= wxACCEL_ALT;

    if (str.Contains("CTRL"))
        mod |= wxACCEL_CTRL;

    if (str.Contains("SHIFT"))
        mod |= wxACCEL_SHIFT;

    return mod;
}

int wxKeyBind::GetKeyModifier(wxKeyEvent &event)
{
    int mod = 0;
    if (event.AltDown())
        mod |= wxACCEL_ALT;

    if (event.ControlDown())
        mod |= wxACCEL_CTRL;

    if (event.ShiftDown())
        mod |= wxACCEL_SHIFT;

    return mod;
}

wxString wxKeyBind::GetKeyStrokeString(wxKeyEvent &event)
{
	// key stroke string = key modifiers (Ctrl, Alt or Shift) + key code
    return KeyModifierToString(GetKeyModifier(event)) + 
		KeyCodeToString(event.GetKeyCode());
}






// ----------------------------------------------------------------------------
// wxKeyBind
// ----------------------------------------------------------------------------

bool wxKeyBind::MatchKey(const wxKeyEvent &key) const
{
	bool b = (key.GetKeyCode() == m_nKeyCode);

	// all the flags must match
	b &= (key.AltDown() == ((m_nFlags & wxACCEL_ALT) != 0));
	b &= (key.ControlDown() == ((m_nFlags & wxACCEL_CTRL) != 0));
	b &= (key.ShiftDown() == ((m_nFlags & wxACCEL_SHIFT) != 0));

	return b;
}





// --------------------
// wxCmdArray
// --------------------

void wxCmdArray::Remove(int n)
{
	if (n < 0 || n >= GetCount())
		return;

	// first, delete the memory associated with the n-th wxCmd
	delete Item(n);

	// then, remove that pointer from the array
	m_arr.RemoveAt(n);
}

void wxCmdArray::Clear()
{
	for (int i=GetCount(); i > 0; i--)
		Remove(0);

	// the array should be already empty
	m_arr.Clear();
}





// ----------------------------------------------------------------------------
// wxKeyBinder
// ----------------------------------------------------------------------------

void wxKeyBinder::ImportMenuBarCmd(wxMenuBar *p)
{
	wxMenuShortcutWalker wlkr;
	
	// it's very easy performing this task using a wxMenuShortcutWalker  :-)
	wlkr.ImportMenuBarCmd(p, &m_arrCmd);
}

void wxKeyBinder::OnChar(wxKeyEvent &event)
{
	// this is the first function which is called when the user presses
	// a key in the window Attach()ed to this key binder: we have to
	// find if the given keyevent matches a key shortcut and eventually
	// exec the command associated with that shortcut...
	wxCmd *p = GetMatchingCmd(event);

	// AVOID TO INTERCEPT Alt+F4 KEYPRESSES !!!
	// For some reasons on wxMSW 2.5.2 (at least) this provokes a crash
	// which is really difficult to spot... better leave it...
	if (p && p->IsBindTo(wxKeyBind("Alt+F4")))
		return;

	// if the given event is not a shortcut key...
	if (p == NULL)
		event.Skip();		// ... skip it		
	else
		p->Exec(event.GetEventObject(),		// otherwise, tell wxCmd to send the
				this->GetNextHandler());	// associated wxEvent to the next handler in the chain
}






// ----------------------------------------------------------------------------
// wxKeyMonitorTextCtrl
// ----------------------------------------------------------------------------

void wxKeyMonitorTextCtrl::OnKey(wxKeyEvent &event)
{
	// backspace cannot be used as shortcut key...
    if (event.GetKeyCode() == WXK_BACK) {

		// this text ctrl contains something and the user pressed backspace...
		// we must delete the keypress...
		Clear();
		return;
	}

	if (event.GetEventType() == wxEVT_KEY_DOWN || 
		(event.GetEventType() == wxEVT_KEY_UP && !IsValidKeyComb())) {

		// the user pressed some key combination which must be displayed 
		// in this text control.... or he has just stopped pressing a
		// modifier key like shift, ctrl or alt without adding any
		// other alphanumeric char, thus generating an invalid keystroke
		// which must be cleared out...
		SetValue(wxKeyBind::GetKeyStrokeString(event));
		SetInsertionPointEnd();
	}
}





// ----------------------------------------------------------------------------
// wxKeyConfigPanel - general functions
// ----------------------------------------------------------------------------

wxKeyConfigPanel::wxKeyConfigPanel(wxWindow* parent, 
				wxKeyProfile *pBinder,	// can be NULL
				bool showApplyBtn, wxWindowID id, 
				const wxPoint& pos, const wxSize& size, 
				long style, const wxString& name)
				: wxPanel(parent, id, pos, size, style, name)
{
    // creating wx controls
    m_pCommands = new wxTreeCtrl(this, KEYBINDER_COMMANDS_BOX_ID, wxDefaultPosition, 
		wxDefaultSize, wxTR_HAS_BUTTONS | wxSUNKEN_BORDER);//wxListBox(this, GROUPS_BOX_ID);    
    m_pKeyField = new wxKeyMonitorTextCtrl(this, KEYBINDER_KEY_FIELD_ID);
    m_pBindings = new wxListBox(this, KEYBINDER_BINDINGS_BOX_ID);

    m_pAssignBtn = new wxButton(this, KEYBINDER_ASSIGN_KEY_ID, "&Add");
    m_pRemoveBtn = new wxButton(this, KEYBINDER_REMOVE_KEY_ID, "&Remove");
	
	m_pDescLabel = new wxStaticText(this, -1, "", wxDefaultPosition, 
		wxSize(-1, 40), wxSUNKEN_BORDER | wxST_NO_AUTORESIZE);
    //m_pDescLabel = new wxTextCtrl(this, -1, "", wxDefaultPosition, 
	// wxDefaultSize, wxTE_READONLY);
	m_pCurrCmdField = new wxStaticText(this, -1, "", wxDefaultPosition, 
		wxSize(-1, 20), wxSUNKEN_BORDER | wxST_NO_AUTORESIZE | wxALIGN_CENTRE);


	// KEY PROFILES
	// create the key profiles combobox & panel
	m_bEnableKeyProfiles = TRUE;

	m_pKeyProfiles = new wxComboBox(this, KEYBINDER_KEYPROFILES_ID,
		wxEmptyString, wxDefaultPosition, wxDefaultSize, 
		0, NULL, wxCB_READONLY | wxCB_SORT);

	m_pKeyProfilesSizer = new wxBoxSizer(wxVERTICAL);//wxPanel(this, -1, , );
	m_pKeyProfilesSizer->Add(new wxStaticText(this, -1, "Key profile:"), 0, wxGROW | wxALL, 5);
	m_pKeyProfilesSizer->Add(m_pKeyProfiles, 0, wxGROW | wxLEFT | wxRIGHT, 5);
	m_pKeyProfilesSizer->Add(new wxStaticLine(this, -1), 0, wxGROW | wxALL, 5);


	// FIRST COLUMN:
	// "Groups"
	wxBoxSizer *column1 = new wxBoxSizer(wxVERTICAL);
    column1->Add(new wxStaticText(this, -1, "Commands:"), 0, wxGROW | wxALL, 5);
	column1->Add(m_pCommands, 1, wxGROW | wxRIGHT | wxLEFT | wxBOTTOM, 5);


	// SECOND COLUMN:
	// "Current keys" + "Press new shortcut key"
	wxBoxSizer *column2 = new wxBoxSizer(wxVERTICAL);
    column2->Add(new wxStaticText(this, -1, "Current shortcuts:"), 0, wxGROW | wxALL, 5);
    column2->Add(m_pBindings, 2, wxGROW | wxRIGHT | wxLEFT, 5);
    column2->Add(m_pRemoveBtn, 1, wxGROW | wxALL, 5);

	column2->Add(new wxStaticLine(this, -1), 0, wxGROW | wxALL, 10);
	
	column2->Add(new wxStaticText(this, -1, "New shortcut:"), 0, wxGROW | wxALL, 5);
    column2->Add(m_pKeyField, 0, wxGROW | wxLEFT | wxRIGHT, 5);

	column2->Add(new wxStaticText(this, -1, "Currently assigned to:"), 0, wxGROW | wxALL, 5);
	column2->Add(m_pCurrCmdField, 0, wxGROW | wxLEFT | wxRIGHT, 5);

    column2->Add(m_pAssignBtn, 1, wxGROW | wxALL, 5);



	// set up the column container
	wxBoxSizer *cont = new wxBoxSizer(wxHORIZONTAL);
	cont->Add(column1, 5, wxGROW);
	cont->Add(10, 1, 0, wxGROW);
	cont->Add(column2, 4, wxGROW);

	// create the main sizer
	wxBoxSizer *main = new wxBoxSizer(wxVERTICAL);
	main->Add(m_pKeyProfilesSizer, 0, wxGROW);
	main->Add(cont, 5, wxGROW);
	main->Add(new wxStaticLine(this, -1), 0, wxGROW | wxALL, 5);

	// key description
	main->Add(new wxStaticText(this, -1, "Description:"), 0, wxGROW | wxALL, 5);
    main->Add(m_pDescLabel, 0, wxGROW | wxLEFT | wxRIGHT | wxBOTTOM, 5);


	// if required, add Apply and Cancel buttons
	if (showApplyBtn)
    {
	    wxBoxSizer *btn = new wxBoxSizer(wxHORIZONTAL);

        wxButton *applyBtn = new wxButton(this, wxID_APPLY,  "A&pply");
        wxButton *cancelBtn = new wxButton(this, wxID_CANCEL, "C&ancel");

		// create the button panel with some interleaved spacer
		btn->Add(1, 1, 1, wxGROW);
        btn->Add(applyBtn, 4, wxGROW | wxALL, 5);
		btn->Add(1, 1, 1, wxGROW);
        btn->Add(cancelBtn, 4, wxGROW | wxALL, 5);
		btn->Add(1, 1, 1, wxGROW);
 	
		main->Add(1, 1, 0, wxGROW);
		main->Add(btn, 1, wxGROW | wxALL, 5);
   }

	// set the final sizer as window's sizer
	SetSizer(main);
	main->SetSizeHints(this);

	// set up the controls: the user of the panel must call one of the
	// ImportXXXX() functions to enanble the use of the panel !!!!
	AddProfile(pBinder);
    m_pCommands->SetFocus();
    UpdateButtons();
}



void wxKeyConfigPanel::ImportMenuBarCmd(wxMenuBar *p, const wxString &rootname)
{
	// do some std things...
	Reset();
	AddRootIfMissing(rootname);

	// the wxKeyBinder class can easily generate a tree structure...
	wxMenuTreeWalker wlkr;
	wlkr.FillTreeCtrl(p, m_pCommands, rootname);

	// expand the root (just for aesthetic/comfort reasons)...
	m_pCommands->Expand(m_pCommands->GetRootItem());
}

void wxKeyConfigPanel::ImportKeyProfileCmd(int n, const wxString &rootname)
{
	// do some std things...
	Reset();
	AddRootIfMissing(rootname);
	wxTreeItemId rootid = m_pCommands->GetRootItem();

	// scan all the commands of the key binder...
	wxCmdArray *arr = GetProfile(n)->GetArray();
	for (int i=0; i < (int)arr->GetCount(); i++) {

		// to each tree branch attach a wxTreeItemData containing 
		// the ID of the menuitem which it represents...
		wxExTreeItemData *treedata = new wxExTreeItemData(arr->Item(i)->GetId());

		// create the new item in the tree ctrl
		m_pCommands->AppendItem(rootid, arr->Item(i)->GetName(), -1, -1, treedata);
	}

	// expand the root (just for aesthetic/comfort reasons)...
	m_pCommands->Expand(m_pCommands->GetRootItem());
}

void wxKeyConfigPanel::AddProfile(wxKeyProfile *p, bool bImport, const wxString &rootname)
{	
	// add a new profile to the array	
	m_pKeyProfiles->Append(p->GetName(), (void *)p);
	
	if (GetProfiles()->GetCount() == 1) {
		m_pKeyProfiles->SetSelection(0);

		// generate a fake event: SetSelection does not generate it
		wxCommandEvent ev;
		OnProfileSelected(ev);
	}

	// eventually import the commands contained in the given keyprofile
	if (bImport)
		ImportKeyProfileCmd(GetSelProfileIdx(), rootname);
}

void wxKeyConfigPanel::Reset()
{
	m_pCommands->DeleteAllItems();
	m_pBindings->Clear();
	m_pDescLabel->SetLabel("");//Clear();
	m_pKeyField->Clear();
}

void wxKeyConfigPanel::AddRootIfMissing(const wxString &rootname)
{
	if (!m_pCommands->GetRootItem().IsOk()) {

		// I don't know what happens if we try to create the root
		// of the control while there are still some items in the
		// control... even if I don't think some items maybe
		// present in the control if the root is not okay....
		m_pCommands->DeleteAllItems();

		m_pCommands->AddRoot(rootname);
	}
}

wxCmd *wxKeyConfigPanel::GetSelCmd()
{
	wxTreeItemId treeid = GetSelCmdId();
	if (!treeid.IsOk())
		return NULL;

	// each tree item has a wxMenuTreeItemData associated which contains
	// the ID of the menu item which is attached...
	int id  = ((wxExTreeItemData *)m_pCommands->GetItemData(treeid))->GetMenuItemId();
	return m_kBinder.GetCmd(id);
}

wxTreeItemId wxKeyConfigPanel::GetSelCmdId()
{
    wxTreeItemId selection = m_pCommands->GetSelection();

	// selection must be valid...
    if (!selection.IsOk())
		return selection;

	// ...and the just selected must be a valid menu item...
	wxTreeItemData *p = m_pCommands->GetItemData(selection);
	if (p == NULL)
		return wxTreeItemId();		// an empty wxTreeItemId is always invalid...

	return selection;
}

wxString wxKeyConfigPanel::GetSelCmdStr()
{
	wxTreeItemId id = GetSelCmdId();

	// wxEmpyString is returned if there's no valid selection
	if (!id.IsOk())
		return wxEmptyString;

	return m_pCommands->GetItemText(id);
}

void wxKeyConfigPanel::UpdateButtons()
{
	wxLogDebug("wxKeyConfigPanel::UpdateButtons");
	wxString str;

	// is the remove button to be enabled ?
    m_pRemoveBtn->Enable(m_pBindings->GetSelection() >= 0);

	// is the assign button to be enabled ?
	bool b = GetSelCmdId().IsOk() && m_pKeyField->IsValidKeyComb();
    m_pAssignBtn->Enable(b);

	// must the "Currently assigned to" field be updated ?
	if (m_pKeyField->IsValidKeyComb()) {
		wxCmd *p = m_kBinder.GetCmdBindTo(m_pKeyField->GetValue());
		
		if (p) {
			
			// another command already owns this key bind...
			m_pCurrCmd = p;
			str = p->GetName();

		} else {

			str = "None";
			m_pCurrCmd = NULL;
		}
	}
	
	m_pCurrCmdField->SetLabel(str);
}

void wxKeyConfigPanel::UpdateDesc()
{
	wxLogDebug("wxKeyConfigPanel::UpdateDesc");
	wxCmd *p = GetSelCmd();

	if (p != NULL) {
		
		// and then update the description
		m_pDescLabel->SetLabel(p->GetDescription());

	} else {

		// an invalid command is selected ? clear this field...
		m_pDescLabel->SetLabel("");
	}
}

void wxKeyConfigPanel::FillInBindings()
{
	// remove (eventually present) previous shortcuts...
	m_pBindings->Clear();

	// is the selected item okay ?
	wxCmd *p = GetSelCmd();
	if (p == NULL)
		return;

	// update the listbox with the shortcuts for the selected menuitem
    m_pBindings->Append(p->GetShortcutsList());
}

void wxKeyConfigPanel::ApplyChanges()
{
	wxLogDebug("wxKeyConfigPanel::ApplyChanges");

	// just copy the internal key binder used to allow a sort of
	// "undo" feature into the original binder set through the
	// #SetBinder function.
    GetSelProfile()->DeepCopy(&m_kBinder);
}

void wxKeyConfigPanel::EnableKeyProfiles(bool bEnable)
{
	m_bEnableKeyProfiles = bEnable;

	// we must hide the keyprofile combobox		
	ShowSizer(m_pKeyProfilesSizer, m_bEnableKeyProfiles);
}

void wxKeyConfigPanel::ShowSizer(wxSizer *toshow, bool show)
{
	// first of all, update the items of the sizer
	toshow->ShowItems(m_bEnableKeyProfiles);

	// then, since the size has been changed...
	wxSizer *main = GetSizer();

	// check the old show state...
	// VERY IMPORTANT: this is not only an optimization
	//                 the code below expect a size change and
	//                 if the size change did not happen it will
	//                 set some vars to wrong values
	bool oldshow = main->IsShown(toshow);
	if ((show && oldshow) || (!show && !oldshow))
		return;

	// add or detach the given sizer
	if (show)
		main->Prepend(toshow, 0, wxGROW);
	else
		main->Detach(toshow);


	// THIS PIECE OF CODE HAS BEEN COPIED & PASTED
	// FROM THE wxLogDialog::OnDetails OF THE 
	// wxWidgets/src/generic/logg.cpp FILE
	// -------------------------------------------
	m_minHeight = m_maxHeight = -1;
	
   // wxSizer::FitSize() is private, otherwise we might use it directly...
    wxSize sizeTotal = GetSize(),
           sizeClient = GetClientSize();

    wxSize size = main->GetMinSize();
    size.x += sizeTotal.x - sizeClient.x;
    size.y += sizeTotal.y - sizeClient.y;

    // we don't want to allow expanding the dialog in vertical direction as
    // this would show the "hidden" details but we can resize the dialog
    // vertically while the details are shown
    if ( !show )
        m_maxHeight = size.y;

    SetSizeHints(size.x, size.y, m_maxWidth, m_maxHeight);

    // don't change the width when expanding/collapsing
    SetSize(wxDefaultCoord, size.y);

#ifdef __WXGTK__
    // VS: this is neccessary in order to force frame redraw under
    // WindowMaker or fvwm2 (and probably other broken WMs).
    // Otherwise, detailed list wouldn't be displayed.
    Show();
#endif // wxGTK
}




// ----------------------------------------------------------------------------
// wxKeyConfigPanel - event handlers
// ----------------------------------------------------------------------------

void wxKeyConfigPanel::OnApplyChanges(wxCommandEvent &event)
{
    // apply changed bindings to the original binder
    ApplyChanges();	
	event.Skip();		// let parent know that changes were applied
}

void wxKeyConfigPanel::OnCommandSelected(wxTreeEvent &)
{
	wxLogDebug("wxKeyConfigPanel::OnCommandSelected");

	// one of the group in the tree ctrl has been selected:
	// the listbox with the keybindings must be synchronized...
    FillInBindings();

	// if the user selected an invalid/valid item in the tree ctrl,
	// the buttons must be disabled/enabled...
    UpdateButtons();

	// ...and the description must be updated
	UpdateDesc();
}

void wxKeyConfigPanel::OnBindingSelected(wxCommandEvent &)
{
	wxLogDebug("wxKeyConfigPanel::OnBindingSelected");

	// the remove button should be enabled if the
	// element just selected is valid...
	UpdateButtons();
}

void wxKeyConfigPanel::OnProfileSelected(wxCommandEvent &)
{
	wxLogDebug("wxKeyConfigPanel::OnProfileSelected");

	wxKeyProfile *sel = GetSelProfile();
	if (!sel) return;

	// copy original bindings to the temporary binder, 
	// which is the one which the user edits (and which
	// will be copied in the original one if the 
	// #ApplyChanges function is called...)
	m_kBinder.DeepCopy(sel);
	
	// call other event handlers
	wxTreeEvent ev;
	OnCommandSelected(ev);
}

void wxKeyConfigPanel::OnAssignKey(wxCommandEvent &)
{
	// the new key combination should be valid because only when
	// it's valid this button is enabled...	
	wxASSERT(m_pKeyField->IsValidKeyComb());	

	// actually add the new shortcut key
	// (if there are already the max. number of shortcuts for 
	// this command, the shortcut won't be added).
	GetSelCmd()->AddShortcut(m_pKeyField->GetValue());

	// if the just added key bind was owned by another command,
	// remove it from the old command...
	if (m_pCurrCmd) {
		wxKeyBind tmp(m_pKeyField->GetValue());
		int n;

		wxASSERT(m_pCurrCmd->IsBindTo(tmp, &n));
		m_pCurrCmd->RemoveShortcut(n);
	}

	// and update the list of the key bindings
	FillInBindings();
	m_pKeyField->Clear();
}

void wxKeyConfigPanel::OnRemoveKey(wxCommandEvent &)
{
	// remove the selected shortcut
	GetSelCmd()->RemoveShortcut(m_pBindings->GetSelection());

	// and update the list of the key bindings
	FillInBindings();
	UpdateButtons();
}

void wxKeyConfigPanel::OnKeyPressed(wxCommandEvent &)
{
	// if there were no key combinations in the wxKeyMonitorTextCtrl
	// before this event, maybe that now there is one.... this means
	// that the "Assign" could be enabled...
	UpdateButtons();
}






