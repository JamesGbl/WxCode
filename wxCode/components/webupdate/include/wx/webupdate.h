/////////////////////////////////////////////////////////////////////////////
// Name:        webupdate.h
// Purpose:     wxWebUpdateXMLScript, wxWebUpdatePackage, wxWebUpdateDownload
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
#include "wx/webupdatedef.h"
#include "wx/installer.h"
#include "wx/xml/xml.h"
#include "wx/url.h"


// defined later
class WXDLLIMPEXP_WEBUPDATE wxDownloadThread;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateDownload;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdatePackage;

//! A global wxWebUpdateDownload variable which contains empty (and thus invalid)
//! settings.
extern WXDLLIMPEXP_DATA_WEBUPDATE(wxWebUpdateDownload) wxEmptyWebUpdateDownload;
extern WXDLLIMPEXP_DATA_WEBUPDATE(wxWebUpdatePackage) wxEmptyWebUpdatePackage;


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
	wxWUP_MGL,				//!< wxMGL.
	wxWUP_ANY				//!< platform-indepedent.
};


//! This is a possible result from wxUpdateCheck::Check function.
enum wxWebUpdateCheckFlag {

	wxWUCF_FAILED,		 //!< Something went wrong while performing the check;
						 //!< maybe you're not using the version format MAJ.MIN.REL
						 //!< or that the local version of this program is greater
						 //!< than the latest available version on the website.
    wxWUCF_UPDATED,      //!< Your program is still up-to-date.
    wxWUCF_OUTOFDATE     //!< The web server holds an updated version.
};


//! The possible values of the "IMPORTANCE" attribute of the <LATEST-VERSION>
//! tag of a wxWebUpdatePackage XML script.
enum wxWebUpdatePackageImportance {

	wxWUPI_HIGH,		//!< This update should be installed !
	wxWUPI_NORMAL,		//!< Normal importance.
	wxWUPI_LOW,			//!< Do not download it if you have a slow connection !
};


//! This is the most generic way to handle the 'version' of something.
//! Currently wxWebUpdate classes support only the version format
//! MAJOR.MINOR.RELEASE (in each field only numbers are allowed); 
//! if you need to support some other version format (maybe 1.23.4.4a 
//! or 1.2b or v4.3 ...) then you need to overload the following functions:
//! - wxWebUpdate
typedef wxString		wxVersion;
#define wxEmptyVersion  wxEmptyString



//! A simple container of the two basic info which wxWebUpdate classes 
//! need to know about user *local* packages: the NAME and the local VERSION.
//! This class should not be confused with wxWebUpdatePackage which instead
//! contains the info parsed from a wxWebUpdateXMLScript about a package
//! on the *remote* server.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateLocalPackage : public wxObject
{
public:		// to avoid setters/getters

	//! The name of this package.
	wxString m_strName;

	//! The version of this package.
	wxVersion m_version;

public:
	wxWebUpdateLocalPackage(const wxString &name = wxEmptyString, 
							const wxVersion &ver = wxEmptyVersion)
		: m_strName(name), m_version(ver) {}
	virtual ~wxWebUpdateLocalPackage() {}

private:
	DECLARE_CLASS(wxWebUpdateLocalPackage)
};

// a container of wxWebUpdateLocalPackage used by wxWebUpdater
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxWebUpdateLocalPackage, wxWebUpdateLocalPackageArray, WXDLLIMPEXP_WEBUPDATE);


