/////////////////////////////////////////////////////////////////////////////
// Name:        menuutils.cpp
// Purpose:     wxMenuCmd, wxMenuWalker, wxMenuTreeWalker, 
//              wxMenuShortcutWalker...
// Author:      Francesco Montorsi
// Modified by:
// Created:     2004/02/19
// RCS-ID:      $Id: menuutils.cpp,v 1.0
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#ifdef __GNUG__
#pragma implementation "menuutils.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// includes
#include "wx/menuutils.h"





// ----------------------------------------------------------------------------
// Global utility functions
// ----------------------------------------------------------------------------

int wxFindMenuItem(wxMenuBar *p, const wxString &str)
{
	int id = wxNOT_FOUND;

	for (int i=0; i < (int)p->GetMenuCount(); i++) {

		id = p->GetMenu(i)->FindItem(str);
		if (id != wxNOT_FOUND)
			break;
	}

	return id;
}





// ----------------------------------------------------------------------------
// wxMenuCmd
// ----------------------------------------------------------------------------

void wxMenuCmd::Update()
{
	wxString str = m_pItem->GetLabel();

#ifdef __WXGTK__
	// on GTK, an optimization in wxMenu::SetText checks
	// if the new label is identic to the old and in this
	// case, it returns without doing nothing...
	// to solve the problem, a space is added or removed 
	// from the label.
	str.Trim();
	if (str == m_pItem->GetLabel())
		str += " ";
#endif

	if (m_nShortcuts <= 0) {

		// no more shortcuts for this menuitem: SetText()
		// will delete the hotkeys associated...
		m_pItem->SetText(str);
		return;
	}

#ifdef __WXMSW__

		// change the accelerator...
	   m_pItem->SetText(str+"\t"+GetShortcut(0)->GetStr());
	   //wxAcceleratorEntry acc = GetAccelerator(0);
	   //m_pItem->SetAccel(&acc);

#elif __WXGTK__

	   // on GTK, the SetAccel() function doesn't have any effect...
	   m_pItem->SetText(str+"\t"+GetShortcut(0)->GetStr());
#endif
}


void wxMenuCmd::Exec(wxObject *origin, wxEvtHandler *client)
{
	wxCommandEvent menuEvent(wxEVT_COMMAND_MENU_SELECTED, GetId()); 

	// set up the event and process it...
	menuEvent.SetEventObject(origin);
	client->AddPendingEvent(menuEvent);//ProcessEvent(menuEvent);
}




// ----------------------------------------------------------------------------
// wxMenuWalker
// ----------------------------------------------------------------------------

void wxMenuWalker::WalkMenuItem(wxMenuBar *p, wxMenuItem *m, void *data)
{
	void *tmp = OnMenuItemWalk(p, m, data);

	if (m->GetSubMenu()) {

		// if this item contains a sub menu, add recursively the menu items
		// of that sub menu... using the cookie from OnMenuItemWalk.
		WalkMenu(p, m->GetSubMenu(), tmp);
	}

	// we can delete the cookie we got form OnMenuItemWalk
	DeleteData(tmp);
}

void wxMenuWalker::WalkMenu(wxMenuBar *p, wxMenu *m, void *data)
{
	for (int i=0; i < (int)m->GetMenuItemCount(); i++) {

		wxMenuItem *pitem = m->GetMenuItems().Item(i)->GetData();
		
		// inform the derived class that we have reached a menu
		// and get the cookie to give on WalkMenuItem...
		void *tmp = OnMenuWalk(p, m, data);

		// skip separators (on wxMSW they are marked as wxITEM_NORMAL
		// but they do have empty labels)...
		if (pitem->GetKind() != wxITEM_SEPARATOR && 
			pitem->GetLabel() != wxEmptyString)
			WalkMenuItem(p, pitem, tmp);

		// the cookie we gave to WalkMenuItem is not useful anymore
		DeleteData(tmp);
	}
}

