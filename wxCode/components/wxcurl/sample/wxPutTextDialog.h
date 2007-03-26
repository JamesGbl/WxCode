/*
 *	wxPutTextDialog.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Tue Jun 08 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxPutTextDialog.h: interface for the wxPutTextDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXPUTTEXTDIALOG_H__INCLUDED_
#define _WXPUTTEXTDIALOG_H__INCLUDED_

#include <wx/dialog.h>
#include <wx/textctrl.h>

class wxPutTextDialog : public wxDialog  
{
public:
	wxPutTextDialog(wxWindow* pParent);
	virtual ~wxPutTextDialog();

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
	DECLARE_CLASS(wxPutTextDialog)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};

#endif // _WXPUTTEXTDIALOG_H__INCLUDED_
