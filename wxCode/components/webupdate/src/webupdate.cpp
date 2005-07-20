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
#include <wx/wfstream.h>
#include <wx/filesys.h>
#include "wx/webupdate.h"
#include "wx/download.h"


// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateXMLScript, wxXmlDocument)
IMPLEMENT_CLASS(wxWebUpdatePackage, wxObject)
IMPLEMENT_CLASS(wxWebUpdateDownload, wxObject)

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxWebUpdateDownloadArray);

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxWebUpdatePackageArray);


// global objects
wxWebUpdateDownload wxEmptyWebUpdateDownload(wxT("invalid"));
wxWebUpdatePackage wxEmptyWebUpdatePackage(wxT("invalid"));




// ---------------------
// wxWEBUPDATEDOWNLOAD
// ---------------------

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
	if (is == NULL || !is->IsOk() || 
		u.GetProtocol().GetError() != wxPROTO_NOERR) {
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
	// FIXME: how can we do it better ?
	return m_urlDownload.AfterLast('/');
}

bool wxWebUpdateDownload::DownloadSynch(const wxString &path, const wxString &proxy,
								const wxString &user, const wxString &password) 
{
	if (path.IsEmpty()) return FALSE;
	
	wxURL u(m_urlDownload);
	if (u.GetError() != wxURL_NOERR)
		return FALSE;
	
	// set advanced URL options
	if (!proxy.IsEmpty())
		u.SetProxy(proxy);
	u.GetProtocol().SetUser(user);
	u.GetProtocol().SetPassword(password);
	
	// now work on streams; wx docs says that using wxURL::GetInputStream
	// is deprecated but this is the only way to set advanced info like
	// proxy, user & password...
	wxFileOutputStream out(path);
	wxInputStream *in = u.GetInputStream();
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
									wxEvtHandler *phandler,	const wxString &proxy,
									const wxString &user, const wxString &password) 
{
	if (path.IsEmpty()) return NULL;
	wxDownloadThread *thread = new wxDownloadThread();

	// just set the download options
	thread->m_strHTTPAuthPassword = password;
	thread->m_strHTTPAuthUsername = user;
	thread->m_strOutput = path;
	thread->m_strProxyHostname = proxy.BeforeFirst(wxT(':'));
	thread->m_strProxyPort = proxy.AfterFirst(wxT(':'));
	thread->m_pHandler = phandler;

	// launch the download
	if (thread->Create() != wxTHREAD_NO_ERROR ||
		thread->Run() != wxTHREAD_NO_ERROR) {
		delete thread;
		return NULL;
	}

	return thread;
}

bool wxWebUpdateDownload::Install(wxWebUpdateInstaller *touse) const
{
	bool unrecognized = FALSE;
	if (!touse) touse = wxWebUpdateInstaller::Get();
	wxWebUpdateActionHashMap hashmap = touse->GetActionHashMap();

	// installation means: execute all <action> tags for this download...
	for (int i=0; i<(int)m_arrActions.GetCount(); i++) {
		wxString n = m_arrActions[i].GetName();
		if (hashmap.find(n) == hashmap.end()) {
			unrecognized = TRUE;
			wxLogDebug(wxT("wxWebUpdateDownload::Install - unregistered action ") + n);
			continue;	// skip this unknown action
		}

		if (!hashmap[n].Run())
			return FALSE;		// stop here
	}

	return !unrecognized;
}




// ---------------------
// wxWEBUPDATEPACKAGE
// ---------------------

