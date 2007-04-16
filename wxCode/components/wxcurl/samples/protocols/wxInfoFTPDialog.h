/*
 *	wxInfoFTPDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Jul 28 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxInfoFTPDialog.h: interface for the wxInfoFTPDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXINFOFTPDIALOG_H__INCLUDED_
#define _WXINFOFTPDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxInfoFTPDialog : public wxDialog  
{
public:
	wxInfoFTPDialog(wxWindow* pParent);
	virtual ~wxInfoFTPDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnInfo(wxCommandEvent& event);

protected:
	wxTextCtrl* m_pTextCtrl;
	wxTextCtrl* m_pHostCtrl;

	wxTextCtrl* m_pUserCtrl;
	wxTextCtrl* m_pPassCtrl;

	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxInfoFTPDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXINFOFTPDIALOG_H__INCLUDED_
