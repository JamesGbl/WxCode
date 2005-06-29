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
#include "wx/checkedlistctrl.h"
#include <wx/stattext.h>
#include <wx/gauge.h>
#include <wx/image.h>
#include <wx/filesys.h>
#include <wx/fs_mem.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_all.h>
#include <wx/listctrl.h>
#include <wx/progdlg.h>


#define wxWUT_NOTIFICATION			0xABCDEF
#define wxWUD_TIMETEXT_PREFIX		wxT("Time remaining: ")

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


//! The thread helper which downloads the webupdate script and/or packages.
class wxWebUpdateThread : public wxThreadHelper
{
protected:

	//! This is the result state of the last download.
	bool m_bSuccess;

public:		// to avoid setters/getters

	//! The wxEvtHandler which will receive our wxWUT_NOTIFICATION events.
	wxEvtHandler *m_pHandler;

	//! The URI of the resource this thread will download.
	wxString m_strURI;

	//! The name of the file where the downloaded file will be saved.
	wxString m_strOutput;

	//! The name of the resource we are downloading.
	//! This is used by wxWebUpdateDlg only for error messages.
	wxString m_strResName;

public:
	wxWebUpdateThread(wxEvtHandler *dlg = NULL, 
		const wxString &uri = wxEmptyString, 
		const wxString &outfile = wxEmptyString,
		const wxString &resname = wxEmptyString)
		: m_pHandler(dlg), m_strURI(uri), 
		  m_strOutput(outfile), m_strResName(resname)
		{ m_bSuccess=FALSE; }
	virtual ~wxWebUpdateThread() {}

    //! Downloads the file and then sends the wxWUT_NOTIFICATION event
	//! to the #m_pHandler event handler.
	//! Also sets the #m_bSuccess flag before exiting.
    virtual void *Entry();

	//! Returns TRUE if the last download was successful.
	bool DownloadWasSuccessful() const		{ return m_bSuccess; }
};


//! The dialog which lets the user update this program.
//! NOTE: it's quite difficult to derive a new class from wxProgressDialog
//!       (which would be more suited rather than the generic wxDialog as
//!       base class) so we emulate some of wxProgressDialog features
//!       even if we are not using it. 
class wxWebUpdateDlg : public wxDialog 
{
protected:		// pointers to our controls
	
	wxStaticText *m_pAppNameText, *m_pTimeText, *m_pDownloadStatusText;
#if wxWU_USE_CHECKEDLISTCTRL
	wxCheckedListCtrl *m_pUpdatesList;
#else
	wxListCtrl *m_pUpdatesList;
#endif
	wxGauge *m_pGauge;
	wxTextCtrl *m_pDownloadPathTextCtrl;

protected:		// other member variables

	//! The webupdate XML script. This variable becomes valid only once
	//! the first download has been completed.
	wxWebUpdateXMLScript m_xmlScript;

	//! This is a little helper which is set to TRUE during the first download;
	//! that is, it's set to TRUE when we are downloading the XML script.
	bool m_bDownloadingScript;

	//! The local packages we are going to handle with this dialog.
	const wxWebUpdateLocalPackage *m_pLocalPackages;
	int m_nLocalPackages;		//!< The number of entries in #m_pLocalPackages.

	//! The packages we have downloaded from the web.
	wxWebUpdatePackageArray m_arrUpdatedPackages;

	//! The URI of the XML webupdate script.
	wxString m_strURI;

	//! The name of the application which holds all the local packages
	//! handled by this dialog. This string should not contain version !
	wxString m_strAppName;

	//! The threadhelper we use to download the webupdate script & packages.	
	wxWebUpdateThread m_thread;

protected:

	//! Loads the XRC for this dialog and init the control pointers.
	void InitWidgetsFromXRC();

	//! Shows to the user a simple wxMessageBox with the error description
	//! customized for the current application.
	void ShowErrorMsg(const wxString &) const;

	//! Called when the XML script file has been downloaded.
	void OnScriptDownload(const wxString &xmluri);


protected:		// event handlers

	void OnDownload(wxCommandEvent &);
	void OnBrowse(wxCommandEvent &);
	void OnCancel(wxCommandEvent &);
	void OnShowFilter(wxCommandEvent &);
	void OnDownloadComplete(wxUpdateUIEvent &);

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
			m_pLocalPackages=arr; m_nLocalPackages=count; 			
			m_bDownloadingScript=FALSE; InitWidgetsFromXRC(); }

	virtual ~wxWebUpdateDlg() {}


	//! Shows the dialog and immediately start the download of the webupdate script.
	int ShowModal();

private:
	DECLARE_CLASS(wxWebUpdateDlg)
	DECLARE_EVENT_TABLE()
};


#endif // _WX_WEBUPDATEDLG_H_

