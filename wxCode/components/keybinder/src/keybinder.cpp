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
#include "wx/config.h"
#include "wx/tokenzr.h"


// class definition for wxKeyProfile
IMPLEMENT_CLASS(wxKeyProfile, wxKeyBinder)


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
	EVT_TEXT(wxKEYBINDER_KEY_FIELD_ID, wxKeyConfigPanel::OnKeyPressed)
	EVT_TEXT(wxKEYBINDER_KEYPROFILES_ID, wxKeyConfigPanel::OnProfileEditing)
	EVT_COMBOBOX(wxKEYBINDER_KEYPROFILES_ID, wxKeyConfigPanel::OnProfileSelected)
	EVT_LISTBOX(wxKEYBINDER_BINDINGS_BOX_ID, wxKeyConfigPanel::OnBindingSelected)

	// used when the treectrl is used
	EVT_TREE_SEL_CHANGED(wxKEYBINDER_COMMANDS_BOX_ID, 
						wxKeyConfigPanel::OnTreeCommandSelected)
    
	// used when the listbox+combobox is used
	EVT_COMBOBOX(wxKEYBINDER_CATEGORIES_ID, wxKeyConfigPanel::OnCategorySelected)
	EVT_LISTBOX(wxKEYBINDER_COMMANDS_BOX_ID, wxKeyConfigPanel::OnListCommandSelected)

	// buttons
	EVT_BUTTON(wxID_APPLY, wxKeyConfigPanel::OnApplyChanges)
    EVT_BUTTON(wxKEYBINDER_ASSIGN_KEY_ID, wxKeyConfigPanel::OnAssignKey)
    EVT_BUTTON(wxKEYBINDER_REMOVE_KEY_ID, wxKeyConfigPanel::OnRemoveKey)
    EVT_BUTTON(wxKEYBINDER_REMOVEALL_KEY_ID, wxKeyConfigPanel::OnRemoveAllKey)
    EVT_BUTTON(wxKEYBINDER_ADD_PROFILEBTN_ID, wxKeyConfigPanel::OnAddProfile)
    EVT_BUTTON(wxKEYBINDER_REMOVE_PROFILEBTN_ID, wxKeyConfigPanel::OnRemoveProfile)

	// during idle cycles, wxKeyConfigPanel checks if the wxKeyMonitorTextCtrl
	// associated must be cleared...	

END_EVENT_TABLE()



// some statics
int wxCmd::m_nCmdTypes = 0;
wxCmd::wxCmdType wxCmd::m_arrCmdType[];




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
// wxCmd
// --------------------

// static
wxCmd::wxCmdType *wxCmd::FindCmdType(int type)
{
	int found = -1;
	for (int i=0; i < m_nCmdTypes; i++)
		if (m_arrCmdType[i].type == type)
			found = i;
	
	if (found == -1) 
		return NULL;
	return &m_arrCmdType[found];
}

// static
wxCmd *wxCmd::CreateNew(int type, int id)
{
	wxCmdType *found = FindCmdType(type);
	if (!found) return NULL;

	// get the creation function address
	wxCmdCreationFnc fnc = found->cmdCreateFnc;
	wxASSERT(fnc);

	// create the wxCmd-derived class & init it
	wxCmd *ret = fnc(id);
	ret->Update();

	return ret;
}

// static
void wxCmd::AddCmdType(int type, wxCmdCreationFnc f)
{
	if (FindCmdType(type) != NULL)
		return;		// already registered

	m_arrCmdType[m_nCmdTypes].type = type;
	m_arrCmdType[m_nCmdTypes].cmdCreateFnc = f;
	m_nCmdTypes++;
}

