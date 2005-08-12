/////////////////////////////////////////////////////////////////////////////
// Name:        httpenginedlg.cpp
// Purpose:     HTTPE Engine Dialog
// Author:      Angelo Mandato
// Created:     2005/08/11
// RCS-ID:      $Id$
// Copyright:   (c) 2002-2005 Angelo Mandato
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "httpengineapp.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
	// here goes the #include <wx/abc.h> directives for those
	// files which are not included by wxprec.h
	#include <wx/wx.h>
#endif

#include "httpengineapp.h"
#include "httpenginedlg.h"



// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. wxHTTPEngineApp and
// not wxApp)
IMPLEMENT_APP(wxHTTPEngineApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

wxHTTPEngineApp::wxHTTPEngineApp()
{
}

wxHTTPEngineApp::~wxHTTPEngineApp()
{
}

// wxT('Main program') equivalent: the program execution "starts" here
bool wxHTTPEngineApp::OnInit()
{

    // create the main application window
    wxHTTPEngineDialog *dialog = new wxHTTPEngineDialog(_T("wxHTTP Engine"),
                                 wxPoint(50, 50), wxSize(400, 350));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    dialog->Show(TRUE);
	SetTopWindow(dialog);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}

// eof
