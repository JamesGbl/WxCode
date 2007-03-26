/*
 *	wxDeleteDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Jun 02 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxDeleteDialog.h: interface for the wxDeleteDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXDELETEDIALOG_H__INCLUDED_
#define _WXDELETEDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxDeleteDialog : public wxDialog  
{
public:
	wxDeleteDialog(wxWindow* pParent);
	virtual ~wxDeleteDialog();

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
	DECLARE_CLASS(wxDeleteDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXDELETEDIALOG_HINCLUDED_
