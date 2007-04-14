/*
 *	base.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Tue Jun 29 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 *
 *	Licence: wxWidgets Licence
 */

//////////////////////////////////////////////////////////////////////
// Headers
//////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include <stdio.h>
#include <stdarg.h>

#include <wx/curl/base.h>
#include <wx/filename.h>

//////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// C Functions for LibCURL
//////////////////////////////////////////////////////////////////////

extern "C"
{
	int wxcurl_evt_progress_func(void* ptr, double rDlTotal, double rDlNow, 
                                 double rUlTotal, double rUlNow)
	{
        wxCurlBase *curl = wx_static_cast(wxCurlBase*, ptr);
		if(curl)
		{
            if (rUlTotal == 0 || rUlNow == 0)
            {
                /* should be a download event */
                wxCurlDownloadEvent evt(curl->GetId(), curl, rDlTotal, rDlNow, curl->GetCurrentFullURL());
                wxPostEvent(curl->GetEvtHandler(), evt);
            }

            if (rDlTotal == 0 || rDlNow == 0)
            {
                /* should be an upload event */
                wxCurlDownloadEvent evt(curl->GetId(), curl, rUlTotal, rUlNow, curl->GetCurrentFullURL());
                wxPostEvent(curl->GetEvtHandler(), evt);
            }
		}

		return 0;
	}

    int wxcurl_verbose_stream_write(CURL * crlptr, curl_infotype info, 
                                    char * cStrMessage, size_t msgSize, void * buffer)
    {
		wxString szMessage((wxChar*)cStrMessage, msgSize);
		wxString szVerboseMessage;

		wxOutputStream* pBuf = (wxOutputStream*)buffer;

		switch (info)
		{
			case CURLINFO_TEXT:
				szVerboseMessage = wxString(_T("Text: ")) + szMessage + _T("\n");
				break;
			case CURLINFO_HEADER_IN:
				szVerboseMessage = wxString(_T("Header in: ")) + szMessage + _T("\n");
				break;
			case CURLINFO_HEADER_OUT:
				szVerboseMessage = wxString(_T("Header out: ")) + szMessage + _T("\n");
				break;
			case CURLINFO_DATA_IN:
				szVerboseMessage = wxString(_T("Data in: ")) + szMessage + _T("\n");
				break;
			case CURLINFO_DATA_OUT:
				szVerboseMessage = wxString(_T("Data out: ")) + szMessage + _T("\n");
				break;
			case CURLINFO_END:
				szVerboseMessage = wxString(_T("End: ")) + szMessage + _T("\n");
				break;
			case CURLINFO_SSL_DATA_IN:
				szVerboseMessage = wxString(_T("SSL Data in: ")) + szMessage + _T("\n");
				break;
			case CURLINFO_SSL_DATA_OUT:
				szVerboseMessage = wxString(_T("SSL Data out: ")) + szMessage + _T("\n");
				break;
        }
        
        pBuf->Write(szVerboseMessage, szVerboseMessage.Len() * sizeof(wxChar));

        return 0;
    }
    
	size_t wxcurl_header_func(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		size_t iRealSize = size * nmemb;

		wxString* pStr = (wxString*) stream;

		if(pStr)
		{
			(*pStr) += ((wxChar*)(char*)ptr);
		}

		return iRealSize;
	}

	size_t wxcurl_str_write(void* ptr, size_t size, size_t nmemb, void* stream)
	{
		size_t iRealSize = size * nmemb;

		wxString* pStr = (wxString*) stream;

		if(pStr)
		{
			*pStr += (wxChar*)(char*)ptr;
		}

		return iRealSize;
	}

	size_t wxcurl_stream_write(void* ptr, size_t size, size_t nmemb, void* stream)
	{
		size_t iRealSize = size * nmemb;

		wxOutputStream* pBuf = (wxOutputStream*)stream;

		if(pBuf)
		{
			pBuf->Write(ptr, iRealSize);

			return pBuf->LastWrite();
		}

		return 0;
	}

	size_t wxcurl_str_read(void* ptr, size_t size, size_t nmemb, void* stream)
	{
		size_t iRealSize = size * nmemb;
		size_t iRetVal = 0;

		wxString* pStr = (wxString*) stream;

		if(pStr)
		{
			if(pStr->Len() >= iRealSize)
			{
				strncpy((char*)ptr, (const char*)(pStr->c_str()), iRealSize);
				iRetVal = iRealSize;
			}
			else
			{
				strncpy((char*)ptr, (const char*)(pStr->c_str()), pStr->Len());
				iRetVal = pStr->Len();
			}

			*pStr = pStr->Right(pStr->Len() - iRetVal);
		}

		return iRetVal;
	}

	size_t wxcurl_stream_read(void* ptr, size_t size, size_t nmemb, void* stream)
	{
		size_t iRealSize = size * nmemb;

		wxInputStream* pBuf = (wxInputStream*)stream;

		if(pBuf)
		{
			pBuf->Read(ptr, iRealSize);

			return pBuf->LastRead();
		}

		return 0;
	}
}


