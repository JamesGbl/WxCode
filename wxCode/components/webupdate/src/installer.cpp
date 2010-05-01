/////////////////////////////////////////////////////////////////////////////
// Name:        installer.cpp
// Purpose:     wxBaseInstaller and wxZIPInstaller
// Author:      Francesco Montorsi
// Created:     2005/06/23
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
    #include "wx/log.h"
    #include <wx/intl.h>        // for _() support
#endif

#include "wx/installer.h"
#include "wx/webupdate.h"
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
            pathlist.AddEnvList(wxS("PATH"));
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
    pathList.AddEnvList(wxS("PATH"));
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
    m_hashKeywords[wxS("tempdir")] =
        wxFileName::CreateTempFileName(wxS("webupdate")).BeforeLast(sep)
        + sep;

    // the folder where we put the downloaded files
    m_hashKeywords[wxS("downloaddir")] = m_hashKeywords[wxS("tempdir")];        // by default it's the temp folder

    // a new temporary folder
    wxString newtempdir(m_hashKeywords[wxS("tempdir")] + sep +
            wxS("webupdate") + wxDateTime::Now().Format(wxS("%d%H%M%S")) + sep);
    if (wxFileName::Mkdir(newtempdir))
        m_hashKeywords[wxS("newtempdir")] = newtempdir;

    // the updater root folder
    m_hashKeywords[wxS("programdir")] = wxGetCwd();

    // the updater path & filename
    m_hashKeywords[wxS("updater")] = wxFileName(wxTheApp->argv[0]).
                GetPath(wxPATH_GET_SEPARATOR | wxPATH_GET_VOLUME);
    m_hashKeywords[wxS("updatername")] = wxFileName(wxTheApp->argv[0]).GetName();

    // the webupdater process ID
    m_hashKeywords[wxS("pid")] = wxString::Format(wxS("%lu"), wxGetProcessId());

    // some command names
#ifdef __WXMSW__
    m_hashKeywords[wxS("cp")] = wxS("cmd.exe /c copy /y");
    m_hashKeywords[wxS("mv")] = wxS("cmd.exe /c move /y");
    m_hashKeywords[wxS("cd")] = wxS("cmd.exe /c cd");
    m_hashKeywords[wxS("mkdir")] = wxS("cmd.exe /c mkdir");
    m_hashKeywords[wxS("exe")] = wxS(".exe");
#else
    m_hashKeywords[wxS("cp")] = wxS("cp -f");
    m_hashKeywords[wxS("mv")] = wxS("mv -f");
    m_hashKeywords[wxS("cd")] = wxS("cd");
    m_hashKeywords[wxS("mkdir")] = wxS("mkdir");
    m_hashKeywords[wxS("exe")] = wxS("");
#endif

    // some other keywords will be added later by other
    // wxWebUpdate* classes
}

void wxWebUpdateInstaller::FreeKeywords()
{
    // remove the newtempdir we created in #InitDefaultKeywords
    wxString folder(m_hashKeywords[wxS("newtempdir")]);
    if (!wxFileName::Rmdir(folder))
        wxLogAdvMsg(wxS("wxWebUpdateInstaller::FreeKeywords - could not remove the ")
                    wxS("temporary folder [") + folder + wxS("] created during initialization"));
}

void wxWebUpdateInstaller::InitDefaultActions()
{
    m_hashActions[wxS("run")] = new wxWebUpdateActionRun();
    m_hashActions[wxS("extract")] = new wxWebUpdateActionExtract();
    m_hashActions[wxS("copy")] = new wxWebUpdateActionCopy();
    m_hashActions[wxS("mkdir")] = new wxWebUpdateActionMkdir();
    m_hashActions[wxS("mkfile")] = new wxWebUpdateActionMkfile();
    m_hashActions[wxS("open")] = new wxWebUpdateActionOpen();
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
                wxLogAdvMsg(wxS("wxWebUpdateInstaller::CreateNewAction - couldn't ")
                        wxS("set correctly the properties for the new action [") +
                        name + wxS("] - proceeding anyway..."));

        return handler;
    }

    return NULL;        // unknown action name
}

