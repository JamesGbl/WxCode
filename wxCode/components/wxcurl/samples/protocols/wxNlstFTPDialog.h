/*
 *	wxNlstFTPDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Jul 28 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxNlstFTPDialog.h: interface for the wxNlstFTPDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXNLSTFTPDIALOG_H__INCLUDED_
#define _WXNLSTFTPDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxNlstFTPDialog : public wxDialog  
{
public:
	wxNlstFTPDialog(wxWindow* pParent);
	virtual ~wxNlstFTPDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnNlst(wxCommandEvent& event);

protected:
	wxTextCtrl* m_pTextCtrl;
	wxTextCtrl* m_pHostCtrl;

	wxTextCtrl* m_pUserCtrl;
	wxTextCtrl* m_pPassCtrl;

	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxNlstFTPDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXNLSTFTPDIALOG_H__INCLUDED_
