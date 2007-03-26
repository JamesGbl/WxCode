/*
 *	wxCURLDlg.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Mon May 24 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxCURLDlg.h: interface for the wxCURLDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXCURLDIALOG_H__INCLUDED_
#define _WXCURLDIALOG_H__INCLUDED_

#include <wx/dialog.h>

class wxCURLDialog : public wxDialog
{
public:
	wxCURLDialog(const wxString& title, const wxPoint& pos, const wxSize& size);
	virtual ~wxCURLDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnQuit(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnAbout(wxCommandEvent& event);

	void OnGet(wxCommandEvent& event);
	void OnGetFile(wxCommandEvent& event);

	void OnPut(wxCommandEvent& event);
	void OnPutData(wxCommandEvent& event);

	void OnPropfind(wxCommandEvent& event);
	void OnGetFS(wxCommandEvent& event);

	void OnDelete(wxCommandEvent& event);
	void OnMkcol(wxCommandEvent& event);

	void OnOptions(wxCommandEvent& event);
	void OnHead(wxCommandEvent& event);
	void OnTrace(wxCommandEvent& event);
	void OnPost(wxCommandEvent& event);

	void OnGetFtp(wxCommandEvent& event);
	void OnGetFileFtp(wxCommandEvent& event);

	void OnPutFtp(wxCommandEvent& event);
	void OnPutDataFtp(wxCommandEvent& event);

	void OnMkdirFtp(wxCommandEvent& event);
	void OnRmdirFtp(wxCommandEvent& event);

	void OnDeleteFtp(wxCommandEvent& event);
	void OnRenameFtp(wxCommandEvent& event);

	void OnListFtp(wxCommandEvent& event);
	void OnNlstFtp(wxCommandEvent& event);
	void OnInfoFtp(wxCommandEvent& event);

	void OnGetFSFtp(wxCommandEvent& event);

private:
	DECLARE_CLASS(wxCURLDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXCURLDIALOG_H__INCLUDED_