// base.cpp: implementation of the wxCurlProgressEvent class.
//
//////////////////////////////////////////////////////////////////////

wxTimeSpan wxCurlProgressEvent::GetElapsedTime() const
{
    // NOTE: even if docs say that this info can be retrieved only at the end of the
    //       transfer, this is not really true (as explained by libCURL's authors!)
    //       so that the following is perfectly working:
    double secs;
    m_pCURL->GetInfo(CURLINFO_TOTAL_TIME, &secs);

    // convert from seconds to a wxTimeSpan (which the user can easily convert
    // to a wxString using wxTimeSpan::Format)
    return wxTimeSpan(int(secs/3600.0),     // hours
                      int(secs/60) % 60,    // minutes
                      int(secs) % 60,       // seconds
                      (long)((secs - floor(secs))*1000));                   // milliseconds
}

wxTimeSpan wxCurlProgressEvent::GetEstimatedTime() const
{
    double nBytesPerSec = GetSpeed();
    if (nBytesPerSec == 0)
        return wxTimeSpan(0);       // avoid division by zero

    // compute remaining seconds; here we assume that the current
    // download speed will be constant also in future
    double secs = GetTotalBytes() / nBytesPerSec;

    return wxTimeSpan(int(secs/3600.0),     // hours
                      int(secs/60) % 60,    // minutes
                      int(secs) % 60,       // seconds
                      0);                   // milliseconds
}

wxTimeSpan wxCurlProgressEvent::GetEstimatedRemainingTime() const
{
    wxTimeSpan est = GetEstimatedTime(),
               elapsed = GetElapsedTime();

    if (est.IsLongerThan(elapsed))
        return est - elapsed;
    return wxTimeSpan(0);       // probably est==0 because GetTotalBytes()==0
}

wxString wxCurlProgressEvent::GetHumanReadableSpeed(const wxString &invalid) const
{
    double speed = GetSpeed();
    if (speed == 0)
        return invalid;

    wxULongLong ull((wxULongLong_t)speed);
    return wxFileName::GetHumanReadableSize(ull) + wxT("/s");
}


// base.cpp: implementation of the wxCurlDownloadEvent class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DEFINE_EVENT_TYPE(wxCURL_DOWNLOAD_EVENT);

IMPLEMENT_DYNAMIC_CLASS(wxCurlDownloadEvent, wxEvent);

wxCurlDownloadEvent::wxCurlDownloadEvent()
: wxCurlProgressEvent(-1, wxCURL_DOWNLOAD_EVENT),
  m_rDownloadNow(0.0), m_rDownloadTotal(0.0)
{
}

wxCurlDownloadEvent::wxCurlDownloadEvent(int id, wxCurlBase *originator,
                                         const double& rDownloadTotal, const double& rDownloadNow, 
                                         const wxString& szURL /*= wxEmptyString*/)
: wxCurlProgressEvent(id, wxCURL_DOWNLOAD_EVENT, originator, szURL),
  m_rDownloadTotal(rDownloadTotal), m_rDownloadNow(rDownloadNow)
{
}

wxCurlDownloadEvent::wxCurlDownloadEvent(const wxCurlDownloadEvent& event)
: wxCurlProgressEvent(event)
{
	m_rDownloadNow = event.m_rDownloadNow;
	m_rDownloadTotal = event.m_rDownloadTotal;
}

double wxCurlDownloadEvent::GetSpeed() const
{
    // NOTE: even if docs say that this info can be retrieved only at the end of the
    //       transfer, this is not really true (as explained by libCURL's authors!)
    //       so that the following is perfectly working:

    double ret;
    m_pCURL->GetInfo(CURLINFO_SPEED_DOWNLOAD, &ret);
    return ret;
}



