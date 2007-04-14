/*
 *	wxPutDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Tue May 25 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxPutDialog.h: interface for the wxPutDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXPUTDIALOG_H__INCLUDED_
#define _WXPUTDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/gauge.h>

#include <wx/curl/dav.h>

class wxPutDialog : public wxDialog  
{
public:
	wxPutDialog(wxWindow* pParent);
	virtual ~wxPutDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnBrowse(wxCommandEvent& event);
	void OnPutFile(wxCommandEvent& event);

	void OnProgress(wxCurlUploadEvent& event);

protected:
	wxTextCtrl*	m_pSourceCtrl;
	wxTextCtrl*	m_pDestCtrl;

	wxTextCtrl*	m_pUserCtrl;
	wxTextCtrl*	m_pPassCtrl;

	wxTextCtrl* m_pResponseCtrl;

	wxString	m_szDefaultSource;
	wxString	m_szDefaultDest;
	wxString	m_szDefaultUser;
	wxString	m_szDefaultPass;

	wxGauge*	m_pGauge;

private:
	DECLARE_CLASS(wxPutDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXPUTDIALOG_H__INCLUDED_
