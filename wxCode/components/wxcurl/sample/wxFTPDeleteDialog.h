/*
 *	wxFTPDeleteDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jul 23 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxFTPDeleteDialog.h: interface for the wxFTPDeleteDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXFTPDELETEDIALOG_H__INCLUDED_
#define _WXFTPDELETEDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxFTPDeleteDialog : public wxDialog  
{
public:
	wxFTPDeleteDialog(wxWindow* pParent);
	virtual ~wxFTPDeleteDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnDelete(wxCommandEvent& event);

protected:
	wxTextCtrl*	m_pDeleCtrl;

	wxTextCtrl*	m_pUserCtrl;
	wxTextCtrl*	m_pPassCtrl;

	wxTextCtrl* m_pResponseCtrl;

	wxString	m_szDefaultDele;
	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxFTPDeleteDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXFTPDELETEDIALOG_HINCLUDED_
