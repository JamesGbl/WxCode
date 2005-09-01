/////////////////////////////////////////////////////////////////////////////
// Name:        download.cpp
// Purpose:     wxDownloadThread, wxSizeCacherThread
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
#include "wx/webupdate.h"
#include <wx/wfstream.h>
#include <wx/filename.h>

#if wxUSE_MD5
#include "wx/md5.h"
#endif

#if wxUSE_HTTPENGINE
#include <wx/httpbuilder.h>
#endif

// wxWidgets RTTI
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DOWNLOAD_COMPLETE);
DEFINE_EVENT_TYPE(wxEVT_COMMAND_CACHESIZE_COMPLETE);

// statics
#if wxUSE_HTTPENGINE
wxProxySettings wxDownloadThread::m_proxy;
wxHTTPAuthSettings wxDownloadThread::m_auth;
#endif



// ---------------------
// Globals
// ---------------------

bool wxIsFileProtocol(const wxString &uri)
{
	// file: is the signature of a file URI...
	return uri.StartsWith(wxT("file:"));
}

bool wxIsHTTPProtocol(const wxString &uri)
{
	// http: is the signature of a file URI...
	return uri.StartsWith(wxT("http:"));
}

bool wxIsWebProtocol(const wxString &uri)
{
	// AFAIK the only protocol which does not require an internet connection is file:
	return !wxIsFileProtocol(uri);
}

wxFileName wxGetFileNameFromURI(const wxString &uri)
{
	// remove the file: prefix
	wxString path(uri);
	path.Remove(0, wxString(wxT("file:")).Len());

	// now just build a wxfilename
	return wxFileName(path);			// URIs always use the '/' directory separator
}

wxString wxMakeFileURI(const wxFileName &fn)
{
	wxString path = fn.GetFullPath();

	// in case we are using win32 paths with backslashes...
	// (this must be done also under Unix since even there we could
	//  be forced to handle win32 paths)
	path.Replace(wxT("\\"), wxT("/"));
	
	// now use wxURI as filter
	return wxURI(wxT("file:") + path).BuildURI();
}

wxInputStream *wxGetInputStreamFromURI(const wxString &uri)
{
	wxInputStream *in;

	if (wxIsFileProtocol(uri)) {

		// we can handle file:// protocols ourselves
		wxLogAdvMsg(wxT("wxGetInputStreamFromURI - using wxURL"));
		wxURI u(uri);
		//if (u.GetError() != wxURL_NOERR)
		//	wxDT_ABORT_DOWNLOAD(wxT("wxURL cannot parse this url"));
		in = new wxFileInputStream(u.GetPath());

	} else {

#if wxUSE_HTTPENGINE
		wxLogAdvMsg(wxT("wxGetInputStreamFromURI - using wxHTTPBuilder"));
		wxHTTPBuilder http;
		//http.InitContentTypes(); // Initialise the content types on the page			

		// NOTES: 
		// 1) we use the static proxy & auth settings of wxDownloadThread
		//    because this function is an helper function of wxDownloadThread
		// 2) the proxy & auth settings should have been initialized by the
		//    user of wxDownloadThread
		// 3) the wx*Settings classes contain a boolean switch which allows
		//    wxHTTPBuilder to understand if they are marked as "used" or not;
		//    thus, setting them with the Set*() functions below does not
		//    necessarily mean that they will be used.
		http.SetProxySettings(wxDownloadThread::m_proxy);
		http.SetAuthentication(wxDownloadThread::m_auth);
		
		// just to help debugging....
		if (wxDownloadThread::m_proxy.m_bUseProxy)
			wxLogAdvMsg(wxT("wxGetInputStreamFromURI - using the proxy settings"));
		if (wxDownloadThread::m_auth.m_authType != wxHTTPAuthSettings::wxHTTP_AUTH_NONE)
			wxLogAdvMsg(wxT("wxGetInputStreamFromURI - using the basic authentication settings"));

		in = http.GetInputStream(uri);
#else
		wxLogAdvMsg(wxT("wxGetInputStreamFromURI - using wxURL"));		
		wxURL u(uri);
		if (u.GetError() != wxURL_NOERR) {
			wxLogUsrMsg(wxString(wxT("wxURL cannot parse this url [") + 
									uri + wxT("]")));
			return NULL;
		}
		
		in = u.GetInputStream();
#endif
	}
	
	return in;
}

unsigned long wxGetSizeOfURI(const wxString &uri)
{
	wxInputStream *is = wxGetInputStreamFromURI(uri);
	if (is == NULL)
		return 0;
	
	if (!is->IsOk()) {
		delete is;		// be sure to avoid leaks
		return 0;
	}

	// intercept the 302 HTTP "return code"
#if 0
	wxProtocol &p = u.GetProtocol();
	wxHTTP *http = wxDynamicCast(&p, wxHTTP);
	if (http != NULL && http->GetResponse() == 302) {
		wxLogUsrMsg(wxT("wxGetSizeOfURI - can't get the size of the resource located at [") +
			uri + wxT("] because the request has been redirected... update your URL"));
		return 0;
	}
#endif	

	unsigned long sz = (unsigned long)is->GetSize();
	delete is;

	// see wxHTTP::GetInputStream docs
	if (sz == 0xffffffff)
		sz = 0;
	return sz;
}




