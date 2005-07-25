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
#include "wx/webupdate.h"
#include "wx/webupdatedlg.h"
#include <wx/wfstream.h>
#include <wx/filesys.h>
#include <wx/utils.h>
#include <wx/archive.h>
#include <wx/zipstrm.h>


// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateAction, wxObject)
IMPLEMENT_CLASS(wxWebUpdateInstaller, wxObject)
DEFINE_EVENT_TYPE(wxWUIT_INSTALLATION_COMPLETE);
DEFINE_EVENT_TYPE(wxWUAE_EXIT);
DEFINE_EVENT_TYPE(wxWUAR_EXECUTE);

IMPLEMENT_CLASS(wxWebUpdater, wxEvtHandler)
BEGIN_EVENT_TABLE(wxWebUpdater, wxEvtHandler)
    EVT_COMMAND(wxID_ANY, wxWUAE_EXIT, wxWebUpdater::OnUpdateExit)
    EVT_COMMAND(wxID_ANY, wxWUAR_EXECUTE, wxWebUpdater::OnUpdateExec)
    EVT_COMMAND(wxID_ANY, wxWUD_INIT, wxWebUpdater::OnWebUpdateDlgShow)
	EVT_COMMAND(wxID_ANY, wxWUD_DESTROY, wxWebUpdater::OnWebUpdateDlgDestroy)
END_EVENT_TABLE()


// default wxWebUpdate actions
IMPLEMENT_CLASS(wxWebUpdateActionRun, wxWebUpdateAction)
IMPLEMENT_CLASS(wxWebUpdateActionExtract, wxWebUpdateAction)
IMPLEMENT_CLASS(wxWebUpdateActionExit, wxWebUpdateAction)

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxWebUpdateActionArray);

#include <wx/ptr_scpd.h>
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveEntry);



// global objects
wxWebUpdateInstaller *wxWebUpdateInstaller::m_pTheInstaller = NULL;
wxWebUpdater *wxWebUpdater::m_pTheUpdater = NULL;




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

	// unfortunately we cannot use ::wxExecute from a secondary thread
	// (and wxWebUpdateAction run from a wxWebUpdateInstallThread) so we
	// are forced to send a message to wxApp which launches the command for us
	wxCommandEvent runev(wxWUAR_EXECUTE);
	runev.SetString(m_strFile + wxT(" ") + m_strArgs);
	runev.SetInt(m_nExecFlag | wxEXEC_NODISABLE | wxEXEC_NODISABLE);
	wxWebUpdater::Get()->AddPendingEvent(runev);

	return TRUE;
}

bool wxWebUpdateActionRun::SetProperties(const wxArrayString &propnames,
										const wxArrayString &propvalues)
{
	wxString flags;

	m_strArgs = wxEmptyString;			// the ARGS default value
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

	// validate the properties
	wxFileName f(m_strFile);			// the FILE property is required !

	// we won't do the wxFileName::FileExists check because the file we need to run
	// could be a file which does not exist yet (e.g. its in the update package)
	if (m_strFile.IsEmpty() || !f.IsOk()) 
		return FALSE;

	return TRUE;
}




// --------------------------
// wxWEBUPDATEACTIONEXTRACT
// --------------------------

bool wxWebUpdateActionExtract::Run() const
{
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

        // now just dump this entry to a new uncompressed file...
		wxFileOutputStream out(dir + name);
		if (!out.Write(*in)) {

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
		if (propnames[i] == wxT("where"))
			m_strWhere = propvalues[i];
		else if (propnames[i] == wxT("file"))
			m_strFile = propvalues[i];
		else if (propnames[i] == wxT("type"))
			m_strType = propvalues[i];
		else
			wxLogDebug(wxT("wxWebUpdateActionExtract::SetProperties - unknown property: ") 
						+ propnames[i]);
	}

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



// --------------------------
// wxWEBUPDATEACTIONEXIT
// --------------------------

bool wxWebUpdateActionExit::Run() const
{
	int flags(m_nFlags);
	wxString msg = wxT("This programs needs to exit in order to update itself.\n");

	// tell the user that we are going to exit the application
	if (flags & wxWUAE_NOTIFYUSER) {

		if (flags & wxWUAE_RESTART)
			msg += wxT("Then it will automatically restart...");

		wxMessageBox(msg, wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("appname")));

	} else if (flags & wxWUAE_ASKUSER) {

		wxASSERT_MSG(m_nFlags & wxWUAE_RESTART, 
			wxT("If we just need to exit I have nothing to ask to the user"));
		
		int res = wxMessageBox(
			msg + wxT("Do you want to restart it immediately after the update ?"),
			wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("appname")),
			wxYES_NO);
		if (res == wxNO)
			flags &= ~wxWUAE_RESTART;	// remove the restart flag
	}


	// we need to exit our app	
	wxCommandEvent exitev(wxWUAE_EXIT);
	wxWebUpdater::Get()->AddPendingEvent(exitev);

	return TRUE;
}

