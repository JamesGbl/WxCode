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
#include "wx/xml/xml.h"
#include "wx/url.h"
#include "wx/log.h"
#include "wx/textfile.h"


// defined later
class WXDLLIMPEXP_WEBUPDATE wxDownloadThread;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateDownload;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdatePackage;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateLocalPackage;

#if wxUSE_HTTPENGINE
class wxProxySettings;
#endif

//! A global wxWebUpdateDownload variable which contains empty (and thus invalid)
//! settings.
extern WXDLLIMPEXP_DATA_WEBUPDATE(wxWebUpdateDownload) wxEmptyWebUpdateDownload;
extern WXDLLIMPEXP_DATA_WEBUPDATE(wxWebUpdatePackage) wxEmptyWebUpdatePackage;
extern WXDLLIMPEXP_DATA_WEBUPDATE(wxWebUpdateLocalPackage) wxEmptyWebUpdateLocalPackage;

WXDLLIMPEXP_WEBUPDATE void wxUninitializeWebUpdate();

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
    wxWUCF_OUTOFDATE,    //!< The web server holds an updated version.
	wxWUCF_NOTINSTALLED	 //!< The package is not installed on the user's PC.
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



// the message sent to wxApp by wxWebUpdateAction::wxExecute
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBUPDATE, wxEVT_COMMAND_EXECUTE, -1);

#define EVT_EXECUTE(id, func)		\
	EVT_COMMAND(id, wxEVT_COMMAND_EXECUTE, func)





// --------------------------------
// LOG FUNCTIONS USED BY WEBUPDATE
// --------------------------------

enum
{
    wxLOG_UsrMsg = wxLOG_User,  // messages shown to the user + saved in the log file
    wxLOG_AdvMsg,				// messages saved in the log file only
    wxLOG_NewSection			// a new phase in program flow
};

DECLARE_LOG_FUNCTION(AdvMsg);
DECLARE_LOG_FUNCTION(UsrMsg);
DECLARE_LOG_FUNCTION(NewSection);

//! A wxLogPassThrough-derived log target for the WebUpdate classes.
//! This target processes in smart way the log strings sent by
//!   wxLogAdvMsg
//!   wxLogUsrMsg
//!   wxLogNewSection
//! global functions providing both facilities for saving the log in
//! a wxTextCtrl and into a wxTextFile.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateLog : public wxLogPassThrough
{
	wxTextCtrl *m_pTextCtrl;
	wxTextFile m_txtFile;

public:
	wxWebUpdateLog() { m_pTextCtrl=NULL; }
	virtual ~wxWebUpdateLog() { StopFileLog(); }

	void WriteUsrMsgAlsoToTextCtrl(wxTextCtrl *);
	void WriteAllMsgAlsoToFile(const wxString &filename);
	void StopFileLog();

	void DoLog(wxLogLevel, const wxChar *, time_t);
	void DoLogDecoratedString(wxLogLevel, const wxChar *);
	
private:
	DECLARE_CLASS(wxWebUpdateLog)
};




// -----------------------------------------------------------------------------
// CLASSES WHICH WORK WITH *LOCAL* FILES
// -----------------------------------------------------------------------------

//! A simple container of the two basic info which wxWebUpdate classes 
//! need to know about user *local* packages: the NAME and the local VERSION.
//! This class should not be confused with wxWebUpdatePackage which instead
//! contains the info parsed from a wxWebUpdateXMLScript about a package
//! on the *remote* server.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateLocalPackage : public wxObject
{
protected:

	//! The name of this package.
	wxString m_strName;

	//! The version of this package.
	wxVersion m_version;

public:
	wxWebUpdateLocalPackage(const wxString &name = wxEmptyString, 
							const wxVersion &ver = wxEmptyVersion)
		: m_strName(name), m_version(ver) {}
	virtual ~wxWebUpdateLocalPackage() {}


	bool IsOk() const
		{ return !m_strName.IsEmpty() && !m_version.IsEmpty(); }

	wxVersion GetVersion() const
		{ return m_version; }
	wxString GetName() const
		{ return m_strName; }

	//! Sets the version for this local package.
	void SetVersion(const wxVersion &v)
		{ m_version = v; }

private:
	DECLARE_CLASS(wxWebUpdateLocalPackage)
};

// a container of wxWebUpdateLocalPackage used by wxWebUpdater
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxWebUpdateLocalPackage, wxWebUpdateLocalPackageArray, WXDLLIMPEXP_WEBUPDATE);

