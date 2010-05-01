/////////////////////////////////////////////////////////////////////////////
// Name:        webupdate.cpp
// Purpose:     wxWebUpdateLog, wxWebUpdateLocalPackage wxWebUpdateLocalXMLScript,
//              wxWebUpdateAction, wxWebUpdateDownload, wxWebUpdatePackage,
//              wxWebUpdateXMLScript
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
    #include <wx/log.h>
    #include <wx/textctrl.h>
    #include <wx/intl.h>        // for _() support
#endif

#include <wx/url.h>
#include <wx/file.h>
#include <wx/app.h>
#include <wx/wfstream.h>
#include <wx/filesys.h>
#include "wx/webupdate.h"
#include "wx/download.h"
#include "wx/installer.h"
#include <wx/tokenzr.h>
#include <wx/regex.h>

#if wxUSE_HTTPENGINE
#include <wx/httpbuilder.h>
#endif

// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateLog, wxObject)
IMPLEMENT_CLASS(wxWebUpdateLocalPackage, wxObject)
IMPLEMENT_CLASS(wxWebUpdateLocalXMLScript, wxXmlDocument)

IMPLEMENT_CLASS(wxWebUpdateAction, wxObject)
IMPLEMENT_CLASS(wxWebUpdateDownload, wxObject)
IMPLEMENT_CLASS(wxWebUpdatePackage, wxObject)
IMPLEMENT_CLASS(wxWebUpdateXMLScript, wxXmlDocument)

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxWebUpdateActionArray);

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxWebUpdateDownloadArray);

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxWebUpdatePackageArray);

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxWebUpdateLocalPackageArray);


// global objects
wxWebUpdateDownload wxEmptyWebUpdateDownload(wxS("invalid"));
wxWebUpdatePackage wxEmptyWebUpdatePackage(wxS("invalid"));
wxWebUpdateLocalPackage wxEmptyWebUpdateLocalPackage(wxS("invalid"));
int wxWebUpdateAction::m_nExecResult = 0;




// --------------------
// GLOBALS
// --------------------

void wxUninitializeWebUpdate()
{
    // remove the singleton objects
    wxLogAdvMsg(wxS("wxUninitializeWebUpdate - deleting the WebUpdate installer"));
    wxWebUpdateInstaller *old = wxWebUpdateInstaller::Set(NULL);
    if (old) delete old;
}

bool wxIsFileProtocol(const wxString &uri)
{
    // file: is the signature of a file URI...
    return uri.StartsWith(wxS("file:"));
}

bool wxIsHTTPProtocol(const wxString &uri)
{
    // http: is the signature of a file URI...
    return uri.StartsWith(wxS("http:"));
}

bool wxIsWebProtocol(const wxString &uri)
{
    // AFAIK the only protocol which does not require an internet connection is file:
    return !wxIsFileProtocol(uri);
}

//! Creates an input stream tied to the URL given in the constructor.
//! This helper class provides a system to keep the wxURL which produces
//! the wxHTTPStream alive until the stream is needed.
//! The following code in fact
//! \code
//!     wxInputStream *in;
//!         {
//!     wxURL url(wxS("http://www.google.com"));
//!     in = url.GetInputStream();
//!     }
//!     in->Read(somewhere, somebytes);
//! \endcode
//! will fail because wxURL (which contains the wxProtocol which estabilishes
//! the socket connection) is gone out of scope when the wxInputStream is used.
//! This class makes such code:
//! \code
//!     wxInputStream *in;
//!         {
//!     in = new wxURLInputStream(wxS("http://www.google.com"));
//!     }
//!     in->Read(somewhere, somebytes);
//! \endcode
//! possible.
//! NOTE: various hacks are required also to get around the fact that the wxStreamBase::IsOk()
//!       function is not virtual and thus we always have to set our error variable accordingly
//!       after each operation.
class wxURLInputStream : public wxInputStream
{
protected:
    wxURL m_url;
    wxInputStream *m_pStream;

public:
    wxURLInputStream(const wxString &url)
        : m_url(url), m_pStream(NULL) { InitStream(); }
    virtual ~wxURLInputStream() { wxDELETE(m_pStream); }

    wxFileOffset SeekI( wxFileOffset pos, wxSeekMode mode )
        { wxASSERT(m_pStream); wxFileOffset fo = m_pStream->SeekI(pos, mode);
            Synch(); return fo; }
    wxFileOffset TellI() const
        { wxASSERT(m_pStream); return m_pStream->TellI(); }

    bool IsOk() const
        { if (m_pStream == NULL) return FALSE; return m_pStream->IsOk(); }
    size_t GetSize() const
        { wxASSERT(m_pStream); return m_pStream->GetSize(); }
    bool Eof() const
        { wxASSERT(m_pStream); return m_pStream->Eof(); }

protected:

    bool InitStream() {
        if (m_url.GetError() != wxURL_NOERR) {
            m_lasterror = wxSTREAM_READ_ERROR;
            wxLogDebug(wxS("wxURLInputStream - error with given URL..."));
            return FALSE;
        }

        wxLogDebug(wxS("wxURLInputStream - no URL parsing errors..."));

        m_url.GetProtocol().SetTimeout(30);         // 30 sec are much better rather than 10 min !!!
        wxLogDebug(wxS("wxURLInputStream - calling wxURL::GetInputStream"));
        m_pStream = m_url.GetInputStream();
        wxLogDebug(wxS("wxURLInputStream - call was successful"));
        Synch();

        return IsOk();
    }

    void Synch() {
        if (m_pStream)
            m_lasterror = m_pStream->GetLastError();
        else
            m_lasterror = wxSTREAM_READ_ERROR;
    }

    size_t OnSysRead(void *buffer, size_t bufsize)
        { wxASSERT(m_pStream); size_t ret = m_pStream->Read(buffer, bufsize).LastRead(); Synch(); return ret; }
};

#if wxUSE_HTTPENGINE

// exactly like wxURLInputStream just for wxHTTPEngine
class wxSafeHTTPEngineInputStream : public wxInputStream
{
protected:
    wxHTTPBuilder m_http;
    wxInputStream *m_pStream;

public:
    wxSafeHTTPEngineInputStream(const wxString &url,
                    const wxProxySettings &proxy,
                    const wxHTTPAuthSettings &auth)
                    : m_pStream(NULL) {
        m_http.SetProxySettings(proxy);
        m_http.SetAuthentication(auth);
        InitStream(url);
    }

