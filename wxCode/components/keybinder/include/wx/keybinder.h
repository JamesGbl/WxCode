/////////////////////////////////////////////////////////////////////////////
// Name:        keybinder.h
// Purpose:     Classes for binding keypresses to commands.
// Author:      Aleksandras Gluchovas
// Modified by: Francesco Montorsi (6/1/2004)
// Created:     2000/02/10
// RCS-ID:      $Id: keybinder.h,v 2.0
// Copyright:   (c) Aleksandras Gluchovas
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////




#ifndef __KEYBINDER_G__
#define __KEYBINDER_G__

#ifdef __GNUG__
#pragma interface "keybinder.h"
#endif

// required includes
#include "wx/panel.h"
#include "wx/dialog.h"
#include "wx/listbox.h"
#include "wx/treectrl.h"
#include "wx/menu.h"
#include "wx/button.h"
#include "wx/stattext.h"



// The maximum number of shortcuts associated with each wxCmd.
#define wxCMD_MAX_SHORTCUTS				3

#ifndef wxID_INVALID

	// A value used to represent an invalid id...
	#define wxID_INVALID				-1
#endif




// Identifies a keypress, that is a key binding.
// This class can be used to represent key combinations like:
//   Ctrl+F      Alt+Ctrl+Shift+K      Shift+C     .....
// that is, combinations of [ALT, CTRL, SHIFT]+[ASCII character].
// An array of wxKeyBind is contained in each wxCmd... see wxCmd
// for more info.
//
// NOTE: This class implements a lot of static utilities which 
//       are used by the following classes.
//
class wxKeyBind
{
protected:

	// One of wxACCEL_ALT, wxACCEL_CTRL or wxACCEL_SHIFT flags.
	int m_nFlags;

	// One of the wxWidgets key code which defines the key shortcut.
	int m_nKeyCode;

public:

	wxKeyBind() {
		m_nKeyCode = m_nFlags = -1;
	}

	wxKeyBind(int flags, int keycode) {
		Set(flags, keycode);
	}

	wxKeyBind(const wxString &key) {
		m_nFlags = StringToKeyModifier(key);
		m_nKeyCode = StringToKeyCode(key.AfterLast('+').AfterLast('-'));
	}

	virtual void DeepCopy(const wxKeyBind *p) {
		m_nFlags = p->m_nFlags;
		m_nKeyCode = p->m_nKeyCode;
	}

	virtual ~wxKeyBind() {}



public:

	// Sets the key binding keycode and flags.
	void Set(int flags, int keycode) {
		m_nFlags = flags;
		m_nKeyCode = keycode;
	}

	// Returns TRUE if the given key event matches this key binding.
	bool MatchKey(const wxKeyEvent &key) const;

	// Returns TRUE if the given wxKeyBind object is equivalent to this.
	bool Match(const wxKeyBind &key) const {
		if (m_nFlags == key.m_nFlags && m_nKeyCode == key.m_nKeyCode)
			return TRUE;
		return FALSE;
	}



	// Getters
	// ------------------

	int GetKeyCode() const {
		return m_nKeyCode;
	}

	int GetModifiers() const {
		return m_nFlags;
	}

	// Returns the string which describes this key combination.
	wxString GetStr() const {
		return KeyModifierToString(m_nFlags) + 
			KeyCodeToString(m_nKeyCode);
	}


public:		// static utilities

	static wxString KeyCodeToString(int keyCode);
	static wxString KeyModifierToString(int keyModifier);

	static int StringToKeyCode(const wxString &keyName);
	static int StringToKeyModifier(const wxString &keyModifier);

	static int GetKeyModifier(wxKeyEvent &event);
	static wxString GetKeyStrokeString(wxKeyEvent &event);
};




