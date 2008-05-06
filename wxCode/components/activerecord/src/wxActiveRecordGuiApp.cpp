/////////////////////////////////////////////////////////////////////////////
// Name:        wxActiveRecordGuiApp.cpp
// Purpose:     
// Author:      Matías Szeftel
// Modified by: 
// Created:     Fri 09 Feb 2007 00:58:40 ART
// RCS-ID:      
// Copyright:   (c) 2006 Matías Szeftel <mszeftel@yahoo.com.ar>
// Licence:     GPL (GNU Public License)
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "wxActiveRecordGuiApp.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "../include/wxActiveRecordGuiApp.h"

////@begin XPM images

////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( WxActiveRecordGuiApp )
////@end implement app

/*!
 * WxActiveRecordGuiApp type definition
 */

IMPLEMENT_CLASS( WxActiveRecordGuiApp, wxApp )

/*!
 * WxActiveRecordGuiApp event table definition
 */

BEGIN_EVENT_TABLE( WxActiveRecordGuiApp, wxApp )

////@begin WxActiveRecordGuiApp event table entries
////@end WxActiveRecordGuiApp event table entries

END_EVENT_TABLE()

/*!
 * Constructor for WxActiveRecordGuiApp
 */

WxActiveRecordGuiApp::WxActiveRecordGuiApp()
{
    Init();
}

/*!
 * Member initialisation 
 */

void WxActiveRecordGuiApp::Init()
{
////@begin WxActiveRecordGuiApp member initialisation
////@end WxActiveRecordGuiApp member initialisation
}
/*!
 * Initialisation for WxActiveRecordGuiApp
 */

bool WxActiveRecordGuiApp::OnInit()
{    
	wxImage::AddHandler(new wxICOHandler);
////@begin WxActiveRecordGuiApp initialisation
    // Remove the comment markers above and below this block
    // to make permanent changes to the code.

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif
    wxActiveRecordGeneratorFrm* mainWindow = new wxActiveRecordGeneratorFrm( NULL, ID_FRAME );
    mainWindow->Show(true);
////@end WxActiveRecordGuiApp initialisation
	
	mainWindow->SetVersion(wxT("1.2.0-rc3"));
	mainWindow->SetTitle(mainWindow->GetTitle()+wxT(" ")+mainWindow->GetVersion());

	if(argc>1){
		if(!mainWindow->LoadOnInit(argv[1])){
			wxMessageBox(wxT("There was a problem loading ")+wxString(argv[1]),wxT("Xml Loading Error"));
		}
	}
    
	return true;
}

/*!
 * Cleanup for WxActiveRecordGuiApp
 */
int WxActiveRecordGuiApp::OnExit()
{    
////@begin WxActiveRecordGuiApp cleanup
    return wxApp::OnExit();
////@end WxActiveRecordGuiApp cleanup
}