bool wxWebUpdateActionExit::SetProperties(const wxArrayString &propnames,
										const wxArrayString &propvalues)
{
	wxString flags;

	for (int i=0; i < (int)propnames.GetCount(); i++) {
		if (propnames[i] == wxT("flags"))
			flags = propvalues[i];
		else
			wxLogDebug(wxT("wxWebUpdateActionExit::SetProperties - unknown property: ") 
						+ propnames[i]);
	}

	// set defaults
	m_nFlags = 0;
	if (flags.Contains(wxT("ASKUSER")))
		m_nFlags |= wxWUAE_ASKUSER;
	if (flags.Contains(wxT("NOTIFYUSER")))
		m_nFlags |= wxWUAE_NOTIFYUSER;
	if (flags.Contains(wxT("RESTART")))
		m_nFlags |= wxWUAE_RESTART;
	if (flags.IsEmpty())
		m_nFlags = wxWUAE_ASKUSER;		// the default value

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

	// the folder where we put the downloaded files
	m_hashKeywords[wxT("downloaddir")] = m_hashKeywords[wxT("tempdir")];		// by default it's the temp folder

	// a new temporary folder
	wxString newtempdir(m_hashKeywords[wxT("tempdir")] + sep + 
			wxT("webupdate") + wxDateTime::Now().Format(wxT("%d%H%M%S")));
	if (wxFileName::Mkdir(newtempdir))
		m_hashKeywords[wxT("newtempdir")] = newtempdir;

	// the program root folder
	m_hashKeywords[wxT("programdir")] = wxGetCwd();

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

void wxWebUpdateInstaller::FreeKeywords()
{
	// remove the newtempdir we created in #InitDefaultKeywords
	if (!wxFileName::Rmdir(m_hashKeywords[wxT("newtempdir")]))
		wxLogDebug(wxT("wxWebUpdateInstaller::FreeKeywords - could not remove the temporary folder I created"));
}

void wxWebUpdateInstaller::InitDefaultActions()
{
	m_hashActions[wxT("run")] = new wxWebUpdateActionRun();
	m_hashActions[wxT("extract")] = new wxWebUpdateActionExtract();
	m_hashActions[wxT("exit")] = new wxWebUpdateActionExit();
}

void wxWebUpdateInstaller::FreeActionHashMap()
{
	wxWebUpdateActionHashMap::iterator it;
    for ( it = m_hashActions.begin(); it != m_hashActions.end(); ++it ) {
        wxWebUpdateAction *p = it->second;
        if (p) delete p;
    }
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
						wxT("set correctly the properties for the new action ") +
						name);

		return handler;
	}

	return NULL;		// unknown action name
}



// -------------------------
// wxWEBUPDATEINSTALLTHREAD
// -------------------------

// this macro avoid the repetion of a lot of code;
#define ABORT_INSTALL() {								\
			wxLogDebug(wxT("wxWebUpdateInstallThread::Entry - INSTALLATION ABORTED !!!"));		\
			m_bSuccess = FALSE;							\
			m_mStatus.Lock();							\
			m_nStatus = wxWUITS_WAITING;				\
			m_mStatus.Unlock();							\
			wxPostEvent(m_pHandler, updatevent);		\
			continue;									\
	}

void *wxWebUpdateInstallThread::Entry()
{
	// we'll use wxPostEvent to post this event since this is the
	// only thread-safe way to post events !
	wxCommandEvent updatevent(wxWUIT_INSTALLATION_COMPLETE);

	// begin our loop
	while (!TestDestroy()) {

		if (m_nStatus == wxWUITS_WAITING) {
			//wxLogDebug(wxT("wxWebUpdateInstallThread::Entry - sleeping 1sec"));
			wxThread::Sleep(100);
			continue;
		}

		wxLogDebug(wxT("wxWebUpdateInstallThread::Entry - installing ") + m_strUpdateFile);
		m_bSuccess = m_pDownload->Install();
		wxLogDebug(wxT("wxWebUpdateInstallThread::Entry - completed installation"));

		// we have successfully download the file
		m_mStatus.Lock();
		m_nStatus = wxWUITS_WAITING;
		m_mStatus.Unlock();
		wxPostEvent(m_pHandler, updatevent);
		m_nInstallationCount++;
	}

	return (void*)FALSE;
}



// -------------------------
// wxWEBUPDATER
// -------------------------

void wxWebUpdater::OnUpdateExit(wxCommandEvent &)
{
 /*   wxWindowList::compatibility_iterator current = GetTopWindow()->GetChildren().GetFirst();
    while (current)
    {
        wxWindow *childWin = current->GetData();
		childWin->Close(true);
        current = current->GetNext();
    }*/
	if (m_pWebUpdateDlg) {
		m_pWebUpdateDlg->AbortDialog();
		//wxSleep(300);
		m_pWebUpdateDlg->Destroy();
	}

	wxTheApp->GetTopWindow()->Close(true);
	//wxExit();
	//OnFatalException();
}

void wxWebUpdater::OnUpdateExec(wxCommandEvent &ce)
{
	wxString cmd = ce.GetString();
	int flags = ce.GetInt();

	long res = ::wxExecute(cmd, flags);
/*	if ((m_nExecFlag & wxEXEC_SYNC) && res != -1)
		return TRUE;
	if ((m_nExecFlag & wxEXEC_ASYNC) && res != 0)
		return TRUE;
	return FALSE;*/
}

void wxWebUpdater::OnWebUpdateDlgShow(wxCommandEvent &ce)
{
	m_pWebUpdateDlg = (wxWebUpdateDlg *)ce.GetClientData();
}

void wxWebUpdater::OnWebUpdateDlgDestroy(wxCommandEvent &ce)
{
	m_pWebUpdateDlg = NULL;
}