// Represents one of the commands which are handled by a wxKeyBinder.
// This is an abstract class so it cannot be allocated directly: it
// is used instead as base class for specific type of wxCmd.
//
//
// The command has up to wxCMD_MAX_SHORTCUTS associated key shortcuts.
// Besides, the command has three other properties: a description string,
// a name string and an ID which should be unique for each wxKeyBinder.
//
class wxCmd
{
	// wxKeyBinder must be allowed to call #Exec()
	friend class wxKeyBinder;

protected:

	// the shortcuts
	wxKeyBind m_keyShortcut[wxCMD_MAX_SHORTCUTS];
	int m_nShortcuts;		// num of valid entries of m_keyShortcut[]

	// The name of this command. This should be a short string.
	wxString m_strName;

	// The description associated with this command.
	wxString m_strDescription;

	// The ID which identifies this command.
	int m_nId;

public:

	wxCmd(const wxKeyBind &first, int id, 
		const wxString &name = wxEmptyString,
		const wxString &desc = wxEmptyString) {
		m_strName = name;
		m_strDescription = desc;
		m_nShortcuts = 1;
		m_keyShortcut[0] = first;
		m_nId = id;
	}

	wxCmd(int id = wxID_INVALID, 
		const wxString &name = wxEmptyString,
		const wxString &desc = wxEmptyString) {
		m_strName = name;
		m_strDescription = desc;
		m_nId = id;
		m_nShortcuts = 0;
	}

	virtual void DeepCopy(const wxCmd *cmd) {
		m_strName = cmd->m_strName;
		m_strDescription = cmd->m_strDescription;
		m_nId = cmd->m_nId;
		m_nShortcuts = cmd->m_nShortcuts;

		for (int i=0; i < m_nShortcuts; i++)
			m_keyShortcut[i].DeepCopy(&cmd->m_keyShortcut[i]);
	}

	virtual wxCmd *Clone() const = 0;

	// Destructor
	virtual ~wxCmd() {}


public:

	// Add/Remove functions
	// ----------------------------

	// Adds the given key binding to this command.
	void AddShortcut(const wxKeyBind &key) {
		if (m_nShortcuts >= wxCMD_MAX_SHORTCUTS) return;
		m_keyShortcut[m_nShortcuts++] = key;
		Update();
	}

	// Builds and adds a key binding to this command.
	void AddShortcut(int flags, int keycode) {
		if (m_nShortcuts >= wxCMD_MAX_SHORTCUTS) return;
		wxKeyBind key(flags, keycode);
		AddShortcut(key);
		// update is called by the previous call
	}

	// Builds and adds a key binding to this command using the
	// given key description.
	void AddShortcut(const wxString &key) {
		if (m_nShortcuts >= wxCMD_MAX_SHORTCUTS) return;
		m_keyShortcut[m_nShortcuts++] = wxKeyBind(key);
		Update();
	}

	// Removes the n-th key shortcut associated to this command.
	void RemoveShortcut(int n) {
		for (int i=n; i < m_nShortcuts; i++)	// shift array left
			m_keyShortcut[i] = m_keyShortcut[i+1];
		m_nShortcuts--;
		Update();
	}



	// Miscellaneous
	// ---------------------

	// Returns TRUE if at least one of the key bindings matches
	// the given key event.
	bool MatchKey(const wxKeyEvent &ev) const {
		for (int i=0; i < m_nShortcuts; i++)
			if (m_keyShortcut[i].MatchKey(ev))
				return TRUE;
		return FALSE;
	}

	// Returns TRUE if this command is bind to a wxKeyBind object
	// identic to the given one.
	// If "n" is not NULL, it is set to the index of the entry
	// which contains the given shortcut key (if there is such
	// a shortcut registered in this class); otherwise it is
	// left untouched.
	bool IsBindTo(const wxKeyBind &key, int *n = NULL) const {
		for (int i=0; i < m_nShortcuts; i++) {
			if (m_keyShortcut[i].Match(key)) {
				if (n) *n = i;
				return TRUE;
			}
		}
		return FALSE;
	}



	// Getters
	// ---------------------

