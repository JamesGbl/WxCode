/*
 *	dav.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Jun 02 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 *
 *	Licence: wxWidgets Licence
 */

// dav.cpp: implementation of the wxCurlDAV class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Headers
//////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include <wx/mstream.h>

#include <wx/curl/dav.h>

//////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxCurlDAV::wxCurlDAV(const wxString& szURL /*= wxEmptyString*/, 
                     const wxString& szUserName /*= wxEmptyString*/, 
                     const wxString& szPassword /*= wxEmptyString*/, 
                     wxEvtHandler* pEvtHandler /*= NULL*/, 
                     long flags /*=wxCURL_DEFAULT_FLAGS*/)
: wxCurlHTTP(szURL, szUserName, szPassword, pEvtHandler, flags)
{
}

wxCurlDAV::~wxCurlDAV()
{
}

//////////////////////////////////////////////////////////////////////
// Member Data Access Methods
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Action Methods
//////////////////////////////////////////////////////////////////////

bool wxCurlDAV::Mkcol(const wxString& szRemoteCol /*= wxEmptyString*/)
{
	if(m_pCURL)
	{
		SetCurlHandleToDefaults();

		m_szCurrFullURL = m_szBaseURL + szRemoteCol;

		SetOpt(CURLOPT_URL, m_szCurrFullURL.c_str());
		SetOpt(CURLOPT_CUSTOMREQUEST, "MKCOL");
		SetOpt(CURLOPT_WRITEFUNCTION, wxcurl_str_write);
		SetOpt(CURLOPT_WRITEDATA, (void*)&m_szResponseBody);

		if(Perform())
		{
			GetInfo(CURLINFO_RESPONSE_CODE, &m_iResponseCode);

			return ((m_iResponseCode > 199) && (m_iResponseCode < 300));
		}
	}

	return false;
}

bool wxCurlDAV::Propfind(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	return Propfind(NULL, 0, szRemoteLoc);
}

bool wxCurlDAV::Propfind(const wxArrayString& arrTags, const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxString szXml = ConstructPropfindXml(arrTags);

	return Propfind((const char*)(szXml.c_str()), szXml.Len(), szRemoteLoc);
}

bool wxCurlDAV::Propfind(const char* buffer, size_t size, const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxMemoryInputStream inStream(buffer, size);

	return Propfind(inStream, szRemoteLoc);
}

bool wxCurlDAV::Propfind(wxInputStream& buffer, const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	curl_off_t iSize = buffer.GetSize();

	if(m_pCURL && buffer.IsOk())
	{
		if(iSize == (~(size_t)0))	// wxCurlDAV does not know how to upload unknown length streams.
			return false;

		SetCurlHandleToDefaults();

		m_arrHeaders.Add(wxT("Depth: 1"));
		m_arrHeaders.Add(wxT("Content-Type: text/xml; charset=\"utf-8\""));

		SetHeaders();

		m_szCurrFullURL = m_szBaseURL + szRemoteLoc;

		SetOpt(CURLOPT_URL, m_szCurrFullURL.c_str());

		if(iSize > 0)
		{
			SetOpt(CURLOPT_UPLOAD, TRUE);
			SetOpt(CURLOPT_INFILESIZE_LARGE, iSize);
			SetOpt(CURLOPT_READFUNCTION, wxcurl_stream_read);
			SetOpt(CURLOPT_READDATA, (void*)&buffer);
		}

		SetOpt(CURLOPT_CUSTOMREQUEST, "PROPFIND");
		SetOpt(CURLOPT_WRITEFUNCTION, wxcurl_str_write);
		SetOpt(CURLOPT_WRITEDATA, (void*)&m_szResponseBody);

		if(Perform())
		{
			GetInfo(CURLINFO_RESPONSE_CODE, &m_iResponseCode);

			ResetHeaders();

			return ((m_iResponseCode > 199) && (m_iResponseCode < 300));
		}

		ResetHeaders();
	}

	return false;
}

bool wxCurlDAV::Proppatch(const char* buffer, size_t size, const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxMemoryInputStream inStream(buffer, size);

	return Proppatch(inStream, szRemoteLoc);
}

