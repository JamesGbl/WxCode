/*
 *	wxGetFSDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jun 18 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxGetFSDialog.h: interface for the wxGetFSDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXGETFSDIALOG_H__INCLUDED_
#define _WXGETFSDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxGetFSDialog : public wxDialog  
{
public:
	wxGetFSDialog(wxWindow* pParent);
	virtual ~wxGetFSDialog();

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
	DECLARE_CLASS(wxGetFSDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXGETFSDIALOG_H__INCLUDED_
