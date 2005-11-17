/////////////////////////////////////////////////////////////////////////////
// Name:        httpengineapp.h
// Purpose:     wxHTTPEngine example
// Author:      Angelo Mandato
// Created:     2005/08/10
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Angelo Mandato (http://www.spaceblue.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "httpengineapp.h"
#endif

#ifndef HTTPEngineApp_H
#define HTTPEngineApp_H

// Define a new application type, each program should derive a class from wxApp
class wxHTTPEngineApp : public wxApp
{
public:
    wxHTTPEngineApp();
    virtual ~wxHTTPEngineApp();

    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
    virtual int OnExit();
};

DECLARE_APP(wxHTTPEngineApp)

#endif // HTTPEngineApp_H


