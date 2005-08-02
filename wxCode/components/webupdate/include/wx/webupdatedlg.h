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
#include "wx/installer.h"
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>

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

	void OnItemCheck(wxListEvent &ev);
	void OnItemUncheck(wxListEvent &ev);

	// for event raised by our wxCacheSizerThread....
	void OnCacheSizeComplete(wxCommandEvent &);

	//! Launches our thread for caching the sizes of the packages shown
	//! in this listctrl.	
	//! You need to call #RebuildPackageList after this function.
	void CacheDownloadSizes();

public:

	//! Constructs a wxWebUpdateAdvPanel.
	wxWebUpdateListCtrl::wxWebUpdateListCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
		long style = wxLC_ICON, const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = wxListCtrlNameStr)
		: wxWUDLC_BASECLASS(parent, id, pos, size, style, validator, name)
		{ /*InitLocalPackages();*/ }

	virtual ~wxWebUpdateListCtrl() 
		{}


	//! Initializes the local packages for this application asking them to
	//! wxWebUpdater.
//	void InitLocalPackages()
//		{ m_arrLocalPackages = wxWebUpdater::Get()->GetLocalPackages(); }

	//! Rebuilds the list of the packages inside the main wxListCtrl
	//! using the #m_arrUpdatedPackages array. Removes any old content.
	void RebuildPackageList(bool bShowOnlyOutOfDate = TRUE);


public:		// getters

	//! Returns the array of updated packages parsed from the WebUpdate XML Script.
	wxWebUpdatePackageArray &GetRemotePackages()
		{ return m_arrUpdatedPackages; }

	//! Returns the array of updated packages taken from the wxWebUpdater.
	wxWebUpdateLocalPackageArray &GetLocalPackages()
		{ return m_arrLocalPackages; }

	//! Returns a pointer to the local package with the given name or NULL if such
	//! package could not be found.
	wxWebUpdateLocalPackage &GetLocalPackage(const wxString &name);

	//! Returns the remote package with the given name.
	wxWebUpdatePackage &GetRemotePackage(const wxString &name);

	//! Returns the comma-separed list of the required packages for the n-th package.
	wxString GetRequiredList(int n) const
		{ return m_arrUpdatedPackages[n].GetPrerequisites(); }

	bool IsPackageUp2date(const wxString &name);

public:		// setters

	//! Sets the array of remote packages.
	//! You need to call #RebuildPackageList after this function.
	void SetRemotePackages(const wxWebUpdatePackageArray &arr)
		{ m_arrUpdatedPackages = arr; CacheDownloadSizes(); }
		
	//! Sets the array of local packages.
	void SetLocalPackages(const wxWebUpdateLocalPackageArray &arr)
		{ m_arrLocalPackages = arr; }

private:
	DECLARE_CLASS(wxWebUpdateListCtrl)
	DECLARE_EVENT_TABLE()
};

// a container of wxURLs used by wxSizeCacherThread
WX_DECLARE_USER_EXPORTED_OBJARRAY(bool, wxArrayBool, WXDLLIMPEXP_WEBUPDATE);



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

	//! The number of files downloaded by our #m_dThread.
	//! Even if wxDownloadThread::GetDownloadCount() could be used for
	//! retrieving this info, we keep a separed field here so that we
	//! can remove from the filecount the webupdate XML remote script.
	//! We could also do a check in each UpdateUI event and avoid this
	//! field but it's much more easier to keep this and remove the check ;-)
	int m_nFileCount;
	
protected:		// XML scripts

	//! The webupdate XML script. This variable becomes valid only once
	//! the first download has been completed.
	wxWebUpdateXMLScript m_xmlRemote;
	
	//! The local XML script.
	wxWebUpdateLocalXMLScript m_xmlLocal;

