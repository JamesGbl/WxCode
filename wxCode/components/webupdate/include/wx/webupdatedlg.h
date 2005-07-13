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
#include <wx/textctrl.h>
#include <wx/image.h>
#include <wx/filesys.h>
#include <wx/fs_mem.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_all.h>
#include <wx/listctrl.h>
#include <wx/progdlg.h>


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

//! Our wxWUD_GAUGE control accepts values from zero to wxWUD_GAUGE_RANGE.
#define wxWUD_GAUGE_RANGE				1000



//! Returns a string with a short size description for the given number of bytes.
WXDLLIMPEXP_WEBUPDATE wxString wxGetSizeStr(unsigned long bytesize);


// this is the even sent by a wxWebUpdateThread class to the wxEvtHandler
// which is given it in its constructor.
DECLARE_EVENT_TYPE(wxWUT_DOWNLOAD_COMPLETE, -1);


#define wxWUT_BUF_TEMP_SIZE				4096




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


//! The thread helper which downloads the webupdate script and/or packages.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateThread : public wxThread
{
protected:		// these are written by this thread and they must be only read
				// by other classes; making them protected and exposing only
				// getters for these vars we ensure that we do not need to use
				// mutexes...

	//! The moment in the time where the download of the current file started.
	wxDateTime m_dtStart;

	//! How much of the file has been currently downloaded.
	unsigned long m_nCurrentSize;

	//! The size of the download.
	unsigned long m_nFinalSize;

	//! This is the result state of the last download.
	bool m_bSuccess;

protected:		// these are vars protected by mutexes...

	//! TRUE if we are downloading in this moment.
	//! The value of this variable is independent from the current thread status
	//! (i.e. when the thread is running this var can be set to FALSE because we
	//!  have already completed our download)...
	bool m_bDownloading;

	//! The mutex over the #m_bDownloading var.
	wxMutex m_mDownloading;

public:		// to avoid setters/getters (these vars are only read by this thread;
			// they are never written and so they are not protected with mutexes).

	//! The wxEvtHandler which will receive our wxWUT_NOTIFICATION events.
	wxEvtHandler *m_pHandler;

public:		// related to current download

	//! The URI of the resource this thread will download.
	wxString m_strURI;

	//! The name of the file where the downloaded file will be saved.
	wxString m_strOutput;

	//! The name of the resource we are downloading.
	//! This is used by wxWebUpdateDlg only for error messages.
	wxString m_strResName;

	//! The MD5 file checksum.
	//! This is used by wxWebUpdateDlg only for error messages.
	wxString m_strMD5;

public:
	wxWebUpdateThread(wxEvtHandler *dlg = NULL, 
		const wxString &uri = wxEmptyString, 
		const wxString &outfile = wxEmptyString,
		const wxString &resname = wxEmptyString)
		: wxThread(wxTHREAD_JOINABLE), m_pHandler(dlg), m_strURI(uri), 
		  m_strOutput(outfile), m_strResName(resname)
		{ m_bSuccess=FALSE; m_dtStart = wxDateTime::UNow(); }
	virtual ~wxWebUpdateThread() {}

    //! Downloads the file and then sends the wxWUT_NOTIFICATION event
	//! to the #m_pHandler event handler.
	//! Also sets the #m_bSuccess flag before exiting.
    virtual void *Entry();

public:		// getters

	//! Returns TRUE if the last download was successful.
	bool DownloadWasSuccessful() const		
		{ return m_bSuccess; }

	//! Returns the number of milliseconds elapsed from the start of the
	//! current download.
	wxLongLong GetElapsedMSec() const
		{ wxTimeSpan t = wxDateTime::UNow() - m_dtStart; return t.GetMilliseconds(); }

	//! Returns the number of bytes currently downloaded.
	unsigned long GetCurrDownloadedBytes() const
		{ return m_nCurrentSize; }

	//! Returns TRUE if this thread is downloading a file.
	//! We don't need to use m_mDownloading since we are just reading 
	//! the #m_bDownloading var...
	bool IsDownloading() const
		{ return m_bDownloading; }

	//! Returns a string containing the current download speed.
	//! The speed is calculated using #GetCurrDownloadedBytes and #GetElapsedMSec.
	virtual wxString GetDownloadSpeed() const;

	//! Returns a string containing the current time left for this download.
	virtual wxString GetRemainingTime() const;

public:		// miscellaneous

	//! This function must be called only when the thread is not running and 
	void BeginNewDownload() {
		wxASSERT(!IsDownloading());
		wxMutexLocker lock(m_mDownloading);
		m_bDownloading = TRUE;
	}

	//! Aborts the current download.
	void AbortDownload() {
		wxASSERT(IsDownloading());
		wxMutexLocker lock(m_mDownloading);
		m_bDownloading = FALSE;
	}
};


//! The advanced panel of a wxWebUpdateDlg.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateAdvPanel : public wxPanel 
{
protected:		// pointers to our controls

	

protected:

	//! Loads the XRC for this dialog and init the control pointers.
	void InitWidgetsFromXRC();


protected:		// event handlers


public:

	//! Constructs a wxWebUpdateAdvPanel.
	wxWebUpdateAdvPanel::wxWebUpdateAdvPanel(wxWindow *parent)
		{ m_parent = parent; InitWidgetsFromXRC(); }

	virtual ~wxWebUpdateAdvPanel() 
		{}


	//! Returns the proxy name host.
	wxString GetProxyHostName() const;
	wxString GetProxyPostNumber() const;
	wxString GetDownloadPath() const;


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
	wxButton *m_pOk, *m_pCancel;

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

	//! TRUE if we intentionally called wxWebUpdateThread::AbortDownload
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
	wxWebUpdateThread *m_thread;

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
	//! This function takes care of our wxWebUpdateThread, infact.
	void AbortDialog();


protected:		// event handlers

	void OnDownload(wxCommandEvent &);
	void OnBrowse(wxCommandEvent &);
	void OnCancel(wxCommandEvent &);
	void OnShowFilter(wxCommandEvent &);	
	void OnUpdateUI(wxUpdateUIEvent &);

	// called by our wxWebUpdateThread....
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

