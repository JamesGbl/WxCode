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


//! Contains the info about an available download.
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
    wxWebUpdateAction(const wxString &name = wxEmptyString, 
					const wxArrayString *propnames = NULL,
					const wxArrayString *propvalues = NULL)
         : m_strName(name)
	{ if (propnames) m_arrPropName=*propnames; 
		if (propvalues) m_arrPropValue=*propvalues; }

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

	//! Run this action.
	bool Run() const;
    
public:     // getters

    wxString GetName() const
        { return m_strName; } 

private:
	DECLARE_CLASS(wxWebUpdateAction)
};



// a container of wxWebUpdateAction used by wxWebUpdateDownload
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxWebUpdateAction, wxWebUpdateActionArray, WXDLLIMPEXP_WEBUPDATE);

// an hash map with wxString as keys and wxWebUpdateAction as values.
// Used by wxWebUpdateInstaller.
WX_DECLARE_STRING_HASH_MAP_WITH_DECL(wxWebUpdateAction, wxWebUpdateActionHashMap, class WXDLLIMPEXP_WEBUPDATE);

// used by wxWebUpdateInstaller to store the keywords to substitute in the XML webupdate script
WX_DECLARE_STRING_HASH_MAP_WITH_DECL(wxString, wxStringStringHashMap, class WXDLLIMPEXP_WEBUPDATE);


//! A singleton class which contains the hash map with all the 
//! registered wxWebUpdateAction which are recognized in the webupdate
//! script in installation stage.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateInstaller : public wxObject
{
protected:

	//! The global instance of this class.
	static wxWebUpdateInstaller *m_pTheInstaller;

	//! The hash map with all registered wxWebUpdateActions.
	wxWebUpdateActionHashMap m_hashActions;

	//! The hash map with all registered keywords to substitute.
	wxStringStringHashMap m_hashKeywords;

public:
	wxWebUpdateInstaller() 
		{ InitDefaultKeywords(); }
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

	wxWebUpdateActionHashMap &GetActionHashMap()
		{ return m_hashActions; }

	wxStringStringHashMap &GetKeywords()
		{ return m_hashKeywords; }

	void InitDefaultKeywords();

private:
	DECLARE_CLASS(wxWebUpdateInstaller)
};

#endif // _WX_INSTALLER_H_