bool wxCurlDAV::Proppatch(wxInputStream& buffer, const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	curl_off_t iSize = buffer.GetSize();

	if(m_pCURL && buffer.IsOk())
	{
		if(iSize == (~(size_t)0))	// wxCurlDAV does not know how to upload unknown length streams.
			return false;

		SetCurlHandleToDefaults();

		m_arrHeaders.Add(wxT("Content-Type: text/xml; charset=\"utf-8\""));

		SetHeaders();

		m_szCurrFullURL = m_szBaseURL + szRemoteLoc;

		SetOpt(CURLOPT_URL, m_szCurrFullURL.c_str());

		if(iSize > 0)
		{
			SetOpt(CURLOPT_UPLOAD, TRUE);
			SetOpt(CURLOPT_INFILESIZE_LARGE, iSize);
			SetOpt(CURLOPT_READFUNCTION, wxcurl_stream_read);
			SetOpt(CURLOPT_READDATA, (void*)&buffer);
		}

		SetOpt(CURLOPT_CUSTOMREQUEST, "PROPPATCH");
		SetOpt(CURLOPT_WRITEFUNCTION, wxcurl_str_write);
		SetOpt(CURLOPT_WRITEDATA, (void*)&m_szResponseBody);

		if(Perform())
		{
			GetInfo(CURLINFO_RESPONSE_CODE, &m_iResponseCode);

			ResetHeaders();

			return ((m_iResponseCode > 199) && (m_iResponseCode < 300));
		}

		ResetHeaders();
	}

	return false;
}

bool wxCurlDAV::Copy(const wxString& szRemoteLocDest, const bool& bOverwrite /*= true*/, const wxString& szRemoteLocSrc /*= wxEmptyString*/)
{
	if(m_pCURL)
	{
		SetCurlHandleToDefaults();

		m_arrHeaders.Add(wxT("Destination: ") + szRemoteLocDest);
		
		if(!bOverwrite)
			m_arrHeaders.Add(wxT("Overwrite: F"));

		SetHeaders();

		m_szCurrFullURL = m_szBaseURL + szRemoteLocSrc;

		SetOpt(CURLOPT_URL, m_szCurrFullURL.c_str());
		SetOpt(CURLOPT_CUSTOMREQUEST, "COPY");
		SetOpt(CURLOPT_WRITEFUNCTION, wxcurl_str_write);
		SetOpt(CURLOPT_WRITEDATA, (void*)&m_szResponseBody);

		if(Perform())
		{
			GetInfo(CURLINFO_RESPONSE_CODE, &m_iResponseCode);

			ResetHeaders();

			return ((m_iResponseCode > 199) && (m_iResponseCode < 300));
		}

		ResetHeaders();
	}

	return false;
}

bool wxCurlDAV::Move(const wxString& szRemoteLocDest, const bool& bOverwrite /*= true*/, const wxString& szRemoteLocSrc /*= wxEmptyString*/)
{
	if(m_pCURL)
	{
		SetCurlHandleToDefaults();

		m_arrHeaders.Add(wxT("Destination: ") + szRemoteLocDest);
		
		if(!bOverwrite)
			m_arrHeaders.Add(wxT("Overwrite: F"));

		SetHeaders();

		m_szCurrFullURL = m_szBaseURL + szRemoteLocSrc;

		SetOpt(CURLOPT_URL, m_szCurrFullURL.c_str());
		SetOpt(CURLOPT_CUSTOMREQUEST, "MOVE");
		SetOpt(CURLOPT_WRITEFUNCTION, wxcurl_str_write);
		SetOpt(CURLOPT_WRITEDATA, (void*)&m_szResponseBody);

		if(Perform())
		{
			GetInfo(CURLINFO_RESPONSE_CODE, &m_iResponseCode);

			ResetHeaders();

			return ((m_iResponseCode > 199) && (m_iResponseCode < 300));
		}

		ResetHeaders();
	}

	return false;
}

bool wxCurlDAV::Lock(const wxString& WXUNUSED(szRemoteLoc /*=wxEmptyString*/))
{
	return false;
}

bool wxCurlDAV::Unlock(const wxString& WXUNUSED(szRemoteLoc /*=wxEmptyString*/))
{
	return false;
}

//////////////////////////////////////////////////////////////////////
// Helper Methods
//////////////////////////////////////////////////////////////////////

wxString wxCurlDAV::ConstructPropfindXml(const wxArrayString& arrTags)
{
	wxString szXml;

	szXml += _T("<?xml version=\"1.0\" encoding=\"utf-8\" ?>") _T("\n");
	szXml += _T("<D:propfind xmlns:D=\"DAV:\">") _T("\n");

	if(arrTags.Count() > 0)
	{
		szXml += _T("\t") _T("<D:prop>") _T("\n");
	
		for(unsigned int i = 0; i < arrTags.Count(); i++)
		{
			szXml += _T("\t\t") _T("<D:");
			szXml += arrTags[i];
			szXml += _T("/>") _T("\n");
		}

		szXml += _T("\t") _T("</D:prop>") _T("\n");
	}
	else
	{
		szXml += _T("\t") _T("<D:allprop/>") _T("\n");
	}

	szXml += _T("</D:propfind>") _T("\n");

	return szXml;
}