	wxKeyBind *GetShortcut(int n)				 { return &m_keyShortcut[n]; }
	const wxKeyBind *GetShortcut(int n)	const	 { return &m_keyShortcut[n]; }

	wxAcceleratorEntry GetAccelerator(int n) const {
		return wxAcceleratorEntry(GetShortcut(n)->GetModifiers(),
			GetShortcut(n)->GetKeyCode(), m_nId);
	}

	int GetId() const {
		return m_nId;
	}

	int GetShortcutCount() const {
		return m_nShortcuts;
	}

	wxString GetDescription() const {
		return m_strDescription;
	}

	wxString GetName() const {
		return m_strName;
	}

	wxArrayString GetShortcutsList() const {
		wxArrayString arr;
		for (int i=0; i < m_nShortcuts; i++)
			arr.Add(m_keyShortcut[i].GetStr());
		return arr;
	}

protected:

	// Called after that an Add/Remove function is called.
	// Default implementation does nothing.
	virtual void Update() {}

	// Executes the command.
	// - "origin" is the object which generated the event that
	//   was recognized as a shortcut key associated to this class.
	// - "client" is the event handler which should receive the
	//   event associated with this command.
	virtual void Exec(wxObject *obj, wxEvtHandler *client) = 0;
};




// Defines a wxObjArray-like array of wxCmd.
// However, we cannot use the WX_DECLARE_OBJARRAY macro
// because wxCmd is an abstract class and thus we need
// to keep simple pointers stored, not the objects themselves.
class wxCmdArray
{
	wxArrayPtrVoid m_arr;

public:
	wxCmdArray() {}
	virtual ~wxCmdArray() { Clear(); }

	void DeepCopy(const wxCmdArray &arr) {
		Clear();
		for (int i=0; i < arr.GetCount(); i++)
			Add(arr.Item(i)->Clone());
	}

	void Add(wxCmd *p)			{ m_arr.Add(p); }
	void Remove(int n);
	void Clear();

	int GetCount() const		{ return m_arr.GetCount(); }
	wxCmd *Item(int n) const	{ return (wxCmd *)m_arr.Item(n); }
};





// Define the wxADD_KEYBINDER_SUPPORT: you should use it inside a protected
// block of the windows which want to use the wxKeyBinder facilities
#ifdef __WXMSW__

	// This is a virtual function used in wxMSW which can be used to allow
	// or disable the message preprocessing and thus the preprocessing of
	// keyshortcuts; to add keybinder support, the preprocessing must be
	// disabled: wxKeyBinder will care of checking the keypresses for
	// eventual hotkeys...
	#define wxADD_KEYBINDER_SUPPORT()										\
		virtual bool MSWShouldPreProcessMessage(WXMSG*) { return FALSE; }

#else

	// the other ports doesn't use MSWShouldPreProcessMessage...
	#define wxADD_KEYBINDER_SUPPORT()			{}

#endif


// This is the real keybinder. This object is an event handler which
// can be #Attach()ed to any wxWindow to intercept the keypresses
// that window receives and then to (if they match a shortcut key
// associated with one of the wxCmd contained) translate them in
// events (the type of the event which is generated depends from
// the type wxCmd which is triggered; see wxCmd::Exec) which are
// sent to the window which is #Attach()ed to this key binder.
//
// This class contains an array of wxCmd and exposes some utilities
// to add/remove/edit them and their wxKeyBind.
// 
// VERY IMPORTANT: to allow the wxKeyBinder class to intercept *all*
//                 the keypresses which are sent to the #Attach()ed 
//                 window, also hotkeys, you must use the 
//                 wxADD_KEYBINDER_SUPPORT macro inside the declaration 
//                 of that window.
//
class wxKeyBinder : public wxEvtHandler
{
protected:

	// The array of wxCmd-derived classes.
	wxCmdArray m_arrCmd;