    virtual ~wxSafeHTTPEngineInputStream() { wxDELETE(m_pStream); }


    wxFileOffset SeekI( wxFileOffset pos, wxSeekMode mode )
        { wxASSERT(m_pStream); wxFileOffset fo = m_pStream->SeekI(pos, mode);
            Synch(); return fo; }
    wxFileOffset TellI() const
        { wxASSERT(m_pStream); return m_pStream->TellI(); }

    bool IsOk() const
        { if (m_pStream == NULL) return FALSE; return m_pStream->IsOk(); }
    size_t GetSize() const
        { wxASSERT(m_pStream); return m_pStream->GetSize(); }
    bool Eof() const
        { wxASSERT(m_pStream); return m_pStream->Eof(); }

protected:

    bool InitStream(const wxString &url) {
        m_http.SetTimeout(30);      // 30 sec are much better rather than 10 min !!!
        m_pStream = m_http.GetInputStream(url);
        Synch();

        return (m_pStream != NULL);
    }

    void Synch() {
        if (m_pStream)
            m_lasterror = m_pStream->GetLastError();
        else
            m_lasterror = wxSTREAM_READ_ERROR;
    }

    size_t OnSysRead(void *buffer, size_t bufsize)
        { wxASSERT(m_pStream); size_t ret = m_pStream->Read(buffer, bufsize).LastRead(); Synch(); return ret; }
};

#endif

wxInputStream *wxGetInputStreamFromURI(const wxString &uri)
{
    wxInputStream *in;

    if (wxIsFileProtocol(uri)) {

        // we can handle file:// protocols ourselves
        wxLogAdvMsg(wxS("wxGetInputStreamFromURI - using wxFileInputStream"));
        wxURI u(uri);
        in = new wxFileInputStream(u.GetPath());

    } else {

#if wxUSE_HTTPENGINE_FIXME_FIXME
        wxLogAdvMsg(wxS("wxGetInputStreamFromURI - using wxHTTPBuilder"));
        in = new wxSafeHTTPEngineInputStream(uri,
                wxDownloadThread::m_proxy, wxDownloadThread::m_auth);

        // NOTES:
        // 1) we use the static proxy & auth settings of wxDownloadThread
        //    because this function is an helper function of wxDownloadThread
        // 2) the proxy & auth settings should have been initialized by the
        //    user of wxDownloadThread
        // 3) the wx*Settings classes contain a boolean switch which allows
        //    wxHTTPBuilder to understand if they are marked as "used" or not;
        //    thus, setting them with the Set*() functions below does not
        //    necessarily mean that they will be used.

        // just to help debugging....
        if (wxDownloadThread::m_proxy.m_bUseProxy)
            wxLogAdvMsg(wxS("wxGetInputStreamFromURI - using the proxy settings"));
        if (wxDownloadThread::m_auth.m_authType != wxHTTPAuthSettings::wxHTTP_AUTH_NONE)
            wxLogAdvMsg(wxS("wxGetInputStreamFromURI - using the basic authentication settings"));
#else

        // we won't directly use wxURL because it must be alive together with
        // the wxInputStream it generates... wxURLInputStream solves this problem
        wxLogAdvMsg(wxS("wxGetInputStreamFromURI - using wxURL"));
        in = new wxURLInputStream(uri);
#endif
    }

    return in;
}

unsigned long wxGetSizeOfURI(const wxString &uri)
{
    wxLogDebug(wxS("wxGetSizeOfURI - getting size of [") + uri + wxS("]"));
    wxInputStream *is = wxGetInputStreamFromURI(uri);
    if (is == NULL) {
        wxLogDebug(wxS("wxGetSizeOfURI - aborting; invalid URL !"));
        return 0;
    }

    if (!is->IsOk()) {
        wxLogDebug(wxS("wxGetSizeOfURI - aborting; invalid URL !"));
        delete is;          // be sure to avoid leaks
        return 0;
    }

    // intercept the 302 HTTP "return code"
#if 0
    wxProtocol &p = u.GetProtocol();
    wxHTTP *http = wxDynamicCast(&p, wxHTTP);
    if (http != NULL && http->GetResponse() == 302) {
        wxLogUsrMsg(_("wxGetSizeOfURI - can't get the size of the resource located at [%s]" \
                      " because the request has been redirected... update your URL"), uri);
        return 0;
    }
#endif

    size_t sz = is->GetSize();
    delete is;

    // see wxHTTP::GetInputStream docs
    if (sz == (size_t)-1)
        sz = 0;
    return sz;
}





// ---------------------------
// wxWEBUPDATE log functions
// ---------------------------

void wxWebUpdateLog::DoLog(wxLogLevel level, const wxChar *msg, time_t time)
{
    switch (level) {
    case wxLOG_UsrMsg:
    case wxLOG_AdvMsg:
        // log this line as it is
        DoLogDecoratedString(level, msg);
        break;

    case wxLOG_NewSection:
        // log some additional lines before & after;
        // declass this message to User Message Level
        DoLogDecoratedString(wxLOG_UsrMsg, wxEmptyString);
        DoLogDecoratedString(wxLOG_UsrMsg, msg);
        DoLogDecoratedString(wxLOG_UsrMsg, wxString(wxS('-'), wxStrlen(msg)));
        DoLogDecoratedString(wxLOG_UsrMsg, wxEmptyString);
        break;

    default:
        wxLogPassThrough::DoLog(level, msg, time);
    }
}

