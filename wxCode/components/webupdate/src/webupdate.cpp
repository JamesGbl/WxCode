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


IMPLEMENT_CLASS(wxWebUpdateXMLScript, wxXmlDocument)

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(wxWebUpdateDownloadArray);


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




// ---------------------
// wxWEBUPDATEPACKAGE
// ---------------------

wxWebUpdateDownload wxWebUpdatePackage::GetDownloadPackage(wxWebUpdatePlatform code) const
{
	if (code == wxWUP_INVALID) code = wxWebUpdateDownload::GetThisPlatformCode();
	for (int i=0; i<(int)m_arrWebUpdates.GetCount(); i++)
		if (m_arrWebUpdates.Item(i).GetPlatform() == code)
			return m_arrWebUpdates.Item(i);
		return wxEmptyWebUpdateDownload;
}




// ----------------------
// wxWEBUPDATEXMLSCRIPT
// ----------------------

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

	if (!package) return NULL;	// could not found the required package


	// init the return value
	wxWebUpdatePackage *ret = new wxWebUpdatePackage(packagename);	

	// parse this package
	wxXmlNode *child = package->GetChildren();
	while (child) {
		if (child->GetName() == wxT("latest-version")) {

			wxXmlNode *text = child->GetChildren();
			if (text) 
				ret->m_strLatestVersion = text->GetContent();

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

wxWebUpdateCheckFlag wxWebUpdateXMLScript::Check(const wxString &version,
                                       const wxString &packagename) const
{


	return wxWUCF_UPDATED;
}

bool wxWebUpdateXMLScript::Load(const wxString &strURL)
{
	wxLogDebug(wxT("wxWebUpdateXMLScript::Load - loading ") + strURL);

    // refer to "webupdate.dtd" for a definition of the XML webupdate info script
    // first of all, we need to open a connection to the given url
    wxURL url(strURL);
    if (url.GetError() != wxURL_NOERR) {

        return FALSE;
    }
    
	wxInputStream *xml = url.GetInputStream();
	if (!xml) {
		
        return FALSE;
	}

	wxFile fileTest(wxT("test.txt"), wxFile::write);
	wxFileOutputStream sout(fileTest);
	if (!sout.Ok())
	{
		return FALSE;
	}
	
	xml->Read(sout);
	delete xml;
	sout.Close();
	
	wxFileInputStream sin(wxT("test.txt"));
	return wxXmlDocument::Load(sin);
}

