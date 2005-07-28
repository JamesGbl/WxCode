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
#define wxDT_BUF_TEMP_SIZE				2048

//! The size of the queue of a wxDownloadThread.
#define wxDT_QUEUE_SIZE					32


//! The possible values of the wxDownloadThread::m_nStatus variable.
enum wxDownloadThreadStatus {
	wxDTS_WAITING = -1,
	wxDTS_DOWNLOADING,
	wxDTS_COMPUTINGMD5	
};

//! A download entry for wxDownloadThread.
class wxDownloadThreadEntry 
{
public:		// to avoid setters/getters

	//! The URI of the resource this thread will download.
	wxString m_strURI;

	//! The name of the file where the downloaded file will be saved.
	wxString m_strOutput;

	//! The MD5 file checksum.
	wxString m_strMD5;

	//! The name of the resource we are downloading.
	//! This is used by wxWebUpdateDlg only for error messages.
	wxString m_strResName;

	//! The ID of the resource we are downloading.
	//! This is used by wxWebUpdateDlg only for error messages.
	wxString m_strID;

public:
	wxDownloadThreadEntry() {}
	virtual ~wxDownloadThreadEntry() {}
};


// a container of wxDownloadThreadEntry used by wxDownloadThread
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxDownloadThreadEntry, wxDownloadThreadEntryArray, WXDLLIMPEXP_WEBUPDATE);


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
				// getters for these vars ensures that we do not need to use
				// mutexes...

	//! The moment in the time where the download of the current file started.
	wxDateTime m_dtStart;

	//! How much of the file has been currently downloaded.
	unsigned long m_nCurrentSize;

	//! The size of the download.
	unsigned long m_nFinalSize;

	//! This is the result state of the last download.
	bool m_bSuccess;

#if wxDT_USE_MD5

	//! The MD5 checksum for the downloaded file computed when the file has been
	//! completely downloaded.
	//! Use #GetComputedMD5() to get this string.
	wxString m_strComputedMD5;

#endif

	//! The number of files downloaded by this thread.
	int m_nFileCount;

protected:		// these are vars protected by mutexes...

	//! The value of this variable represents the current thread status;
	//! see the #wxDownloadThreadStatus enum for more info.
	wxDownloadThreadStatus m_nStatus;

	//! The mutex over the #m_nStatus var.
	wxMutex m_mStatus;

	//! The current element being downloaded.
	int m_nCurrentIndex;

	//! The mutex over the m_nCurrentIndex var.
	wxMutex m_mIndex;

public:		// to avoid setters/getters (these vars are only read by this thread;
			// they are never written and so they are not protected with mutexes).

	//! The wxEvtHandler which will receive our wxDT_NOTIFICATION events.
	wxEvtHandler *m_pHandler;

	//! Our download queue.
	wxDownloadThreadEntryArray m_downloads;

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
	wxDownloadThread(wxEvtHandler *dlg = NULL)
		: wxThread(wxTHREAD_JOINABLE), m_pHandler(dlg)
		{ m_bSuccess=FALSE; m_dtStart = wxDateTime::UNow(); m_nFileCount=0; 
		  m_nStatus = wxDTS_WAITING; m_nFinalSize = 0; m_nCurrentSize = 0; 
		  m_nCurrentIndex=0; }

	virtual ~wxDownloadThread() {}

    //! Downloads the file and then sends the wxDT_NOTIFICATION event
	//! to the #m_pHandler event handler.
	//! Also sets the #m_bSuccess flag before exiting.
    virtual void *Entry();

public:		// getters

	//! Returns TRUE if the last download was successful.
	bool DownloadWasSuccessful() const		
		{ return m_bSuccess; }

	//! Returns the number of files successfully installed by this thread.
	int GetDownloadCount() const
		{ return m_nFileCount; }

	//! Returns the number of milliseconds elapsed from the start of the
	//! current download.
	wxLongLong GetElapsedMSec() const
		{ wxTimeSpan t = wxDateTime::UNow() - m_dtStart; return t.GetMilliseconds(); }

	//! Returns the number of bytes currently downloaded.
	unsigned long GetCurrDownloadedBytes() const
		{ return m_nCurrentSize; }

	//! Returns a string containing the current download speed.
	//! The speed is calculated using #GetCurrDownloadedBytes and #GetElapsedMSec.
	virtual wxString GetDownloadSpeed() const;

	//! Returns a string containing the current time left for this download.
	virtual wxString GetRemainingTime() const;