void wxWebUpdateLog::DoLogDecoratedString(wxLogLevel lev, const wxChar *str)
{
    wxString time;
    TimeStamp(&time);

    switch (lev) {
    case wxLOG_UsrMsg:

        // user messages first go in the text control...
        if (m_pTextCtrl) {

            wxString msg(str);

            // remove the name of the function which generated this message
            msg = msg.AfterFirst(wxS('-'));
            msg.Trim(FALSE);
            msg.Trim(TRUE);

            // make the first letter uppercase...
            wxChar c = msg[0];
            msg = wxString(c, 1).MakeUpper() + msg.Remove(0, 1);

            // don't put the timestamp... user messages must be kept short & easy to read
            m_pTextCtrl->AppendText(wxS("-> ") + msg + wxS("\n"));
        }
        // don't break here

    case wxLOG_AdvMsg:

        // adv messages go in the log file, if present
        if (m_txtFile.IsOpened()) {

            wxString prefix(wxS(" ") + time);
            wxString msg(str);
            msg.Replace(wxS("\n"), wxS("\n") + wxString(wxS(' '), prefix.Len()));
            m_txtFile.AddLine(prefix + msg);
        }

        // finally they also go in the old logger (which does something only in debug builds)
        wxLogPassThrough::DoLog(wxLOG_Debug, str, 0);
        break;

    default:
        // how does this msg get here ??
        break;
    }
}

void wxWebUpdateLog::WriteAllMsgAlsoToFile(const wxString &filename)
{
    wxRemoveFile(filename);
    m_txtFile.Create(filename);
    m_txtFile.Open(filename);
    m_txtFile.AddLine(wxEmptyString);
}

void wxWebUpdateLog::WriteUsrMsgAlsoToTextCtrl(wxTextCtrl *p)
{
    m_pTextCtrl = p;
}

void wxWebUpdateLog::StopFileLog()
{
    if (!m_txtFile.IsOpened())
        return;

    m_txtFile.Write();
    m_txtFile.Close();
}

#if !wxCHECK_VERSION(2,9,0)

// generic log function
extern void wxVLogGeneric(wxLogLevel level, const wxChar *szFormat, va_list argptr);

#define IMPLEMENT_LOG_FUNCTION(level)                     \
void wxLog##level(const wxChar *szFormat, ...)            \
{                                                         \
    va_list argptr;                                       \
    va_start(argptr, szFormat);                           \
    wxVLogGeneric(wxLOG_##level, szFormat, argptr);       \
    va_end(argptr);                                       \
}

IMPLEMENT_LOG_FUNCTION(UsrMsg)
IMPLEMENT_LOG_FUNCTION(AdvMsg)
IMPLEMENT_LOG_FUNCTION(NewSection)
//IMPLEMENT_LOG_FUNCTION(Info)

#endif




// ---------------------------
// wxWEBUPDATELOCALXMLSCRIPT
// ---------------------------

wxWebUpdateLocalXMLScript::wxWebUpdateLocalXMLScript(const wxString &strURI)
{
    // we can load
    if (!strURI.IsEmpty()) Load(strURI);
}

wxWebUpdateLocalPackage *wxWebUpdateLocalXMLScript::GetPackage(const wxXmlNode *package) const
{
    if (!package || package->GetName() != wxS("local-package")) return NULL;
    wxString packagename = wxWebUpdateXMLScript::GetPackageID(package);

    // parse this package
    wxString ver;
    wxXmlNode *child = package->GetChildren();
    while (child) {

        if (child->GetName() == wxS("version")) {

            // get the version string
            ver = wxWebUpdateXMLScript::GetNodeContent(child);
        }

        // proceed
        child = child->GetNext();
    }

    if (ver.IsEmpty())
        return NULL;

    return new wxWebUpdateLocalPackage(packagename, ver);   // the caller must delete it
}

wxWebUpdateLocalPackage *wxWebUpdateLocalXMLScript::GetPackage(const wxString &packagename) const
{
    if (!IsOk())
        return NULL;
    wxXmlNode *package = GetRoot()->GetChildren();

    bool matches = FALSE;
    while (package && !matches) {

        while (package && package->GetName() != wxS("local-package"))
            package = package->GetNext();
        if (!package) return NULL;          // could not found other PACKAGE tags in this script !

        matches = (wxWebUpdateXMLScript::GetPackageID(package) == packagename);
    }

    if (!package)
        return NULL;    // could not found the required package
    return GetPackage(package);
}

wxWebUpdateLocalPackageArray wxWebUpdateLocalXMLScript::GetAllPackages() const
{
    wxWebUpdateLocalPackageArray ret;

    // check root of the local script file
    if (!IsOk())
        return ret;         // empty array = error
    wxXmlNode *package = GetRoot()->GetChildren();

    while (package) {

        if (package->GetName() == wxS("local-package")) {

            // add the package
            wxWebUpdateLocalPackage *toadd = GetPackage(package);
            if (toadd) ret.Add(toadd);
        }

        package = package->GetNext();
    }

    return ret;
}

bool wxWebUpdateLocalXMLScript::Load(const wxString &uri)
{
    wxLogUsrMsg(_("wxWebUpdateXMLScript::Load - loading [%s]"), uri.c_str());

    // refer to "webupdate.dtd" for a definition of the XML webupdate info script
    // first of all, we need to open a connection to the given url
    wxFileSystem fs;
    wxFSFile *xml = fs.OpenFile(uri);
    if (!xml) return FALSE;

    // parse the XML file
    wxXmlDocument::Load(*xml->GetStream());
    delete xml;
    if (!IsOk())
        return FALSE;

    // look at the version
    if (wxWebUpdateXMLScript::GetWebUpdateVersion(GetRoot(), m_verWebUpdate) == wxWUCF_FAILED)
        return FALSE;

    // save the URI of this local XML script.
    // NOTE: this has to be done *before* processing the XML file since this info
    //       is used by ::SetRemoteScriptURI
    m_strLocalURI = uri;
    wxWebUpdateInstaller::Get()->SetKeywordValue(wxS("localxml"), uri);

    // save also the folders
    wxFileName f(wxFileSystem::URLToFileName(m_strLocalURI));
    wxWebUpdateInstaller::Get()->SetKeywordValue(wxS("localdir"),
                        f.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));

    // start processing the XML file
    wxXmlNode *child = GetRoot()->GetChildren();
    while (child) {

        if (child->GetName() == wxS("appname")) {

            // save the name of the app using WebUpdate
            m_strAppName = wxWebUpdateXMLScript::GetNodeContent(child);
            wxWebUpdateInstaller::Get()->SetKeywordValue(wxS("appname"), m_strAppName);

        } else if (child->GetName() == wxS("xrc")) {

            // save the name of the XRC file to use
            m_strXRC = wxWebUpdateInstaller::Get()->DoSubstitution(
                                    wxWebUpdateXMLScript::GetNodeContent(child));

        } else if (child->GetName() == wxS("res")) {

            // save the name of the XRC resource to use
            m_strXRCRes = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(
                                    wxWebUpdateXMLScript::GetNodeContent(child));

        } else if (child->GetName() == wxS("savelog")) {

            // save the log file ?
            m_bSaveLog = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(
                                    wxWebUpdateXMLScript::GetNodeContent(child)) == wxS("1");

        } else if (child->GetName() == wxS("restart")) {

            // restart the updated application ?
            m_bRestart = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(
                                    wxWebUpdateXMLScript::GetNodeContent(child)) == wxS("1");

        } else if (child->GetName() == wxS("appfile")) {

            // save the filename of the program to update
            m_strAppFile = wxWebUpdateInstaller::Get()->DoSubstitution(
                                    wxWebUpdateXMLScript::GetNodeContent(child));

    } else if (child->GetName() == wxS("remoteuri")) {

            SetRemoteScriptURI(wxWebUpdateXMLScript::GetNodeContent(child));

        } else if (child->GetName() == wxS("keywords")) {

            // this should be a comma separed list of pairs:  key=value
            int count = wxWebUpdateInstaller::Get()->ParsePairValueList(
                    wxWebUpdateXMLScript::GetNodeContent(child), m_arrNames, m_arrValues);

            for (int i=0; i < count; i++)
                wxWebUpdateInstaller::Get()->SetKeywordValue(m_arrNames[i], m_arrValues[i]);
        }

        child = child->GetNext();
    }

    // is this local file valid ?
    if (m_strAppName.IsEmpty() || m_strAppFile.IsEmpty())
        return FALSE;

    return TRUE;
}

