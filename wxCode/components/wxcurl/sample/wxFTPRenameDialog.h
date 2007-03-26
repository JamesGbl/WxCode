/*
 *	wxFTPRenameDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jul 23 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxFTPRenameDialog.h: interface for the wxFTPRenameDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXFTPRENAMEDIALOG_H__INCLUDED_
#define _WXFTPRENAMEDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxFTPRenameDialog : public wxDialog  
{
public:
	wxFTPRenameDialog(wxWindow* pParent);
	virtual ~wxFTPRenameDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnRename(wxCommandEvent& event);

protected:
	wxTextCtrl*	m_pOrigCtrl;
	wxTextCtrl* m_pNewCtrl;

	wxTextCtrl*	m_pUserCtrl;
	wxTextCtrl*	m_pPassCtrl;

	wxTextCtrl* m_pResponseCtrl;

	wxString	m_szDefaultOrig;
	wxString	m_szDefaultNew;
	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxFTPRenameDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXFTPRENAMEDIALOG_HINCLUDED_
