/////////////////////////////////////////////////////////////////////////////
// Name:        webupdate.cpp
// Purpose:     wxWebUpdateXMLScript
// Author:      Francesco Montorsi
// Created:     2005/06/23
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
	#pragma implementation "webupdate.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
	#include <wx/log.h>
#endif

#include <wx/url.h>
#include <wx/file.h>
#include <wx/app.h>
#include <wx/wfstream.h>
#include <wx/filesys.h>
#include "wx/webupdate.h"
#include "wx/download.h"
#include "wx/installer.h"

#if wxUSE_HTTPENGINE
#include <wx/httpengine/httpbuilder.h>
#endif

// wxWidgets RTTI
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
wxWebUpdateDownload wxEmptyWebUpdateDownload(wxT("invalid"));
wxWebUpdatePackage wxEmptyWebUpdatePackage(wxT("invalid"));
wxWebUpdateLocalPackage wxEmptyWebUpdateLocalPackage(wxT("invalid"));
int wxWebUpdateAction::m_nExecResult = 0;



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
	if (!package || package->GetName() != wxT("local-package")) return NULL;
	wxString packagename = wxWebUpdateXMLScript::GetPackageID(package);

	// parse this package
	wxString ver;
	wxXmlNode *child = package->GetChildren();
	while (child) {

		if (child->GetName() == wxT("version")) {

			// get the version string
			ver = wxWebUpdateXMLScript::GetNodeContent(child);
		}

		// proceed
		child = child->GetNext();
	}
	
	if (ver.IsEmpty())
		return NULL;
	
	return new wxWebUpdateLocalPackage(packagename, ver);	// the caller must delete it
}

wxWebUpdateLocalPackage *wxWebUpdateLocalXMLScript::GetPackage(const wxString &packagename) const
{
	if (!IsOk())
		return NULL;
	wxXmlNode *package = GetRoot()->GetChildren();
    
	bool matches = FALSE;
	while (package && !matches) {

		while (package && package->GetName() != wxT("local-package"))
			package = package->GetNext();
		if (!package) return NULL;		// could not found other PACKAGE tags in this script !

		matches = (wxWebUpdateXMLScript::GetPackageID(package) == packagename);
	}

	if (!package) 
		return NULL;	// could not found the required package
	return GetPackage(package);
}

