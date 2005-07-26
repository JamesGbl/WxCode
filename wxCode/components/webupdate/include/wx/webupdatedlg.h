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
#define wxWUD_SPEEDTEXT_PREFIX			wxT("Status: ")

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


//! The ID of the XML webupdate resource script.
#define wxWUD_XMLSCRIPT_ID				wxT("XMLSCRIPT")



//! Returns a string with a short size description for the given number of bytes.
WXDLLIMPEXP_WEBUPDATE wxString wxGetSizeStr(unsigned long bytesize);


//! The event sent to wxWebUpdater by wxWebUpdateDlg.
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBUPDATE, wxWUD_INIT, -1);
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBUPDATE, wxWUD_DESTROY, -1);


//! The possible values of the wxWebUpdateDlg::m_nStatus variable.
//! This should be used internally by wxWebUpdateDlg only and user
//! should not care about it.
enum wxWebUpdateDlgStatus {
	wxWUDS_UNDEFINED = -1,	
	wxWUDS_DOWNLOADING,
	wxWUDS_COMPUTINGMD5,	
	wxWUDS_INSTALLING
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


	// proxy stuff
	wxString GetProxyHostName() const
		{ return m_pProxyHostname->GetValue(); } 
	wxString GetProxyPortNumber() const
		{ return m_pProxyPortNumber->GetValue(); } 

	// HTTP authentication stuff
	wxString GetUsername() const
		{ return m_pUsername->GetValue(); } 
	wxString GetPassword() const
		{ return m_pPassword->GetValue(); } 

	//! Returns the path chosen by the user for the downloaded file.
	//! This one is initialized to the temporary folder for the current user.
	wxString GetDownloadPath() const
		{ return m_pDownloadPathTextCtrl->GetValue(); } 

	//! Returns TRUE if the user has chosen to remove the downloaded files.
	bool RemoveFiles() const
		{ wxASSERT(m_pRemoveFiles); return m_pRemoveFiles->GetValue(); }


private:
	DECLARE_CLASS(wxWebUpdateAdvPanel)
	DECLARE_EVENT_TABLE()
};


#if wxWU_USE_CHECKEDLISTCTRL
	#define wxWUDLC_BASECLASS wxCheckedListCtrl
#else
	#define wxWUDLC_BASECLASS wxListCtrl
#endif

//! The wxListCtrl which lists the packages that are shown to the user.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateListCtrl : public wxWUDLC_BASECLASS
{
protected:

	//! The local packages we are going to handle with this dialog.
	wxWebUpdateLocalPackageArray m_arrLocalPackages;	

	//! The packages we have downloaded from the web.
	//! This array is valid only when #m_xmlScript is valid.
	wxWebUpdatePackageArray m_arrUpdatedPackages;


protected:		// event handlers

	void OnItemSelect(wxListEvent &ev);

public:

	//! Constructs a wxWebUpdateAdvPanel.
	wxWebUpdateListCtrl::wxWebUpdateListCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
		long style = wxLC_ICON, const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = wxListCtrlNameStr)
		: wxWUDLC_BASECLASS(parent, id, pos, size, style, validator, name)
		{ InitLocalPackages(); }

	virtual ~wxWebUpdateListCtrl() 
		{}


	//! Initializes the local packages for this application asking them to
	//! wxWebUpdater.
	void InitLocalPackages()
		{ m_arrLocalPackages = wxWebUpdater::Get()->GetLocalPackages(); }

	//! Rebuilds the list of the packages inside the main wxListCtrl
	//! using the #m_arrUpdatedPackages array. Removes any old content.
	void RebuildPackageList(bool bShowOnlyOutOfDate = TRUE);


