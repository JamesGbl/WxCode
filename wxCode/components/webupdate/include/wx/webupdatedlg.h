/////////////////////////////////////////////////////////////////////////////
// Name:        webupdatedlg.h
// Purpose:     wxWebUpdateDlg
// Author:      Francesco Montorsi
// Created:     2005/06/28
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_WEBUPDATEDLG_H_
#define _WX_WEBUPDATEDLG_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "webupdatedlg.h"
#endif

// wxWidgets headers
#include "wx/webupdate.h"
#include <wx/image.h>
#include <wx/filesys.h>
#include <wx/fs_mem.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_all.h>


//! The dialog which lets the user update this program.
class wxWebUpdateDlg : public wxDialog 
{
protected:		// pointers to our controls

	wxStaticBitmap* IDWUD_IMAGE;
	wxStaticText* IDWUD_INTRO_TEXT;
	wxStaticText* IDWUD_LOCAL_VERSION;
	wxStaticText* IDWUD_WEB_VERSION;
	wxStaticText* IDWUD_PROGRESS_TEXT;
	wxGauge* IDWUD_GAUGE;
	wxStaticText* IDWUD_TEXT1;
	wxTextCtrl* IDWUD_DOWNLOAD_PATH;
	wxButton* IDWUD_BROWSE;
	wxButton* IDWUD_CANCEL;
	wxButton* IDWUD_OK;
	
private:

	//! Loads the XRC for this dialog and init the control pointers.
	void InitWidgetsFromXRC();

protected:		// event handlers

	void OnDownload(wxCommandEvent &);

public:

	wxWebUpdateDlg::wxWebUpdateDlg(wxWindow *parent, wxWindowID id = -1, 
		const wxString& title = wxT("Update"), const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, 
		const wxString& name = wxT("wxWebUpdateDlg"))
		//: wxDialog(parent, id, title, pos, size, style, name)
		{ InitWidgetsFromXRC();	 }

	virtual ~wxWebUpdateDlg() {}

private:
	DECLARE_CLASS(wxWebUpdateDlg)
	DECLARE_EVENT_TABLE()
};


#endif // _WX_WEBUPDATEDLG_H_

