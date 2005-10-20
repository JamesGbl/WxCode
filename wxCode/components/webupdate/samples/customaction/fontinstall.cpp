/////////////////////////////////////////////////////////////////////////////
// Name:        fontinstall.cpp
// Purpose:     wxWebUpdateActionFontInstall - an example of a custom action
// Author:      Francesco Montorsi
// Created:     2005/08/26
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
#endif

#include "wx/fontinstall.h"


// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateActionFontInstall, wxWebUpdateAction)

// be sure the build system is defining the MODDED symbol; otherwise our
// wxInitializeMod() and wxUninitializeMod() functions will never be called !!
#ifndef

// if you get this error, then probably your "mod.bkl" file does not sets
// the MODDED variable to "1"... see webupdate.bkl for more info
#error The MODDED symbol should be defined ! Check the mod.bkl file.
#endif



// ----------------------------------
// GLOBAL MOD INIT & CLEAN FUNCTIONS
// ----------------------------------

bool wxInitializeMod()
{
    // called after that the wxWebUpdateInstaller singleton object has
    // been created.
    // You should add your custom webupdate actions to it here:
    wxWebUpdateInstaller::Get()->AddActionHandler(new wxWebUpdateFontInstall);
    
    // the action handlers will be automatically cleaned by wxWebUpdateInstaller
    // so that the "new" above won't create a memory leak
}

void wxUninitializeMod()
{
    // called before the wxWebUpdateInstaller singleton object is deleted.
    // you typically shouldn't do anything here...
}




// ------------------------------
// wxWEBUPDATEACTIONFONTINSTALL
// ------------------------------

bool wxWebUpdateActionFontInstall::Run() const
{
	// be sure that the font to install exists
	wxFileName f(m_strFile);
	if (!f.FileExists()) {
    
        // you should use two different types of wxLogXXX functions:
        // - wxLogUsrMsg for msg to be shown to the user & saved in the log file
        // - wxLogAdvMsg for msg to be saved in the log file only
		wxLogUsrMsg(wxT("wxWebUpdateActionFontInstall::Run - the font \"") + m_strFile +
				wxT("\" does not exist !"));
		return FALSE;
	}

#ifdef __WXMSW__

	// use Win32 APIs to install fonts
	AddFontResource(f.GetFullPath());
	
#else

	wxLogError(wxT("wxWebUpdateActionFontInstall is not implemented for this platform !"));

#endif

	return TRUE;
}

bool wxWebUpdateActionFontInstall::SetProperties(const wxArrayString &propnames,
												const wxArrayString &propvalues)
{
	wxString flags;

	m_strArgs = wxEmptyString;			// the ARGS default value
	for (int i=0; i < (int)propnames.GetCount(); i++) {
		wxLogAdvMsg(wxT("wxWebUpdateActionFontInstall::SetProperties - name: [")
				+ propnames[i] + wxT("], value: [") + propvalues[i] + wxT("]"));

		if (propnames[i] == wxT("file"))
			m_strFile = propvalues[i];
		else
			wxLogAdvMsg(wxT("wxWebUpdateActionFontInstall::SetProperties - unknown property: ") 
						+ propnames[i]);
	}

	// do substitutions on the paths
	m_strFile = wxWebUpdateInstaller::Get()->DoSubstitution(m_strFile);
	if (m_strFile.IsEmpty())
		m_strFile = wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("thisfile"));

	// validate the properties
	wxFileName f(m_strFile);			// the FILE property is required !

	// we won't do the wxFileName::FileExists check because the file we need to run
	// could be a file which does not exist yet (e.g. its in the update package)
	if (!f.IsOk()) 
		return FALSE;

	return TRUE;
}