//! This is the class which performs the XML parsing of the webupdate
//! LOCAL scripts; local scripts are used just to keep trace of the 
//! package installed on the PC and their versions.
//! See #wxWebUpdateXMLScript for the class which parses the XML remote
//! scripts (which are much more complex since they keep installation info).
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateLocalXMLScript : public wxXmlDocument
{
	//! The original un-substituted URI of the remote update script.
	//! This is kept for saving it unchanged in Save().
	wxString m_strRemoteURIOriginal;

	//! The original array of variable names of a <keywords> tag.
	//! This is kept for saving it unchanged in Save().
	wxArrayString m_arrNames;

	//! The original array of variable values of a <keywords> tag.
	//! This is kept for saving it unchanged in Save().
	wxArrayString m_arrValues;

protected:

	//! The version of this WebUpdate XML script.
	//! This is different from the XML version (which should always be "1.0");
	//! this is the version of the WebUpdate DTD which is followed by this doc.
	wxVersion m_strWebUpdateVersion;
	
	//! The name of the application whose packages must be updated.
	wxString m_strAppName;

	//! The XRC file to load.
	wxString m_strXRC;

	//! The name of the XRC resource file which should be used to update
	//! the local packages.
	wxString m_strXRCRes;
	
	//! The URI of the remote update script.
	wxString m_strRemoteURI;
	
	//! The application executable file name.
	wxString m_strAppFile;
	
	//! The URI of this local update script.
	//! Set by #Load and used in #Save.
	wxString m_strLocalURI;
	
	//! The content of the \<savelog\> tag.
	bool m_bSaveLog;
	
	//! The content of the \<restart\> tag.
	bool m_bRestart;
	
protected:

	//! Creates a wxWebUpdateLocalPackage from the given XML node.
	//! The caller must delete the returned pointer.
	wxWebUpdateLocalPackage *GetPackage(const wxXmlNode *package) const;

	//! Returns the text content of the given node.
	wxString GetNodeContent(const wxXmlNode *node) const;

	//! Builds the header of the local script and returns it.
	wxXmlNode *BuildHeader() const;
	
	//! Saves the given remote script URI.
	void SetRemoteScriptURI(const wxString &uri);

public:

	wxWebUpdateLocalXMLScript(const wxString &localURI = wxEmptyString);
	virtual ~wxWebUpdateLocalXMLScript() {}

public:		// main functions

	//! Returns TRUE if at least the root of this document is valid.
	bool IsOk() const;

	//! Returns TRUE if all the required info which are stored inside a local XML
	//! script (the remote XML script location, the XRC resource name... etc)
	//! have been correctly loaded.
	bool IsComplete() const;

    //! Parses the local XML script located at the given URI.
	//! This function can open any resource which can be handled
	//! by wxFileSystem but it should typically be used to load
	//! a file stored in the user's PC.
	//! Returns TRUE if the document was successfully parsed.
    virtual bool Load(const wxString &uri);
    
    //! Saves this XML file with the updated contents in the same location
    //! given to the #Load function (since local update scripts should always
    //! be kept in the fixed-name files).
    virtual bool Save() const;


public:		// getters

	//! Returns the wxWebUpdatePackage stored in this document for
	//! the given package. Returns NULL if the package you asked for
	//! is not present in this XML document.
	virtual wxWebUpdateLocalPackage *GetPackage(const wxString &packagename) const;

	//! Returns all the available packages from this XML document.
	virtual wxWebUpdateLocalPackageArray GetAllPackages() const;
	
	//! Returns the name of the application whose packages must be updated.
	wxString GetAppName() const
		{ return m_strAppName; }
		
	//! Returns the filename of the application to update.
	wxString GetAppFile() const
		{ return m_strAppFile; }
		
	//! Returns the nameof the XRC file to load.
	wxString GetXRC() const
		{ return m_strXRC; }
  		
	//! Returns the name of the XRC resources to use to update the packages.
	wxString GetXRCResName() const
		{ return m_strXRCRes; }
		
	//! Returns the URI for the WebUpdate remote script.
	wxString GetRemoteScriptURI() const
		{ return m_strRemoteURI; }
		
	//! Returns the URI for this WebUpdate local script.
	wxString GetLocalScriptURI() const
		{ return m_strLocalURI; }
		
	//! Returns TRUE if the program-to-update should be restarted.
	bool IsAppToRestart() const
		{ return m_bRestart; }

	//! Returns TRUE if the log file should be saved.
	bool IsLogToSave() const
		{ return m_bSaveLog; }

public: 	// setters

	//! Forces the removal of the old XML script and the reconstruction of the
	//! new one based on the given array.
	void SetPackageArray(const wxWebUpdateLocalPackageArray &arr);

	void OverrideXRC(const wxString &xrc)
		{ m_strXRC = xrc; }
	void OverrideXRCResName(const wxString &xrcres)
		{ m_strXRCRes = xrcres; }
	void OverrideRestartFlag(bool val)
		{ m_bRestart = val; }
	void OverrideSaveLogFlag(bool val)
		{ m_bSaveLog = val; }
	void OverrideRemoteScriptURI(const wxString &remote)
		{ SetRemoteScriptURI(remote); }
		
private:
	DECLARE_CLASS(wxWebUpdateLocalXMLScript)
};






