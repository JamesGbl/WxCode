/////////////////////////////////////////////////////////////////////////////
// Name:        wx/avahi/typedb.h
// Purpose:     Avahi service type database.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef TYPEDB_H_
#define TYPEDB_H_

#include "wx/avahi/avahidef.h"

#include <wx/wx.h>

WX_DECLARE_STRING_HASH_MAP(wxString, wxAvahiLangServiceTypeName);
WX_DECLARE_STRING_HASH_MAP(wxAvahiLangServiceTypeName, wxAvahiServiceTypeMap);

class wxAvahiServiceTypeDatabaseModule;

/**
 * Dictionary to translate service type in service name (in different languages).
 * Use the avahi's "service-types" files.
 * You can specify a (list of) new path to look for this file.
 */
class WXDLLIMPEXP_AVAHI wxAvahiServiceTypeDatabase : public wxAvahiServiceTypeMap
{
	friend class wxAvahiServiceTypeDatabaseModule;
public:
	/** Constructor.
	 * \param path path where to search for "service-types" file.
	 */
	wxAvahiServiceTypeDatabase(const wxString& path=wxT(""));
	virtual ~wxAvahiServiceTypeDatabase();
	
	/** Read the db file and fill the dictionary.*/
	virtual void Evaluate();

	/** Add a service name translation.
	 * \param service Service type.
	 * \param name Name for the service type.
	 * \param lang Language code, "" for default entry (ie english).
	 */
	void SetServiceName(const wxString& service, const wxString& name, const wxString& lang=wxT(""));
	/** Get a service name.
	 * \param service Service type.
	 * \param lang Language to use, "" for default name.
	 * \param bDefaultIfNotPresent If true, use default name if service is not translated for the requested language.
	 */
	wxString GetServiceName(const wxString& service, const wxString& lang=wxT(""), bool bDefaultIfNotPresent = true)const;
	
	/** Default path where to search the db file;*/
	static wxString GetDefaultPath();
	
	/** Get the global database. */
	static wxAvahiServiceTypeDatabase& GetDatabase();
private:
	wxArrayString m_paths;
	static wxAvahiServiceTypeDatabase* s_database;
};

#endif /*TYPEDB_H_*/
