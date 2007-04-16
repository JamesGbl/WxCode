/*
 *	wxPutFTPTextDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jul 23 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxPutFTPTextDialog.h: interface for the wxPutFTPTextDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXPUTFTPTEXTDIALOG_H__INCLUDED_
#define _WXPUTFTPTEXTDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxPutFTPTextDialog : public wxDialog  
{
public:
	wxPutFTPTextDialog(wxWindow* pParent);
	virtual ~wxPutFTPTextDialog();

	// Event Handlers (These Functions Should NOT be Virtual)
	void OnPut(wxCommandEvent& event);

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
	DECLARE_CLASS(wxPutFTPTextDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXPUTFTPTEXTDIALOG_H__INCLUDED_
