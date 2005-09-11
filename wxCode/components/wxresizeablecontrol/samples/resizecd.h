//-----------------------------------------------------------------------------
// Name:        resizecd.cpp
// Purpose:     resizeable controls sample: Main application file
// Author:      Markus Greither
// RCS-ID:      $Id: resizecd.h,v 1.3 2005-09-11 15:25:31 frm Exp $
// Copyright:   (c) Markus Greither
// Licence:     wxWindows licence
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Begin single inclusion of this .h file condition
//-----------------------------------------------------------------------------

#ifndef _RESIZEDEMO_H_
#define _RESIZEDEMO_H_

//-----------------------------------------------------------------------------
// GCC interface
//-----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma interface "resizecd.h"
#endif

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

#include "wx/app.h"             // wxApp

//-----------------------------------------------------------------------------
// Class definition: MyApp
//-----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
//! Application class for demo program
class MyApp : public wxApp
{

public:

    // Override base class virtuals:
    // wxApp::OnInit() is called on application startup and is a good place
    // for the app initialization (doing it here and not in the ctor
    // allows to have an error return: if OnInit() returns false, the
    // application terminates)
    //! Initialize the application and show main frame
    virtual bool OnInit();
	virtual int OnExit();
};

//-----------------------------------------------------------------------------
// End single inclusion of this .h file condition
//-----------------------------------------------------------------------------

#endif  //_RESIZEDEMO_H_
