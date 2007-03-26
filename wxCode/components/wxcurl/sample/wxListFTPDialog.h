/*
 *	wxListFTPDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Jul 28 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxListFTPDialog.h: interface for the wxListFTPDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXLISTFTPDIALOG_H__INCLUDED_
#define _WXLISTFTPDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxListFTPDialog : public wxDialog  
{
public:
	wxListFTPDialog(wxWindow* pParent);
	virtual ~wxListFTPDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnList(wxCommandEvent& event);

protected:
	wxTextCtrl* m_pTextCtrl;
	wxTextCtrl* m_pHostCtrl;

	wxTextCtrl* m_pUserCtrl;
	wxTextCtrl* m_pPassCtrl;

	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxListFTPDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXLISTFTPDIALOG_H__INCLUDED_