wxWebUpdateDownload &wxWebUpdatePackage::GetDownloadPackage(wxWebUpdatePlatform code) const
{
	if (code == wxWUP_INVALID) code = wxWebUpdateDownload::GetThisPlatformCode();
	for (int i=0; i<(int)m_arrWebUpdates.GetCount(); i++)
		if (m_arrWebUpdates.Item(i).GetPlatform() == code)
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
	
	// catch invalid cases
	// i.e. when local version greater than latest available version
	if (maj < lmaj) return wxWUCF_FAILED;
	if (maj == lmaj && min < lmin) return wxWUCF_FAILED;
	if (maj == lmaj && min == lmin && rel < lrel) return wxWUCF_FAILED;

	// do the check
	if (maj > lmaj) return wxWUCF_OUTOFDATE;
	if (maj == lmaj && min > lmin) return wxWUCF_OUTOFDATE;
	if (maj == lmaj && min == lmin && rel > lrel) return wxWUCF_OUTOFDATE;
	if (maj == lmaj && min == lmin && rel == lrel) return wxWUCF_UPDATED;

	wxASSERT_MSG(0, wxT("All cases should have been handled... "));
	return wxWUCF_FAILED;
}

bool wxWebUpdatePackage::ExtractVersionNumbers(const wxString &str, int *maj, 
											   int *min, int *rel)
{
	unsigned long n;	

	// extract the single version numbers in string format
	wxString major = str.BeforeFirst(wxT('.'));
	wxString minor = str.AfterFirst(wxT('.')).BeforeFirst(wxT('.'));
	wxString release = str.AfterFirst(wxT('.')).AfterFirst(wxT('.'));

	if (major.IsEmpty() || minor.IsEmpty() || release.IsEmpty())
		return FALSE;

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

wxWebUpdateXMLScript::wxWebUpdateXMLScript(const wxString &strURI, 
										   wxWebUpdateInstaller *installer)
{ 
	// the installer is the first thing to set since #Load uses it !
	if (!installer) installer = wxWebUpdateInstaller::Get(); 
	m_pInstaller = installer;

	// now we can load
	if (!strURI.IsEmpty()) Load(strURI); 
}

// taken from wx/src/xrc/xmlres.cpp, wxXmlResourceHandler::GetNodeContent
wxString wxWebUpdateXMLScript::GetNodeContent(const wxXmlNode *node) const
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

wxString wxWebUpdateXMLScript::DoKeywordSubstitution(const wxString &str) const
{
	wxStringStringHashMap &list = m_pInstaller->GetKeywords();
	wxString text(str);

	// iterate over all the elements in the class
    wxStringStringHashMap::iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        wxString key = it->first, value = it->second;
		if (value.IsEmpty()) continue;		// skip empty values

		text.Replace(wxT("$(") + key + wxT(")"), value);
    }

#ifdef __WXDEBUG__
	if (text.Contains(wxT("$(")))
		wxLogDebug(wxT("wxWebUpdateXMLScript::DoKeywordSubstitution - ")
				wxT("found unknown keywords in the string:\n") + text);
#endif

	return text;
}

wxWebUpdateDownload wxWebUpdateXMLScript::GetDownload(const wxXmlNode *latestdownload) const
{
	if (latestdownload->GetName() != wxT("latest-download"))
		return wxEmptyWebUpdateDownload;

	// the info we need to build a wxWebUpdateDownload...
	wxString platform, md5, uri;
	wxWebUpdateActionArray actions;

	// find the platform for which this download is
	wxXmlNode *child = latestdownload->GetChildren();
	while (child) {
		
		// is this a well-formed tag ?
		if (child->GetName() == wxT("uri"))
			uri = DoKeywordSubstitution(GetNodeContent(child));
		else if (child->GetName() == wxT("md5"))
			md5 = GetNodeContent(child);
		else if (child->GetName() == wxT("platform")) {

			// search the "name" property
			wxXmlProperty *prop = child->GetProperties();
			while (prop) {
				if (prop->GetName() == wxT("name"))
					platform = prop->GetValue();
				prop = prop->GetNext();
			}

		} else if (child->GetName() == wxT("action")) {

			// convert to a wxArrayString the properties
			wxArrayString names, values;
			wxXmlProperty *prop = child->GetProperties();
			while (prop) {

				names.Add(prop->GetName());

				// the values can contain keywords to substitute
				values.Add(DoKeywordSubstitution(prop->GetValue()));
				prop = prop->GetNext();
			}

			// add a new action
			actions.Add(new wxWebUpdateAction(GetNodeContent(child), &names, &values));
		}


		// parse next child...
		child = child->GetNext();
	}

	// is this a valid download ?
	if (platform.IsEmpty() || actions.GetCount() <= 0)
		return wxEmptyWebUpdateDownload;

	return wxWebUpdateDownload(uri, platform, md5, &actions);	
}