wxWebUpdateLocalPackageArray wxWebUpdateLocalXMLScript::GetAllPackages() const
{
	wxWebUpdateLocalPackageArray ret;

	// check root of the local script file
	if (!IsOk())
		return ret;		// empty array = error
	wxXmlNode *package = GetRoot()->GetChildren();

	while (package) {
	
		if (package->GetName() == wxT("local-package")) {
		
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
	wxLogDebug(wxT("wxWebUpdateXMLScript::Load - loading ") + uri);

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
	if (wxWebUpdateXMLScript::GetWebUpdateVersion(GetRoot(), 
				m_strWebUpdateVersion) == wxWUCF_FAILED)
		return FALSE;
		
    wxXmlNode *child = GetRoot()->GetChildren();
	while (child) {

		if (child->GetName() == wxT("appname")) {
		
			// save the name of the app using WebUpdate
			m_strAppName = wxWebUpdateXMLScript::GetNodeContent(child);
			wxWebUpdateInstaller::Get()->SetKeywordValue(wxT("appname"), m_strAppName);

		} else if (child->GetName() == wxT("dlgxrc")) {
		
			// save the name of the XRC file to use
			m_strXRC = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(
									wxWebUpdateXMLScript::GetNodeContent(child));
		
  		} else if (child->GetName() == wxT("appfile")) {
		
			// save the filename of the program to update
			m_strAppFile = wxWebUpdateInstaller::Get()->DoSubstitution(
									wxWebUpdateXMLScript::GetNodeContent(child));
  		
    	} else if (child->GetName() == wxT("remoteuri")) {
		
			// save the location of the remote script
			m_strRemoteURI = wxWebUpdateInstaller::Get()->DoSubstitution(
									wxWebUpdateXMLScript::GetNodeContent(child));

			// support for file: URIs
			if (wxIsFileProtocol(m_strRemoteURI)) {

				// is this a relative path ?
				wxFileName fn = wxGetFileNameFromURI(m_strRemoteURI);				
				if (fn.IsRelative()) {

					// <remoteuri> tag should specify relative paths
					// using the folder containing the local XML script
					// as the folder used to solve the relative path.
					wxFileName currdir(uri);
					wxASSERT_MSG(currdir.IsAbsolute(), wxT("Invalid local URI"));
					fn.MakeAbsolute(currdir.GetPath());
				}

				// replace our file URI with the wxFileName-filtered uri.
				m_strRemoteURI = wxMakeFileURI(fn);
			}

  		} else if (child->GetName() == wxT("keywords")) {
		
			// this should be a comma separed list of pairs:  key=value
			wxArrayString keys, values;
			int count = wxWebUpdateInstaller::Get()->ParsePairValueList(
					wxWebUpdateXMLScript::GetNodeContent(child), keys, values);

			for (int i=0; i < count; i++)
				wxWebUpdateInstaller::Get()->SetKeywordValue(keys[i], values[i]);
  		}
  		
		child = child->GetNext();
	}

	// is this local file valid ?
	if (m_strAppName.IsEmpty() || m_strXRC.IsEmpty() || m_strAppFile.IsEmpty() ||
		m_strRemoteURI.IsEmpty())
		return FALSE;

	// save the URI of this local XML script.
	m_strLocalURI = uri;
	wxWebUpdateInstaller::Get()->SetKeywordValue(wxT("localxml"), uri);

	// save the URI of the remote XML script.
	wxWebUpdateInstaller::Get()->SetKeywordValue(wxT("remotexml"), m_strRemoteURI);

	// save also the folders
	wxFileName f(wxGetFileNameFromURI(m_strRemoteURI));
	wxWebUpdateInstaller::Get()->SetKeywordValue(wxT("remotedir"), 
						f.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));
	wxFileName f2(wxGetFileNameFromURI(m_strLocalURI));
	wxWebUpdateInstaller::Get()->SetKeywordValue(wxT("localdir"), 
						f2.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));

	return TRUE;
}

// some simple helpers for wxWebUpdateLocalXMLScript::RebuildHeader
wxXmlNode *wxCreateElemNode(const wxString &name)
{ return new wxXmlNode(NULL, wxXML_ELEMENT_NODE, name, wxEmptyString, NULL, NULL); }

wxXmlNode *wxCreateElemTextNode(const wxString &name, const wxString &content = wxEmptyString){ 
	wxXmlNode *n = wxCreateElemNode(name); 
	n->AddChild(new wxXmlNode(NULL, wxXML_TEXT_NODE, wxEmptyString, content, NULL, NULL));
	return n;
}

wxXmlNode *wxWebUpdateLocalXMLScript::BuildHeader() const
{
	// the webupdate root
	wxXmlNode *webupdate = wxCreateElemNode(wxT("webupdate"));
	webupdate->AddProperty(new wxXmlProperty(wxT("version"),
								wxWebUpdateInstaller::Get()->GetVersion(), NULL));
	
	// set the appname
	wxXmlNode *appname = wxCreateElemTextNode(wxT("appname"), m_strAppName);
	webupdate->AddChild(appname);
	
	// set the appfile
	wxXmlNode *appfile = wxCreateElemTextNode(wxT("appfile"), m_strAppFile);
	appname->SetNext(appfile);
	
	// set the XRC name
	wxXmlNode *xrc = wxCreateElemTextNode(wxT("dlgxrc"), m_strXRC);
	appfile->SetNext(xrc);
	
	// set the remote URI
	wxXmlNode *uri = wxCreateElemTextNode(wxT("remoteuri"), m_strRemoteURI);
	xrc->SetNext(uri);

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
		wxXmlNode *lp = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("local-package"),
										wxEmptyString, new wxXmlProperty(wxT("id"), 
          								arr[i].GetName(), NULL), NULL);
 		
 		// create its VERSION subtag
 		lp->AddChild(wxCreateElemTextNode(wxT("version"), arr[i].GetVersion()));

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
	wxASSERT_MSG(!m_strLocalURI.IsEmpty(), wxT("Invalid filename !"));
	return wxXmlDocument::Save(m_strLocalURI);
}




