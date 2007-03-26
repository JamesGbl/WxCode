/*
 *	wxPropfindDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Tue Jun 08 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxPropfindDialog.h: interface for the wxPropfindDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXPROPFINDDIALOG_H__INCLUDED_
#define _WXPROPFINDDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxPropfindDialog : public wxDialog  
{
public:
	wxPropfindDialog(wxWindow* pParent);
	virtual ~wxPropfindDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnPropfind(wxCommandEvent& event);

protected:
	wxTextCtrl* m_pTextCtrl;
	wxTextCtrl* m_pHostCtrl;

	wxTextCtrl* m_pUserCtrl;
	wxTextCtrl* m_pPassCtrl;

	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxPropfindDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXPROPFINDDIALOG_H__INCLUDED_