// -----------------------------------------------------------------------------
// CLASSES WHICH WORK WITH *REMOTE* FILES
// -----------------------------------------------------------------------------

//! The base abstract class for the handler of an \<actions\> subtag of
//! a webupdate XML script. See webupdate.dtd for the specification of
//! the format of the WebUpdate XML script and for the description of
//! the standard wxWebUpdateAction-derived classes.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateAction : public wxObject
{
public:		// see the hack in wxApp::OnExecute

	//! Holds the return code of the last wxExecute which was executed by
	//! the wxApp under request of our ::wxExecute function.
	static int m_nExecResult;

protected:

	//! The name of this action.
	wxString m_strName;

	//! The list of properties for this action.
	wxArrayString m_arrPropName;

	//! The list of the property values for this action.
	wxArrayString m_arrPropValue;

public:
    wxWebUpdateAction(const wxString &name = wxEmptyString)
		: m_strName(name) { }

    virtual ~wxWebUpdateAction() {}
    
public:		// utilities

	//! This is a nice way to execute an arbitrary command in a thread-safe
	//! way since if this action is being executed from a secondary thread,
	//! it will then add an event in the main thread's queue which will exec
	//! the command. So, from the action-programmer's point of view, this
	//! wxExecute works transparently as the standard wxWidgets's one does.
	virtual long wxExecute(const wxString &command, int flags = wxEXEC_ASYNC) const;

public:		// miscellaneous
    
	//! Returns TRUE if this action was correctly initialized.
    bool IsOk() const
        { return !m_strName.IsEmpty() && 
			m_arrPropName.GetCount() == m_arrPropValue.GetCount(); }

	//! Returns the value for the given property or wxEmptyString if that
	//! property has not been set for this object.
	wxString GetPropValue(const wxString &propname) const
		{ int n=m_arrPropName.Index(propname); if (n!=wxNOT_FOUND) return m_arrPropValue.Item(n); return wxEmptyString; }

	//! Parses the properties of the XML action tag.
	//! Called by wxWebUpdateInstaller when the action tag handled
    //! by this class is found while parsing the remote XML script.
    //! This function should store inside the class' variables, the data extracted
    //! by the tag properties.
    //! The first array contains the names of the properties of the XML action tag while
    //! the second array contains the relative values.
    //! The two arrays always have the same number of elements.
	virtual bool SetProperties(const wxArrayString &propnames,
								const wxArrayString &propvalues) = 0;

	//! Run this action.
	//! This function is called by wxWebUpdateInstaller when the download of the
	//! relative package has been successfully completed and when all the previous
	//! actions have been successfully completed, too.
	//! This function contains the core implementation of the WebUpdate action.
	//! It should return TRUE if the action has been successfully completed or
	//! completed with warnings. It should return FALSE if the action could not
	//! be completed and the installation of the relative package should stop.
	virtual bool Run() const = 0;

	//! Returns a copy of this action.
	virtual wxWebUpdateAction *Clone() const = 0;
    
public:     // getters

    wxString GetName() const
        { return m_strName; } 

private:
	DECLARE_CLASS(wxWebUpdateAction)
};

// a container of wxWebUpdateAction used by wxWebUpdateDownload
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxWebUpdateAction*, wxWebUpdateActionArray, WXDLLIMPEXP_WEBUPDATE);

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

	//! The URL to the download. Remember that an URL does not mean esclusively
	//! strings starting with "http:"; URLs can also point to local files and
	//! start with "file:".
	//! Note: this info was initially kept in a wxURL variable but since
	//!       wxURL::GetInputStream() is deprecated in favour of wxFileSystem
	//!       and wxURI is not very comfortable to use; I prefer to use here a 
	//!       wxString which is easier to handle.    
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
	//!          See also #IsDownloadSizeCached().
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
         : m_platform(wxWUP_INVALID), m_strMD5(md5), m_urlDownload(url)
		{ SetPlatform(plat); m_size=1; if (actions) m_arrActions = *actions; }

    virtual ~wxWebUpdateDownload();

public:		// handle in the right way the issue of the m_arrActions array
			// which contains pointers to classes which needs to be copied
			// in order to avoid double-destruction (and thus invalid memory
			// accesses) of the same pointers !
	
	wxWebUpdateDownload(const wxWebUpdateDownload &tocopy) 
		: wxObject()		// avoid warnings with GCC
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

	//! Returns TRUE if the download size for this file has been already cached.
	bool IsDownloadSizeCached() const
		{ return m_size != 1; }
		
	//! Sets the download size for this file.
	void SetDownloadSize(int n)
		{ m_size=n; }

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
								wxEvtHandler *handler
