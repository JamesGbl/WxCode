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
#include "wx/xml.h"


enum wxWebUpdatePlatform {
    wxWUP_INVALID = -1,
    wxWUP_MSW,
    wxWUP_GTK,
    wxWUP_OS2,
    wxWUP_MAC,
    wxWUP_MOTIF,
    wxWUP_X11
};


//! Contains the info about an available download.
class wxWebUpdateDownload
{
protected:
    
    wxWebUpdatePlatform m_platform;
    wxURL m_urlDownload;
     
public:
    wxWebUpdateDownload(const wxString &url, const wxString &plat = wxEmptyString)
         : m_platform(wxWUP_INVALID) { SetURL(url); SetPlatform(plat); }
    virtual ~wxWebUpdateDownload() {}
    
    bool IsOk() const
        { return m_urlDownload.GetError() != wxURL_NOERR && m_platform != wxWUP_INVALID; }
    bool IsOkForThisPort() const
        { return m_platform == GetThisPlatform(); }
    
    static wxWebUpdatePlatform GetThisPlatform();
        
        
public:     // setters

    bool SetURL(const wxString &url)
        { wxURL u(url); if (u.GetError() != wxURL_NOERR) return FALSE; }
        
    bool SetPlatform(const wxString &plat) {
        m_platform = wxWUP_INVALID;
        if (plat == wxT("msw")) m_platform = wxWUP_MSW;
        if (plat == wxT("gtk")) m_platform = wxWUP_GTK;
        if (plat == wxT("os2")) m_platform = wxWUP_OS2;
        if (plat == wxT("mac")) m_platform = wxWUP_MAC;
        if (plat == wxT("motif")) m_platform = wxWUP_MOTIF;
        if (plat == wxT("x11")) m_platform = wxWUP_X11;
        if (m_platform == wxWUP_INVALID) return FALSE;
        return TRUE;
    }
    
public:     // getters

    wxURL GetDownloadURL() const
        { return m_urlDownload; } 
        
    wxWebUpdatePlatform GetPlatform() const
        { return m_platform; }
};

//! Contains the info about a package update.
class wxWebUpdatePackage
{
protected:
    
    wxString m_strID;
    wxString m_strLatestVersion;
    wxString m_strUpdateAvailableMsg;
    wxString m_strUpdateNotAvailableMsg;
    
public:
    wxWebUpdatePackage() {}
    virtual ~wxWebUpdatePackage() {}
    
    virtual bool ParsePackage(const wxXmlNode *p); 
};

//! This is a possible result from wxUpdateCheck::Check function.
enum wxUpdateCheckFlag {
    
    wxUCF_UPDATED,      //!< Your program is still up-to-date.
    wxUCF_OUTOFDATE     //!< The web server holds an updated version.
};


WX_DEFINE_ARRAY(wxWebUpdatePackage*, wxWebUpdatePackageArray);


//! This is the class which performs all transactions with the server.
//! It uses the wxSocket facilities.
class wxWebUpdateXMLScript : public wxXmlDocument
{
protected:
    
    //! The array containing the packages which are listed in
    //! the web server update script.
    wxWebUpdatePackageArray m_arrPackages;
    
public:
	wxWebUpdateXMLScript(const wxString &strURL) {}
	virtual ~wxWebUpdateXMLScript() {}

    //! Couldn't connect to the given URL for some reason.
    virtual bool Load();

    //! Checks if the webserver holds a more recent version of the given package.
    //! \param strURL The URL of the XML file stored in the web server which holds 
    //!        the informations to parse.
    //! \param version The string version of the package which is being tested.
    //! \param packagename The name of the package whose version must be tested.
	virtual wxUpdateCheckFlag Check(const wxString &version,
                                    const wxString &packagename) const;

protected:
    


private:
	DECLARE_CLASS(wxWebUpdateXMLScript)
	//DECLARE_EVENT_TABLE()
};

#endif // _WX_WEBUPDATE_H_