// base.cpp: implementation of the wxCurlUploadEvent class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DEFINE_EVENT_TYPE(wxCURL_UPLOAD_EVENT);

IMPLEMENT_DYNAMIC_CLASS(wxCurlUploadEvent, wxEvent);

wxCurlUploadEvent::wxCurlUploadEvent()
: wxCurlProgressEvent(-1, wxCURL_UPLOAD_EVENT),
  m_rUploadNow(0.0), m_rUploadTotal(0.0)
{
}

wxCurlUploadEvent::wxCurlUploadEvent(int id, wxCurlBase *originator,
                                         const double& rUploadTotal, const double& rUploadNow, 
                                         const wxString& szURL /*= wxEmptyString*/)
: wxCurlProgressEvent(id, wxCURL_UPLOAD_EVENT, originator, szURL),
  m_rUploadTotal(rUploadTotal), m_rUploadNow(rUploadNow)
{
}

wxCurlUploadEvent::wxCurlUploadEvent(const wxCurlUploadEvent& event)
: wxCurlProgressEvent(event)
{
    m_rUploadNow = event.m_rUploadNow;
    m_rUploadTotal = event.m_rUploadTotal;
}

double wxCurlUploadEvent::GetSpeed() const
{
    // NOTE: even if docs say that this info can be retrieved only at the end of the
    //       transfer, this is not really true (as explained by libCURL's authors!)
    //       so that the following is perfectly working:

    double ret;
    m_pCURL->GetInfo(CURLINFO_SPEED_UPLOAD, &ret);
    return ret;
}



// wxCurlBase.cpp: implementation of the wxCurlBeginPerformEvent class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DEFINE_EVENT_TYPE(wxCURL_BEGIN_PERFORM_EVENT);

IMPLEMENT_DYNAMIC_CLASS(wxCurlBeginPerformEvent, wxEvent);

wxCurlBeginPerformEvent::wxCurlBeginPerformEvent()
: wxEvent(-1, wxCURL_BEGIN_PERFORM_EVENT)
{
}

wxCurlBeginPerformEvent::wxCurlBeginPerformEvent(int id, const wxString& szURL)
: wxEvent(id, wxCURL_BEGIN_PERFORM_EVENT),
  m_szURL(szURL)
{
}

wxCurlBeginPerformEvent::wxCurlBeginPerformEvent(const wxCurlBeginPerformEvent& event)
: wxEvent(event)
{
	m_szURL = event.m_szURL;
}

// wxCurlBase.cpp: implementation of the wxCurlEndPerformEvent class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DEFINE_EVENT_TYPE(wxCURL_END_PERFORM_EVENT);

IMPLEMENT_DYNAMIC_CLASS(wxCurlEndPerformEvent, wxEvent);

wxCurlEndPerformEvent::wxCurlEndPerformEvent()
: wxEvent(-1, wxCURL_END_PERFORM_EVENT),
  m_iResponseCode(0)
{
}

wxCurlEndPerformEvent::wxCurlEndPerformEvent(int id, const wxString& szURL, const long& iResponseCode)
: wxEvent(id, wxCURL_END_PERFORM_EVENT),
  m_szURL(szURL),
  m_iResponseCode(iResponseCode)
{
}

wxCurlEndPerformEvent::wxCurlEndPerformEvent(const wxCurlEndPerformEvent& event)
: wxEvent(event)
{
	m_szURL = event.m_szURL;
	m_iResponseCode = event.m_iResponseCode;
}

// wxCurlBase.cpp: implementation of the wxCurlBase class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxCurlBase::wxCurlBase(const wxString& szURL /*= wxEmptyString*/,
                       const wxString& szUserName /*= wxEmptyString*/,
                       const wxString& szPassword /*= wxEmptyString*/,
                       wxEvtHandler* pEvtHandler /*= NULL*/,
                       int id /*= wxID_ANY*/,
                       long flags /*=wxCURL_DEFAULT_FLAGS*/)
: m_szBaseURL(szURL), m_szCurrFullURL(szURL), m_szUsername(szUserName), 
  m_szPassword(szPassword),
  m_iHostPort(-1), m_iResponseCode(-1),
  m_bUseProxy(false), m_iProxyPort(-1),
  m_pCURL(NULL), m_pHeaders(NULL), 
  m_pEvtHandler(pEvtHandler), m_nId(id),
  m_nFlags(flags),
  m_bVerbose(false)
{
	m_szErrorBuffer[0] = '\0';
    m_progressCallback = wxcurl_evt_progress_func;
    m_progressData = this;

	InitHandle();
}

