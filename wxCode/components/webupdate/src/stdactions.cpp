/////////////////////////////////////////////////////////////////////////////
// Name:        stdactions.cpp
// Purpose:     wxWebUpdateAction-derived classes
// Author:      Francesco Montorsi
// Created:     2005/07/30
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
	#pragma implementation "stdactions.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
#include "wx/log.h"
#endif

#include "wx/webupdate.h"
#include "wx/stdactions.h"
#include "wx/installer.h"
#include <wx/wfstream.h>
#include <wx/filesys.h>
#include <wx/utils.h>
#include <wx/archive.h>
#include <wx/zipstrm.h>
#include <wx/msgdlg.h>
#include <wx/app.h>


// wxWidgets RTTI
DEFINE_EVENT_TYPE(wxEVT_COMMAND_EXECUTE);

// default wxWebUpdate actions
IMPLEMENT_CLASS(wxWebUpdateActionRun, wxWebUpdateAction)
IMPLEMENT_CLASS(wxWebUpdateActionExtract, wxWebUpdateAction)

#include <wx/ptr_scpd.h>
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveEntry);




// ---------------------
// wxWEBUPDATEACTIONRUN
// ---------------------

bool wxWebUpdateActionRun::Run() const
{
	// be sure that the file to run exists
	wxFileName f(m_strFile);
	if (!f.FileExists()) {

		wxLogDebug(wxT("wxWebUpdateActionRun::Run - the file \"") + m_strFile +
				wxT("\" does not exist; proceeding anyway (maybe it's in PATH)"));

		// proceed: the executable could be in the system path...
	}

	int retcode = wxExecute(m_strFile + wxT(" ") + m_strArgs, 
 							m_nExecFlag | wxEXEC_NODISABLE);

	// FIXME: how do we know if this retcode means success or not ?
	//        (some programs could not respect the 0=success UNIX standard...)	
	if (retcode != 0) 
		wxLogDebug(wxT("wxWebUpdateActionRun::Run - got a non-null return code for ")
					wxT("the last command; proceeding anyway..."));
	return TRUE;
}

bool wxWebUpdateActionRun::SetProperties(const wxArrayString &propnames,
										const wxArrayString &propvalues)
{
	wxString flags;

	m_strArgs = wxEmptyString;			// the ARGS default value
	for (int i=0; i < (int)propnames.GetCount(); i++) {
		wxLogDebug(wxT("wxWebUpdateActionRun::SetProperties - name: [")
				+ propnames[i] + wxT("], value: [") + propvalues[i] + wxT("]"));

		if (propnames[i] == wxT("args"))
			m_strArgs = propvalues[i];
		else if (propnames[i] == wxT("file") || propnames[i] == wxT("cmd"))
			m_strFile = propvalues[i];
		else if (propnames[i] == wxT("flags"))
			flags = propvalues[i];
		else
			wxLogDebug(wxT("wxWebUpdateActionRun::SetProperties - unknown property: ") 
						+ propnames[i]);
	}

	// set defaults	
	if (flags.IsEmpty())
		m_nExecFlag = wxEXEC_ASYNC;		// the FLAGS default value
	else if (flags == wxT("ASYNC"))
		m_nExecFlag = wxEXEC_ASYNC;
	else if (flags == wxT("SYNC"))
		m_nExecFlag = wxEXEC_SYNC;
	else {
		m_nExecFlag = wxEXEC_ASYNC;
		wxLogDebug(wxT("wxWebUpdateActionRun::SetProperties - unknown exec flag: ") 
						+ flags);
	}

	if (m_strFile.IsEmpty())
		m_strFile = wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("thisfile"));

	// validate the properties
	wxFileName f(m_strFile);			// the FILE property is required !

	// we won't do the wxFileName::FileExists check because the file we need to run
	// could be a file which does not exist yet (e.g. its in the update package)
	if (!f.IsOk()) 
		return FALSE;

	return TRUE;
}




// --------------------------
// wxWEBUPDATEACTIONEXTRACT
// --------------------------