//! Contains the info about an available download.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateDownload : public wxObject
{
protected:
    
	//! The ID code of the platform this download is designed for.
    wxWebUpdatePlatform m_platform;

	//! The MD5 checksum for this download.
	//! This is the MD5 which is used for comparison with the actual
	//! downloaded files and is taken from the wxWebUpdateXMLScript.
	wxString m_strMD5;

	//! The URL to the download.
	//! Note: this info was initially kept in a wxURL variable but since
	//!       wxURL::GetInputStream() is deprecated in favour of wxFileSystem
	//!       I use here a wxString which is easier to handle.    
	wxString m_urlDownload;

	//! The (rough) size of the resource pointed by #m_urlDownload.
	//! Use the #GetDownloadSize() function to get this info.
	//! Note: since it can take some time (from 0.1 to ~1 sec) to get
	//!       this info, this variable is not set directly when constructing
	//!       this object but only the first time that #GetDownloadSize is
	//!       called...
	//! Note #2: it's best to use an unsigned long for this field since big
	//!          updates could require all the available bits of this var...
	//! Note #3: when this variable has not been cached it is set to the value
	//!          '1' (since it's unsigned -1 cannot be used and 0 can be useful
	//!          to indicate not-reachable files !) which thus means
	//!          "uninitialized"; this also means that you cannot use update
	//!          packages sized 1 byte only since they would force continuous
	//!          recalculations of this var !
	unsigned long m_size;

	//! The actions to perform after this file has been downloaded.
	//! Since wxWebUpdateAction is an abstract class, this is an array of pointers
	//! to the actions for this download and needs to be cleaned up manually in the
	//! destructor.
	wxWebUpdateActionArray m_arrActions;
     
public:
    wxWebUpdateDownload(const wxString &url, 
						const wxString &plat = wxEmptyString,
						const wxString &md5 = wxEmptyString,
						const wxWebUpdateActionArray *actions = NULL)
         : m_platform(wxWUP_INVALID), m_urlDownload(url), m_strMD5(md5)
		{ SetPlatform(plat); m_size=1; if (actions) m_arrActions = *actions; }

    virtual ~wxWebUpdateDownload();

public:		// handle in the right way the issue of the m_arrActions array
			// which contains pointers to classes which needs to be copied
			// in order to avoid double-destruction (and thus invalid memory
			// accesses) of the same pointers !
	
	wxWebUpdateDownload(const wxWebUpdateDownload &tocopy)
		{ Copy(tocopy); }

	//! Copies the given wxWebUpdateDownload object into this one, duplicating
	//! in the proper way the action array.
	void Copy(const wxWebUpdateDownload &tocopy);

	//! The assignment operator
    wxWebUpdateDownload &operator =(const wxWebUpdateDownload &tocopy) 
		{ Copy(tocopy); return *this; }

public:		// miscellaneous
    
	//! Returns TRUE if this package was correctly initialized.
    bool IsOk() const
        { return wxURL(m_urlDownload).GetError() == wxURL_NOERR && 
			(m_strMD5.IsEmpty() || m_strMD5.Len() == 32) && m_platform != wxWUP_INVALID; }

	//! Like #IsOk() but returns TRUE only if this download is designed for
	//! the platform where the program is currently running on.
    bool IsOkForThisPlatform() const
        { return IsOk() && m_platform == GetThisPlatformCode(); }

	//! Returns the size (in bytes) of the update asking it to the webserver.
	//! The first time this function is called, it caches this value so that
	//! following calls do not take time.
	//! If something fails (e.g. cannot connect to the file or the file does
	//! not exists), the function returns 0.
	//! NOTE: the first call is quite slow !
	virtual unsigned long GetDownloadSize(bool forceRecalc = FALSE);

	//! Returns the filename of the resource pointed by the current URL
	//! (thus the returned name is extracted from the download URL).
	virtual wxString GetFileName() const;

public:		// main functions

	//! Downloads this package using wxDownloadThread and put it in the given path.
	//! The given event handler will receive a wxDT_DOWNLOAD_COMPLETE event when
	//! the download has been completed.
	//! \param proxy The proxy settings in the form "hostname:portnumber"
	//! \param user The username for the HTTP authentication (if it's required).
	//! \param password The password for the HTTP authentication (if it's required).
	virtual wxDownloadThread *DownloadAsynch(const wxString &path, 
								wxEvtHandler *handler,
								const wxString &proxy = wxEmptyString,
								const wxString &user = wxEmptyString,
								const wxString &password = wxEmptyString);

	//! Downloads this package synchronously and thus when the function returns
	//! the package has been completely downloaded (if result is TRUE).
	virtual bool DownloadSynch(const wxString &path,
								const wxString &proxy = wxEmptyString,
								const wxString &user = wxEmptyString,
								const wxString &password = wxEmptyString);

	//! Installs this download using the global wxWebUpdateInstaller class
	//! (see wxWebUpdateInstaller::Get).
	virtual bool Install() const;
    
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

	//! Sets the URL for this download. Returns FALSE if the given string
	//! is not a valid URL.
    bool SetURL(const wxString &url) { 
		wxURL u(url); if (u.GetError() != wxURL_NOERR) return FALSE; 
		m_urlDownload=url; return TRUE; 
	}

	//! Sets the platform targeted by this download.
    bool SetPlatform(const wxString &plat) {
        m_platform = GetPlatformCode(plat);
        if (m_platform == wxWUP_INVALID) 
			return FALSE;
        return TRUE;
    }
    
public:     // getters

    wxURL GetDownloadURL() const
        { return m_urlDownload; } 
  
    wxString GetDownloadString() const
        { return m_urlDownload; } 
      
    wxWebUpdatePlatform GetPlatform() const
        { return m_platform; }

	wxString GetMD5Checksum() const
		{ return m_strMD5; }

private:
	DECLARE_CLASS(wxWebUpdateDownload)
};


// a container of wxWebUpdateDownload used by wxWebUpdatePackage
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxWebUpdateDownload, wxWebUpdateDownloadArray, WXDLLIMPEXP_WEBUPDATE);


