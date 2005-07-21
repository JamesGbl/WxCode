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
		{ return new wxWebUpdateActionRun(); }

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
		{ return new wxWebUpdateActionExtract(); }

private:
	DECLARE_CLASS(wxWebUpdateActionExtract)
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
		{ /* user needs to delete the global wxWebUpdateInstaller object !! 
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

	void InitDefaultKeywords();
	void InitDefaultActions();

private:
	DECLARE_CLASS(wxWebUpdateInstaller)
};

/*
//! The thread used to install the packages.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateInstallThread : public wxThread
{
public:		// to avoid setters/getters (these vars are only read by this thread;
			// they are never written and so they are not protected with mutexes).

	//! The wxEvtHandler which will receive our wxDT_NOTIFICATION events.
	wxEvtHandler *m_pHandler;

	//! The downloaded file which is our update package.
	wxString m_strUpdateFile;

	//! TRUE if the package was installed successfully.

public:
	wxWebUpdateInstallThread(wxEvtHandler *dlg, 
							const wxString &updatefile, 
							const wxWebUpdatePackage &toinstall)
		: wxThread(wxTHREAD_JOINABLE), m_pHandler(dlg), m_strUpdateFile(updatefile)		  
		{ m_bSuccess=FALSE; m_dtStart = wxDateTime::UNow(); }
	virtual ~wxWebUpdateInstallThread() {}

    //! Downloads the file and then sends the wxDT_NOTIFICATION event
	//! to the #m_pHandler event handler.
	//! Also sets the #m_bSuccess flag before exiting.
    virtual void *Entry();


private:
	DECLARE_CLASS(wxWebUpdateInstallThread)
};
*/

#endif // _WX_INSTALLER_H_

