/*
 *	wxTraceDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Jul 14 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxTraceDialog.h: interface for the wxTraceDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXTRACEDIALOG_H__INCLUDED_
#define _WXTRACEDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxTraceDialog : public wxDialog  
{
public:
	wxTraceDialog(wxWindow* pParent);
	virtual ~wxTraceDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnTrace(wxCommandEvent& event);

protected:
	wxTextCtrl* m_pTextCtrl;
	wxTextCtrl* m_pHostCtrl;

	wxTextCtrl* m_pUserCtrl;
	wxTextCtrl* m_pPassCtrl;

	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

private:
	DECLARE_CLASS(wxTraceDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXTRACEDIALOG_H__INCLUDED_