public:		// getters

	//! Returns the array of updated packages parsed from the WebUpdate XML Script.
	wxWebUpdatePackageArray GetRemotePackages() const
		{ return m_arrUpdatedPackages; }

	//! Returns the array of updated packages taken from the wxWebUpdater.
	wxWebUpdateLocalPackageArray GetLocalPackages() const
		{ return m_arrLocalPackages; }

	//! Returns a pointer to the local package with the given name or NULL if such
	//! package could not be found.
	wxWebUpdateLocalPackage &GetLocalPackage(const wxString &name);

	//! Returns the remote package with the given name.
	wxWebUpdatePackage &GetRemotePackage(const wxString &name);

	//! Returns the comma-separed list of the required packages for the n-th package.
	wxString GetRequiredList(int n) const
		{ return m_arrUpdatedPackages[n].GetPrerequisites(); }

public:		// setters

	//! Sets the array of remote packages.
	void SetRemotePackages(const wxWebUpdatePackageArray &arr)
		{ m_arrUpdatedPackages = arr; }

private:
	DECLARE_CLASS(wxWebUpdateListCtrl)
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
	wxGauge *m_pGauge;
	wxTextCtrl *m_pDescription;
	wxCheckBox *m_pShowOnlyOOD;

	// our listctrl
	wxWebUpdateListCtrl *m_pUpdatesList;

	// we store advanced settings here:
	wxWebUpdateAdvPanel *m_pAdvPanel;
	

protected:		// remote-related stuff

	//! The webupdate XML script. This variable becomes valid only once
	//! the first download has been completed.
	wxWebUpdateXMLScript m_xmlScript;

	//! The URI of the XML webupdate script.
	wxString m_strURI;

	//! The thread we use to download the webupdate script & packages.	
	wxDownloadThread *m_dThread;

	//! The thread we use to install the packages.
	wxWebUpdateInstallThread *m_iThread;

protected:		// wxWebUpdateDlg-internals

	//! TRUE if we intentionally called wxDownloadThread::AbortDownload
	//! because user asked us to do so.
	bool m_bUserAborted;

	//! The current status of the dialog. Used by the dialog functions to
	//! let #OnUpdateUI event handler know which label must be set in the
	//! various static text controls of the dialog.
	wxWebUpdateDlgStatus m_nStatus;


protected:		// wxWebUpdateDlg-user stuff

	//! The name of the application which holds all the local packages
	//! handled by this dialog. This string should not contain version !
	wxString m_strAppName;

protected:

	//! Loads the XRC for this dialog and init the control pointers.
	void InitWidgetsFromXRC();

	//! Shows to the user a simple wxMessageBox with the error description
	//! customized for the current application.
	void ShowErrorMsg(const wxString &) const;

	//! Called when the XML script file has been downloaded.
	void OnScriptDownload(const wxString &xmluri);

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
	void OnTextURL(wxTextUrlEvent &);

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
							const wxString &uri)							
		{ m_parent=parent; m_strAppName=appname; m_strURI=uri; 			
			m_bUserAborted=FALSE; m_nStatus=wxWUDS_UNDEFINED;
			InitWidgetsFromXRC(); }

	virtual ~wxWebUpdateDlg() 
		{ if (m_dThread) delete m_dThread;
			if (m_iThread) delete m_iThread;}


public:		// main functions

	//! Shows the dialog and immediately start the download of the webupdate script.
	int ShowModal();

	//! Destroys the dialog.
	bool Destroy();

	//! Call this function instead of EndModal(wxCANCEL) to abort this dialog.
	//! This function takes care of our wxDownloadThread, infact.
	void AbortDialog();

	//! Returns the array of updated packages parsed from the WebUpdate XML Script.
	wxWebUpdatePackageArray GetRemotePackages() const
		{ return m_pUpdatesList->GetRemotePackages(); }

	//! Returns the array of local packages taken from wxWebUpdater.
	wxWebUpdateLocalPackageArray GetLocalPackages() const
		{ return m_pUpdatesList->GetLocalPackages(); }

private:
	DECLARE_CLASS(wxWebUpdateDlg)
	DECLARE_EVENT_TABLE()
};


#endif // _WX_WEBUPDATEDLG_H_