	// The array of windows attached to this keybinder.
	// These info are very important when deleting the keybinder
	// (which can automatically #Detach() himself).
	// Besides, with this array, something interesting could be
	// done in future (like global shortcuts: command events sent
	// to all attached windows even if the command shortcut comes
	// from one of the attached windows only)...
	wxArrayPtrVoid m_arrAttachedWnd;
	
	//wxWindowList m_lstAttachedWnd;		// I don't like wxList...

protected:

	// The event handler for wxKeyEvents
	void OnChar(wxKeyEvent &event);	


	// Returns the index of the first command with the given ID.
	int FindCmd(int id) const {
		for (int i=0; i < (int)m_arrCmd.GetCount(); i++)
			if (id == m_arrCmd.Item(i)->GetId())
				return i;
		return -1;
	}

	// Returns the index of the first command that contains the
	// given key binding.
	int FindCmdBindTo(const wxKeyBind &key, int *n = NULL) const {
		for (int i=0; i < (int)m_arrCmd.GetCount(); i++)
			if (m_arrCmd.Item(i)->IsBindTo(key, n))
				return i;
		return -1;
	}

	// Returns the index of the first command which matches the
	// given key event.
	int FindMatchingCmd(const wxKeyEvent &key) const {
		for (int i=0; i < (int)m_arrCmd.GetCount(); i++)
			if (m_arrCmd.Item(i)->MatchKey(key))
				return i;
		return -1;
	}


public:

	wxKeyBinder() {}
	virtual ~wxKeyBinder() { DetachAll(); }



	// Deep copies the given object.
	void DeepCopy(const wxKeyBinder *p) {
		m_arrCmd.DeepCopy(p->m_arrCmd);
	}

	// Resets everything associated with this class.
	void Reset() {
		m_arrCmd.Clear();
	}


	// Attaches this class to the given window.
	void Attach(wxWindow *p) {
		if (m_arrAttachedWnd.Index((void *)p) != wxNOT_FOUND)
			return;		// already attached !!!
		p->PushEventHandler(this);
		m_arrAttachedWnd.Add((void*)p);
	}

	// Detaches this event handler from the given window.
	void Detach(wxWindow *p) {
		if (m_arrAttachedWnd.Index((void *)p) == wxNOT_FOUND)
			return;		// this is not attached...
		p->RemoveEventHandler(this);
		m_arrAttachedWnd.Remove((void*)p);
	}

	// Detaches this event handler from all the window it's attached to.
	void DetachAll() {
		for (int i=0; i < (int)m_arrAttachedWnd.GetCount(); i++)
			((wxWindow *)m_arrAttachedWnd.Item(i))->RemoveEventHandler(this);
		m_arrAttachedWnd.Clear();
	}

	// Imports the wxMenuCmd created importing them from
	// the given menu bar.
	void ImportMenuBarCmd(wxMenuBar *p);



	// Add functions
	// -------------------

	void AddCmd(wxCmd *p) {
		m_arrCmd.Add(p);
	}

	void AddShortcut(int id, const wxString &key) {
		wxCmd *p = GetCmd(id);
		if (p) p->AddShortcut(key);
	}

	void AddShortcut(int id, const wxKeyBind &key) {
		wxCmd *p = GetCmd(id);
		if (p) p->AddShortcut(key);
	}



	// Getters
	// -------------------

	wxCmdArray *GetArray()					{ return &m_arrCmd;	}
	const wxCmdArray *GetArray() const		{ return &m_arrCmd; }

	wxCmd *GetMatchingCmd(const wxKeyEvent &key) const {
		int i = FindMatchingCmd(key);
		if (i != -1)
			return m_arrCmd.Item(i);
		return NULL;
	}

	wxCmd *GetCmd(int id) const {
		int i = FindCmd(id);
		if (i != -1)
			return m_arrCmd.Item(i);
		return NULL;
	}

