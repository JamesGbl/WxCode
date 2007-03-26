/*
 *	wxGetFSFTPDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Aug 18 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxGetFSFTPDialog.h: interface for the wxGetFSFTPDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXGETFSFTPDIALOG_H__INCLUDED_
#define _WXGETFSFTPDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxGetFSFTPDialog : public wxDialog  
{
public:
	wxGetFSFTPDialog(wxWindow* pParent);
	virtual ~wxGetFSFTPDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnGetFS(wxCommandEvent& event);

protected:
	wxTextCtrl* m_pTextCtrl;
	wxTextCtrl* m_pHostCtrl;

	wxTextCtrl* m_pUserCtrl;
	wxTextCtrl* m_pPassCtrl;

	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxGetFSFTPDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXGETFSFTPDIALOG_H__INCLUDED_