void wxWebUpdateLocalXMLScript::SetRemoteScriptURI(const wxString &uri)
{
    // we keep the original, un-substituted URI to save it later in ::Save
    m_strRemoteURIOriginal = uri;

    // save the location of the remote script: we avoid to do path substitution
    // (i.e. '//' substitution ) because it's important not to
    // do the '//' substitution in strings which can contain http://
    // strings...
    m_strRemoteURI = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(m_strRemoteURIOriginal);

    // support for "file:" URIs
    if (wxIsFileProtocol(m_strRemoteURI)) {

        // is this a relative path ?
        wxFileName fn = wxFileSystem::FileNameToURL(m_strRemoteURI);
        if (fn.IsRelative()) {

            // <remoteuri> tag should specify relative paths
            // using the folder containing the local XML script
            // as the folder used to solve the relative path.
            wxFileName currdir(m_strLocalURI);
            wxASSERT_MSG(currdir.IsAbsolute(), wxS("Invalid local URI"));
            fn.MakeAbsolute(currdir.GetPath());
        }

        // replace our file URI with the wxFileName-filtered uri.
        m_strRemoteURI = wxFileSystem::FileNameToURL(fn);
    }

    // save the URI of the remote XML script.
    wxWebUpdateInstaller::Get()->SetKeywordValue(wxS("remotexml"), m_strRemoteURI);

    wxFileName f(wxFileSystem::FileNameToURL(m_strRemoteURI));
    wxWebUpdateInstaller::Get()->SetKeywordValue(wxS("remotedir"),
                        f.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));
}

bool wxWebUpdateLocalXMLScript::IsOk() const
{
    if (!GetRoot() || GetRoot()->GetName() != wxS("webupdate"))
        return FALSE;
    return TRUE;
}

bool wxWebUpdateLocalXMLScript::IsComplete() const
{
    // we should have loaded these
    // - by the XML local script
    // - or by the command line options...
    if (m_strXRC.IsEmpty() || m_strXRCRes.IsEmpty() || m_strRemoteURI.IsEmpty())
        return FALSE;
    return TRUE;
}

// some simple helpers for wxWebUpdateLocalXMLScript::RebuildHeader
wxXmlNode *wxCreateElemNode(const wxString &name)
{ return new wxXmlNode(NULL, wxXML_ELEMENT_NODE, name, wxEmptyString, NULL, NULL); }

wxXmlNode *wxCreateElemTextNode(const wxString &name, const wxString &content = wxEmptyString)
{
    wxXmlNode *n = wxCreateElemNode(name);
    n->AddChild(new wxXmlNode(NULL, wxXML_TEXT_NODE, wxEmptyString, content, NULL, NULL));
    return n;
}

wxXmlNode *wxCreateElemTextNode(wxXmlNode *prev, const wxString &name, const wxString &content = wxEmptyString)
{
    wxXmlNode *n = wxCreateElemTextNode(name, content);
    prev->SetNext(n);
    return n;
}

wxXmlNode *wxWebUpdateLocalXMLScript::BuildHeader() const
{
    // the webupdate root
    wxXmlNode *webupdate = wxCreateElemNode(wxS("webupdate"));
    webupdate->AddAttribute(new wxXmlAttribute(wxS("version"),
                                wxWebUpdateInstaller::Get()->GetVersion(), NULL));

    // set the appname
    wxXmlNode *appname = wxCreateElemTextNode(wxS("appname"), m_strAppName);
    webupdate->AddChild(appname);       // link this node as child of webupdate

    // set all other children nodes
    wxXmlNode *appfile = wxCreateElemTextNode(appname, wxS("appfile"), m_strAppFile);
    wxXmlNode *xrc = wxCreateElemTextNode(appfile, wxS("xrc"), m_strXRC);
    wxXmlNode *res = wxCreateElemTextNode(xrc, wxS("res"), m_strXRCRes);
    wxXmlNode *restart = wxCreateElemTextNode(res, wxS("restart"), m_bRestart ? wxS("1") : wxS("0"));
    wxXmlNode *savelog = wxCreateElemTextNode(restart, wxS("savelog"), m_bSaveLog ? wxS("1") : wxS("0"));

    // set the remote URI
    wxXmlNode *uri = wxCreateElemTextNode(savelog, wxS("remoteuri"),
        m_strRemoteURIOriginal.IsEmpty() ? m_strRemoteURI : m_strRemoteURIOriginal);

    // set the keywords
    wxString content;
    for (int i=0; i < (int)m_arrNames.GetCount(); i++)
        content += m_arrNames[i] + wxS("=") + m_arrValues[i] + wxS(",");
    if (!content.IsEmpty())
        content.RemoveLast();       // remove the last comma
    wxXmlNode *keywords = wxCreateElemTextNode(wxS("keywords"), content);
    uri->SetNext(keywords);

    return webupdate;
}

