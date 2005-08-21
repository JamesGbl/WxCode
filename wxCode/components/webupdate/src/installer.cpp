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
#include <wx/tokenzr.h>


// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateInstaller, wxObject)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_INSTALLATION_COMPLETE);

// global objects
wxWebUpdateInstaller *wxWebUpdateInstaller::m_pTheInstaller = NULL;




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
		wxFileName::CreateTempFileName(wxT("webupdate")).BeforeLast(sep)
  		+ sep;

	// the folder where we put the downloaded files
	m_hashKeywords[wxT("downloaddir")] = m_hashKeywords[wxT("tempdir")];		// by default it's the temp folder

	// a new temporary folder
	wxString newtempdir(m_hashKeywords[wxT("tempdir")] + sep + 
			wxT("webupdate") + wxDateTime::Now().Format(wxT("%d%H%M%S")) + sep);
	if (wxFileName::Mkdir(newtempdir))
		m_hashKeywords[wxT("newtempdir")] = newtempdir;

	// the updater root folder
	m_hashKeywords[wxT("programdir")] = wxGetCwd();

	// the updater path & filename
	m_hashKeywords[wxT("updater")] = wxFileName(wxTheApp->argv[0]).
				GetPath(wxPATH_GET_SEPARATOR | wxPATH_GET_VOLUME);
	m_hashKeywords[wxT("updatername")] = wxFileName(wxTheApp->argv[0]).GetName();

	// the webupdater process ID
	m_hashKeywords[wxT("pid")] = wxString::Format(wxT("%d"), wxGetProcessId());

	// some command names
#ifdef __WXMSW__
	m_hashKeywords[wxT("cp")] = wxT("cmd.exe /c copy /y");
	m_hashKeywords[wxT("mv")] = wxT("cmd.exe /c move /y");
	m_hashKeywords[wxT("cd")] = wxT("cmd.exe /c cd");
	m_hashKeywords[wxT("mkdir")] = wxT("cmd.exe /c mkdir");
	m_hashKeywords[wxT("exe")] = wxT(".exe");
#else
	m_hashKeywords[wxT("cp")] = wxT("cp -f");
	m_hashKeywords[wxT("mv")] = wxT("mv -f");
	m_hashKeywords[wxT("cd")] = wxT("cd");
	m_hashKeywords[wxT("mkdir")] = wxT("mkdir");
	m_hashKeywords[wxT("exe")] = wxEmptyString;
#endif

	// some other keywords will be added later by other
	// wxWebUpdate* classes
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
	m_hashActions[wxT("copy")] = new wxWebUpdateActionCopy();
	m_hashActions[wxT("make")] = new wxWebUpdateActionMake();
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

wxString wxWebUpdateInstaller::DoPathSubstitution(const wxString &str)
{
	wxString copy(str), sep = wxFileName::GetPathSeparator();
	copy.Replace(wxT("//"), sep);
	return copy;
}

int wxWebUpdateInstaller::ParsePairValueList(const wxString &str, wxArrayString &names, wxArrayString &values)
{		
	// this should be a comma separed list of pairs:  key=value
	wxStringTokenizer tkz(str, wxT(","));
	while ( tkz.HasMoreTokens() ) {
		wxString token = tkz.GetNextToken();
	
		// is this a valid token ?
		if (token.Contains(wxT("="))) { 

			wxString keyname = token.BeforeFirst(wxT('='));
			if (keyname.IsEmpty() || keyname.Contains(wxT(')'))) {

				wxLogDebug(wxT("wxWebUpdaterInstaller::ParsePairValueList - found an invalid keyword name: ")
							+ keyname);
				continue;
			}

			wxString value = token.AfterFirst(wxT('='));
			if (value.IsEmpty()) {

				wxLogDebug(wxT("wxWebUpdaterInstaller::ParsePairValueList - found an invalid keyword value: ")
							+ value);
				continue;
			}

			// keyword values can contain other keywords...
			value = DoSubstitution(value);

			// save this key & value
			names.Add(keyname);
			values.Add(value);

		} else {

			wxLogDebug(wxT("wxWebUpdateInstaller::ParsePairValueList - found an invalid keyword token: ")
							+ token);
		}
	}
	
	wxASSERT(names.GetCount() == values.GetCount());
	return names.GetCount();
}

wxWebUpdateCheckFlag wxWebUpdateInstaller::VersionCheck(const wxVersion &ver) const
{
	int maj, min, rel;
	if (!wxWebUpdatePackage::ExtractVersionNumbers(ver, &maj, &min, &rel)) {
	
		wxLogDebug(wxT("wxWebUpdateInstaller::VersionCheck - invalid version format (")
  					+ ver + wxT(") ?"));
		return wxWUCF_FAILED;
	}
	
	int r = wxWebUpdatePackage::StdVersionCheck(maj, min, rel,
			wxWUI_VERSION_MAJOR, wxWUI_VERSION_MINOR, wxWUI_VERSION_RELEASE);
	if (r == 1) return wxWUCF_FAILED;
	if (r == 0) return wxWUCF_UPDATED;
	return wxWUCF_OUTOFDATE;
}

void wxWebUpdateInstaller::ShowErrorMsg(const wxString &str)
{
	// both log it
	wxLogDebug(str);
	
	// and notify the user
	wxMessageBox(str + wxT("\nContact the support team of ") + 
					GetKeywordValue(wxT("appname")) +
					wxT(" for help."), wxT("Error"), wxOK | wxICON_ERROR);
}

void wxWebUpdateInstaller::ShowNotificationMsg(const wxString &str, const wxString &title)
{
	// both log it
	wxLogDebug(str);

	// and notify the user
	wxMessageBox(str, title.IsEmpty() ? GetKeywordValue(wxT("appname")) : title, 
 						wxOK | wxICON_INFORMATION);
}





// -------------------------
// wxWEBUPDATEINSTALLTHREAD
// -------------------------

// this macro avoid the repetion of a lot of code;
#define wxWUIT_ABORT_INSTALL() {								\
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