wxCurlBase::~wxCurlBase()
{
	CleanupHandle();
	ResetHeaders();
}

//////////////////////////////////////////////////////////////////////
// LibCURL Abstraction Methods
//////////////////////////////////////////////////////////////////////

typedef int (*func_T)(void);
bool wxCurlBase::SetOpt(CURLoption option, ...)
{
	va_list arg;

	func_T param_func = (func_T)0;
	long param_long = 0;
	void *param_obj = NULL;
	curl_off_t param_offset = 0;

	va_start(arg, option);

	CURLcode res = CURLE_OK;

	// This code stolen from easy.c from LibCURL - It is needed to ensure that
	// types are maintained.
	if(option < CURLOPTTYPE_OBJECTPOINT) {
		/* This is a LONG type */
		param_long = va_arg(arg, long);
		res = curl_easy_setopt(m_pCURL, option, param_long);
	}
	else if(option < CURLOPTTYPE_FUNCTIONPOINT) {
		/* This is a object pointer type */
		param_obj = va_arg(arg, void *);
		res = curl_easy_setopt(m_pCURL, option, param_obj);
	}
	else if(option < CURLOPTTYPE_OFF_T) {
		/* This is a function pointer type */
		param_func = va_arg(arg, func_T );
		res = curl_easy_setopt(m_pCURL, option, param_func);
	} else {
		/* This is a curl_off_t type */
		param_offset = va_arg(arg, curl_off_t);
		res = curl_easy_setopt(m_pCURL, option, param_offset);
	}

	va_end(arg);

    DumpErrorIfNeed(res);
	return (res == CURLE_OK);
}

bool wxCurlBase::GetInfo(CURLINFO info, ...) const
{
	va_list arg;
	void* pParam;

	va_start(arg, info);
	pParam = va_arg(arg, void*);

	CURLcode res = CURLE_OK;

	res = curl_easy_getinfo(m_pCURL, info, pParam);

    DumpErrorIfNeed(res);
	return (res == CURLE_OK);
}

bool wxCurlBase::Perform()
{
	CURLcode res = CURLE_OK;

	if((m_nFlags & wxCURL_SEND_BEGINEND_EVENTS) && m_pEvtHandler)
	{
		wxCurlBeginPerformEvent bgnEvent(m_nId, m_szCurrFullURL);

		wxPostEvent(m_pEvtHandler, bgnEvent);
	}

	res = curl_easy_perform(m_pCURL);

	if((m_nFlags & wxCURL_SEND_BEGINEND_EVENTS) && m_pEvtHandler)
	{
		wxCurlEndPerformEvent endEvent(m_nId, m_szCurrFullURL, m_iResponseCode);

		wxPostEvent(m_pEvtHandler, endEvent);
	}

    DumpErrorIfNeed(res);
	return (res == CURLE_OK);
}

bool wxCurlBase::InitHandle()
{
	if(m_pCURL)
		return false;

	m_pCURL = curl_easy_init();

	return (m_pCURL != NULL);
}

bool wxCurlBase::ReInitHandle()
{
	CleanupHandle();
	return InitHandle();
}

bool wxCurlBase::CleanupHandle()
{
	if(m_pCURL)
	{
		curl_easy_cleanup(m_pCURL);
		m_pCURL = NULL;
	}

	return true;
}

bool wxCurlBase::ResetHandle()
{
	curl_easy_reset(m_pCURL);

	return true;
}

void wxCurlBase::DumpErrorIfNeed(CURLcode error) const
{
    if (m_bVerbose && error != CURLE_OK)
    {
        wxString errStr = wxT("wxCURL: ") + wxString(curl_easy_strerror(error), wxConvLocal);

        wxCurlBase *us = wx_const_cast(wxCurlBase*, this);
        us->m_mosVerbose.Write(errStr.c_str(), errStr.Len());
    }
}

//////////////////////////////////////////////////////////////////////
// Member Data Access Methods
//////////////////////////////////////////////////////////////////////

bool wxCurlBase::SetEvtHandler(wxEvtHandler* pEvtHandler, int id)
{
	m_pEvtHandler = pEvtHandler;
    m_nId = id;

	return true;
}

wxEvtHandler* wxCurlBase::GetEvtHandler() const
{
	return m_pEvtHandler;
}