void wxWebUpdateLocalXMLScript::SetPackageArray(const wxWebUpdateLocalPackageArray &arr)
{
    wxXmlNode *webupdate = BuildHeader();

    // get the last child node
    wxXmlNode *last = webupdate->GetChildren();
    while (last)
        if (last->GetNext() == NULL)
            break;
        else
            last = last->GetNext();

    // create the array of local packages
    wxXmlNode *prev = last;
    for (int i=0; i < (int)arr.GetCount(); i++) {

        // create the local-package tag with its ID property
        wxXmlNode *lp = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxS("local-package"),
                                        wxEmptyString, new wxXmlAttribute(wxS("id"),
                                    arr[i].GetName(), NULL), NULL);

        // create its VERSION subtag
        lp->AddChild(wxCreateElemTextNode(wxS("version"), arr[i].GetVersion()));

        // set the parent
        lp->SetParent(webupdate);

        // proceed with next
        prev->SetNext(lp);
        prev = lp;
    }

    // replace the old document
    SetRoot(webupdate);
}

bool wxWebUpdateLocalXMLScript::Save() const
{
    wxASSERT_MSG(!m_strLocalURI.IsEmpty(), wxS("Invalid filename !"));
    return wxXmlDocument::Save(m_strLocalURI);
}




// ---------------------
// wxWEBUPDATEACTION
// ---------------------

long wxWebUpdateAction::wxExecute(const wxString &command, int flags) const
{
    if (wxIsMainThread())
        return wxExecute(command, flags);           // no problems !

    // unfortunately we cannot use ::wxExecute from a secondary thread
    // (and wxWebUpdateActions run from a wxWebUpdateInstallThread) so we
    // are forced to send a message to wxApp which launches the command for us
    wxCommandEvent runev(wxEVT_COMMAND_EXECUTE);
    runev.SetString(command);
    runev.SetInt(flags);

    // our app should process this event...
    wxLogAdvMsg(wxS("wxWebUpdateAction::wxExecute - sending to wxTheApp the command [")
                    + runev.GetString() + wxS("]"));

    // to understand when the wxApp object has executed the command, we need
    // a wxCondition and we also need to pass it to the wxApp using the event
    wxMutex m;
    wxCondition cond(m);
    runev.SetClientData(&cond);
    runev.SetExtraLong((long)&m);

    // start !
    m.Lock();
    wxTheApp->AddPendingEvent(runev);

    // then wait for wxApp to do its job
    cond.Wait();

    // the wxApp should have stored the wxExecute return code in our
    // m_nExecResult variable
    return m_nExecResult;
}




// ---------------------
// wxWEBUPDATEPLATFORM
// ---------------------

bool wxWebUpdatePlatform::Matches(const wxWebUpdatePlatform &plat) const
{
    // check port name
    if (GetPortId() != plat.GetPortId() &&
        (GetPortId() != wxPORT_ANY && plat.GetPortId() != wxPORT_ANY))
        return FALSE;

    // check architecture
    if (GetArchitecture() != plat.GetArchitecture() &&
        (GetArchitecture() != wxARCH_ANY && plat.GetArchitecture() != wxARCH_ANY))
        return FALSE;

    // check platform ID using regexp
    wxRegEx our(m_strID);
    if (!our.IsValid()) {
        wxLogDebug(wxS("wxWebUpdatePlatform::Matches - [") + m_strID +
                    wxS("] is a malformed regular expression !"));
        return FALSE;       // syntax error !
    }

    if (our.Matches(plat.m_strID))
        return TRUE;
    return FALSE;
}

wxString wxWebUpdatePlatform::GetAsString() const
{
    // concatenate all our info
    return GetPortIdName(GetPortId(), false) + wxS(" - ") + 
           GetArchName(GetArchitecture()) +
           wxS(" - ") + m_strID;
}

// static
wxWebUpdatePlatform wxWebUpdatePlatform::GetThisPlatform()
{
    wxWebUpdatePlatform plat;

    ((wxPlatformInfo&)plat) = wxPlatformInfo::Get();

    // get ID
    plat.SetID(wxGetOsDescription());

    return plat;
}





// ---------------------
// wxWEBUPDATEDOWNLOAD
// ---------------------

wxWebUpdateDownload::~wxWebUpdateDownload()
{
    // clean our array of actions (since it holds pointers it won't
    // auto-clean itself in the proper way !)
    for (int i=0; i < (int)m_arrActions.GetCount(); i++)
        if (m_arrActions.Item(i))
            delete m_arrActions.Item(i);
}

void wxWebUpdateDownload::Copy(const wxWebUpdateDownload &tocopy)
{
    m_platform = tocopy.m_platform;
    m_strMD5 = tocopy.m_strMD5;
    m_urlDownload = tocopy.m_urlDownload;
    m_size = tocopy.m_size;

    m_arrActions.Clear();
    for (int i=0; i < (int)tocopy.m_arrActions.GetCount(); i++) {
        wxWebUpdateAction *newact = tocopy.m_arrActions.Item(i)->Clone();
        m_arrActions.Add(newact);
    }
}

unsigned long wxWebUpdateDownload::GetDownloadSize(bool forceRecalc)
{
    if (!forceRecalc && m_size != 1)
        return m_size;      // we have already calculated it...

    // we need to calculate it...
    m_size = wxGetSizeOfURI(m_urlDownload);
    return m_size;
}

