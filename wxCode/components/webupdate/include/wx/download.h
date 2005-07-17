/////////////////////////////////////////////////////////////////////////////
// Name:        download.h
// Purpose:     wxDownloadThread
// Author:      Francesco Montorsi
// Created:     2005/07/14
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_DOWNLOAD_H_
#define _WX_DOWNLOAD_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "download.h"
#endif

// wxWidgets headers
#include "wx/webupdatedef.h"		// for WXDLLIMPEXP_WEBUPDATE macros only
#include <wx/url.h>
#include <wx/datetime.h>


// this is the even sent by a wxDownloadThread class to the wxEvtHandler
// which is given it in its constructor.
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBUPDATE, wxDT_DOWNLOAD_COMPLETE, -1);


//! The size of the temporary buffer used by wxDownloadThread to store the
//! downloaded data.
#define wxDT_BUF_TEMP_SIZE				4096



//! The thread helper which downloads the webupdate script and/or packages.
//! This class is generic and can be used to download anything you need
//! from any URL.
//! It already provides some useful functions like #GetDownloadSpeed() and
//! #GetRemainingTime() which helps you to create dialogs with progress bars
//! and/or time indicators.
class WXDLLIMPEXP_WEBUPDATE wxDownloadThread : public wxThread
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

	//! The wxEvtHandler which will receive our wxDT_NOTIFICATION events.
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

public:		// advanced options

	//! The username used to get the current file.
	//! HTTP Basic authentication is enabled only when this var is not empty.
	wxString m_strHTTPAuthUsername;

	//! The password used to get the current file.
	//! HTTP Basic authentication is enabled only when this var is not empty.
	wxString m_strHTTPAuthPassword;


	//! The hostname of the proxy server.
	wxString m_strProxyHostname;

	//! The port number of the proxy server.
	wxString m_strProxyPort;


public:
	wxDownloadThread(wxEvtHandler *dlg = NULL, 
		const wxString &uri = wxEmptyString, 
		const wxString &outfile = wxEmptyString,
		const wxString &resname = wxEmptyString)
		: wxThread(wxTHREAD_JOINABLE), m_pHandler(dlg), m_strURI(uri), 
		  m_strOutput(outfile), m_strResName(resname)
		{ m_bSuccess=FALSE; m_dtStart = wxDateTime::UNow(); }
	virtual ~wxDownloadThread() {}

    //! Downloads the file and then sends the wxDT_NOTIFICATION event
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
		{ return (IsRunning() && m_bDownloading); }

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

#endif // _WX_DOWNLOAD_H_

