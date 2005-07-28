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
#include "wx/url.h"


// defined later
class WXDLLIMPEXP_WEBUPDATE wxDownloadThread;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateDownload;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdatePackage;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateLocalPackageArray;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateDlg;

extern WXDLLIMPEXP_DATA_WEBUPDATE(wxWebUpdateDownload) wxEmptyWebUpdateDownload;

// this is the even sent by a wxDownloadThread class to the wxEvtHandler
// which is given it in its constructor.
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBUPDATE, wxWUIT_INSTALLATION_COMPLETE, -1);

//! The possible values of the wxDownloadThread::m_nStatus variable.
enum wxWebUpdateInstallThreadStatus {
	wxWUITS_WAITING = -1,
	wxWUITS_INSTALLING
};

// the message sent to wxApp by wxWebUpdateActionExit
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBUPDATE, wxWUAE_EXIT, -1);

// the message sent to wxApp by wxWebUpdateActionRun
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_WEBUPDATE, wxWUAR_EXECUTE, -1);


//! The base abstract class for the handler of an <action> tag of
//! a webupdate script.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateAction : public wxObject
{
protected:

	//! The name of this action.
	wxString m_strName;

	//! The list of properties for this action.
	wxArrayString m_arrPropName;

	//! The list of the property values for this action.
	wxArrayString m_arrPropValue;

public:
    wxWebUpdateAction(const wxString &name = wxEmptyString)
         : m_strName(name) {}

    virtual ~wxWebUpdateAction() {}

public:		// miscellaneous
    
	//! Returns TRUE if this action was correctly initialized.
    bool IsOk() const
        { return !m_strName.IsEmpty() && 
			m_arrPropName.GetCount() == m_arrPropValue.GetCount(); }

	//! Returns the value for the given property or wxEmptyString if that
	//! property has not been set for this object.
	wxString GetPropValue(const wxString &propname) const
		{ int n=m_arrPropName.Index(propname); if (n!=wxNOT_FOUND) return m_arrPropValue.Item(n); return wxEmptyString; }

	//! Sets the property names & values for this action.
	virtual bool SetProperties(const wxArrayString &propnames,
							const wxArrayString &propvalues) = 0;

	//! Run this action.
	virtual bool Run() const = 0;

	//! Returns a copy of this action.
	virtual wxWebUpdateAction *Clone() const = 0;
    
public:     // getters

    wxString GetName() const
        { return m_strName; } 

private:
	DECLARE_CLASS(wxWebUpdateAction)
};


//! The "run" action.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateActionRun : public wxWebUpdateAction
{
protected:

	//! The arguments to give to the program which is run.
	wxString m_strArgs;

	//! The path & name of the executable file to run.
	wxString m_strFile;

	//! The run flags: wxEXEC_ASYNC, wxEXEC_SYNC, wxEXEC_NOHIDE.
	int m_nExecFlag;

public:
    wxWebUpdateActionRun()
         : wxWebUpdateAction(wxT("run")) {}
    virtual ~wxWebUpdateActionRun() {}

public:

	//! Sets the property names & values for this action.
	virtual bool SetProperties(const wxArrayString &propnames,
							const wxArrayString &propvalues);

	//! Run this action.
	virtual bool Run() const;

	//! Returns a copy of this action.
	virtual wxWebUpdateAction *Clone() const
		{ return new wxWebUpdateActionRun(*this); }

private:
	DECLARE_CLASS(wxWebUpdateActionRun)
};


//! The "extract" action.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateActionExtract : public wxWebUpdateAction
{
protected:

	//! The place where the contents of the file will be extracted.
	wxString m_strWhere;

	//! The file to extract.
	wxString m_strFile;

	//! The type of archive.
	wxString m_strType;

public:
    wxWebUpdateActionExtract()
         : wxWebUpdateAction(wxT("extract")) {}
    virtual ~wxWebUpdateActionExtract() {}

public:

	//! Sets the property names & values for this action.
	virtual bool SetProperties(const wxArrayString &propnames,
							const wxArrayString &propvalues);

	//! Run this action.
	virtual bool Run() const;

	//! Returns a copy of this action.
	virtual wxWebUpdateAction *Clone() const
		{ return new wxWebUpdateActionExtract(*this); }

private:
	DECLARE_CLASS(wxWebUpdateActionExtract)
};


#define wxWUAE_RESTART				1
#define wxWUAE_ASKUSER				2
#define	wxWUAE_NOTIFYUSER			4

