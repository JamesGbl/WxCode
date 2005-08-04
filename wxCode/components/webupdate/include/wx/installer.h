/////////////////////////////////////////////////////////////////////////////
// Name:        installer.h
// Purpose:     wxWebUpdateAction and wxWebUpdateInstaller
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
		{ FreeActionHashMap();
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


public:		// action hashmap

	void AddActionHandler(wxWebUpdateAction *custom)
		{ if (!custom) return; m_hashActions[custom->GetName()] = custom; }

	wxWebUpdateAction *CreateNewAction(const wxString &name, 
		const wxArrayString *propnames, const wxArrayString *propvalues);

	wxWebUpdateActionHashMap &GetActionHashMap()
		{ return m_hashActions; }

	virtual void InitDefaultActions();
	virtual void FreeActionHashMap();


public:		// keywords hashmap

	wxStringStringHashMap &GetKeywords()
		{ return m_hashKeywords; }

	wxString GetKeywordValue(const wxString &name)
		{ return m_hashKeywords[name]; }

	void SetKeywordValue(const wxString &name, const wxString &val)
		{ m_hashKeywords[name] = val; }

	//! Does string substitution using the current keyword hashmap.
	wxString DoKeywordSubstitution(const wxString &str);

	//! Does the '//' string substitution.
	wxString DoPathSubstitution(const wxString &str);

	virtual void InitDefaultKeywords();
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
	wxWebUpdateDownload &m_pDownload;

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
		: wxThread(wxTHREAD_JOINABLE), m_pHandler(dlg), 
  			m_pDownload(wxEmptyWebUpdateDownload)
		{ m_bSuccess=TRUE; m_nInstallationCount=0; m_nCurrentIndex=0;
		  m_nStatus = wxWUITS_WAITING; }

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

