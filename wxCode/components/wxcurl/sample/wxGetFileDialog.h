/*
 *	wxGetFileDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Tue Jun 08 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxGetFileDialog.h: interface for the wxGetFileDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXGETFILEDIALOG_H__INCLUDED_
#define _WXGETFILEDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/gauge.h>

#include <wx/curl/dav.h>

class wxGetFileDialog : public wxDialog  
{
public:
	wxGetFileDialog(wxWindow* pParent);
	virtual ~wxGetFileDialog();

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
	DECLARE_CLASS(wxGetFileDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXGETFILEDIALOG_H__INCLUDED_
