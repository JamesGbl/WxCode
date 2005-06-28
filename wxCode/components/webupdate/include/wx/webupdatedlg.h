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
protected:

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

	void InitWidgetsFromXRC() {
		//wxXmlResource::Get()->LoadObject(this, NULL, wxT("wxWebUpdateDlg"), wxT("wxDialog"));
		  //wxXmlResource::Get()->LoadObject(this,NULL,_T("wxWebUpdateDlg"), _T("wxDialog"));
		/*if (!wxXmlResource::Get()->Load(wxT("../src/webupdatedlg.xrc")))
			return;*/

		wxImage::AddHandler(new wxPNGHandler);
		    wxXmlResource::Get()->InitAllHandlers();

    // Load all of the XRC files that will be used. You can put everything
    // into one giant XRC file if you wanted, but then they become more
    // diffcult to manage, and harder to reuse in later projects.
    // The menubar
    wxXmlResource::Get()->Load(wxT("../src/webupdatedlg.xrc"));


	 

	/*	IDWUD_IMAGE = XRCCTRL(*this,"IDWUD_IMAGE",wxStaticBitmap);
		IDWUD_INTRO_TEXT = XRCCTRL(*this,"IDWUD_INTRO_TEXT",wxStaticText);
		IDWUD_LOCAL_VERSION = XRCCTRL(*this,"IDWUD_LOCAL_VERSION",wxStaticText);
		IDWUD_WEB_VERSION = XRCCTRL(*this,"IDWUD_WEB_VERSION",wxStaticText);
		IDWUD_PROGRESS_TEXT = XRCCTRL(*this,"IDWUD_PROGRESS_TEXT",wxStaticText);
		IDWUD_GAUGE = XRCCTRL(*this,"IDWUD_GAUGE",wxGauge);
		IDWUD_TEXT1 = XRCCTRL(*this,"ID_TEXT",wxStaticText);
		IDWUD_DOWNLOAD_PATH = XRCCTRL(*this,"ID_TEXTCTRL",wxTextCtrl);
		IDWUD_BROWSE = XRCCTRL(*this,"ID_BUTTON",wxButton);
		IDWUD_CANCEL = XRCCTRL(*this,"ID_BUTTON",wxButton);*/
		//IDWUD_OK = XRCCTRL(*this,"IDWUD_OK",wxButton);
	}


public:

	wxWebUpdateDlg::wxWebUpdateDlg(wxWindow *parent, wxWindowID id = -1, 
		const wxString& title = wxT("Update"), const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, 
		const wxString& name = wxT("wxWebUpdateDlg"))
		//: wxDialog(parent, id, title, pos, size, style, name)
		{ InitWidgetsFromXRC();	wxXmlResource::Get()->LoadDialog(this, parent, wxT("wxWebUpdateDlg")); }
	virtual ~wxWebUpdateDlg() {}

private:
	DECLARE_CLASS(wxWebUpdateDlg)
};


//void InitXmlResource();

#endif // _WX_WEBUPDATEDLG_H_