wxString wxWebUpdateDownload::GetFileName() const
{
    return wxFileSystem::URLToFileName(m_urlDownload).GetFullName();
}
/*
bool wxWebUpdateDownload::DownloadSynch(const wxString &path
#if wxUSE_HTTPENGINE
                                        , const wxProxySettings &
#endif
                                        )
{
    if (path.IsEmpty()) return FALSE;

#if wxUSE_HTTPENGINE
    // set advanced URL options
    wxHTTPBuilder u;
#else
    wxURL u(m_urlDownload);
    if (u.GetError() != wxURL_NOERR)
        return FALSE;
#endif

    // now work on streams; wx docs says that using wxURL::GetInputStream
    // is deprecated but this is the only way to set advanced info like
    // proxy, user & password...
    wxFileOutputStream out(path);
#if wxUSE_HTTPENGINE
    wxInputStream *in = u.GetInputStream(m_urlDownload);
#else
    wxInputStream *in = u.GetInputStream();
#endif
    if (in == NULL)
        return FALSE;
    if (!out.IsOk()) {
        delete in;
        return FALSE;
    }

    out.Write(*in);
    delete in;
    if (!out.IsOk() || out.GetSize() == 0 ||
        (GetDownloadSize() > 0 && out.GetSize() != GetDownloadSize()))
        return FALSE;

    wxLogUsrMsg(_("wxWebUpdateDownload::DownloadSynch - completed download of %lu bytes"),
                out.GetSize());

    // we have successfully download the file
    return TRUE;
}

wxDownloadThread *wxWebUpdateDownload::DownloadAsynch(const wxString &path,
                                                        wxEvtHandler *phandler
#if wxUSE_HTTPENGINE
                                                        , const wxProxySettings &proxy
#endif
                                    )
{
    if (path.IsEmpty()) return NULL;
    wxDownloadThread *thread = new wxDownloadThread(phandler);

    // just set the download options
#if wxUSE_HTTPENGINE
    thread->m_proxy = proxy;
#endif

    // launch the download
    if (thread->Create() != wxTHREAD_NO_ERROR ||
        thread->Run() != wxTHREAD_NO_ERROR) {
        delete thread;
        return NULL;
    }

    thread->m_strOutput = path;
    thread->BeginNewDownload();

    return thread;
}*/

bool wxWebUpdateDownload::Install() const
{
    wxWebUpdateInstaller *touse = wxWebUpdateInstaller::Get();
    wxWebUpdateActionHashMap hashmap = touse->GetActionHashMap();

    // installation means: execute all <action> tags for this download...
    for (int i=0; i<(int)m_arrActions.GetCount(); i++)
        if (!m_arrActions[i]->Run())
            return FALSE;       // stop here

    return TRUE;
}




// ---------------------
// wxWEBUPDATEPACKAGE
// ---------------------

wxArrayString wxWebUpdatePackage::GetParsedPrerequisites() const
{
    wxArrayString ret;
    wxStringTokenizer tkz(m_strPrerequisites, wxS(","));
    while (tkz.HasMoreTokens())
        ret.Add(tkz.GetNextToken());
    return ret;
}

wxWebUpdateDownload &wxWebUpdatePackage::GetDownload(wxWebUpdatePlatform plat) const
{
    // get current platform if given one is not valid
    if (!plat.IsOk())
        plat = wxWebUpdatePlatform::GetThisPlatform();

    for (int i=0; i<(int)m_arrWebUpdates.GetCount(); i++)
        if (m_arrWebUpdates.Item(i).GetPlatform().Matches(plat))
            return m_arrWebUpdates.Item(i);

    // could not find a download for the given platform....
    return wxEmptyWebUpdateDownload;
}

void wxWebUpdatePackage::CacheDownloadSizes()
{
    for (int i=0; i<(int)m_arrWebUpdates.GetCount(); i++)
        m_arrWebUpdates.Item(i).GetDownloadSize();
}



// ----------------------
// wxWEBUPDATEXMLSCRIPT
// ----------------------

wxWebUpdateXMLScript::wxWebUpdateXMLScript(const wxString &strURI)
{
    // we can load
    if (!strURI.IsEmpty()) Load(strURI);
}

// static; taken from wx/src/xrc/xmlres.cpp, wxXmlResourceHandler::GetNodeContent
wxString wxWebUpdateXMLScript::GetNodeContent(const wxXmlNode *node)
{
    const wxXmlNode *n = node;
    if (n == NULL) return wxEmptyString;
    n = n->GetChildren();

    while (n)
    {
        if (n->GetType() == wxXML_TEXT_NODE ||
            n->GetType() == wxXML_CDATA_SECTION_NODE)
            return n->GetContent();
        n = n->GetNext();
    }

    return wxEmptyString;
}

// static
bool wxWebUpdateXMLScript::IsLangPropertyMatching(const wxXmlNode *n, const wxString &lang)
{
    // look for the LANG property of this tag...
    wxString l = n->GetAttribute(wxS("lang"), wxS("en"));    // english is the default

    // does the LANG property match the given language name ?
    if (l.IsSameAs(lang, FALSE) ||
        l.BeforeFirst(wxS('-')).IsSameAs(lang.BeforeFirst(wxS('_')), FALSE)) {
        wxLogDebug(wxS("wxWebUpdateXMLScript::IsLangPropertyMatching - ")
                    wxS("found a [%s] LANG property matching the given [%s] language name"),
                    l.c_str(), lang.c_str());
        return TRUE;
    } else
        wxLogDebug(wxS("wxWebUpdateXMLScript::IsLangPropertyMatching - ")
                    wxS("found a [%s] LANG property not matching the given [%s] language name"),
                    l.c_str(), lang.c_str());

    return FALSE;
}

// static
wxString wxWebUpdateXMLScript::ChooseLanguage(const wxXmlNode *n, const wxString &lang,
                                              bool *bDescriptionLangFound, bool *bEnglishLangFound,
                                              const wxString &currentmsg)
{
    wxASSERT(bDescriptionLangFound && bEnglishLangFound);   // cannot be NULL!
    wxString out = currentmsg;
    if (*bDescriptionLangFound == FALSE) {

        if (IsLangPropertyMatching(n, lang)) {

            out = GetNodeContent(n);
            *bDescriptionLangFound = TRUE;       // found the language we were searching for!

        } else if (*bEnglishLangFound == FALSE) {

            // if we did not find the description msg in the right language
            // (i.e. bDescriptionLangFound==FALSE) and we haven't found the
            // english version of the message yet (i.e. bEnglishLangFound==FALSE),
            // then save this description message as the current default
            out = GetNodeContent(n);

            // is this the english message ?
            if (IsLangPropertyMatching(n, wxS("en")))
                *bEnglishLangFound = TRUE;
        }

    }

    return out;
}