//! Contains the info about a package update.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdatePackage : public wxObject
{
	friend class wxWebUpdateXMLScript;

protected:		// member variables
    
	//! The ID/name of this package. This must be unique in each XML webupdate script.
    wxString m_strID;
	
	//! The version of the downloads available for this package; i.e. the latest
	//! version available on the webserver.
	//! This is stored as a wxVersion so that it's easy to override the #Check functions
	//! and use another parser (rather than #ExtractVersionNumbers) to do the check.
    wxVersion m_strLatestVersion;

	//! The description for this package.
	wxString m_strDescription;

	//! The importance level of this package.
	wxWebUpdatePackageImportance m_importance;

	//! The array containing all the webupdate downloads available for this package.
	//! (Each download is for the same version of this package but for a different
	//! platform).
	wxWebUpdateDownloadArray m_arrWebUpdates;

	//! The comma separed list of prerequisites for this package.
	//! This list should contain only the IDs of other existing packages.
	wxString m_strPrerequisites;

protected:		// utilities

	//! Returns in the given int* the parsed version numbers of the given wxVersion.
	static bool ExtractVersionNumbers(const wxVersion &str, int *maj, int *min, int *rel);
    
public:
    wxWebUpdatePackage(const wxString &id = wxEmptyString) : m_strID(id) {}
    virtual ~wxWebUpdatePackage() { m_arrWebUpdates.Clear(); }
    
public:		// package utilities

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

	//! Caches the download sizes of all contained packages.
	virtual void CacheDownloadSizes();

public:		// version check

    //! Checks if the webserver holds a more recent version of the given package.
    //! \param strURL The URL of the XML file stored in the web server which holds 
    //!        the informations to parse.
    //! \param version The string version of the package which is being tested.
    //! \param packagename The name of the package whose version must be tested.
	virtual wxWebUpdateCheckFlag Check(const wxVersion &localversion) const;
	virtual wxWebUpdateCheckFlag Check(int maj, int min, int rel) const;

public:		// getters

	//! Returns the name of this package.
	wxString GetName() const					{ return m_strID; }

	//! Returns the latest available version for this package.
	wxString GetLatestVersion() const			{ return m_strLatestVersion; }

	//! Returns the description for this package.
	wxString GetDescription() const				{ return m_strDescription; }

	//! Returns the importance level of the updates contained for this package.
	wxWebUpdatePackageImportance GetImportance() const
		{ return m_importance; }

	//! Returns a comma-separed list of the names of the required packages.
	wxString GetPrerequisites() const			{ return m_strPrerequisites; }

private:
	DECLARE_CLASS(wxWebUpdatePackage)
};


// a container of wxWebUpdatePackage used by wxWebUpdateXMLScript
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxWebUpdatePackage, wxWebUpdatePackageArray, WXDLLIMPEXP_WEBUPDATE);


//! This is the class which performs all the XML parsing of the webupdate script.
//! It uses the wxFileSystem facilities to download the file (see #Load) thus
//! you can load both local & remote files.
//! Use the #GetAllPackages to do the XML parsing of the entire script or
//! #GetPackage to parse a single package.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateXMLScript : public wxXmlDocument
{
protected:

	//! The content of the <msg-update-available> tag, if present.
    wxString m_strUpdateAvailableMsg;
	
	//! The content of the <msg-update-notavailable> tag, if present.
    wxString m_strUpdateNotAvailableMsg;

protected:

	//! Creates a wxWebUpdatePackage from the given XML node.
	//! The caller must delete the returned pointer.
	wxWebUpdatePackage *GetPackage(const wxXmlNode *package) const;

	//! Creates a wxWebUpdateDownload from the given XML node.
	wxWebUpdateDownload GetDownload(const wxXmlNode *latestdownload) const;

	//! Returns the text content of the given node.
	wxString GetNodeContent(const wxXmlNode *node) const;

	//! Does string substitution using the current keyword hashmap of
	//! the given wxWebUpdateInstaller.
	wxString DoKeywordSubstitution(const wxString &str) const;

public:

	wxWebUpdateXMLScript(const wxString &strURI = wxEmptyString);
	virtual ~wxWebUpdateXMLScript() {}

public:		// main functions

    //! Parses the XML script located at the given URI.
	//! This function can open any resource which can be handled
	//! by wxFileSystem but it should typically be used to load
	//! a file stored in a webserver.
	//! Returns TRUE if the document was successfully parsed.
    virtual bool Load(const wxString &uri);

	//! Returns the wxWebUpdatePackage stored in this document for
	//! the given package. Returns NULL if the package you asked for
	//! is not present in this XML document.
	virtual wxWebUpdatePackage *GetPackage(const wxString &packagename) const;

	//! Returns all the available packages from this XML document.
	virtual wxWebUpdatePackageArray GetAllPackages() const;

	//! Returns the content of the <msg-update-available> tag, if present. 
	wxString GetUpdateAvailableMsg() const		{ return m_strUpdateAvailableMsg; }

	//! Returns the content of the <msg-update-notavailable> tag, if present. 
	wxString GetUpdateNotAvailableMsg() const	{ return m_strUpdateNotAvailableMsg; }

private:
	DECLARE_CLASS(wxWebUpdateXMLScript)
};

#endif // _WX_WEBUPDATE_H_

