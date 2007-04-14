/*
 *	wxGetFTPFileDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jul 23 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxGetFTPFileDialog.h: interface for the wxGetFTPFileDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXGETFTPFILEDIALOG_H__INCLUDED_
#define _WXGETFTPFILEDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/gauge.h>

#include <wx/curl/dav.h>

class wxGetFTPFileDialog : public wxDialog  
{
public:
	wxGetFTPFileDialog(wxWindow* pParent);
	virtual ~wxGetFTPFileDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnGet(wxCommandEvent& event);
	void OnBrowseDest(wxCommandEvent& event);
	
	void OnProgress(wxCurlDownloadEvent& event);

protected:
	wxTextCtrl*		m_pDestCtrl;
	wxTextCtrl*		m_pSrcCtrl;

	wxString		m_szDefaultDest;
	wxString		m_szDefaultSrc;

	wxTextCtrl*		m_pUserCtrl;
	wxTextCtrl*		m_pPassCtrl;

	wxString		m_szDefaultUser;
	wxString		m_szDefaultPass;

	wxTextCtrl*		m_pRespCtrl;

	wxGauge*		m_pDlGauge;

private:
	DECLARE_CLASS(wxGetFTPFileDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXGETFTPFILEDIALOG_H__INCLUDED_