protected:		// remote-related stuff

	//! The thread we use to download the webupdate script & packages.	
	wxDownloadThread *m_dThread;

	//! The thread we use to install the packages.
	wxWebUpdateInstallThread *m_iThread;
	
	//! An array of flags; TRUE for the n-th entry means that the n-th package
	//! (referring to the m_pUpdatesList listctrl package array) has already
	//! been downloaded and is ready for installation.
	wxArrayBool m_bDownloaded;
	
	//! An array of flags; TRUE for the n-th entry means that the n-th package
	//! (referring to the m_pUpdatesList listctrl package array) has already
	//! been installed.
//	wxArrayBool m_bInstalled;
	
	//! The index of the package that we are currently downloading.
	int m_nCurrentIdx;

protected:		// wxWebUpdateDlg-internals

	//! TRUE if we intentionally called wxDownloadThread::AbortDownload
	//! because user asked us to do so.
	bool m_bUserAborted;

	//! The current status of the dialog. Used by the dialog functions to
	//! let #OnUpdateUI event handler know which label must be set in the
	//! various static text controls of the dialog.
	wxWebUpdateDlgStatus m_nStatus;

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

	//! Downloads the first item in the listctrl which is checked and which 
	//! has not been already downloaded.
	//! Returns TRUE if there were checked & not-downloaded packages.
	bool DownloadFirstPackage();
	
	//! 
	bool InstallFirstPackage();
	
	//!
	bool IsReadyForInstallation(int i) const;

protected:		// event handlers

	void OnDownload(wxCommandEvent &);
	void OnCancel(wxCommandEvent &);
	void OnShowFilter(wxCommandEvent &);	
	void OnShowHideAdv(wxCommandEvent &);	
	void OnUpdateUI(wxUpdateUIEvent &);
	void OnIdle(wxIdleEvent &);
	void OnTextURL(wxTextUrlEvent &);

	// for event raised by our wxDownloadThread....
	void OnDownloadComplete(wxCommandEvent &);

	// for event raised by our wxWebUpdateInstallThread...
	void OnInstallationComplete(wxCommandEvent &);

public:

	//! Constructs a wxWebUpdateDlg.
	//! This function does not take so many parameters like a typical
	//! wxWindow or wxDialog does because it uses XRC to set all the
	//! attributes: size, position, id, names...
	//! NOTE: the line
	//!         wxDialog(parent, id, title, pos, size, style, name)
	//! is not required since we are using XRC system
	wxWebUpdateDlg::wxWebUpdateDlg(wxWindow *parent, 
									const wxWebUpdateLocalXMLScript &script)
		{ m_parent=parent; m_xmlLocal=script; 			
			m_bUserAborted=FALSE; m_nStatus=wxWUDS_UNDEFINED;
			m_nCurrentIdx=-1; m_nFileCount=0; InitWidgetsFromXRC(); }

	virtual ~wxWebUpdateDlg() 
		{ if (m_dThread) delete m_dThread;
			if (m_iThread) delete m_iThread;}


public:		// main functions

	//! Shows the dialog and immediately start the download of the webupdate script.
	//int ShowModal();
	bool Show(const bool show);

	//! Destroys the dialog.
	bool Destroy();

	//! Call this function instead of EndModal(wxCANCEL) to abort this dialog.
	//! This function takes care of our wxDownloadThread, infact.
	void AbortDialog();

	//! Returns the array of updated packages parsed from the WebUpdate XML Script.
	wxWebUpdatePackageArray &GetRemotePackages()
		{ return m_pUpdatesList->GetRemotePackages(); }

	//! Returns the array of local packages taken from wxWebUpdater.
	wxWebUpdateLocalPackageArray &GetLocalPackages()
		{ return m_pUpdatesList->GetLocalPackages(); }
	
	//! Sets the local XML script which acts as configuration file for the
	//!	entire dialog (it contains the name of the application to update and
	//! the XRC resource which should be used for this dialog).
	void SetLocalScript(const wxWebUpdateLocalXMLScript &script)
		{ m_xmlLocal = script; }
		
	//! Returns the name of the application to update.
	wxString GetAppName() const
		{ return m_xmlLocal.GetAppName(); }
	
private:
	DECLARE_CLASS(wxWebUpdateDlg)
	DECLARE_EVENT_TABLE()
};


#endif // _WX_WEBUPDATEDLG_H_

