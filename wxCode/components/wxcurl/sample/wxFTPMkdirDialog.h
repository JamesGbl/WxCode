/*
 *	wxFTPMkdirDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jul 23 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxFTPMkdirDialog.h: interface for the wxFTPMkdirDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXFTPMKDIRDIALOG_H__INCLUDED_
#define _WXFTPMKDIRDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxFTPMkdirDialog : public wxDialog  
{
public:
	wxFTPMkdirDialog(wxWindow* pParent);
	virtual ~wxFTPMkdirDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnMkdir(wxCommandEvent& event);

protected:
	wxTextCtrl*	m_pMkdirCtrl;

	wxTextCtrl*	m_pUserCtrl;
	wxTextCtrl*	m_pPassCtrl;

	wxTextCtrl* m_pResponseCtrl;

	wxString	m_szDefaultMkdir;
	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxFTPMkdirDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXFTPMKDIRDIALOG_H__INCLUDED_
