/*
 *	wxPutFTPDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jul 23 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxPutFTPDialog.h: interface for the wxPutFTPDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXPUTFTPDIALOG_H__INCLUDED_
#define _WXPUTFTPDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/gauge.h>

#include <wx/curl/dav.h>

class wxPutFTPDialog : public wxDialog  
{
public:
	wxPutFTPDialog(wxWindow* pParent);
	virtual ~wxPutFTPDialog();

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
	DECLARE_CLASS(wxPutFTPDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXPUTFTPDIALOG_H__INCLUDED_