#if wxDT_USE_MD5
	//! Returns the MD5 computed for the last downloaded file.
	//! This variable is empty if no files have been downloaded yet.
	wxString GetComputedMD5() const
		{ return m_strComputedMD5; }

	//! Returns TRUE if the MD5 stored in the #m_strMD5 variable
	//! (which should have been set by the user to the MD5 precalculated
	//!  and made available on the website) and the MD5 computed for the
	//! downloaded file match.	
	bool IsMD5Ok() const
		{ return m_downloads[0].m_strMD5 == m_strComputedMD5; }
#endif

public:		// current status
	
	//! Returns TRUE if this thread is downloading a file.
	//! We don't need to use m_mDownloading since we are just reading 
	//! the #m_bDownloading var...
	bool IsDownloading() const
		{ return (IsRunning() && m_nStatus == wxDTS_DOWNLOADING); }

	//! Returns TRUE if this thread is computing the MD5 file checksum.
	bool IsComputingMD5() const
		{ return (IsRunning() && m_nStatus == wxDTS_COMPUTINGMD5); }

	//! Returns TRUE if this thread is running but it's not downloading anything.
	bool IsWaiting() const 
		{ return (IsRunning() && m_nStatus == wxDTS_WAITING); }

	//! Returns the current status of the thread.
	//! please note that this status is completely independent from the
	//! running/paused status of a simple wxThread.
	//! The returned status refers only to the action currently performed
	//! by wxDownloadThread when it's running.
	wxDownloadThreadStatus GetStatus() const
		{ return m_nStatus; }