// ---------------------
// wxWEBUPDATEACTION
// ---------------------

long wxWebUpdateAction::wxExecute(const wxString &command, int flags) const
{
	if (wxIsMainThread())
		return wxExecute(command, flags);		// no problems !

	// unfortunately we cannot use ::wxExecute from a secondary thread
	// (and wxWebUpdateActions run from a wxWebUpdateInstallThread) so we
	// are forced to send a message to wxApp which launches the command for us
	wxCommandEvent runev(wxEVT_COMMAND_EXECUTE);
	runev.SetString(command);
	runev.SetInt(flags);
	
	// our app should process this event...
	wxLogDebug(wxT("wxWebUpdateAction::wxExecute - sending to wxTheApp the command: ")
					+ runev.GetString());
	
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

wxWebUpdatePlatform wxWebUpdateDownload::GetThisPlatformCode()
{
	switch ( wxGetOsVersion() )
	{
	case wxMOTIF_X:
		return wxWUP_MOTIF;
		
	case wxMAC:
	case wxMAC_DARWIN:
		return wxWUP_MAC;
		
	case wxGTK:
	case wxGTK_WIN32:
	case wxGTK_OS2:
	case wxGTK_BEOS:
		return wxWUP_GTK;
		
	case wxWINDOWS:
	case wxPENWINDOWS:
	case wxWINDOWS_NT:
	case wxWIN32S:
	case wxWIN95:
	case wxWIN386:
		return wxWUP_MSW;
		
	case wxMGL_UNIX:
	case wxMGL_X:
	case wxMGL_WIN32:
	case wxMGL_OS2:
		return wxWUP_MGL;
		
	case wxWINDOWS_OS2:
	case wxOS2_PM:
		return wxWUP_OS2;
		
	default:
		return wxWUP_INVALID;    
	}
}

wxWebUpdatePlatform wxWebUpdateDownload::GetPlatformCode(const wxString &plat)
{
	wxWebUpdatePlatform ret = wxWUP_INVALID;
    if (plat == wxT("msw")) ret = wxWUP_MSW;
    if (plat == wxT("gtk")) ret = wxWUP_GTK;
    if (plat == wxT("os2")) ret = wxWUP_OS2;
	if (plat == wxT("mac")) ret = wxWUP_MAC;
	if (plat == wxT("motif")) ret = wxWUP_MOTIF;
	if (plat == wxT("x11")) ret = wxWUP_X11;
	if (plat == wxT("any")) ret = wxWUP_ANY;
	return ret;
}

wxString wxWebUpdateDownload::GetPlatformString(wxWebUpdatePlatform code)
{
	wxString ret;
    switch (code) {
	case wxWUP_MSW:
		ret = wxT("msw");
		break;
	case wxWUP_GTK:
		ret = wxT("gtk");
		break;
	case wxWUP_OS2:
		ret = wxT("os2");
		break;
	case wxWUP_MAC:
		ret = wxT("mac");
		break;
	case wxWUP_MOTIF:
		ret = wxT("motif");
		break;
	case wxWUP_X11:
		ret = wxT("x11");
		break;
	case wxWUP_ANY:
		ret = wxT("any");
		break;
	default:
		return wxEmptyString;
	}

	return ret;
}

unsigned long wxWebUpdateDownload::GetDownloadSize(bool forceRecalc)
{
	if (!forceRecalc && m_size != 1)
		return m_size;		// we have already calculated it...

	// we need to calculate it...
	wxURL u(m_urlDownload);
	if (u.GetError() != wxURL_NOERR) {
		m_size = 0;		// set it as "already calculated"
		return 0;
	}

	wxInputStream *is = u.GetInputStream();	
	if (is == NULL) {
		m_size = 0;		// set it as "already calculated"
		return 0;
	}
	if (!is->IsOk() || u.GetProtocol().GetError() != wxPROTO_NOERR) {
		delete is;		// be sure to avoid leaks
		m_size = 0;		// set it as "already calculated"
		return 0;
	}

#ifdef __WXDEBUG__
	wxProtocol &p = u.GetProtocol();
	wxHTTP *http = wxDynamicCast(&p, wxHTTP);
	if (http != NULL && http->GetResponse() == 302)
		wxLogDebug(wxT("wxWebUpdateDownload::GetDownloadSize - can't get the file size ")
			wxT("because the request of this download has been redirected... update your URL"));
#endif

	m_size = (unsigned long)is->GetSize();
	delete is;

	// see wxHTTP::GetInputStream docs
	if (m_size == 0xffffffff)
		m_size = 0;
	return m_size;
}

wxString wxWebUpdateDownload::GetFileName() const
{	
	return wxGetFileNameFromURI(m_urlDownload).GetFullName();
}

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
	
	wxLogDebug(wxT("wxWebUpdateDownload::DownloadSynch - completed download of %d bytes"),
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
}

bool wxWebUpdateDownload::Install() const
{
	wxWebUpdateInstaller *touse = wxWebUpdateInstaller::Get();
	wxWebUpdateActionHashMap hashmap = touse->GetActionHashMap();

	// installation means: execute all <action> tags for this download...
	for (int i=0; i<(int)m_arrActions.GetCount(); i++)
		if (!m_arrActions[i]->Run())
			return FALSE;		// stop here

	return TRUE;
}




// ---------------------
// wxWEBUPDATEPACKAGE
// ---------------------

wxWebUpdateDownload &wxWebUpdatePackage::GetDownload(wxWebUpdatePlatform code) const
{
	if (code == wxWUP_INVALID) 
		code = wxWebUpdateDownload::GetThisPlatformCode();

	for (int i=0; i<(int)m_arrWebUpdates.GetCount(); i++)
		if (m_arrWebUpdates.Item(i).GetPlatform() == code ||
				m_arrWebUpdates.Item(i).GetPlatform() == wxWUP_ANY)
			return m_arrWebUpdates.Item(i);
		
	// could not find a download for the given platform....
	return wxEmptyWebUpdateDownload;
}

wxWebUpdateCheckFlag wxWebUpdatePackage::Check(const wxString &localversion) const
{
	// get the version of the installed (local) program
	int lmaj, lmin, lrel;
	if (!ExtractVersionNumbers(localversion, &lmaj, &lmin, &lrel))
		return wxWUCF_FAILED;

	return Check(lmaj, lmin, lrel);
}

wxWebUpdateCheckFlag wxWebUpdatePackage::Check(int lmaj, int lmin, int lrel) const
{
	// get the version of the package hosted in the webserver
	int maj, min, rel;
	if (!ExtractVersionNumbers(m_strLatestVersion, &maj, &min, &rel))
		return wxWUCF_FAILED;

	int res = StdVersionCheck(maj, min, rel,		// remote package
							lmaj, lmin, lrel);		// local package

	// catch invalid cases
	// i.e. when local version is greater than latest available version	
	if (res == -1) return wxWUCF_FAILED;
	
	// catch other cases
	if (res == 0) return wxWUCF_UPDATED;
	if (res == 1) return wxWUCF_OUTOFDATE;

	wxASSERT_MSG(0, wxT("All cases should have been handled... "));
	return wxWUCF_FAILED;
}

// static
int wxWebUpdatePackage::StdVersionCheck(int maj1, int min1, int rel1,
										int maj2, int min2, int rel2)
{
	// package 1 < package 2 ?
	if (maj1 < maj2) return -1;
	if (maj1 == maj2 && min1 < min2) return -1;
	if (maj1 == maj2 && min1 == min2 && rel1 < rel2) return -1;

	// package 1 > package 2 ?
	if (maj1 > maj2) return 1;
	if (maj1 == maj2 && min1 > min2) return 1;
	if (maj1 == maj2 && min1 == min2 && rel1 > rel2) return 1;
	
	// package 1 == package 2 ?
	wxASSERT(maj1 == maj2 && min1 == min2 && rel1 == rel2);
 	return 0;
}

// static
bool wxWebUpdatePackage::ExtractVersionNumbers(const wxString &str, int *maj, 
											   int *min, int *rel)
{
	unsigned long n;	

	// extract the single version numbers in string format
	wxString major = str.BeforeFirst(wxT('.'));
	wxString minor = str.AfterFirst(wxT('.')).BeforeFirst(wxT('.'));
	wxString release = str.AfterFirst(wxT('.')).AfterFirst(wxT('.'));

	if (major.IsEmpty() || !major.IsNumber() || 
		(!minor.IsEmpty() && !minor.IsNumber()) ||
		(!release.IsEmpty() && !release.IsNumber()))
		return FALSE;			// invalid version format !
	if (minor.IsEmpty())
		minor = wxT("0");				// allow version formats of the type "1" = "1.0.0"
	if (release.IsEmpty())
		release = wxT("0");			// allow version formats of the type "1.2" = "1.2.0"

	// then convert them in numbers
	major.ToULong(&n);
	if (maj) *maj = (int)n;
	minor.ToULong(&n);
	if (min) *min = (int)n;
	release.ToULong(&n);
	if (rel) *rel = (int)n;

	return TRUE;
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

wxWebUpdateActionArray wxWebUpdateXMLScript::GetActionArray(const wxXmlNode *actions) const
{	
	wxWebUpdateActionArray ret;

	if (actions->GetName() != wxT("actions"))
		return ret;

	// parse all action tags
	wxXmlNode *child = actions->GetChildren();
	while (child) {

		wxString actname(child->GetName());
		
		// convert to a wxArrayString the properties of this node
		wxArrayString names, values;
		wxXmlProperty *prop = child->GetProperties();
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
			wxLogDebug(wxT("wxWebUpdateXMLScript::GetActionArray - unknown action: ") + actname);

		child = child->GetNext();
	}

	return ret;
}

wxWebUpdateDownload wxWebUpdateXMLScript::GetDownload(const wxXmlNode *latestdownload) const
{
	if (latestdownload->GetName() != wxT("latest-download"))
		return wxEmptyWebUpdateDownload;

	// the info we need to build a wxWebUpdateDownload...
	wxString platform, md5, uri;
	wxWebUpdateActionArray actions;
	wxStringStringHashMap &list = wxWebUpdateInstaller::Get()->GetKeywords();

	// find the platform for which this download is
	wxXmlNode *child = latestdownload->GetChildren();
	while (child) {
		
		// is this a well-formed tag ?
		if (child->GetName() == wxT("uri")) {

			// extract filename for this download
			uri = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(GetNodeContent(child));

			// the substitution process could have put some '\' (on win32) in the URI var... fix them
			uri.Replace(wxT("\\"), wxT("/"));

			// be sure that our "uri" var contains a valid URI...
			uri = wxURI(uri).BuildURI();

			// this keyword will be removed when exiting from this XML node
			list[wxT("thisfile")] = list[wxT("downloaddir")] +
				wxFileName::GetPathSeparator() + 
				wxGetFileNameFromURI(uri).GetFullName();

		} else if (child->GetName() == wxT("md5")) {

			// this is the precomputed MD5 for this file
			md5 = GetNodeContent(child);

		} else if (child->GetName() == wxT("platform")) {

			// search the "name" property
			wxXmlProperty *prop = child->GetProperties();
			while (prop) {
				if (prop->GetName() == wxT("name"))
					platform = prop->GetValue();
				prop = prop->GetNext();
			}

		} else if (child->GetName() == wxT("actions")) {

			// get the actions for this download
			actions = GetActionArray(child);
		}


		// parse next child...
		child = child->GetNext();
	}

	// remove the keywords we added while parsing
	list[wxT("thisfile")] = wxEmptyString;

	// is this a valid download ?
	if (platform.IsEmpty() || actions.GetCount() <= 0)
		return wxEmptyWebUpdateDownload;

	return wxWebUpdateDownload(uri, platform, md5, &actions);	
}

wxWebUpdatePackage *wxWebUpdateXMLScript::GetPackage(const wxXmlNode *package) const
{
	if (!package || package->GetName() != wxT("package")) return NULL;
	wxString packagename = GetPackageID(package);
	
	// init the return value
	wxWebUpdatePackage *ret = new wxWebUpdatePackage(packagename);	

	// add to the current keyword list the current ID
	wxStringStringHashMap &list = wxWebUpdateInstaller::Get()->GetKeywords();
	list[wxT("id")] = packagename;		// will be removed when exiting

	// parse this package
	wxXmlNode *child = package->GetChildren();
	while (child) {

		if (child->GetName() == wxT("requires")) {

			// read the list of required packages
			ret->m_strPrerequisites = GetNodeContent(child);

		} else if (child->GetName() == wxT("latest-version")) {

			// get the version string
			ret->m_strLatestVersion = GetNodeContent(child);
			ret->m_importance = wxWUPI_NORMAL;		// by default

			// add the "latest-version" keyword
			list[wxT("latest-version")] = ret->m_strLatestVersion;		// will be removed when exiting

			// and this version's importance (if available)
			wxXmlProperty *prop = child->GetProperties();
			while (prop && prop->GetName() != wxT("importance"))
				prop = prop->GetNext();
			if (prop) {
				wxString imp = prop->GetValue();
				if (imp == wxT("high")) ret->m_importance = wxWUPI_HIGH;
				if (imp == wxT("normal")) ret->m_importance = wxWUPI_NORMAL;
				if (imp == wxT("low")) ret->m_importance = wxWUPI_LOW;
			}

		} else if (child->GetName() == wxT("latest-download")) {

			// parse this download
			wxWebUpdateDownload update = GetDownload(child);
			if (update.IsOk())
				ret->AddDownload(update);
			else
				wxLogDebug(wxT("wxWebUpdateXMLScript::GetPackage - skipping an invalid <latest-download> tag"));

		} else if (child->GetName() == wxT("description")) {

			// do keywords substitution in the description
			ret->m_strDescription = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(GetNodeContent(child));
		}

		// proceed
		child = child->GetNext();
	}

	// remove the keywords we added while parsing
	list[wxT("id")] = wxEmptyString;
	list[wxT("latest-version")] = wxEmptyString;

	// be sure this package is valid
#ifdef __WXDEBUG__
	wxASSERT_MSG(ret->m_arrWebUpdates.GetCount() > 0, 
			wxT("No downloads defined for this package in the webupdate script ?"));
#else
	if (ret->m_arrWebUpdates.GetCount() <= 0) {
		delete ret;
		return NULL;
	}
#endif

	return ret;		// the caller must delete it
}

// static
wxString wxWebUpdateXMLScript::GetPackageID(const wxXmlNode *package)
{
	wxXmlProperty *prop = package->GetProperties();
	while (prop && prop->GetName() != wxT("id")) {
		wxLogDebug(wxT("wxWebUpdateXMLScript::GetPackageID - found unsupported ")
  				wxT("package property: ") + prop->GetName() + wxT("=") + prop->GetValue());
		prop = prop->GetNext();
	}
 
  	if (prop)
   		return prop->GetValue();		// found !
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

		while (package && package->GetName() != wxT("package"))
			package = package->GetNext();
		if (!package) return NULL;		// could not found other PACKAGE tags in this script !

		matches = (GetPackageID(package) == packagename);
	}

	if (!package) 
		return NULL;	// could not found the required package
	return GetPackage(package);
}

wxWebUpdatePackageArray wxWebUpdateXMLScript::GetAllPackages() const
{
	wxWebUpdatePackageArray ret;

	// now it's time to parse the XML file we expect to be in 'xml' input stream    
	if (!IsOk())
		return ret;		// empty array = error
	wxXmlNode *package = GetRoot()->GetChildren();

	while (package) {
		wxWebUpdatePackage *toadd = GetPackage(package);
		package = package->GetNext();
		if (toadd == NULL) continue;		// skip this

		ret.Add(toadd);
	}

	return ret;
}

// static
wxWebUpdateCheckFlag wxWebUpdateXMLScript::GetWebUpdateVersion(const wxXmlNode *package, wxVersion &ver)
{
	wxXmlProperty *prop = package->GetProperties();
	while (prop && prop->GetName() != wxT("version")) {
		wxLogDebug(wxT("wxWebUpdateXMLScript::GetWebUpdateVersion - found unsupported ")
  				wxT("webupdate property: ") + prop->GetName() + wxT("=") + prop->GetValue());
		prop = prop->GetNext();
    }
    
    if (prop == NULL) {
		wxLogDebug(wxT("wxWebUpdateXMLScript::GetWebUpdateVersion - could not find the ")
  				wxT("version property in the root tag. Defaulting to version ")
      			+ wxWebUpdateInstaller::Get()->GetVersion());
		ver = wxWebUpdateInstaller::Get()->GetVersion();
	
	} else {

		ver = prop->GetValue();
		wxLogDebug(wxT("wxWebUpdateXMLScript::GetWebUpdateVersion - the version ")
  				wxT("of this XML WebUpdate document is ") + ver);
	}
    
    // do the version check
	wxWebUpdateCheckFlag f = wxWebUpdateInstaller::Get()->VersionCheck(ver);
	if (f == wxWUCF_UPDATED)
		wxLogDebug(wxT("wxWebUpdateXMLScript::GetWebUpdateVersion - the version ")
  				wxT("of this XML WebUpdate document matches the installer engine version"));
	else if (f == wxWUCF_OUTOFDATE)
		wxLogDebug(wxT("wxWebUpdateXMLScript::GetWebUpdateVersion - the version ")
  				wxT("of this XML WebUpdate document is older than the installer engine... ")
      			wxT("continuing anyway"));
	else
		wxLogDebug(wxT("wxWebUpdateXMLScript::GetWebUpdateVersion - the version ")
  				wxT("of this XML WebUpdate document is newer than the installer engine... ")
      			wxT("aborting (cannot handle the new features!)"));
	return f;
}

bool wxWebUpdateXMLScript::Load(const wxString &uri)
{
	wxLogDebug(wxT("wxWebUpdateXMLScript::Load - loading ") + uri);

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
	while (child) {

		// parse the children of <webupdate> which are not packages
		if (child->GetName() == wxT("msg-update-available"))
			m_strUpdateAvailableMsg = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(GetNodeContent(child));
		else if (child->GetName() == wxT("msg-update-notavailable"))
			m_strUpdateNotAvailableMsg = wxWebUpdateInstaller::Get()->DoKeywordSubstitution(GetNodeContent(child));

		child = child->GetNext();
	}

	// don't save here the URI of this XML script; it has been already saved by
	// wxWebUpdateLocalXMLScript::Load
	// (also we are probably parsing a temporary copy on the local PC here...)

	return TRUE;
}

