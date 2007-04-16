/*
 *	wxPostDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Thu Jul 16 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxPostDialog.h: interface for the wxPostDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXPOSTDIALOG_H__INCLUDED_
#define _WXPOSTDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxPostDialog : public wxDialog  
{
public:
	wxPostDialog(wxWindow* pParent);
	virtual ~wxPostDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnPost(wxCommandEvent& event);

protected:
	wxTextCtrl*		m_pDestCtrl;
	wxTextCtrl*		m_pUserCtrl;
	wxTextCtrl*		m_pPassCtrl;

	wxTextCtrl*		m_pDataCtrl;
	wxTextCtrl*		m_pRespCtrl;

	wxString		m_szDefaultDest;
	wxString		m_szDefaultUser;
	wxString		m_szDefaultPass;

private:
	DECLARE_CLASS(wxPostDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXPOSTDIALOG_H__INCLUDED_