public:		// miscellaneous

	//! Starts a new download.
	//! This function must be called only when this thread is not downloading
	//! nor computing any MD5...
	void QueueNewDownload(wxDownloadThreadEntry &down) {
		//wxASSERT(!IsDownloading() && !IsComputingMD5());
		m_downloads.Add(down);
		wxMutexLocker lock(m_mStatus);
		m_nStatus = wxDTS_DOWNLOADING;		
	}

	//! Aborts the current download.
	void AbortDownload() {
		wxASSERT(IsDownloading() || IsComputingMD5());
		wxMutexLocker lock(m_mStatus);
		m_nStatus = wxDTS_WAITING;
	}

	//! Returns the current element which is being downloaded.
	wxDownloadThreadEntry &GetCurrent()
		{ return m_downloads[m_nCurrentIndex]; }

	//! Returns the oldest download in the queue (i.e. the first).
	wxDownloadThreadEntry &GetOldest()
		{ return m_downloads[0]; } 

	//! Removes the oldest downloaded file from the queue.
	void RemoveOldestDownload() {
		//wxASSERT(IsPaused());
		wxMutexLocker lock(m_mIndex);
		m_downloads.RemoveAt(0, 1);
		m_nFileCount--;
		if (m_nCurrentIndex > 0) 
			m_nCurrentIndex--;
	}

	//! Cleans all the download queue.
	void CleanQueue() {
		wxASSERT(!IsDownloading() && !IsComputingMD5());
		m_downloads.Clear();
		m_nCurrentIndex = 0;
		wxMutexLocker lock(m_mStatus);
		m_nStatus = wxDTS_WAITING;
		m_nFileCount = 0;
	}
};
/*
//! This threads just retrieve the sizes of the given files and cache
//! them in the 
class WXDLLIMPEXP_WEBUPDATE wxCacheSizeThread : public wxThread
{
protected:		// these are written by this thread and they must be only read
				// by other classes; making them protected and exposing only
				// getters for these vars ensures that we do not need to use
				// mutexes...

	//! The number of files downloaded by this thread.
	int m_nFileCount;

protected:		// these are vars protected by mutexes...

	//! The value of this variable represents the current thread status;
	//! see the #wxDownloadThreadStatus enum for more info.
	wxDownloadThreadStatus m_nStatus;

	//! The mutex over the #m_nStatus var.
	wxMutex m_mStatus;

public:		// to avoid setters/getters (these vars are only read by this thread;
			// they are never written and so they are not protected with mutexes).

	//! The wxEvtHandler which will receive our wxDT_NOTIFICATION events.
	wxEvtHandler *m_pHandler;

public:		// related to current download

	//! The URI of the resource this thread will download.
	wxString m_strURI;

	//! The name of the file where the downloaded file will be saved.
	wxString m_strOutput;

	//! The MD5 file checksum.
	wxString m_strMD5;

	//! The name of the resource we are downloading.
	//! This is used by wxWebUpdateDlg only for error messages.
	wxString m_strResName;

	//! The ID of the resource we are downloading.
	//! This is used by wxWebUpdateDlg only for error messages.
	wxString m_strID;

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
	wxCacheSizeThread(wxEvtHandler *dlg = NULL, 
		const wxString &uri = wxEmptyString, 
		const wxString &outfile = wxEmptyString,
		const wxString &resname = wxEmptyString)
		: wxThread(wxTHREAD_JOINABLE), m_pHandler(dlg), m_strURI(uri), 
		  m_strOutput(outfile), m_strResName(resname)
		{ m_bSuccess=FALSE; m_dtStart = wxDateTime::UNow(); m_nFileCount=0; 
		  m_nStatus = wxDTS_WAITING; m_nFinalSize = 0; m_nCurrentSize = 0; }

	virtual ~wxCacheSizeThread() {}

    //! Downloads the file and then sends the wxDT_NOTIFICATION event
	//! to the #m_pHandler event handler.
	//! Also sets the #m_bSuccess flag before exiting.
    virtual void *Entry();

public:		// getters

	//! Returns TRUE if the last download was successful.
	bool DownloadWasSuccessful() const		
		{ return m_bSuccess; }

	//! Returns the number of files successfully installed by this thread.
	int GetDownloadCount() const
		{ return m_nFileCount; }

	//! Returns the number of milliseconds elapsed from the start of the
	//! current download.
	wxLongLong GetElapsedMSec() const
		{ wxTimeSpan t = wxDateTime::UNow() - m_dtStart; return t.GetMilliseconds(); }

	//! Returns the number of bytes currently downloaded.
	unsigned long GetCurrDownloadedBytes() const
		{ return m_nCurrentSize; }

	//! Returns a string containing the current download speed.
	//! The speed is calculated using #GetCurrDownloadedBytes and #GetElapsedMSec.
	virtual wxString GetDownloadSpeed() const;

	//! Returns a string containing the current time left for this download.
	virtual wxString GetRemainingTime() const;

#if wxDT_USE_MD5
	//! Returns the MD5 computed for the last downloaded file.
	//! This variable is empty if no files have been downloaded yet.
	wxString GetComputedMD5() const
		{ return m_strComputedMD5; }

	//! Returns TRUE if the MD5 stored in the #m_strMD5 variable
	//! (which should have been set by the user to the MD5 precalculated
	//!  and made available on the website) and the MD5 computed for the
	//! downloaded file match.	
	bool IsMD5Ok() const
		{ return m_strMD5 == m_strComputedMD5; }
#endif

public:		// current status
	
	//! Returns TRUE if this thread is downloading a file.
	//! We don't need to use m_mDownloading since we are just reading 
	//! the #m_bDownloading var...
	bool IsDownloading() const
		{ return (IsRunning() && m_nStatus == wxDTS_DOWNLOADING); }

	//! Returns TRUE if this thread is computing the MD5 file checksum.
	bool IsComputingMD5() const
		{ return (IsRunning() && m_nStatus == wxDTS_COMPUTINGMD5); }

	//! Returns TRUE if this thread is running but it's not downloading anything.
	bool IsWaiting() const 
		{ return (IsRunning() && m_nStatus == wxDTS_WAITING); }

	//! Returns the current status of the thread.
	//! please note that this status is completely independent from the
	//! running/paused status of a simple wxThread.
	//! The returned status refers only to the action currently performed
	//! by wxDownloadThread when it's running.
	wxDownloadThreadStatus GetStatus() const
		{ return m_nStatus; }

public:		// miscellaneous

	//! Starts a new download.
	//! This function must be called only when this thread is not downloading
	//! nor computing any MD5...
	void BeginNewDownload() {
		wxASSERT(!IsDownloading() && !IsComputingMD5());
		wxMutexLocker lock(m_mStatus);
		m_nStatus = wxDTS_DOWNLOADING;
	}

	//! Aborts the current download.
	void AbortDownload() {
		wxASSERT(IsDownloading() || IsComputingMD5());
		wxMutexLocker lock(m_mStatus);
		m_nStatus = wxDTS_WAITING;
	}
};
*/
#endif // _WX_DOWNLOAD_H_

