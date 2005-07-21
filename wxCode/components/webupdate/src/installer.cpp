/////////////////////////////////////////////////////////////////////////////
// Name:        installer.cpp
// Purpose:     wxBaseInstaller and wxZIPInstaller
// Author:      Francesco Montorsi
// Created:     2005/06/23
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
	#pragma implementation "installer.h"
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

#include "wx/installer.h"
#include <wx/wfstream.h>
#include <wx/filesys.h>
#include <wx/utils.h>
#include <wx/archive.h>
#include <wx/zipstrm.h>


// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateAction, wxObject)
IMPLEMENT_CLASS(wxWebUpdateInstaller, wxObject)

// default wxWebUpdate actions
IMPLEMENT_CLASS(wxWebUpdateActionRun, wxWebUpdateAction)
IMPLEMENT_CLASS(wxWebUpdateActionExtract, wxWebUpdateAction)


#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxWebUpdateActionArray);

#include <wx/ptr_scpd.h>
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveEntry);



// global objects
wxWebUpdateInstaller *wxWebUpdateInstaller::m_pTheInstaller = NULL;




// ---------------------
// wxWEBUPDATEACTIONRUN
// ---------------------

bool wxWebUpdateActionRun::Run() const
{
	// be sure that the file to run exists
	wxFileName f(m_strFile);
	if (!f.FileExists()) {

		wxLogDebug(wxT("wxWebUpdateActionRun::Run - the file \"") + m_strFile +
				wxT("\" does not exist !"));
		return FALSE;
	}

	// launch the file
	long res = wxExecute(m_strFile + wxT(" ") + m_strArgs, m_nExecFlag | wxEXEC_NODISABLE);
	if ((m_nExecFlag & wxEXEC_SYNC) && res != -1)
		return TRUE;
	if ((m_nExecFlag & wxEXEC_ASYNC) && res != 0)
		return TRUE;
	return FALSE;
}

bool wxWebUpdateActionRun::SetProperties(const wxArrayString &propnames,
										const wxArrayString &propvalues)
{
	wxString flags;

	for (int i=0; i < (int)propnames.GetCount(); i++) {
		if (propnames[i] == wxT("args"))
			m_strArgs = propvalues[i];
		else if (propnames[i] == wxT("file"))
			m_strFile = propvalues[i];
		else if (propnames[i] == wxT("flags"))
			flags = propvalues[i];
		else
			wxLogDebug(wxT("wxWebUpdateActionRun::SetProperties - unknown property: ") 
						+ propnames[i]);
	}

	// set defaults
	if (flags.IsEmpty())
		m_nExecFlag = wxEXEC_ASYNC;
	else if (flags == wxT("ASYNC"))
		m_nExecFlag = wxEXEC_ASYNC;
	else if (flags == wxT("SYNC"))
		m_nExecFlag = wxEXEC_SYNC;
	else {
		m_nExecFlag = wxEXEC_ASYNC;
		wxLogDebug(wxT("wxWebUpdateActionRun::SetProperties - unknown exec flag: ") 
						+ flags);
	}

	// validate the properties
	wxFileName f(m_strFile);

	// we won't do the wxFileName::FileExists check because the file we need to run
	// could be a file which does not exist yet (e.g. its in the update package)
	if (m_strFile.IsEmpty() || !f.IsOk() || f.IsDir()) 
		return FALSE;

	return TRUE;
}




// --------------------------
// wxWEBUPDATEACTIONEXTRACT
// --------------------------

