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
#include "wx/stdactions.h"
#include <wx/wfstream.h>
#include <wx/filesys.h>
#include <wx/utils.h>
#include <wx/archive.h>
#include <wx/zipstrm.h>
#include <wx/msgdlg.h>
#include <wx/app.h>


// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateInstaller, wxObject)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_INSTALLATION_COMPLETE);

// global objects
wxWebUpdateInstaller *wxWebUpdateInstaller::m_pTheInstaller = NULL;
//wxWebUpdater *wxWebUpdater::m_pTheUpdater = NULL;




// ---------------------
// wxWEBUPDATEINSTALLER
// ---------------------

#include "wx/filename.h"

wxString GetExecutablePath()
{
    static bool found = false;
    static wxString path;

    if (found)
        return path;
    else
    {
#ifdef __WXMSW__

        wxChar buf[512];
        *buf = '\0';
#if wxUSE_UNICODE
		GetModuleFileNameW
#else
		GetModuleFileNameA
#endif
			(NULL, buf, 511);
        path = buf;

#elif defined(__WXMAC__)

        ProcessInfoRec processinfo;
        ProcessSerialNumber procno ;
        FSSpec fsSpec;

        procno.highLongOfPSN = NULL ;
        procno.lowLongOfPSN = kCurrentProcess ;
        processinfo.processInfoLength = sizeof(ProcessInfoRec);
        processinfo.processName = NULL;
        processinfo.processAppSpec = &fsSpec;

        GetProcessInformation( &procno , &processinfo ) ;
        path = wxMacFSSpec2MacFilename(&fsSpec);
#else
        wxString argv0 = wxTheApp->argv[0];

        if (wxIsAbsolutePath(argv0))
            path = argv0;
        else
        {
            wxPathList pathlist;
            pathlist.AddEnvList(wxT("PATH"));
            path = pathlist.FindAbsoluteValidPath(argv0);
        }

        wxFileName filename(path);
        filename.Normalize();
        path = filename.GetFullPath();
#endif
        found = true;
        return path;
    }
}

wxString wxFindAppPath(const wxString& argv0, const wxString& cwd, const wxString& appVariableName)
{
    wxString str;

    // Try appVariableName
    if (!appVariableName.IsEmpty())
    {
        str = wxGetenv(appVariableName);
        if (!str.IsEmpty())
            return str;
    }

#if defined(__WXMAC__) && !defined(__DARWIN__)
    // On Mac, the current directory is the relevant one when
    // the application starts.
    return cwd;
#endif

    if (wxIsAbsolutePath(argv0))
        return wxPathOnly(argv0);
    else
    {
        // Is it a relative path?
        wxString currentDir(cwd);
        if (currentDir.Last() != wxFILE_SEP_PATH)
            currentDir += wxFILE_SEP_PATH;

        str = currentDir + argv0;
        if (wxFileExists(str))
            return wxPathOnly(str);
    }

    // OK, it's neither an absolute path nor a relative path.
    // Search PATH.

    wxPathList pathList;
    pathList.AddEnvList(wxT("PATH"));
    str = pathList.FindAbsoluteValidPath(argv0);
    if (!str.IsEmpty())
        return wxPathOnly(str);

    // Failed
    return wxEmptyString;
}

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

	// the updater path & filename
	m_hashKeywords[wxT("updater")] = wxTheApp->argv[0];

	// the program-to-update path & filename
	m_hashKeywords[wxT("program")] = 
		wxFileName(m_hashKeywords[wxT("updater")]).
			GetPath(wxPATH_GET_SEPARATOR | wxPATH_GET_VOLUME)
			;
		//wxFindAppPath(wxTheApp->argv[0], wxGetCwd(), wxTheApp->GetAppName());

	// the webupdater process ID
	m_hashKeywords[wxT("pid")] = wxString::Format(wxT("%d"), wxGetProcessId());

	// the program name
	if (wxTheApp) 
		m_hashKeywords[wxT("appname")] = wxEmptyString;//wxTheApp->GetAppName();	
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

	// some command names
	m_hashKeywords[wxT("cp")] = 
