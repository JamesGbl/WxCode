/////////////////////////////////////////////////////////////////////////////
// Name:        download.cpp
// Purpose:     wxDownloadThread
// Author:      Francesco Montorsi
// Created:     2005/06/23
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
	#pragma implementation "download.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
#include "wx/log.h"
#endif

#include "wx/download.h"
#include <wx/wfstream.h>

#if wxDT_USE_MD5
#include "wx/md5.h"
#endif

// wxWidgets RTTI
DEFINE_EVENT_TYPE(wxDT_DOWNLOAD_COMPLETE);

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxDownloadThreadEntryArray);



// ---------------------
// wxDOWNLOADTHREAD
// ---------------------

// this macro avoids the repetion of a lot of code
#define ABORT_DOWNLOAD() {								\
			wxLogDebug(wxT("wxDownloadThread::Entry - DOWNLOAD ABORTED !!!"));		\
			m_bSuccess = FALSE;							\
			m_mStatus.Lock();							\
			m_nStatus = wxDTS_WAITING;					\
			m_mStatus.Unlock();							\
			wxPostEvent(m_pHandler, updatevent);		\
			continue;									\
	}


void *wxDownloadThread::Entry()
{
	// we'll use wxPostEvent to post this event since this is the
	// only thread-safe way to post events !
	wxCommandEvent updatevent(wxDT_DOWNLOAD_COMPLETE);

	// begin our loop
	while (!TestDestroy()) {

		if (m_nStatus == wxDTS_WAITING) {
			//wxLogDebug(wxT("wxDownloadThread::Entry - sleeping 1sec"));
			wxThread::Sleep(100);
			continue;
		}

		// start to download the first entry in the queue
		wxDownloadThreadEntry &dte = m_downloads[0];

		// reset our variables
		m_nFinalSize = 0;
		m_nCurrentSize = 0;

		// we are starting the download of a file; update our datetime field
		m_dtStart = wxDateTime::UNow();

		wxLogDebug(wxT("wxDownloadThread::Entry - downloading ") + dte.m_strURI);

		// ensure we can build a wxURL from the given URI
		wxURL u(dte.m_strURI);
		if (u.GetError() != wxURL_NOERR)
			ABORT_DOWNLOAD();

		// set advanced URL options
		if (!m_strProxyHostname.IsEmpty() && !m_strProxyPort.IsEmpty())
			u.SetProxy(m_strProxyHostname + wxT(":") + m_strProxyPort);
		u.GetProtocol().SetUser(m_strHTTPAuthUsername);
		u.GetProtocol().SetPassword(m_strHTTPAuthPassword);
		
		// now work on streams; wx docs says that using wxURL::GetInputStream
		// is deprecated but this is the only way to set advanced info like
		// proxy, user & password...
		wxFileOutputStream out(dte.m_strOutput);
		wxInputStream *in = u.GetInputStream();
		if (in == NULL)
			ABORT_DOWNLOAD();
		if (!in->IsOk() || !out.IsOk()) {
			delete in;
			ABORT_DOWNLOAD();
		}
		m_nFinalSize = in->GetSize();

		// see wxHTTP docs
		if (m_nFinalSize == 0xffffffff)
			m_nFinalSize = 0;

		// write the downloaded stuff in the output file
		// without using the 
		//          out.Write(*in);
		// command; that would be easier but would not allow
		// the program to stop this thread while downloading
		// the file since the TestDestroy() function would not
		// be called in that way...
		char buf[wxDT_BUF_TEMP_SIZE];
		while (!TestDestroy() && m_nStatus == wxDTS_DOWNLOADING) {
			size_t bytes_read = in->Read(buf, WXSIZEOF(buf)).LastRead();
			if ( !bytes_read )
				break;

			if ( out.Write(buf, bytes_read).LastWrite() != bytes_read )
				break;

			// update our downloaded bytes var
			m_nCurrentSize = out.GetSize();
			
#ifdef __WXDEBUG__
			// do not send too many log messages; send a log message
			// each 20 cycles (i.e. each 20*wxDT_BUF_TEMP_SIZE byte downloaded)
			if ((m_nCurrentSize % (wxDT_BUF_TEMP_SIZE*20)) == 0)
				wxLogDebug(wxT("wxDownloadThread::Entry - downloaded %d bytes"),
						m_nCurrentSize);
#endif
		}
		
		// if m_nFinalSize is set to zero, then we cannot trust it;
		// we must consider the size of the remote file as unavailable
		// since the wxHTTP protocol does not allow us to get it...
		delete in;
		if (!out.IsOk() || out.GetSize() == 0 ||
			(out.GetSize() != m_nFinalSize && m_nFinalSize != 0))
			ABORT_DOWNLOAD();
		
		wxLogDebug(wxT("wxDownloadThread::Entry - completed download of %d bytes"),
						m_nCurrentSize);

		// do we have to compute MD5 ?
#if wxDT_USE_MD5
		m_mStatus.Lock();
		m_nStatus = wxDTS_COMPUTINGMD5;
		m_mStatus.Unlock();

		// get the md5 checksum for the just downloaded file
		m_strComputedMD5 = wxMD5::GetFileMD5(dte.m_strOutput);
#endif

		// we have successfully download the file
		m_bSuccess = TRUE;
		m_mStatus.Lock();
		m_nStatus = wxDTS_WAITING;
		m_mStatus.Unlock();
		wxPostEvent(m_pHandler, updatevent);
	
		m_nFileCount++;

		// we reset our variables here because there is a delay between the
		// wxDownloadThread::BeginNewDownload() calls and the execution of the
		// first statements of this thread...
		m_nCurrentSize = 0;
		m_nFinalSize = 0;
	}

	return (void*)FALSE;
}

wxString wxDownloadThread::GetDownloadSpeed() const
{
	wxASSERT(IsDownloading());
	wxLongLong msec = GetElapsedMSec();
	if (msec <= 0)
		return wxT("0 KB/s");		// avoid division by zero

	wxLongLong nBytesPerMilliSec = wxLongLong(GetCurrDownloadedBytes()) / msec;

	// we don't like bytes per millisecond as measure unit !
	long nKBPerSec = (nBytesPerMilliSec * 1000/1024).ToLong();		// our conversion factor 
	return wxString::Format(wxT("%d KB/s"), nKBPerSec);
}

wxString wxDownloadThread::GetRemainingTime() const
{
	wxASSERT(IsDownloading());
	wxLongLong sec = GetElapsedMSec()/1000;
	if (sec <= 0)
		return wxT("not available");		// avoid division by zero	

	// remaining time is the number of bytes we still need to download
	// divided by our download speed...
	wxLongLong nBytesPerSec = wxLongLong(GetCurrDownloadedBytes()) / sec;
	if (nBytesPerSec <= 0)
		return wxT("not available");		// avoid division by zero
	
	long remsec = (wxLongLong(m_nFinalSize-GetCurrDownloadedBytes())/nBytesPerSec).ToLong();
	if (remsec < 0)
		return wxT("not available");

	if (remsec < 60)
		return wxString::Format(wxT("%d sec"), remsec);	
	else if (remsec < 60*60)
		return wxString::Format(wxT("%d min, %d sec"), remsec/60, remsec%60);
	else if (remsec < 60*60*24)
		return wxString::Format(wxT("%d hours, %d min, %d sec"), 
					remsec/3600, (remsec/60)%60, (remsec/3600)%60);
	else
		return wxT("not available");
}

