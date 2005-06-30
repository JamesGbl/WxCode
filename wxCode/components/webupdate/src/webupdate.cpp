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
#endif

#include "wx/url.h"
#include "wx/webupdate.h"
#include "wx/file.h"
#include "wx/wfstream.h"
#include "wx/filesys.h"


// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateXMLScript, wxXmlDocument)
IMPLEMENT_CLASS(wxWebUpdatePackage, wxObject)
IMPLEMENT_CLASS(wxWebUpdateDownload, wxObject)

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(wxWebUpdateDownloadArray);

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(wxWebUpdatePackageArray);


// global objects
wxWebUpdateDownload wxEmptyWebUpdateDownload(wxT("invalid"));




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

	m_size = (unsigned long)is->GetSize();
	delete is;

	return m_size;
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
	
	// parse this package
	wxXmlNode *child = package->GetChildren();
	while (child) {
		if (child->GetName() == wxT("latest-version")) {

			wxXmlNode *text = child->GetChildren();
			if (text) {
				ret->m_strLatestVersion = text->GetContent();
				ret->m_importance = wxWUPI_NORMAL;		// by default
			}

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

			// find the platform for which this download is
			wxXmlProperty *prop = child->GetProperties();
			while (prop && prop->GetName() != wxT("platform"))
				prop = prop->GetNext();

			// is this a well-formed tag ?
			if (prop->GetName() == wxT("platform")) {

				wxXmlNode *text = child->GetChildren();
				if (text) {
					wxWebUpdateDownload update(text->GetContent(), prop->GetValue());

					// last check 
					if (update.IsOk())
						ret->AddDownloadPackage(update);
				}
			}

		} else if (child->GetName() == wxT("msg-update-available")) {

			wxXmlNode *text = child->GetChildren();
			if (text) 
				ret->m_strUpdateAvailableMsg = text->GetContent();

		} else if (child->GetName() == wxT("msg-update-notavailable")) {

			wxXmlNode *text = child->GetChildren();
			if (text) 
				ret->m_strUpdateNotAvailableMsg = text->GetContent();
		}

		// proceed
		child = child->GetNext();
	}

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

	return success;
}

