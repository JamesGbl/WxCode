/////////////////////////////////////////////////////////////////////////////
// Name:        webupdate.h
// Purpose:     wxUpdateCheck
// Author:      Francesco Montorsi
// Created:     2005/06/23
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_WEBUPDATE_H_
#define _WX_WEBUPDATE_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "webupdate.h"
#endif

// wxWidgets headers
#include "wx/xml/xml.h"
#include "wx/url.h"



// for shared builds
#ifdef WXMAKINGDLL_WEBUPDATE
    #define WXDLLIMPEXP_WEBUPDATE				WXEXPORT
    #define WXDLLIMPEXP_DATA_WEBUPDATE(type)	WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_WEBUPDATE WXIMPORT
    #define WXDLLIMPEXP_DATA_NWEBUPDATE(type)	WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_WEBUPDATE
    #define WXDLLIMPEXP_DATA_WEBUPDATE(type)	type
#endif



//! A global wxWebUpdateDownload variable which contains empty (and thus invalid)
//! settings.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateDownload;		// defined later
extern wxWebUpdateDownload wxEmptyWebUpdateDownload;


//! The possible values of the "platform" attribute of the
//! LATEST-DOWNLOAD tag supported by wxWebUpdateXMLScript.
enum wxWebUpdatePlatform {
    wxWUP_INVALID = -1,		//!< Unrecognized platform.
    wxWUP_MSW,				//!< wxMSW.
    wxWUP_GTK,				//!< wxGTK.
    wxWUP_OS2,				//!< wxOS2.
    wxWUP_MAC,				//!< wxMac.
    wxWUP_MOTIF,			//!< wxMotif.
    wxWUP_X11,				//!< wxX11.
	wxWUP_MGL				//!< wxMGL.
};


//! This is a possible result from wxUpdateCheck::Check function.
enum wxWebUpdateCheckFlag {
    
    wxWUCF_UPDATED,      //!< Your program is still up-to-date.
    wxWUCF_OUTOFDATE     //!< The web server holds an updated version.
};



//! Contains the info about an available download.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateDownload
{
protected:
    
	//! The ID code of the platform this download is designed for.
    wxWebUpdatePlatform m_platform;

	//! The URL to the download.
    wxURL m_urlDownload;
     
public:
    wxWebUpdateDownload(const wxString &url, const wxString &plat = wxEmptyString)
         : m_platform(wxWUP_INVALID), m_urlDownload(url) { SetPlatform(plat); }
    virtual ~wxWebUpdateDownload() {}
    
	//! Returns TRUE if this package was correctly initialized.
    bool IsOk() const
        { return m_urlDownload.GetError() == wxURL_NOERR && m_platform != wxWUP_INVALID; }

	//! Like #IsOk() but returns TRUE only if this download is designed for
	//! the platform where the program is currently running on.
    bool IsOkForThisPlatform() const
        { return IsOk() && m_platform == GetThisPlatformCode(); }
    
public:		// static platform utilities

	//! Gets a wxWebUpdatePlatform code for the platform where this program
	//! is currently running on.
    static wxWebUpdatePlatform GetThisPlatformCode();

	//! Returns the string associated with this platform code.
    static wxString GetThisPlatformString() 
		{ return GetPlatformString(GetThisPlatformCode()); }
	
	//! Returns the code for the platform with the given name.
	static wxWebUpdatePlatform GetPlatformCode(const wxString &platname);

	//! Returns the string for the platform with the given code.
 	static wxString GetPlatformString(wxWebUpdatePlatform code);
       
        
public:     // setters

    bool SetURL(const wxString &url)
        { wxURL u(url); if (u.GetError() != wxURL_NOERR) return FALSE; }
        
    bool SetPlatform(const wxString &plat) {
        m_platform = GetPlatformCode(plat);
        if (m_platform == wxWUP_INVALID) 
			return FALSE;
        return TRUE;
    }
    
public:     // getters

    wxURL GetDownloadURL() const
        { return m_urlDownload; } 
        
    wxWebUpdatePlatform GetPlatform() const
        { return m_platform; }

public:		// operators

	wxWebUpdateDownload &operator=(const wxWebUpdateDownload &tocopy)
		{ m_platform = tocopy.m_platform; m_urlDownload = tocopy.m_urlDownload; return *this; }
};




WX_DECLARE_OBJARRAY(wxWebUpdateDownload, wxWebUpdateDownloadArray);


//! Contains the info about a package update.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdatePackage
{
	friend class wxWebUpdateXMLScript;

protected:
    
	//! The ID/name of this package. This must be unique in each XML webupdate script.
    wxString m_strID;
	
	//! The version of the downloads available for this package; i.e. the latest
	//! version available on the webserver.
    wxString m_strLatestVersion;

	//! The content of the <msg-update-available> tag, if present.
    wxString m_strUpdateAvailableMsg;
	
	//! The content of the <msg-update-notavailable> tag, if present.
    wxString m_strUpdateNotAvailableMsg;

	//! The array containing all the webupdate downloads available for this package.
	//! (Each download is for the same vresion of this package but for a different
	//! platform).
	wxWebUpdateDownloadArray m_arrWebUpdates;
    
public:
    wxWebUpdatePackage(const wxString &id = wxEmptyString) : m_strID(id) {}
    virtual ~wxWebUpdatePackage() {}
    
	//! Adds the given download package to the array.
	virtual void AddDownloadPackage(const wxWebUpdateDownload &toadd)
		{ m_arrWebUpdates.Add(toadd); }

	//! Returns a reference to the download package for the given platform string.
	virtual wxWebUpdateDownload &GetDownloadPackage(const wxString &platform) const
		{ return GetDownloadPackage(wxWebUpdateDownload::GetPlatformCode(platform)); }	
	
	//! Returns a reference to the download package for the given platform code.
	//! In each package it should exist only a single download for each platform
	//! so the returned object should be the only item which is designed for the
	//! given platform.
	virtual wxWebUpdateDownload &GetDownloadPackage(
				wxWebUpdatePlatform code = wxWUP_INVALID) const;
};


//! This is the class which performs all transactions with the server.
//! It uses the wxSocket facilities.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateXMLScript : public wxXmlDocument
{
protected:
    
    //! The array containing the packages which are listed in
    //! the web server update script.
    //wxWebUpdatePackageArray m_arrPackages;
    
public:
	wxWebUpdateXMLScript(const wxString &strURL) { Load(strURL); }
	virtual ~wxWebUpdateXMLScript() {}

    //! Couldn't connect to the given URL for some reason.
    virtual bool Load(const wxString &url);

	//! Returns the wxWebUpdatePackage stored in this document for
	//! the given package. Returns NULL if the package you asked for
	//! is not present in this XML document.
	virtual wxWebUpdatePackage *GetPackage(const wxString &packagename) const;

    //! Checks if the webserver holds a more recent version of the given package.
    //! \param strURL The URL of the XML file stored in the web server which holds 
    //!        the informations to parse.
    //! \param version The string version of the package which is being tested.
    //! \param packagename The name of the package whose version must be tested.
	virtual wxWebUpdateCheckFlag Check(const wxString &version,
                                    const wxString &packagename) const;

private:
	DECLARE_CLASS(wxWebUpdateXMLScript)
};

#endif // _WX_WEBUPDATE_H_

