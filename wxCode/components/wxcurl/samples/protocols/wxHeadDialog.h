/*
 *	wxHeadDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Jul 14 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxHeadDialog.h: interface for the wxHeadDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXHEADDIALOG_H__INCLUDED_
#define _WXHEADDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxHeadDialog : public wxDialog  
{
public:
	wxHeadDialog(wxWindow* pParent);
	virtual ~wxHeadDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnHead(wxCommandEvent& event);

protected:
	wxTextCtrl* m_pTextCtrl;
	wxTextCtrl* m_pHostCtrl;

	wxTextCtrl* m_pUserCtrl;
	wxTextCtrl* m_pPassCtrl;

	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxHeadDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXHEADDIALOG_H__INCLUDED_