	wxCmd *GetCmdBindTo(const wxString &key, int *n = NULL) const {
		int i = FindCmdBindTo(wxKeyBind(key), n);
		if (i != -1)
			return m_arrCmd.Item(i);
		return NULL;
	}

	wxKeyBind *GetShortcut(int id, int n) const {
		wxCmd *p = GetCmd(id);
		if (p) return p->GetShortcut(n);
		return NULL;
	}

	wxString GetShortcutStr(int id, int n) const {
		wxKeyBind *p = GetShortcut(id, n);
		if (p) return p->GetStr();
		return wxEmptyString;
	}

	wxArrayString GetShortcutsList(int id) const {
		wxCmd *p = GetCmd(id);
		if (p) return p->GetShortcutsList();
		return wxArrayString();
	}

	wxArrayPtrVoid *GetAttachedWndArr()					{ return &m_arrAttachedWnd;	}
	const wxArrayPtrVoid *GetAttachedWndArr() const		{ return &m_arrAttachedWnd;	}

private:
	DECLARE_CLASS(wxKeyBinder)
	DECLARE_EVENT_TABLE()
};







// A special text control where the user keypresses are displayed
// as the contained text.
// This class is used by wxKeyConfigPanel class to allow the user
// to input a shortcut key which can be associated with a wxCmd.
//
// Example: the user presses CTRL+SHIFT+A inside this window;
//          the wxKeyMonitorTextCtrl displays exactly the string
//          "Ctrl+Shift+A"
//
class wxKeyMonitorTextCtrl : public wxTextCtrl
{
public:
	wxKeyMonitorTextCtrl(
		wxWindow* parent,
		wxWindowID id,
		const wxString& value = "",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTE_PROCESS_ENTER,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxTextCtrlNameStr) :
		wxTextCtrl(parent, id, value, pos, size, style, validator, name) {}

	virtual ~wxKeyMonitorTextCtrl() {}

public:

	// Handles the keypresses and changes accordingly
	// the text displayed in text ctrl.
	void OnKey(wxKeyEvent &);

	// Returns TRUE if this window is containing a valid key combination.
	bool IsValidKeyComb() {
		return !GetValue().IsEmpty() && GetValue().Last() != '+';
	}

private:
	DECLARE_CLASS(wxKeyMonitorTextCtrl)
	DECLARE_EVENT_TABLE()
};






// IDs used by wxKeyConfigPanel
#define KEYBINDER_BASEID					30000		// start from this value

#define KEYBINDER_COMMANDS_BOX_ID			KEYBINDER_BASEID+1
#define KEYBINDER_BINDINGS_BOX_ID			KEYBINDER_BASEID+2
#define KEYBINDER_KEY_FIELD_ID				KEYBINDER_BASEID+3
#define KEYBINDER_ASSIGN_KEY_ID				KEYBINDER_BASEID+4
#define KEYBINDER_REMOVE_KEY_ID				KEYBINDER_BASEID+5



// A flexible panel which contains 4 main controls:
//
// - The tree control with all the commands whose shortcut keys can be
//   changed by the given keybinder. The items in the tree control
//   can be added through the #ImportXXXXXX() functions.
//
// - The listbox with all the key combinations associated with the
//   selected command
//
// - A wxKeyMonitorTextCtrl and the buttons which can be used by the
//   user to remove & add new key combinations for the selected command.
//
// - The description field which can be used to view the help string
//   attached to the selected command.
//
// If you want to use this panel you must:
//
// 1) create it as every other wxPanel
//
// 2) if you don't want to pass it directly while constructing the panel,
//    you must use #SetBinder() to attach the key binder containing the 
//    key binds which will be shown as user-editable in this panel
//
// 3) call one of the ImportXXXX() functions in order to create a nice
//    tree control which will be used by the user to edit the key bindings
//
// 4) when the user has finished to edit the key bindings, the
//    #ApplyChanges() function should be called if you want to apply
//    the changes to the keybinder which has been attached with the
//    #SetBinder() function
//
class wxKeyConfigPanel : public wxPanel
{
public:

	// Constructs this panel and links it with the given key binder.
	wxKeyConfigPanel(wxWindow* parent,
		wxKeyBinder *pBinder,			// the key binder to edit (can be NULL)
		bool showApplyBtn = FALSE,
		wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL,
		const wxString& name = "panel");

	virtual ~wxKeyConfigPanel() {}



	// Adds to the tree of user-editable commands, the menuitems 
	// contained in the given menubar.
	//
	// If the root of the tree control doesn't exist (because none
	// of the ImportXXXX functions have been called yet), this
	// function creates it with the "rootname" label.
	virtual void ImportMenuBarCmd(wxMenuBar *, 
		const wxString &rootname = "Menu bar");

	// Adds to the tree of user-editable commands, the raw list of
	// the commmands contained in the current key binder.
	// Instead of a tree structure (like the one generated by
	// #ImportMenuBarCmd), this function creates a raw list of the
	// commands contained in the key binder (which doesn't not contain
	// hierarchical info about the keybindings).
	virtual void ImportKeyBinderCmd(const wxString &rootname = "Commands");



	// Replaces the currently associated key binder with the given one,
	// eventually using it to populate the tree control.
	virtual void SetBinder(wxKeyBinder *p, bool bImport = FALSE, 
		const wxString &rootname = "Commands") {
		
		m_kBinder.Reset();
		m_pBinder = p;
		
		if (m_pBinder) {

			// copy original bindings to the temporary binder, 
			// which is the one which the user edits (and which
			// will be copied in the original one if the 
			// #ApplyChanges function is called...)
			m_kBinder.DeepCopy(m_pBinder);
			if (bImport) ImportKeyBinderCmd(rootname);
		}
	}

	// Returns the currently associated key binder (which is left untouched
	// until the #ApplyChanges function is called).
	virtual wxKeyBinder *GetBinder() {
		return m_pBinder;
	}



	// Copies the internal key binder used to allow a sort of
	// "undo" feature into the original binder set through the
	// #SetBinder function.
	virtual void ApplyChanges();



protected:		// event handlers

	// updates the wxKeyMonitorTextCtrl control...
	void OnIdle(wxIdleEvent &event);

	void OnCommandSelected(wxTreeEvent &event);
	void OnBindingSelected(wxCommandEvent &event);

	void OnApplyChanges(wxCommandEvent &event);
	void OnAssignKey(wxCommandEvent &event);
	void OnRemoveKey(wxCommandEvent &event);

	// notifications received from the wxKeyMonitorTextCtrl
	void OnKeyPressed(wxCommandEvent &event);


protected:

	virtual void UpdateButtons();
	virtual void UpdateDesc();
	virtual void FillInBindings();
	virtual void Reset();
	virtual void AddRootIfMissing(const wxString &rootname);


	// Returns the tree item ID of the currently selected command.
	wxTreeItemId GetSelCmdId();

	// Returns the name of the currently selected command.
	wxString GetSelCmdStr();

	// Returns the selected command.
	wxCmd *GetSelCmd();

protected:

	wxTreeCtrl *m_pCommands;
	wxListBox  *m_pBindings;

	// the special text control used to intercept keypresses
	wxKeyMonitorTextCtrl *m_pKeyField;

	wxButton    *m_pAssignBtn;
	wxButton    *m_pRemoveBtn;

	//wxTextCtrl  *m_pDescLabel;
	wxStaticText *m_pDescLabel;
	wxStaticText *m_pCurrCmdField;

	wxCmd *m_pCurrCmd;

	wxKeyBinder *m_pBinder;   // target, to which short-cuts will be applied
	wxKeyBinder  m_kBinder;   // used while interactively editing shortcuts

private:
	DECLARE_CLASS(wxKeyConfigPanel)
	DECLARE_EVENT_TABLE()
};



#endif // __KEYBINDER_G__