int wxCurlBase::GetId() const
{
    return m_nId;
}

void wxCurlBase::SetFlags(long flags)
{
	m_nFlags = flags;
}

long wxCurlBase::GetFlags() const
{
	return m_nFlags;
}

void wxCurlBase::SetBaseURL(const wxString& szBaseURL)
{
	m_szBaseURL = szBaseURL;
}

wxString wxCurlBase::GetBaseURL() const
{
	return m_szBaseURL;
}

void wxCurlBase::SetPort(const long& iPort)
{
	m_iHostPort = iPort;
}

long wxCurlBase::GetPort() const
{
	return m_iHostPort;
}

void wxCurlBase::SetUsername(const wxString& szUsername)
{
	m_szUsername = szUsername;
}

wxString wxCurlBase::GetUsername() const
{
	return m_szUsername;
}

void wxCurlBase::SetPassword(const wxString& szPassword)
{
	m_szPassword = szPassword;
}

wxString wxCurlBase::GetPassword() const
{
	return m_szPassword;
}

wxString wxCurlBase::GetResponseHeader() const
{
	return m_szResponseHeader;
}

wxString wxCurlBase::GetResponseBody() const
{
	return m_szResponseBody;
}

long wxCurlBase::GetResponseCode() const
{
	return m_iResponseCode;
}

wxString wxCurlBase::GetErrorString() const
{
	return wxString((const wxChar*)m_szErrorBuffer);
}

void wxCurlBase::UseProxy(const bool& bUseProxy)
{
	m_bUseProxy = bUseProxy;
}

bool wxCurlBase::UseProxy() const
{
	return m_bUseProxy;
}

void wxCurlBase::SetProxyHost(const wxString& szProxyHost)
{
	m_szProxyHost = szProxyHost;
}

wxString wxCurlBase::GetProxyHost() const
{
	return m_szProxyHost;
}

void wxCurlBase::SetProxyUsername(const wxString& szProxyUsername)
{
	m_szProxyUsername = szProxyUsername;
}

wxString wxCurlBase::GetProxyUsername() const
{
	return m_szProxyUsername;
}

void wxCurlBase::SetProxyPassword(const wxString& szProxyPassword)
{
	m_szProxyPassword = szProxyPassword;
}

wxString wxCurlBase::GetProxyPassword() const
{
	return m_szProxyPassword;
}

void wxCurlBase::SetProxyPort(const long& iProxyPort)
{
	m_iProxyPort = iProxyPort;
}

long wxCurlBase::GetProxyPort() const
{
	return m_iProxyPort;
}

void wxCurlBase::SetVerbose(const bool& bVerbose)
{
	m_bVerbose = bVerbose;
}

bool wxCurlBase::IsVerbose() const
{
	return m_bVerbose;
}

bool wxCurlBase::GetVerboseStream(wxOutputStream& destStream) const
{
	if(m_bVerbose)
	{
		if(m_mosVerbose.IsOk())
		{
			char* pBuffer = new char[m_mosVerbose.GetSize()];

			if(pBuffer)
			{
				m_mosVerbose.CopyTo(pBuffer, m_mosVerbose.GetSize());

				destStream.Write(pBuffer, m_mosVerbose.GetSize());

                delete [] pBuffer;

				return destStream.IsOk();
			}
		}
	}

	return false;
}