bool wxCmd::Save(wxConfigBase *p, const wxString &key) const
{
	// build the shortcut string separing each one with a "|"
	wxString shortcuts;
	for (int j=0; j < GetShortcutCount(); j++)
		shortcuts += GetShortcut(j)->GetStr() + "|";
	
	// write the entry in the format NAME|DESC|SHORTCUT1|SHORTCUT2...|SHORTCUTn
	wxString value = wxString::Format("%s|%s|%s", 									
									GetName().c_str(),
									GetDescription().c_str(), 
									shortcuts.c_str());
	return p->Write(key, value);
}

bool wxCmd::Load(wxConfigBase *p, const wxString &key)
{
	wxString fmt;
	if (!p->Read(key, &fmt, "|"))
		return FALSE;
			
	// extract name & desc
	wxStringTokenizer tknzr(fmt, "|");	
	wxString name = tknzr.GetNextToken();
	wxString desc = tknzr.GetNextToken();
	if (name.IsEmpty())
		return FALSE;	// this is an invalid entry...

	// extract the keybindings...
	while (tknzr.HasMoreTokens())
		AddShortcut(tknzr.GetNextToken());

	Update();
	return TRUE;
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

bool wxKeyBinder::Save(wxConfigBase *cfg, const wxString &key) const
{
	wxString basekey = (key.IsEmpty() ? wxEmptyString : key + "/");
	bool b = TRUE;
	
	for (int i=0; i < m_arrCmd.GetCount(); i++) {
		
		wxCmd *curr = m_arrCmd.Item(i);

		// write the key in the format: bindID-typeID
		wxString keyname = wxString::Format("%sbind%d-type%d", 
			basekey.c_str(), curr->GetId(), curr->GetType());

		// save this wxCmd...
		b &= curr->Save(cfg, keyname);
	}

	return b;
}

bool wxKeyBinder::Load(wxConfigBase *p, const wxString &key)
{
	wxString str;
	bool cont;
	bool b = TRUE;
	int total = 0;
	long idx;

	// before starting...
	p->SetPath(key);
	m_arrCmd.Clear();

	cont = p->GetFirstEntry(str, idx);
	while (cont) {

		// try to decode this entry
		if (str.StartsWith("bind")) {

			wxString id(str.BeforeFirst('-'));
			wxString type(str.AfterFirst('-'));
			id = id.Right(id.Len()-wxString("bind").Len());	
			type = type.Right(type.Len()-wxString("type").Len());	

			// is this a valid entry ?
			if (id.IsNumber() && type.IsNumber() &&
				p->GetEntryType(str) == wxConfigBase::EntryType::Type_String) {

				// we will interpret this group as a command ID
				int nid = atoi(id);
				int ntype = atoi(type);

				// create & load this command
				wxCmd *cmd = wxCmd::CreateNew(ntype, nid);
				if (!cmd->Load(p, str)) {
					cont = FALSE;
					break;
				}

				m_arrCmd.Add(cmd);		// add to the array
				total++;
			}
		}

		// proceed with next entry (if it does exist)
		cont &= p->GetNextEntry(str, idx);
	}

	return (b && total > 0);
}





// ----------------------------------------------------------------------------
// wxKeyProfile
// ----------------------------------------------------------------------------

bool wxKeyProfile::Save(wxConfigBase *cfg, const wxString &key) const
{
	wxString basekey = (key.IsEmpty() ? "" : "/") + wxString("keyprof-") + GetName();	

	if (!cfg->Write(basekey + "/desc", GetDesc()))
		return FALSE;

	// tell wxKeyBinder to save all keybindings into a group with our name
	return wxKeyBinder::Save(cfg, basekey);
}

bool wxKeyProfile::Load(wxConfigBase *p, const wxString &key)
{
	wxString str;
	bool cont;
	long idx;

	// before starting...
	p->SetPath(key);

	cont = p->GetFirstGroup(str, idx);
	while (cont) {

		// try to decode this group name
		if (str.StartsWith("keyprof-")) {

			wxString name(str.AfterFirst('-'));
			wxString desc;
			p->SetPath(str);		// enter into this group

			// is this a valid entry ?
			if (p->HasEntry("desc")) {

				if (!p->Read("desc", &desc))
					return FALSE;

				SetName(name);
				SetDesc(desc);
				return wxKeyBinder::Load(p, key + "/" + str);	// load from this path
			}
		}

		// proceed with next entry (if it does exist)
		cont &= p->GetNextGroup(str, idx);
	}

	return FALSE;		// could not find a valid group to load !
}




// ----------------------------------------------------------------------------
// wxKeyProfileArray
// ----------------------------------------------------------------------------

bool wxKeyProfileArray::Save(wxConfigBase *cfg, const wxString &key) const
{
	bool b = TRUE;
	for (int i=0; i<GetCount(); i++)
		b &= Item(i)->Save(cfg, key);

	return b;
}

bool wxKeyProfileArray::Load(wxConfigBase *cfg, const wxString &key)
{
	bool b = TRUE;
	for (int i=0; i<GetCount(); i++)
		b &= Item(i)->Load(cfg, key);

	return b;
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
// wxKeyConfigPanel - BUILD functions
// ----------------------------------------------------------------------------

wxKeyConfigPanel::wxKeyConfigPanel(wxWindow* parent, 
				wxKeyProfile *pBinder,	// can be NULL
				int buildMode, wxWindowID id, 
				const wxPoint& pos, const wxSize& size, 
				long style, const wxString& name)
				: wxPanel(parent, id, pos, size, style, name)
{
	m_nBuildMode = buildMode;

	// build everything
	BuildCtrls();
	wxSizer *column1 = BuildColumn1();
	wxSizer *column2 = BuildColumn2();
	wxSizer *main = BuildMain(column1, column2, 
		(m_nBuildMode & wxKEYBINDER_SHOW_APPLYBUTTON) != 0);

	// set the final sizer as window's sizer
	SetSizer(main);
	main->SetSizeHints(this);

	// set up the controls: the user of the panel must call one of the
	// ImportXXXX() functions to enanble the use of the panel !!!!
	AddProfile(pBinder);
    GetMainCtrl()->SetFocus();
    UpdateButtons();
}

void wxKeyConfigPanel::BuildCtrls()
{
    if (m_nBuildMode & wxKEYBINDER_USE_TREECTRL) {
    
		// use a wxTreeCtrl to show the commands hierarchy
		m_pCommandsTree = new wxTreeCtrl(this, wxKEYBINDER_COMMANDS_BOX_ID, wxDefaultPosition, 
									wxDefaultSize, wxTR_HAS_BUTTONS | wxSUNKEN_BORDER);
	} else {

		// use a combobox + a listbox
		m_pCommandsList = new wxListBox(this, wxKEYBINDER_COMMANDS_BOX_ID, wxDefaultPosition,
									wxDefaultSize, 0, NULL);
		m_pCategories = new wxComboBox(this, wxKEYBINDER_CATEGORIES_ID,
								wxEmptyString, wxDefaultPosition, wxDefaultSize, 
								0, NULL, wxCB_READONLY);
	}

    m_pKeyField = new wxKeyMonitorTextCtrl(this, wxKEYBINDER_KEY_FIELD_ID);
    m_pBindings = new wxListBox(this, wxKEYBINDER_BINDINGS_BOX_ID);

    m_pAssignBtn = new wxButton(this, wxKEYBINDER_ASSIGN_KEY_ID, "&Add");
    m_pRemoveBtn = new wxButton(this, wxKEYBINDER_REMOVE_KEY_ID, "&Remove");
    m_pRemoveAllBtn = new wxButton(this, wxKEYBINDER_REMOVEALL_KEY_ID, "Remove all");
	
	m_pDescLabel = new wxStaticText(this, -1, "", wxDefaultPosition, 
		wxSize(-1, 40), wxSIMPLE_BORDER | wxST_NO_AUTORESIZE);
    //m_pDescLabel = new wxTextCtrl(this, -1, "", wxDefaultPosition, 
	// wxDefaultSize, wxTE_READONLY);
	m_pCurrCmdField = new wxStaticText(this, -1, "", wxDefaultPosition, 
		wxSize(-1, 20), wxSUNKEN_BORDER | wxST_NO_AUTORESIZE | wxALIGN_CENTRE);
	m_pCurrCmdField->SetBackgroundColour(wxColour(255, 255, 255));
	m_pDescLabel->SetBackgroundColour(wxColour(255, 255, 255));

	// KEY PROFILES
	// create the key profiles combobox & panel
	m_bEnableKeyProfiles = TRUE;

	// the style of a combobox must be set at the beginning:
	// you cannot change the wxCB_READONLY flag presence later...
	// VERY IMPORTANT: *NEVER* ADD THE CB_SORT STYLE:
	// IT WOULD GIVE US GREAT PROBLEMS WHEN EDITING THE KEYPROFILE...
	long style = (m_nBuildMode & wxKEYBINDER_ENABLE_PROFILE_EDITING) ? 0 : wxCB_READONLY;
	m_pKeyProfiles = new wxComboBox(this, wxKEYBINDER_KEYPROFILES_ID,
								wxEmptyString, wxDefaultPosition, wxDefaultSize,
								0, NULL, style);

	wxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_pKeyProfiles, 6, wxGROW);

	if (m_nBuildMode & wxKEYBINDER_SHOW_ADDREMOVE_PROFILE) {

		// create the Add & remove profile buttons
		sizer->Add(new wxButton(this, wxKEYBINDER_ADD_PROFILEBTN_ID, "Add new"), 0, 
							wxGROW | wxLEFT | wxRIGHT | wxBOTTOM, 5);
		sizer->Add(new wxButton(this, wxKEYBINDER_REMOVE_PROFILEBTN_ID, "Remove"), 0, 
							wxGROW | wxLEFT | wxRIGHT | wxBOTTOM, 5);
	}

	m_pKeyProfilesSizer = new wxBoxSizer(wxVERTICAL);
	m_pKeyProfilesSizer->Add(new wxStaticText(this, -1, "Key profile:"), 0, wxGROW | wxALL, 5);
	m_pKeyProfilesSizer->Add(sizer, 0, wxGROW | wxLEFT | wxRIGHT, 5);
	m_pKeyProfilesSizer->Add(new wxStaticLine(this, -1), 0, wxGROW | wxALL, 5);
}

wxSizer *wxKeyConfigPanel::BuildColumn1()
{
	// FIRST COLUMN: "Groups"
	wxBoxSizer *column1 = new wxBoxSizer(wxVERTICAL);

	if (m_nBuildMode & wxKEYBINDER_USE_TREECTRL) {

		// add the treectrl
		column1->Add(new wxStaticText(this, -1, "Commands:"), 0, wxGROW | wxALL, 5);
		column1->Add(m_pCommandsTree, 1, wxGROW | wxRIGHT | wxLEFT | wxBOTTOM, 5);

	} else {

		// add a combobox + listbox
		column1->Add(new wxStaticText(this, -1, "Categories:"), 0, wxGROW | wxALL, 5);
		column1->Add(m_pCategories, 1, wxGROW | wxRIGHT | wxLEFT | wxBOTTOM, 5);

		column1->Add(new wxStaticText(this, -1, "Commands:"), 0, 
									wxGROW | wxRIGHT | wxLEFT | wxBOTTOM, 5);
		column1->Add(m_pCommandsList, 5, wxGROW | wxRIGHT | wxLEFT | wxBOTTOM, 5);
	}

	return column1;
}

wxSizer *wxKeyConfigPanel::BuildColumn2()
{
	// SECOND COLUMN: "Current keys" + "Press new shortcut key"
	wxBoxSizer *column2 = new wxBoxSizer(wxVERTICAL);
    column2->Add(new wxStaticText(this, -1, "Current shortcuts:"), 0, wxGROW | wxALL, 5);
    column2->Add(m_pBindings, 2, wxGROW | wxRIGHT | wxLEFT, 5);

	wxBoxSizer *removebtns = new wxBoxSizer(wxHORIZONTAL);
    removebtns->Add(m_pRemoveBtn, 1, wxGROW | wxALL, 5);
    removebtns->Add(m_pRemoveAllBtn, 1, wxGROW | wxALL, 5);

	column2->Add(removebtns, 0, wxGROW);
	//column2->Add(new wxStaticLine(this, -1), 0, wxGROW | wxALL, 10);
	column2->Add(new wxStaticText(this, -1, "New shortcut:"), 0, wxGROW | wxALL, 5);
    column2->Add(m_pKeyField, 0, wxGROW | wxLEFT | wxRIGHT, 5);
	column2->Add(new wxStaticText(this, -1, "Currently assigned to:"), 0, wxGROW | wxALL, 5);
	column2->Add(m_pCurrCmdField, 0, wxGROW | wxLEFT | wxRIGHT, 5);
    column2->Add(m_pAssignBtn, 0, wxGROW | wxALL, 5);

	return column2;
}

wxSizer *wxKeyConfigPanel::BuildMain(wxSizer *column1, wxSizer *column2, bool bApplyBtn)
{
	// set up the column container
	wxBoxSizer *cont = new wxBoxSizer(wxHORIZONTAL);
	cont->Add(column1, 4, wxGROW);
	cont->Add(1, 1, 0, wxGROW);
	cont->Add(column2, 4, wxGROW);
	
	// create the main sizer
	wxBoxSizer *main = new wxBoxSizer(wxVERTICAL);
	main->Add(m_pKeyProfilesSizer, 0, wxGROW);
	main->Add(cont, 5, wxGROW);
	main->Add(new wxStaticLine(this, -1), 0, wxGROW | wxALL, 5);
	
	// key description
	main->Add(new wxStaticText(this, -1, "Description:"), 0, wxGROW | wxALL, 5);
    main->Add(m_pDescLabel, 1, wxGROW | wxLEFT | wxRIGHT | wxBOTTOM, 5);
		
	// if required, add Apply and Cancel buttons
	if (bApplyBtn) {

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
	
	return main;
}




// ----------------------------------------------------------------------------
// wxKeyConfigPanel - IMPORT functions
// ----------------------------------------------------------------------------

void wxKeyConfigPanel::ImportMenuBarCmd(wxMenuBar *p, const wxString &rootname)
{
	// do some std things...
	Reset();

	// the wxKeyBinder class can easily generate a tree structure...
	if (IsUsingTreeCtrl()) {
	
		AddRootIfMissing(rootname);

		wxMenuTreeWalker wlkr;
		wlkr.FillTreeCtrl(p, m_pCommandsTree, rootname);

		// expand the root (just for aesthetic/comfort reasons)...
		m_pCommandsTree->Expand(m_pCommandsTree->GetRootItem());

	} else {

		wxMenuComboListWalker wlkr;
		wlkr.FillComboListCtrl(p, m_pCategories);

		// select the first item (just for aesthetic/comfort reasons)...
		m_pCategories->SetSelection(0);

		wxCommandEvent fake;
		OnCategorySelected(fake);
	}
}

void wxKeyConfigPanel::ImportKeyProfileCmd(int n, const wxString &rootname)
{
	// do some std things...
	Reset();

	if (IsUsingTreeCtrl()) {

		AddRootIfMissing(rootname);
		wxTreeItemId rootid = m_pCommandsTree->GetRootItem();

		// scan all the commands of the key binder...
		wxCmdArray *arr = GetProfile(n)->GetArray();
		for (int i=0; i < (int)arr->GetCount(); i++) {

			// to each tree branch attach a wxTreeItemData containing 
			// the ID of the menuitem which it represents...
			wxExTreeItemData *treedata = new wxExTreeItemData(arr->Item(i)->GetId());

			// create the new item in the tree ctrl
			m_pCommandsTree->AppendItem(rootid, arr->Item(i)->GetName(), -1, -1, treedata);
		}

		// expand the root (just for aesthetic/comfort reasons)...
		m_pCommandsTree->Expand(m_pCommandsTree->GetRootItem());

	} else {

		wxCmdArray *arr = GetProfile(n)->GetArray();
		for (int i=0; i < (int)arr->GetCount(); i++) {

			// create a list of items containing as untyped client data
			// (void*) the INT which is their ID...
			m_pCommandsList->Append(arr->Item(i)->GetName(), 
									(void*)(arr->Item(i)->GetId()));
		}

		// in the categories combobox just add a generic title
		m_pCategories->Append("Generic");
	}
}





// ----------------------------------------------------------------------------
// wxKeyConfigPanel - MISCELLANEOUS functions
// ----------------------------------------------------------------------------

void wxKeyConfigPanel::AddProfile(wxKeyProfile *p, bool bImport, const wxString &rootname)
{	
	if (p == NULL) return;

	// add a new profile to the array	
	m_pKeyProfiles->Append(p->GetName(), (void *)p);
	
	if (m_pKeyProfiles->GetCount() == 1) {
		m_pKeyProfiles->SetSelection(0);
		m_nCurrentProf = 0;

		// generate a fake event: SetSelection does not generate it
		wxCommandEvent ev;
		OnProfileSelected(ev);
	}

	// eventually import the commands contained in the given keyprofile
	if (bImport)
		ImportKeyProfileCmd(GetSelProfileIdx(), rootname);
}

void wxKeyConfigPanel::AddProfiles(wxKeyProfileArray &arr)
{
	int old = m_pKeyProfiles->GetCount();
	for (int i=0; i < arr.GetCount(); i++)
		m_pKeyProfiles->Append(arr.Item(i)->GetName(), (void *)arr.Item(i));
	
	if (old == 0) {
		m_pKeyProfiles->SetSelection(0);
		m_nCurrentProf = 0;

		// generate a fake event: SetSelection does not generate it
		wxCommandEvent ev;
		OnProfileSelected(ev);
	}
}

void wxKeyConfigPanel::Reset()
{
	if (IsUsingTreeCtrl()) {

		m_pCommandsTree->DeleteAllItems();

	} else {

		m_pCommandsList->Clear();
		m_pCategories->Clear();
	}

	m_pBindings->Clear();
	m_pDescLabel->SetLabel("");//Clear();
	m_pKeyField->Clear();
}

void wxKeyConfigPanel::AddRootIfMissing(const wxString &rootname)
{
	wxASSERT(IsUsingTreeCtrl());	// this function is treectrl-specific

	if (!m_pCommandsTree->GetRootItem().IsOk()) {

		// I don't know what happens if we try to create the root
		// of the control while there are still some items in the
		// control... even if I don't think some items maybe
		// present in the control if the root is not okay....
		m_pCommandsTree->DeleteAllItems();

		m_pCommandsTree->AddRoot(rootname);
	}
}

wxCmd *wxKeyConfigPanel::GetSelCmd() const
{
	int id;
	
	if (IsUsingTreeCtrl()) {

		wxTreeItemId treeid = GetSelCmdId();
		if (!treeid.IsOk())
			return NULL;

		// each tree item has a wxMenuTreeItemData associated which contains
		// the ID of the menu item which is attached...
		id  = ((wxExTreeItemData *)m_pCommandsTree->GetItemData(treeid))->GetMenuItemId();

	} else {

		int sel = m_pCommandsList->GetSelection();
		if (sel < 0)
			return NULL;

		id = (int)m_pCommandsList->GetClientData(sel);
	}

	return m_kBinder.GetCmd(id);
}

wxTreeItemId wxKeyConfigPanel::GetSelCmdId() const
{
    wxTreeItemId selection = m_pCommandsTree->GetSelection();

	// selection must be valid...
    if (!selection.IsOk())
		return selection;

	// ...and the just selected must be a valid menu item...
	wxTreeItemData *p = m_pCommandsTree->GetItemData(selection);
	if (p == NULL)
		return wxTreeItemId();		// an empty wxTreeItemId is always invalid...

	return selection;
}

wxControl *wxKeyConfigPanel::GetMainCtrl() const
{
	if (m_nBuildMode & wxKEYBINDER_USE_TREECTRL)
		return m_pCommandsTree;
	return m_pCommandsList;
}

wxString wxKeyConfigPanel::GetSelCmdStr() const
{
	wxTreeItemId id = GetSelCmdId();

	// wxEmpyString is returned if there's no valid selection
	if (!id.IsOk())
		return wxEmptyString;

	return m_pCommandsTree->GetItemText(id);
}

bool wxKeyConfigPanel::IsSelectedValidCmd() const
{
	if (IsUsingTreeCtrl())
		return GetSelCmdId().IsOk();
	else
		return m_pCommandsList->GetSelection() >= 0;
}

wxKeyProfileArray wxKeyConfigPanel::GetProfiles() const
{
	wxKeyProfileArray arr;

	// just copy the combobox item's client data (which are wxKeyProfiles)
	// into our array...
	for (int i=0; i<m_pKeyProfiles->GetCount(); i++)		
		arr.Add(GetProfile(i));
	return arr;
}






// ----------------------------------------------------------------------------
// wxKeyConfigPanel - UPDATE functions
// ----------------------------------------------------------------------------

void wxKeyConfigPanel::UpdateButtons()
{
	wxLogDebug("wxKeyConfigPanel::UpdateButtons");
	wxString str;

	// is the remove button to be enabled ?
    m_pRemoveBtn->Enable(m_pBindings->GetSelection() >= 0);
    m_pRemoveAllBtn->Enable(m_pBindings->GetCount() > 0);

	// is the assign button to be enabled ?
	bool b = IsSelectedValidCmd() && m_pKeyField->IsValidKeyComb();
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
	wxKeyProfile *prof = GetSelProfile();
	wxASSERT(prof);

	// just copy the internal key binder used to allow a sort of
	// "undo" feature into the original binder set through the
	// #SetBinder function.
    prof->wxKeyBinder::DeepCopy(&m_kBinder);
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

void wxKeyConfigPanel::OnProfileEditing(wxCommandEvent &ev)
{
	wxASSERT(m_nCurrentProf != -1);

	// change the name of the current profile
	GetProfile(m_nCurrentProf)->SetName(ev.GetString());	
}

void wxKeyConfigPanel::OnTreeCommandSelected(wxTreeEvent &)
{
	wxLogDebug("wxKeyConfigPanel::OnTreeCommandSelected");

	// one of the group in the tree ctrl has been selected:
	// the listbox with the keybindings must be synchronized...
    FillInBindings();

	// if the user selected an invalid/valid item in the tree ctrl,
	// the buttons must be disabled/enabled...
    UpdateButtons();

	// ...and the description must be updated
	UpdateDesc();
}

void wxKeyConfigPanel::OnListCommandSelected(wxCommandEvent &)
{
	wxLogDebug("wxKeyConfigPanel::OnListCommandSelected");

	// like OnTreeCommandSelected
    FillInBindings();
    UpdateButtons();
	UpdateDesc();
}

void wxKeyConfigPanel::OnBindingSelected(wxCommandEvent &)
{
	wxLogDebug("wxKeyConfigPanel::OnBindingSelected");

	// the remove button should be enabled if the
	// element just selected is valid...
	UpdateButtons();
}

void wxKeyConfigPanel::OnCategorySelected(wxCommandEvent &ev)
{
	wxLogDebug("wxKeyConfigPanel::OnCategorySelected");

	// update selection
	int sel = m_pCategories->GetSelection();
	if (sel == -1) return;

	wxExComboItemData *data = (wxExComboItemData*)m_pCategories->GetClientObject(sel);
	wxArrayString &arr = data->GetCmdNameArr();

	// clear the old elements & insert the news
	m_pCommandsList->Clear();
	for (int i=0; i < (int)arr.GetCount(); i++)
		m_pCommandsList->Append(arr.Item(i), (void *)data->GetID(i));

	// select the first
	m_pCommandsList->Select(0);
	OnListCommandSelected(ev);
}

void wxKeyConfigPanel::OnProfileSelected(wxCommandEvent &)
{
	wxLogDebug("wxKeyConfigPanel::OnProfileSelected");

	int selidx = m_pKeyProfiles->GetSelection();
	wxKeyProfile *sel = NULL;

	if (selidx != -1) {

		// update the current selected profile index
		m_nCurrentProf = selidx;
		sel = GetProfile(m_nCurrentProf);

	} else {

		// GetSelProfile() will use our current m_nCurrentProf element...
		sel = GetSelProfile();
	}

	if (!sel) return;

	// copy original bindings to the temporary binder, 
	// which is the one which the user edits (and which
	// will be copied in the original one if the 
	// #ApplyChanges function is called...)
	m_kBinder.DeepCopy(sel);
	
	// call other event handlers
	if (IsUsingTreeCtrl()) {
	
		wxTreeEvent ev;
		OnTreeCommandSelected(ev);

	} else {
	
		wxCommandEvent ev;
		OnListCommandSelected(ev);
	}
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

void wxKeyConfigPanel::OnRemoveAllKey(wxCommandEvent &)
{
	// remove the selected shortcut
	GetSelCmd()->RemoveAllShortcuts();

	// and update the list of the key bindings
	FillInBindings();
	UpdateButtons();
}

void wxKeyConfigPanel::OnAddProfile(wxCommandEvent &)
{
	wxKeyProfile *sel = GetProfile(m_nCurrentProf);//GetSelProfile();
	if (!sel) return;

	wxTextEntryDialog dlg(this, 
		"Input the name of the new profile.\n"
		"The new profile will be initially set to a copy of the last selected profile.",
		"Add new profile");
	dlg.SetValue(sel->GetName());

	if (dlg.ShowModal() == wxID_CANCEL)
		return;

	wxKeyProfile *newprof = new wxKeyProfile(*sel);
	newprof->SetName(dlg.GetValue());
	AddProfile(newprof);

	m_nCurrentProf = m_pKeyProfiles->GetCount()-1;
	m_pKeyProfiles->SetSelection(m_nCurrentProf);
}

void wxKeyConfigPanel::OnRemoveProfile(wxCommandEvent &)
{
	wxASSERT(m_nCurrentProf != -1);

	if (m_pKeyProfiles->GetCount() == 1) {

		wxMessageBox("Cannot delete this profile. It's the only profile available.");
		return;
	}

	m_pKeyProfiles->Delete(m_nCurrentProf);	

	// update the currently selected profile
	m_nCurrentProf--;
	if (m_nCurrentProf < 0) m_nCurrentProf=0;
	wxASSERT(m_nCurrentProf < m_pKeyProfiles->GetCount());

	// keep sync m_nCurrentProf with the currently really selected item
	m_pKeyProfiles->Select(m_nCurrentProf);
}

void wxKeyConfigPanel::OnKeyPressed(wxCommandEvent &)
{
	// if there were no key combinations in the wxKeyMonitorTextCtrl
	// before this event, maybe that now there is one.... this means
	// that the "Assign" could be enabled...
	UpdateButtons();
}