void wxMenuWalker::Walk(wxMenuBar *p, void *data)
{
	wxASSERT(p);

	for (int i=0; i < (int)p->GetMenuCount(); i++) {

		// create a new tree branch for the i-th menu of this menubar
		wxMenu *m = p->GetMenu(i);
		void *tmp = OnMenuWalk(p, m, data);

		// and fill it...
		WalkMenu(p, m, tmp);
		DeleteData(tmp);
	}
}




// ----------------------------------------------------------------------------
// wxMenuTreeWalker
// ----------------------------------------------------------------------------

void wxMenuTreeWalker::FillTreeBranch(wxMenuBar *p, wxTreeCtrl *ctrl, wxTreeItemId branch)
{
	// these will be used in the recursive functions...
	m_root = branch;
	m_pTreeCtrl = ctrl;

	// be sure that the given tree item is empty...
	m_pTreeCtrl->DeleteChildren(branch);

	// ...start !!!
	Walk(p, &branch);
}

void *wxMenuTreeWalker::OnMenuWalk(wxMenuBar *p, wxMenu *m, void *data)
{
	wxTreeItemId *id = (wxTreeItemId *)data;
	int i;

	// if we receive an invalid tree item ID, we must stop everything...
	// (in fact a NULL value given as DATA in wxMenuTreeWalker function
	// implies the immediate processing stop)...
	if (!id->IsOk())
		return NULL;
	
	// if this is the first level of menus, we must create a new tree item
	if (*id == m_root) {

		// find the index of the given menu
		for (i=0; i < (int)p->GetMenuCount(); i++)
			if (p->GetMenu(i) == m)
				break;
		wxASSERT(i != (int)p->GetMenuCount());
	
		// and append a new tree branch with the appropriate label
		wxTreeItemId newId = m_pTreeCtrl->AppendItem(*id, 
			wxMenuItem::GetLabelFromText(p->GetLabelTop(i)));

		// menu items contained in the given menu must be added
		// to the just created branch
		return new wxTreeItemId(newId);
	}

	// menu items contained in the given menu must be added
	// to this same branch...
	return new wxTreeItemId(*id);
}

void *wxMenuTreeWalker::OnMenuItemWalk(wxMenuBar *, wxMenuItem *m, void *data)
{
	wxTreeItemId *id = (wxTreeItemId *)data;
	if (id->IsOk()) {

		// to each tree branch attach a wxTreeItemData containing 
		// the ID of the menuitem which it represents...
		wxExTreeItemData *treedata = new wxExTreeItemData(m->GetId());

		// create the new item in the tree ctrl
		wxTreeItemId newId = m_pTreeCtrl->AppendItem(*id, 
			m->GetLabel(), -1, -1, treedata);
		
		return new wxTreeItemId(newId);
	}
	
	return NULL;
}

void wxMenuTreeWalker::DeleteData(void *data)
{
	wxTreeItemId *p = (wxTreeItemId *)data;
	wxSAFE_DELETE(p);
}




// ----------------------------------------------------------------------------
// wxMenuShortcutWalker
// ----------------------------------------------------------------------------

void *wxMenuShortcutWalker::OnMenuItemWalk(wxMenuBar *, wxMenuItem *m, void *)
{
	wxASSERT(m);

	// add an entry to the command array
	wxCmd *cmd = new wxMenuCmd(m, m->GetLabel(), m->GetHelp());
	m_pArr->Add(cmd);

	// check for shortcuts
	wxAcceleratorEntry *a = m->GetAccel();		// returns a pointer which we have to delete
	if (a) {

		// this menuitem has an associated accelerator... add an entry
		// to the array of bindings for the relative command...
		cmd->AddShortcut(a->GetFlags(), a->GetKeyCode());
	}

	// cleanup
	wxSAFE_DELETE(a);

	return NULL;
}

void wxMenuShortcutWalker::DeleteData(void *data)
{
	wxASSERT_MSG(data == NULL, 
		"wxMenuShortcutWalker does not use the 'data' parameter");
	data = data;	// to avoid warnings about unused arg
}