#if wxUSE_HTTPENGINE
								, const wxProxySettings &proxy
#endif
								);

	//! Downloads this package synchronously and thus when the function returns
	//! the package has been completely downloaded (if result is TRUE).
	virtual bool DownloadSynch(const wxString &path
#if wxUSE_HTTPENGINE
								, const wxProxySettings &proxy
#endif
								);

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

public:		// utilities

	//! Returns in the given int* the parsed version numbers of the given wxVersion.
	static bool ExtractVersionNumbers(const wxVersion &str, int *maj, int *min, int *rel);

	//! Returns
	//!   1 when the package 1 is more updated than package 2
	//!   0 when the package 1 has the same version of package 2
	//!  -1 when the package 1 is older than package 2
	static int StdVersionCheck(int maj1, int min1, int rel1,
							int maj2, int min2, int rel2);
    
public:
    wxWebUpdatePackage(const wxString &id = wxEmptyString) : m_strID(id) {}
    virtual ~wxWebUpdatePackage() { m_arrWebUpdates.Clear(); }
    
public:		// package utilities

	//! Returns TRUE if this package was correctly initialized.
	bool IsOk() const
		{ return !m_strID.IsEmpty() && !m_strLatestVersion.IsEmpty() &&
				m_arrWebUpdates.GetCount() > 0; }

	//! Adds the given download package to the array.
	virtual void AddDownload(const wxWebUpdateDownload &toadd)
		{ m_arrWebUpdates.Add(toadd); }

	//! Returns a reference to the download package for the given platform string.
	virtual wxWebUpdateDownload &GetDownload(const wxString &platform) const
		{ return GetDownload(wxWebUpdateDownload::GetPlatformCode(platform)); }	
	
	//! Returns a reference to the download package for the given platform code.
	//! In each package it should exist only a single download for each platform
	//! so the returned object should be the only item which is designed for the
	//! given platform.
	virtual wxWebUpdateDownload &GetDownload(
				wxWebUpdatePlatform code = wxWUP_INVALID) const;

	//! Caches the download sizes of all contained packages.
	virtual void CacheDownloadSizes();

public:		// version check

    //! Checks if the webserver holds a more recent version of the given package.
	//! This function never returns the wxWUCF_NOTINSTALLED symbol since it's assumed
	//! that the version you give is the version of the same package installed on
	//! the user's PC.
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

	//! The version of this WebUpdate XML script.
	//! This is different from the XML version (which should always be "1.0");
	//! this is the version of the WebUpdate DTD which is followed by this doc.
	wxVersion m_strWebUpdateVersion;

	//! The content of the <msg-update-available> tag, if present.
    wxString m_strUpdateAvailableMsg;
	
	//! The content of the <msg-update-notavailable> tag, if present.
    wxString m_strUpdateNotAvailableMsg;

public:		// static utilities (used also by wxWebUpdateLocalXMLScript)

	//! Returns the ID (i.e. the name) of a package from the given
	//! XML node or wxEmptyString if the ID could not be found.
	static wxString GetPackageID(const wxXmlNode *package);
	
	//! Returns teh version property value stored in the given node.
	//! Does also the version check against wxWebUpdateInstaller's version.
	static wxWebUpdateCheckFlag GetWebUpdateVersion(const wxXmlNode *root, wxVersion &);

	//! Returns the text content of the given node.
	static wxString GetNodeContent(const wxXmlNode *node);
	
protected:	// internal utilities	
	
	//! Parses the given <actions> node.
	wxWebUpdateActionArray GetActionArray(const wxXmlNode *actions) const;

	//! Creates a wxWebUpdatePackage from the given XML node.
	//! The caller must delete the returned pointer.
	wxWebUpdatePackage *GetPackage(const wxXmlNode *package) const;

	//! Creates a wxWebUpdateDownload from the given XML node.
	wxWebUpdateDownload GetDownload(const wxXmlNode *latestdownload) const;

public:

	wxWebUpdateXMLScript(const wxString &strURI = wxEmptyString);
	virtual ~wxWebUpdateXMLScript() {}

public:		// main functions

	//! Returns TRUE if at least the root of this document is valid.
	bool IsOk() const
		{ if (!GetRoot() || GetRoot()->GetName() != wxT("webupdate")) return FALSE; return TRUE; }

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
	wxString GetUpdateAvailableMsg() const
 		{ return m_strUpdateAvailableMsg; }

	//! Returns the content of the <msg-update-notavailable> tag, if present. 
	wxString GetUpdateNotAvailableMsg() const
 		{ return m_strUpdateNotAvailableMsg; }

private:
	DECLARE_CLASS(wxWebUpdateXMLScript)
};

#endif // _WX_WEBUPDATE_H_