#ifdef __WXMSW__
		wxT("copy /y");
#else
		wxT("cp -f");
#endif
	m_hashKeywords[wxT("mv")] = 
#ifdef __WXMSW__
		wxT("move /y");
#else
		wxT("mv -f");
#endif
	m_hashKeywords[wxT("cd")] = wxT("cd");

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

wxString wxWebUpdateInstaller::DoKeywordSubstitution(const wxString &str)
{
	wxStringStringHashMap &list = GetKeywords();
	wxString text(str);

	// iterate over all the elements in the class
    wxStringStringHashMap::iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        wxString key = it->first, value = it->second;
		if (value.IsEmpty()) continue;		// skip empty values

		text.Replace(wxT("$(") + key + wxT(")"), value);
    }

	if (text.Contains(wxT("$(")))
		wxLogDebug(wxT("wxWebUpdateInstaller::DoKeywordSubstitution - ")
				wxT("found unknown keywords in the string:\n") + text);

	return text;
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
	wxCommandEvent updatevent(wxEVT_COMMAND_INSTALLATION_COMPLETE);

	// begin our loop
	while (!TestDestroy()) {

		if (m_nStatus == wxWUITS_WAITING) {
			//wxLogDebug(wxT("wxWebUpdateInstallThread::Entry - sleeping 1sec"));
			wxThread::Sleep(100);
			continue;
		}

		wxLogDebug(wxT("wxWebUpdateInstallThread::Entry - installing ") + 
					m_strUpdateFile);
		m_bSuccess = m_pDownload.Install();
		wxLogDebug(wxT("wxWebUpdateInstallThread::Entry - completed installation"));

		// go in wait mode
		{
  			wxMutexLocker locker(m_mStatus);
			m_nStatus = wxWUITS_WAITING;
		}

		wxPostEvent(m_pHandler, updatevent);
		m_nInstallationCount++;
	}

	return (void*)FALSE;
}



// -------------------------
// wxWEBUPDATER
// -------------------------
/*
void wxWebUpdater::OnUpdateExit(wxCommandEvent &)
{
	if (m_pWebUpdateDlg) {

		// close any wxWebUpdateDlg(-derived) window which is open
		wxLogDebug(wxT("wxWebUpdater::OnUpdateExit - closing the WebUpdate dialog"));
		m_pWebUpdateDlg->AbortDialog();
		m_pWebUpdateDlg->Destroy();
	}

	// close the main window => close the app
	wxLogDebug(wxT("wxWebUpdater::OnUpdateExit - exiting the app"));
	wxTheApp->GetTopWindow()->Close(true);
}

void wxWebUpdater::OnUpdateExec(wxCommandEvent &ce)
{
	wxString cmd = ce.GetString();
	int flags = ce.GetInt();

	wxLogDebug(wxT("wxWebUpdater::OnUpdateExec - executing the command:\n\n\t\t") +
				cmd + wxT("\n\n with flags: %d"), flags);
	/*long res =* ::wxExecute(cmd, flags);
/*	if ((m_nExecFlag & wxEXEC_SYNC) && res != -1)
		return TRUE;
	if ((m_nExecFlag & wxEXEC_ASYNC) && res != 0)
		return TRUE;
	return FALSE;*
}

void wxWebUpdater::OnWebUpdateDlgShow(wxCommandEvent &ce)
{
	wxLogDebug(wxT("wxWebUpdater::OnWebUpdateDlgShow - a wxWebUpdate dialog has been shown"));
	m_pWebUpdateDlg = (wxWebUpdateDlg *)ce.GetClientData();
}

void wxWebUpdater::OnWebUpdateDlgDestroy(wxCommandEvent &)
{
	wxLogDebug(wxT("wxWebUpdater::OnWebUpdateDlgDestroy - a wxWebUpdate dialog has been closed"));
	m_pWebUpdateDlg = NULL;
}
*/