bool wxWebUpdateActionExtract::Run() const
{
	// be sure that the destination directory exists
	wxFileName f(m_strWhere), f2(m_strFile);
	if (!f.DirExists() || !f2.FileExists()) {

		wxLogDebug(wxT("wxWebUpdateActionExtract::Run - the folder \"") + m_strWhere +
				wxT("\" or the file \"") + m_strFile + wxT("\" does not exist !"));
		return FALSE;
	}

	// create the archive factory
	wxArchiveClassFactory *factory = new wxZipClassFactory;
	

	// extract the package 
	wxArchiveEntryPtr entry;
	wxFFileInputStream input(m_strFile);
	wxArchiveInputStream *in = factory->NewStream(input);

    while (entry.reset(in->GetNextEntry()), entry.get() != NULL)
    {
        // access meta-data
        wxString name = entry->GetName();

        // now just dump this entry to a new uncompressed file...
		wxFileOutputStream out(m_strWhere + name);
		if (!out.Write(*in)) {

			wxLogDebug(wxT("wxWebUpdateActionExtract::Run - couldn't decompress ") + name);
			delete in;
			return FALSE;
		}
    }

	delete in;
	return FALSE;
}

bool wxWebUpdateActionExtract::SetProperties(const wxArrayString &propnames,
										const wxArrayString &propvalues)
{
	wxString flags;

	for (int i=0; i < (int)propnames.GetCount(); i++) {
		if (propnames[i] == wxT("where"))
			m_strWhere = propvalues[i];
		else if (propnames[i] == wxT("file"))
			m_strFile = propvalues[i];
		else
			wxLogDebug(wxT("wxWebUpdateActionExtract::SetProperties - unknown property: ") 
						+ propnames[i]);
	}

	// set defaults
	if (m_strFile.IsEmpty())
		m_strFile = wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("thisfile"));

	// validate the properties
	wxFileName f(m_strWhere), f2(m_strFile);

	// we won't do the wxFileName::FileExists check because the file we need to run
	// could be a file which does not exist yet (e.g. its in the update package)
	if (m_strWhere.IsEmpty() || m_strFile.IsEmpty() || 
		!f.IsOk() || !f.IsDir() || !f2.IsOk() || f2.IsDir()) 
		return FALSE;

	return TRUE;
}






// ---------------------
// wxWEBUPDATEINSTALLER
// ---------------------

void wxWebUpdateInstaller::InitDefaultKeywords()
{	
	wxChar sep = wxFileName::GetPathSeparator();

	// a temporary folder
	m_hashKeywords[wxT("tempdir")] = 
		wxFileName::CreateTempFileName(wxT("webupdate")).BeforeLast(sep);

	// the program root folder
	m_hashKeywords[wxT("programroot")] = wxGetCwd();

	// the program process ID
	m_hashKeywords[wxT("pid")] = wxString::Format(wxT("%d"), wxGetProcessId());

	// the program name
	if (wxTheApp) 
		m_hashKeywords[wxT("appname")] = wxTheApp->GetAppName();	
#ifdef __WXDEBUG__
	else
		wxLogDebug(wxT("wxWebUpdateInstaller::InitDefaultKeywords - wxTheApp is not initialized !"));
#endif

	// the name of the main frame of the program
	if (wxTheApp && wxTheApp->GetTopWindow()) 
		m_hashKeywords[wxT("framename")] = wxTheApp->GetTopWindow()->GetTitle();
#ifdef __WXDEBUG__
	else
		wxLogDebug(wxT("wxWebUpdateInstaller::InitDefaultKeywords - wxTheApp/the top window is not initialized !"));
#endif
}

void wxWebUpdateInstaller::InitDefaultActions()
{
	m_hashActions[wxT("run")] = new wxWebUpdateActionRun();
	m_hashActions[wxT("extract")] = new wxWebUpdateActionExtract();
}

wxWebUpdateAction *wxWebUpdateInstaller::CreateNewAction(const wxString &name, 
						const wxArrayString *names, const wxArrayString *values)
{
	// search in our hashmap for the given keyname
	if (m_hashActions[name] != NULL) {
	
		wxWebUpdateAction *handler = m_hashActions[name]->Clone();
		if (names && values) 
			if (!handler->SetProperties(*names, *values))
				wxLogDebug(wxT("wxWebUpdateInstaller::CreateNewAction - couldn't ")
						wxT("set correctly the properties for the new action"));

		return handler;
	}

	return NULL;		// unknown action name
}
