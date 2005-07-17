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
#include "wx/download.h"
#include "wx/checkedlistctrl.h"
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/checkbox.h>

// defined later
class wxStaticText;
class wxButton;
class wxTextCtrl;
class wxGauge;


//! The prefix of the static text control which shows the remaining time.
#define wxWUD_TIMETEXT_PREFIX			wxT("Time remaining: ")

//! The prefix of the static text control which shows the current status.
#define wxWUD_SPEEDTEXT_PREFIX			wxT("Download status: ")

//! The possible labels of the wxWUD_OK button.
#define wxWUD_OK_DEFAULT_LABEL			wxT("Download")
#define wxWUD_OK_INSTALL				wxT("Install")

#define wxWUD_CANCEL_DEFAULT_LABEL		wxT("Cancel")
#define wxWUD_CANCEL_DOWNLOAD			wxT("Stop download")
#define wxWUD_CANCEL_INSTALLATION		wxT("Stop installation")


#define wxWUD_SHOWHIDEADV_SHOW			wxT("Show advanced settings >>")
#define wxWUD_SHOWHIDEADV_HIDE			wxT("Hide advanced settings <<")


//! Our wxWUD_GAUGE control accepts values from zero to wxWUD_GAUGE_RANGE.
#define wxWUD_GAUGE_RANGE				1000



//! Returns a string with a short size description for the given number of bytes.
WXDLLIMPEXP_WEBUPDATE wxString wxGetSizeStr(unsigned long bytesize);



//! A simple container of the two basic info which wxWebUpdateDlg needs to know
//! about user *local* packages: the NAME and the local VERSION.
//! This class should not be confused with wxWebUpdatePackage.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateLocalPackage : public wxObject
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


//! The advanced panel of a wxWebUpdateDlg.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateAdvPanel : public wxPanel 
{
protected:		// pointers to our controls

	wxTextCtrl *m_pProxyHostname, *m_pProxyPortNumber,
				*m_pUsername, *m_pPassword;
	wxTextCtrl *m_pDownloadPathTextCtrl;
	wxCheckBox *m_pRemoveFiles;

protected:

	//! Loads the XRC for this dialog and init the control pointers.
	void InitWidgetsFromXRC();


protected:		// event handlers

	void OnBrowse(wxCommandEvent &);

public:

	//! Constructs a wxWebUpdateAdvPanel.
	wxWebUpdateAdvPanel::wxWebUpdateAdvPanel(wxWindow *parent)
		{ m_parent = parent; InitWidgetsFromXRC(); }

	virtual ~wxWebUpdateAdvPanel() 
		{}


	//! Returns the proxy name host.
	wxString GetProxyHostName() const
		{ return m_pProxyHostname->GetValue(); } 
	wxString GetProxyPortNumber() const
		{ return m_pProxyPortNumber->GetValue(); } 

	wxString GetDownloadPath() const
		{ return m_pDownloadPathTextCtrl->GetValue(); } 

	//! Returns TRUE if the user has chosen to remove the downloaded files.
	bool RemoveFiles() const
		{ wxASSERT(m_pRemoveFiles); return m_pRemoveFiles->GetValue(); }


private:
	DECLARE_CLASS(wxWebUpdateAdvPanel)
	DECLARE_EVENT_TABLE()
};



//! The dialog which lets the user update this program.
//! NOTE: it's quite difficult to derive a new class from wxProgressDialog
//!       (which would be more suited rather than the generic wxDialog as
//!       base class) so we emulate some of wxProgressDialog features
//!       even if we are not using it. 
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateDlg : public wxDialog 
{
protected:		// pointers to our controls
	
	wxStaticText *m_pAppNameText, *m_pTimeText, *m_pSpeedText;
	wxButton *m_pOkBtn, *m_pCancelBtn, *m_pShowHideAdvBtn;

#if wxWU_USE_CHECKEDLISTCTRL
	wxCheckedListCtrl *m_pUpdatesList;
#else
	wxListCtrl *m_pUpdatesList;
#endif
	wxGauge *m_pGauge;

	// we store advanced settings here:
	wxWebUpdateAdvPanel *m_pAdvPanel;
	


protected:		// other member variables

	//! The webupdate XML script. This variable becomes valid only once
	//! the first download has been completed.
	wxWebUpdateXMLScript m_xmlScript;

	//! This is a little helper which is set to TRUE during the first download;
	//! that is, it's set to TRUE when we are downloading the XML script.
	bool m_bDownloadingScript;

	//! TRUE if we intentionally called wxDownloadThread::AbortDownload
	//! because user asked us to do so.
	bool m_bUserAborted;

	//! The local packages we are going to handle with this dialog.
	const wxWebUpdateLocalPackage *m_pLocalPackages;
	int m_nLocalPackages;		//!< The number of entries in #m_pLocalPackages.

	//! The packages we have downloaded from the web.
	//! This array is valid only when #m_xmlScript is valid.
	wxWebUpdatePackageArray m_arrUpdatedPackages;

	//! The URI of the XML webupdate script.
	wxString m_strURI;

	//! The name of the application which holds all the local packages
	//! handled by this dialog. This string should not contain version !
	wxString m_strAppName;

	//! The threadhelper we use to download the webupdate script & packages.	
	wxDownloadThread *m_thread;

protected:

	//! Loads the XRC for this dialog and init the control pointers.
	void InitWidgetsFromXRC();

	//! Shows to the user a simple wxMessageBox with the error description
	//! customized for the current application.
	void ShowErrorMsg(const wxString &) const;

	//! Called when the XML script file has been downloaded.
	void OnScriptDownload(const wxString &xmluri);

	//! Rebuilds the list of the packages inside the main wxListCtrl
	//! using the #m_arrUpdatedPackages array. Removes any old content.
	void RebuildPackageList();

	//! Call this function instead of EndModal(wxCANCEL) to abort this dialog.
	//! This function takes care of our wxDownloadThread, infact.
	void AbortDialog();

	//! Shows or hides the child window with the given name and then returns
	//! a pointer to it.
	wxWindow *ShowHideChild(const wxString &name);


protected:		// event handlers

	void OnDownload(wxCommandEvent &);
	void OnCancel(wxCommandEvent &);
	void OnShowFilter(wxCommandEvent &);	
	void OnShowHideAdv(wxCommandEvent &);	
	void OnUpdateUI(wxUpdateUIEvent &);
	void OnIdle(wxIdleEvent &);

	// called by our wxDownloadThread....
	void OnDownloadComplete(wxCommandEvent &);

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
			m_bDownloadingScript=FALSE; m_bUserAborted=FALSE;
			InitWidgetsFromXRC(); }

	virtual ~wxWebUpdateDlg() 
		{ if (m_thread) delete m_thread; }


	//! Shows the dialog and immediately start the download of the webupdate script.
	int ShowModal();

private:
	DECLARE_CLASS(wxWebUpdateDlg)
	DECLARE_EVENT_TABLE()
};


#endif // _WX_WEBUPDATEDLG_H_