wxString wxWebUpdateInstaller::DoKeywordSubstitution(const wxString &str)
{
    wxStringStringHashMap &list = GetKeywords();
    wxString text(str);

    // iterate over all the elements in the class
    wxStringStringHashMap::iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        wxString key = it->first, value = it->second;
        if (value.IsEmpty()) continue;      // skip empty values

        text.Replace(wxS("$(") + key + wxS(")"), value);
    }

    if (text.Contains(wxS("$(")))
        wxLogAdvMsg(wxS("wxWebUpdateInstaller::DoKeywordSubstitution - ")
                wxS("found unknown keywords in the string: [") + text + wxS("]"));

    return text;
}

wxString wxWebUpdateInstaller::DoPathSubstitution(const wxString &str)
{
    wxString copy(str), sep = wxFileName::GetPathSeparator();
    copy.Replace(wxS("//"), sep);
    return copy;
}

int wxWebUpdateInstaller::ParsePairValueList(const wxString &str, wxArrayString &names, wxArrayString &values)
{
    // this should be a comma separed list of pairs:  key=value
    wxStringTokenizer tkz(str, wxS(","));
    while ( tkz.HasMoreTokens() ) {
        wxString token = tkz.GetNextToken();

        // is this a valid token ?
        if (token.Contains(wxS("="))) {

            wxString keyname = token.BeforeFirst(wxS('='));
            if (keyname.IsEmpty() || keyname.Contains(wxS(')'))) {

                wxLogAdvMsg(wxS("wxWebUpdaterInstaller::ParsePairValueList - found an invalid keyword name: [")
                            + keyname + wxS("]"));
                continue;
            }

            wxString value = token.AfterFirst(wxS('='));
            if (value.IsEmpty()) {

                wxLogAdvMsg(wxS("wxWebUpdaterInstaller::ParsePairValueList - found an invalid keyword value: [")
                            + value + wxS("]"));
                continue;
            }

            // keyword values can contain other keywords...
            value = DoSubstitution(value);

            // save this key & value
            names.Add(keyname);
            values.Add(value);

        } else {

            wxLogAdvMsg(wxS("wxWebUpdateInstaller::ParsePairValueList - found an invalid keyword token: [")
                            + token + wxS("]"));
        }
    }

    wxASSERT(names.GetCount() == values.GetCount());
    return names.GetCount();
}

void wxWebUpdateInstaller::ShowErrorMsg(const wxString &str)
{
    // both log it
    wxLogUsrMsg(str);

    // and notify the user
    wxMessageBox(str +
            wxString::Format(_("\nContact the support team of %s for help"),
                             GetKeywordValue(wxS("appname")).c_str()),
            _("Error"), wxOK | wxICON_ERROR);
}

void wxWebUpdateInstaller::ShowNotificationMsg(const wxString &str, const wxString &title)
{
    // both log it
    wxLogDebug(str);

    // and notify the user
    wxMessageBox(str, title.IsEmpty() ? GetKeywordValue(wxS("appname")) : title,
                        wxOK | wxICON_INFORMATION);
}





// -------------------------
// wxWEBUPDATEINSTALLTHREAD
// -------------------------

// this macro avoid the repetion of a lot of code;
#define wxWUIT_ABORT_INSTALL() {                                                              \
            wxLogUsrMsg(_("wxWebUpdateInstallThread::Entry - INSTALLATION ABORTED !!!"));     \
            m_bSuccess = FALSE;                         \
            m_mStatus.Lock();                           \
            m_nStatus = wxWUITS_WAITING;                \
            m_mStatus.Unlock();                         \
            wxPostEvent(m_pHandler, updatevent);        \
            continue;                                   \
    }

void *wxWebUpdateInstallThread::Entry()
{
    // we'll use wxPostEvent to post this event since this is the
    // only thread-safe way to post events !
    wxCommandEvent updatevent(wxEVT_COMMAND_INSTALLATION_COMPLETE);

    // begin our loop
    while (!TestDestroy()) {

        if (m_nStatus == wxWUITS_WAITING) {
            //wxLogDebug(wxS("wxWebUpdateInstallThread::Entry - sleeping 1sec"));
            wxThread::Sleep(100);
            continue;
        }

        wxLogUsrMsg(_("wxWebUpdateInstallThread::Entry - installing [%s]"),
                    m_strUpdateFile.c_str());
        m_bSuccess = m_pDownload->Install();
        wxLogUsrMsg(_("wxWebUpdateInstallThread::Entry - completed installation of [%s]"),
                    m_strUpdateFile.c_str());

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


