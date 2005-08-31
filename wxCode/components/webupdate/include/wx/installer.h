/////////////////////////////////////////////////////////////////////////////
// Name:        installer.h
// Purpose:     wxWebUpdateInstaller and wxWebUpdateInstallThread
// Author:      Francesco Montorsi
// Created:     2005/07/14
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_INSTALLER_H_
#define _WX_INSTALLER_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "installer.h"
#endif

// wxWidgets headers
#include "wx/webupdatedef.h"		// for WXDLLIMPEXP_WEBUPDATE macros only
#include "wx/webupdate.h"


//! The version of the supported local & remote XML scripts.
#define wxWUI_VERSION_MAJOR			1
#define wxWUI_VERSION_MINOR			0
#define wxWUI_VERSION_RELEASE		0		// should always be zero (not used)
#define wxWUI_VERSION_STRING		wxT("1.0")

// defined later
class WXDLLIMPEXP_WEBUPDATE wxDownloadThread;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateDlg;

// this is the even sent by a wxDownloadThread class to the wxEvtHandler
// which is given it in its constructor.
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBUPDATE, wxEVT_COMMAND_INSTALLATION_COMPLETE, -1);

#define EVT_INSTALLATION_COMPLETE(id, func)		\
	EVT_COMMAND(id, wxEVT_COMMAND_INSTALLATION_COMPLETE, func)


//! The possible values of the wxDownloadThread::m_nStatus variable.
enum wxWebUpdateInstallThreadStatus {
	wxWUITS_WAITING = -1,
	wxWUITS_INSTALLING
};

// an hash map with wxString as keys and wxWebUpdateAction as values.
// Used by wxWebUpdateInstaller.
WX_DECLARE_STRING_HASH_MAP_WITH_DECL(wxWebUpdateAction*, wxWebUpdateActionHashMap, class WXDLLIMPEXP_WEBUPDATE);

// used by wxWebUpdateInstaller to store the keywords to substitute in the XML webupdate script
WX_DECLARE_STRING_HASH_MAP_WITH_DECL(wxString, wxStringStringHashMap, class WXDLLIMPEXP_WEBUPDATE);



//! A singleton class which contains the list with all the 
//! registered wxWebUpdateAction which are recognized in the webupdate
//! script in installation stage.
//! To register a new wxWebUpdateAction-derived class, then do:
//!    
//!    wxWebUpdateInstaller::Get()->AddActionHandler(new myDerivedAction);
//!
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateInstaller : public wxObject
{
protected:

	//! The global instance of this class.
	static wxWebUpdateInstaller *m_pTheInstaller;

	//! The list of all registered wxWebUpdateAction-derived classes.
	wxWebUpdateActionHashMap m_hashActions;

	//! The hash map with all registered keywords to substitute.
	wxStringStringHashMap m_hashKeywords;

public:
	wxWebUpdateInstaller() 
		{ InitDefaultKeywords(); InitDefaultActions(); }
	virtual ~wxWebUpdateInstaller() 
		{ FreeActionHashMap(); FreeKeywords();
		  /* user needs to delete the global wxWebUpdateInstaller object !! 
		     using  
		             delete wxWebUpdateInstaller::Set(NULL);
		     code in its wxApp::OnExit() function */ }

public:		// single ton accessors

    //! Gets the global wxWebUpdateInstaller object or creates one if none exists.
    static wxWebUpdateInstaller *Get()
	{ if (!m_pTheInstaller) m_pTheInstaller = new wxWebUpdateInstaller(); return m_pTheInstaller;}

    //! Sets the global wxWebUpdateInstaller object and returns a pointer to the 
	//! previous one (may be NULL).
    static wxWebUpdateInstaller *Set(wxWebUpdateInstaller *res)
	{ wxWebUpdateInstaller *old = m_pTheInstaller; m_pTheInstaller = res; return old; }


public:		// global utilities

	//! Shows to the user a simple wxMessageBox with the error description
	//! customized for the current application. Also logs it.
	void ShowErrorMsg(const wxString &);
	
	//! Shows to the user a notification message. Also logs it.
	void ShowNotificationMsg(const wxString &, const wxString &title = wxEmptyString);

public:		// supported version for the local & remote XML scripts

	//! Returns the version of the local & remote XML scripts which are
	//! supported by this wxWebUpdateInstaller.
	//! This version is also the version of the WebUpdater application.
	wxVersion GetVersion() const
		{ return wxWUI_VERSION_STRING; }
		
	//! Does the version check for the given wxVersion object.
	wxWebUpdateCheckFlag VersionCheck(const wxVersion &v) const;

public:		// action hashmap

	//! Adds the handler for a new action.
	//! To understand how to add support for a new action in the remote XML
 	//! script, look at the WebUpdate docs.
	void AddActionHandler(wxWebUpdateAction *custom)
		{ if (!custom) return; m_hashActions[custom->GetName()] = custom; }

	//! Creates a new action to handle an \<actions\> subtag with the given name.
	//! The \c propnames and \c propvalues parameters are the array of the
	//! property names and property values found in the tag.
	//! This function is called by wxWebUpdateXMLScript only and should not be
	//! called by the user.
	wxWebUpdateAction *CreateNewAction(const wxString &name, 
		const wxArrayString *propnames, const wxArrayString *propvalues);

	//! Returns a reference to the entire action hashmap.
	wxWebUpdateActionHashMap &GetActionHashMap()
		{ return m_hashActions; }

	//! Initializes the default actions (see stdactions.h).
	virtual void InitDefaultActions();
	
	//! Removes all actions from the hash map.
	virtual void FreeActionHashMap();


public:		// keywords hashmap

	//! Returns a reference to the entire keywords hashmap.
	wxStringStringHashMap &GetKeywords()
		{ return m_hashKeywords; }

	//! Returns the value of the given keyword or wxEmptyString if such
	//! keyword does not exist.
	wxString GetKeywordValue(const wxString &name)
		{ return m_hashKeywords[name]; }

	//! Creates a key with the given name & value.
	//! If such key already existed, then it is overwritten.
	void SetKeywordValue(const wxString &name, const wxString &val)
		{ m_hashKeywords[name] = val; }

	//! Does $(xxx) substitutions on the given text and then returns it.
	wxString DoKeywordSubstitution(const wxString &str);

	//! Does the '//' string substitution; i.e. substitutes the '//'
	//! characters into '\' char for win32 and '/' char for unix-based OS.
	wxString DoPathSubstitution(const wxString &str);
	
	//! Does both the keyword and the path substitution.
	wxString DoSubstitution(const wxString &str)
		{ return DoPathSubstitution(DoKeywordSubstitution(str)); }
		
	//! Parses the given comma-separed list of tokens in the form name=value
	//! and saves all the names & values in the two given wxArrayString.
	//! Calls #DoSubstitution on the values.
	//! Returns the number of names/values saved in the arrays.
	int ParsePairValueList(const wxString &str, wxArrayString &names, wxArrayString &values);

	//! Initializes the default keywords (and their values!).
	virtual void InitDefaultKeywords();
	
	//! Does the cleanup for the keywords hashmap.
	//! Since the hashmap cleans itself automatically when it's deleted,
	//! this function should just remove temporary files/folders/variables
	//! allocated by #InitDefaultKeywords.
	virtual void FreeKeywords();

private:
	DECLARE_CLASS(wxWebUpdateInstaller)
};