wxWebUpdateActionArray wxWebUpdateXMLScript::GetActionArray(const wxXmlNode *actions) const
{
    wxWebUpdateActionArray ret;

    if (actions->GetName() != wxS("actions"))
        return ret;

    // parse all action tags
    wxXmlNode *child = actions->GetChildren();
    while (child) {

        wxString actname(child->GetName());

        // convert to a wxArrayString the properties of this node
        wxArrayString names, values;
        wxXmlAttribute *prop = child->GetAttributes();
        while (prop) {

            names.Add(prop->GetName());

            // the values can contain keywords to substitute
            wxString v = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(prop->GetValue());

            // and also path separator characters
            v = wxWebUpdateInstaller::Get()->DoPathSubstitution(v);

            // add this new value
            values.Add(v);
            prop = prop->GetNext();
        }

        // add a new action
        wxWebUpdateAction *a = wxWebUpdateInstaller::Get()->CreateNewAction(actname, &names, &values);
        if (a)
            ret.Add(a);
        else
            wxLogAdvMsg(wxS("wxWebUpdateXMLScript::GetActionArray - unknown action: ") + actname);

        child = child->GetNext();
    }

    return ret;
}

wxWebUpdateDownload wxWebUpdateXMLScript::GetDownload(const wxXmlNode *latestdownload) const
{
    if (latestdownload->GetName() != wxS("latest-download"))
        return wxEmptyWebUpdateDownload;

    // the info we need to build a wxWebUpdateDownload...
    wxString platform, md5, uri, arch, id;
    wxWebUpdateActionArray actions;
    wxStringStringHashMap &list = wxWebUpdateInstaller::Get()->GetKeywords();

    // find the platform for which this download is
    wxXmlNode *child = latestdownload->GetChildren();
    while (child) {

        // is this a well-formed tag ?
        if (child->GetName() == wxS("uri")) {

            // extract filename for this download
            wxLogDebug(wxS("wxWebUpdateXMLScript::GetDownload - the URI for this download is [") + GetNodeContent(child) + wxS("]"));
            uri = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(GetNodeContent(child));

            // the substitution process could have put some '\' (on win32) in the URI var... fix them
            uri.Replace(wxS("\\"), wxS("/"));

            // be sure that our "uri" var contains a valid URI...
            uri = wxURI(uri).BuildURI();
            wxLogDebug(wxS("wxWebUpdateXMLScript::GetDownload - the expanded URI for this download is [") + uri + wxS("]"));

            // this keyword will be removed when exiting from this XML node
            list[wxS("thisfile")] = list[wxS("downloaddir")] +
                wxFileName::GetPathSeparator() +
                wxFileSystem::URLToFileName(uri).GetFullName();

        } else if (child->GetName() == wxS("md5")) {

            // this is the precomputed MD5 for this file
            md5 = GetNodeContent(child);

        } else if (child->GetName() == wxS("platform")) {

            // search the known properties
            platform = child->GetAttribute(wxS("name"), wxS("invalid"));
            arch = child->GetAttribute(wxS("arch"), wxS("any"));
            id = child->GetAttribute(wxS("id"), wxS(""));

        } else if (child->GetName() == wxS("actions")) {

            // get the actions for this download
            actions = GetActionArray(child);
        }


        // parse next child...
        child = child->GetNext();
    }

    // remove the keywords we added while parsing
    list[wxS("thisfile")] = wxEmptyString;

    // is this a valid download ?
    if (platform.IsEmpty() || actions.GetCount() <= 0)
        return wxEmptyWebUpdateDownload;

    return wxWebUpdateDownload(uri,
                                wxWebUpdatePlatform(platform, arch, id),
                                md5, &actions);
}

wxWebUpdatePackage *wxWebUpdateXMLScript::GetPackage(const wxXmlNode *package) const
{
    if (!package || package->GetName() != wxS("package")) return NULL;
    wxString packagename = GetPackageID(package);

    // init the return value
    wxLogAdvMsg(wxS("wxWebUpdateXMLScript::GetPackage - parsing package [") + packagename + wxS("]"));
    wxWebUpdatePackage *ret = new wxWebUpdatePackage(packagename);

    // add to the current keyword list the current ID
    wxStringStringHashMap &list = wxWebUpdateInstaller::Get()->GetKeywords();
    list[wxS("id")] = packagename;      // will be removed when exiting

    // parse this package
    wxXmlNode *child = package->GetChildren();
    bool bDescriptionLangFound = FALSE, bEnglishLangFound = FALSE;
    while (child) {

        if (child->GetName() == wxS("requires")) {

            // read the list of required packages
            ret->m_strPrerequisites = GetNodeContent(child);

        } else if (child->GetName() == wxS("latest-version")) {

            // get the version string
            ret->m_verLatest = GetNodeContent(child);
            ret->m_importance = wxWUPI_NORMAL;          // by default

            // add the "latest-version" keyword
            list[wxS("latest-version")] = ret->m_verLatest;  // will be removed when exiting

            // and this version's importance (if available)
            wxString imp = child->GetAttribute(wxS("importance"), wxS(""));
            if (imp == wxS("high")) ret->m_importance = wxWUPI_HIGH;
            if (imp == wxS("normal")) ret->m_importance = wxWUPI_NORMAL;
            if (imp == wxS("low")) ret->m_importance = wxWUPI_LOW;

        } else if (child->GetName() == wxS("latest-download")) {

            // parse this download
            wxWebUpdateDownload update = GetDownload(child);
            if (update.IsOk())
                ret->AddDownload(update);
            else
                wxLogAdvMsg(wxS("wxWebUpdateXMLScript::GetPackage - skipping an invalid <latest-download> tag"));

        } else if (child->GetName() == wxS("description")) {

            // all language-choose logic is in ChooseLanguage (which is called also in other places)
            ret->m_strDescription = ChooseLanguage(child, m_strLanguageID, &bDescriptionLangFound,
                                                   &bEnglishLangFound, ret->m_strDescription);
        }

        // proceed
        child = child->GetNext();
    }

    // do keywords substitution in the package description
    ret->m_strDescription = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(
                                                         ret->m_strDescription);

    // remove the keywords we added while parsing
    list[wxS("id")] = wxEmptyString;
    list[wxS("latest-version")] = wxEmptyString;

    // be sure this package is valid
#ifdef __WXDEBUG__
    wxASSERT_MSG(ret->m_arrWebUpdates.GetCount() > 0,
            wxS("No downloads defined for this package in the webupdate script ?"));
#else
    if (ret->m_arrWebUpdates.GetCount() <= 0) {
        delete ret;
        return NULL;
    }
#endif

    return ret;         // the caller must delete it
}

