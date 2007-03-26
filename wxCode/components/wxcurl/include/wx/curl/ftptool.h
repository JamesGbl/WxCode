/*
 *	ftptool.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Aug 13 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 *	Licence: wxWidgets Licence
 */

#ifndef _WXCURLFTPTOOL_H__INCLUDED_
#define _WXCURLFTPTOOL_H__INCLUDED_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "curl/ftptool.h"
#endif

#include <wx/curl/ftp.h>

// C Function Declarations for LibCURL
//
//////////////////////////////////////////////////////////////////////

extern "C"
{
}

// ftptool.h: interface for the wxCurlFTPFs class.
//
//////////////////////////////////////////////////////////////////////

class wxCurlFTPTool;

class WXDLLIMPEXP_CURL wxCurlFTPFs
{
friend class wxCurlFTPTool;

public:
	wxCurlFTPFs();
	wxCurlFTPFs(const wxString&	szName			,
				  const bool&		bIsDir			,
				  const bool&		bIsFile			,
				  const time_t&		tLastModified	,
				  const long&		iContentLength	);
	~wxCurlFTPFs();

	bool IsDirectory() const			{ return m_bIsDir; }
	bool IsFile() const					{ return m_bIsFile; }

	wxString GetName() const			{ return m_szName; }

	wxDateTime GetLastModified() const	{ return m_dtLastModified; }

	long GetContentLength() const		{ return m_iContentLength; }

	wxString GetFileSuffix() const		{ return m_szName.AfterLast('.'); }


protected:
	wxString		m_szName;

	bool			m_bIsDir;
	bool			m_bIsFile;

	wxDateTime		m_dtLastModified;

	long			m_iContentLength;
};

WX_DECLARE_OBJARRAY(wxCurlFTPFs, wxArrayFTPFs);

// ftptool.h: interface for the wxCurlFTPTool class.
//
//////////////////////////////////////////////////////////////////////

class WXDLLIMPEXP_CURL wxCurlFTPTool : public wxCurlFTP
{
public:
	wxCurlFTPTool(const wxString& szURL = wxEmptyString, const wxString& szUserName = wxEmptyString, const wxString& szPassword = wxEmptyString, wxEvtHandler* pEvtHandler = NULL, const bool& bSendUpdateEvents = false, const bool& bSendBeginEndEvents = false);
	virtual ~wxCurlFTPTool();

	// More Complex Action Methods - These All Make Calls To: curl_easy_perform()
	// These routines have more 'intelligence' than simple FTP calls.
	bool		GetFTPFs(wxArrayFTPFs& fs, const wxString& szRemoteLoc = wxEmptyString);
	bool		Exists(const wxString& szRemoteLoc = wxEmptyString);
	bool		IsDirectory(const wxString& szRemoteLoc = wxEmptyString);
	bool		HasDirectory(const wxString& szRemoteLoc = wxEmptyString) { return IsDirectory(szRemoteLoc); }
	wxDateTime	GetLastModified(const wxString& szRemoteLoc = wxEmptyString);
	long		GetContentLength(const wxString& szRemoteLoc = wxEmptyString);
	wxString	GetFileSuffix(const wxString& szRemoteLoc = wxEmptyString);

protected:

private:

};

#endif // _WXCURLFTPTOOL_H__INCLUDED_