bool wxWebUpdateActionExtract::Run() const
{
	wxArrayString orig, output;
	wxLogDebug(wxT("wxWebUpdateActionExtract::Run - going to extract the file [")
				+ m_strFile + wxT("] of type [") + m_strType + wxT("] in\n\n")
				+ m_strWhere + wxT("\n\n"));

	// wxFileName wants a path separator at the end of directory names
	wxString dir(m_strWhere);
	if (dir.Last() != wxFileName::GetPathSeparator())
		dir += wxFileName::GetPathSeparator();

	// be sure that the destination directory exists
	wxFileName f(dir), f2(m_strFile);
	if (!f.DirExists() || !f2.FileExists()) {

		wxLogDebug(wxT("wxWebUpdateActionExtract::Run - the folder \"") + m_strWhere +
				wxT("\" or the file \"") + m_strFile + wxT("\" does not exist !"));
		return FALSE;
	}
	
	// parse the namemap
	wxArrayString compressed, extracted;
	int count = wxWebUpdateInstaller::Get()->ParsePairValueList(m_strNameMap, compressed, extracted);

	// do the substitutions also on the compressed filenames
	for (int i=0; i < count; i++)
		compressed[i] = wxWebUpdateInstaller::Get()->DoSubstitution(compressed[i]);

	// create the archive factory
	wxArchiveClassFactory *factory = NULL;
	if (m_strType == wxT("zip"))
		factory = new wxZipClassFactory;	

	// extract the package 
	wxArchiveEntryPtr entry;
	wxFFileInputStream input(m_strFile);
	wxArchiveInputStream *in = factory->NewStream(input);
	delete factory;

    while (entry.reset(in->GetNextEntry()), entry.get() != NULL)
    {
        // access meta-data
        wxString name = entry->GetName();
		if (entry->IsDir()) {

			if (!wxMkdir(name))
				wxLogDebug(wxT("wxWebUpdateActionExtract::Run - could not create the [") + name + wxT("] folder.. proceeding anyway"));
			else
				wxLogDebug(wxT("wxWebUpdateActionExtract::Run - created the [") + name + wxT("] folder"));
		
			continue;		// this entry contains no data
		}

        wxString output = dir + name;
        
        // is this file registered in the name map ?
        int idx = compressed.Index(name);
        if (idx != wxNOT_FOUND)
        	output = dir + extracted[idx];
        
		wxLogDebug(wxT("wxWebUpdateActionExtract::Run - extracting [") + name +
			wxT("] as [") + output + wxT("]..."));

        // now just dump this entry to a new uncompressed file...
		wxFileOutputStream out(output);
		if (!out.IsOk() || !out.Write(*in)) {

			wxLogDebug(wxT("wxWebUpdateActionExtract::Run - couldn't decompress ") + name);
			delete in;
			return FALSE;
		}
    }

	delete in;
	return TRUE;
}

bool wxWebUpdateActionExtract::SetProperties(const wxArrayString &propnames,
										const wxArrayString &propvalues)
{
	wxString flags;

	for (int i=0; i < (int)propnames.GetCount(); i++) {
		wxLogDebug(wxT("wxWebUpdateActionExtract::SetProperties - name: [")
				+ propnames[i] + wxT("], value: [") + propvalues[i] + wxT("]"));

		if (propnames[i] == wxT("where"))
			m_strWhere = propvalues[i];
		else if (propnames[i] == wxT("file"))
			m_strFile = propvalues[i];
		else if (propnames[i] == wxT("type"))
			m_strType = propvalues[i];
		else if (propnames[i] == wxT("namemap"))
			m_strNameMap = propvalues[i];
		else
			wxLogDebug(wxT("wxWebUpdateActionExtract::SetProperties - unknown property: ") 
						+ propnames[i]);
	}
	
	// by default, WebUpdater executables must be extracted with the '_'
	// character prepended
	wxString str = wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("updatername")) +
					wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("exe"));
	m_strNameMap = str + wxT("=_") + str + m_strNameMap;

	// set defaults
	if (m_strFile.IsEmpty())		// the FILE default value is $(thisfile)
		m_strFile = wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("thisfile"));
	if (m_strWhere.IsEmpty())		// the WHERE default value is $(programdir)
		m_strWhere = wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("programdir"));
	if (m_strType.IsEmpty())		// the TYPE default value is "zip"
		m_strType = wxT("zip");

	// validate the properties
	wxFileName f(m_strWhere), f2(m_strFile);

	// we won't do the wxFileName::FileExists check because the file we need to run
	// could be a file which does not exist yet (e.g. its in the update package)
	//
	// NOTE: wxFileName::IsDir() only checks if the given string ends with a path
	//       separator character (there are no real ways to do a ::IsDir check
	//       without trying to access that path!) and thus we won't use it
	if (m_strWhere.IsEmpty() || m_strFile.IsEmpty() || 
		!f.IsOk() || !f2.IsOk()) 
		return FALSE;

	return TRUE;
}