// static
wxString wxWebUpdateXMLScript::GetPackageID(const wxXmlNode *package)
{
    wxXmlAttribute *prop = package->GetAttributes();
    while (prop && prop->GetName() != wxS("id")) {
        wxLogAdvMsg(wxS("wxWebUpdateXMLScript::GetPackageID - found unsupported ")
                wxS("package property: ") + prop->GetName() + wxS("=") + prop->GetValue());
        prop = prop->GetNext();
    }

    if (prop)
        return prop->GetValue();        // found !
    return wxEmptyString;
}

wxWebUpdatePackage *wxWebUpdateXMLScript::GetPackage(const wxString &packagename) const
{
    // check root
    if (!IsOk())
        return NULL;
    wxXmlNode *package = GetRoot()->GetChildren();

    bool matches = FALSE;
    while (package && !matches) {

        while (package && package->GetName() != wxS("package"))
            package = package->GetNext();
        if (!package) return NULL;          // could not found other PACKAGE tags in this script !

        matches = (GetPackageID(package) == packagename);
    }

    if (!package)
        return NULL;    // could not found the required package
    return GetPackage(package);
}

wxWebUpdatePackageArray wxWebUpdateXMLScript::GetAllPackages() const
{
    wxWebUpdatePackageArray ret;

    // now it's time to parse the XML file we expect to be in 'xml' input stream
    if (!IsOk())
        return ret;         // empty array = error
    wxXmlNode *package = GetRoot()->GetChildren();

    while (package) {
        wxWebUpdatePackage *toadd = GetPackage(package);
        package = package->GetNext();
        if (toadd == NULL) continue;        // skip this

        ret.Add(toadd);
    }

    return ret;
}

// static
wxWebUpdateCheckFlag wxWebUpdateXMLScript::GetWebUpdateVersion(const wxXmlNode *package, wxVersion &ver)
{
    wxXmlAttribute *prop = package->GetAttributes();
    while (prop && prop->GetName() != wxS("version")) {
        wxLogAdvMsg(wxS("wxWebUpdateXMLScript::GetWebUpdateVersion - found unsupported ")
                wxS("webupdate property: ") + prop->GetName() + wxS("=") + prop->GetValue());
        prop = prop->GetNext();
    }

    if (prop == NULL) {
        wxLogAdvMsg(wxS("wxWebUpdateXMLScript::GetWebUpdateVersion - could not find the ")
                wxS("version property in the root tag. Defaulting to version ")
            + wxWebUpdateInstaller::Get()->GetVersion());
        ver = wxWebUpdateInstaller::Get()->GetVersion();

    } else {

        ver = prop->GetValue();
        wxLogAdvMsg(wxS("wxWebUpdateXMLScript::GetWebUpdateVersion - the version ")
                wxS("of this XML WebUpdate document is ") + ver);
    }

    // do the version check
    wxWebUpdateCheckFlag f = wxWebUpdateInstaller::Get()->VersionCheck(ver);
    if (f == wxWUCF_UPDATED)
        wxLogAdvMsg(wxS("wxWebUpdateXMLScript::GetWebUpdateVersion - the version ")
                wxS("of this XML WebUpdate document matches the installer engine version"));
    else if (f == wxWUCF_OUTOFDATE)
        wxLogAdvMsg(wxS("wxWebUpdateXMLScript::GetWebUpdateVersion - the version ")
                wxS("of this XML WebUpdate document is older than the installer engine... ")
            wxS("continuing anyway"));
    else
        wxLogAdvMsg(wxS("wxWebUpdateXMLScript::GetWebUpdateVersion - the version ")
                wxS("of this XML WebUpdate document is newer than the installer engine... ")
            wxS("aborting (cannot handle the new features!)"));
    return f;
}

bool wxWebUpdateXMLScript::Load(const wxString &uri, const wxString &langID)
{
    wxLogUsrMsg(_("wxWebUpdateXMLScript::Load - loading [%s]"), uri.c_str());
    m_strLanguageID = langID;
    wxASSERT(!m_strLanguageID.IsEmpty());

    // refer to "webupdate.dtd" for a definition of the XML webupdate info script
    // first of all, we need to open a connection to the given url
    wxFileSystem fs;
    wxFSFile *xml = fs.OpenFile(uri);
    if (!xml) return FALSE;

    // parse the XML file
    wxXmlDocument::Load(*xml->GetStream());
    delete xml;
    if (!IsOk())
        return FALSE;

    // look at the version
    if (GetWebUpdateVersion(GetRoot(), m_strWebUpdateVersion) == wxWUCF_FAILED)
        return FALSE;

    wxXmlNode *child = GetRoot()->GetChildren();
    bool bMsgUpdateAvailLangFound = FALSE, bMsgUpdateAvailEnglishFound = FALSE,
         bMsgUpdateNotAvailLangFound = FALSE, bMsgUpdateNotAvailEnglishFound = FALSE;
    while (child) {

        // parse the children of <webupdate> which are not packages
        if (child->GetName() == wxS("msg-update-available")) {
            m_strUpdateAvailableMsg = ChooseLanguage(child, m_strLanguageID, &bMsgUpdateAvailLangFound,
                                                   &bMsgUpdateAvailEnglishFound, m_strUpdateAvailableMsg);
            m_strUpdateAvailableMsg = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(m_strUpdateAvailableMsg);

        } else if (child->GetName() == wxS("msg-update-notavailable")) {
            m_strUpdateNotAvailableMsg = ChooseLanguage(child, m_strLanguageID, &bMsgUpdateNotAvailLangFound,
                                                   &bMsgUpdateNotAvailEnglishFound, m_strUpdateNotAvailableMsg);
            m_strUpdateNotAvailableMsg = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(m_strUpdateNotAvailableMsg);
        }

        child = child->GetNext();
    }

    // don't save here the URI of this XML script; it has been already saved by
    // wxWebUpdateLocalXMLScript::Load
    // (also we are probably parsing a temporary copy on the local PC here...)

    return TRUE;
}