wxWebUpdatePackage *wxWebUpdateXMLScript::GetPackage(const wxXmlNode *package) const
{
	if (!package || package->GetName() != wxT("package")) return NULL;

	wxString packagename;
	wxXmlProperty *prop = package->GetProperties();
	while (prop && prop->GetName() != wxT("id"))
		prop = prop->GetNext();
	if (prop) packagename = prop->GetValue();
	
	// init the return value
	wxWebUpdatePackage *ret = new wxWebUpdatePackage(packagename);	

	// add to the current keyword list the current ID
	wxStringStringHashMap &list = m_pInstaller->GetKeywords();
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
				ret->AddDownloadPackage(update);
			else
				wxLogDebug(wxT("wxWebUpdateXMLScript::GetPackage - skipping an invalid <latest-download> tag"));

		} else if (child->GetName() == wxT("description")) {
			ret->m_strDescription = DoKeywordSubstitution(GetNodeContent(child));
		}

		// proceed
		child = child->GetNext();
	}

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

	list[wxT("id")] = wxEmptyString;
	list[wxT("latest-version")] = wxEmptyString;
	return ret;		// the caller must delete it
}

wxWebUpdatePackage *wxWebUpdateXMLScript::GetPackage(const wxString &packagename) const
{
	// now it's time to parse the XML file we expect to be in 'xml' input stream    
    wxXmlNode *webupdate = GetRoot();
	if (!webupdate || webupdate->GetName() != wxT("webupdate"))
		return NULL;
	wxXmlNode *package = webupdate->GetChildren();
    
	bool matches = FALSE;
	while (package && !matches) {

		while (package && package->GetName() != wxT("package"))
			package = package->GetNext();
		if (!package) return NULL;		// could not found other PACKAGE tags in this script !

		wxXmlProperty *prop = package->GetProperties();
		while (prop && prop->GetName() != wxT("id"))
			prop = prop->GetNext();
		matches = prop->GetValue() == packagename;
	}

	if (!package) 
		return NULL;	// could not found the required package
	return GetPackage(package);
}

wxWebUpdatePackageArray wxWebUpdateXMLScript::GetAllPackages() const
{
	wxWebUpdatePackageArray ret;

	// now it's time to parse the XML file we expect to be in 'xml' input stream    
    wxXmlNode *webupdate = GetRoot();
	if (!webupdate || webupdate->GetName() != wxT("webupdate"))
		return ret;		// empty array = error
	wxXmlNode *package = webupdate->GetChildren();

	while (package) {
		wxWebUpdatePackage *toadd = GetPackage(package);
		package = package->GetNext();
		if (toadd == NULL) continue;		// skip this

		ret.Add(toadd);
	}

	return ret;
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
	bool success = wxXmlDocument::Load(*xml->GetStream());
	delete xml;

	// load the use custom messages (if present)
    wxXmlNode *webupdate = GetRoot();
	if (!success || !webupdate || webupdate->GetName() != wxT("webupdate"))
		return FALSE;

    wxXmlNode *child = webupdate->GetChildren();
	while (child) {

		if (child->GetName() == wxT("msg-update-available"))
			m_strUpdateAvailableMsg = DoKeywordSubstitution(GetNodeContent(child));
		else if (child->GetName() == wxT("msg-update-notavailable"))
			m_strUpdateNotAvailableMsg = DoKeywordSubstitution(GetNodeContent(child));

		child = child->GetNext();
	}

	return TRUE;
}

