/*
 *	wxMkcolDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Jun 02 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxMkcolDialog.h: interface for the wxMkcolDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXMKCOLDIALOG_H__INCLUDED_
#define _WXMKCOLDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxMkcolDialog : public wxDialog  
{
public:
	wxMkcolDialog(wxWindow* pParent);
	virtual ~wxMkcolDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnMkcol(wxCommandEvent& event);

protected:
	wxTextCtrl*	m_pMkcolCtrl;

	wxTextCtrl*	m_pUserCtrl;
	wxTextCtrl*	m_pPassCtrl;

	wxTextCtrl* m_pResponseCtrl;

	wxString	m_szDefaultMkcol;
	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxMkcolDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXMKCOLDIALOG_H__INCLUDED_
