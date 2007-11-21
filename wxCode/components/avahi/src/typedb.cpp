/////////////////////////////////////////////////////////////////////////////
// Name:        typedb.cpp
// Purpose:     Avahi service type database.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: client.cpp,v 1.4 2005/10/21 16:42:58 frm Exp $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
	#include <wx/tokenzr.h>
	#include <wx/filesys.h>
	#include <wx/txtstrm.h>
#endif

#include "wx/avahi/typedb.h"


//
// wxAvahiServiceTypeDatabase
//
wxAvahiServiceTypeDatabase* wxAvahiServiceTypeDatabase::s_database = NULL;

wxAvahiServiceTypeDatabase& wxAvahiServiceTypeDatabase::GetDatabase()
{
	if(s_database==NULL)
		s_database = new wxAvahiServiceTypeDatabase;
	return *s_database;
}


wxAvahiServiceTypeDatabase::wxAvahiServiceTypeDatabase(const wxString& path):
	wxAvahiServiceTypeMap()
{
	if(!path.IsEmpty())
		m_paths = wxStringTokenize(path, wxT("|"));
	else
		m_paths = wxStringTokenize(GetDefaultPath(), wxT("|"));

	Evaluate();
}

wxAvahiServiceTypeDatabase::~wxAvahiServiceTypeDatabase()
{
}

wxString wxAvahiServiceTypeDatabase::GetDefaultPath()
{
	return wxT("file:///usr/share/avahi/service-types|file:///usr/local/share/avahi/service-types");
}

void wxAvahiServiceTypeDatabase::SetServiceName(const wxString& service, const wxString& name, const wxString& lang)
{
	operator[](service).operator[](lang) = name;
}

wxString wxAvahiServiceTypeDatabase::GetServiceName(const wxString& service, const wxString& lang, bool bDefaultIfNotPresent)const
{
	wxAvahiServiceTypeMap::const_iterator iter1 = find(service);
	if(iter1==end())
		return wxT("");
	wxAvahiLangServiceTypeName::const_iterator iter2 = iter1->second.find(lang);
	if(iter2==iter1->second.end())
	{
		if(!bDefaultIfNotPresent)
			return wxT("");
		else
		{
			iter2 = iter1->second.find(wxT(""));
			if(iter2==iter1->second.end())
				return wxT("");
		}
	}
		
	return iter2->second;
}


void wxAvahiServiceTypeDatabase::Evaluate()
{
	wxFileSystem fs;
	for(int i=0; i<(int)m_paths.GetCount(); i++)
	{
		wxFSFile* file = fs.OpenFile(m_paths[i]);
		if(file)
		{
			wxInputStream* is = file->GetStream();
			if(is)
			{
				wxTextInputStream stm(*is);
				while(!is->Eof())
				{
					wxString str = stm.ReadLine();
					str = str.BeforeFirst(wxT('#'));
					str.Trim(true).Trim(false);
					if(!str.IsEmpty())
					{
						wxString value = str.AfterFirst(wxT(':'));
						str = str.BeforeFirst(wxT(':'));
						wxString name = str.BeforeFirst(wxT('['));
						str = str.AfterFirst(wxT('['));
						wxString lang = str.BeforeFirst(wxT(']'));
						
						str = name + wxT("[") + lang + wxT("] = ") + value;
						
						SetServiceName(name, value, lang);
					}
				}
			}
			delete file;
		}
	}
}
