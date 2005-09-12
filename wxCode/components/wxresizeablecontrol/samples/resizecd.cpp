//-----------------------------------------------------------------------------
// Name:        resizecdd.cpp
// Purpose:     resizeable controls sample: Main application file
// Author:      Markus Greither
// RCS-ID:      $Id: resizecd.cpp,v 1.4 2005-09-12 09:40:17 frm Exp $
// Copyright:   (c) Markus Greither
// Licence:     wxWindows licence
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GCC implementation
//-----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "resizecd.h"
#endif

//-----------------------------------------------------------------------------
// Standard wxWindows headers
//-----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// For all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//-----------------------------------------------------------------------------
// Header of this .cpp file
//-----------------------------------------------------------------------------

#include "resizecd.h"

//-----------------------------------------------------------------------------
// Remaining headers: Needed wx headers, then wx/contrib headers, then application headers
//-----------------------------------------------------------------------------

#include "wx/image.h"               // wxImage
#include "wx/textbox.h"

//-----------------------------------------------------------------------------

#include "myframe.h"

//-----------------------------------------------------------------------------
// wxWindows macro: Declare the application.
//-----------------------------------------------------------------------------

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. the_app and
// not wxApp).
IMPLEMENT_APP(MyApp)

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // This example uses GIFs, but if
    // you want PNGs, then add a PNG handler, etc. See wxImage::AddHandler()
    // documentation for the types of image handlers available.
#ifdef __WXGTK__
    wxImage::AddHandler(new wxPNGHandler); // required for GTK clipboard
#endif
    wxImage::AddHandler(new wxGIFHandler);

	wxTextSpan::Init();
	wxLog::SetVerbose();

    // Make an instance of your derived frame. Passing NULL (the default value
    // of MyFrame's constructor is NULL) as the frame doesn't have a frame
    // since it is the first window.
    MyFrame *frame = new MyFrame();

    // Show the frame.
    frame->Show(TRUE);
    
    // Return TRUE to tell program to continue (FALSE would terminate).
    return TRUE;
}

int MyApp::OnExit()
{
	wxTextSpan::Cleanup();
#ifdef __WXDEBUG__
	wxLogDebug(wxT("The maximum number of used spans is %d"), wxTextSpanArray::s_nMaxUsed);
#endif

	return wxApp::OnExit();
}

