/*
 *	wxFTPRmdirDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jul 23 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxFTPRmdirDialog.h: interface for the wxFTPRmdirDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXFTPRMDIRDIALOG_H__INCLUDED_
#define _WXFTPRMDIRDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxFTPRmdirDialog : public wxDialog  
{
public:
	wxFTPRmdirDialog(wxWindow* pParent);
	virtual ~wxFTPRmdirDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnRmdir(wxCommandEvent& event);

protected:
	wxTextCtrl*	m_pRmdirCtrl;

	wxTextCtrl*	m_pUserCtrl;
	wxTextCtrl*	m_pPassCtrl;

	wxTextCtrl* m_pResponseCtrl;

	wxString	m_szDefaultRmdir;
	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxFTPRmdirDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXFTPRMDIRDIALOG_H__INCLUDED_
