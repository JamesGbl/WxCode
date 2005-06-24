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

//! The possible values of the "platform" attribute of the
//! LATEST-DOWNLOAD tag supported by wxWebUpdateXMLScript.
enum wxWebUpdatePlatform {
    wxWUP_INVALID = -1,
    wxWUP_MSW,
    wxWUP_GTK,
    wxWUP_OS2,
    wxWUP_MAC,
    wxWUP_MOTIF,
    wxWUP_X11,
	wxWUP_MGL
};


//! Contains the info about an available download.
class wxWebUpdateDownload
{
protected:
    
    wxWebUpdatePlatform m_platform;
    wxURL m_urlDownload;
     
public:
    wxWebUpdateDownload(const wxString &url, const wxString &plat = wxEmptyString)
         : m_platform(wxWUP_INVALID), m_urlDownload(url) { SetPlatform(plat); }
    virtual ~wxWebUpdateDownload() {}
    
    bool IsOk() const
        { return m_urlDownload.GetError() == wxURL_NOERR && m_platform != wxWUP_INVALID; }
    bool IsOkForThisPlatform() const
        { return IsOk() && m_platform == GetThisPlatform(); }
    
public:		// static platform utilities

    static wxWebUpdatePlatform GetThisPlatform();
	static wxWebUpdatePlatform GetPlatformCode(const wxString &platname);
        
        
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
};




WX_DECLARE_OBJARRAY(wxWebUpdateDownload, wxWebUpdateDownloadArray);


//! Contains the info about a package update.
class wxWebUpdatePackage
{
	friend class wxWebUpdateXMLScript;

protected:
    
    wxString m_strID;
    wxString m_strLatestVersion;
    wxString m_strUpdateAvailableMsg;
    wxString m_strUpdateNotAvailableMsg;

	wxWebUpdateDownloadArray m_arrWebUpdates;
    
public:
    wxWebUpdatePackage(const wxString &id = wxEmptyString) : m_strID(id) {}
    virtual ~wxWebUpdatePackage() {}
    
	virtual void AddDownloadPackage(const wxWebUpdateDownload &toadd)
		{ m_arrWebUpdates.Add(toadd); }
	
	virtual wxWebUpdateDownload GetDownloadPackage(const wxString &platform) const
		{ return GetDownloadPackage(wxWebUpdateDownload::GetPlatformCode(platform)); }
	
	virtual wxWebUpdateDownload GetDownloadPackage(wxWebUpdatePlatform code = wxWUP_INVALID) const {
		if (code == wxWUP_INVALID) code = wxWebUpdateDownload::GetThisPlatform();
		for (int i=0; i<(int)m_arrWebUpdates.GetCount(); i++)
			if (m_arrWebUpdates.Item(i).GetPlatform() == code)
				return m_arrWebUpdates.Item(i);
		return wxWebUpdateDownload(wxT("invalid"));
	}
};

//! This is a possible result from wxUpdateCheck::Check function.
enum wxWebUpdateCheckFlag {
    
    wxWUCF_UPDATED,      //!< Your program is still up-to-date.
    wxWUCF_OUTOFDATE     //!< The web server holds an updated version.
};



//! This is the class which performs all transactions with the server.
//! It uses the wxSocket facilities.
class wxWebUpdateXMLScript : public wxXmlDocument
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
	//DECLARE_EVENT_TABLE()
};

#endif // _WX_WEBUPDATE_H_