// ---------------------
// wxDOWNLOADTHREAD
// ---------------------

// this macro avoids the repetion of a lot of code
#define wxDT_ABORT_DOWNLOAD(msg) {									\
			wxLogUsrMsg(wxT("wxDownloadThread::Entry - ") +			\
				wxString(msg) + wxT(" - DOWNLOAD ABORTED !!!"));	\
			m_bSuccess = FALSE;										\
			m_mStatus.Lock();										\
			m_nStatus = wxDTS_WAITING;								\
			m_mStatus.Unlock();										\
			wxPostEvent(m_pHandler, updatevent);					\
			continue;												\
	}


void *wxDownloadThread::Entry()
{
	// we'll use wxPostEvent to post this event since this is the
	// only thread-safe way to post events !
	wxCommandEvent updatevent(wxEVT_COMMAND_DOWNLOAD_COMPLETE);

	// begin our loop
	while (!TestDestroy()) {

		if (m_nStatus == wxDTS_WAITING) {
			//wxLogDebug(wxT("wxDownloadThread::Entry - sleeping 1sec"));
			m_bReady = TRUE;
			wxThread::Sleep(100);
			continue;
		}

		// reset our variables
		m_nFinalSize = 0;
		m_nCurrentSize = 0;

		// we are starting the download of a file; update our datetime field
		m_dtStart = wxDateTime::UNow();

		wxLogUsrMsg(wxT("wxDownloadThread::Entry - downloading ") + m_strURI);

		// ensure we can build a wxURL from the given URI
		wxInputStream *in = wxGetInputStreamFromURI(m_strURI);

		// check INPUT
		if (in == NULL) {
			// something is wrong with the input URL...
			wxDT_ABORT_DOWNLOAD(wxT("Cannot open the INPUT stream; ")
				wxT("url is [") + m_strURI + wxT("]"));
		}
		if (!in->IsOk()) {
			delete in;
			wxDT_ABORT_DOWNLOAD(wxT("Cannot init the INPUT stream"));
		}
	
		// now work on streams; wx docs says that using wxURL::GetInputStream
		// is deprecated but this is the only way to set advanced info like
		// proxy, user & password...
		wxFileOutputStream out(m_strOutput);
		if (!out.IsOk()) {
			delete in;
			wxDT_ABORT_DOWNLOAD(wxT("Cannot open/init the OUPUT stream [")
								+ m_strOutput + wxT("]"));
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
				wxLogUsrMsg(wxT("wxDownloadThread::Entry - downloaded %d bytes"),
							m_nCurrentSize);
#endif
		}
		
		// we don't need the INPUT stream anymore...
		delete in;

		// if m_nFinalSize is set to zero, then we cannot trust it;
		// we must consider the size of the remote file as unavailable
		// since the wxHTTP protocol does not allow us to get it...
		if (!out.IsOk() || out.GetSize() == 0 ||
			(out.GetSize() != m_nFinalSize && m_nFinalSize != 0))
			wxDT_ABORT_DOWNLOAD(wxT("Output FILE stream size is wrong"));
		
		wxLogUsrMsg(wxT("wxDownloadThread::Entry - completed download of %d bytes"),
						m_nCurrentSize);

		// do we have to compute MD5 ?
#if wxUSE_MD5
		m_mStatus.Lock();
		m_nStatus = wxDTS_COMPUTINGMD5;
		m_mStatus.Unlock();

		// get the md5 checksum for the just downloaded file
		m_strComputedMD5 = wxMD5::GetFileMD5(m_strOutput);
#endif

		// we have successfully download the file
		m_bSuccess = TRUE;		
	
		{
			// go in wait mode
			wxMutexLocker locker(m_mStatus);
			m_nStatus = wxDTS_WAITING;
		}

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



// ---------------------
// wxSIZECACHERTHREAD
// ---------------------

void *wxSizeCacherThread::Entry()
{
	wxLogAdvMsg(wxT("wxSizeCacherThread::Entry - caching file sizes"));
	bool allok = TRUE;

	// be sure to have n null entries in our cache array, where
	// 'n' is the number of URLs whose size must be cached
	m_urlSizes = new wxArrayLong();
	m_urlSizes->Add((long)0, m_urls.GetCount());

	// begin our loop
	for (int i=0; i<(int)m_urls.GetCount() && !TestDestroy(); i++) {
	
		// getting the input stream for the url is the only way
		// to get the size of the resource pointed by that url...
		m_urlSizes->Item(i) = wxGetSizeOfURI(m_urls[i]);
		allok &= (m_urlSizes->Item(i) != 0);
	}

	wxLogAdvMsg(wxT("wxSizeCacherThread::Entry - caching of file sizes completed"));
	return (void *)allok;
}

void wxSizeCacherThread::OnExit()
{
	// we'll use wxPostEvent to post this event since this is the
	// only thread-safe way to post events !
	wxCommandEvent updatevent(wxEVT_COMMAND_CACHESIZE_COMPLETE);
	
	// the event handler must delete the wxArrayLong which we pass to it in the event !
	updatevent.SetClientData(m_urlSizes);
	wxPostEvent(m_pHandler, updatevent);
}