//! The "exit" action.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateActionExit : public wxWebUpdateAction
{
protected:

	//! One of wxWUAE_RESTART, wxWUAE_ASKUSER, wxWUAE_NOTIFYUSER.
	int m_nFlags;

public:
    wxWebUpdateActionExit()
         : wxWebUpdateAction(wxT("exit")) {}
    virtual ~wxWebUpdateActionExit() {}

public:

	//! Sets the property names & values for this action.
	virtual bool SetProperties(const wxArrayString &propnames,
							const wxArrayString &propvalues);

	//! Run this action.
	virtual bool Run() const;

	//! Returns a copy of this action.
	virtual wxWebUpdateAction *Clone() const
		{ return new wxWebUpdateActionExit(*this); }

private:
	DECLARE_CLASS(wxWebUpdateActionExit)
};




// a container of wxWebUpdateAction used by wxWebUpdateDownload
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxWebUpdateAction*, wxWebUpdateActionArray, WXDLLIMPEXP_WEBUPDATE);

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

	wxStringStringHashMap &GetKeywords()
		{ return m_hashKeywords; }

	wxString GetKeywordValue(const wxString &name)
		{ return m_hashKeywords[name]; }

	void SetKeywordValue(const wxString &name, const wxString &val)
		{ m_hashKeywords[name] = val; }
	
	virtual void InitDefaultKeywords();
	virtual void FreeKeywords();
	virtual void InitDefaultActions();
	virtual void FreeActionHashMap();

private:
	DECLARE_CLASS(wxWebUpdateInstaller)
};



//! A singleton class which responds to the wxWebUpdateAction events.
//! This class code will be executed from the main thread.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdater : public wxEvtHandler
{
protected:

	//! The global instance of this class.
	static wxWebUpdater *m_pTheUpdater;

	//! The instance of the current wxWebUpdateDlg shown to the user.
	wxWebUpdateDlg *m_pWebUpdateDlg;

public:
	wxWebUpdater() { m_pWebUpdateDlg = NULL; }
	virtual ~wxWebUpdater() {}


public:		// to be implemented

	//! Returns the list of the currently installed local packages.
	virtual wxWebUpdateLocalPackageArray GetLocalPackages() const = 0;

public:		// event handlers

	void OnUpdateExit(wxCommandEvent &);
	void OnUpdateExec(wxCommandEvent &);
	void OnWebUpdateDlgShow(wxCommandEvent &);
	void OnWebUpdateDlgDestroy(wxCommandEvent &);

public:		// single ton accessors

    //! Gets the global wxWebUpdateInstaller object or creates one if none exists.
    static wxWebUpdater *Get()
	{ wxASSERT_MSG(m_pTheUpdater, 
		wxT("The wxWebUpdater class should have been created in wxApp::OnInit")); 
		return m_pTheUpdater;}

    //! Sets the global wxWebUpdateInstaller object and returns a pointer to the 
	//! previous one (may be NULL).
    static wxWebUpdater *Set(wxWebUpdater *res)
	{ wxWebUpdater *old = m_pTheUpdater; m_pTheUpdater = res; return old; }

private:
	DECLARE_CLASS(wxWebUpdateInstaller)
	DECLARE_EVENT_TABLE()
};



//! An installer entry for wxWebUpdateInstallThread.
class wxWebUpdateInstallThreadEntry 
{
public:		// to avoid setters/getters

	//! The downloaded file which is our update package.
	wxString m_strUpdateFile;

	//! The package which contains the wxWebUpdateAction to be executed.
	wxWebUpdateDownload &m_pDownload;

public:
	wxWebUpdateInstallThreadEntry() : m_pDownload(wxEmptyWebUpdateDownload) {}
	virtual ~wxWebUpdateInstallThreadEntry() {}
};

// a container of wxWebUpdateInstallThreadEntry used by wxWebUpdateInstallThread
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxWebUpdateInstallThreadEntry, wxWebUpdateInstallThreadEntryArray, WXDLLIMPEXP_WEBUPDATE);


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

	//! The packages to install.
	wxWebUpdateInstallThreadEntryArray m_entries;

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
	void QueueNewInstall(wxWebUpdateInstallThreadEntry &install) {
		//wxASSERT(!IsInstalling());
		m_entries.Add(install);
		wxMutexLocker lock(m_mStatus);
		m_nStatus = wxWUITS_INSTALLING;
	}

	//! Aborts the current installation.
	void AbortInstall() {
		wxASSERT(IsInstalling());
		wxMutexLocker lock(m_mStatus);
		m_nStatus = wxWUITS_WAITING;
	}

	//! Returns the current element which is being downloaded.
	wxWebUpdateInstallThreadEntry &GetCurrent()
		{ return m_entries[m_nCurrentIndex]; }

	//! Returns the oldest download in the queue (i.e. the first).
	wxWebUpdateInstallThreadEntry &GetOldest()
		{ return m_entries[0]; } 

	//! Returns TRUE if the last installation was successful.
	bool InstallationWasSuccessful() const		
		{ return m_bSuccess; }

	//! Returns the number of packages successfully installed by this thread.
	int GetInstallationCount() const
		{ return m_nInstallationCount; }
};


#endif // _WX_INSTALLER_H_