bool wxCurlBase::GetVerboseString(wxString& szStream) const
{
	if(m_bVerbose)
	{
		char* pBuff;
		wxString szBuff;

		pBuff = new char[m_mosVerbose.GetSize()];

		if(pBuff)
		{
			m_mosVerbose.CopyTo(pBuff, m_mosVerbose.GetSize());
			szStream.Append((const wxChar*)pBuff);

			delete[] pBuff;

			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
// Helper Methods
//////////////////////////////////////////////////////////////////////

void wxCurlBase::SetCurlHandleToDefaults()
{
	if(m_pCURL && ResetHandle())
	{
		ResetResponseVars();

		SetOpt(CURLOPT_HEADERFUNCTION, wxcurl_header_func);
		SetOpt(CURLOPT_WRITEHEADER, &m_szResponseHeader);
		SetOpt(CURLOPT_ERRORBUFFER, m_szErrorBuffer);

		if(m_pEvtHandler && (m_nFlags & wxCURL_SEND_PROGRESS_EVENTS))
		{
			SetOpt(CURLOPT_NOPROGRESS, FALSE);
			SetOpt(CURLOPT_PROGRESSFUNCTION, m_progressCallback);
			SetOpt(CURLOPT_PROGRESSDATA, m_progressData);
		}

		if(!m_szUsername.IsEmpty() || !m_szPassword.IsEmpty())
		{
			m_szUserPass = m_szUsername + wxT(":") + m_szPassword;

			SetOpt(CURLOPT_USERPWD, m_szUserPass.c_str());
			SetOpt(CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		}

		if(m_iHostPort != -1)
		{
			SetOpt(CURLOPT_PORT, m_iHostPort);
		}

		if(m_bUseProxy && !m_szProxyHost.IsEmpty())
		{
			SetOpt(CURLOPT_PROXY, m_szProxyHost.c_str());
		}

		if(m_bUseProxy && (m_iProxyPort != -1))
		{
			SetOpt(CURLOPT_PROXYPORT, m_iProxyPort);
		}

		if(m_bUseProxy && (!m_szProxyUsername.IsEmpty() || !m_szProxyPassword.IsEmpty()))
		{
			m_szProxyUserPass = m_szProxyUsername + wxT(":") + m_szProxyPassword;

			SetOpt(CURLOPT_PROXYUSERPWD, m_szProxyUserPass.c_str());
		}

		if(m_bVerbose)
		{
			SetOpt(CURLOPT_VERBOSE, TRUE);
			SetOpt(CURLOPT_DEBUGFUNCTION, wxcurl_verbose_stream_write);
			SetOpt(CURLOPT_DEBUGDATA, (void*) &m_mosVerbose);
		}
	}
}

void wxCurlBase::SetHeaders()
{
	if(!m_arrHeaders.IsEmpty())
	{
		if(m_pHeaders)
		{
			curl_slist_free_all(m_pHeaders);

			m_pHeaders = NULL;

			SetOpt(CURLOPT_HTTPHEADER, NULL);
		}

		for(unsigned int i = 0; i < m_arrHeaders.Count(); i++)
		{
			m_pHeaders = curl_slist_append(m_pHeaders, (const char*)(m_arrHeaders[i].c_str()));
		}

		SetOpt(CURLOPT_HTTPHEADER, m_pHeaders);
	}
}

void wxCurlBase::ResetHeaders()
{
	m_arrHeaders.Clear();

	if(m_pHeaders)
	{
		curl_slist_free_all(m_pHeaders);

		m_pHeaders = NULL;

		SetOpt(CURLOPT_HTTPHEADER, NULL);
	}
}

void wxCurlBase::ResetResponseVars()
{
	m_szResponseHeader = wxEmptyString;
	m_szResponseBody = wxEmptyString;
	m_iResponseCode = -1;
	m_szCurrFullURL = wxEmptyString;
}

//////////////////////////////////////////////////////////////////////
// Static Initialization and Shutdown Methods
//////////////////////////////////////////////////////////////////////

void wxCurlBase::Init()
{
	// Initialize LibCURL
	curl_global_init(CURL_GLOBAL_ALL);
}

void wxCurlBase::Shutdown()
{
	// Shutdown CurlLib
	curl_global_cleanup();
}

//////////////////////////////////////////////////////////////////////
// Static Utility Methods
//////////////////////////////////////////////////////////////////////

wxDateTime wxCurlBase::GetDateFromString(const wxString& szDate)
{
	time_t now = wxDateTime::Now().GetTicks();

	return wxDateTime(curl_getdate((const char*)(szDate.c_str()), &now));
}

wxString wxCurlBase::GetURLEncodedString(const wxString& szData)
{
	char* pszRetVal = curl_escape((const char*)(szData.c_str()), szData.Len());

	if(pszRetVal)
	{
		wxString szRetVal = (wxChar*)pszRetVal;

		curl_free(pszRetVal);

		return szRetVal;
	}

	return wxEmptyString;
}

wxString wxCurlBase::GetStringFromURLEncoded(const wxString& szData)
{
	char* pszRetVal = curl_unescape((const char*)(szData.c_str()), szData.Len());

	if(pszRetVal)
	{
		wxString szRetVal = (wxChar*)pszRetVal;

		curl_free(pszRetVal);

		return szRetVal;
	}

	return wxEmptyString;
}

wxString wxCurlBase::GetCURLVersion()
{
    return wxString(curl_version(), wxConvUTF8);
}
