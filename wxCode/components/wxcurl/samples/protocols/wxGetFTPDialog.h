/*
 *	wxGetFTPDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jul 23 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxGetDialog.h: interface for the wxGetDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXGETFTPDIALOG_H__INCLUDED_
#define _WXGETFTPDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxGetFTPDialog : public wxDialog
{
public:
	wxGetFTPDialog(wxWindow* pParent);
	virtual ~wxGetFTPDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnGetUrl(wxCommandEvent& event);

protected:
	wxTextCtrl* m_pTextCtrl;
	wxTextCtrl* m_pHostCtrl;

	wxTextCtrl* m_pUserCtrl;
	wxTextCtrl* m_pPassCtrl;

	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxGetFTPDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXGETFTPDIALOG_H__INCLUDED_
