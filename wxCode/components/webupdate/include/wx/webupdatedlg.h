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
#include <wx/stattext.h>
#include <wx/gauge.h>
#include <wx/image.h>
#include <wx/filesys.h>
#include <wx/fs_mem.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_all.h>
#include <wx/listctrl.h>


//! A simple container of the two basic info which wxWebUpdateDlg needs to know
//! about user packages: the NAME and the local VERSION.
class wxWebUpdateLocalPackage : public wxObject
{
public:		// to avoid setters/getters

	//! The name of this package.
	wxString m_strName;

	//! The version of this package.
	wxVersion m_version;

public:
	wxWebUpdateLocalPackage(const wxString &name = wxEmptyString, 
							const wxVersion &ver = wxEmptyVersion)
		: m_strName(name), m_version(ver) {}
	virtual ~wxWebUpdateLocalPackage() {}

private:
	DECLARE_CLASS(wxWebUpdateLocalPackage)
};



//! The dialog which lets the user update this program.
class wxWebUpdateDlg : public wxDialog 
{
protected:		// pointers to our controls
	
	wxStaticText *m_pAppNameText;
	wxListCtrl *m_pUpdatesList;
	wxGauge *m_pGauge;
	wxTextCtrl *m_pDownloadPathTextCtrl;

	//! The packages we are going to handle with this dialog.
	const wxWebUpdateLocalPackage *m_pLocalPackages;
	int m_nLocalPackages;		//!< The number of entries in #m_pLocalPackages.

	//! The URI of the XML webupdate script.
	wxString m_strURI;

	//! The name of the application which holds all the local packages
	//! handled by this dialog. This string should not contain version !
	wxString m_strAppName;

protected:

	//! Loads the XRC for this dialog and init the control pointers.
	void InitWidgetsFromXRC();

	//! 
	//void ;

protected:		// event handlers

	//! Handles clicks on the download button.
	void OnDownload(wxCommandEvent &);

	void OnBrowse(wxCommandEvent &);
	void OnCancel(wxCommandEvent &);

public:

	//! Constructs a wxWebUpdateDlg.
	//! This function does not take so many parameters like a typical
	//! wxWindow or wxDialog does because it uses XRC to set all the
	//! attributes: size, position, id, names...
	//! NOTE: the line
	//!         wxDialog(parent, id, title, pos, size, style, name)
	//! is not required since we are using XRC system
	wxWebUpdateDlg::wxWebUpdateDlg(wxWindow *parent, 
							const wxString &appname,
							const wxString &uri, 
							const wxWebUpdateLocalPackage *arr, 
							int count)
		{ m_parent=parent; m_strAppName=appname; m_strURI=uri; 
			m_pLocalPackages=arr; m_nLocalPackages=count; InitWidgetsFromXRC(); }

	virtual ~wxWebUpdateDlg() {}

private:
	DECLARE_CLASS(wxWebUpdateDlg)
	DECLARE_EVENT_TABLE()
};


#endif // _WX_WEBUPDATEDLG_H_