//! The thread used to install the packages.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateInstallThread : public wxThread
{
protected:		// these are written by this thread and they must be only read
				// by other classes; making them protected and exposing only
				// getters for these vars ensures that we do not need to use
				// mutexes...
	
	//! TRUE if the package was installed successfully.
	bool m_bSuccess;

	//! The number of packages installed by this thread.
	int m_nInstallationCount;

public:		// to avoid setters/getters (these vars are only read by this thread;
			// they are never written and so they are not protected with mutexes).

	//! The wxEvtHandler which will receive our wxDT_NOTIFICATION events.
	wxEvtHandler *m_pHandler;

	//! The downloaded file which is our update package.
	wxString m_strUpdateFile;

	//! The package which contains the wxWebUpdateAction to be executed.
	//! This is a pointer to avoid unwanted copies (both for performances and
	//! to avoid duplicate log messages).
	wxWebUpdateDownload *m_pDownload;

protected:		// these are vars protected by mutexes...

	//! The value of this variable represents the current thread status;
	//! see the #wxWebUpdateInstallThreadStatus enum for more info.
	wxWebUpdateInstallThreadStatus m_nStatus;

	//! The mutex over the #m_nStatus var.
	wxMutex m_mStatus;

	//! The package which is currently being installed.
	int m_nCurrentIndex;

public:
	wxWebUpdateInstallThread(wxEvtHandler *dlg = NULL)
		: wxThread(wxTHREAD_JOINABLE), m_pHandler(dlg)
		{ m_bSuccess=TRUE; m_nInstallationCount=0; m_nCurrentIndex=0;
		  m_nStatus = wxWUITS_WAITING; m_pDownload=NULL; }

	virtual ~wxWebUpdateInstallThread() {}


    //! Installs the packages.
    virtual void *Entry();


public:		// current status
	
	//! Returns TRUE if this thread is installing a package.
	bool IsInstalling() const
		{ return (IsRunning() && m_nStatus == wxWUITS_INSTALLING); }

	//! Returns TRUE if this thread is running but it's not installing anything.
	bool IsWaiting() const 
		{ return (IsRunning() && m_nStatus == wxWUITS_WAITING); }

	//! Returns the current status of the thread.
	//! please note that this status is completely independent from the
	//! running/paused status of a simple wxThread.
	//! The returned status refers only to the action currently performed
	//! by wxDownloadThread when it's running.
	wxWebUpdateInstallThreadStatus GetStatus() const
		{ return m_nStatus; }

public:		// miscellaneous

	//! Starts a new installation.
	//! This function must be called only when this thread is not installing anything else.
	void BeginNewInstall() {
		wxASSERT(!IsInstalling());
		wxMutexLocker lock(m_mStatus);
		m_nStatus = wxWUITS_INSTALLING;
	}

	//! Aborts the current installation.
	void AbortInstall() {
		wxASSERT(IsInstalling());
		wxMutexLocker lock(m_mStatus);
		m_nStatus = wxWUITS_WAITING;
	}

	//! Returns TRUE if the last installation was successful.
	bool InstallationWasSuccessful() const		
		{ return m_bSuccess; }

	//! Returns the number of packages successfully installed by this thread.
	int GetInstallationCount() const
		{ return m_nInstallationCount; }
};


#endif // _WX_INSTALLER_H_

